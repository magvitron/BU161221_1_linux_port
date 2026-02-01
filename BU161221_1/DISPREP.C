#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <ctype.h>
#include <dir.h>

#include "disprep.h"
#include "disputl.cpp"

STRUCT_SPECKS speckData;
//char   	szOriginalParams[MAXLEN_SELECT_PARAM];//Commented by:sdt:08102005:1120
//Added by:sdt:09092001:1135
int BufNotEmpty(char *cbuf);
//Added by:sdt:05092001:1135
SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
char LimitTypes[5][30]={"Intra-Adjacent Pairs","Intra-Nonadjacent Pairs","All Combinations","Adjacent Units","NonAdjacent Units"}; //MOD08072002
//Added by:sdt:29082001//Commented by:sdt:29082001
//void InitAdjForBothModeStruct();
//void SavePairAdjacency(int nPno,int nFreqNum,READING *Reading);
//void ModifyConfigLimit(int nParam,int nFreqNum,CONFIGLIMIT *configLimit);
//void FreeAdjForBothModeStruct();
//Added by:sdt:24082001
//ADJFORBOTHMODE far *AdjForBothMode;//Commented by:sdt:29082001

SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex);
CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit);
//Added by:sdt:12082001
int MoreFreqSelected(int nParamNo,int nFreqCount);
//Added by:sdt:11082001
IndPowSum far *ptrIpsum;
void IntermediatePsum(READING *t_Reading);
void CalFinalIndPoweSum(void);
void DisplayPowerSum(int index,int AvgIndPsumFlag);//Added by:sdt:11082001
//Added by:sdt:06062001
WORSTPAIRNEXT Worstpairnext;

//Added by:sdt:09052001
//void SetLimitsForBothMode(int Param);//Commented by:sdt:29082001
//Added by:sdt:08052001::Commented by:sdt:06062001
//float WorstElfext[MAXFREQUENCIES][2];
//float WorstOnext[MAXFREQUENCIES][2];

int endcount;
PARAM_DETAILS Param;//02052001

int IMPEDANCE,FREQUENCY,NORM_TEMP,NORM_LENGTH;
int ZchHfAray[5][MAXFREQUENCIES];//Added by:sdt:09042001:modifid by:sdt:06062001
int nArraySize=0;//Added by:sdt:02042001
//Added by:sdt:02042001
REPORT_PRM  far * prmarray;//[MAXPARAMETERS];
int nMultipleFreqFlag;
int HFFreqSelected,LFFreqSelected;
char SpeckList[MAXFREQUENCIES][30];//by:sdt:24032001
char RamDrv[2]; //jj:22012001
int 					Text1,Text2,Text3;//Added:pnb:27052000
int						bNewPage;
//int 					Unit1=0,Unit2=0,UnitDiff=0;//Commented by:sdt:06092001:1500
//SPECIFICATION           m_Specks;//Commented by:sdt:08102005:2335
INFORMATION             far     m_FileInfo;
//READING                 m_Reading;

//PARAMETER               parameterarray[MAXPARAMETERS];
CONFIG                  nConfig;
CTPAIR                  CtPair;
Page					m_Page;
FILE                    *fpRep ,*fpData;
ParamHeader             far m_ParamHeader[MAXPARAMETERS];
REPSETINGS              *m_RepSettings;
RunSetup runSetup;//Added by:sdt:09102005:1400
//Modified by:sdt:02042001
PRM						far *paramarray;//[MAXPARAMETERS];


long int	lStart;// Tells from where the actual data begins
int			nTempUnitOf,nLastUnitSize,nUnits,nNeedOfAppend;

//Modified by:sdt:03042001
int			far **nDispMatrix;
int 		far **nFreqMatrix;

char        cSerialNo [30];

//Commented:jj:21052001
//void ReadFreqList(void);//Added by:sdt:24032001
int CheckForMultipleFreq(void);//Added by:sdt:24032001
int FindNewDeciNo(float fFlotVal,int nNo,int nParamNo);
void InitialiseReportPrm(int nPno); //Added by:sdt:02042001
void AllocateDispmatrix(void);//Added by:sdt:03042001

inline  int TRUEREADINGS (int nPNo)
{
	return prmarray[nPNo].nReadings==0?1:prmarray[nPNo].nReadings;
}

inline int SUMEITHER (int nPNo)
{
	return m_RepSettings->nNormUnnorm[nPNo]==SUM_BOTH?2:1;
}

void main (int argc,char *argv[])
{
	int argcnt;
    char path[MAXPATH];//Added by:sdt:29082001
    nMultipleFreqFlag=0;
	argc+=0;
	if ( argc == 1 )
		return;

     if( !strcmp(argv[1],"ver") )//Added:jj:22012001:1215
     {
     	char version[10];
          clrscr();
          printf("\n�FX20731");//Alt245

          GetPrivateProfileString ("MAIN","VER","FXS20705",version,"ver.sig",0);
     	printf("\nCompatible to  %s",version);

          getch();
          exit(0);
     }

	sscanf (argv[1],"%p",&m_RepSettings);


  /* m_RepSettings = (REPSETINGS *)malloc(sizeof(REPSETINGS));

	m_RepSettings->nSumm_Detailed=0;
	m_RepSettings->nRemark=1;
	m_RepSettings->nNormUnnorm[0]=3;
	m_RepSettings->nNormUnnorm[1]=3;
	m_RepSettings->nNormUnnorm[2]=3;
	m_RepSettings->nNormUnnorm[3]=3;
	m_RepSettings->nNormUnnorm[4]=3;
	m_RepSettings->nNormUnnorm[5]=3;
	m_RepSettings->nNormUnnorm[6]=3;
	m_RepSettings->nNormUnnorm[7]=3;
	m_RepSettings->nNormUnnorm[8]=3;
	m_RepSettings->nNormUnnorm[9]=3;
    m_RepSettings->nNormUnnorm[10]=3;
    m_RepSettings->nNormUnnorm[11]=3;
    m_RepSettings->nNormUnnorm[12]=3;
    m_RepSettings->nNormUnnorm[13]=3;
    m_RepSettings->nNormUnnorm[14]=3;
	m_RepSettings->nGroupRep = 1;
	m_RepSettings->nParams[0]=9;
	m_RepSettings->nParams[1]=10;//PARAMEND;//10;
	m_RepSettings->nParams[2]=14;//PARAMEND;
	m_RepSettings->nParams[3]=PARAMEND;
	m_RepSettings->nParams[4]=PARAMEND;
	m_RepSettings->nParams[5]=8;//PARAMEND;
	m_RepSettings->nParams[6]=9;//PARAMEND;
	m_RepSettings->nParams[7]=10;//PARAMEND;
	m_RepSettings->nParams[8]=11;//PARAMEND;
	m_RepSettings->nParams[9]=12;//PARAMEND;
    m_RepSettings->nParams[10]=PARAMEND;
    m_RepSettings->nParams[11]=PARAMEND;
    m_RepSettings->nParams[12]=13;//PARAMEND;
    m_RepSettings->nParams[13]=14;//PARAMEND;
    m_RepSettings->nParams[14]=PARAMEND;


     RamDrv[0]='g';//22012001
     RamDrv[1]='\0';*/

	argcnt=4;//Added:pnb:26052000
	if(strcmp(argv[argcnt],"Y")&&strcmp(argv[argcnt],"N"))//Added:pnb:26052000
		argcnt++;

    if(!strcmp(argv[argcnt++],"Y"))//Added:pnb:26052000
	Text1=1;
    if(!strcmp(argv[argcnt++],"Y"))//Added:pnb:26052000
	Text2=1;
    if(!strcmp(argv[argcnt++],"Y"))//Added:pnb:26052000
	Text3=1;


	if ((fpData = fopen (argv[2],"rb")) == NULL)
	{
		fprintf (stderr,"Cannot open the source file %s \n", argv[2]);
		getch ();
		exit (0);
	}
	if ((fpRep = fopen (argv[3],"wt")) == NULL)
	{
		fprintf (stderr,"Cannot open the destination file %s\n", argv[3]);
		getch ();
		exit (0);
	}
    RamDrv[0]=argv[3][0];//jj:22012001
    RamDrv[1]='\0';//jj:22012001

    //Added by:sdt:02042001
   	prmarray = (REPORT_PRM *)farmalloc(sizeof(REPORT_PRM)*MAXPARAMETERS);


    //Commented:jj:21052001
    //ReadFreqList();

	PageSetup();
	ReadBasicData ();
	PageStart ();
	DisplaySetup ();
    nMultipleFreqFlag=CheckForMultipleFreq();
	DispRepHeader ();
    //InitAdjForBothModeStruct();//Added by:sdt:29082001//Commented by:sdt:29082001
	if (m_RepSettings->nSumm_Detailed == SUM_DETAILED)
	{
		DispDetailedReport();
		ReportEnd();
		PageStart();
	}
	m_RepSettings->nRemark = 1;//Commented:jj:14032001 remark made selectable through menu for summary also
   	DispSummaryRep();
	ReportEnd();
	FileClose ();
    //FreeAdjForBothModeStruct();//Added by:sdt:29082001//Commented by:sdt:29082001
    farfree(prmarray);//Added by:sdt:02042001
    sprintf(path,"%s:\\replimit.bin",RamDrv);//Added by:sdt:29082001
    remove(path);//Added by:sdt:29082001
 	sprintf(path,"%s:\\repsum.bin",RamDrv);//Added by:sdt:05092001:0935
    remove(path);//Added by:sdt:05092001:0935

}

void FileClose ()
{

	if(fpRep){fclose(fpRep);}
	if(fpData){fclose(fpData);}
}

void PageSetup()
{
		PageSize();
		Margins ();
		m_Page.nRowsPP = (m_Page.fInchHeight-m_Page.fbottom)*RPI;
		HeaderFooter();
		m_Page.nCurrentln = 1;
		m_Page.nCurrentPg = 1;
		m_Page.nTotalPg = 1;
}

void Margins()
{
		char path[80];//22012001
		char szStr[20]; // to get float data items

          sprintf(path,"%s:\\disprep.ini",RamDrv);//jj:22012001
		GetPrivateProfileString("MARGINS","LEFT","0",szStr,path,0);
		m_Page.fleft = atof (szStr);
		GetPrivateProfileString("MARGINS","TOP","0",szStr,path,0);
		m_Page.ftop = atof (szStr);
		GetPrivateProfileString("MARGINS","RIGHT","0",szStr,path,0);
		m_Page.fright = atof (szStr);
		GetPrivateProfileString("MARGINS","BOTTOM","0",szStr,path,0);
		m_Page.fbottom = atof (szStr);

		if ((m_Page.ftop > m_Page.fInchHeight - m_Page.fbottom) ||
				(m_Page.fleft > m_Page.fInchWidth - m_Page.fright))
		{

			printf ("Please check the Margins !!!\n");
			getch ();
			exit (0);
		}
}

void PageSize ()
{
	char szStr[20]; // To get float data Items

	GetPrivateProfileString("PAGE","HEIGHT","0",szStr,"default.ini",0);
	m_Page.fInchHeight = atof (szStr);
	GetPrivateProfileString("PAGE","WIDTH","0",szStr,"default.ini",0);
	m_Page.fInchWidth = atof (szStr);

	if (m_Page.fInchHeight < 0 || m_Page.fInchWidth < 0)
	{
		printf ("Height or Width cannot be negative\n");
		getch ();
		exit (0);
	}
}

void HeaderFooter ()
{
	char path[80];//22012001
	char szStr[MAXWIDTH]; // To get Header & Footer strings
	int nCheck=0;
	// Header Information

     sprintf(path,"%s:\\disprep.ini",RamDrv);//22012001
	m_Page.bHeader = GetPrivateProfileInt("HEADER","HEADER_DISPLAY",0,path);
	if (m_Page.bHeader == YES)
	{
		nCheck++;
		m_Page.bHAllPg = GetPrivateProfileInt("PAGES","ALL_HEADER",0,path);
		GetPrivateProfileString("HEADER","HEADER_STRING","0",szStr,path,0);

		strcpy (m_Page.szHStr,szStr);
		GetPrivateProfileString("HEADER","HEADER_ALLIGNMENT","0",szStr,path,0);

		if (strcmp (szStr,"CENTER")==0)
			m_Page.bHAllignment = CENTER;
		else if (strcmp (szStr,"LEFT")==0)
			m_Page.bHAllignment = LEFT;
		else if (strcmp (szStr,"RIGHT")==0)
			m_Page.bHAllignment = RIGHT;

		m_Page.bHBorder = GetPrivateProfileInt("BORDER","HEADER_BORDER",0,path);
		if (m_Page.bHBorder == YES)
		{
			nCheck+=2;
			GetPrivateProfileString("BORDER","HEADER_BORDER_STRING","0",szStr,path,0);
			strcpy (m_Page.szHBStr,szStr);
		}
	}

	// Footer Information
	m_Page.bFooter = GetPrivateProfileInt("FOOTER","FOOTER_DISPLAY",0,path);
	if (m_Page.bFooter == YES)
	{
		m_Page.nRowsPP--; // for Footer
		m_Page.bFAllPg = GetPrivateProfileInt("PAGES","ALL_FOOTER",0,path);
		GetPrivateProfileString("FOOTER","FOOTER_STRING","0",szStr,path,0);
		strcpy (m_Page.szFStr,szStr);
		GetPrivateProfileString("FOOTER","FOOTER_ALLIGNMENT","0",szStr,path,0);

		if (strcmp (szStr,"CENTER")==0)
			m_Page.bFAllignment = CENTER;
		else if (strcmp (szStr,"LEFT")==0)
			m_Page.bFAllignment = LEFT;
		else if (strcmp (szStr,"RIGHT")==0)
			m_Page.bFAllignment = RIGHT;

		m_Page.bFBorder = GetPrivateProfileInt("BORDER","FOOTER_BORDER",0,path);
		if (m_Page.bFBorder == YES)
		{
			m_Page.nRowsPP-=2; //To display border around the footer
			GetPrivateProfileString("BORDER","FOOTER_BORDER_STRING","0",szStr,path,0);
			strcpy (m_Page.szFBStr,szStr);
		}
	}
	if (m_Page.nRowsPP == nCheck)
	{
		printf ("Insufficient page size \n");
		getch();
		exit (0);
	}
}

void PageStart ()//modified:pnb:To enable multiline Title:14122000:1635
{
	char cBuf [MAXWIDTH];
    // char cbuf[MAXWIDTH];
     cBuf[0]='\0';

	ClearBuffer (cBuf,MAXWIDTH);
	SkipTopMargin();
	if (m_Page.bHeader == YES)
	{
		if (m_Page.nCurrentPg == 1 || m_Page.bHAllPg == YES)
		{
			if (m_Page.bHBorder == YES)
			{
				FILE *fp;
				char cCh;
				if ( (fp=fopen("title.inf", "r")) == NULL )
				{
				printf("\nCannot open Title.Inf File...");
				getch();
				exit(1);
				}
				while(!feof(fp))
				{
				cCh=fgetc(fp);
				if ( cCh == '\n' )
					{
                        fputc(cCh,fpRep);
                    	m_Page.nCurrentln++;
					}///LineFeed (fpPrn,1);
				else
					fputc(cCh,fpRep);
				}
				if(fp){fclose(fp);}
			}
			else
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szHStr,cBuf,CPI17,m_Page.bHAllignment,endcount);
               cBuf[80] = '\0';
			//WriteStringToFile (fpPrn,cBuf);//Commented:pnb:12112000:17:25
			LineFeed (fpRep,1);
		}
	}
}



void PageEnd ()
{
	char cBuf [MAXWIDTH];

	ClearBuffer (cBuf,MAXWIDTH);
	if (m_Page.bFooter == YES)
	{
		LineFeed (fpRep,1);
		if(m_FileInfo.nFileNumber > 0 )
			sprintf (m_Page.szFStr,"Page#%d [ File Number : %d ]",m_Page.nCurrentPg,m_FileInfo.nFileNumber);
		else
			sprintf (m_Page.szFStr,"Page#%d [ File Number : Untitled ]",m_Page.nCurrentPg);
		if (m_Page.nCurrentPg == 1 || m_Page.bFAllPg == YES)
		{
			if (m_Page.bFBorder == YES)
			{
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFBStr,cBuf,CPI17,CENTER,endcount);
				WriteStringToFile (fpRep,cBuf,endcount);
				LineFeed (fpRep,1);
				ClearBuffer (cBuf,MAXWIDTH);
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFStr,cBuf,CPI17,CENTER,endcount);
				WriteStringToFile (fpRep,cBuf,endcount);
				LineFeed (fpRep,1);
				ClearBuffer (cBuf,MAXWIDTH);
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szHBStr,cBuf,CPI17,CENTER,endcount);
			}
			else
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFStr,cBuf,CPI17,CENTER,endcount);

			WriteStringToFile (fpRep,cBuf,endcount);
			LineFeed (fpRep,1);
		}
	}
	SkipBottomMargin();
	m_Page.nCurrentln = 1;
	m_Page.nCurrentPg++;
}


void SkipTopMargin()
{
	int rows;
	rows = (int)(m_Page.ftop*RPI);
	LineFeed (fpRep,rows);
}

void SkipBottomMargin()
{
	int rows;
	rows = (int)(m_Page.fbottom*RPI);
	LineFeed (fpRep,rows);
}


void ReadBasicData ()
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:05092001:0922
    FILE *fpSummaryFile;//Added by:sdt:05092001:0925
    FILE *fpLimitFile;//Added by:sdt:24082001
    SPECKLIMIT specksLimit;//Added by:sdt:24082001
    char szbuf[20];//Added by:sdt:24082001

	fread (&nConfig,sizeof (nConfig),1,fpData);
	fread ((void*)&m_FileInfo,sizeof (m_FileInfo),1,fpData);
	//fread (&m_Specks,sizeof (m_Specks),1,fpData);
	fwrite(&runSetup,sizeof(runSetup),1,fpData); //Added by:sdt:08102005:2335
	fwrite (&speckData,sizeof(speckData),1,fpData);//Added by:sdt:08102005:2335

	for(int i=0;i<MAXPARAMETERS;i++)
	{   paramarray = (PRM *) farmalloc(sizeof(PRM));
		fread (paramarray,sizeof (PRM),1,fpData);
		InitialiseReportPrm(i);
		farfree(paramarray);
	}
	fread (&CtPair,sizeof (CtPair),1,fpData);
	fread (&cSerialNo,sizeof (cSerialNo),1,fpData);
//	fread (&Decimal,sizeof (Decimal),1,fpData);
	fread (&nTempUnitOf,sizeof(nTempUnitOf),1,fpData);
	fread (&nLastUnitSize,sizeof(nLastUnitSize),1,fpData);
	fread (&nUnits,sizeof(nUnits),1,fpData);
	fread (&nNeedOfAppend,sizeof(nNeedOfAppend),1,fpData);
	//fread (&ZchHfAray,sizeof(ZchHfAray),1,fpData);//Commented by:sdt:08102005:2340

    fread (&Param,sizeof(Param),1,fpData);//Added:jj:02052001

    //Added by:sdt:08052001::Commented by:sdt:06062001
    //fread (&WorstElfext,sizeof(WorstElfext),1,fpData);
    //fread (&WorstOnext,sizeof(WorstOnext),1,fpData);
    fread (&Worstpairnext,sizeof(WORSTPAIRNEXT),1,fpData);

    //Added:jj:21052001
    fread (&SpeckList,sizeof(SpeckList),1,fpData);
	//fread (&szOriginalParams,sizeof(szOriginalParams),1,fpData);//Commented by:sdt:08102005:2340
    //Added by:sdt:24082001
    //This code will writes the limit from .USE FIle into "RepLimit.bin"
	//file in RAMDRIVE.
    //start
   	sprintf(szbuf,"%s:\\replimit.bin",RamDrv);
    remove(szbuf);
   	fpLimitFile=fopen(szbuf,"wb");
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
    if(fpLimitFile!=NULL)
    {
		for(int nParam=0;nParam<MAXPARAMETERS-1;nParam++)
    	{
        	for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
            {
	           	for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
    	        {
        	       	fread(&specksLimit, sizeof (SPECKLIMIT), 1, fpData);
                   	fwrite (&specksLimit, sizeof (SPECKLIMIT), 1, fpLimitFile);
        		}
			}
		}
    }
    if(fpLimitFile){fclose(fpLimitFile);}
    //End

    //Added by:sdt:05092001:0930
    //This code will writes the Summary values from .USE FIle into
	//"repsum.bin" file in RAMDRIVE.
    //start
   	sprintf(szbuf,"%s:\\repsum.bin",RamDrv);//Added by:sdt:05092001
    remove(szbuf);
   	fpSummaryFile=fopen(szbuf,"wb");
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
    if(fpSummaryFile!=NULL)
    {
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)
    	{
        	for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
            {
	           	for(int nSumType=0;nSumType<MAXSUMMARYTYPES;nSumType++)
    	        {
        	       	fread(&SummaryInfo, sizeof (SUMMARYINFO), 1, fpData);
                   	fwrite (&SummaryInfo, sizeof (SUMMARYINFO), 1, fpSummaryFile);
        		}
			}
		}
    }
    if(fpSummaryFile){fclose(fpSummaryFile);}
    //End

	lStart = ftell (fpData); //Tells from where the actual data begins
}

void DisplaySetup ()
{
	int nTmpArray[] ={PARAMEND,PARAMEND};
    int nGrParams[MAXPARAMETERS+1],nIndParams[MAXPARAMETERS+1];//Added by:sdt:31032001
	MemSet (nGrParams,MAXPARAMETERS+1,PARAMEND);
	MemSet (nIndParams,MAXPARAMETERS+1,PARAMEND);

    //Commented by:sdt:03042001
    //MemSet (*nDispMatrix,(MAXPARAMETERS+10)*15,PARAMEND);
    //MemSet (*nFreqMatrix,(MAXPARAMETERS+10)*15,PARAMEND);
	for (int i=0;i<MAXPARAMETERS-1;i++)//Changed by:sdt:08042001:for Zchms
		for (int j=MAXLEVELS-1;j >= 0;j--)
			FillParamHeader(i,j,CPI17);
    AllocateDispmatrix();//Added by:sdt:03042001
	if (m_RepSettings->nGroupRep == YES)
		GroupParams (nGrParams);

	if (nGrParams[0] != PARAMEND)
		if (m_RepSettings->nGroupRep == YES) // because GroupRep changes the value of m_RepSettings->nGroupRep
			DisplayMatrix (nGrParams);
	IndividualParams (nGrParams ,nIndParams);

	if (nIndParams[0] != PARAMEND)
	{
		for (i = 0;nIndParams[i] != PARAMEND;i++)
		{
			nTmpArray[0] = nIndParams[i];
			DisplayMatrix (nTmpArray);
		}
	}
}

void GroupParams (int *nArray)
{
	char path[80];//22012001
	int temp[MAXPARAMETERS+1] ,i = 0,j ,nCount = 0;
	char szStr [MAXWIDTH],*p;

	MemSet (temp,MAXPARAMETERS+1,PARAMEND);
     sprintf(path,"%s:\\disprep.ini",RamDrv);//jj:22012001
	GetPrivateProfileString("REPORT","GROUP_PARAMS","",szStr,path,0);
	if (szStr[0]==NULL)
	{
		m_RepSettings->nGroupRep = FALSE;
		return;
	}
	p = strtok (szStr,",");
	while (p)
	{
		 temp[i++] = atoi(p);
		 p = strtok (NULL,",");
	}
	for (i = 0;temp[i] != PARAMEND;i++)
	{
		for (j = 0;m_RepSettings->nParams[j] != PARAMEND; j++)
		{
			if ((temp[i] == m_RepSettings->nParams[j]) &&
			(prmarray[temp[i]].nTestingMethod == SINGLEPAIR))
			{
				nArray [nCount++] = temp[i];
				break;
			}
		}
	}
}

void IndividualParams (int *nGrArray ,int *nIndArray)
{
	int i ,j ,nCount = 0;
	char bFound;

	for (i = 0;m_RepSettings->nParams[i] != PARAMEND;i++)
	{
		bFound = FALSE;
		for (j = 0;nGrArray[j] != PARAMEND; j++)
		{
			if (m_RepSettings->nParams[i] == nGrArray [j])
			{
				bFound = TRUE;
				break;
			}
		}
		if (bFound != TRUE)
			nIndArray [nCount++] = m_RepSettings->nParams[i];
	}
}

void FillParamHeader(char nPNo,char nLevel,int cpi)
{
	int k = 0,n,i,j,nTemp=0;
	char *p,szTmp[MAXPARAMNAME]; //Currently 55 bytes
	switch (nLevel)
	{
		case 3:
			m_ParamHeader[nPNo].Levels[nLevel].nNoofParts=TRUEREADINGS(nPNo)*SUMEITHER(nPNo);
			for (i = 0;i < TRUEREADINGS(nPNo);i ++)
			{
				for (j = 1;j <= SUMEITHER(nPNo);j++)
				{
					m_ParamHeader[nPNo].Levels[nLevel].Parts[k].nCpi=cpi;
					m_ParamHeader[nPNo].Levels[nLevel].Parts[k].nTChars=MAXCOLCHAR;
					strcpy (szTmp,RdTypeHeader(nPNo,j));
					strcpy (m_ParamHeader[nPNo].Levels[nLevel].Parts[k++].szPHeader,szTmp);
				}
			}
			break;
		case 2:
			m_ParamHeader[nPNo].Levels[nLevel].nNoofParts=TRUEREADINGS(nPNo);
			for (i = 0,k = 0;i < TRUEREADINGS(nPNo);i++)
			{
				n = 0;
				for (j=0;j < SUMEITHER(nPNo);j++,k++)
					n += m_ParamHeader[nPNo].Levels[nLevel+1].Parts[k].nTChars;
				m_ParamHeader[nPNo].Levels[nLevel].Parts[i].nTChars = n;
				m_ParamHeader[nPNo].Levels[nLevel].Parts[i].nCpi=cpi;
			}
			if (prmarray[nPNo].nReadings > 1)
			{
				strcpy (szTmp,prmarray[nPNo].szReadingNames);
				p = strtok (szTmp,",");
				for (j = 0;j < prmarray[nPNo].nReadings;j++)
				{
					if (p) {
						strcpy (m_ParamHeader[nPNo].Levels[nLevel].Parts[j].szPHeader,p);
						p = strtok (NULL,",");
					}
				}
			}
			else
				strcpy (m_ParamHeader[nPNo].Levels[nLevel].Parts[0].szPHeader," ");
			break;
        case 1:
            //Added by:sdt:10042001
            for (i = 0;i<m_ParamHeader[nPNo].Levels[MAXLEVELS-1].nNoofParts;i++)
				nTemp+=m_ParamHeader[nPNo].Levels[MAXLEVELS-1].Parts[i].nTChars;
            m_ParamHeader[nPNo].Levels[nLevel].nNoofParts = 1;
            m_ParamHeader[nPNo].Levels[nLevel].Parts[0].nCpi=cpi;
			m_ParamHeader[nPNo].Levels[nLevel].Parts[0].nTChars=nTemp;
            strcpy(m_ParamHeader[nPNo].Levels[nLevel].Parts[0].szPHeader,"");
            break;
		case 0:
			m_ParamHeader[nPNo].nCpi = cpi;
			for (i = 0;i<m_ParamHeader[nPNo].Levels[MAXLEVELS-1].nNoofParts;i++)
				nTemp+=m_ParamHeader[nPNo].Levels[MAXLEVELS-1].Parts[i].nTChars;
			m_ParamHeader[nPNo].nTChars = nTemp;
			m_ParamHeader[nPNo].nNoofLevels = MAXLEVELS;
			m_ParamHeader[nPNo].Levels[nLevel].nNoofParts = 1;
			m_ParamHeader[nPNo].Levels[nLevel].Parts[0].nTChars = nTemp;
			m_ParamHeader[nPNo].Levels[nLevel].Parts[0].nCpi = cpi;
			strcpy(szTmp,prmarray[nPNo].szParamNameAbr);
			strcpy(m_ParamHeader[nPNo].Levels[nLevel].Parts[0].szPHeader,szTmp);
	}
}

char *RdTypeHeader(char nPNo,char nSumType)
{
	char path[80];//22012001
	int nSwitchOn;
	char szTmp[MAXPARAMNAME]; //Currently 55 bytes

     sprintf(path,"%s:\\cts.ini",RamDrv);//jj:22012001
	nSwitchOn=m_RepSettings->nNormUnnorm[nPNo]==SUM_BOTH?nSumType:m_RepSettings->nNormUnnorm[nPNo];
	switch (nSwitchOn)
	{
		case SUM_NORMALISED :
			if ( GetPrivateProfileInt("BASIC_INFO",
				"NORMALISE_KM",0,path) )
				sprintf (szTmp,"%s/Km",prmarray[nPNo].szUnit);
			else if ( atof(speckData.fNormLength) == (float)0
				   || atof(speckData.fNormLength) ==	(float)1000)
				sprintf (szTmp,"%s/Km",prmarray[nPNo].szUnit);
			else
				sprintf (szTmp,"%s/NL=%s",prmarray[nPNo].szUnit,
						speckData.fNormLength );
			break;
		case SUM_UNNORMALISED :
			sprintf (szTmp,"%s/L",prmarray[nPNo].szUnit);
			break;
		case SUM_NEUTRAL :
			strcpy (szTmp,prmarray[nPNo].szUnit);
			break;
	}
	return szTmp;
}



READING *GetData (int nPno,int nFreqNo)
{
	int		nTReadings,nCount;
	char    ValidData = FALSE;
	READING	*Data;
	READING	t_Readings;
    //int nLoopCount=0;
	nTReadings  =  TReadings(nPno,nFreqNo);  //pr // TReadings - gives total readings, OK + NOTOK.
	Data = (READING far *)farmalloc (sizeof (READING)*nTReadings);
	fseek( fpData, long(0), SEEK_END);
	long san = ftell(fpData);
     san;
	fseek (fpData,lStart,SEEK_SET);

	while (!feof (fpData))
	{
		if (fread (&t_Readings,sizeof (READING),1,fpData) > 0)
			ValidData = TRUE;
		if (t_Readings.cParam == nPno&&t_Readings.nFreq==nFreqNo)
		{
			nCount = 0;
			nTReadings =  TReadings(nPno,nFreqNo);
			do
			{
				Data[nCount++] = t_Readings;
				fread (&t_Readings,sizeof (READING),1,fpData);
			}while ((t_Readings.cParam == nPno&&t_Readings.nFreq==nFreqNo) && !feof (fpData) &&
				nCount<=nTReadings);
			break;
		}
		else
		{
			nTReadings =  TReadings(t_Readings.cParam,t_Readings.nFreq)-1;
			if (nTReadings < 0)
				return NULL;
			if (fseek (fpData,(long)sizeof(READING)*nTReadings,SEEK_CUR)!=0)
				return NULL;
		}
	}
	if (ValidData == TRUE && nCount != 0)
		return Data;
	return NULL;
}


int TReadings(int nPno,int nFreqNo)
{
	return m_FileInfo.nReadingsOk[nPno][nFreqNo] + m_FileInfo.nReadingsNotOk[nPno][nFreqNo];
}

//Commeted by:sdt:23032001
/*float AreainChar (int nPno)
{
	return (float)m_ParamHeader[nPno].nTChars;
}*/

float RepeatSet (int nSetStart,int nSetEnd,int *Array,int *col,
					int nCharAvail,int nCharReqr)
{
	int i,j;

	while (nCharReqr < nCharAvail)
	{
		nCharAvail -= nCharReqr;
		for (i=nSetStart,j=*col;i<nSetEnd;i++,j++)
			Array[j] = Array[i];
		*col = j;
	}
	return nCharAvail;
}

void RowComplete (int nSStart,int nSEnd,int *row,int *col,int *Array,int nTChar,int nCharAvail)
{
	int nAdjustingFactor,i,j,k,nCount = 1,nTCharReqr,nParts,nTmpFactor;

	nTCharReqr = nTChar - nCharAvail;
	if (m_RepSettings->nRemark == YES)
		Array[*col] = 'R';
	for (i = 0;i < *col; i++)
		if (Array[i] != 'R' && Array[i] != 'C')
			nCount++;
	nAdjustingFactor = (nTChar - nTCharReqr)/nCount;
	for (i = nSStart;i < nSEnd; i++)
	{
		if (Array[i] != 'R' && Array[i] != 'C')
		{
			m_ParamHeader[Array[i]].nTChars+=nAdjustingFactor;
			for (j = 0;j < MAXLEVELS;j++)
			{
				nParts=m_ParamHeader[Array[i]].Levels[j].nNoofParts;
				nTmpFactor = nAdjustingFactor / nParts;
				for (k=0;k<nParts;k++)
					m_ParamHeader[Array[i]].Levels[j].Parts[k].nTChars += nTmpFactor;
				m_ParamHeader[Array[i]].Levels[j].Parts[k-1].nTChars+=nAdjustingFactor%nParts;
			}
		}
	}
	(*row)++,*col = 0;
}

void DispDetailedReport ()
{
    //char cbuf1[MAXWIDTH+100];//Added by:sdt:07092001:1150
    int nPrevUnit1,nPrevUnit2;//Added by:sdt:07092001:1500
    char nPrevAdjacency=0,nCurrAdjacency=0;
    int nUnit2;//Added by:sdt:26042001
	int nprTemp1;
	int i=0,j,s,r,nPC,nSwitchOn,NotOk=FALSE,nReadings,nPno;//,nCount = 0; //nPC Pair Count
	int nTChars,nCpi,nValidPno,nUnitNo,flagWithinSpeck=TRUE;
	float fs,fe;   // Start & End
	READING *t_Reading,param_Reading;
	char cbuf[MAXWIDTH+100],ctmp[MAXWIDTH+100];
    //long ParaPosition[MAXPARAMETERS][MAXFREQUENCIES];
    //Data type modified from long to unsigned long //MOD17012002:02:sdt::1255
    unsigned long ParaPosition[MAXPARAMETERS][MAXFREQUENCIES];

    int nTReadings;
	char Path[80];
    int AVGINDPSUM=0; //Added by:sdt:12082001:this is act as switch
    			   //for selection of Avg. Ind. Power Sum
//	while ((int)m_FileInfo.cPairsBypassed[i]!=127)
//		nCount++,i++;

    //Added by:sdt:09042001
    //Options Selection for Parameter header.
    //start

    sprintf(Path,"%s:\\Disprep.ini",RamDrv);
    IMPEDANCE = GetPrivateProfileInt("PARAM_HEADER_OPTION","IMPEDANCE",0,Path);
    FREQUENCY = GetPrivateProfileInt("PARAM_HEADER_OPTION","FREQUENCY",0,Path);
    NORM_TEMP = GetPrivateProfileInt("PARAM_HEADER_OPTION","NORM_TEMP",0,Path);
    NORM_LENGTH = GetPrivateProfileInt("PARAM_HEADER_OPTION","NORM_LENGTH",0,Path);
    AVGINDPSUM =GetPrivateProfileInt("AVG_IND_PSUM","AVG_PSUM",0,Path); //commented by:sdt:14082001
    //End
    //Added by:sdt:04042001
    fseek (fpData,lStart,SEEK_SET);
    for(i=0;i<MAXPARAMETERS-1;i++)//Changed by:sdt:08042001:for Zchms
    {
    	for(j=0;j<MAXFREQUENCIES;j++)
        {
            //if(nConfig.Specks.speckparams[i].freq[j]&&m_FileInfo.cParamTested[i][j])
            if(m_FileInfo.cParamTested[i][j])
            {
        		while (!feof (fpData))
				{
					if (fread (&param_Reading,sizeof (READING),1,fpData) > 0)
					{	//ValidData = TRUE;
						if (param_Reading.cParam ==i &&param_Reading.nFreq==j)
						{
	        	            ParaPosition[i][j]=ftell(fpData)-sizeof(READING);
                            //Added by:sdt:06042001
                            fseek(fpData,ftell(fpData)-sizeof(READING),SEEK_SET);
							break;
						}
						else
						{
							nTReadings =  TReadings(param_Reading.cParam,param_Reading.nFreq)-1;
							fseek (fpData,(long)sizeof(READING)*nTReadings,SEEK_CUR);//!=0)
						}
               		}
				}
    	    }
        }
    }



	for (i=0;nDispMatrix[i][0]!=PARAMEND;i++)
	{
		nValidPno = nDispMatrix[i][1];
        nCurrAdjacency=-1;//Added by:sdt:07092001
		nPrevAdjacency=-1;//Added by:sdt:07092001
        //Added by:sdt:11082001
        //Allocate memory for Individual Powesum variable for total no. of pairs
        //and Initialise it with 0.
        if(nValidPno ==SRNO_NEXT)
        {
        	ptrIpsum = (IndPowSum *)farmalloc(sizeof(IndPowSum)*m_FileInfo.cNoofPairs);
            memset(ptrIpsum,0,sizeof(IndPowSum)*m_FileInfo.cNoofPairs);
            //AvgIndPsum = (IndPowSum *)farmalloc(sizeof(IndPowSum)*m_FileInfo.cNoofPairs);
        }
		ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
		DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
		ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
		m_FileInfo.cUnitof = nTempUnitOf; // For restoring normal no. of units
		nPC = -1,nUnitNo=0;nUnit2=0;
        nPrevUnit1=0;nPrevUnit2=0;//Added by:sdt:07092001:1500
		nReadings=TReadings(nValidPno,nFreqMatrix[i][1])/TRUEREADINGS(nValidPno);
		while (1)
		{
			ClearBuffer(cbuf,MAXWIDTH);
			fs=fe=m_Page.fleft , NotOk = FALSE;
			for (j=0;nDispMatrix[i][j]!=PARAMEND;j++)
			{
				ClearBuffer(ctmp,MAXWIDTH);
				if (nPC>=nReadings)
					break;
				nPno=nDispMatrix[i][j];
				switch(nPno)
				{
				case 'C' :
						// 'C' indicates the begining of new pair reading
						nPC++;
						if (nPC>=nReadings)
							break;
						nPno=nDispMatrix[i][j+1]; // Valid Parameter
						t_Reading=OneGrReading(nPno,nPC*TRUEREADINGS(nPno),ParaPosition[nPno],nFreqMatrix[i][j+1]);
						if (t_Reading == NULL)
							break;

 // Added : Start : jj:14032001
						nUnitNo= ((t_Reading->cPair1 - 1)/m_FileInfo.cUnitof) + 1;//Added by:sdt:09092001
                        nUnit2=((t_Reading->cPair2 - 1)/m_FileInfo.cUnitof) + 1;//Added by:sdt:09092001
						//if (nUnitNo!=nUnits && (t_Reading->cPair1 > m_FileInfo.cUnitof*nUnitNo||t_Reading->cPair2 > m_FileInfo.cUnitof*nUnit2))//Modified:jj:14032001
                        //Commented by:sdt:07092001:1430
                        //if (nUnitNo!=nUnits &&((nUnitNo!=nPrevUnit1)||(nUnit2!=nPrevUnit2)))
                        //Modified by:sdt:10092001:1200
                        //Condition is modified because last unit number was
						//not displayed.
                        if ((nUnitNo!=nPrevUnit1)||(nUnit2!=nPrevUnit2))
						{
							ClearBuffer (ctmp,MAXWIDTH);
							if (strcmp (cbuf,ctmp)!=0)
							{
								bNewPage = FinishRow(j,i,fs,fe,cbuf,NotOk);
                                NotOk = FALSE; //MOD17012002:04:sdt::1655
                                fs=fe=m_Page.fleft;//MOD18012002:01:sdt::1025
                                j=0;//MOD18012002:01:sdt::1050
								//bNewPage = LineFeed(fpRep,1);//uncommented by:sdt:13092001
								if (bNewPage == YES)
								{
								  	PageEnd();
								    PageStart();
								    if (nPC < nReadings)
                                    {
                                     	ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
									 	DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
                                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
                                    }
								}
                                ClearBuffer(ctmp,MAXWIDTH);
                	            ClearBuffer(cbuf,MAXWIDTH);
							}
							//nUnitNo++;
                            //Commented by:sdt:07092001
                            //nUnitNo=((t_Reading->cPair1 - 1)/m_FileInfo.cUnitof) + 1;
                            //nUnit2=((t_Reading->cPair2 - 1)/m_FileInfo.cUnitof) + 1;//Added by:sdt:26042001
                            //Added by:sdt:07092001:1500
                              nPrevUnit1=nUnitNo;nPrevUnit2=nUnit2;
						   	if(nUnitNo==nUnits)
							  /*if(m_FileInfo.nAppend && nNeedOfAppend)
                              { */
								m_FileInfo.cUnitof=nLastUnitSize;     //san
                             // }

							ClearBuffer (cbuf,MAXWIDTH);

							if( nValidPno == SRNO_CUPPI )   // added : jj:jp:07022001 : for parameter cuppi
							{
                                //Added by:sdt:10092001:1105
                                //start
                                //For the CUPPI's detailed report
                                //label "Adjacent Units / NonAdjacent Units"
                                //should be before the actual unit number
                                //is to be displayed. therefore this condition
                                //is added over here which print unit numbers
                                //after label "Adjacent Units / NonAdjacent Units".
                                nCurrAdjacency=t_Reading->nUnitAdjacency;
                                if(nCurrAdjacency!=nPrevAdjacency)
                                {
	                                strcpy(ctmp,LimitTypes[nCurrAdjacency+2]);

		                            PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,CPI17,CENTER,endcount);

    		                        WriteStringToFile (fpRep,cbuf,endcount);
						            bNewPage = LineFeed(fpRep,1);
							        if (bNewPage == YES)
						  			{
										PageEnd();
										PageStart();
										if (nPC < nReadings)
    	        			        	{
        	            					ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
											DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
	            				            ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
				    	                }
									}
            	            	    ClearBuffer(ctmp,MAXWIDTH);
                	            	ClearBuffer(cbuf,MAXWIDTH);
	                                //NotOk = FALSE;//Added by:sdt:10092001:0905
                                }
                                nPrevAdjacency=nCurrAdjacency;
                                //end.
    		                    sprintf(ctmp,"Unit: #%d-#%d",nUnitNo,nUnit2);//((t_Reading->cPair1 - 1)/m_FileInfo.cUnitof) + 1,);

                	        }
                            else
                            	sprintf (ctmp,"Unit: #%d",nUnitNo);
                            fs=m_Page.fleft;

                            //if(nValidPno != SRNO_CUPPI )//Commented by:sdt:10092001:1050


    						PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,CPI17,CENTER,endcount);
							WriteStringToFile (fpRep,cbuf,endcount);
							bNewPage = LineFeed (fpRep,1);
							if (bNewPage == YES)
							{
							  PageEnd();
							  PageStart();
							  if (nPC < nReadings)
	                          {
    	                    	 ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
								 DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
            	                 ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
                	           }
							}

							ClearBuffer(ctmp,MAXWIDTH);
							ClearBuffer(cbuf,MAXWIDTH);
						}

                        //Added by:sdt:07092001:1100
	                    //start
    	                //This condition is added for displaying Adjacent &
        	            //Nonadjcent readings separately.
                        //Modified by:sdt:10092001:1155
            	        if(!prmarray[nPno].nTestingMethod&&(nPno!=SRNO_CUPPI))
                	    {
                            //Commented by:sdt:10092001:1140
                            //Adjacency information for CUPPI is displayed
                            //above.
	                    	/*if(nPno==SRNO_CUPPI)
    	                    	nCurrAdjacency=t_Reading->nUnitAdjacency;
        	                else*/
            	            	nCurrAdjacency=t_Reading->nPairAdjacency;
                	        if(nCurrAdjacency!=nPrevAdjacency)
                    	    {
                	    	   	//WriteStringToFile (fpRep,cbuf,endcount);//commented by:sdt:09092001:1035
                                if(BufNotEmpty(cbuf))//Added by:sdt:09092001:1035
                                {
                                   	bNewPage = FinishRow(j,i,fs,fe,cbuf,NotOk);//Added by:sdt:09092001:1035
                                    NotOk=FALSE;  //MOD17012002:04:sdt::1700
                                    fs=fe=m_Page.fleft;//MOD18012002:01:sdt::1025
                                    j=0;//MOD18012002:01:sdt::1050
                                }
                                else
					            	bNewPage = LineFeed(fpRep,1);
						        if (bNewPage == YES)
					  			{
									PageEnd();
									PageStart();
									if (nPC < nReadings)
            				        {
                    					ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
										DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
            			            	ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
				                    }
								}
        	                    ClearBuffer(ctmp,MAXWIDTH);
            	                ClearBuffer(cbuf,MAXWIDTH);
                                //Commented by:sdt:10092001:1140
                                //Adjacency information for CUPPI is displayed
	                            //above.
	               	            /*if(nPno==SRNO_CUPPI)
	                	            strcpy(ctmp,LimitTypes[nCurrAdjacency+2]);
                        	    else*/
                            		strcpy(ctmp,LimitTypes[nCurrAdjacency]);

	                            PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,CPI17,CENTER,endcount);

    	                        WriteStringToFile (fpRep,cbuf,endcount);
					            bNewPage = LineFeed(fpRep,1);
						        if (bNewPage == YES)
					  			{
									PageEnd();
									PageStart();
									if (nPC < nReadings)
            			        	{
                    					ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
										DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
	            			            ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
				                    }
								}
            	                ClearBuffer(ctmp,MAXWIDTH);
                	            ClearBuffer(cbuf,MAXWIDTH);
                                //Added by:sdt:10092001:0905
                                //If this variable is not initializes done as
                                //below, then it may affect the remark of
                                //readings on next line
                                NotOk = FALSE;
                    	    	//fs=fe=m_Page.fleft;


                        	}

	                        nPrevAdjacency=nCurrAdjacency;

        	            }
            	        //end
						fs=fe,fe += (float)MAXCOLCHAR / CPI17;
						if (prmarray[nPno].nTestingMethod==SINGLEPAIR)
							sprintf (ctmp,"%d",t_Reading->cPair1);
						else
							sprintf (ctmp,"%d-%d",t_Reading->cPair1,t_Reading->cPair2);
						PrepareString(fs,fe,ctmp,cbuf,CPI17,CENTER,endcount);
                        //Added by:sdt:01042001
                        farfree(t_Reading);
						break;
				case 'R' :
						fs=fe,fe += (float)MAXCOLCHAR / CPI17;
						if (NotOk == TRUE)
							PrepareString(fs,fe,"Not Ok",cbuf,CPI17,CENTER,endcount);
						else
							PrepareString(fs,fe,"Ok",cbuf,CPI17,CENTER,endcount);
						break;
				default :
				  int k=0;
				  for (r=0;r<TRUEREADINGS(nPno);r++)
				  {
					t_Reading=OneGrReading(nPno,nPC*TRUEREADINGS(nPno)+r,ParaPosition[nPno],nFreqMatrix[i][j]);
					if (t_Reading == NULL)
						break;

					for (s=1;s<=SUMEITHER(nPno);s++)
					{
					  ClearBuffer(ctmp,MAXWIDTH);
					  nSwitchOn=m_RepSettings->nNormUnnorm[nPno]==SUM_BOTH?s:m_RepSettings->nNormUnnorm[nPno];
					  switch (nSwitchOn)
					  {
						  case SUM_NORMALISED :
							//nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[r*TRUEREADINGS(nPno)+s-1].nTChars;
							//nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[r*TRUEREADINGS(nPno)+s-1].nCpi;
							nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k].nTChars;
							nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k++].nCpi;
							fs=fe,fe+=(float)nTChars/nCpi;
                            //Below WithinSpeck() function calls modified by
                            //:sdt:24082001
                            //Modified by:sdt:26042001
                            if(t_Reading->nPairAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,(t_Reading->nPairAdjacency));
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,(t_Reading->nPairAdjacency));
                            else
                            if(t_Reading->nUnitAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,t_Reading->nUnitAdjacency+2);
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,t_Reading->nUnitAdjacency+2);
                            else
                                //flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,0);//,t_Reading->nPairAdjacency);
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,0);

							NotOk = flagWithinSpeck == FALSE ? TRUE: NotOk;

////////////	///begin pranav
							if (t_Reading->fNormVal != 0.0)
								nprTemp1 = FindNewDeciNo(t_Reading->fNormVal,
												prmarray[nPno].Decimal[0],nPno);
							else
								nprTemp1 = 1;
							sprintf (ctmp ,"%4.*f%c",nprTemp1,t_Reading->fNormVal,IFMARK(flagWithinSpeck));
////////////////////////	//
//pr//						sprintf (ctmp ,"%4.*f%c",Decimal[nPno],t_Reading->fNormVal,IFMARK(flagWithinSpeck));
							PrepareString(fs,fe,ctmp,cbuf,nCpi,CENTER,endcount);
							break;
						  case SUM_UNNORMALISED :
							//nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[r*TRUEREADINGS(nPno)+s-1].nTChars;
							//nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[r*TRUEREADINGS(nPno)+s-1].nCpi;
							nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k].nTChars;
							nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k++].nCpi;
							fs=fe,fe+=(float)nTChars/nCpi;
							//flagWithinSpeck = WithinSpeck (nPno,t_Reading.fRawVal);
							//NotOk = flagWithinSpeck == FALSE ? TRUE: NotOk;

////////////////////	//////////////////////////////////
	///////////////begin pranav
							if (t_Reading->fRawVal != 0.0)
								nprTemp1 = FindNewDeciNo(t_Reading->fRawVal,
															prmarray[nPno].Decimal[0],nPno);
							else
							   nprTemp1 = 1;
							sprintf (ctmp ,"%4.*f",nprTemp1,t_Reading->fRawVal);
//////////////////////////
//pr//						sprintf (ctmp ,"%4.*f",Decimal[nPno],t_Reading->fRawVal);
							PrepareString(fs,fe,ctmp,cbuf,nCpi,CENTER,endcount);
							break;
						  case SUM_NEUTRAL :
							nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k].nTChars;
							nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k++].nCpi;
							fs=fe,fe+=(float)nTChars/nCpi;
							//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fRawVal);

                            //Below WithinSpeck() function calls modified by
                            //:sdt:24082001
                            //Modified by:sdt:26042001
                            if(t_Reading->nPairAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,(t_Reading->nPairAdjacency));
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,(t_Reading->nPairAdjacency));
                            else
                            if(t_Reading->nUnitAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,t_Reading->nUnitAdjacency+2);
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,t_Reading->nUnitAdjacency+2);
                            else
                                //flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,0);//,t_Reading->nPairAdjacency);
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,0);


							NotOk = flagWithinSpeck == FALSE ? TRUE: NotOk;

//////////////////////////////////////////////////////
///////////////begin pranav
							if (t_Reading->fRawVal != 0.0)
								nprTemp1 = FindNewDeciNo(t_Reading->fRawVal,
									prmarray[nPno].Decimal[0],nPno);
							else
							   nprTemp1 = 1;
							sprintf (ctmp ,"%4.*f%c",nprTemp1,t_Reading->fRawVal,IFMARK(flagWithinSpeck));
////////////	//////////////
//pr//						sprintf (ctmp ,"%4.*f%c",Decimal[nPno],t_Reading->fRawVal,IFMARK(flagWithinSpeck));
							PrepareString(fs,fe,ctmp,cbuf,nCpi,CENTER,endcount);
							break;
					  }
					}
                    //Intermediate stage of calculating Individual
                    //Power sum.
                    if(nPno==SRNO_NEXT) //Condition Added by:sdt:12082001
                    {
						IntermediatePsum(t_Reading);
                    }
                    //commented by:sdt:29082001
                    //Condition Added by:sdt:27082001
                    //This will save Adjacencies used pairs, used to display
                    //summary readings.
                    /*if((!prmarray[nPno].nTestingMethod)&&(m_FileInfo.nPairAdjacency[nPno]==BOTHADJ
						||m_FileInfo.nUnitAdjacency[nPno]==BOTHADJ))
                    {
                    	SavePairAdjacency(nPno,nFreqMatrix[i][j],t_Reading);
                    }*/
                    //Added by:sdt:01042001
                  	farfree(t_Reading);
				  }
				  break;
				}
                //Added by:sdt:29032001
            	//farfree(t_Reading);
			}
			ClearBuffer(ctmp,MAXWIDTH);
			if (nPC != nReadings)
			{
				  WriteStringToFile (fpRep,cbuf,endcount);
				  bNewPage = LineFeed(fpRep,1);
				  if (bNewPage == YES)
				  {
					PageEnd();
					PageStart();
					if (nPC < nReadings)
                    {
                    	ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
						DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
                    }
				  }
                  ClearBuffer(ctmp,MAXWIDTH);
                  ClearBuffer(cbuf,MAXWIDTH);
			}
			if (strcmp (cbuf,ctmp)!=0)
			{
			  if (nPC==nReadings)
			  {
				bNewPage = FinishRow (j-1,i,fs,fe,cbuf,NotOk);
                NotOk = FALSE; //MOD17012002:04:sdt:1700
                fs=fe=m_Page.fleft;//MOD18012002:01:sdt::1025
                j=0;//MOD18012002:01:sdt::1050
                ClearBuffer(ctmp,MAXWIDTH);
                ClearBuffer(cbuf,MAXWIDTH);
                //bNewPage = LineFeed(fpRep,1);//Added by:sdt:13092001
				if (bNewPage == YES)
				{
					PageEnd();
					PageStart();
					if (nPC <nReadings)
                    {
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
						DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
                    }
				}
			  }
			}
           //commentedby:sdt:29032001
		   if (nPC==nReadings)
		   {
			break;
		   }
		}
        if(nValidPno ==SRNO_NEXT)
        {
			CalFinalIndPoweSum(); //Added by:sdt:11082001
	        DisplayPowerSum(i,AVGINDPSUM);//Added by:sdt:11082001
        	farfree(ptrIpsum);
        }
	}

    //Added by:sdt:03042001
	for(i=0;i<nArraySize;i++)
    {
    	farfree(nDispMatrix[i]);
 		farfree(nFreqMatrix[i]);
    }
}

int FinishRow (int col,int row,float fs,float fe,char *cbuf,int bNotOk)
{
	int nPno,nTChars,nCpi,bNewPage = FALSE;
	for (int k=col;nDispMatrix[row][k]!=PARAMEND;k++)
	{
	  nPno=nDispMatrix[row][k];
	  switch (nPno)
	  {
	  case 'C':
		fs=fe,fe += (float)MAXCOLCHAR / CPI17;
		break;
	  case 'R':
		fs=fe,fe += (float)MAXCOLCHAR / CPI17;
		if (bNotOk == TRUE)
			PrepareString(fs,fe,"Not Ok",cbuf,CPI17,CENTER,endcount);
		else
			PrepareString(fs,fe,"Ok",cbuf,CPI17,CENTER,endcount);
		//WriteStringToFile (fpRep,cbuf);
		//bNewPage = LineFeed(fpRep,1);
		break;
	  default :
		nTChars=m_ParamHeader[nPno].nTChars;
		nCpi=m_ParamHeader[nPno].nCpi;
		fs=fe,fe+=(float)nTChars/nCpi;
		break;
	  }
  }
  WriteStringToFile (fpRep,cbuf,endcount);
  bNewPage = LineFeed(fpRep,1);
  /*if (bNewPage == YES)//Added Condition : pnb: 27121999.
  {
	PageEnd();
	PageStart();

    if (nPC <nReadings)
    {
	   	ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
		DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
	    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
    }


  }*/
  return bNewPage;
}

// Function to check if the parameter is in group parameter
//commented by:sdt:31032001
/*int CheckParamType(int nPno)
{
	int i,bfound=FALSE;

	for (i=0;nGrParams[i]!=PARAMEND;i++)
		if (nPno==nGrParams[i])
			bfound=TRUE;
	return bfound;
} */

//MOD17012002:02:sdt::1255
//Data type of variable *ParaFilePos modified from long to unsigned long
READING *OneGrReading(int nPno,int nReadingNum,unsigned long *ParaFilePos,int nFreqNo)
{

	char    ValidData = FALSE;
    long    lFilePos;
	READING	*Data;//Modified by:sdt:01042001
	READING	t_Readings;
	Data = (READING far *)farmalloc (sizeof (READING)); //Changed by:sdt:29032001
	fseek( fpData, long(0), SEEK_END);
	long san = ftell(fpData);

     san;
	fseek (fpData,lStart,SEEK_SET);
    lFilePos = ParaFilePos[nFreqNo];
    //lFilePos +=sizeof(READING)*nReadingNum;


    //Statement modified by:sdt:20117 //Mod17012002:02:sdt::1430
    //In Prevois statement ReadingNum is an Integer. The product of
    //"sizeof(READING)*nReadingNum" is also returns the interger.
    //In Cuppi when maximum reading goes upto 4000. For reading numbers above
    //3500(not exactly)  product of "sizeof(READING)*nReadingNum" crosses
    //the value of MAX intger limit and give any random value. Due to this
    //readings where not diplayed for the reading nos above 3500 in CUPPI.
    //Therefore now these values are converted into "unsigned long".
    lFilePos +=(unsigned long)(sizeof(READING))*(unsigned long)(nReadingNum);

    fseek (fpData,lFilePos,SEEK_SET); //Added by:sdt:30032001

    if (fread (&t_Readings,sizeof (READING),1,fpData) > 0)
			ValidData = TRUE;
    else
    	return NULL;
	if (t_Readings.cParam == nPno&&t_Readings.nFreq==nFreqNo)
	{
    	Data[0] = t_Readings;
        return Data;
    }
    else
    	return NULL;

}

void DispParamHeader (int *array,int *nFreqArray,int cpi)
{
	int nPno,nTChars,nCpi,i,j,l,bNewPage;
	float S,E;
	char cbuf[MAXWIDTH],ctmp[MAXWIDTH];//changed by:sdt:26032001

	for (l = 0; l<MAXLEVELS ;l++)
	{
		S = E = m_Page.fleft;
		ClearBuffer (cbuf,MAXWIDTH);
		for (i = 0;array[i]!=PARAMEND;i++)
		{
			nPno = array[i];
			switch (nPno)
			{
			case 'C' :
				S = E,E += (float)MAXCOLCHAR/cpi;
				ClearBuffer (ctmp,MAXWIDTH);
				if (l==0) //Indicates first level
					strcpy (ctmp,"Pair");
				else
					strcpy (ctmp,"\0");
				PrepareString (S,E,ctmp,cbuf,cpi,CENTER,endcount);
				break;
			case 'R' :
				S = E,E += (float)MAXCOLCHAR/cpi;
				ClearBuffer (ctmp,MAXWIDTH);
				if (l==0) //Indicates first level
					strcpy (ctmp,"Remark");
				else
					strcpy (ctmp,"\0");
				PrepareString (S,E,ctmp,cbuf,cpi,CENTER,endcount);
				break;
			default :
				for (j=0;j<m_ParamHeader[nPno].Levels[l].nNoofParts;j++)
				{
					ClearBuffer (ctmp,MAXWIDTH);
					nTChars=m_ParamHeader[nPno].Levels[l].Parts[j].nTChars;
					nCpi=m_ParamHeader[nPno].Levels[l].Parts[j].nCpi;
                    if(!l)
                    {
                        if(nMultipleFreqFlag)
                    	{
                            //changes done by:sdt:27032001:to make CR
                            //Freq. independent.
                            strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);
                            if(nPno!=SRNO_CR&&nPno!=SRNO_LR&&nPno!=SRNO_RU&&FREQUENCY)
                            {
                                strcat(ctmp,"@");
								strcat(ctmp,SpeckList[nFreqArray[i]]);
                            }
                        }
                        else
                        strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);
                    }
                    else
                    if(l==1)//Condition added by:sdt:09042001:
                    	 	//For Impedance option.
                    {
                        if(prmarray[nPno].nParamType&&IMPEDANCE)
						 {	//strcpy(ctmp,"@");
	                        //strcat(ctmp,ZchHfAray[nPno-SRNO_ATN][nFreqArray[i]]);

                            //Modified:jj:02052001
                            //sprintf(ctmp,"@%d Ohms",ZchHfAray[nPno-SRNO_ATN][nFreqArray[i]]);

                            //if(nPno==SRNO_ATN )
                            if(nPno==SRNO_ATN && ZchHfAray[0][nFreqArray[i]]!=0) //MOD30072002:jj
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[0][nFreqArray[i]]);
                            if(nPno==SRNO_NEXT)
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[1][nFreqArray[i]]);
                            if(nPno==SRNO_ELFEXT)
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[2][nFreqArray[i]]);
                            if(nPno==SRNO_INEXT)//Condition Added by:sdt:06062001
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[3][nFreqArray[i]]);
                            if(nPno==SRNO_FEXT) //Condition Added by:sdt:06062001
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[4][nFreqArray[i]]);


                         }
                         else
                         strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);
                         //Added by:sdt:13072001
                         if(((nPno==SRNO_CR)&&NORM_TEMP)||((nPno==SRNO_ATN)&&NORM_TEMP))
                         {
                            char sNorm_Temp[10];
							sprintf(sNorm_Temp," @%s�C",speckData.fNormTemp);
                         	strcat(ctmp,sNorm_Temp);
                         }
                    }
                    else
					strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);

					S=E,E+=(float)nTChars/nCpi;
					PrepareString (S,E,ctmp,cbuf,nCpi,CENTER,endcount);
				}
				break;
			}
		}
		WriteStringToFile(fpRep,cbuf,endcount);
		bNewPage = LineFeed (fpRep,1);
		if (bNewPage==YES)
		{
			PageEnd();
			PageStart();
		}
	}
}

int LineFeed (FILE *fp,int nTimes)
{
	int i;
	for (i = 0;i < nTimes;i ++)
	{
		m_Page.nCurrentln++;
		if (m_Page.nCurrentln == m_Page.nRowsPP)
			return TRUE; //Indicates Page End
		WriteStringToFile (fp,"\n",1);//07052001
	}
	return FALSE; //Indicates current page
}

void ReportEnd ()
{
   int bNewPage=FALSE;
   while (bNewPage==FALSE)
	 bNewPage=LineFeed (fpRep,1);
   PageEnd();
}


void DispRepHeader ()
{
	char szString [25][MAXHEADERSIZE] ,szData [25][MAXHEADERSIZE],cbuf[MAXWIDTH];
	int i = 0,bNewPage;
	float fss,fes,fsd,fed;  //Start & End of Data and String

    PrepareHeaderStrings (szString);
    PrepareHeaderData (szData);

    while (strcmp (szString[i],"\0") != 0)
    {
		fss = m_Page.fleft,fes = fss + STRWIDTH;
		fsd = fes ,fed = fsd + DATAWIDTH;
		ClearBuffer (cbuf,MAXWIDTH);
		while (fed < m_Page.fInchWidth-m_Page.fright &&
			   fes < m_Page.fInchWidth-m_Page.fright &&
			   strcmp (szString[i],"\0") != 0)
	{

			//Added:Start:jj:13022001
			  if(strcmpi (szString[i],"LOW FREQUENCY") == 0)
				if(!GetPrivateProfileInt("REPORT","DISPLAY_HEADER_LFREQ",0,"default.ini"))
					{
						 i++;
						continue;
					}

			  if(strcmpi (szString[i],"HIGH FREQUENCY") == 0)
               	if(!GetPrivateProfileInt("REPORT","DISPLAY_HEADER_HFREQ",0,"default.ini"))
                    {
                         i++;
                    	continue;
                    }
              //End:jj:13022001


			PrepareString (fss,fes,szString[i],cbuf,CPI17,LEFT,endcount);
			PrepareString (fsd,fed,szData[i],cbuf,CPI17,LEFT,endcount);
			fss = fed,fes = fss+STRWIDTH;
			fsd=fes,fed=fsd+DATAWIDTH;
			i ++;
		}
		WriteStringToFile (fpRep,cbuf,endcount);
		bNewPage = LineFeed (fpRep,1);
		if (bNewPage==TRUE)
		{
			PageEnd();
			PageStart();
		}
	}
	bNewPage = LineFeed (fpRep,2);
	if (bNewPage==TRUE)
	{
		PageEnd();
		PageStart();
	}
}

void PrepareHeaderStrings (char szString[40][MAXHEADERSIZE])
{
	int nStrIndex=0;
	strcpy (szString [0],"Print Date");
	strcpy (szString [1],"Test Date");
	strcpy (szString [2],m_FileInfo.ENTRY[0].szTitle);
	strcpy (szString [3],"Ambient Temp.");
	strcpy (szString [4],"Start Time");
	strcpy (szString [5],"Stop Time");
	strcpy (szString [6],"Cable Type");
	strcpy (szString [7],"Testing type");
	strcpy (szString [8],m_FileInfo.ENTRY[1].szTitle);
	strcpy (szString [9],m_FileInfo.ENTRY[2].szTitle);
	strcpy (szString [10],m_FileInfo.ENTRY[3].szTitle);
	strcpy (szString [11],m_FileInfo.ENTRY[4].szTitle);
	strcpy (szString [12],"Length");
	strcpy (szString [13],"ILM-FLM");
	strcpy (szString [14],m_FileInfo.ENTRY[5].szTitle);
	strcpy (szString [15],"Cable Gauge");
	strcpy (szString [16],"Specification No.");
	strcpy (szString [17],"Number of Quads");
	strcpy (szString [18],"Low Frequency");
	strcpy (szString [19],"High Frequency");
	strcpy (szString [20],m_FileInfo.ENTRY[6].szTitle);
	strcpy (szString [21],"Starting Fix. Pos.");
	strcpy (szString [22],"FIXTURE(S)");
	//Commented by:sdt:09102005:1305
	/*if(!nMultipleFreqFlag)
	{
		strcpy (szString [15],"LOW FREQUENCY");
		strcpy (szString [16],"HIGH FREQUENCY");
		nStrIndex = 16;
	}
	else
		nStrIndex = 14;

	for(int i=0,j=0;m_FileInfo.ENTRY[i].szTitle[0]!=NULL;i++)
	{
		//if (strcmp (strupr(m_FileInfo.ENTRY[i].szTitle),"CABLE SIZE")==0)
		//	continue;
		sprintf (szString[nStrIndex+j+1],"%s",m_FileInfo.ENTRY[i].szTitle);
		j++;
	}*/
	strcpy (szString [23],"\0");
}
void PrepareHeaderData (char szData[40][MAXHEADERSIZE])
{
    struct date d;
	int    hfStart=HALFCLOCK(m_FileInfo.StartTime),hfStop=HALFCLOCK(m_FileInfo.StopTime);
    int nStrIndex=0;
    getdate(&d);
	sprintf (szData[0],"%02d-%02d-%02d",d.da_day,d.da_mon,d.da_year);
	sprintf (szData[1],"%02d-%02d-%02d",m_FileInfo.FileOpenDate.cDay,
			m_FileInfo.FileOpenDate.cMon,m_FileInfo.FileOpenDate.nYear);
	sprintf (szData[2],"%s",m_FileInfo.ENTRY[0].szEntry);
	sprintf (szData[3],"%2.2f Degree C.",m_FileInfo.fRoomtemp);
	sprintf (szData[4],"%02d:%02d:%02d %s",
		m_FileInfo.StartTime.cHour-hfStart,
		m_FileInfo.StartTime.cMin,m_FileInfo.StartTime.cSec,NOON(m_FileInfo.StartTime));
	sprintf (szData[5],"%02d:%02d:%02d %s",
		m_FileInfo.StopTime.cHour-hfStop,
			m_FileInfo.StopTime.cMin,m_FileInfo.StopTime.cSec,NOON(m_FileInfo.StopTime));
	sprintf (szData[6],"%s",m_FileInfo.szCableType);
	strcpy (szData[7]," ");
	sprintf (szData[8],"%s",m_FileInfo.ENTRY[1].szEntry);
	sprintf (szData[9],"%s",m_FileInfo.ENTRY[2].szEntry);
	sprintf (szData[10],"%s",m_FileInfo.ENTRY[3].szEntry);
	sprintf (szData[11],"%s",m_FileInfo.ENTRY[4].szEntry);
	sprintf (szData[12],"%u meters",m_FileInfo.nLength);
	sprintf (szData[13],"%u-%u meters",m_FileInfo.nIlm,m_FileInfo.nFlm);
	sprintf (szData[14],"%s",m_FileInfo.ENTRY[5].szEntry);
	sprintf (szData[15],"%1.2f mm.",m_FileInfo.fCableGuage);
	sprintf (szData[16],"%s",speckData.specksnumber);
	sprintf (szData[17],"%d",m_FileInfo.cNoofPairs);
	sprintf (szData[18],"%s",nConfig.szLFreq);
	sprintf (szData[19],"%s",nConfig.szHFreq);
	sprintf (szData[20],"%s",m_FileInfo.ENTRY[6].szEntry);
	sprintf (szData[21],"%s",m_FileInfo.cStartFixPos);
	switch(m_FileInfo.cFixtureNo)
	{
	case 0 :
		strcpy(szData[22],"INNER");
		break;
	case 1 :
		strcpy(szData[22],"OUTER");
		break;
	case 2 :
		strcpy(szData[22],"BOTH");
		break;
	}
	//Commented by:sdt:09102005:1340
	/*if(m_FileInfo.nFileNumber > 0)
		sprintf (szData[14],"%u",m_FileInfo.nFileNumber);
	else
		strcpy (szData[14],"Untitled");

	if(!nMultipleFreqFlag)
	{
		sprintf (szData [15],"%s",nConfig.szLFreq);
		sprintf (szData [16],"%s",nConfig.szHFreq);
		nStrIndex=16;
	}
	else
		nStrIndex=14;
	for(int i=0,j=0;m_FileInfo.ENTRY[i].szTitle[0]!=NULL;i++)
	{
		//if (strcmp (strupr(m_FileInfo.ENTRY[i].szTitle),"CABLE SIZE")==0)
		//	continue;
		sprintf (szData[nStrIndex+j+1],"%s",m_FileInfo.ENTRY[i].szEntry);
		j++;
	}*/
	strcpy (szData[23],"\0");
}

void DispSummaryRep ()
{
	int nTempSpeckIndex;//Added by:sdt:07092001:0925
    SUMMARYINFO SummaryInfo;//Added by:sdt:05092001:1030
    int nIterationCount=0;//Added by:sdt:05092001:1025
    SPECKLIMIT specksLimit;//Added by:sdt:24082001
    CONFIGLIMIT configLimit;//Added by:sdt:24082001
    int nSpeckLimitIndex;//Added by:sdt:26042001
    int Speck_Passed;//Added:jj:22012001:0915
    int SumCategory[MAXPARAMETERS]; //MOD10072002

	FILE *fpText;
	int nprTemp1, ncount=0;
	int SumOpt[MAXPARAMETERS],i,nPno,cpi=CPI10,nCheck,nRem,bNewPage;
	char ctmp[MAXWIDTH],ctmp1[MAXWIDTH],cbuf[MAXWIDTH],*p;
	float fWidth=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
	float fs,fe,ftmp,fSumColWidth=fWidth/9.5;
	// 7 because there are 7 columns in the summary report
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
    memset(&specksLimit,0,sizeof(specksLimit));//Added by:sdt:24082001
    memset(&configLimit,0,sizeof(configLimit));//Added by:sdt:24082001
	Speck_Passed=GetPrivateProfileInt("REPORT","SUMM_SPEC_PASSED",0,"default.ini");//Added:jj:22012001:0920

	MemSet (SumOpt,MAXPARAMETERS,0);
    MemSet (SumCategory,MAXPARAMETERS,0); //MOD10072002

	fs=m_Page.fleft;
	//ParamBorder (fs,fWidth-0.1,'*',cpi);
	//ClearBuffer (cbuf,MAXWIDTH);
	//PrepareString(fs,fWidth-0.1,"Summary Report",cbuf,cpi,LEFT);
	//WriteStringToFile (fpRep,cbuf);
	//LineFeed(fpRep,1);
	//ParamBorder (fs,fWidth-0.1,'*',cpi);
	for (i=0;m_RepSettings->nParams[i]!=PARAMEND;i++)
	{
	  ncount=0;
	  sprintf (ctmp,"%s_SUM",strupr(prmarray[m_RepSettings->nParams[i]].szParamNameAbr));
													       		// SanNM 09/12/2000
	  GetPrivateProfileString("SUMMARY_OPTIONS",ctmp," ",cbuf,"default.ini",0); 	// from cts.ini to default.ini
	  p = strtok (cbuf,",");
	  while (p != NULL)
	  {
		if (p[0] == '1')
			SumOpt[m_RepSettings->nParams[i]] |= 1 << ncount;
		ncount++;
		p = strtok (NULL,",");
	  }
	}

////////////////MOD10072002
	for (i=0;m_RepSettings->nParams[i]!=PARAMEND;i++)
	{
	  ncount=0;
	  sprintf (ctmp,"%s",strupr(prmarray[m_RepSettings->nParams[i]].szParamNameAbr));
	  GetPrivateProfileString("SUMMARY_CATEGORIES",ctmp," ",cbuf,"default.ini",0);
	  p = strtok (cbuf,",");
	  while (p != NULL)
	  {
		if (p[0] == '1')
			SumCategory[m_RepSettings->nParams[i]] |= 1 << ncount;
		ncount++;
		p = strtok (NULL,",");
	  }
	}
////////////////

	for (i=0;m_RepSettings->nParams[i]!=PARAMEND;i++)
	{
		nPno = m_RepSettings->nParams[i];
        if(prmarray[nPno].nTestingMethod)
        {
          	nSpeckLimitIndex=0;
            nIterationCount=1;
        }
        else
        {
            //Conditions modified by:sdt:10052001
            nIterationCount=1;
        	if(nPno!=SRNO_CUPPI)
            {
            	//Commented:MOD31072002:jj
	            /*if(m_FileInfo.nPairAdjacency[nPno]!=BOTHADJ)
    	        {
        	    	nSpeckLimitIndex=m_FileInfo.nPairAdjacency[nPno];
            	}
                else*/
                {
                    //Commented by:sdt:28082001
                	/*SetLimitsForBothMode(nPno);
                    nSpeckLimitIndex=0;*/
                    //nSpeckLimitIndex=4;//Added by:sdt:28082001//Commented by:sdt:05092001:1020
                    nSpeckLimitIndex=0;//Added by:sdt:05092001:1020
                    //nIterationCount=2;
                    nIterationCount=3; //MOD08072002
                }
            }
            else
            {
            	//Commented:MOD31072002:jj
            	/*if(m_FileInfo.nUnitAdjacency[nPno]!=BOTHADJ)
    	        {
        	    	//nSpeckLimitIndex=m_FileInfo.nUnitAdjacency[nPno]+2;
                    nSpeckLimitIndex=m_FileInfo.nUnitAdjacency[nPno]+3;//MOD08072002
            	}
                else*/
                {
                    //nSpeckLimitIndex=2;//Added by:sdt:05092001:1020
                    nSpeckLimitIndex=3; //MOD08072002
                    //nIterationCount=3; //MOD08072002
                    nIterationCount=2; //MOD10072002
                }
            }
        }
        nTempSpeckIndex=nSpeckLimitIndex;
        for(int nFreqNum=0;nFreqNum<MAXFREQUENCIES;nFreqNum++)
        {

            //if(nConfig.Specks.speckparams[nPno].freq[nFreqNum])//Commented by:sdt:06042001
            //if(m_FileInfo.cParamTested[nPno][nFreqNum])//Added by:sdt:06042001//MOD02022002:03:sdt:1540
            //if(!strcmp(m_Specks.speckparams[nPno].freq[nFreqNum],"Yes")
            //	&&m_FileInfo.cParamTested[nPno][nFreqNum]) //MOD02022002:03:sdt:1540
            //start:MOD28062002:01:sdt:1515
			//if((!strcmp(m_Specks.speckparams[nPno].freq[nFreqNum],"Yes")
			//Modified by:sdt:09102005:1420
			if(((runSetup.ulParamRunSetup[nPno]&(0x1l<<(((nFreqNum+1)*3)-1)))!=0)
                ||(nPno==SRNO_CR)||(nPno==SRNO_LR)||(nPno==SRNO_RU))
                &&m_FileInfo.cParamTested[nPno][nFreqNum])
            //end:MOD28062002:01:sdt:1515
            {
            //Added by:sdt:05092001:1230
            //This will create differnt Summary report for Adjacent (pair/Unit)
            //and NonAdjacent (pair/Unit) for a given Parameter.
            nSpeckLimitIndex=nTempSpeckIndex;
            for(int nIndex=0;nIndex<nIterationCount;nIndex++,nSpeckLimitIndex++)
            {
            	//MOD10072002
            	if(!prmarray[nPno].nTestingMethod){
	            	if(!(SumCategory[nPno] & int(pow(2,nIndex))))
    	            	continue ;
                    if(nPno !=SRNO_CUPPI && m_FileInfo.nPairAdjacency[nPno]!=nIndex&&m_FileInfo.nPairAdjacency[nPno]!=2 && nIndex!=2) //Added:MOD31072002:jj
						continue ;
                }

                //Added by:sdt:05092001:1035
                //start
                if(nPno==SRNO_CUPPI)
					//SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex-2);
                    SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex-3); //MOD08072002
                else
                	SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex);
                //end
                specksLimit=FindLimits(nPno,nFreqNum,nSpeckLimitIndex);//Added by:sdt:24082001
                configLimit=CovertSpeckLimitToConfigLimit(specksLimit);//Added by:sdt:24082001

                //Commented by:sdt:29082001
                /*if(nSpeckLimitIndex==4||nSpeckLimitIndex==5)//Added by:sdt:28082001
                {
                	ModifyConfigLimit(nPno,nFreqNum,&configLimit);
                    if(configLimit.lowerlimit!=(float)BLANKVALUE)//Added by:sdt:29082001
                    {
                    	sprintf(specksLimit.lowerlimit,"%.2f",configLimit.lowerlimit);
                    }
                    else
                        strcpy(specksLimit.lowerlimit,"");

                    if(configLimit.upperlimit!=(float)BLANKVALUE)//Added by:sdt:29082001
                    {
                    	sprintf(specksLimit.upperlimit,"%.2f",configLimit.upperlimit);
                    }
                    else
                        strcpy(specksLimit.upperlimit,"");

                }*/
				ClearBuffer (cbuf,MAXWIDTH);
				fs=fe=m_Page.fleft;
				//DispSumHeader (nPno,fSumColWidth,nFreqNum);//Added by:sdt:01042001:0900
                //Modified by:sdt:05092001:1205
                DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
				if (SumOpt[nPno] & MIN)
				{
					fs=fe=m_Page.fleft;
					ClearBuffer (ctmp,MAXWIDTH);
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe,fe+=fSumColWidth;
    	        	PrepareString(fs,fe,"MIN",cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
					if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
					{
						if (nPno==SRNO_CR)    //Parameter SRNO_CR
						{
                            //Commented by:sdt:05092001
							/*if (m_FileInfo.STATRAW[nPno][nFreqNum].cMinTiporRing==TIP)
								sprintf (ctmp,"%d (T)",m_FileInfo.STATRAW[nPno][nFreqNum].cMinPair1);
							else
								sprintf (ctmp,"%d (R)",m_FileInfo.STATRAW[nPno][nFreqNum].cMinPair1);*/
                            //Added by:sdt:05092001:1045
                            if (SummaryInfo.STATRAW.cMinTiporRing==TIP)
								sprintf (ctmp,"%d (T)",SummaryInfo.STATRAW.cMinPair1);
							else
								sprintf (ctmp,"%d (R)",SummaryInfo.STATRAW.cMinPair1);
						}
						else
						//sprintf (ctmp,"%d",m_FileInfo.STATRAW[nPno][nFreqNum].cMinPair1);
                        sprintf (ctmp,"%d",SummaryInfo.STATRAW.cMinPair1);//Added by:sdt:05092001:1045
					}
					else
					//sprintf (ctmp,"%d-%d",m_FileInfo.STATRAW[nPno][nFreqNum].cMinPair1,m_FileInfo.STATRAW[nPno][nFreqNum].cMinPair2);
					//Modified by:sdt:05092001:1045
                    sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMinPair1,SummaryInfo.STATRAW.cMinPair2);
	                PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth*2;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Condtion Added by:sdt:26042001

					//strcpy (ctmp,m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].lowerlimit);
					//strcpy (ctmp,specksLimit.lowerlimit);//Modified by:sdt:24082001
					sprintf(ctmp,"%0.2f",specksLimit.lowerlimit);//Modified by:sdt:09102005:1425

        		    PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1045
					/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fMin != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fMin,
									prmarray[nPno].Decimal[0],nPno);
					else
					   nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fMin);*/
                    //Added by:sdt:05092001:1045
                    if (SummaryInfo.STATRAW.fMin != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fMin,
									prmarray[nPno].Decimal[0],nPno);
					else
					   nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fMin);


		            if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1640
        			   	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1046
					/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin,
										 prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin);*/
                    //Added by:sdt:05092001:1046
                    if (SummaryInfo.STATNORMALISED.fMin != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fMin,
										 prmarray[nPno].Decimal[0],nPno);
					else
			   			nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fMin);

	                if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1640
     	          	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					if (m_RepSettings->nRemark == TRUE)
					{
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//ftmp=m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin;
                        //Modified by:sdt:05092001:1048
                        ftmp=SummaryInfo.STATNORMALISED.fMin;
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].lowerlimit[0]!=NULL)
						//if(specksLimit.lowerlimit[0]!=NULL)//Commented by:sdt:09102005:1425:Temporary
						{
                            //nCheck=WithinSpeck(nPno,ftmp,nSpeckLimitIndex);
                            //Modified by:sdt:24082001
                            nCheck=WithinSpeck(nPno,nFreqNum,ftmp,nSpeckLimitIndex);
							if(!nCheck)
	                             PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
							else
		                         PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
						}
					}
					WriteStringToFile (fpRep,cbuf,endcount);
					bNewPage = LineFeed (fpRep,1);
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        //Modified by:sdt:05092001:1205
						DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}
				}
				if (SumOpt[nPno] & MAX)
				{
					fs=fe=m_Page.fleft;
					ClearBuffer (ctmp,MAXWIDTH);
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe,fe+=fSumColWidth;
		            PrepareString(fs,fe,"MAX",cbuf,cpi,LEFT,endcount);
		            fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
					if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
					{
						if (nPno==2)    //Parameter SRNO_CR
						{
							/*if (m_FileInfo.STATRAW[nPno][nFreqNum].cMaxTiporRing==TIP)
								sprintf (ctmp,"%d (T)",m_FileInfo.STATRAW[nPno][nFreqNum].cMaxPair1);
							else
								sprintf (ctmp,"%d (R)",m_FileInfo.STATRAW[nPno][nFreqNum].cMaxPair1);*/
							//Modified by:sdt:05092001
                            if (SummaryInfo.STATRAW.cMaxTiporRing==TIP)
								sprintf (ctmp,"%d (T)",SummaryInfo.STATRAW.cMaxPair1);
							else
								sprintf (ctmp,"%d (R)",SummaryInfo.STATRAW.cMaxPair1);
						}
						else
							//sprintf (ctmp,"%d",m_FileInfo.STATRAW[nPno][nFreqNum].cMaxPair1);
                            //Modified by:sdt:05092001:1050
                            sprintf (ctmp,"%d",SummaryInfo.STATRAW.cMaxPair1);
					}
					else
						//sprintf (ctmp,"%d-%d",m_FileInfo.STATRAW[nPno][nFreqNum].cMaxPair1,m_FileInfo.STATRAW[nPno][nFreqNum].cMaxPair2);
                        //Modified by:sdt:05092001:1050
                        sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMaxPair1,SummaryInfo.STATRAW.cMaxPair2);
		            PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth*2;
					ClearBuffer (ctmp,MAXWIDTH);
					//strcpy (ctmp,m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].upperlimit);
					//strcpy (ctmp,specksLimit.upperlimit);//commented by:sdt:09102005:1430
					sprintf(ctmp,"%0.2f",specksLimit.upperlimit);//Added by:sdt:09102005:1430
                    PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1053
					/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fMax != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fMax,
							prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fMax);*/
                    //Added by:sdt:05092001:1053
                    if (SummaryInfo.STATRAW.fMax != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fMax,
							prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fMax);

	                if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1640
      		          	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1055
					/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax,
								prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax);*/
                    //Added by:sdt:05092001:1055
                    if (SummaryInfo.STATNORMALISED.fMax != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fMax,
								prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fMax);

        	       if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1640
		               	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					if (m_RepSettings->nRemark == TRUE)
					{
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//ftmp=m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax;
                        //Modified by:sdt:05092001:1055
                        ftmp=SummaryInfo.STATNORMALISED.fMax;
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].upperlimit[0]!=NULL)
						//if(specksLimit.upperlimit[0]!=NULL)//Commented by:sdt:09102005:1430 //Temporary
						{
							//nCheck=WithinSpeck(nPno,ftmp,nSpeckLimitIndex);
                            //Modified by:sdt:24082001
                            nCheck=WithinSpeck(nPno,nFreqNum,ftmp,nSpeckLimitIndex);
							if(!nCheck)
		                         PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
							else
		                         PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
						}
					}
					WriteStringToFile (fpRep,cbuf,endcount);
					bNewPage=LineFeed (fpRep,1);
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        //Modified by:sdt:05092001:1208
                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}
				}
				if (SumOpt[nPno] & AVG)
				{
					fs=fe=m_Page.fleft;
					ClearBuffer (ctmp,MAXWIDTH);
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe,fe+=fSumColWidth;
	               PrepareString(fs,fe,"AVG",cbuf,cpi,LEFT,endcount);
					fs=fe+fSumColWidth,fe+=fSumColWidth*3; // for skipping the Pair No.
					ClearBuffer (ctmp,MAXWIDTH);
					ncount=0;
					//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow[0]!=NULL)
                    	//ncount=sprintf(ctmp,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow);
					//if (specksLimit.averagelow[0]!=NULL)//Commented by:sdt:09102005:1430
                    	ncount=sprintf(ctmp,"%4.2f",configLimit.averagelow);//Modified by:sdt:24082001
					//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi[0]!=NULL)
					//if (specksLimit.averagehi[0]!=NULL)//Commented by:sdt:09102005:1430
					{
						//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow[0]!=NULL)
						//if (specksLimit.averagelow[0]!=NULL)//Commented by:sdt:09102005:1430
							ncount+=sprintf(ctmp+ncount,"-");
							//ncount+=sprintf(ctmp+ncount,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi);
                            ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.averagehi);//Modified by:sdt:24082001
					}
					ctmp[ncount] = NULL;
	               PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1058
					/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fAvg != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fAvg,
									prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fAvg);*/
                    //Added by:sdt:05092001:1059
                    if (SummaryInfo.STATRAW.fAvg != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fAvg,
									prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fAvg);


    		        if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1641
            		   	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1100
					/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg,
													prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg);*/
                    //Added by:sdt:05092001:1100
                    if (SummaryInfo.STATNORMALISED.fAvg != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fAvg,
													prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fAvg);

          	        if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1641
		               	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					if (m_RepSettings->nRemark == TRUE)
					{
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						nCheck=TRUE;
						nRem=FALSE;
						//ftmp=m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg;
                        //Modified by:sdt:05092001:1100
                        ftmp=SummaryInfo.STATNORMALISED.fAvg;
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow[0]!=NULL)
						//if(specksLimit.averagelow[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							nRem=TRUE;
							//if(ftmp<(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow))
							if(ftmp<specksLimit.averagelow) //Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi[0]!=NULL)
						//if(specksLimit.averagehi[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							nRem=TRUE;
							//if(ftmp>(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi))
							if(ftmp>specksLimit.averagehi)//Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						if(nRem)
						{
							if(!nCheck)
			                    PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
							else
	                            PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
						}
					}
					WriteStringToFile (fpRep,cbuf,endcount);
					bNewPage=LineFeed (fpRep,1);
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        //Modified by:sdt:05092001:1208
                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}
				}
				if (SumOpt[nPno] & RMS)
				{
					fs=fe=m_Page.fleft;
					ClearBuffer (ctmp,MAXWIDTH);
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe,fe+=fSumColWidth;
                    PrepareString(fs,fe,"RMS",cbuf,cpi,LEFT,endcount);
					fs=fe+fSumColWidth,fe+=fSumColWidth*3;
					ClearBuffer (ctmp,MAXWIDTH);
					//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow[0]!=NULL)
					//	ncount=sprintf(ctmp,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow);
					//if (specksLimit.rmslow[0]!=NULL)//Commented by:sdt:09102005:1430
						ncount=sprintf(ctmp,"%4.2f",configLimit.rmslow);

					//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi[0]!=NULL)
					//if (specksLimit.rmshi[0]!=NULL)//Commented by:sdt:09102005:1430
					{
						//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow[0]!=NULL)
						//if (specksLimit.rmslow[0]!=NULL)//Commented by:sdt:09102005:1430
							ncount+=sprintf(ctmp+ncount,"-");
						//ncount+=sprintf(ctmp+ncount,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi);
                        ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.rmshi);//Modified by:sdt:24082001
					}
					ctmp[ncount] = NULL;
                    PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1105
					/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fRms != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fRms,
									prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fRms);*/
                    //Added by:sdt:05092001:1105
                    if (SummaryInfo.STATRAW.fRms != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fRms,
									prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fRms);

        		    if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1641
		               	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1106
					/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms,
									prmarray[nPno].Decimal[0],nPno);
					else
					   nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms);*/
                    //Added by:sdt:05092001:1106
                    if (SummaryInfo.STATNORMALISED.fRms != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fRms,
									prmarray[nPno].Decimal[0],nPno);
					else
					   nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fRms);

	               if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1641
		               	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

					if (m_RepSettings->nRemark == TRUE)
					{
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						nCheck=TRUE;
						nRem=FALSE;
						//ftmp=m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms;
                        //modified by:sdt:05092001:1107
                        ftmp=SummaryInfo.STATNORMALISED.fRms;
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow[0]!=NULL)
						//if(specksLimit.rmslow[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							 nRem=TRUE;
							 //if(ftmp<(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow))
							 if(ftmp<specksLimit.rmslow)//Modified by:sdt:09102005:1430
							nCheck=FALSE;
						}
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi[0]!=NULL)
						//if(specksLimit.rmshi[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							nRem=TRUE;
							//if(ftmp>(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi))
							if(ftmp>specksLimit.rmshi)//Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						if(nRem)
						{
							if(!nCheck)
			                     PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
							else
    	                          PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
						}
					}
					WriteStringToFile (fpRep,cbuf,endcount);
					//LineFeed (fpRep,1);//Commented:jj:15032001

        		    //Added:jj:15032001
		            bNewPage=LineFeed (fpRep,1);
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        //Modified by:sdt:05092001:1209
                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}
				}
				if (SumOpt[nPno] & STDDEV)
				{
					fs=fe=m_Page.fleft;
					ClearBuffer (ctmp,MAXWIDTH);
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe,fe+=fSumColWidth;
                    PrepareString(fs,fe,"STD DEV",cbuf,cpi,LEFT,endcount);
					fs=fe+fSumColWidth,fe+=fSumColWidth*3;
					ClearBuffer (ctmp,MAXWIDTH);
					//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow[0]!=NULL)
                    //	ncount=sprintf(ctmp,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow);
					//if (specksLimit.stdlow[0]!=NULL)//Commented by:sdt:09102005:1430
                    	ncount=sprintf(ctmp,"%4.2f",configLimit.stdlow);

					//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi[0]!=NULL)
					//if (specksLimit.stdhi[0]!=NULL)//Commented by:sdt:09102005:1430
					{
						//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow[0]!=NULL)
						//if (specksLimit.stdlow[0]!=NULL)//Commented by:sdt:09102005:1430
							ncount+=sprintf(ctmp+ncount,"-");
						//ncount+=sprintf(ctmp+ncount,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi);
                        ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.stdhi);
					}
					ctmp[ncount] = NULL;
					PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1110
					/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fStddev != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fStddev,
								prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fStddev);*/
                    //Added by:sdt:05092001:1110
                    if (SummaryInfo.STATRAW.fStddev != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fStddev,
								prmarray[nPno].Decimal[0],nPno);
					else
						nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fStddev);

   		            if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1641
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
                    //Commented by:sdt:05092001:1130
					/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev != 0.0)
						nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev,
									prmarray[nPno].Decimal[0],nPno);
					else
					   nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev);*/
                    //Added by:sdt:05092001:1130
                    if (SummaryInfo.STATNORMALISED.fStddev != 0.0)
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fStddev,
									prmarray[nPno].Decimal[0],nPno);
					else
					   nprTemp1 = 1;
					sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fStddev);

	               if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1641
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

      				if (m_RepSettings->nRemark == TRUE)
					{
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						nCheck=TRUE;
						nRem=FALSE;
						//ftmp=m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev;
                        //Modified by:sdt:05092001:1130
                        ftmp=SummaryInfo.STATNORMALISED.fStddev;
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow[0]!=NULL)
						//if(specksLimit.stdlow[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							 nRem=TRUE;
							 //if(ftmp<(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow))
							 if(ftmp<specksLimit.stdlow)//Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi[0]!=NULL)
						//if(specksLimit.stdhi[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							nRem=TRUE;
							//if(ftmp>(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi))
							if(ftmp>specksLimit.stdhi)//Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						if(nRem)
						{
							if(!nCheck)
								PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
							else
								PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
						}
					}
					WriteStringToFile (fpRep,cbuf,endcount);
					//LineFeed (fpRep,1);//Commented:jj:15032001

		            //Added:jj:15032001
		            bNewPage=LineFeed (fpRep,1);
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        //Modified by:sdt:07092001:1210
                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}
				}
				if(SumOpt[nPno] & MEAN_STDDEV)//Added:SanNM 09/12/2000 of PS thru Default.ini
                    //Commented by:sdt:01042001
			        //if (nPno==7||nPno==90||nPno==8||nPno==9)//SRNO_NEXT,SRNO_FEXT,SRNO_ELFEXT,SRNO_INEXT
                    //Condition modified by:sdt:01042001:for Zch.
                    if (nPno==SRNO_ELFEXT||nPno==SRNO_NEXT||nPno==SRNO_FEXT||nPno==SRNO_INEXT)//Conition Modified by:sdt:14092001
					{
						fs=fe=m_Page.fleft;
						ClearBuffer (ctmp,MAXWIDTH);
						ClearBuffer (cbuf,MAXWIDTH);
						fs=fe,fe+=fSumColWidth;
						PrepareString(fs,fe,"P.S",cbuf,cpi,LEFT,endcount);

                        //MOD0507002:01:jj
						//fs=fe,fe+=fSumColWidth*2;
                        fs=fe+fSumColWidth,fe+=fSumColWidth*3;

						ClearBuffer (ctmp,MAXWIDTH);

	                    ////////Added MOD0507002:01:jj
						//if (specksLimit.psumlow[0]!=NULL)//Commented by:sdt:09102005:1430
							ncount=sprintf(ctmp,"%4.2f",configLimit.psumlow);

						//if (specksLimit.psumhi[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							//if (specksLimit.psumlow[0]!=NULL)//Commented by:sdt:09102005:1430
								ncount+=sprintf(ctmp+ncount,"-");
	                        ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.psumhi);
						}
						ctmp[ncount] = NULL;
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						/////////////////////

						//if (nConfig.Specks.speckparams[nPno].stdlow!=BLANKVALUE)
						//sprintf(ctmp,"%4.2f",nConfig.Specks.speckparams[nPno].stdlow);


                        //Commented:MOD0507002:01:jj
						/*PrepareString(fs,fe,"",cbuf,cpi,LEFT,endcount);

						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//if (nConfig.Specks.speckparams[nPno].stdhi!=BLANKVALUE)
						//sprintf(ctmp,"%4.2f",nConfig.Specks.speckparams[nPno].stdhi);
						PrepareString(fs,fe,"",cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;*/
						ClearBuffer (ctmp,MAXWIDTH);
						//////pranav
                        //Commented by:sdt:05092001:1130
						/*nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fPowerSum,
								prmarray[nPno].Decimal[0],nPno);
			      		if ( nprTemp1 == 0.0 )
							nprTemp1 = 1;
					    sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fPowerSum);*/
                        //Added by:sdt:05092001:1130
                        nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fPowerSum,
								prmarray[nPno].Decimal[0],nPno);
			      		if ( nprTemp1 == 0.0 )
							nprTemp1 = 1;
					    sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fPowerSum);
/////////////////////
//pr//			sprintf (ctmp,"%4.2f",m_FileInfo.STATRAW[nPno].fStddev);
						if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:jj:14032001
							PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);

//////pranav
                        //Commented by:sdt:05092001:1135
						/*nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fPowerSum,
								prmarray[nPno].Decimal[0],nPno);
		  				if ( nprTemp1 == 0 )
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fPowerSum);*/
                        //Added by:sdt:05092001:1135
                        nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fPowerSum,
								prmarray[nPno].Decimal[0],nPno);
		  				if ( nprTemp1 == 0 )
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fPowerSum);
/////////////////////
//pr//			sprintf (ctmp,"%4.2f",m_FileInfo.STATNORMALISED[nPno].fStddev);
						if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:jj:14032001
							PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

	                    //////////Added MOD0507002:01:jj
    	  				if (m_RepSettings->nRemark == TRUE)
						{
							fs=fe,fe+=fSumColWidth;
							ClearBuffer (ctmp,MAXWIDTH);
							nCheck=TRUE;
							nRem=FALSE;
    	                    ftmp=SummaryInfo.STATNORMALISED.fPowerSum;
							//if(specksLimit.psumlow[0]!=NULL)//Commented by:sdt:09102005:1430
							{
								 nRem=TRUE;
								 if(ftmp<specksLimit.psumlow)//Modified by:sdt:09102005:1430
									nCheck=FALSE;
							}
							//if(specksLimit.psumhi[0]!=NULL)//Commented by:sdt:09102005:1430
							{
								nRem=TRUE;
								if(ftmp>specksLimit.psumhi)//Modified by:sdt:09102005:1430
									nCheck=FALSE;
							}
							if(nRem)
							{
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
									PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
							}
						}
						///////////////////

						WriteStringToFile (fpRep,cbuf,endcount);
							bNewPage=LineFeed (fpRep,1);
							if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                            //Modified by:sdt:05092001:1210
                            DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
	            //Added by:sdt:08052001
				//if (SumOpt[nPno] & WPN)
                if ((SumOpt[nPno] & WPN) && nIndex==2) //MOD08072002
				{
                	float worstComb=0.0 ; //Added:MOD23072002:jj
					fs=fe=m_Page.fleft;
					ClearBuffer (ctmp,MAXWIDTH);
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe,fe+=fSumColWidth;
					PrepareString(fs,fe,"1%W",cbuf,cpi,LEFT,endcount);

                    //MOD23072002:jj
					//fs=fe,fe+=fSumColWidth*2;
                    fs=fe+fSumColWidth,fe+=fSumColWidth*3;

					ClearBuffer (ctmp,MAXWIDTH);

                    ////////Added MOD2307002:jj
					//if (specksLimit.wpnlow[0]!=NULL)//Commented by:sdt:09102005:1430
       	            	ncount=sprintf(ctmp,"%4.2f",configLimit.wpnlow);
					//if (specksLimit.wpnhi[0]!=NULL)//Commented by:sdt:09102005:1430
					{
						//if (specksLimit.wpnlow[0]!=NULL)//Commented by:sdt:09102005:1430
							ncount+=sprintf(ctmp+ncount,"-");
                        ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.wpnhi);
					}
					ctmp[ncount] = NULL;
					PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
					/////////////////////

                    //Commented:MOD23072002:jj
					/*PrepareString(fs,fe,"",cbuf,cpi,LEFT,endcount);

					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);
					PrepareString(fs,fe,"",cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);*/

                    if(nPno==SRNO_ELFEXT)
                    {
					nprTemp1 = FindNewDeciNo(Worstpairnext.Elfext[nFreqNum][0],
								prmarray[nPno].Decimal[0],nPno);
                    }
                    if(nPno==SRNO_NEXT)
                    {
                    nprTemp1 = FindNewDeciNo(Worstpairnext.Onext[nFreqNum][0],
								prmarray[nPno].Decimal[0],nPno);
                    }
                    if(nPno==SRNO_FEXT)//Condition added by:sdt:06062001
                    {
					nprTemp1 = FindNewDeciNo(Worstpairnext.Fext[nFreqNum][0],
								prmarray[nPno].Decimal[0],nPno);
                    }
                    if(nPno==SRNO_INEXT)//Condition added by:sdt:06062001
                    {
                    nprTemp1 = FindNewDeciNo(Worstpairnext.Inext[nFreqNum][0],
								prmarray[nPno].Decimal[0],nPno);
                    }
			      	if ( nprTemp1 == 0.0 )
						nprTemp1 = 1;

                    if(nPno==SRNO_ELFEXT)
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Elfext[nFreqNum][0]);
                    if(nPno==SRNO_NEXT)
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Onext[nFreqNum][0]);
                    if(nPno==SRNO_FEXT)//Condition added by:sdt:06062001
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Fext[nFreqNum][0]);
                    if(nPno==SRNO_INEXT)//Condition added by:sdt:06062001
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Inext[nFreqNum][0]);

					if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
					fs=fe,fe+=fSumColWidth;
					ClearBuffer (ctmp,MAXWIDTH);

                    if(nPno==SRNO_ELFEXT)
						nprTemp1 = FindNewDeciNo(Worstpairnext.Elfext[nFreqNum][1],
							prmarray[nPno].Decimal[0],nPno);
                    if(nPno==SRNO_NEXT)
						nprTemp1 = FindNewDeciNo(Worstpairnext.Onext[nFreqNum][1],
							prmarray[nPno].Decimal[0],nPno);
                    if(nPno==SRNO_FEXT)//Condition added by:sdt:06062001
						nprTemp1 = FindNewDeciNo(Worstpairnext.Fext[nFreqNum][1],
							prmarray[nPno].Decimal[0],nPno);
                    if(nPno==SRNO_INEXT)//Condition added by:sdt:06062001
						nprTemp1 = FindNewDeciNo(Worstpairnext.Inext[nFreqNum][1],
							prmarray[nPno].Decimal[0],nPno);


	  				if ( nprTemp1 == 0 )
						nprTemp1 = 1;
                    if(nPno==SRNO_ELFEXT){
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Elfext[nFreqNum][1]);
                        worstComb=Worstpairnext.Elfext[nFreqNum][1] ; //Added:MOD23072002:jj
                        }

                    if(nPno==SRNO_NEXT){
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Onext[nFreqNum][1]);
                        worstComb=Worstpairnext.Onext[nFreqNum][1] ;//Added:MOD23072002:jj
                        }

                    if(nPno==SRNO_FEXT){//Condition added by:sdt:06062001
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Fext[nFreqNum][1]);
                        worstComb=Worstpairnext.Fext[nFreqNum][1] ; //Added:MOD23072002:jj
                        }

                    if(nPno==SRNO_INEXT){//Condition added by:sdt:06062001
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Inext[nFreqNum][1]);
                        worstComb=Worstpairnext.Inext[nFreqNum][1] ;//Added:MOD23072002:jj
                        }

					if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

                    //////////Added MOD2307002:jj
   	  				if (m_RepSettings->nRemark == TRUE)
					{
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						nCheck=TRUE;
						nRem=FALSE;
   	                    ftmp=worstComb;
						//if(specksLimit.wpnlow[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							 nRem=TRUE;
							 if(ftmp<specksLimit.wpnlow)//Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						//if(specksLimit.wpnhi[0]!=NULL)//Commented by:sdt:09102005:1430
						{
							nRem=TRUE;
							if(ftmp>specksLimit.wpnhi)//Modified by:sdt:09102005:1430
								nCheck=FALSE;
						}
						if(nRem)
						{
							if(!nCheck)
								PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
							else
								PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
						}
					}
					///////////////////

					WriteStringToFile (fpRep,cbuf,endcount);
					bNewPage=LineFeed (fpRep,1);
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        //Modified by:sdt:05092001:1210
                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}
				}
				ClearBuffer (ctmp,MAXWIDTH);
				ClearBuffer (ctmp1,MAXWIDTH);
				ClearBuffer (cbuf,MAXWIDTH);
				if (prmarray[nPno].nTestingMethod==SINGLEPAIR)
				{
					if (nPno == 2) // for SRNO_CR
					{
						strcpy (ctmp,"No of Conductor(s) Tested :");
						strcpy (ctmp1,"No of Conductor(s) Passed :");
					}
					else
					{
						strcpy (ctmp,"No of Pair(s) Tested :");
						strcpy (ctmp1,"No of Pair(s) Specks Passed :");
					}
				}
				else
				{
					strcpy (ctmp,"No of Combination(s) Tested :");
					strcpy (ctmp1,"No of Combination(s) Passed :");//Uncommented:jj:14032001
				}
				fs=m_Page.fleft,fe=fs+fSumColWidth*3;
				PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
				fs=fe,fe+=fSumColWidth;
				ClearBuffer(ctmp,MAXWIDTH);
				sprintf (ctmp,"%d",m_FileInfo.nReadingsOk[nPno][nFreqNum]+m_FileInfo.nReadingsNotOk[nPno][nFreqNum]);
				PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
				WriteStringToFile (fpRep,cbuf,endcount);
				bNewPage=LineFeed (fpRep,1);
				ClearBuffer (cbuf,MAXWIDTH);
				if (bNewPage == YES)
				{
					PageEnd();
					PageStart();
					//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                    //Modified by:sdt:05092001:1210
                    DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
				}
					// Commented to remove No. of Combinations Passed for RPG
					//fs=m_Page.fleft,fe=fs+fSumColWidth*3;
					//PrepareString (fs,fe,ctmp1,cbuf,cpi,LEFT);
					//fs=fe,fe+=fSumColWidth;
					//sprintf (ctmp,"%d",m_FileInfo.nReadingsOk[nPno]);
					//PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT);
					//WriteStringToFile (fpRep,cbuf);
					//bNewPage=LineFeed (fpRep,1);
					//if (bNewPage == YES)
					//{
						//PageEnd();
						 //PageStart();
					//DispSumHeader (nPno,fSumColWidth);
					//}
				//Added:Start:jj:22012001:0925
       	        fs=m_Page.fleft,fe=fs+fSumColWidth*3;
                PrepareString (fs,fe,ctmp1,cbuf,cpi,LEFT,endcount);
				fs=fe,fe+=fSumColWidth;
				sprintf (ctmp,"%d",m_FileInfo.nReadingsOk[nPno][nFreqNum]);
				PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

    			if(Speck_Passed)
				{
					WriteStringToFile (fpRep,cbuf,endcount);
					bNewPage=LineFeed (fpRep,1);
		        }
				if (bNewPage == YES)
				{
					PageEnd();
					PageStart();
					//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                    //Modified by:sdt:05092001:1210
                    DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
				}//End:jj:22012001:0925
			}
            }
    	}
	}
//pr//	ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
	ParamBorder(m_Page.fleft,m_Page.fInchWidth-0.1,'-',cpi);
////////////////////////
	fs=m_Page.fleft,fe=fs+fSumColWidth*3;
	ClearBuffer (cbuf,MAXWIDTH);

     if ( m_FileInfo.cPairsBypassed[0]!=127 )//Added:pnb:14122000:1825
	{                  //Should not display Bypass if pairs are not actually bypassed.
		PrepareString (fs,fe,"Bypassed Pair Number(s) : ",cbuf,cpi,LEFT,endcount);
		strcpy (ctmp," ");
		for (i = 0;(int)m_FileInfo.cPairsBypassed[i]!=127;i++)
		{
			sprintf (ctmp1,"%d ",m_FileInfo.cPairsBypassed[i] - m_FileInfo.cStartFixPos + 1);
			strcat (ctmp,ctmp1);
		}
		fs = fe,fe=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
		PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
		WriteStringToFile (fpRep,cbuf,endcount);
		LineFeed (fpRep,1);
		ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
     }

	ClearBuffer (cbuf,MAXWIDTH);

     //Start:jj:22012001:1700
     if(nConfig.cBreak=='Y')
     {
          ClearBuffer (ctmp,MAXWIDTH);

	     fs=m_Page.fleft,fe=fs+fSumColWidth*3;
	     PrepareString (fs,fe,"Break Test :",cbuf,cpi,LEFT,endcount);

	     if(nConfig.BSResult==0x00 || nConfig.BSResult==0x02)//BREAK_NOTOK
	     	sprintf(ctmp,"Not OK");
	     else
	     	sprintf(ctmp,"OK");
	     fs = fe,fe=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
	     PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

		WriteStringToFile (fpRep,cbuf,endcount);
		LineFeed (fpRep,1);
          ClearBuffer (cbuf,MAXWIDTH);
          ClearBuffer (ctmp,MAXWIDTH);
     }

     if(nConfig.cShort=='Y')
     {
          ClearBuffer (ctmp,MAXWIDTH);

	     fs=m_Page.fleft,fe=fs+fSumColWidth*3;
	     PrepareString (fs,fe,"Short Test :",cbuf,cpi,LEFT,endcount);

	     if(nConfig.BSResult == 0x00 || nConfig.BSResult == 0x01)//SHORT_NOTOK
	     	sprintf(ctmp,"Not OK");
	     else
	     	sprintf(ctmp,"OK");
		fs = fe,fe=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
	     PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

		WriteStringToFile (fpRep,cbuf,endcount);
		LineFeed (fpRep,1);
          ClearBuffer (cbuf,MAXWIDTH);
          ClearBuffer (ctmp,MAXWIDTH);
     }
     //End:jj:22012001:1700


	fs=m_Page.fleft,fe=m_Page.fInchWidth-(fs+m_Page.fright);
	ParamBorder (fs,fe-0.2,'*',cpi);
	fs=m_Page.fleft,fe=m_Page.fInchWidth-(fs+m_Page.fright);
	PrepareString (fs,fe,"End of Report",cbuf,cpi,CENTER,endcount);
	WriteStringToFile (fpRep,cbuf,endcount);
	LineFeed (fpRep,1);

	if(Text1)//Added:pnb:260452000
    {
	   LineFeed (fpRep,1);
	   fpText=fopen("Text1.txt","r");
	   if(fpText==NULL)
			exit(1);
	   while(fgets(cbuf,80,fpText)!=NULL)
		{
			WriteStringToFile (fpRep,cbuf,endcount);
		  LineFeed (fpRep,1);
        }
        if(fpText){fclose(fpText);}
    }
    if(Text2)//Added:pnb:260452000
    {
        LineFeed (fpRep,1);
        fpText=fopen("Text2.txt","r");
        if(fpText==NULL)
       		exit(1);
        while(fgets(cbuf,80,fpText)!=NULL)
		{
			WriteStringToFile (fpRep,cbuf,endcount);
        	LineFeed (fpRep,1);
        }
        if(fpText){fclose(fpText);}
    }
    if(Text3)//Added:pnb:260452000
    {
        LineFeed (fpRep,1);
	   fpText=fopen("Text3.txt","r");
        if(fpText==NULL)
       		exit(1);
	   while(fgets(cbuf,80,fpText)!=NULL)
		{
			WriteStringToFile (fpRep,cbuf,endcount);
        	LineFeed (fpRep,1);
        }
        if(fpText){fclose(fpText);}
	}
}

//void DispSumHeader (int nPno,float fColWidth,int nFreqNum)
//Modified by:sdt:1205
void DispSumHeader (int nPno,float fColWidth,int nFreqNum,int nSwitchMode)
{
	char path[80];//22012001
	float fs,fe,fWidth;
	char ctmp[MAXWIDTH],cbuf[MAXWIDTH];
	int cpi = CPI10,bNewPage, i;

	if (m_Page.nCurrentln + 7 >= m_Page.nRowsPP) //5 lines are required for Summ Header & for footer
	{
		PageEnd();
		PageStart();
	}
	fWidth=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
	ParamBorder(m_Page.fleft,fWidth-0.1,'-',cpi);
	ClearBuffer (cbuf,MAXWIDTH);
	ClearBuffer (ctmp,MAXWIDTH);
	fs=fe=m_Page.fleft;

    //Condition Added by:sdt:01042001
    if(nPno !=SRNO_CR&& nPno!=SRNO_RU&&nPno!=SRNO_LR)
    {
        //Commented by:sdt:30082001
		//sprintf(ctmp, "SUMMARY:%s At Freq. %s.", prmarray[nPno].szParamName,SpeckList[nFreqNum]);
        //Following Code Added by:sdt:30082001.
        //Summary header become very long with string "At Freq." .
		//Now this text was going on next line.
        //Also the Impedence values to be added in the summary header.

        sprintf(ctmp, "SUMMARY:%s ", prmarray[nPno].szParamName);
        i = 0;
		while(ctmp[i] != NULL)
		{
			ctmp[i] = toupper(ctmp[i]);
			i++;
		}
		PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,cpi,LEFT,endcount);
        WriteStringToFile (fpRep,cbuf,endcount);

        //Added by:sdt:05092001:1155
        //start
        //This code will add the switching information in the Summary of
        //Parameter.
        bNewPage=LineFeed(fpRep,1);
		if (bNewPage==YES)
		{
			PageEnd();
			PageStart();
		}
        ClearBuffer (cbuf,MAXWIDTH);
        fs=fe=m_Page.fleft;
    	if(!prmarray[nPno].nTestingMethod)
        {
            sprintf(ctmp,"For %s",LimitTypes[nSwitchMode]);
	        PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,cpi,LEFT,endcount);
    	    WriteStringToFile (fpRep,cbuf,endcount);

	        bNewPage=LineFeed(fpRep,1);
			if (bNewPage==YES)
			{
				PageEnd();
				PageStart();
			}
	        ClearBuffer (cbuf,MAXWIDTH);
    	    fs=fe=m_Page.fleft;
	    }
        //end
        if(nPno==SRNO_ATN){
        	if(ZchHfAray[0][nFreqNum]!=0) //Added:MOD30072002:jj
	    	   	sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[0][nFreqNum]);
            else
	            sprintf(ctmp,"At Freq. %s. ",SpeckList[nFreqNum]);
        }
        else
	    if(nPno==SRNO_NEXT)
    	   	sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[1][nFreqNum]);
        else
	    if(nPno==SRNO_ELFEXT)
    	   	sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[2][nFreqNum]);
        else
	    if(nPno==SRNO_INEXT)
    	   	sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[3][nFreqNum]);
        else
	    if(nPno==SRNO_FEXT)
    	   	sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[4][nFreqNum]);
        else
     		sprintf(ctmp,"At Freq. %s. ",SpeckList[nFreqNum]);
    }
    else
		sprintf(ctmp, "SUMMARY:%s.", prmarray[nPno].szParamName);

	i = 0;
	while(ctmp[i] != NULL)
	{
		ctmp[i] = toupper(ctmp[i]);
		i++;
	}
	PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,cpi,LEFT,endcount);
	WriteStringToFile (fpRep,cbuf,endcount);
	bNewPage=LineFeed(fpRep,1);
	if (bNewPage==YES)
	{
		PageEnd();
		PageStart();
	}
	ClearBuffer (cbuf,MAXWIDTH);
	fs=fe,fe+=fColWidth;
	//PrepareString (fs,fe,"Limit",cbuf,cpi,CENTER);
	fs=fe,fe+=fColWidth;
     PrepareString (fs,fe,"PAIR",cbuf,cpi,LEFT,endcount);
	fs=fe,fe+=fColWidth*2;
     //PrepareString (fs,fe,"S",cbuf,cpi,LEFT);
	//fs=fe,fe+=fColWidth;
     PrepareString (fs,fe,"SPEC VAL",cbuf,cpi,LEFT,endcount);
	fs=fe,fe+=fColWidth*2;
     //PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,LEFT);

     //Start:Added:pnb:14122000:1800
 	if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED )
     	PrepareString (fs,fe-0.5,"MEAS VAL",cbuf,cpi,RIGHT,endcount);
     if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED )
		PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,LEFT,endcount);
     if(m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH )
          PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,CENTER,endcount);
	if(m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM )
     	PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,LEFT,endcount);
     //End:14122000:1800

	if (m_RepSettings->nRemark==YES)
	{
	  fs=fe,fe+=fColWidth;
       PrepareString (fs,fe,"Remark",cbuf,cpi,LEFT,endcount);
	}
	WriteStringToFile (fpRep,cbuf,endcount);
	bNewPage=LineFeed(fpRep,1);
	if (bNewPage==YES)
	{
		PageEnd();
		PageStart();
	}
	ClearBuffer (cbuf,MAXWIDTH);
	fs=m_Page.fleft+fColWidth,fe=fs+fColWidth;
     //PrepareString (fs,fe,"No",cbuf,cpi,CENTER);
	fs=fe,fe+=fColWidth*2;
	sprintf (ctmp,"%s",prmarray[nPno].szUnit);
     PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
	fs=fe,fe+=fColWidth;
	sprintf (ctmp,"%s/L",prmarray[nPno].szUnit);

     if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1805
     	PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

	fs=fe,fe+=fColWidth;
     sprintf(path,"%s:\\cts.ini",RamDrv);//jj:22012001
	if ( GetPrivateProfileInt("BASIC_INFO",
		"NORMALISE_KM",0,path) )
		sprintf (ctmp,"%s/Km",prmarray[nPno].szUnit);
	else if ( atof(speckData.fNormLength) == (float)0
		   || atof(speckData.fNormLength) == (float)1000 )
		sprintf (ctmp,"%s/Km",prmarray[nPno].szUnit);
	else
		sprintf (ctmp,"%s/NL=%s",prmarray[nPno].szUnit,
						speckData.fNormLength );

     if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1805
     	PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

	WriteStringToFile (fpRep,cbuf,endcount);
	bNewPage=LineFeed(fpRep,1);
	if (bNewPage==YES)
	{
		PageEnd();
		PageStart();
	}
	//ParamBorder (m_Page.fleft,fWidth-0.1,'-',cpi);
}

void ParamBorder (float fs,float fWidth,char ch,int cpi)
{
	char cbuf[MAXWIDTH],ctmp[MAXWIDTH];

	ClearBuffer (cbuf,MAXWIDTH);
	ClearBuffer (ctmp,MAXWIDTH);
	RepeatChar (ch,fWidth*cpi,ctmp);
	PrepareString (fs,fWidth,ctmp,cbuf,cpi,LEFT,endcount);
	WriteStringToFile (fpRep,cbuf,endcount);
	bNewPage=LineFeed(fpRep,1);//Added bNewPage :pnb:27121999
	if (bNewPage==TRUE)// Added:pnb:27121999
	{
		PageEnd();
		PageStart();
	}
}



int WithinSpeck (int nParam,int nFreqNum,float fTmp,int nAdjacency)
{
	 float nLower,nUpper;
	 int  nRet=TRUE;
     SPECKLIMIT specksLimit;//Added by:sdt:24082001
     memset(&specksLimit,0,sizeof(SPECKLIMIT));//Added by:sdt:24082001
     //Commented by:sdt:30082001
     /*if(nParam!=SRNO_CUPPI)//Condition Added by:sdt:24082001
     {
     	specksLimit=FindLimits(nParam,nFreqNum,nAdjacency);//Added by:sdt:24082001
     }
     else
     {
     	specksLimit=FindLimits(nParam,nFreqNum,nAdjacency+2);//Added by:sdt:24082001
     }*/
     specksLimit=FindLimits(nParam,nFreqNum,nAdjacency);//Added by:sdt:30082001
	 fTmp=(float)fabs((double)fTmp);

	 //nLower=SpeckValue(specksLimit.lowerlimit);//Commented by:sdt:09102005:1430
	 //nUpper=SpeckValue(specksLimit.upperlimit);//Commented by:sdt:09102005:1430
	 nLower=specksLimit.lowerlimit;//Modified by:sdt:09102005:1430
	 nUpper=specksLimit.upperlimit;//Modified by:sdt:09102005:1430
     //Condition Commented by:sdt:24082001
     /*if(nParam!=SRNO_CUPPI)
     {
		 nLower=SpeckValue(m_Specks.speckparams[nParam].specklimits[nAdjacency].lowerlimit);
		 nUpper=SpeckValue(m_Specks.speckparams[nParam].specklimits[nAdjacency].upperlimit);

     }
     else
     {
		 nLower=SpeckValue(m_Specks.speckparams[nParam].specklimits[nAdjacency+2].lowerlimit);
		 nUpper=SpeckValue(m_Specks.speckparams[nParam].specklimits[nAdjacency+2].upperlimit);

     }*/

	 if(nLower!=(float)BLANKVALUE)
	 {
			if(fTmp<nLower)
				 nRet=FALSE;
	 }
	 if(nUpper!=(float)BLANKVALUE)
	 {
	  if(fTmp>nUpper)
		 nRet=FALSE;
   }
   return nRet;
}


float SpeckValue(char *string)
{
	float fTemp;

	if(string[0]==NULL)
		fTemp=(float)BLANKVALUE;
	else
		fTemp=atof(string);
	return fTemp;
}

int FindNewDeciNo(float fFlotVal,int nNo,int nParamNo)
{
  int nLen;
  char szRaw[15];

  nLen = strlen(itoa(fFlotVal,szRaw,10));
	if ( prmarray[nParamNo].Decimal[1] == 0)
	{
	nNo = 0;
	return(nNo);
	}

	if (nLen < nNo)
	 nNo = nNo - nLen;
	else
	 nNo = 0;
	if (fFlotVal == 0.0)
		nNo=0;
 return(nNo);
}

//Added by:sdt:23032001
void DisplayMatrix (int *nGrParams)
{
	int i,nSetStart ,nSetEnd ,nCharAvail ,nCharReqr ,nTChar;
	float fTArea;
	static int row = 0,col = 0;
    int temprow=0,tempcol=0;
//    int nFreqNo=0;
    int nCharUsedCount=0;
	fTArea = m_Page.fInchWidth - (m_Page.fleft+m_Page.fright);
	nTChar = nCharAvail = fTArea * CPI17; // Calculation for data is done wrt 17 cpi

	if (m_RepSettings->nRemark == YES)
		nCharAvail-=REMARKCOLSZ; // Remark will be displayed in 17 cpi
	nDispMatrix[row][col++] = 'C';
    nFreqMatrix[row][col-1] = 'C';//nFreqNo-1;//Changed by:sdt:28032001
	nCharAvail -= MAXCOLCHAR;

	for (i = 0;nGrParams [i] != PARAMEND; i++)
	{
		nCharReqr = AreainChar(nGrParams[i]);
       // for(int j=0;j<nConfig.Specks.speckparams[nGrParams[i]].nNumofFreqSelected;
       //             j++)

        for(int j=0;j<MAXFREQUENCIES;j++)
        {
            //Added by:sdt:25032001

            //if(nConfig.Specks.speckparams[nGrParams [i]].freq[j]==1)//commented by:sdt:06042001
            //if(m_FileInfo.cParamTested[nGrParams [i]][j])//Added by:sdt:06042001//MOD02022002:01:sdt:1245
            //if(!strcmp(m_Specks.speckparams[nGrParams [i]].freq[j],"Yes")
			//	&&m_FileInfo.cParamTested[nGrParams [i]][j])//MOD02022002:01:sdt:1245
            //start:MOD28022002:01:sdt:1515
			//if((!strcmp(m_Specks.speckparams[nGrParams [i]].freq[j],"Yes")
			//Modified by:sdt:09102005:1420
			if(((runSetup.ulParamRunSetup[nPno]&(0x1l<<(((nFreqNum+1)*3)-1)))!=0)
                ||(i==SRNO_CR)||(i==SRNO_LR)||(i==SRNO_RU))
				&&m_FileInfo.cParamTested[nGrParams [i]][j])
            //end:MOD28022002:01:sdt:1515
            {
				if (nCharReqr < nCharAvail)
				{
				    nDispMatrix[row][col++] = nGrParams [i];
            	    nFreqMatrix[row][col-1] = j;//nFreqNo-1;
					nCharAvail -= m_ParamHeader[nGrParams [i]].nTChars;
					nCharReqr -=m_ParamHeader[nGrParams [i]].nTChars;
				}
                //This case added by:sdt:27032001
                //To Group all frequencies of a single parameter in one group
                else
                //if(nCharReqr > nCharAvail &&(nCharAvail == nTChar-REMARKCOLSZ-MAXCOLCHAR-nCharUsedCount)
                //Modified by:sdt:14082001
                if(nCharReqr > nCharAvail &&(nCharAvail == nTChar-MAXCOLCHAR-nCharUsedCount
                -(m_RepSettings->nRemark == YES?REMARKCOLSZ:0))
				   &&(nCharAvail-m_ParamHeader[nGrParams [i]].nTChars>0))
                {
					//Commented by:sdt:27032001
                	//if(nCharAvail-m_ParamHeader[nGrParams [i]].nTChars>0)
                    //{
                    	nDispMatrix[row][col++] = nGrParams [i];
	            	    nFreqMatrix[row][col-1] = j;//nFreqNo-1;
						nCharAvail -= m_ParamHeader[nGrParams [i]].nTChars;
						nCharReqr -=m_ParamHeader[nGrParams [i]].nTChars;
                    	nCharUsedCount +=m_ParamHeader[nGrParams [i]].nTChars;
                   // }
                }
				else
				{
					nSetStart = 0,nSetEnd = col;
					//nCharReqr = nTChar-nCharAvail-REMARKCOLSZ;
                    //temprow=row;tempcol=col;
                    RepeatSet (nSetStart,nSetEnd,nFreqMatrix[temprow],&tempcol,nCharAvail,nCharReqr);
					nCharAvail=RepeatSet (nSetStart,nSetEnd,nDispMatrix[row],&col,nCharAvail,nCharReqr);
                    //Commented by :sdt:27032001

                     nCharUsedCount=0; //added by:sdt:27032001
                     if (m_RepSettings->nRemark == YES)//Added by:sdt:14082001
                     {
                     	nDispMatrix[row][col++]='R'; //added by:sdt:27032001
                     	nFreqMatrix[row][col-1]='R';
                     }
                     row++;col=0; //added by:sdt:27032001

    		        if (nGrParams[i] != PARAMEND)
        		    {
					    nCharAvail = fTArea * CPI17; // Calculation for data is done wrt 17 cpi

						if (m_RepSettings->nRemark == YES)
	        				nCharAvail-=REMARKCOLSZ; // Remark will be displayed in 17 cpi
						nDispMatrix[row][col++] = 'C';
        	            nFreqMatrix[row][col-1] = 'C';//j;Changed by:sdt:28032001
						nCharAvail -= MAXCOLCHAR;
	        	        nDispMatrix[row][col++] = nGrParams [i];
    	                nFreqMatrix[row][col-1] = j;
	            	    nCharReqr -=m_ParamHeader[nGrParams [i]].nTChars;
						nCharAvail -= m_ParamHeader[nGrParams [i]].nTChars;
                        //Added by:sdt:03042001
                        nCharUsedCount +=m_ParamHeader[nGrParams [i]].nTChars;
	              	}
				}
                //Added by:sdt:11082001
                //This will show the readings for the parameter ONEXT,
				// for one frequency in one row.

            	if(nGrParams [i]==SRNO_NEXT)
                {

                    if(MoreFreqSelected(nGrParams [i],j)&&j<MAXFREQUENCIES)//Condition Modified by:sdt:12082001
                    {
	                	nSetStart = 0,nSetEnd = col;
                        if (m_RepSettings->nRemark == YES) //Condition Added by:sdt:14082001
					    	nCharReqr = nTChar-nCharAvail-REMARKCOLSZ;//Added by:sdt:10042001
                        else
                        	nCharReqr = nTChar-nCharAvail;
        	            temprow=row;tempcol=col;
					    RepeatSet (nSetStart,nSetEnd,nFreqMatrix[temprow],&tempcol,nCharAvail,nCharReqr);
						nCharAvail=RepeatSet (nSetStart,nSetEnd,nDispMatrix[row],&col,nCharAvail,nCharReqr);
                        if (m_RepSettings->nRemark == YES) //Condition Added by:sdt:14082001
                        {
	                    	nDispMatrix[row][col++]='R'; //added by:sdt:27032001
						    nFreqMatrix[row][col-1]='R';
                        }
					    row++;col=0;//added by:sdt:27032001

						nCharAvail = fTArea * CPI17;
                    	if (m_RepSettings->nRemark == YES)
	        				nCharAvail-=REMARKCOLSZ;
						nDispMatrix[row][col++] = 'C';
    	    	        nFreqMatrix[row][col-1] = 'C';
						nCharAvail -= MAXCOLCHAR;
                    }

                }
            }
        }
	}
	nSetStart = 0,nSetEnd = col;
	//nCharReqr = nTChar-nCharAvail;//Commented by:sdt:10042001
    if (m_RepSettings->nRemark == YES) //Condition Added by:sdt:14082001
    	nCharReqr = nTChar-nCharAvail-REMARKCOLSZ;//Added by:sdt:10042001
    else
    	nCharReqr = nTChar-nCharAvail;
    temprow=row;tempcol=col;
    RepeatSet (nSetStart,nSetEnd,nFreqMatrix[temprow],&tempcol,nCharAvail,nCharReqr);
	nCharAvail=RepeatSet (nSetStart,nSetEnd,nDispMatrix[row],&col,nCharAvail,nCharReqr);

    if (m_RepSettings->nRemark == YES) //Condition Added by:sdt:14082001
    {
	    nDispMatrix[row][col++]='R'; //added by:sdt:27032001
    	nFreqMatrix[row][col-1]='R';
    }
    row++;col=0;//added by:sdt:27032001

}

//added by:sdt:23032001
float AreainChar (int nPno)
{
	return (float)(m_ParamHeader[nPno].nTChars*nConfig.Specks.speckparams[nPno].nNumofFreqSelected) ;
}


int CheckForMultipleFreq(void)
{
	int nFlag=1;
    HFFreqSelected=0;
    LFFreqSelected=0;
    for(int i=0;i<MAXPARAMETERS-1;i++)//Changed by:sdt:08042001:for Zchms
    {
    	nFlag|=nConfig.Specks.speckparams[i].nNumofFreqSelected;
    }
    if(nFlag>1)
    	return 1;
    else
    {
        int FreqSelected[MAXFREQUENCIES];
        memset(FreqSelected,0,sizeof(FreqSelected));

        //Modified:jj:02052001
        //for(i=SRNO_CM;i<=SRNO_CUPP;i++)
        for(i=Param.Lf_Param_Start;i<=Param.Lf_Param_End;i++)
        {
        	for(int j=0;j<MAXFREQUENCIES;j++)
            {
            	if(nConfig.Specks.speckparams[i].freq[j])
                 	FreqSelected[j] = 1;
            }
        }
        int Count=0;
        for(i=0;i<MAXFREQUENCIES;i++)
        {
        	if(FreqSelected[i])
            {
                LFFreqSelected=i;
            	Count++;
            }
        }
        if(Count>1)
        	return 1;

        memset(FreqSelected,0,sizeof(FreqSelected));

        //Modified:jj:02052001
        for(i=Param.Hf_Param_Start;i<=Param.Hf_Param_End;i++)
        {
        	for(int j=0;j<MAXFREQUENCIES;j++)
            {
            	if(nConfig.Specks.speckparams[i].freq[j])
                 	FreqSelected[j] = 1;
            }
        }
        Count=0;
        for(i=0;i<MAXFREQUENCIES;i++)
        {
        	if(FreqSelected[i])
            {
                HFFreqSelected=i;
            	Count++;
            }
        }
        if(Count>1)
        	return 1;
    }
    return 0;
}

//Added by:sdt:02042001
void InitialiseReportPrm(int nPno)
{
    strcpy(prmarray[nPno].szParamName,paramarray->szParamName);
	strcpy(prmarray[nPno].szParamNameAbr,paramarray->szParamNameAbr);
	strcpy(prmarray[nPno].szUnit,paramarray->szUnit);
	prmarray[nPno].nParamType=paramarray->nParamType;
	prmarray[nPno].nTestingMethod=paramarray->nTestingMethod;  //(pair to pair /single pair)
	prmarray[nPno].nReadings = paramarray->nReadings;
	strcpy(prmarray[nPno].szReadingNames,paramarray->szReadingNames);
    for(int j=0;j<2;j++)
    {
		prmarray[nPno].Decimal[0]=paramarray->Decimal[0];
		prmarray[nPno].Decimal[1]=paramarray->Decimal[1];
    }
}

//function added by:sdt:03042001
//Function added to allocate the DispMatrix array Dynamically.
void AllocateDispmatrix(void)
{
    int nCharAvail ,nCharReqr;
    float fTArea;


    fTArea = m_Page.fInchWidth - (m_Page.fleft+m_Page.fright);
	nCharAvail = fTArea * CPI17;

	if (m_RepSettings->nRemark == YES)
		nCharAvail-=REMARKCOLSZ; // Remark will be displayed in 17 cpi
	nCharAvail-=MAXCOLCHAR;//for displaying pair number.
    for(int i=0;i<MAXPARAMETERS-1;i++)//Changed by:sdt:08042001:for Zchms
    {
    	//nCharReqr = AreainChar(i);
        //nArraySize+=int(nCharReqr/nCharAvail)+1;

        //Condition Added by:sdt:17012002 //MOD17012002:01:sdt::1255
        //For ONEXT, to display the Individual Powersum reading for every
		//frequency is displayed separately.
        if(i!=SRNO_NEXT)
        {
	    	nCharReqr = AreainChar(i);
    	    nArraySize+=int(nCharReqr/nCharAvail)+1;
        }
        else
        {
        	nArraySize+=nConfig.Specks.speckparams[i].nNumofFreqSelected;
        }

    }
   	nDispMatrix = (int **)farmalloc(sizeof(int *)*nArraySize);
   	nFreqMatrix = (int **)farmalloc(sizeof(int *)*nArraySize);
	for(i=0;i<nArraySize;i++)
    {
    	nDispMatrix[i] = (int *)farmalloc(sizeof(int)*(MAXWIDTH/MAXCOLCHAR));
        nFreqMatrix[i] = (int *)farmalloc(sizeof(int)*(MAXWIDTH/MAXCOLCHAR));
    }
	for(i=0;i<nArraySize;i++)
    {
 		MemSet (nDispMatrix[i],(MAXWIDTH/MAXCOLCHAR),PARAMEND);
	    MemSet (nFreqMatrix[i],(MAXWIDTH/MAXCOLCHAR),PARAMEND);
    }
}



//Function Added by:sdt:11082001
//This will done the Intermediate Calculation of Ind. PowerSum
//For pairs present in passed structure.
void IntermediatePsum(READING *t_Reading)
{
	ptrIpsum[t_Reading->cPair1-1].RawVal +=pow(10,((t_Reading->fRawVal*(-1))/10));
	ptrIpsum[t_Reading->cPair1-1].NorVal +=pow(10,((t_Reading->fNormVal*(-1))/10));
    ptrIpsum[t_Reading->cPair2-1].RawVal +=pow(10,((t_Reading->fRawVal*(-1))/10));
	ptrIpsum[t_Reading->cPair2-1].NorVal +=pow(10,((t_Reading->fNormVal*(-1))/10));
}

//Function added by:sdt:11082001
//This will caculate Power Sum for Total no. of pairs
void CalFinalIndPoweSum(void)
{
    for(int i=0;i<m_FileInfo.cNoofPairs;i++)
    {
    	if(ptrIpsum[i].RawVal!=0)
        {
        	ptrIpsum[i].RawVal=abs(10*log10(ptrIpsum[i].RawVal));
        }
        if(ptrIpsum[i].NorVal!=0)
        {
        	ptrIpsum[i].NorVal=abs(10*log10(ptrIpsum[i].NorVal));
        }
    }
}

//Function Added by:sdt:11082001
//This function will display Detailed Individual Power sum &
//Average Ind. PowSum for every Unit.
void DisplayPowerSum(int index,int AvgIndPsumFlag)//Added by:sdt:11082001
{
    char cBuf[MAXWIDTH],cTmp[MAXWIDTH],cTmp1[20];
    //int nStrlen1,nRawLen,nNorLen,TotalLen;
    int nCharReq,nReadperRow=0,nCharAvail,nReading;
    float fTArea,fs,fe;
    IndPowSum AvgIndPsum; //Added by:sdt:12082001
    int nAvgCount=0;
    //nStrlen1=strlen("IND PSUM for Pair #")+3;
    //nRawLen =MAXCOLCHAR,nNorLen=MAXCOLCHAR;
    //TotalLen=nStrlen1+nRawLen+nNorLen;

    //Write header for Ind Pow Sum.
    //start
    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
    sprintf(cBuf,"Individual Power Sum for %s at Frequency %s",
				 prmarray[nDispMatrix[index][1]].szParamNameAbr,
                 SpeckList[nFreqMatrix[index][1]]);
    WriteStringToFile (fpRep,cBuf,endcount);
    ClearBuffer (cBuf,MAXWIDTH);
    bNewPage = LineFeed (fpRep,1);
	if (bNewPage == YES)
	{
		 PageEnd();
		 PageStart();
    }
    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
    //End

    //Create param Header for IND pow sum.
    //start
    for(int i=0;i<MAXPARAMETERS;i++)
    {
    	if(m_RepSettings->nParams[i]==SRNO_NEXT)
        {
        	break;
        }
    }
    fTArea = m_Page.fInchWidth - (m_Page.fleft+m_Page.fright);
	nCharAvail = fTArea * CPI17; // Calculation for data is done wrt 17 cpi

    //if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)//Commented by:sdt:13082001
    if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)//Modified by:sdt:13082001
    {
    	nCharReq = MAXCOLCHAR*3;
    }
    else
    {
     	nCharReq = MAXCOLCHAR*2;
    }

	if (m_RepSettings->nRemark == YES)
		nCharAvail-=REMARKCOLSZ;

    fs=m_Page.fleft;
	fe=m_Page.fleft+((float)MAXCOLCHAR)/CPI17;
    while(nCharAvail>=nCharReq)
    {
        nReadperRow++;
       	sprintf(cTmp,"Pair");
	    PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);
        fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
    	//if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)//Commented:by:sdt:13082001
        if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH) //Modified by:sdt:13082001
	    {
			sprintf(cTmp,"%s",RdTypeHeader(SRNO_NEXT,1));//Commented by:sdt:12082001
            //sprintf(cTmp,"%s",RdTypeHeader(i,1));
        	PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);
            fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
            sprintf(cTmp,"%s",RdTypeHeader(SRNO_NEXT,2));//Commented by:sdt:12082001
            //sprintf(cTmp,"%s",RdTypeHeader(i,2));
        	PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);
        }
        else
        {
        	sprintf(cTmp,"%s",RdTypeHeader(SRNO_NEXT,m_RepSettings->nNormUnnorm[SRNO_NEXT]));//Modified by:sdt:13082001
        	PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);
        }
    	nCharAvail-=nCharReq;
        fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
    }
    WriteStringToFile (fpRep,cBuf,endcount);
    ClearBuffer (cBuf,MAXWIDTH);
    bNewPage = LineFeed (fpRep,1);
    if (bNewPage == YES)
	{
		 PageEnd();
		 PageStart();
    }
    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);

    //End
    //Displaying Values of Ind Pow sum.
    //start

	nReading=0;
    while(nReading<m_FileInfo.cNoofPairs)
    {
        fs=m_Page.fleft;
	    fe=m_Page.fleft+((float)MAXCOLCHAR)/CPI17;
        for(int j=0;j<nReadperRow&&nReading<m_FileInfo.cNoofPairs;j++)
        {
        	if(!(nReading%m_FileInfo.cUnitof))
	        {
                if(fs!=m_Page.fleft)
                {
                	WriteStringToFile (fpRep,cBuf,endcount);
	                ClearBuffer (cBuf,MAXWIDTH);
    	            bNewPage = LineFeed (fpRep,1);
    		        if (bNewPage == YES)
					{
						 PageEnd();
						 PageStart();
	    			}
                }
                //Below If Condition Added to Print the Average
                //Ind. PowerSum for each unit.
                if(nReading!=0&&AvgIndPsumFlag==TRUE)//Added by:sdt:12082001
                {
                    AvgIndPsum.RawVal/=nAvgCount;
                    AvgIndPsum.NorVal/=nAvgCount;
                    //if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)//Commented by:sdt:13082001
                    if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)//Modified by:sdt:13082001
                    {
                       	sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf",(nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal);
                        sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,1));
                        strcat(cTmp,cTmp1);
                        sprintf(cTmp1,"    %.2lf",AvgIndPsum.NorVal);
                        strcat(cTmp,cTmp1);
                        sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,2));
                        strcat(cTmp,cTmp1);
						//%s %.2lf %s.",
						//			  (nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal,RdTypeHeader(SRNO_NEXT,1),
                        //              AvgIndPsum.NorVal,RdTypeHeader(SRNO_NEXT,2));
                    }
                    else
                    {
                    	//if(m_RepSettings->nNormUnnorm[i]==SUM_UNNORMALISED)//Commented by:sdt:13082001
                        if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_UNNORMALISED)//Modified by:sdt:13082001
                        {
                            //Commented by:sdt:12082001
                        	//sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf %s .",
							//		  (nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal,RdTypeHeader(SRNO_NEXT,1));
                            sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf ",
                            			  (nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal);
                            sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,1));
                        	strcat(cTmp,cTmp1);
                        }
                        else
                        {   //Commented by:sdt:12082001
                        	//sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf %s ",
							//		  (nReading/m_FileInfo.cUnitof),AvgIndPsum.NorVal,RdTypeHeader(SRNO_NEXT,2));
                            sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf ",
                            			  (nReading/m_FileInfo.cUnitof),AvgIndPsum.NorVal);
                            sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,2));
                        	strcat(cTmp,cTmp1);
                        }
                    }
                    PrepareString (m_Page.fleft,m_Page.fInchWidth-m_Page.fright,cTmp,cBuf,CPI17,LEFT,endcount);
		            WriteStringToFile (fpRep,cBuf,endcount);
    	            ClearBuffer (cBuf,MAXWIDTH);
        	        bNewPage = LineFeed (fpRep,1);
    	    	    if (bNewPage == YES)
					{
						 PageEnd();
						 PageStart();
    				}
                }
                AvgIndPsum.RawVal = 0.0;
                AvgIndPsum.NorVal = 0.0;
                nAvgCount=0;

    	    	sprintf(cTmp,"Unit #%d",(nReading/m_FileInfo.cUnitof)+1);
        	    PrepareString (m_Page.fleft,m_Page.fInchWidth-m_Page.fright,cTmp,cBuf,CPI17,CENTER,endcount);
	            WriteStringToFile (fpRep,cBuf,endcount);
                j=0;//MOD18012002:01:sdt::1105
                fs=m_Page.fleft; //MOD18012002:01:sdt::1105
			    fe=m_Page.fleft+((float)MAXCOLCHAR)/CPI17;//MOD18012002:01:sdt::1105
                ClearBuffer (cBuf,MAXWIDTH);
                bNewPage = LineFeed (fpRep,1);
    	        if (bNewPage == YES)
				{
					 PageEnd();
					 PageStart();
    			}
        	}
            if((ptrIpsum[nReading].RawVal!=0.0)||(ptrIpsum[nReading].NorVal!=0.0))
            {
	            sprintf(cTmp,"%d",nReading+1);
    	        PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);

    	        //if(m_RepSettings->nNormUnnorm[i]==SUM_UNNORMALISED||m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
                //Modified by:sdt:13082001
                if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_UNNORMALISED||m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
	            {
                    fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
		            sprintf(cTmp,"%.2lf",ptrIpsum[nReading].RawVal);
	    	        PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);
    	        }
	            //fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
    	        //if(m_RepSettings->nNormUnnorm[i]==SUM_NORMALISED||m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
                //Modified by:sdt:13082001
                if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_NORMALISED||m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
	            {
                    fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
		            sprintf(cTmp,"%.2lf",ptrIpsum[nReading].NorVal);
	    	        PrepareString (fs,fe,cTmp,cBuf,CPI17,CENTER,endcount);
    	        }
        	    fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;

                AvgIndPsum.RawVal += ptrIpsum[nReading].RawVal; //Added by:sdt:12082001
                AvgIndPsum.NorVal += ptrIpsum[nReading].NorVal; //Added by:sdt:12082001
                nAvgCount++; //Added by:sdt:12082001
            }
            nReading++;
        }
        WriteStringToFile (fpRep,cBuf,endcount);
        ClearBuffer (cBuf,MAXWIDTH);
        bNewPage = LineFeed (fpRep,1);
	    if (bNewPage == YES)
		{
			 PageEnd();
		 	PageStart();
    	}
        if(nReading>m_FileInfo.cNoofPairs)
        	break;
    }
    //Following Code will Print Average Ind Power Sum For Last Unit
    //start
    if(AvgIndPsumFlag==TRUE)
    {
	    AvgIndPsum.RawVal/=nAvgCount;
    	AvgIndPsum.NorVal/=nAvgCount;
	    //if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
        //modified by:sdt:13082001
        if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
        {
          	sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf",(nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal);
            sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,1));
            strcat(cTmp,cTmp1);
            sprintf(cTmp1,"    %.2lf",AvgIndPsum.NorVal);
            strcat(cTmp,cTmp1);
            sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,2));
            strcat(cTmp,cTmp1);
						//%s %.2lf %s.",
						//			  (nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal,RdTypeHeader(SRNO_NEXT,1),
                        //              AvgIndPsum.NorVal,RdTypeHeader(SRNO_NEXT,2));
        }
        else
        {
           	//if(m_RepSettings->nNormUnnorm[i]==SUM_UNNORMALISED)
            //Modified by:sdt:13082001
            if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_UNNORMALISED)
            {
                            //Commented by:sdt:12082001
                        	//sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf %s .",
							//		  (nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal,RdTypeHeader(SRNO_NEXT,1));
            	sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf ",
                   			  (nReading/m_FileInfo.cUnitof),AvgIndPsum.RawVal);
                sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,1));
                strcat(cTmp,cTmp1);
            }
            else
            {   //Commented by:sdt:12082001
                        	//sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf %s ",
							//		  (nReading/m_FileInfo.cUnitof),AvgIndPsum.NorVal,RdTypeHeader(SRNO_NEXT,2));
            	sprintf(cTmp,"Average Individual Power Sum For Unit #%d = %.2lf ",
                   			  (nReading/m_FileInfo.cUnitof),AvgIndPsum.NorVal);
                sprintf(cTmp1," %s",RdTypeHeader(SRNO_NEXT,2));
                strcat(cTmp,cTmp1);
            }
        }
	    PrepareString (m_Page.fleft,m_Page.fInchWidth-m_Page.fright,cTmp,cBuf,CPI17,LEFT,endcount);
		WriteStringToFile (fpRep,cBuf,endcount);
	    ClearBuffer (cBuf,MAXWIDTH);
	    bNewPage = LineFeed (fpRep,1);
	    if (bNewPage == YES)
		{
			PageEnd();
			PageStart();
	    }
    }
    //End

}

//Function Added by:sdt:12082001
//This will any next frequency selcted for the given parameter.
int MoreFreqSelected(int nParamNo,int nFreqCount)
{
	for(int i=nFreqCount+1;i<MAXFREQUENCIES;i++)
    {
    	if(m_FileInfo.cParamTested[nParamNo][i])
        {
        	return TRUE;
        }
    }
	return FALSE;
}


//Function is Added by:sdt:24082001
//This function will return the limits for given parameter , at given
//Frequency and given Limit Index.
SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex)
{
    FILE *lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
    sprintf(szSpeckFile,"%s:\\replimit.bin",RamDrv);
    lFp=fopen(szSpeckFile,"rb");
    //fseek(lFp,sizeof(SPECKLIMIT)*nParam*nFreq*4,SEEK_SET);//Commented by:sdt:22082001
    fseek(lFp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*MAXLIMITTYPES)+(nFreq*MAXLIMITTYPES)),SEEK_SET);
    fseek(lFp,sizeof(SPECKLIMIT)*(nLimitIndex),SEEK_CUR);
	fread(&specksLimit, sizeof (SPECKLIMIT), 1, lFp);
    if(lFp){fclose(lFp);}
    return specksLimit;
}

//Function Added by:sdt:24082001
//this function will convert limits in the character format to float format.

CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit)
{
	CONFIGLIMIT configLimit;
	//Commented by:sdt:09102005:1435
	/*configLimit.lowerlimit=strcmp(specksLimit.lowerlimit,"")?atof(specksLimit.lowerlimit):BLANKVALUE;
	configLimit.upperlimit=strcmp(specksLimit.upperlimit,"")?atof(specksLimit.upperlimit):BLANKVALUE;
	configLimit.averagelow=strcmp(specksLimit.averagelow,"")?atof(specksLimit.averagelow):BLANKVALUE;
	//configLimit.averagehi=strcmp(specksLimit.averagelow,"")?atof(specksLimit.averagehi):BLANKVALUE;
    //MOD17012002:03:sdt::1455
    //Here first "specksLimit.averagehi" variable was bymistakly written as
    //"specksLimit.averagelow"
    configLimit.averagehi=strcmp(specksLimit.averagehi,"")?atof(specksLimit.averagehi):BLANKVALUE;
	configLimit.stdlow=strcmp(specksLimit.stdlow,"")?atof(specksLimit.stdlow):BLANKVALUE;
	configLimit.stdhi=strcmp(specksLimit.stdhi,"")?atof(specksLimit.stdhi):BLANKVALUE;
	configLimit.rmslow=strcmp(specksLimit.rmslow,"")?atof(specksLimit.rmslow):BLANKVALUE;
	configLimit.rmshi=strcmp(specksLimit.rmshi,"")?atof(specksLimit.rmshi):BLANKVALUE;

    //MOD0507002:01:jj
	configLimit.psumlow=strcmp(specksLimit.psumlow,"")?atof(specksLimit.psumlow):BLANKVALUE;
	configLimit.psumhi=strcmp(specksLimit.psumhi,"")?atof(specksLimit.psumhi):BLANKVALUE;

    //MOD23072002:jj
	configLimit.wpnlow=strcmp(specksLimit.wpnlow,"")?atof(specksLimit.wpnlow):BLANKVALUE;
	configLimit.wpnhi=strcmp(specksLimit.wpnhi,"")?atof(specksLimit.wpnhi):BLANKVALUE;*/
	configLimit = specksLimit;//Added by:sdt:09102005:1435

    return configLimit;
}


//Added by:sdt:05092001:0937
//This function will give summary for given parameter at given frequency
//with given Adjacency.
SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType)
{
	char path[MAXPATH];
    FILE *fpSummary;
    SUMMARYINFO SummaryInfo;
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
    sprintf(path,"%s:\\repsum.bin",RamDrv);
    fpSummary=fopen(path,"rb");
    fseek(fpSummary,sizeof(SUMMARYINFO)*((nParam*MAXFREQUENCIES*MAXSUMMARYTYPES)+(nFreqNum*MAXSUMMARYTYPES)),SEEK_SET);
    fseek(fpSummary,sizeof(SUMMARYINFO)*(nSumType),SEEK_CUR);
    fread(&SummaryInfo, sizeof(SUMMARYINFO), 1, fpSummary);
    if(fpSummary){fclose(fpSummary);}
    return SummaryInfo;
}

//Added by:sdt:09092001:1130
//This function will check for passes string should not be empty.
int BufNotEmpty(char *cbuf)
{
	if(cbuf==NULL)
    	return 0;
    else
    {
    	for(int i=0;i<MAXWIDTH;i++)
        {
        	if((cbuf[i]!=' ')&&(cbuf[i]!='\0'))
            	return 1;
        }
    }
    return 0;
}


