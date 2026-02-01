#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dir.h>

typedef unsigned char  BYTE;
typedef unsigned int   WORD;

#define MAXBUFFERSIZE	3000

static char szSignature[7] = {"LOCKED"};
//static char szPassword[7] = {"FX#2K"};
//Modified:jj:07092001
static char szPassword[7] = {"E/D#2K"};

void GetSeed(char *pszPassword, BYTE *pucSeed1, BYTE *pucSeed2)
{
	int     i, j;

    *pucSeed1 = 0;
	*pucSeed2 = 0;

	j = strlen(pszPassword);

	for(i = 1; i <= strlen(pszPassword); i++, j--)
	{
	   *pucSeed1 += (BYTE)(pszPassword[i-1]*i);
	   *pucSeed2 += (BYTE)(pszPassword[i-1]*j);
	}
}

void DecodeFile(FILE *fpSrc, FILE *fpDest, BYTE cbSeed1, BYTE cbSeed2)
{
	BYTE    ucSeed1 = cbSeed1, ucSeed2 = cbSeed2;
	int     nBlock = 0, i;
	long	lBytesRead = 0L;
	BYTE	cbBuf[MAXBUFFERSIZE];

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
	BYTE    cbSeed1, cbSeed2;
	BYTE    cbSeed1x, cbSeed2x;
	char	cBuf[7];
	char	*pszPassword;

	if(argc < 3)
	   return;

	if(!(fpSrc = fopen(argv1, "rb")))
		return;

	fread(cBuf, 1, 6, fpSrc);
	cBuf[6] = 0;
	if(strcmpi(cBuf, szSignature) != 0)
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
		pszPassword = szPassword;

	GetSeed(pszPassword, &cbSeed1, &cbSeed2);

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
