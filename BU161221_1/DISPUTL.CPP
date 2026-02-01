#include <cstdio>
#include <cstring>
#include <cstdlib>

// likely where CENTER/LEFT/RIGHT live in your codebase
#include "uses.h"
void WriteStringToFile (FILE *fp,char *str, int endcount)
{
    str[endcount]='\0';
    int i=0;
   //fprintf (fp,"%s",str);
 	while(str[i]!='\0')//Added:jj:06052001
     	fputc (str[i++],fp);

}

//06052001
void PrepareString (float Start,float End,char *src,char *dest,int cpi,int nAllignment,int &endcount)
{
	int i,j,nStartCol,nEndCol;

	nStartCol = (int)(Start * cpi) ,nEndCol = (int)(End * cpi);

	switch (nAllignment)
	{
		case CENTER :
			 nStartCol += (int)((nEndCol - nStartCol) / 2) - (int)(strlen (src) /2);
			 break;
		case RIGHT :
			 nStartCol += (int)(nEndCol - nStartCol) - (int)(strlen (src));
			 break;
		case LEFT :
			 nStartCol = nStartCol;
			 break;
	}
	for (i = nStartCol,j = 0;i < nEndCol && src[j] != '\0';i ++,j ++)
		dest[i] = src[j];

	endcount=i;//06052001
}

void MemSet (int *nArray,int nSize,int nValue)
{
    int i;
    for (i = 0;i < nSize;i ++)
        nArray [i] = nValue;
}

void ClearBuffer (char *cBuf,int nlimit)
{
	int i;
	for (i = 0;i < nlimit-1;i ++)
		cBuf[i] = ' ';
	cBuf[i] = '\0';
}

int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szDefault,
									  char *szStr,char *FileName,int nDefault)
{
	FILE  *pFile;
	char	cBuf[250], *szTemp;
	int	nLen, i,TokenFound=0;

	if((pFile=fopen(FileName,"rt"))==NULL)
		return FALSE;
	while(fgets(cBuf,250,pFile)!=NULL)
	{
		szTemp=strstr(cBuf,szAppName);
		if(szTemp!=NULL)
			TokenFound=1;
		szTemp=strstr(cBuf,szkeyname);
		if(szTemp!=NULL&&TokenFound)
		{
			szTemp+=strlen(szkeyname);
			nLen=strlen(szTemp);
			i=0;
			while((nLen)&&(*szTemp!='='))
			{
				nLen--;
				szTemp++;
			}
			if(nLen)
			{
				szTemp++;
				nLen=strlen( szTemp);
				i=0;
				if(nDefault)
				{
					 while((nLen)&&(*szTemp==' '))
					 {
						 nLen--;
						 szTemp++;
					 }
				}
				if(nLen)
				{
					if(pFile){fclose(pFile);}
					for(i=0,nLen=strlen(szTemp);nLen&&(*szTemp!='\n')&&(*szTemp!=0)&&(*szTemp!=EOF);szTemp++,i++)
						szStr[i] = *szTemp;
					szStr[i] = 0;
					return i;
				}
			}
		}
	}
	if(pFile){fclose(pFile);}
	return sprintf(szStr,szDefault);
}	// End of GetPrivateProfileString

int GetPrivateProfileInt (char *szAppName,char *szkeyname,int nDefault,char *FileName)
{
	char	szDefault[20], szStr[20];

	sprintf(szDefault,"%d",nDefault);
	GetPrivateProfileString(szAppName,szkeyname,szDefault,szStr,FileName,0);
	return(atoi( szStr));
}	// End of GetPrivateProfileInt

void RepeatChar (char ch,int times,char *s)
{
    int i;
    for (i = 0;i < times-1; i++)
        *(s+i) = ch;
    *(s+i) = '\0';
}
