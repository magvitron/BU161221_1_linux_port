#include "borland_compat.h"
#include "uses.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include "math.h"
#include "common.h"

#include "ct.h"

#include "utils.h"
#include "funproto.h"

//Temporarily added here by:sdt:26072005:1020
int GetPrivateProfileString(char *szAppName,char *szKeyName,char *szResult,
							char *szDefault, char *szFileName)
{
	char 	szTmp[80];
	int  	appFound = FALSE;
	int 	keyFound = FALSE;

	ifstream ins( szFileName, ios::in );
	if (!ins.is_open())
	    return 0;
	if ( !ins )
		return FALSE;

	while ( !ins.eof() )
	{
		ins.getline( szTmp, 80, '\n' );
		if  ( !appFound )
		{
			if ( strstr( szTmp, szAppName ) )
				appFound = TRUE;
		}
		else
		{
			if ( strstr( szTmp, szKeyName ) )
			{
				keyFound = TRUE;
				break;
			}
			else
				if ( strstr( szTmp, "[" ) )
					break;
		}
	}

	ins.close();
	if ( (appFound == TRUE) && (keyFound == TRUE) )
	{
		int i = 0;
		char *temp = szTmp;
		while( isspace( szTmp[i] ) && szTmp[i] != '\0' )
			temp++;
		if ( temp[0] == ';' )
			return FALSE;
		while( *temp != '=' && *temp!='\0' ) temp++;
		if ( *temp == '=' )
		{
			temp++;
			while( isspace( *temp ) && *temp != '\0' )
				temp++;
			if ( *temp == '\"' )
			{
				temp++;
				for( i=0;temp[i] != '\"' && temp[i] != '\0' && i < 80;i++ )
					szResult[i] = temp[i];
				szResult[i] = '\0';
			}
			else
			{
				if ( !strlen( temp ) )
					strcpy( szResult, szDefault );
				else
					strcpy( szResult, temp );
			}
		   return TRUE;
		}
	}
	strcpy( szResult, szDefault );
	return FALSE;
}

// Temporarily added here by:sdt:26062005:1500
// function returns the integer value from the specified file,
// under given application key and against the given key name
//
int GetPrivateProfileInt(char *szAppName,char *szKeyName, int nDefault,
						 char *szFileName)
{
	char 	szDef[80];
	char 	szRes[80];

//	strset( szRes, '\0' );
	memset(szRes, 0, sizeof(szRes));
	sprintf( szDef, "%d", nDefault );
	GetPrivateProfileString( szAppName, szKeyName, szRes, szDef, szFileName);
	return atoi( szRes );
}

int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szStr,
									  char *szDefault,char *FileName,int nDefault)
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


// setBlink - This function will set the blinking on if it is reseted by
//			any another program
void setBlink()
{
	union REGS a;

	a.h.ah = 0x10;
	a.h.al = 0x03;
	a.h.bl = 1;

	int86( 0x10, &a, &a );
}

void ftos( double f, char *temp, int width, int dec, char round )
{
	char str[20];
	int i = 0;

	sprintf( str, "%f", f );

	while(str[i] != '.'&&str[i] != '\0'&&i < width) temp[i] = str[i++];

	if ( dec != -1 )
		if ( i+dec < width )
			width = i+dec;

	if ( i < width )
	{
		temp[i] = '.';
		while( str[i] != '\0' && i <= width )	temp[i] = str[i++];
	}
	if ( str[i] != '\0' && round )
		if ( str[i] > 5+'0' )
			temp[i-1] += 1;

	temp[i] = '\0';
	return;
}


int CharOccurInString(char *String,char ch)
{
	int nCharCount=0;
	if(String[0]=='\0')
		return -1;
	for(int i=0;String[i]!='\0';i++)
	{
		if(String[i]==ch)
			nCharCount++;
	}
	return nCharCount;
}

char *NewGetPrivateProfileString (char szAppName[10][30],char *szkeyname,char *szDefault
									  ,char *szStr,char *FileName,int nDefault)
{
	FILE  *pFile=NULL;
	char	cBuf[150], *szTemp;
	int	nLen, i,flagTokenFound=1;
    int *TokenFound,KeyFound=0;
	char tBuf[150];
    char RetStr[150];//modified by:sdt:24042001
    int nNumberofAppNames=0;
    while(strcmp(szAppName[nNumberofAppNames],"")!=0)
    {
    	nNumberofAppNames++;
    }

    if((pFile=fopen(FileName,"rt"))==NULL)
		return 0;
    TokenFound = (int *)malloc(sizeof(int)*nNumberofAppNames);
    memset(TokenFound,0,sizeof(int)*nNumberofAppNames);
    for(i=0;i<nNumberofAppNames;i++)
    {
		strcpy(tBuf,szAppName[i]);
		strcat(tBuf,"_END");

		while(fgets(cBuf,150,pFile)!=NULL)
		{
			szTemp=strstr(cBuf,szAppName[i]);
			if(szTemp!=NULL)
			{
				TokenFound[i]=1;
				break;
			}
			if ( strstr(cBuf,tBuf) )
				break;
		}
    }
    for(i=0;i<nNumberofAppNames;i++)
    {
    	flagTokenFound &= TokenFound[i];
    }
	if ( !flagTokenFound )
		;
	else
	{
		while(fgets(cBuf,150,pFile)!=NULL)
		{
			if ( strstr(cBuf,szkeyname) )
			{
				szTemp=strstr(cBuf,szkeyname);
				KeyFound = 1;
				break;
			}
			if ( strstr(cBuf,tBuf) )
				break;
		}

		if(KeyFound)
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
				if(nLen > 1)
				{
					if(pFile){fclose(pFile);}
                    szkeyname=NULL;
                    szDefault=NULL;

					for(i=0,nLen=strlen(szTemp);nLen&&(*szTemp!='\n')&&(*szTemp!='\0')&&(*szTemp!=EOF);szTemp++,i++)
						RetStr[i] = *szTemp;

                    RetStr[i] = '\0';
                    szStr = RetStr;
                    free(TokenFound);

					return szStr;
				}
			}
		}
	}
	if(pFile){fclose(pFile);}
	for ( i = 0; szDefault[i] != '\x0' && szDefault != NULL; i++ )
		RetStr[i] = szDefault[i];
	RetStr[i] = '\0';
    szStr = RetStr;
    free(TokenFound);
	return szStr;
}	// End of NewGetPrivateProfileString

FILE *ReturnFilePtr(char szAppName[10][30],char *szkeyname,char *FileName)
{
	FILE  *pCurrFile;
	char	cBuf[150], *szTemp;
	int	 i,flagTokenFound=1;
    int *TokenFound,KeyFound=0;
	char tBuf[150];
    long lFilePrevPos;
    int nNumberofAppNames=0;
	//char cCh; //Commented by:sdt:28082005:1145
    while(strcmp(szAppName[nNumberofAppNames],"")!=0)
    {
    	nNumberofAppNames++;
    }

    if((pCurrFile=fopen(FileName,"rt"))==NULL)
		return 0;
    TokenFound = (int *)malloc(sizeof(int)*nNumberofAppNames);
    memset(TokenFound,0,sizeof(int)*nNumberofAppNames);
    for(i=0;i<nNumberofAppNames;i++)
    {
		strcpy(tBuf,szAppName[i]);
		strcat(tBuf,"_END");
		while(fgets(cBuf,150,pCurrFile)!=NULL)
		{
			szTemp=strstr(cBuf,szAppName[i]);
			if(szTemp!=NULL)
			{
				TokenFound[i]=1;
				break;
			}
			if ( strstr(cBuf,tBuf) )
				break;
		}
    }
    for(i=0;i<nNumberofAppNames;i++)
    {
    	flagTokenFound &= TokenFound[i];
    }
	if ( !flagTokenFound )
		;
	else
	{
		lFilePrevPos=ftell(pCurrFile);
		while(fgets(cBuf,150,pCurrFile)!=NULL)
		{
			if ( strstr(cBuf,szkeyname) )
			{
				szTemp=strstr(cBuf,szkeyname);
				KeyFound = 1;
				break;
			}
			if ( strstr(cBuf,tBuf) )
				break;
            lFilePrevPos=ftell(pCurrFile);
		}

		if(KeyFound)
		{
            fseek(pCurrFile,lFilePrevPos,SEEK_SET);
			//while((cCh=fgetc(pCurrFile))!='=');
			//Modified by:sdt:28082005:1145
			while((fgetc(pCurrFile))!='=');
            	return pCurrFile;
        }
	}
    if(pCurrFile){fclose(pCurrFile);}
    return NULL;
}	// End of ReturnFilePtr


//Added by:sdt:07052001
void SortArray(float *Array,int Arraysize)
{
    int temp;
    for(int i=0;i<Arraysize;i++)
    {
    	for(int j=i+1;j<Arraysize;j++)
        {
        	if(Array[i]>Array[j])
            {
            	temp=Array[i];
                Array[i]=Array[j];
                Array[j]=temp;
            }
        }
    }
}

void InsertNumber(float Val,float *Array,int Arraysize)
{
    int nInsertIndex;
	if(Val>=Array[Arraysize-1])
    	return;
    else
    if(Val<=Array[0])
    	nInsertIndex =0;
    else
    nInsertIndex = Search(ceil(float(Arraysize)/2),Array,Val,ceil(float(Arraysize)/2));

    for(int i=Arraysize-1;i>nInsertIndex;i--)
    {
    	Array[i] = Array[i-1];
    }
    Array[nInsertIndex]=Val;
}

int Search(int ArrayIndex,float *Array,float Val,int Arraysize)
{
   int Index;
   //window(10, 24, 50, 25);
   //textcolor(BLACK);
   //textbackground(WHITE);
   //cprintf("Arrayindex=%d ",ArrayIndex);
   //getch();
   if(ArrayIndex>0) //Added by:sdt:13072009//Modification for Assertion failed: Heap Check Error
   {
		if(Val>Array[ArrayIndex-1]&&Val<=Array[ArrayIndex])
		return ArrayIndex;
   }
   if(Val>Array[ArrayIndex])
   {
		//Index = Search(ArrayIndex+ceil(float(Arraysize)/2),Array,Val,ceil(float(Arraysize)/2));
		//Added by:sdt:13072009//Modification for Assertion failed: Heap Check Error
		Index = Search(ArrayIndex+((floor(float(Arraysize)/2))==0?1:(floor(float(Arraysize)/2))),Array,Val,((floor(float(Arraysize)/2))==0?1:(floor(float(Arraysize)/2))));
   }
   else
   {
		Index = Search(ArrayIndex-ceil(float(Arraysize)/2),Array,Val,ceil(float(Arraysize)/2));
   }
   return Index;
}
