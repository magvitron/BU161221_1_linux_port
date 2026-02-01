#include <conio.h>
#include <string.h>
#include <dir.h>
#include "uses.h"   // where strcmpi is defined as strcasecmp(...)

#define MAXBUFFERSIZE	3000

#define ENC_SIGNATURE "LOCKED"
#define PASSWORD "FX#2K"

#include "prodini.h"

void eGetSeed(char *pszPassword, unsigned char *pucSeed1, unsigned char *pucSeed2)
{
	int     i, j;

	*pucSeed1 = 0;
	*pucSeed2 = 0;

	j = strlen(pszPassword);

	for(i = 1; i <= strlen(pszPassword); i++, j--)
	{
		*pucSeed1 += (unsigned char)(pszPassword[i-1]*i);
		*pucSeed2 += (unsigned char)(pszPassword[i-1]*j);
	}
}

void EncodeFile(FILE *fpSrc, FILE *fpDest, unsigned char cbSeed1, unsigned char cbSeed2)
{
	unsigned    ucSeed1 = cbSeed1, ucSeed2 = cbSeed2;
	int     nBlock = 0, i;
	long	lBytesRead = 0L;
	unsigned char	cbBuf[MAXBUFFERSIZE];

	nBlock = fread(cbBuf, 1, MAXBUFFERSIZE, fpSrc);

	lBytesRead += nBlock;
	while(nBlock > 0)
	{
		for(i = 1; i <= nBlock; i++)
		{
			ucSeed1 -= i;
			ucSeed2 += i;
			if(i%2)
				cbBuf[i-1] -= ucSeed1;
			else
				cbBuf[i-1] += ucSeed2;
		}
		fwrite(cbBuf, 1, nBlock, fpDest);
		nBlock = fread(cbBuf, 1, MAXBUFFERSIZE, fpSrc);
		lBytesRead += nBlock;
	}
}

void enc(int argc, char *argv1, char *argv2, char *argv3 )
{
	FILE	*fpSrc, *fpDest;
	unsigned char    cbSeed1, cbSeed2;
	char	cBuf[7];
	char	*pszPassword;

/*	if ( getdisk () != 0 && getdisk () != 1)
		return;*/
	if(argc < 3)
		return;

	if(!(fpSrc = fopen(argv1, "rb")))
		return;


	fread(cBuf, 6, 1, fpSrc);
	cBuf[6] = 0;
	if(strcmpi(cBuf, ENC_SIGNATURE) == 0)
	{
		if(fpSrc){fclose(fpSrc);}
		return;
	}

	if(!(fpDest = fopen(argv2, "wb")))
	{
		if(fpSrc){fclose(fpSrc);}
		return;
	}

	fseek(fpSrc, 0L, SEEK_SET);

	if( (argc == 4) && (strcmp(argv3, "-p") == 0))
		pszPassword = getpass("Enter Password : ");
	else
		pszPassword = PASSWORD;
	eGetSeed(pszPassword, &cbSeed1, &cbSeed2);

	fwrite(ENC_SIGNATURE, 1, 6, fpDest);
	fwrite(&cbSeed1, 1, 1, fpDest);
	fwrite(&cbSeed2, 1, 1, fpDest);

	EncodeFile(fpSrc, fpDest, cbSeed1, cbSeed2);

	if(fpSrc){fclose(fpSrc);}
	if(fpDest){fclose(fpDest);}
}


// Decription Functions

void dGetSeed(char *pszPassword, unsigned char *pucSeed1, unsigned char *pucSeed2)
{
	int     i, j;

    *pucSeed1 = 0;
	*pucSeed2 = 0;

	j = strlen(pszPassword);

	for(i = 1; i <= strlen(pszPassword); i++, j--)
	{
	   *pucSeed1 += (unsigned char)(pszPassword[i-1]*i);
	   *pucSeed2 += (unsigned char)(pszPassword[i-1]*j);
	}
}

void DecodeFile(FILE *fpSrc, FILE *fpDest, unsigned char cbSeed1, unsigned char cbSeed2)
{
	unsigned char	ucSeed1 = cbSeed1, ucSeed2 = cbSeed2;
	int     nBlock = 0, i;
	long	lBytesRead = 0L;
	unsigned char	cbBuf[MAXBUFFERSIZE];

	nBlock = fread(cbBuf, 1, MAXBUFFERSIZE, fpSrc);

	lBytesRead += nBlock;
	while(nBlock > 0)
	{
		for(i = 1; i <= nBlock; i++)
		{
			ucSeed1 -= i;
			ucSeed2 += i;
			if(i%2)
				cbBuf[i-1] += ucSeed1;
			else
				cbBuf[i-1] -= ucSeed2;
		}
		fwrite(cbBuf, 1, nBlock, fpDest);
		nBlock = fread(cbBuf, 1, MAXBUFFERSIZE, fpSrc);
		lBytesRead += nBlock;
	}
}

void dec(int argc, char *argv1,char *argv2,char *argv3)
{
	FILE	*fpSrc, *fpDest;
	unsigned char	cbSeed1, cbSeed2;
	unsigned char	cbSeed1x, cbSeed2x;
	char	cBuf[7];
	char	*pszPassword;

	if(argc < 3)
	   return;

	if(!(fpSrc = fopen(argv1, "rb")))
		return;

	fread(cBuf, 1, 6, fpSrc);
	cBuf[6] = 0;
	if(strcmpi(cBuf, ENC_SIGNATURE) != 0)
	{
		if(fpSrc){fclose(fpSrc);}
		return;
	}

	if(!(fpDest = fopen(argv2, "wb")))
	{
		if(fpSrc){fclose(fpSrc);}
		return;
	}

	if( (argc == 4) && (strcmp(argv3, "-p") == 0))
		pszPassword = getpass("Enter Password : ");
	else
		pszPassword = PASSWORD;

	dGetSeed(pszPassword, &cbSeed1, &cbSeed2);

	fread(&cbSeed1x, 1, 1, fpSrc);
	fread(&cbSeed2x, 1, 1, fpSrc);

	if((cbSeed1 != cbSeed1x) || (cbSeed2 != cbSeed2x))
	{
		if(fpSrc){fclose(fpSrc);}
		if(fpDest){fclose(fpDest);}
		return;
	}

	DecodeFile(fpSrc, fpDest, cbSeed1, cbSeed2);

	if(fpSrc){fclose(fpSrc);}
	if(fpDest){fclose(fpDest);}
}


