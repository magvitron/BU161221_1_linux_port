#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include "common.h"
#include "utils.cpp"


typedef struct CableType
{
	char szCableType[5];
}CABLETYPE;

CABLETYPE *CblTypes;
float *fCblGuage;
char CableTypeSelcted[10];

void ReadCableType();//CABLETYPE *CblTypes );
void ReadCableGuage();//float *fCblGuage);
void FindIndexofCableType();
float ReadCM80HzLenMFs( unsigned int unCableLen );

int main( int argc, char *argv[])
{

   /*ReadCableType();//CblTypes);
   ReadCableGuage();//fCblGuage);
   clrscr();
   int i=0;
   while((strcmp(CblTypes[i].szCableType,"-1")!=0) && (strcmp(CblTypes[i].szCableType,"")!=0))
   {
	  printf("\nCableType%d - %s",i+1,CblTypes[i].szCableType);
	  i++;
   }
   i=0;
   while(fCblGuage[i]!=0 && fCblGuage[i]!=-1)
   {
	  printf("\nCableGuage%d - %1.2f",i+1,fCblGuage[i]);
	  i++;
   }
   printf("\n\nPlease select the Cable Type : ");
   scanf("%s",CableTypeSelcted);
   FindIndexofCableType();*/
   unsigned int unCableLen;
   float fLenMF=1.0;
   printf("\n\nPlease enter cable guage : ");
   scanf("%u",&unCableLen);
   fLenMF = ReadCM80HzLenMFs(unCableLen);
   printf("\n Len=%u MF=%f",unCableLen,fLenMF);
   getch();

   //spawnlp(P_WAIT,"view.com","view.com","temp1.cpp",NULL);

}

void ReadCableType()//CABLETYPE *CblTypes )
{
	char path[80],cbtype[8];
	int nTotal_cbtypes;
	//sprintf(path,"%s:\\lookup.ini",cRamDrive);
	nTotal_cbtypes=GetPrivateProfileInt ("CABLE_TYPE_START","TOTAL_CABLE_TYPES",0,"lookup.ini");

	//CABLETYPE *CblTypes;
	if(nTotal_cbtypes>0)
	{
		 CblTypes = (CABLETYPE *) malloc(sizeof(CABLETYPE)*(nTotal_cbtypes+1));
	}
	else
	{
		//CblTypes = (CABLETYPE *) malloc(sizeof(CABLETYPE));
		printf("Total Cable Types not defined");
		exit(0);
	}
	for(int i=0; i< nTotal_cbtypes; i++)
	{
		sprintf(cbtype,"CBTYPE%d",i+1);
		GetPrivateProfileString("CABLE_TYPE_START",cbtype,CblTypes[i].szCableType,"","lookup.ini");
	}
	strcpy(CblTypes[nTotal_cbtypes].szCableType,"-1");
	//return CblTypes;
}

void ReadCableGuage()//float *fCblGuage)
{

	char path[80],cbtype[8],cbguage[8];
	int nTotalGuages;
	//sprintf(path,"%s:\\lookup.ini",cRamDrive);
	nTotalGuages=GetPrivateProfileInt ("CABLE_GUAGE_START","TOTAL_GUAGES",0,"lookup.ini");


	if(nTotalGuages > 0)
	{
		fCblGuage = (float *)malloc(sizeof(float)*(nTotalGuages+1));
	}
	else
	{
		printf("Total number of Cable Guages not defined");
		exit(0);
	}
	for(int i=0; i< nTotalGuages; i++)
	{
		sprintf(cbtype,"GUAGE%d",i+1);
		GetPrivateProfileString("CABLE_GUAGE_START",cbtype,cbguage,"0","lookup.ini");
		fCblGuage[i]=atof(cbguage);
	}
	fCblGuage[nTotalGuages]=-1;
	//return fCblGuage;

}

void FindIndexofCableType()
{
	char *szcbType, *szcbGuage;
	szcbType = strtok(CableTypeSelcted,"_");
	szcbGuage = strtok(NULL,"_");

	int i=0;
	do
	{
		if(!strcmp(CblTypes[i].szCableType,szcbType))
		{
			printf("\nSelected CableType Index - %d is %s",i+1,CblTypes[i].szCableType);
			break;
		}
		i++;
	}while((strcmp(CblTypes[i].szCableType,"-1")!=0) && (strcmp(CblTypes[i].szCableType,"")!=0));

	int j=0;
	do
	{
		if(fCblGuage[j]==atof(szcbGuage))
		{
			printf("\nSelected CableGuage Index %d - %1.2f",j+1,fCblGuage[j]);
			break;
		}
		j++;
	}while(fCblGuage[j]!=0 && fCblGuage[j]!=-1);

}

float ReadCM80HzLenMFs( unsigned int unCableLen )
{
// Variable     : ATNVarLenOffs  :-  CMVarLenMF[MAX_CAB_TYPE];
// Fields       : nUpperLimit, nLowerLimit - int, fOffset - Float

	char    path[80];
	int     i, j;
	char    szTmp[MAXSTRINGLENGTH1];
	char    cTempStr[20];
	char    *cTempStrPtr,*cTempPtr;
	int     nTempNo_L, nTempNo_U;
	char    cTempStr1[30];
	int     flag;
	int nCMNoofSpans;
	CMVarLenMfs  far *CMVarLenMF;
	float fLenMF=1.0;

	strcpy( path, "cts.ini");//22012001
	nCMNoofSpans = GetPrivateProfileInt("BASIC_INFO","NOOFSPAN_CM80HZMF",0,path);
	if ( nCMNoofSpans <= 0 )
	{
		nCMNoofSpans = 0;
		return 1.0;
	}
	CMVarLenMF = ( CMVarLenMfs far *) farmalloc ( sizeof ( CMVarLenMfs ) *
										(nCMNoofSpans >= MAX_SPANS? MAX_SPANS:nCMNoofSpans));
	if ( CMVarLenMF == NULL )
	{
		//printf( "Cannot Allocate Memory for CMVarLenMF" );
		messageBox( "Cannot Allocate Memory for CMVarLenMF", mfError|mfOKButton );
		exit(1);
	}

	flag = 0; // Used for - whether the Current Range should be checked for
	for (i = 0; i < (nCMNoofSpans >= MAX_SPANS? MAX_SPANS:nCMNoofSpans); i++)
	{
		CMVarLenMF[i].nUpperLimit = 0;
		CMVarLenMF[i].nLowerLimit = 0;
		CMVarLenMF[i].fLenMf = 0.0;
	}
// ( CMVarLenMF[i].nLowerLimit > CMVarLenMF[i-1].nUpperLimit )
	for (i = 0; i < (nCMNoofSpans >= MAX_SPANS? MAX_SPANS:nCMNoofSpans); i++)
	{
		sprintf(szTmp,"SPAN_%d",i+1);
		GetPrivateProfileString("CM80HZMF_SPAN",szTmp,cTempStr,NULL,path);
		if (strcmp( cTempStr,"") == 0 )
			flag = 0;
		else
		{
			cTempStrPtr = strdup(cTempStr);
			nTempNo_L = atoi(strtok(cTempStrPtr,"-"));
			cTempPtr = strchr(cTempStr,'-');
			cTempPtr++;
			nTempNo_U = atoi(cTempPtr);                     // Get the Range Seperated
			free(cTempStrPtr);//added by:sdt:21122016:1900

			if ( flag ) // To be checked
			{
				if ( (nTempNo_L > CMVarLenMF[i-1].nUpperLimit) &&
					 (nTempNo_U > nTempNo_L) )
				{
					CMVarLenMF[i].nLowerLimit = nTempNo_L;
					CMVarLenMF[i].nUpperLimit = nTempNo_U;

					strcpy(cTempStr1,"CM80HZ_LENMF");
					GetPrivateProfileString(cTempStr1,szTmp,cTempStr,NULL,path);//22012001
					if ( strcmp( cTempStr,"") == 0 )
						CMVarLenMF[i].fLenMf = 0.0;
					else
						CMVarLenMF[i].fLenMf = atof( cTempStr );

					flag = 1;   // Check Next Time.
				}
				else
					flag = 0; // Next time to checking & array already Initialise
			}
			else // Not to be checked.
			{
				if ( nTempNo_U > nTempNo_L ) // check if UpperLimit > LowerLimit
				{
					CMVarLenMF[i].nLowerLimit = nTempNo_L;
					CMVarLenMF[i].nUpperLimit = nTempNo_U;

					strcpy(cTempStr1,"CM80HZ_LENMF");
					GetPrivateProfileString(cTempStr1,szTmp,cTempStr,NULL,path);//220012001
					if ( strcmp( cTempStr,"") == 0 )
						CMVarLenMF[i].fLenMf = 0.0;
					else
						CMVarLenMF[i].fLenMf = atof( cTempStr );

					flag = 1;
				}
				else
					flag = 0; // Next Time No Check
			}
		}
	} // End of for Loop

	for( int k = 0; k < (nCMNoofSpans >= MAX_SPANS? MAX_SPANS:nCMNoofSpans); k++)
	{
		if( ( unCableLen >= CMVarLenMF[ k ].nLowerLimit) &&
			( unCableLen <= CMVarLenMF[ k ].nUpperLimit) )
		{
			fLenMF= CMVarLenMF[k].fLenMf;
			break;
		}

	}
	farfree(CMVarLenMF);
	return fLenMF;
} // End of ReadCMVarLenMFs()
