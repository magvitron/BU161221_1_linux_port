#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <ctype.h>
#include <bios.h>
#include "uses.h"

#include "prnrep.h"
//#include "utils.c"

#include "PRODINI.CPP"

//#include "prodini.cpp" //Added by:sdt:08082001
#include "disputl.cpp"
//#include "

int NonNullSpeck=0; //Added by:sdt:07112006:2130 //For End Remark
int OkSpeck=0;		//Added by:sdt:07112006:2130 //For End Remark

void DisplayIndMatrix (int nGrParams, int nFreqNum, int nNormIndex);

STRUCT_SPECKS speckData;

//char   	szOriginalParams[MAXLEN_SELECT_PARAM];//commented by:Sdt:16102005:2310
//Added by:sdt:09092001:1605
int BufNotEmpty(char *cbuf);
//Added by:sdt:09092001:1605
SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
char LimitTypes[5][30]={"Intra-Adjacent Pairs","Intra-Nonadjacent Pairs","All Combinations","Adjacent Units","NonAdjacent Units"}; //MOD10072002

//Added by:sdt:29082001
SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex);
CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit);

//Added by:sdt:12082001
IndPowSum far *ptrIpsum;
void IntermediatePsum(READING *t_Reading);
void CalFinalIndPoweSum(void);
void DisplayPowerSum(int index,int AvgIndPsumFlag);
int MoreFreqSelected(int nParamNo,int nFreqCount);


//Added by:sdt:08082001
BOOL	EndOfReport;
void dec (int argc, char *argv1,char *argv2,char *argv3);
void FindMainExeVersion(char *MainExeVer);
//Added by:sdt:06062001
//WORSTPAIRNEXT Worstpairnext;
//Modified by:sdt:04022007:1615
float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];


//Added by:sdt:09052001
//void SetLimitsForBothMode(int Param);//Commented by:sdt:30082001

//Added by:sdt:08052001::Commented by:sdt:06062001
//float WorstElfext[MAXFREQUENCIES][2];
//float WorstOnext[MAXFREQUENCIES][2];

PARAM_DETAILS Param;//02052001
int endcount;

int IMPEDENCE,FREQUENCY;//Added by:sdt:10042001
int NORM_TEMP,NORM_LENGTH;//Added by:sdt:13072001
int ZchHfAray[5][MAXFREQUENCIES];//Added by:sdt:10042001::modified by:sdt:06062001
int CPI; //Added by:sdt:08042001
char cRepChar;//Added by:sdt:03042001
int CurrentRepType=0;//Added by:sdt:03042001
int nArraySize=0;//Added by:sdt:02042001
//Added by:sdt:02042001
REPORT_PRM  far * prmarray;//[MAXPARAMETERS];
int nMultipleFreqFlag;
int HFFreqSelected,LFFreqSelected;
char SpeckList[MAXFREQUENCIES][10];//by:sdt:24032001
char RamDrv[2]; //jj:22012001
int 					Text1,Text2,Text3;//Added:pnb:27052000
int						bNewPage;
//int 					Unit1=0,Unit2=0,UnitDiff=0;//Commented by:sdt:09092001
//SPECIFICATION           m_Specks;//commented by:Sdt:16102005:2310
INFORMATION             far     m_FileInfo;
//READING                 m_Reading;

//PARAMETER               parameterarray[MAXPARAMETERS];
CONFIG                  nConfig;
CTPAIR                  CtPair;
Page					m_Page;
FILE                    *fpPrn ,*fpData;
ParamHeader             far m_ParamHeader[MAXPARAMETERS];
REPSETINGS              *m_RepSettings;
RunSetup runSetup;//Added by:sdt:16102005:2310
//Modified by:sdt:02042001
PRM						far *paramarray;//[MAXPARAMETERS];


long int	lStart;// Tells from where the actual data begins
int			nTempUnitOf,nLastUnitSize,nUnits,nNeedOfAppend;

//Modified by:sdt:03042001
int			far **nDispMatrix;
int 		far **nFreqMatrix;
int 		far **nNormUnnormMatrix; //Added by:sdt:16102005:2330

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

int main (int argc,char *argv[])
{
	int argcnt;
    char path[MAXPATH];//Added by:sdt:30082001
    EndOfReport = FALSE; //Added by:sdt:08082001
    nMultipleFreqFlag=0;
	argc+=0;
    //int bDraftMode;//Commented by:sdt:21062001
	if ( argc == 1 )
		return 0;

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


	/*m_RepSettings = (REPSETINGS *)malloc(sizeof(REPSETINGS));

	m_RepSettings->nSumm_Detailed=0;
	m_RepSettings->nRemark=1;
	m_RepSettings->nNormUnnorm[0]=2;
	m_RepSettings->nNormUnnorm[1]=2;
	m_RepSettings->nNormUnnorm[2]=2;
	m_RepSettings->nNormUnnorm[3]=2;
	m_RepSettings->nNormUnnorm[4]=2;
	m_RepSettings->nNormUnnorm[5]=2;
	m_RepSettings->nNormUnnorm[6]=2;
	m_RepSettings->nNormUnnorm[7]=2;
	m_RepSettings->nNormUnnorm[8]=2;
	m_RepSettings->nNormUnnorm[9]=2;
	m_RepSettings->nNormUnnorm[10]=2;
	m_RepSettings->nNormUnnorm[11]=2;
	m_RepSettings->nNormUnnorm[12]=2;
	m_RepSettings->nNormUnnorm[13]=2;
	m_RepSettings->nNormUnnorm[14]=3;
	m_RepSettings->nGroupRep = 0;
	m_RepSettings->nParams[0]=1;
	m_RepSettings->nParams[1]=2;//PARAMEND;
	m_RepSettings->nParams[2]=3;//PARAMEND;
	m_RepSettings->nParams[3]=4;//PARAMEND;
	m_RepSettings->nParams[4]=5;//PARAMEND;
	m_RepSettings->nParams[5]=6;//PARAMEND;
	m_RepSettings->nParams[6]=7;//PARAMEND;
	m_RepSettings->nParams[7]=8;//PARAMEND;
	m_RepSettings->nParams[8]=9;//PARAMEND;
	m_RepSettings->nParams[9]=10;//PARAMEND;
	m_RepSettings->nParams[10]=PARAMEND;
	m_RepSettings->nParams[11]=PARAMEND;
	m_RepSettings->nParams[12]=PARAMEND;
	m_RepSettings->nParams[13]=PARAMEND;
	m_RepSettings->nParams[14]=PARAMEND;
	RamDrv[0]='g';//22012001
	RamDrv[1]='\0';*/

	argcnt=5;//Added:pnb:26052000
	//Commented by:sdt:16102005:2315
	//if(strcmp(argv[argcnt],"Y")&&strcmp(argv[argcnt],"N"))//Added:pnb:26052000
	//	argcnt++;

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
	if ((fpPrn = fopen (argv[3],"wt")) == NULL)
	{
		fprintf (stderr,"Cannot open the destination file %s\n", argv[3]);
		getch ();
		exit (0);
	}
	RamDrv[0]=argv[4][0];//jj:22012001
	RamDrv[1]='\0';//jj:22012001

	//Added by:sdt:02042001
	prmarray = (REPORT_PRM *)farmalloc(sizeof(REPORT_PRM)*MAXPARAMETERS);


    //Commented:jj:21052001
    //ReadFreqList();

    //Commented by:sdt:20062001
    /*bDraftMode=SelectPrinterMode();
    if(bDraftMode)
    {
    	CPI = 12;
    }
    else
    	CPI = 17;
	PageSetup();
    SetPageLength ();//Added by:sdt:03042001*/
    int nConMode=PrinterSetup(); //added by:sdt:20062001
	ReadBasicData ();
	PageStart ();
	DisplaySetup ();
	nMultipleFreqFlag=CheckForMultipleFreq();
	DispRepHeader ();
	if (m_RepSettings->nSumm_Detailed == SUM_DETAILED)
	{
        if(nConMode)//bDraftMode)//Modified by:sdt:21062001
		{
            //Condensed Mode
            SelectCondensed (1);//Added by:sdt:03042001
        }
        else
        {
            SelectCondensed (0);//Added by:sdt:08042001
	        SelectDraft();//Added by:sdt:08042001
        }
		DispDetailedReport();
		ReportEnd();
		PageStart();

        //SelectCondensed (0);//Added by:sdt:03042001//commented by:sdt:08042001
	}
	m_RepSettings->nRemark = 1;//Commented:jj:14032001 remark made selectable through menu for summary also
    SelectCondensed (0);//Added by:sdt:03042001
   	DispSummaryRep();
    EndOfReport = TRUE; //Added by:sdt:08082001
	ReportEnd();
    biosprint (1,0x00,0);
	FileClose ();
    farfree(prmarray);//Added by:sdt:02042001
	sprintf(path,"%s:\\replimit.bin",RamDrv);//Added by:sdt:29082001
    remove(path);//Added by:sdt:29082001
    sprintf(path,"%s:\\repsum.bin",RamDrv);//Added by:sdt:05092001:0935
    remove(path);//Added by:sdt:05092001:0935
return 0;

}

void FileClose ()
{
	if(fpPrn){fclose(fpPrn);}
	if(fpData){fclose(fpData);}
}

void PageSetup(char *PrintType)
{
		PageSize(PrintType);//Modified by:sdt:21062001
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

//=====================================================================
//	Modifiications
//	1. Date :-20062001
//		Page Height & Page width is now read from the Disprep.ini insteaf
//		of Default.ini.
//=====================================================================

void PageSize (char *PrintType)
{
	char szStr[20]; // To get float data Items
    //Commented by:sdt:20062001
	/*GetPrivateProfileString("PAGE","HEIGHT","0",szStr,"default.ini",0);
	m_Page.fInchHeight = atof (szStr);
	GetPrivateProfileString("PAGE","WIDTH","0",szStr,"default.ini",0);*/
    char path[80];
    sprintf(path,"%s:\\disprep.ini",RamDrv); //Added by:sdt:20062001
	GetPrivateProfileString(PrintType,"PAGE_HEIGHT","0",szStr,path,0); //Modified by:sdt:20062001
	m_Page.fInchHeight = atof (szStr);
	GetPrivateProfileString(PrintType,"PAGE_WIDTH","0",szStr,path,0);  //Modified by:sdt:20062001
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
                //Added by:sdt:03042001
                SelectElite(1);
    			SelectCondensed(0);

				while(!feof(fp))
				{
				cCh=fgetc(fp);
				if ( cCh == '\n' )
					{
                        fputc(cCh,fpPrn);
                    	m_Page.nCurrentln++;
					}///LineFeed (fpPrn,1);
				else
					fputc(cCh,fpPrn);
				}
				if(fp){fclose(fp);}
                //Added by:sdt:03042001
                SelectElite(0);
    			SelectShrink (0);
			}
			else
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szHStr,cBuf,CPI,m_Page.bHAllignment,endcount);
               cBuf[80] = '\0';
			//WriteStringToFile (fpPrn,cBuf);//Commented:pnb:12112000:17:25
			LineFeed (fpPrn,1);
		}
	}
}


void PageEnd ()
{
	char cBuf [MAXWIDTH];
    //Added by:sdt:03042001
    int i=m_Page.nCurrentln;

	for (i=m_Page.nCurrentln;i<=m_Page.nRowsPP;i++)
	{
		//fputc(10,fpPrn); //Commented by:sdt:09042001
        fputc(13,fpPrn);
        fputc(10,fpPrn); //Added by:sdt:21062001
		m_Page.nCurrentln++;
	}

	ClearBuffer (cBuf,MAXWIDTH);
	if (m_Page.bFooter == YES)
	{
        //Below condition is Added for printing version number at
        //End of the report.
        if(EndOfReport == FALSE)//Condition Added by:sdt:08082001
        {
			//LineFeed (fpRep,1);
			if(m_FileInfo.nFileNumber > 0 )
        	    //Modified by:sdt:08082001
				//sprintf (m_Page.szFStr,"Page#%d [ File Number : %d ]",m_Page.nCurrentPg,m_FileInfo.nFileNumber);
	            //sprintf (m_Page.szFStr,"Page#%d [ F�le Number : %d ]",m_Page.nCurrentPg,m_FileInfo.nFileNumber);
                //Modified by:sdt:12102001:
                //� -> Alt+161, New report ID character is added.
                //previous character was causing problem in some printers.
                sprintf (m_Page.szFStr,"Page#%d [ F�le Number : %d ]",m_Page.nCurrentPg,m_FileInfo.nFileNumber);
			else
	            //Modified by:sdt:08082001
				//sprintf (m_Page.szFStr,"Page#%d [ File Number : Untitled ]",m_Page.nCurrentPg);
	            //sprintf (m_Page.szFStr,"Page#%d [ F�le Number : Untitled ]",m_Page.nCurrentPg);
                //Modified by:sdt:12102001
                sprintf (m_Page.szFStr,"Page#%d [ F�le Number : Untitled ]",m_Page.nCurrentPg);
        }
        else
        {
            char MainExeVer[11];
            FindMainExeVersion(MainExeVer);
			if(m_FileInfo.nFileNumber > 0 )
        	    //sprintf (m_Page.szFStr,"Page#%d [ F�le Number : %d ]       %s",m_Page.nCurrentPg,m_FileInfo.nFileNumber,MainExeVer);
                //Modified by:sdt:12102001
                sprintf (m_Page.szFStr,"Page#%d [ F�le Number : %d ]       %s",m_Page.nCurrentPg,m_FileInfo.nFileNumber,MainExeVer);
			else
	            //sprintf (m_Page.szFStr,"Page#%d [ F�le Number : Untitled ] %s",m_Page.nCurrentPg,MainExeVer);
                //Modified by:sdt:12102001
                sprintf (m_Page.szFStr,"Page#%d [ F�le Number : Untitled ] %s",m_Page.nCurrentPg,MainExeVer);
        }
		if (m_Page.nCurrentPg == 1 || m_Page.bFAllPg == YES)
		{
			if (m_Page.bFBorder == YES)
			{
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFBStr,cBuf,CPI,CENTER,endcount);
				WriteStringToFile (fpPrn,cBuf,endcount);
				LineFeed (fpPrn,1);
				ClearBuffer (cBuf,MAXWIDTH);
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFStr,cBuf,CPI,CENTER,endcount);
				WriteStringToFile (fpPrn,cBuf,endcount);
				LineFeed (fpPrn,1);
				ClearBuffer (cBuf,MAXWIDTH);
				PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szHBStr,cBuf,CPI,CENTER,endcount);
			}
			else
            {
                if ( CurrentRepType == 1 )
					PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFStr,cBuf,CPI10,CENTER,endcount);
                else
                	PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,m_Page.szFStr,cBuf,CPI10,CENTER,endcount); //Modified by:sdt:21062001
            }

			WriteStringToFile (fpPrn,cBuf,endcount);
			LineFeed (fpPrn,1);
            //Commented by:sdt:09082001
            //Following does not giving proper adjustment of version
            // in the printout.
            /*if(EndOfReport == TRUE)//Condition Added by:sdt:08082001
            {
            	char MainExeVer[11];
				FindMainExeVersion(MainExeVer);
                SelectCondensed (1);
                ClearBuffer (cBuf,MAXWIDTH);
                PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,MainExeVer,cBuf,CPI10,RIGHT,endcount);
                WriteStringToFile (fpPrn,cBuf,endcount);
				LineFeed (fpPrn,1);
                SelectCondensed (0);
            }*/
		}
	}
    //added by:sdt:03042001
    while (m_Page.nCurrentln <= (m_Page.fInchHeight-m_Page.fbottom)*RPI)
	{
		//fputc(10,fpPrn);//Commented by:sdt:09042001
        fputc(13,fpPrn);
		m_Page.nCurrentln++;
	}
	SkipBottomMargin();
	m_Page.nCurrentln = 1;
	m_Page.nCurrentPg++;
}


void SkipTopMargin()
{
	int rows;
	rows = (int)(m_Page.ftop*RPI);
    //Added by:sdt:03042001
    for (int i=0;i < rows;i++)
	{
		//fputc (10,fpPrn);//Commented by:sdt:09042001
        fputc(13,fpPrn);
		m_Page.nCurrentln++;
	}

	//LineFeed (fpRep,rows);
}

void SkipBottomMargin()
{
   /*	int rows;
	rows = (int)(m_Page.fbottom*RPI);
	LineFeed (fpRep,rows);*/
    fputc (12,fpPrn);//Added by:sdt:03042001
}


void ReadBasicData ()
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:09092001:1605
    FILE *fpSummaryFile;//Added by:sdt:09092001:1605
	FILE *fpLimitFile;//Added by:sdt:30082001
    SPECKLIMIT specksLimit;//Added by:sdt:30082001
    char szbuf[20];//Added by:sdt:30082001

	fread (&nConfig,sizeof (nConfig),1,fpData);
	fread ((void*)&m_FileInfo,sizeof (m_FileInfo),1,fpData);
	//fread (&m_Specks,sizeof (m_Specks),1,fpData);//Commented by:sdt:16102005:2315
	fread(&runSetup,sizeof(runSetup),1,fpData); //Added by:sdt:16102005:2315
	fread (&speckData,sizeof(speckData),1,fpData);//Added by:sdt:16102005:2315

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
	//fread (&ZchHfAray,sizeof(ZchHfAray),1,fpData);//Commented by:sdt:16102005:2315

    fread (&Param,sizeof(Param),1,fpData);//Added:jj:02052001

    //Added by:sdt:08052001::Commented by:sdt:06062001
    //fread (&WorstElfext,sizeof(WorstElfext),1,fpData);
    //fread (&WorstOnext,sizeof(WorstOnext),1,fpData);
	//fread (&Worstpairnext,sizeof(WORSTPAIRNEXT),1,fpData);
	//Modified by:sdt:04022007:1610
	fread (&Worstpairnext,sizeof(Worstpairnext),1,fpData);


    //Added:jj:21052001
    fread (&SpeckList,sizeof(SpeckList),1,fpData);

	//fread (&szOriginalParams,sizeof(szOriginalParams),1,fpData);//commented by:sdt:16102005:2315
    //Added by:sdt:29082001
    //This code will writes the limit in .USE FIle.
    //start
   	sprintf(szbuf,"%s:\\replimit.bin",RamDrv);
    remove(szbuf);
   	fpLimitFile=fopen(szbuf,"wb");
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
    if(fpLimitFile!=NULL)
    {
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//MAXPARAMTERS-1 modified by:sdt:16102005:2315
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

    //Added by:sdt:09092001:1255
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
	for (int i=0;i<MAXPARAMETERS;i++) //Modified by:sdt:17102005:2020 //MAXPARAMETERS-1
		for (int j=MAXLEVELS-1;j >= 0;j--)
			FillParamHeader(i,j,CPI);
    AllocateDispmatrix();//Added by:sdt:03042001
	if (m_RepSettings->nGroupRep == YES)
		GroupParams (nGrParams);

	if (nGrParams[0] != PARAMEND)
		if (m_RepSettings->nGroupRep == YES) // because GroupRep changes the value of m_RepSettings->nGroupRep
			DisplayMatrix (nGrParams);
	IndividualParams (nGrParams ,nIndParams);

	//if (nIndParams[0] != PARAMEND)
	//{
	//	for (i = 0;nIndParams[i] != PARAMEND;i++)
	//	{
	//		nTmpArray[0] = nIndParams[i];
	//		DisplayMatrix (nTmpArray);
	//	}
	//}
	//Condition modified by:sdt:16102005:2320
	int i;
	if (nIndParams[0] != PARAMEND)
	{
		for (i = 0;nIndParams[i] != PARAMEND;i++)
		{
			nTmpArray[0] = nIndParams[i];
			//Condition added by:sdt:12102005:1335
			//To seperate frequencies of same parameter
			for(int j=0; j<MAXFREQUENCIES;j++)
			{
				//DisplayMatrix (nTmpArray);
				//if((runSetup.ulParamRunSetup[i]&(0x1l<<(((j+1)*3)-1)))!=0)
				if(m_FileInfo.cParamTested[nTmpArray[0]][j])
				{
					//for(int k=0;k<SUMEITHER(nTmpArray[0]); k++)
					//Commented by:sdt:04022007:1205
					/*if(SUMEITHER(nTmpArray[0])!=2)
					{
						DisplayIndMatrix(*nTmpArray, j, m_RepSettings->nNormUnnorm[nTmpArray[0]]);
					}
					else
					{
						DisplayIndMatrix(*nTmpArray, j, 1);
						DisplayIndMatrix(*nTmpArray, j, 2);
					}*/
					//Added by:sdt:04022007:1205
					DisplayIndMatrix(*nTmpArray, j, SUMEITHER(nTmpArray[0]));
				}
			}
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
//	int i;
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

//Function modified by:sdt:16102005:2320
void FillParamHeader(char nPNo,char nLevel,int cpi)
{
	int k = 0,n,i,j,nTemp=0;
	char *p,szTmp[MAXPARAMNAME]; //Currently 55 bytes
	switch (nLevel)
	{
		case 3:
			//UnCommented by:sdt:04022007:1210
			m_ParamHeader[nPNo].Levels[nLevel].nNoofParts=TRUEREADINGS(nPNo)*SUMEITHER(nPNo);
			//Modified by:sdt:12102005:1700//Commneted by:sdt:04022007:1210
			//m_ParamHeader[nPNo].Levels[nLevel].nNoofParts=TRUEREADINGS(nPNo);
			int i;
			for (i = 0;i < TRUEREADINGS(nPNo);i ++)
			{
				for (j = 1;j <= SUMEITHER(nPNo);j++)//Commented by:sdt:12102005:sdt:1715//Uncommented by:sdt:04022007:1210
				{
					m_ParamHeader[nPNo].Levels[nLevel].Parts[k].nCpi=cpi;
					m_ParamHeader[nPNo].Levels[nLevel].Parts[k].nTChars=MAXCOLCHAR;
					strcpy (szTmp,RdTypeHeader(nPNo,j));//Commented by:sdt:12102005:sdt:1715//Uncommented by:sdt:04022007:1210
					strcpy (m_ParamHeader[nPNo].Levels[nLevel].Parts[k++].szPHeader,szTmp);//Commented by:sdt:12102005:sdt:1715//Uncommented by:sdt:04022007:1210
					//k++; //Moved here by:sdt:12102005:1830//Commented by:sdt:04022007:1210
				}
			}
			break;
		case 2:
			m_ParamHeader[nPNo].Levels[nLevel].nNoofParts=TRUEREADINGS(nPNo);
//			int i;
			for (i = 0,k = 0;i < TRUEREADINGS(nPNo);i++)
			{
				n = 0;
				for (j=0;j < SUMEITHER(nPNo);j++,k++)//Commented by:sdt:12102005:sdt:1715//Uncommneted by:sdt:04022007:1210
				{
					n += m_ParamHeader[nPNo].Levels[nLevel+1].Parts[k].nTChars;
					//k++;//Commented by:sdt:04022007:1210
				}
				m_ParamHeader[nPNo].Levels[nLevel].Parts[i].nTChars = n;
				m_ParamHeader[nPNo].Levels[nLevel].Parts[i].nCpi=cpi;
			}
			if (prmarray[nPNo].nReadings >= 1)
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
//	    int i;
            for (i = 0;i<m_ParamHeader[nPNo].Levels[MAXLEVELS-1].nNoofParts;i++)
				nTemp+=m_ParamHeader[nPNo].Levels[MAXLEVELS-1].Parts[i].nTChars;
            m_ParamHeader[nPNo].Levels[nLevel].nNoofParts = 1;
            m_ParamHeader[nPNo].Levels[nLevel].Parts[0].nCpi=cpi;
			m_ParamHeader[nPNo].Levels[nLevel].Parts[0].nTChars=nTemp;
            strcpy(m_ParamHeader[nPNo].Levels[nLevel].Parts[0].szPHeader,"");
            break;
		case 0:
			m_ParamHeader[nPNo].nCpi = cpi;
//			int i;
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
			//else if ( speckData.fNormLength == (float)0
			//	   || speckData.fNormLength ==	(float)1000)
			//Modified by:sdt:31012007:2135
			else if ( speckData.speckparams[nPNo].fNormLength == (float)0
				   || speckData.speckparams[nPNo].fNormLength ==	(float)1000)
				sprintf (szTmp,"%s/Km",prmarray[nPNo].szUnit);
			else
				sprintf (szTmp,"%s/NL=%.0fm",prmarray[nPNo].szUnit,
			//			speckData.fNormLength );
			//Modified by:sdt:31012007:2135
						speckData.speckparams[nPNo].fNormLength);

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
    int nPrevUnit1,nPrevUnit2;//Added by:sdt:09092001:1608
    char nPrevAdjacency=0,nCurrAdjacency=0;//Added by:sdt:09092001:1608
    int nUnit2;
	int nprTemp1;
	int i=0,j,s,r,nPC,nSwitchOn,NotOk=FALSE,nReadings,nPno;//,nCount = 0; //nPC Pair Count
	int nTChars,nCpi,nValidPno,nUnitNo,flagWithinSpeck=TRUE;
	float fs,fe;   // Start & End
	READING *t_Reading,param_Reading;
	char cbuf[MAXWIDTH+100],ctmp[MAXWIDTH+100];
    //long ParaPosition[MAXPARAMETERS][MAXFREQUENCIES];
    //Data type modified from long to unsigned long //MOD17012002:02:sdt::1510
    unsigned long ParaPosition[MAXPARAMETERS][MAXFREQUENCIES];
    int nTReadings;
    char Path[80];//Added by:sdt:10042001

    int AVGINDPSUM=0; //Added by:sdt:12082001:this is act as switch
    			   //for selection of Avg. Ind. Power Sum


//	while ((int)m_FileInfo.cPairsBypassed[i]!=127)
//		nCount++,i++;

    //Added by:sdt:09042001
    //Options Selection for Parameter header.
    //start

    sprintf(Path,"%s:\\Disprep.ini",RamDrv);
	IMPEDENCE = GetPrivateProfileInt("PARAM_HEADER_OPTION","IMPEDENCE",0,Path);
    FREQUENCY = GetPrivateProfileInt("PARAM_HEADER_OPTION","FREQUENCY",0,Path);
    //Added by:sdt:13072001
    NORM_TEMP = GetPrivateProfileInt("PARAM_HEADER_OPTION","NORM_TEMP",0,Path);
    NORM_LENGTH = GetPrivateProfileInt("PARAM_HEADER_OPTION","NORM_LENGTH",0,Path);
    //End
    AVGINDPSUM =GetPrivateProfileInt("AVG_IND_PSUM","AVG_PSUM",0,Path);//Added by:sdt:14082001
    //Added by:sdt:04042001
	fseek (fpData,lStart,SEEK_SET);
	for(i=0;i<MAXPARAMETERS;i++)//Modified by:sdt:17102005:2020 //MAXPARAMETERS-1
    {
    	for(j=0;j<MAXFREQUENCIES;j++)
        {
            //if(nConfig.Specks.speckparams[i].freq[j]&&m_FileInfo.cParamTested[i][j])
            if(m_FileInfo.cParamTested[i][j])//added by:sdt:06042001
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
        nCurrAdjacency=-1;//Added by:sdt:09092001:1610
		nPrevAdjacency=-1;//Added by:sdt:09092001:1610

        //Allocate memory for Individual Powesum variable for total no. of pairs
        //and Initialise it with 0.
		//Commented by:sdt:16102005:2325 //As powersum not required in Quad Software
		//if(nValidPno ==SRNO_NEXT)
		//{
		//	ptrIpsum = (IndPowSum *)farmalloc(sizeof(IndPowSum)*m_FileInfo.cNoofPairs);
		//    memset(ptrIpsum,0,sizeof(IndPowSum)*m_FileInfo.cNoofPairs);
		//}

		ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
		DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
		ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
		m_FileInfo.cUnitof = nTempUnitOf; // For restoring normal no. of units
		nPC = -1,nUnitNo=0,nUnit2=0;
        nPrevUnit1=0;nPrevUnit2=0;//Added by:sdt:09092001:1610
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
                        nUnitNo= ((t_Reading->cPair1 - 1)/m_FileInfo.cUnitof) + 1;//Added by:sdt:09092001:1613
                        nUnit2=((t_Reading->cPair2 - 1)/m_FileInfo.cUnitof) + 1;//Added by:sdt:09092001:1613
						//Unit1= ((t_Reading->cPair1 - 1)/m_FileInfo.cUnitof) + 1;//Commented by:sdt:09092001:1615

						//if (nUnitNo!=nUnits && (t_Reading->cPair1 > m_FileInfo.cUnitof*nUnitNo||t_Reading->cPair2 > m_FileInfo.cUnitof*nUnit2))//Modified:jj:14032001
                        //Modified by:sdt:09092001:1615
                        //if (nUnitNo!=nUnits &&(nUnitNo!=nPrevUnit1)||(nUnit2!=nPrevUnit2))
                        //Modified by:sdt:10092001:1200
						//Condition is modified because last unit number was
						//not displayed.
						//Commented by:sdt:16102005:2325 //As Unit Nos. not required in Quad
						//Uncommented by:sdt:04082006:2125:start //For Pair fornat representation
						if ((nUnitNo!=nPrevUnit1)||(nUnit2!=nPrevUnit2))
						{
							ClearBuffer (ctmp,MAXWIDTH);
							if (strcmp (cbuf,ctmp)!=0)
							{
								bNewPage = FinishRow(j,i,fs,fe,cbuf,NotOk);
								NotOk = FALSE; //MOD17012002:04:sdt:1700
								fs=fe=m_Page.fleft;//MOD18012002:01:sdt::1130
								j=0;//MOD18012002:01:sdt::1130
								//bNewPage = LineFeed(fpRep,1);
								if (bNewPage == YES)
								{
									PageEnd();
									PageStart();
									if (nPC < nReadings)
									{
										ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
										//DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI);
										//Modified by:sdt:04082006:2145
										DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
										ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
									}
								}
							}
							//nUnitNo++;
							//Commented by:sdt:09092001:1615
							//nUnitNo=((t_Reading->cPair1 - 1)/m_FileInfo.cUnitof) + 1;
							//nUnit2=((t_Reading->cPair2 - 1)/m_FileInfo.cUnitof) + 1;//Added by:sdt:26042001
							//Added by:sdt:09092001:1615
							nPrevUnit1=nUnitNo;nPrevUnit2=nUnit2;
							if(nUnitNo==nUnits)
							  //if(m_FileInfo.nAppend && nNeedOfAppend)
							  //{
								m_FileInfo.cUnitof=nLastUnitSize;     //san
							 // }

							ClearBuffer (cbuf,MAXWIDTH);

							//Condition modified by:sdt:26042001
							if( nValidPno == SRNO_CUPPI )   // added : jj:jp:07022001 : for parameter cuppi
							{
								//Added by:sdt:10092001:1210
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

									WriteStringToFile (fpPrn,cbuf,endcount);
									bNewPage = LineFeed(fpPrn,1);
									if (bNewPage == YES)
									{
										PageEnd();
										PageStart();
										if (nPC < nReadings)
										{
											ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
											//DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI17);
											//Modified by:sdt:04082006:2145
											DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
											ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
										}
									}
									ClearBuffer(ctmp,MAXWIDTH);
									ClearBuffer(cbuf,MAXWIDTH);
									//NotOk = FALSE;//Added by:sdt:10092001:0905
								}
								nPrevAdjacency=nCurrAdjacency;
								//end.
								//Modified by:sdt:26042001
								sprintf(ctmp,"Unit: #%d-#%d",nUnitNo,nUnit2);
							}
							else
								sprintf (ctmp,"Unit: #%d",nUnitNo);

							fs=m_Page.fleft;
							PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,CPI,CENTER,endcount);
							WriteStringToFile (fpPrn,cbuf,endcount);
							bNewPage = LineFeed (fpPrn,1);
							if (bNewPage == YES)
							{
								PageEnd();
								PageStart();
								if (nPC < nReadings)
								{
									 ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
									 //DispParamHeader(nDispMatrix[i],nFreqMatrix[i],CPI);
									 //Modified by:sdt:04082006:2145
									 DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
									 ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
								}

							}
							ClearBuffer(ctmp,MAXWIDTH);
							ClearBuffer(cbuf,MAXWIDTH);
						}
						//Uncommented by:sdt:04082006:2125:end

						//Added by:sdt:09092001:1616
						//start
						//This condition is added for displaying Adjacent &
        	            //Nonadjcent readings separately.
            	        //if(!prmarray[nPno].nTestingMethod)
                        //Modified by:sdt:10092001:1210
            	        if(!prmarray[nPno].nTestingMethod&&(nPno!=SRNO_CUPPI))
                	    {
                            //Commented by:sdt:10092001:1210
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
                                	NotOk = FALSE; //MOD17012002:04:sdt:1700
                                    fs=fe=m_Page.fleft;//MOD18012002:01:sdt::1130
	                                j=0;//MOD18012002:01:sdt::1130
                                }
                                else
					            	bNewPage = LineFeed(fpPrn,1);
						        if (bNewPage == YES)
					  			{
									PageEnd();
									PageStart();
									if (nPC < nReadings)
            				        {
                    					ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
										DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
										ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
				                    }
								}
        	                    ClearBuffer(ctmp,MAXWIDTH);
            	                ClearBuffer(cbuf,MAXWIDTH);
                                //Commented by:sdt:10092001:1210
	                            //Adjacency information for CUPPI is displayed
    	                        //above.
                	            /*if(nPno==SRNO_CUPPI)
	                	            strcpy(ctmp,LimitTypes[nCurrAdjacency+2]);
                        	    else*/
                            		strcpy(ctmp,LimitTypes[nCurrAdjacency]);

	                            PrepareString(m_Page.fleft,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,CPI17,CENTER,endcount);

    	                        WriteStringToFile (fpPrn,cbuf,endcount);
					            bNewPage = LineFeed(fpPrn,1);
						        if (bNewPage == YES)
					  			{
									PageEnd();
									PageStart();
									if (nPC < nReadings)
            			        	{
                    					ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI17);
										DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
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
						fs=fe,fe += (float)MAXCOLCHAR / CPI;
						//Condition added by:sdt:16102005:2330
						//Condtion removed by:sdt:04082006:2130
						//if(nPno==SRNO_NEXT||nPno==SRNO_FEXT)
						{
							if (prmarray[nPno].nTestingMethod==SINGLEPAIR)
								sprintf (ctmp,"%d",t_Reading->cPair1);
							else
								sprintf (ctmp,"%d-%d",t_Reading->cPair1,t_Reading->cPair2);
						}
						//else
						//{
						//	if (prmarray[nPno].nTestingMethod==SINGLEPAIR||nPno==SRNO_CUPP)
						//		sprintf (ctmp,"%d",nUnitNo);
						//	else
						//		sprintf (ctmp,"%d-%d",nUnitNo,nUnit2);
						//}

						PrepareString(fs,fe,ctmp,cbuf,CPI,CENTER,endcount);
                        //Added by:sdt:01042001
                        farfree(t_Reading);
						break;
				case 'R' :
						fs=fe,fe += (float)MAXCOLCHAR / CPI;
						if (NotOk == TRUE)
							PrepareString(fs,fe,"Not Ok",cbuf,CPI,CENTER,endcount);
						else
							PrepareString(fs,fe,"Ok",cbuf,CPI,CENTER,endcount);
						break;
				default :
				  int k=0;
				  for (r=0;r<TRUEREADINGS(nPno);r++)
				  {
					t_Reading=OneGrReading(nPno,nPC*TRUEREADINGS(nPno)+r,ParaPosition[nPno],nFreqMatrix[i][j]);
					if (t_Reading == NULL)
						break;
					for (s=1;s<=SUMEITHER(nPno);s++)//Commented by:sdt:16102005:2330//Uncommented by:sdt:04022007:1215
					{
					  ClearBuffer(ctmp,MAXWIDTH);
					  //Uncommneted by:sdt:04022007:1215
					  nSwitchOn=m_RepSettings->nNormUnnorm[nPno]==SUM_BOTH?s:m_RepSettings->nNormUnnorm[nPno];
					  //Modified by:sdt:16102005:2330//Commented by:sdt:04022007:1215
					  //nSwitchOn =nNormUnnormMatrix[i][j];
					  switch (nSwitchOn)
					  {
						  case SUM_NORMALISED :
							//nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[r*TRUEREADINGS(nPno)+s-1].nTChars;
							//nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[r*TRUEREADINGS(nPno)+s-1].nCpi;
							nTChars=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k].nTChars;
							nCpi=m_ParamHeader[nPno].Levels[MAXLEVELS-1].Parts[k++].nCpi;
							fs=fe,fe+=(float)nTChars/nCpi;
                            //Commented by:sdt:26042001
							//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal);
                            //Added by:sdt:26042001
                            if(t_Reading->nPairAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,t_Reading->nPairAdjacency);
								//Modified by:sdt:30082001
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,t_Reading->nPairAdjacency);
                            else
                            if(t_Reading->nUnitAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,(t_Reading->nUnitAdjacency+2));
								//Modified by:sdt:30082001
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,(t_Reading->nUnitAdjacency+2));
                            else
                                //flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,0);//,t_Reading->nPairAdjacency);
                                //Modified by:sdt:30082001
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
                            //Commented by:sdt:26042001
							//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fRawVal);
                            //Modified by:sdt:26042001
                            if(t_Reading->nPairAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,t_Reading->nPairAdjacency);
                                //Modified by:sdt:30082001
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,t_Reading->nPairAdjacency);
                            else
                            if(t_Reading->nUnitAdjacency!=-1)
								//flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,(t_Reading->nUnitAdjacency+2));
                                //Modified by:sdt:30082001
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,(t_Reading->nUnitAdjacency+2));
                            else
                                //flagWithinSpeck = WithinSpeck (nPno,t_Reading->fNormVal,0);//,t_Reading->nPairAdjacency);
                                //Modified by:sdt:30082001
                                flagWithinSpeck = WithinSpeck (nPno,nFreqMatrix[i][j],t_Reading->fNormVal,0);//,t_Reading->nPairAdjacency);

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
					//Commenetd by:sdt:16102005:2345
					//if(nValidPno ==SRNO_NEXT)
					//	IntermediatePsum(t_Reading);

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
				  WriteStringToFile (fpPrn,cbuf,endcount);
				  bNewPage = LineFeed(fpPrn,1);
				  if (bNewPage == YES)
				  {
					PageEnd();
					PageStart();
					if (nPC < nReadings)
                    {
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
						DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
                    }

				  }
			}
			if (strcmp (cbuf,ctmp)!=0)
			{
			  if (nPC==nReadings)
			  {
				bNewPage = FinishRow (j-1,i,fs,fe,cbuf,NotOk);
                NotOk = FALSE; //MOD17012002:04:sdt:1700
                fs=fe=m_Page.fleft;//MOD18012002:01:sdt::1130
                j=0;//MOD18012002:01:sdt::1130
				if (bNewPage == YES)
				{
					PageEnd();
					PageStart();
					if (nPC <nReadings)
                    {
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
						DispParamHeader(nDispMatrix[i],nFreqMatrix[i],nNormUnnormMatrix[i],CPI17);
                        ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
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
		//Commented by:sdt:16102005:2345
		//if(nValidPno ==SRNO_NEXT)
		//{
		//	CalFinalIndPoweSum(); //Added by:sdt:11082001
		//    DisplayPowerSum(i,AVGINDPSUM);//Added by:sdt:11082001
		//	farfree(ptrIpsum);
		//}

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
		fs=fe,fe += (float)MAXCOLCHAR / CPI;
		break;
	  case 'R':
		fs=fe,fe += (float)MAXCOLCHAR / CPI;
		if (bNotOk == TRUE)
			PrepareString(fs,fe,"Not Ok",cbuf,CPI,CENTER,endcount);
		else
			PrepareString(fs,fe,"Ok",cbuf,CPI,CENTER,endcount);
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
  WriteStringToFile (fpPrn,cbuf,endcount);
  bNewPage = LineFeed(fpPrn,1);
 /* if (bNewPage == YES)//Added Condition : pnb: 27121999.
  {
	PageEnd();
	PageStart();
	if (nPC < nReadings)
		DispParamHeader(nDispMatrix[i],CPI);
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

//READING *OneGrReading(int nPno,int nReadingNum,long *ParaFilePos,int nFreqNo)
//MOD17012002:02:sdt::1515
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
    //MOD17012002:02:sdt::1520
    //In Prevois statement ReadingNum is an Integer. The product of
    //"sizeof(READING)*nReadingNum" is also returns the interger.
    //In Cuppi when maximum reading goes upto 4000. For reading numbers above
    //3500(not exactly)  product of "sizeof(READING)*nReadingNum" crosses
    //the value of intger limit and give any random value. Due to this
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

void DispParamHeader (int *array,int *nFreqArray, int *nNormArray, int cpi)
{
	int nPno,nTChars,nCpi,i,j,l,bNewPage;
	float S,E;
	char cbuf[MAXWIDTH],ctmp[MAXWIDTH];//changed by:sdt:26032001
	char szTmp[MAXPARAMNAME]; //Added by:sdt:12102005:1730

	for (l = 0; l<MAXLEVELS ;l++)
	{
		S = E = m_Page.fleft;
		ClearBuffer (cbuf,MAXWIDTH);
		int i;
		for (i = 0;array[i]!=PARAMEND;i++)
		{
			nPno = array[i];
			switch (nPno)
			{
			case 'C' :
				S = E,E += (float)MAXCOLCHAR/cpi;
				ClearBuffer (ctmp,MAXWIDTH);
				if (l==0) //Indicates first level
					//strcpy (ctmp,"Pair");
					//strcpy (ctmp,"Pa�r");//Modified by:sdt:08082001.//Ascii-139
					//Modified by:Sdt:12102001
				{
					//Condition added by:sdt:16102005:2335
					//Condition removed by:sdt:04082006:2130
					//if(array[i+1]==SRNO_NEXT||array[i+1]==SRNO_FEXT)
					{
						strcpy (ctmp,"Pa�r");
					}
					//else
					//	strcpy (ctmp,"Quad");
				}
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
	                if(l==1)//Contion added by:sdt:10042001:
                    	//For Impedance option.
   	                {
						//Commented by:sdt:16102005:2335
						/*if(prmarray[nPno].nParamType&&IMPEDENCE)
						{	//strcpy(ctmp,"@");
	            	            //strcat(ctmp,ZchHfAray[nPno-SRNO_ATN][nFreqArray[i]]);

                            //Modified:jj:02052001
                            //sprintf(ctmp,"@%d Ohms",ZchHfAray[nPno-SRNO_ATN][nFreqArray[i]]);

                            //if(nPno==SRNO_ATN)
                            if(nPno==SRNO_ATN && ZchHfAray[0][nFreqArray[i]]!=0) //MOD30072002:jj
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[0][nFreqArray[i]]);

                            if(nPno==SRNO_NEXT)
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[1][nFreqArray[i]]);
                            if(nPno==SRNO_ELFEXT)
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[2][nFreqArray[i]]);
                            if(nPno==SRNO_INEXT)//Condition Added by:sdt:06062001
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[3][nFreqArray[i]]);
                            if(nPno==SRNO_FEXT)//Condition Added by:sdt:06062001
	                            sprintf(ctmp,"@%d Ohms",ZchHfAray[4][nFreqArray[i]]);
                        }
						else */
						strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);

						//Commented by:sdt:16102005:2340
						//Uncommented by:sdt:11112006:1745
						 if(((nPno==SRNO_CR)&&NORM_TEMP)||((nPno==SRNO_ATN)&&NORM_TEMP))
						 {
							char sNorm_Temp[10];
							sprintf(sNorm_Temp," @%.1f�C",speckData.fNormTemp);
							strcat(ctmp,sNorm_Temp);
						 }
           	        }
					//Condition removed by:sdt:04022007:1215
					//else if(l==3) //Condition Added by:sdt:16102005:2340
					//{
					//	strcpy (szTmp,RdTypeHeader(nPno,nNormArray[i]));//Commented by:sdt:12102005:sdt:1715
					//	strcpy (m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader,szTmp);//Commented by:sdt:12102005:sdt:1715
					//	strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);
					//}
					else
					strcpy(ctmp,m_ParamHeader[nPno].Levels[l].Parts[j].szPHeader);
					S=E,E+=(float)nTChars/nCpi;
					PrepareString (S,E,ctmp,cbuf,nCpi,CENTER,endcount);
				}
				break;
			}
		}
		WriteStringToFile(fpPrn,cbuf,endcount);
		bNewPage = LineFeed (fpPrn,1);
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
       	//if (m_Page.nCurrentln == m_Page.nRowsPP)//Commented by:sdt:04042001
        if (m_Page.nCurrentln >= m_Page.nRowsPP)//Added by:sdt:04042001
			return TRUE; //Indicates Page End
		//WriteStringToFile (fp,"\n");
        fputc (13,fp);//Added by:sdt:03042001
        fputc (10,fp);//Added:jj:06052001
	}
	return FALSE; //Indicates current page
}

void ReportEnd ()
{
   int bNewPage=FALSE;
   while (bNewPage==FALSE)
	 bNewPage=LineFeed (fpPrn,1);
   PageEnd();
}


void DispRepHeader ()
{
	char szString [25][MAXHEADERSIZE] ,szData [25][MAXHEADERSIZE],cbuf[MAXWIDTH];
	int i = 0,bNewPage;
	float fss,fes,fsd,fed;  //Start & End of Data and String

    PrepareHeaderStrings (szString);
    PrepareHeaderData (szData);

    //Added by:sdt:03042001
    SelectCondensed( 0 );
    // Following Part Added by:sdt:08042001
	//start//Display Header in draft mode with 10 Cpi.
    	cRepChar = ESC;
	fputc (cRepChar,fpPrn);
	cRepChar = 120;
	fputc (cRepChar,fpPrn);
	cRepChar = 0;
	fputc (cRepChar,fpPrn);
	cRepChar = ESC;
	fputc (cRepChar,fpPrn);
    cRepChar = 80;
	fputc (cRepChar,fpPrn);
    //End
    ParamBorder (m_Page.fleft,m_Page.fInchWidth-(m_Page.fright+m_Page.fleft),'-',CPI10);
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
              if(strcmp (szString[i],"LOW FREQUENCY") == 0)
               	if(!GetPrivateProfileInt("REPORT","DISPLAY_HEADER_LFREQ",0,"default.ini"))
                    {
                         i++;
                    	continue;
                    }

              if(strcmp (szString[i],"HIGH FREQUENCY") == 0)
               	if(!GetPrivateProfileInt("REPORT","DISPLAY_HEADER_HFREQ",0,"default.ini"))
                    {
                         i++;
                    	continue;
                    }
              //End:jj:13022001


			PrepareString (fss,fes,szString[i],cbuf,CPI10,LEFT,endcount);
			PrepareString (fsd,fed,szData[i],cbuf,CPI10,LEFT,endcount);
			fss = fed,fes = fss+STRWIDTH;
			fsd=fes,fed=fsd+DATAWIDTH;
			i ++;
		}
		WriteStringToFile (fpPrn,cbuf,endcount);
		bNewPage = LineFeed (fpPrn,1);
		if (bNewPage==TRUE)
		{
			PageEnd();
			PageStart();
		}
	}
   //	bNewPage = LineFeed (fpRep,2);//Commented by:sdt:03042001
	if (bNewPage==TRUE)
	{
		PageEnd();
		PageStart();
	}
}

void PrepareHeaderStrings (char szString[40][MAXHEADERSIZE])
{
	//int nStrIndex=0;
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
	strcpy (szString [17],"Total No. of Pairs");
	strcpy (szString [18],"Pairs in an Unit");//Added by:sdt:06082006:1255
	//Uncommented by:SDT:01122015:0600//For UM Cable//Commented by:sdt:02022007:2150
	strcpy (szString [19],"Low Frequency");
	strcpy (szString [20],"High Frequency");
	strcpy (szString [21],m_FileInfo.ENTRY[6].szTitle);
	strcpy (szString [22],"Starting Fixture Pair");
	strcpy (szString [23],"FIXTURE(S)");


	//Commented by:sdt:16102005:2345
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
	}
	strcpy (szString [nStrIndex+j+1],"\0");*/
	strcpy (szString [24],"\0");
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
	//sprintf (szData[17],"%d",m_FileInfo.cNoofPairs);
	//Modified by:sdt:04122005:2330 //For Quad cable
	sprintf (szData[17],"%d",(m_FileInfo.cNoofPairs));///2));//Modified by:sdt:04082006:2100
	sprintf (szData[18],"%d",(m_FileInfo.cUnitof));//Added by:sdt:06082006:1255
	//Uncommented by:sdt:01122015:0620//For UMCable//Commented by:sdt:02022007:2150
	sprintf (szData[19],"%s",nConfig.szLFreq);
	sprintf (szData[20],"%s",nConfig.szHFreq);
	sprintf (szData[21],"%s",m_FileInfo.ENTRY[6].szEntry);
	sprintf (szData[22],"%d",m_FileInfo.cStartFixPos);
	switch(m_FileInfo.cFixtureNo)
	{
	case 0 :
		strcpy(szData[23],"INNER");
		break;
	case 1 :
		strcpy(szData[23],"OUTER");
		break;
	case 2 :
		strcpy(szData[23],"BOTH");
		break;
	}



	//Commented by:sdt:16102005:2350
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
	}
	strcpy (szData[nStrIndex+j+1],"\0");*/
	strcpy (szData[24],"\0");
}

void DispSummaryRep ()
{
	char path[80];//Added by:sdt:07112006:2155
	int nSummaryFreqType=0; //Added by:sdt:23022006:2145 //MOD2302200601
	HFFREQUECYINFO HFfreqInfo; //Added by:sdt:23022006:2145 //MOD2302200601

	int nTempSpeckIndex;//Added by:sdt:07092001:0925
    SUMMARYINFO SummaryInfo;//Added by:sdt:05092001:1030
    int nIterationCount=0;//Added by:sdt:05092001:1025
    SPECKLIMIT specksLimit;//Added by:sdt:29082001
    CONFIGLIMIT configLimit;//Added by:sdt:29082001
    int nSpeckLimitIndex;//Added by:sdt:26042001
    int Speck_Passed;//Added:jj:22012001:0915
    int SumCategory[MAXPARAMETERS]; //MOD10072002

	FILE *fpText;
	int nprTemp1, ncount=0;
	int i,nPno,cpi=CPI10,nCheck,nRem,bNewPage;
	//int SumOpt[MAXPARAMETERS];
	//Modified by:sdt:23022006:2230 //MOD2302200601
	int SumOpt[MAXPARAMETERS][2];

	char ctmp[MAXWIDTH],ctmp1[MAXWIDTH],cbuf[MAXWIDTH],*p;
	float fWidth=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
	float fs,fe,ftmp,fSumColWidth=fWidth/9.5;
	// 7 because there are 7 columns in the summary report
    Speck_Passed=GetPrivateProfileInt("REPORT","SUMM_SPEC_PASSED",0,"default.ini");//Added:jj:22012001:0920

    memset(&specksLimit,0,sizeof(specksLimit));//Added by:sdt:29082001
    memset(&configLimit,0,sizeof(configLimit));//Added by:sdt:29082001

	//MemSet (SumOpt,MAXPARAMETERS,0);
	//Modified by:sdt:23022006:2230 //MOD2302200601
	memset(&SumOpt,0,sizeof(SumOpt));

    MemSet (SumCategory,MAXPARAMETERS,0); //MOD10072002

	fs=m_Page.fleft;
	//ParamBorder (fs,fWidth-0.1,'*',cpi);
	//ClearBuffer (cbuf,MAXWIDTH);
	//PrepareString(fs,fWidth-0.1,"Summary Report",cbuf,cpi,LEFT);
	//WriteStringToFile (fpRep,cbuf);
	//LineFeed(fpRep,1);
	//ParamBorder (fs,fWidth-0.1,'*',cpi);
//	int i;
	for (i=0;m_RepSettings->nParams[i]!=PARAMEND;i++)
	{
		ncount=0;
		//sprintf (ctmp,"%s_SUM",strupr(prmarray[m_RepSettings->nParams[i]].szParamNameAbr));
		//Modified by:sdt:23022006:2130 MOD2302200601
		sprintf (ctmp,"%s_SUM_LF",strupr(prmarray[m_RepSettings->nParams[i]].szParamNameAbr));
																// SanNM 09/12/2000
		GetPrivateProfileString("SUMMARY_OPTIONS",ctmp," ",cbuf,"default.ini",0); 	// from cts.ini to default.ini
		p = strtok (cbuf,",");
		while (p != NULL)
		{
			if (p[0] == '1')
				//SumOpt[m_RepSettings->nParams[i]] |= 1 << ncount;
				//Modified by:sdt:23022006:2130 MOD2302200601
				SumOpt[m_RepSettings->nParams[i]][0] |= 1 << ncount;
			ncount++;
			p = strtok (NULL,",");
		}
		//For HF Frequency //Added by:sdt:23022006:2235:start MOD2302200601
		ncount=0;
		sprintf (ctmp,"%s_SUM_HF",strupr(prmarray[m_RepSettings->nParams[i]].szParamNameAbr));
																	// SanNM 09/12/2000
		GetPrivateProfileString("SUMMARY_OPTIONS",ctmp," ",cbuf,"default.ini",0); 	// from cts.ini to default.ini
		p = strtok (cbuf,",");
		while (p != NULL)
		{
			if (p[0] == '1')
				SumOpt[m_RepSettings->nParams[i]][1] |= 1 << ncount;
			ncount++;
			p = strtok (NULL,",");
		}
		//For HF Frequency //Added by:sdt:23022006:2235:end MOD2302200601
	}

	//Start::Added by:sdt:23022006:2235 //MOD2302200601
	char Path[MAXPATH];
	sprintf(Path,"%s:\\cts.ini",RamDrv);
	HFfreqInfo.HFLstart = GetPrivateProfileInt("HF_LOW_FREQUENCY","START_FREQ_NUMBER",0,Path);
	HFfreqInfo.HFLend = GetPrivateProfileInt("HF_LOW_FREQUENCY","END_FREQ_NUMBER",0,Path);
	HFfreqInfo.HFHstart = GetPrivateProfileInt("HF_HIGH_FREQUENCY","START_FREQ_NUMBER",0,Path);
	HFfreqInfo.HFHend = GetPrivateProfileInt("HF_HIGH_FREQUENCY","END_FREQ_NUMBER",0,Path);
	//End::Added by:sdt:23022006:2235 //MOD2302200601

	
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
		nSpeckLimitIndex=0;//Added by:sdt:16102005:2350
		nIterationCount=1; //Added by:sdt:16102005:2350
		//Commented by:sdt:16102005:2355
		/*if(prmarray[nPno].nTestingMethod)
		{
			nSpeckLimitIndex=0;
			nIterationCount=1;//Added by:sdt:09092001:1725
		}
		else
		{
			nIterationCount=1;
			if(nPno!=SRNO_CUPPI)
			{
				//Commented:MOD31072002:jj
				//if(m_FileInfo.nPairAdjacency[nPno]!=BOTHADJ)
				//{
				//	nSpeckLimitIndex=m_FileInfo.nPairAdjacency[nPno];
				//}
				//else
				{
					//Commented by:sdt:29082001
					//SetLimitsForBothMode(nPno);
					//nSpeckLimitIndex=0;
					//nSpeckLimitIndex=4;//Added by:sdt:28082001//Commented by:sdt:09092001:1410
					nSpeckLimitIndex=0;//Added by:sdt:09092001:1410
					//nIterationCount=2;//Added by:sdt:09092001:1410
					nIterationCount=3;//MOD10072002
				}
			}
			else
			{
				//Commented:MOD31072002:jj
				//if(m_FileInfo.nUnitAdjacency[nPno]!=BOTHADJ)
				//{
				//	//nSpeckLimitIndex=m_FileInfo.nUnitAdjacency[nPno]+2;
				//	nSpeckLimitIndex=m_FileInfo.nUnitAdjacency[nPno]+3;//MOD10072002
				//}
				//else
				{
					//Commented by:sdt:29082001
					//SetLimitsForBothMode(nPno);
					//nSpeckLimitIndex=2;
					//nSpeckLimitIndex=5;//added by:sdt:29082001//Commented by:sdt:09092001:1410
					//nSpeckLimitIndex=2;//Added by:sdt:09092001:1410
					nSpeckLimitIndex=3; //MOD10072002
					nIterationCount=2;//Added by:sdt:09092001:1410
				}
			}
		}*/
		nTempSpeckIndex=nSpeckLimitIndex;//Added by:sdt:09092001:1445
		for(int nFreqNum=0;nFreqNum<MAXFREQUENCIES;nFreqNum++)
		{

			//if(nConfig.Specks.speckparams[nPno].freq[nFreqNum])//commented by:sdt:06042001
			//if(m_FileInfo.cParamTested[nPno][nFreqNum])//Added by:sdt:06042001//MOD02022002:03:sdt:1550
            //if(!strcmp(m_Specks.speckparams[nPno].freq[nFreqNum],"Yes")
            //	&&m_FileInfo.cParamTested[nPno][nFreqNum]) //MOD02022002:03:sdt:1550
            //start:MOD28062002:01:sdt:1520
			//if((!strcmp(m_Specks.speckparams[nPno].freq[nFreqNum],"Yes")
			//Modified by:sdt:16102005:2355
			if((((runSetup.ulParamRunSetup[nPno]&(0x1l<<(((nFreqNum+1)*3)-1)))!=0)
                ||(nPno==SRNO_CR)||(nPno==SRNO_LR)||(nPno==SRNO_RU))
            	&&m_FileInfo.cParamTested[nPno][nFreqNum])
			//end:MOD28062002:01:sdt:1520
			{
				//Start::Added by:sdt:23022006:2235 //MOD2302200601
				if(HFfreqInfo.HFLstart <=(nFreqNum+1)  && HFfreqInfo.HFLend >= (nFreqNum+1))
				{
					nSummaryFreqType=0;
				}
				else
				if(HFfreqInfo.HFHstart <=(nFreqNum+1)  && HFfreqInfo.HFHend >= (nFreqNum+1))
				{
					nSummaryFreqType=1;
				}
				//End::Added by:sdt:23022006:2235 //MOD2302200601

				//Added by:sdt:09092001:1430
	            //This will create differnt Summary report for Adjacent (pair/Unit)
    	        //and NonAdjacent (pair/Unit) for a given Parameter.
        	    nSpeckLimitIndex=nTempSpeckIndex;
                for(int nIndex=0;nIndex<nIterationCount;nIndex++,nSpeckLimitIndex++)
                {

	            	//MOD10072002
					if(!prmarray[nPno].nTestingMethod)
					{
						if(!(SumCategory[nPno] & int(pow(2,nIndex))))
							continue ;
						if(nPno !=SRNO_CUPPI && m_FileInfo.nPairAdjacency[nPno]!=nIndex&&m_FileInfo.nPairAdjacency[nPno]!=2 && nIndex!=2) //Added:MOD31072002:jj
						{
							//Condition added by:sdt:17102005:0000
							//For Xtalks new adjancency is considered i.e. ALL_COMBINATIONS
							//if(!((nPno==SRNO_FEXT||nPno==SRNO_NEXT)&&m_FileInfo.nPairAdjacency[nPno]==ALL_COMBINATIONS))
							//MOD1103200601:sdt:11032006:2230
							//Condition modified for combination as Adajacent Pairs
							//in cable
							if(!(((nPno==SRNO_FEXT||nPno==SRNO_NEXT)&&m_FileInfo.nPairAdjacency[nPno]==ALL_COMBINATIONS)||
								((nPno==SRNO_FEXT||nPno==SRNO_NEXT)&&m_FileInfo.nPairAdjacency[nPno]==ADJPAIRS_INCABLE)))
									continue ;
						}
					}

					//Added by:sdt:09092001:1430
					//start
					if(nPno==SRNO_CUPPI)
						//SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex-2);
						//SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex-3); //MOD10072002
						//Modified by:sdt:17102005:0000
						SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex);
                	else
                		SummaryInfo=ReadSummary(nPno,nFreqNum,nSpeckLimitIndex);
                	//end

                	specksLimit=FindLimits(nPno,nFreqNum,nSpeckLimitIndex);//Added by:sdt:29082001
		            configLimit=CovertSpeckLimitToConfigLimit(specksLimit);//Added by:sdt:29082001
					ClearBuffer (cbuf,MAXWIDTH);
					fs=fe=m_Page.fleft;
					//DispSumHeader (nPno,fSumColWidth,nFreqNum);//Added by:sdt:01042001:0900
                    //Modified by:sdt:05092001:1205
	                DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					//if (SumOpt[nPno] & MIN)
					//Modified by:sdt:23022006:2240 //MOD2302200601
					if (SumOpt[nPno][nSummaryFreqType] & MIN)
					{
						fs=fe=m_Page.fleft;
						ClearBuffer (ctmp,MAXWIDTH);
						ClearBuffer (cbuf,MAXWIDTH);
						fs=fe,fe+=fSumColWidth;
						PrepareString(fs,fe,"MIN",cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);

						//Commented by:sdt:23042006:1640
						//Uncommented by:sdt:04082006:2135:start
						if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
						{
							if (nPno==2)    //Parameter SRNO_CR
							{
								if (SummaryInfo.STATRAW.cMinTiporRing==TIP)
									sprintf (ctmp,"%d (T)",SummaryInfo.STATRAW.cMinPair1);
								else
									sprintf (ctmp,"%d (R)",SummaryInfo.STATRAW.cMinPair1);
							}
							else
							sprintf (ctmp,"%d",SummaryInfo.STATRAW.cMinPair1);//Added by:sdt:09092001:1450
						}
						else
						sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMinPair1,SummaryInfo.STATRAW.cMinPair2);
						//Uncommented by:sdt:04082006:2135:end
						//Commented by:sdt:04082006:2135:start
						//Added by:sdt:23042006:1600
						//This will display MIN in Quad format.
						/*if(nPno==SRNO_NEXT||nPno==SRNO_FEXT)
						{
							sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMinPair1,SummaryInfo.STATRAW.cMinPair2);
						}
						else
						{
							int nUnit1=0, nUnit2=0, nPartno=0;
							nUnit1 = ((SummaryInfo.STATRAW.cMinPair1 - 1)/m_FileInfo.cUnitof) + 1;
							nUnit2 = ((SummaryInfo.STATRAW.cMinPair2 - 1)/m_FileInfo.cUnitof) + 1;
							if (prmarray[nPno].nReadings > 1)
							{
								if((prmarray[nPno].nTestingMethod == SINGLEPAIR)&&(nPno!=SRNO_CR))
								{
									nPartno = (SummaryInfo.STATRAW.cMinPair1 - 1)%m_FileInfo.cUnitof;
								}
								else if(nPno==SRNO_CR)
								{
									//For CR each Pair has 2 readings i.e. tip & Ring of pair has
									//separate reading. Thus in Quad of two pair we will have
									//four readings. For that representation follwing calculation is done.
									nPartno = (SummaryInfo.STATRAW.cMinPair1 - 1)%prmarray[nPno].nReadings;
									if(nPartno==0)
									{
										if (SummaryInfo.STATRAW.cMinTiporRing!=TIP)
											nPartno = 1;
									}
									else if(nPartno == 1)
									{
										if(SummaryInfo.STATRAW.cMinTiporRing==TIP)
											nPartno=2;
										else
											nPartno=3;
									}
								}
								else if(nPno == SRNO_CUPPI)
								{
									if((((SummaryInfo.STATRAW.cMinPair1 - 1)%m_FileInfo.cUnitof)==0)&&
									   (((SummaryInfo.STATRAW.cMinPair2 - 1)%m_FileInfo.cUnitof)==0))
										nPartno=0;
									else
									if((((SummaryInfo.STATRAW.cMinPair1 - 1)%m_FileInfo.cUnitof)==0)&&
									   (((SummaryInfo.STATRAW.cMinPair2 - 1)%m_FileInfo.cUnitof)==1))
										nPartno=1;
									else
									if((((SummaryInfo.STATRAW.cMinPair1 - 1)%m_FileInfo.cUnitof)==1)&&
									   (((SummaryInfo.STATRAW.cMinPair2 - 1)%m_FileInfo.cUnitof)==0))
										nPartno=2;
									else
									if((((SummaryInfo.STATRAW.cMinPair1 - 1)%m_FileInfo.cUnitof)==1)&&
									   (((SummaryInfo.STATRAW.cMinPair2 - 1)%m_FileInfo.cUnitof)==1))
										nPartno=3;
								}
							}

							if(nPno == SRNO_CUPP)
							{
								sprintf (ctmp,"%d",nUnit1);
							}
							else if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
							{
								sprintf (ctmp,"%d(%s)",nUnit1, m_ParamHeader[nPno].Levels[2].Parts[nPartno].szPHeader);
							}
							else
								sprintf (ctmp,"%d-%d(%s)",nUnit1, nUnit2, m_ParamHeader[nPno].Levels[2].Parts[nPartno].szPHeader);
						}*/
						//Commented by:sdt:04082006:2135:end

						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth*2;
						ClearBuffer (ctmp,MAXWIDTH);
						//strcpy (ctmp,m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].lowerlimit);
						//strcpy (ctmp,specksLimit.lowerlimit);//Modified by:sdt:29082001
						if(specksLimit.lowerlimit!=(float)BLANKVALUE)//Condition Added by:sdt:07122015:0500
							sprintf(ctmp,"%0.2f",specksLimit.lowerlimit);//Modified by:sdt:17102005:0000
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
				    //Commented by:sdt:09092001:1500
						/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fMin != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fMin,
										prmarray[nPno].Decimal[0],nPno);
						else
						   nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fMin);*/

				    //Added by:sdt:09092001:1500
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
				    //Commented by:sdt:09092001:1500
						/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin,
											 prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMin);*/

				    //Added by:sdt:09092001:1500
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
							if(specksLimit.lowerlimit!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								//nCheck=WithinSpeck(nPno,ftmp,nSpeckLimitIndex);
								//Modified by:sdt:29082001
								nCheck=WithinSpeck(nPno,nFreqNum,ftmp,nSpeckLimitIndex);
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									 PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									 OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						WriteStringToFile (fpPrn,cbuf,endcount);
						bNewPage = LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
	                        //Modified by:sdt:09092001:1500
    	                    DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
					//if (SumOpt[nPno] & MAX)
					//Modified by:sdt:23022006:2240 //MOD2302200601
					if (SumOpt[nPno][nSummaryFreqType] & MAX)
					{
						fs=fe=m_Page.fleft;
						ClearBuffer (ctmp,MAXWIDTH);
						ClearBuffer (cbuf,MAXWIDTH);
						fs=fe,fe+=fSumColWidth;
		        	    PrepareString(fs,fe,"MAX",cbuf,cpi,LEFT,endcount);
		            	fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//Uncommented by:sdt:04082006:2135:start
						//Commented by:sdt:23042006:1640
						if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
						{
							if (nPno==2)    //Parameter SRNO_CR
							{
								if (SummaryInfo.STATRAW.cMaxTiporRing==TIP)
									sprintf (ctmp,"%d (T)",SummaryInfo.STATRAW.cMaxPair1);
								else
									sprintf (ctmp,"%d (R)",SummaryInfo.STATRAW.cMaxPair1);
							}
							else
								sprintf (ctmp,"%d",SummaryInfo.STATRAW.cMaxPair1);
						}
						else
							sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMaxPair1,SummaryInfo.STATRAW.cMaxPair2);
						//Uncommented by:sdt:04082006:2135:end
						//Commented by:sdt:04082006:2135:start
						//This will display MAX in Quad format.
						/*if(nPno==SRNO_NEXT||nPno==SRNO_FEXT)
						{
							sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMaxPair1,SummaryInfo.STATRAW.cMaxPair2);
						}
						else
						{
							int nUnit1=0, nUnit2=0, nPartno=0;
							nUnit1 = ((SummaryInfo.STATRAW.cMaxPair1 - 1)/m_FileInfo.cUnitof) + 1;
							nUnit2 = ((SummaryInfo.STATRAW.cMaxPair2 - 1)/m_FileInfo.cUnitof) + 1;
							if (prmarray[nPno].nReadings > 1)
							{
								if((prmarray[nPno].nTestingMethod == SINGLEPAIR)&&(nPno!=SRNO_CR))
								{
									nPartno = (SummaryInfo.STATRAW.cMaxPair1 - 1)%m_FileInfo.cUnitof;
								}
								else if(nPno==SRNO_CR)
								{
									//For CR each Pair has 2 readings i.e. tip & Ring of pair has
									//separate reading. Thus in Quad of two pair we will have
									//four readings. For that representation follwing calculation is done.
									nPartno = (SummaryInfo.STATRAW.cMaxPair1 - 1)%prmarray[nPno].nReadings;
									if(nPartno==0)
									{
										if (SummaryInfo.STATRAW.cMaxTiporRing!=TIP)
											nPartno = 1;
									}
									else if(nPartno == 1)
									{
										if(SummaryInfo.STATRAW.cMaxTiporRing==TIP)
											nPartno=2;
										else
											nPartno=3;
									}
								}
								else if(nPno == SRNO_CUPPI)
								{
									if((((SummaryInfo.STATRAW.cMaxPair1 - 1)%m_FileInfo.cUnitof)==0)&&
									   (((SummaryInfo.STATRAW.cMaxPair2 - 1)%m_FileInfo.cUnitof)==0))
										nPartno=0;
									else
									if((((SummaryInfo.STATRAW.cMaxPair1 - 1)%m_FileInfo.cUnitof)==0)&&
									   (((SummaryInfo.STATRAW.cMaxPair2 - 1)%m_FileInfo.cUnitof)==1))
										nPartno=1;
									else
									if((((SummaryInfo.STATRAW.cMaxPair1 - 1)%m_FileInfo.cUnitof)==1)&&
									   (((SummaryInfo.STATRAW.cMaxPair2 - 1)%m_FileInfo.cUnitof)==0))
										nPartno=2;
									else
									if((((SummaryInfo.STATRAW.cMaxPair1 - 1)%m_FileInfo.cUnitof)==1)&&
									   (((SummaryInfo.STATRAW.cMaxPair2 - 1)%m_FileInfo.cUnitof)==1))
										nPartno=3;
								}
							}

							if(nPno == SRNO_CUPP)
							{
								sprintf (ctmp,"%d",nUnit1);
							}
							else if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
							{
								sprintf (ctmp,"%d(%s)",nUnit1, m_ParamHeader[nPno].Levels[2].Parts[nPartno].szPHeader);
							}
							else
								sprintf (ctmp,"%d-%d(%s)",nUnit1, nUnit2, m_ParamHeader[nPno].Levels[2].Parts[nPartno].szPHeader);
						}*/
						//Commented by:sdt:04082006:2135:end

						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth*2;
						ClearBuffer (ctmp,MAXWIDTH);
						//strcpy (ctmp,m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].upperlimit);
						//strcpy (ctmp,specksLimit.upperlimit);//Commented by:sdt:17102005:0000
						if(specksLimit.upperlimit!=(float)BLANKVALUE)//Condition add by:sdt:07122015:0512
							sprintf(ctmp,"%0.2f",specksLimit.upperlimit);//Added by:sdt:17102005:0000
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
                        //Modified by:sdt:09092001:1510
						/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fMax != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fMax,
								prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fMax);*/

                        //Added by:sdt:09092001:1510
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
                        //Commented by:sdt:09092001:1510
						/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax,
									prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax);*/
                        //Added by:sdt:09092001:1515
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
                            //Modified by:sdt:09092001:1515
	                        ftmp=SummaryInfo.STATNORMALISED.fMax;
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].upperlimit[0]!=NULL)
							//if(specksLimit.upperlimit[0]!=NULL)//Commented by:sdt:09102005:1430 //Temporary
							if(specksLimit.upperlimit!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								//nCheck=WithinSpeck(nPno,ftmp,nSpeckLimitIndex);
								//Modified by:sdt:29082001
								nCheck=WithinSpeck(nPno,nFreqNum,ftmp,nSpeckLimitIndex);
								if(!nCheck)
									 PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									 PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									 OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						WriteStringToFile (fpPrn,cbuf,endcount);
						bNewPage=LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                            //Modified by:sdt:09092001:1515
	                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
					//if (SumOpt[nPno] & AVG)
					//Modified by:sdt:23022006:2240 //MOD2302200601
					if (SumOpt[nPno][nSummaryFreqType] & AVG)
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
						//	ncount=sprintf(ctmp,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow);
						//if (specksLimit.averagelow[0]!=NULL)//Commented by:sdt:17102005:0005//Temporary
						if (specksLimit.averagelow!=(float)BLANKVALUE)//Condition added by:sdt:07122015:0520
							ncount=sprintf(ctmp,"%4.2f",configLimit.averagelow);//Modified by:sdt:29082001
						//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi[0]!=NULL)
						//if (specksLimit.averagehi[0]!=NULL)//Commented by:sdt:09102005:1430
						if (specksLimit.averagehi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
						{
							//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow[0]!=NULL)
							//if (specksLimit.averagelow[0]!=NULL)//Commented by:sdt:17102005:0005//Temporary
							if (specksLimit.averagelow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
								ncount+=sprintf(ctmp+ncount,"-");
								//ncount+=sprintf(ctmp+ncount,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi);
							ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.averagehi);//Modified by:sdt:29082001
						}
						ctmp[ncount] = NULL;
	        	        PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
                        //Commented by:sdt:09092001:1530
						/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fAvg != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fAvg,
										prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fAvg);*/
                        //Added by:sdt:09092001:1530
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

                        //commented by:sdt:09092001:1530
						/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg,
														prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fAvg);*/
                        //Added by:sdt:09092001:1530
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
							//if(specksLimit.averagelow[0]!=NULL)//Commented by:sdt:16102005:0010//Temporary
							if(specksLimit.averagelow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								nRem=TRUE;
								//if(ftmp<(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagelow))
								//if(ftmp<(float)atof(specksLimit.averagelow)) //Modified by:sdt:29082001
								if(ftmp<specksLimit.averagelow) //Modified by:sdt:17102005:0010
									nCheck=FALSE;
							}
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi[0]!=NULL)
							//if(specksLimit.averagehi[0]!=NULL)//Commented by:sdt:16102005:0010//Temporary
							if(specksLimit.averagehi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								nRem=TRUE;
								//if(ftmp>(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].averagehi))
								//if(ftmp>(float)atof(specksLimit.averagehi))//Modified by:sdt:29082001
								//Bug removed by:sdt:04122005:2320 //instead of > sign was < //Modified
								if(ftmp>specksLimit.averagehi) //Modified by:sdt:17102005:0010
									nCheck=FALSE;
							}
							if(nRem)
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						WriteStringToFile (fpPrn,cbuf,endcount);
						bNewPage=LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                            //Modified by:sdt:09092001:1530
							DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
					//if (SumOpt[nPno] & RMS)
					//Modified by:sdt:23022006:2240 //MOD2302200601
					if (SumOpt[nPno][nSummaryFreqType] & RMS)
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
						//if (specksLimit.rmslow[0]!=NULL)//Commented by:sdt:16102005:0010//Temporary
						if (specksLimit.rmslow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							ncount=sprintf(ctmp,"%4.2f",configLimit.rmslow);

						//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi[0]!=NULL)
						//if (specksLimit.rmshi[0]!=NULL)//Commented by:sdt:16102005:0010//Temporary
						if (specksLimit.rmshi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
						{
							if(nPno!=SRNO_FEXT)//Condition added Temporary by:sdt:23112005:2310
							{
								//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow[0]!=NULL)
								//if (specksLimit.rmslow[0]!=NULL)//Commented by:sdt:16102005:0010//Temporary
								if (specksLimit.rmslow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
									ncount+=sprintf(ctmp+ncount,"-");
								//ncount+=sprintf(ctmp+ncount,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi);
								ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.rmshi);//Modified by:sdt:29082001
							}
						}
						ctmp[ncount] = NULL;
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
				    //Commented by:sdt:09092001:1535
						/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fRms != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fRms,
										prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fRms);*/
						//Added by:sdt:09092001:1535
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
                        //commented by:sdt:09092001:1536
						/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms,
										prmarray[nPno].Decimal[0],nPno);
						else
						   nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fRms);*/
                        //Added by:sdt:09092001:1536
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
							//modified by:sdt:09092001:1538
							ftmp=SummaryInfo.STATNORMALISED.fRms;
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow[0]!=NULL)
							//if(specksLimit.rmslow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.rmslow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								 nRem=TRUE;
								 //if(ftmp<(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmslow))
								 //if(ftmp<(float)atof(specksLimit.rmslow))//Modified by:sdt:29082001
								 if(ftmp<specksLimit.rmslow)//Modified by:sdt:17102005:0015
								nCheck=FALSE;
							}
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi[0]!=NULL)
							//if(specksLimit.rmshi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.rmshi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								nRem=TRUE;
								//if(ftmp>(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].rmshi))
								//if(ftmp>(float)atof(specksLimit.rmshi))//Modified by:sdt:29082001
								if(ftmp>specksLimit.rmshi)//Modified by:sdt:17102005:0015
									nCheck=FALSE;
							}
							if(nRem)
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						WriteStringToFile (fpPrn,cbuf,endcount);
						//LineFeed (fpRep,1);//Commented:jj:15032001

        			    //Added:jj:15032001
		            	bNewPage=LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                            //Modified by:sdt:09092001:1540
	                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
					//if (SumOpt[nPno] & STDDEV)
					//Modified by:sdt:23022006:2240 //MOD2302200601
					if (SumOpt[nPno][nSummaryFreqType] & STDDEV)
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
						//if (specksLimit.stdlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
						if (specksLimit.stdlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							ncount=sprintf(ctmp,"%4.2f",configLimit.stdlow);

						//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi[0]!=NULL)
						//if (specksLimit.stdhi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
						if (specksLimit.stdhi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
						{
							//if (m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow[0]!=NULL)
							//if (specksLimit.stdlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if (specksLimit.stdlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
								ncount+=sprintf(ctmp+ncount,"-");
							//ncount+=sprintf(ctmp+ncount,"%4.2f",nConfig.Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi);
							ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.stdhi);//Added by:sdt:29082001
						}
						ctmp[ncount] = NULL;
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
                        //Commented by:sdt:09092001:1540
						/*if (m_FileInfo.STATRAW[nPno][nFreqNum].fStddev != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATRAW[nPno][nFreqNum].fStddev,
									prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATRAW[nPno][nFreqNum].fStddev);*/
                        //Added by:sdt:09092001:1540
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
                        //Commented by:sdt:09092001:1540
						/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev,
										prmarray[nPno].Decimal[0],nPno);
						else
						   nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fStddev);*/

                        //Added by:sdt:09092001:1540
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
                            //Modified by:sdt:09092001:1540
	                        ftmp=SummaryInfo.STATNORMALISED.fStddev;
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow[0]!=NULL)
							//if(specksLimit.stdlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.stdlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								 nRem=TRUE;
								 //if(ftmp<(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdlow))
								 //if(ftmp<(float)atof(specksLimit.stdlow))//Modified by:sdt:29082001
								 if(ftmp<specksLimit.stdlow)//Modified by:sdt:17102005:0020
									nCheck=FALSE;
							}
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi[0]!=NULL)
							//if(specksLimit.stdhi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.stdhi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								nRem=TRUE;
								//if(ftmp>(float)atof(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].stdhi))
								//if(ftmp>(float)atof(specksLimit.stdhi))//Modified by:sdt:29082001
								if(ftmp>specksLimit.stdhi)//Modified by:sdt:17102005:0020
									nCheck=FALSE;
							}
							if(nRem)
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						WriteStringToFile (fpPrn,cbuf,endcount);
						//LineFeed (fpRep,1);//Commented:jj:15032001

						//Added:jj:15032001
						bNewPage=LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
					   //Modified by:sdt:09092001:1542
					    DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
					//if(SumOpt[nPno] & MEAN_STDDEV)//Added:SanNM 09/12/2000 of PS thru Default.ini
					//Modified by:sdt:23022006:2245 //MOD2302200601
					if(SumOpt[nPno][nSummaryFreqType] & MEAN_STDDEV)
				  //Commented by:sdt:01042001
				   //if (nPno==7||nPno==90||nPno==8||nPno==9)//SRNO_NEXT,SRNO_FEXT,SRNO_ELFEXT,SRNO_INEXT
				    //Condition modified by:sdt:01042001:for Zch.
				//if (nPno==7||nPno==8)
					if (nPno==SRNO_FEXT||nPno==SRNO_NEXT)//Conition Modified by:sdt:14092001
					{
						fs=fe=m_Page.fleft;
						ClearBuffer (ctmp,MAXWIDTH);
						ClearBuffer (cbuf,MAXWIDTH);
						fs=fe,fe+=fSumColWidth;
						PrepareString(fs,fe,"P.S",cbuf,cpi,LEFT,endcount);

				    //MOD05072002:02:jj
						//fs=fe,fe+=fSumColWidth*2;
				    fs=fe+fSumColWidth,fe+=fSumColWidth*3;

						ClearBuffer (ctmp,MAXWIDTH);

					////////Added MOD05072002:01:jj
						//if (specksLimit.psumlow[0]!=NULL)//Commented by:sdt:17102005:0020//Temporary
						if (specksLimit.psumlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							ncount=sprintf(ctmp,"%4.2f",configLimit.psumlow);

						//if (specksLimit.psumhi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
						if (specksLimit.psumhi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
						{
							//if (specksLimit.psumlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if (specksLimit.psumlow!=(float)BLANKVALUE)//Uncommented by:sdt:05122015:0615
								ncount+=sprintf(ctmp+ncount,"-");
							ncount+=sprintf(ctmp+ncount,"%4.2f",configLimit.psumhi);
						}
						ctmp[ncount] = NULL;
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						/////////////////////

				    //Commented :MOD05072002:02:jj
						/*PrepareString(fs,fe,"",cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						PrepareString(fs,fe,"",cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);*/

						//Added by:sdt:09092001:1545
						nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fPowerSum,
								prmarray[nPno].Decimal[0],nPno);
						if ( nprTemp1 == 0.0 )
							nprTemp1 = 1;
					    sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATRAW.fPowerSum);
						if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:jj:14032001
							PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);

				    //Added by:sdt:09092001:1545
				    nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fPowerSum,
								prmarray[nPno].Decimal[0],nPno);
		  				if ( nprTemp1 == 0 )
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,SummaryInfo.STATNORMALISED.fPowerSum);
						if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:jj:14032001
							PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

	                    //////////Added MOD05072002:02:jj
    	  				if (m_RepSettings->nRemark == TRUE)
						{
							fs=fe,fe+=fSumColWidth;
							ClearBuffer (ctmp,MAXWIDTH);
							nCheck=TRUE;
							nRem=FALSE;
    	                    		ftmp=SummaryInfo.STATNORMALISED.fPowerSum;
							//if(specksLimit.psumlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.psumlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								 nRem=TRUE;
								 if(ftmp<specksLimit.psumlow)//Modified by:sdt:17102005:0020
									nCheck=FALSE;
							}
							//if(specksLimit.psumhi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.psumhi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								nRem=TRUE;
								if(ftmp>specksLimit.psumhi)//Modified by:sdt:0025
									nCheck=FALSE;
							}
							if(nRem)
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						///////////////////

						WriteStringToFile (fpPrn,cbuf,endcount);
							bNewPage=LineFeed (fpPrn,1);
							if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
                            DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
	              	//Added by:sdt:08052001
					//if (SumOpt[nPno] & WPN)
					//if ((SumOpt[nPno] & WPN) && nIndex==2) //MOD10072002
					//Modified by:sdt:23022006:2245 //MOD2302200601
					if ((SumOpt[nPno][nSummaryFreqType] & WPN) && nIndex==2) //MOD08072002
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
						//if (specksLimit.wpnlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
						if (specksLimit.wpnlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							ncount=sprintf(ctmp,"%4.2f",configLimit.wpnlow);
						//if (specksLimit.wpnhi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
						if (specksLimit.wpnhi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
						{
							//if (specksLimit.wpnlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if (specksLimit.wpnlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
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
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_ELFEXT)
						//{
						//	nprTemp1 = FindNewDeciNo(Worstpairnext.Elfext[nFreqNum][0],
						//			prmarray[nPno].Decimal[0],nPno);
						//}
						//commented by:sdt:04022007:1615
						/*if(nPno==SRNO_NEXT)
						{
							nprTemp1 = FindNewDeciNo(Worstpairnext.Onext[nFreqNum][0],
									prmarray[nPno].Decimal[0],nPno);
						}
						if(nPno==SRNO_FEXT)//Condition Added by:sdt:06062001
						{
							nprTemp1 = FindNewDeciNo(Worstpairnext.Fext[nFreqNum][0],
								prmarray[nPno].Decimal[0],nPno);
						}*/
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_INEXT)//Condition Added by:sdt:06062001
						//{
						//	nprTemp1 = FindNewDeciNo(Worstpairnext.Inext[nFreqNum][0],
						//		prmarray[nPno].Decimal[0],nPno);
						//}
						//Added by:sdt:04022007:1615
						nprTemp1 = FindNewDeciNo(Worstpairnext[nPno][nFreqNum][0],
								prmarray[nPno].Decimal[0],nPno);

				      	if ( nprTemp1 == 0.0 )
							nprTemp1 = 1;
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_ELFEXT)
						//	sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Elfext[nFreqNum][0]);
						//Commented by:sdt:04022007:1615
						/*if(nPno==SRNO_NEXT)
							sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Onext[nFreqNum][0]);
						if(nPno==SRNO_FEXT)//Condition Added by:sdt:06062001
							sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Fext[nFreqNum][0]);*/
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_INEXT)//Condition Added by:sdt:06062001
						//	sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Inext[nFreqNum][0]);

						//Added by:sdt:04022007:1615
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext[nPno][nFreqNum][0]);


						if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)
							PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_ELFEXT)
						//	nprTemp1 = FindNewDeciNo(Worstpairnext.Elfext[nFreqNum][1],
						//		prmarray[nPno].Decimal[0],nPno);
						//Commented by:sdt:04022007:1615
						/*if(nPno==SRNO_NEXT)
							nprTemp1 = FindNewDeciNo(Worstpairnext.Onext[nFreqNum][1],
								prmarray[nPno].Decimal[0],nPno);
						if(nPno==SRNO_FEXT)//Condition Added by:sdt:06062001
							nprTemp1 = FindNewDeciNo(Worstpairnext.Fext[nFreqNum][1],
								prmarray[nPno].Decimal[0],nPno);*/
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_INEXT)//Condition Added by:sdt:06062001
						//	nprTemp1 = FindNewDeciNo(Worstpairnext.Inext[nFreqNum][1],
						//		prmarray[nPno].Decimal[0],nPno);

						//Added by:sdt:04022007:1615
						nprTemp1 = FindNewDeciNo(Worstpairnext[nPno][nFreqNum][1],
							prmarray[nPno].Decimal[0],nPno);

		  				if ( nprTemp1 == 0 )
							nprTemp1 = 1;
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_ELFEXT){
						//	sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Elfext[nFreqNum][1]);
						//    worstComb=Worstpairnext.Elfext[nFreqNum][1] ; //Added:MOD23072002:jj
						//    }
						//Commented by:sdt:04022007:1615
						/*if(nPno==SRNO_NEXT){
							sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Onext[nFreqNum][1]);
							worstComb=Worstpairnext.Onext[nFreqNum][1] ;//Added:MOD23072002:jj
							}

						if(nPno==SRNO_FEXT){//Condition Added by:sdt:06062001
							sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Fext[nFreqNum][1]);
							worstComb=Worstpairnext.Fext[nFreqNum][1] ; //Added:MOD23072002:jj
							}*/
						//Commented by:sdt:17102005:0045
						//if(nPno==SRNO_INEXT){//Condition Added by:sdt:06062001
						//	sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext.Inext[nFreqNum][1]);
						//	worstComb=Worstpairnext.Inext[nFreqNum][1] ;//Added:MOD23072002:jj
						//	}

						//Added by:sdt:04022007:1615
						sprintf (ctmp,"%4.*f",nprTemp1,Worstpairnext[nPno][nFreqNum][1]);
						worstComb=Worstpairnext[nPno][nFreqNum][1] ;


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
							//if(specksLimit.wpnlow[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.wpnlow!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								 nRem=TRUE;
								 if(ftmp<specksLimit.wpnlow)//Modified by:sdt:17102005:0025
									nCheck=FALSE;
							}
						   //if(specksLimit.wpnhi[0]!=NULL)//Commented by:sdt:17102005:0010//Temporary
							if(specksLimit.wpnhi!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								nRem=TRUE;
								if(ftmp>specksLimit.wpnhi)//Modified by:sdt:17102005:0025
									nCheck=FALSE;
							}
							if(nRem)
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								if(!nCheck)
									PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
                                }
							}
						}
						///////////////////

						WriteStringToFile (fpPrn,cbuf,endcount);
						bNewPage=LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                            //Modified by:sdt:09092001:1545
	                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
						}
					}
					//Anyother option added by:sdt:19102005:0035
					//if ((SumOpt[nPno] & ANYOTHER)&& (nPno==SRNO_LR))
					//Modified by:sdt:23022006:2245 //MOD2302200601
					if ((SumOpt[nPno][nSummaryFreqType] & ANYOTHER)&& (nPno==SRNO_LR))
					{
						fs=fe=m_Page.fleft;
						ClearBuffer (ctmp,MAXWIDTH);
						ClearBuffer (cbuf,MAXWIDTH);
						fs=fe,fe+=fSumColWidth;
						PrepareString(fs,fe,"(MX-AV)%",cbuf,cpi,LEFT,endcount);
		            	fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//Uncommented by:sdt:04082006:2145:start
						//Commented by:sdt:23042006:1645
						if (prmarray[nPno].nTestingMethod == SINGLEPAIR)
						{
							sprintf (ctmp,"%d",SummaryInfo.STATRAW.cMaxPair1);
						}
						else
							sprintf (ctmp,"%d-%d",SummaryInfo.STATRAW.cMaxPair1,SummaryInfo.STATRAW.cMaxPair2);
						//Uncommented by:sdt:04082006:2145:end
						//Commented by:sdt:04082006:2145:start
						/*//Added by:sdt:23042006:1645:start
						int nUnit1=0, nPartno=0;
						nUnit1 = ((SummaryInfo.STATRAW.cMaxPair1 - 1)/m_FileInfo.cUnitof) + 1;
						nPartno = (SummaryInfo.STATRAW.cMaxPair1 - 1)%m_FileInfo.cUnitof;
						sprintf (ctmp,"%d(%s)",nUnit1, m_ParamHeader[nPno].Levels[2].Parts[nPartno].szPHeader);
						//Added by:sdt:23042006:1635:end*/
						//Commented by:sdt:04082006:2145:end

			            PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth*2;
						ClearBuffer (ctmp,MAXWIDTH);
						//strcpy (ctmp,m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].upperlimit);
						//strcpy (ctmp,specksLimit.upperlimit);//Commented by:sdt:17102005:0000
						if(specksLimit.anyother!=(float)BLANKVALUE)//Condition Added by:sdt:07122015:0625
							sprintf(ctmp,"%0.2f",specksLimit.anyother);//Added by:sdt:17102005:0000
						PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
						//Added by:sdt:09092001:1510
						if (SummaryInfo.STATRAW.fMax != 0.0)
							nprTemp1 = FindNewDeciNo(SummaryInfo.STATRAW.fMax,
								prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;

						sprintf (ctmp,"%4.*f",nprTemp1,(((SummaryInfo.STATRAW.fMax-SummaryInfo.STATRAW.fAvg)/SummaryInfo.STATRAW.fAvg)*100));

	    	            if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)//Added:pnb:14122000:1640
      			          	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

						fs=fe,fe+=fSumColWidth;
						ClearBuffer (ctmp,MAXWIDTH);
                        //Commented by:sdt:09092001:1510
						/*if (m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax != 0.0)
							nprTemp1 = FindNewDeciNo(m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax,
									prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax);*/
                        //Added by:sdt:09092001:1515
	                    if (SummaryInfo.STATNORMALISED.fMax != 0.0)
							nprTemp1 = FindNewDeciNo(SummaryInfo.STATNORMALISED.fMax,
									prmarray[nPno].Decimal[0],nPno);
						else
							nprTemp1 = 1;
						sprintf (ctmp,"%4.*f",nprTemp1,(((SummaryInfo.STATNORMALISED.fMax-SummaryInfo.STATNORMALISED.fAvg)/SummaryInfo.STATNORMALISED.fAvg)*100));

        		       if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)//Added:pnb:14122000:1640
		    	           	PrepareString(fs,fe,ctmp,cbuf,cpi,LEFT,endcount);

						if (m_RepSettings->nRemark == TRUE)
						{
							fs=fe,fe+=fSumColWidth;
							ClearBuffer (ctmp,MAXWIDTH);
							//ftmp=m_FileInfo.STATNORMALISED[nPno][nFreqNum].fMax;
                            //Modified by:sdt:09092001:1515
							ftmp=(((SummaryInfo.STATNORMALISED.fMax-SummaryInfo.STATNORMALISED.fAvg)/SummaryInfo.STATNORMALISED.fAvg)*100);
							//if(m_Specks.speckparams[nPno].specklimits[nSpeckLimitIndex].upperlimit[0]!=NULL)
							//if(specksLimit.upperlimit[0]!=NULL)//Commented by:sdt:09102005:1430 //Temporary
							if(specksLimit.anyother!=(float)BLANKVALUE)//Uncommented by:sdt:07122015:0615
							{
								NonNullSpeck++;//Added by:sdt:07112006:2130//For End Remark
								//nCheck=WithinSpeck(nPno,ftmp,nSpeckLimitIndex);
								//Modified by:sdt:29082001
								//nCheck=WithinSpeck(nPno,nFreqNum,ftmp,nSpeckLimitIndex);
								nCheck=(ftmp<specksLimit.anyother)?1:0;
								if(!nCheck)
									 PrepareString (fs,fe,"Not Ok",cbuf,cpi,LEFT,endcount);
								else
								{
									 PrepareString (fs,fe,"Ok",cbuf,cpi,LEFT,endcount);
									 OkSpeck++;//Added by:sdt:07112006:2130//For End Remark
								}
							}
						}
						WriteStringToFile (fpPrn,cbuf,endcount);
						bNewPage=LineFeed (fpPrn,1);
						if (bNewPage == YES)
						{
							PageEnd();
							PageStart();
							//DispSumHeader (nPno,fSumColWidth,nFreqNum);
							//Modified by:sdt:09092001:1515
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
							//Uncommented by:sdt:04082006:2110
							strcpy (ctmp,"No of Pair(s) Tested :");
							strcpy (ctmp1,"No of Pair(s) Specks Passed :");
							//Commented by:sdt:04082006:2110
							//Modified by:sdt:18102005:2340 //For Quad software
							//strcpy (ctmp,"No of Quads Tested :");
							//strcpy (ctmp1,"No of Quads Specks Passed :");
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
					WriteStringToFile (fpPrn,cbuf,endcount);
					bNewPage=LineFeed (fpPrn,1);
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
						WriteStringToFile (fpPrn,cbuf,endcount);
						bNewPage=LineFeed (fpPrn,1);
			        }
					if (bNewPage == YES)
					{
						PageEnd();
						PageStart();
						//DispSumHeader (nPno,fSumColWidth,nFreqNum);
                        DispSumHeader (nPno,fSumColWidth,nFreqNum,nSpeckLimitIndex);
					}//End:jj:22012001:0925
    	        }
			}
    	}
	}
//pr//	ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
	ParamBorder(m_Page.fleft,m_Page.fInchWidth-0.1,'-',cpi);
////////////////////////
	//Code for final remark
	//Added by:sdt:07112006:2210:start
	sprintf(path,"%s:\\cts.ini",RamDrv);//Added:jj:26072001
	if ( GetPrivateProfileInt("REPORT","FINAL_REMARK",1,path) )//Added:jj:26072001
	if (m_RepSettings->nRemark == TRUE)
	{
		if(NonNullSpeck==OkSpeck)
		{
			if ( GetPrivateProfileInt("REPORT","FINAL_REMARK_OK",0,path) )//Added:jj:28082001
			{

				ClearBuffer (cbuf,MAXWIDTH);
				ClearBuffer (ctmp,MAXWIDTH);
				bNewPage=LineFeed (fpPrn,1);
				//sprintf (ctmp,"All the parameters tested under File No: %d  ARE WITHIN SPECIFIED LIMITS ",m_FileInfo.nFileNumber);
				//sprintf (ctmp,"The tests under File No: %d  ARE WITHIN SPECIFIED LIMITS ",m_FileInfo.nFileNumber);
				//Modified by:sdt:14122001:1040
				//Modified as per the requirement of the Concenpta cables.
				sprintf (ctmp,"The cable is MEETING THE SPECIFICATION in all parameters.");
				fs=m_Page.fleft,fe=fs+fSumColWidth*11;
				PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
				fs=fe,fe+=fSumColWidth;
				WriteStringToFile (fpPrn,cbuf,endcount);
				bNewPage=LineFeed (fpPrn,1);

			}
		}
		else
		{
			if ( GetPrivateProfileInt("REPORT","FINAL_REMARK_NOTOK",1,path) )//Added:jj:28082001
			{

				ClearBuffer (cbuf,MAXWIDTH);
				ClearBuffer (ctmp,MAXWIDTH);
				bNewPage=LineFeed (fpPrn,1);
				//sprintf (ctmp,"All readings tested under File No : %d  ARE NOT COMPLYING WITH ",m_FileInfo.nFileNumber);
				//Modified by:sdt:14122001:1040
				//Modified as per the requirement of the Concenpta cables.
				sprintf (ctmp,"The cable is NOT MEETING THE SPECIFICATION.");
				fs=m_Page.fleft,fe=fs+fSumColWidth*11;
				PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
				fs=fe,fe+=fSumColWidth;
				WriteStringToFile (fpPrn,cbuf,endcount);
				bNewPage=LineFeed (fpPrn,1);
			}
		}
	}
	//Added by:sdt:07112006:2210:end




	fs=m_Page.fleft,fe=fs+fSumColWidth*3;
	ClearBuffer (cbuf,MAXWIDTH);

     if ( m_FileInfo.cPairsBypassed[0]!=127 )//Added:pnb:14122000:1825
	{                  //Should not display Bypass if pairs are not actually bypassed.
		PrepareString (fs,fe,"Bypassed Pair Number(s) : ",cbuf,cpi,LEFT,endcount);
		strcpy (ctmp," ");
		int i;
		for (i = 0;(int)m_FileInfo.cPairsBypassed[i]!=127;i++)
		{
			sprintf (ctmp1,"%d ",m_FileInfo.cPairsBypassed[i] - m_FileInfo.cStartFixPos + 1);
			strcat (ctmp,ctmp1);
		}
		fs = fe,fe=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
		PrepareString (fs,fe,ctmp,cbuf,cpi,LEFT,endcount);
		WriteStringToFile (fpPrn,cbuf,endcount);
		LineFeed (fpPrn,1);
		ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
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

		WriteStringToFile (fpPrn,cbuf,endcount);
		LineFeed (fpPrn,1);
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

		WriteStringToFile (fpPrn,cbuf,endcount);
		LineFeed (fpPrn,1);
          ClearBuffer (cbuf,MAXWIDTH);
          ClearBuffer (ctmp,MAXWIDTH);
     }
     //End:jj:22012001:1700


	fs=m_Page.fleft,fe=m_Page.fInchWidth-(fs+m_Page.fright);
    //Added by:sdt:12102001
	fprintf(fpPrn,"                                  QXX51017.F00");
    for(int bkspcount=0;bkspcount<42;bkspcount++)
    {
    	fputc(8,fpPrn);
    }
	ParamBorder (fs,fe-0.2,'*',cpi);
	fs=m_Page.fleft,fe=m_Page.fInchWidth-(fs+m_Page.fright);
	PrepareString (fs,fe,"End of Report",cbuf,cpi,CENTER,endcount);
	WriteStringToFile (fpPrn,cbuf,endcount);
	LineFeed (fpPrn,1);

	if(Text1)//Added:pnb:260452000
    {
	   LineFeed (fpPrn,1);
	   fpText=fopen("Text1.txt","r");
	   if(fpText==NULL)
			exit(1);
	   while(fgets(cbuf,80,fpText)!=NULL)
		{
			WriteStringToFile (fpPrn,cbuf,endcount);
		  LineFeed (fpPrn,1);
        }
        if(fpText){fclose(fpText);}
    }
    if(Text2)//Added:pnb:260452000
    {
        LineFeed (fpPrn,1);
        fpText=fopen("Text2.txt","r");
        if(fpText==NULL)
       		exit(1);
        while(fgets(cbuf,80,fpText)!=NULL)
		{
			WriteStringToFile (fpPrn,cbuf,endcount);
        	LineFeed (fpPrn,1);
        }
        if(fpText){fclose(fpText);}
    }
    if(Text3)//Added:pnb:260452000
    {
        LineFeed (fpPrn,1);
	   fpText=fopen("Text3.txt","r");
        if(fpText==NULL)
       		exit(1);
	   while(fgets(cbuf,80,fpText)!=NULL)
		{
			WriteStringToFile (fpPrn,cbuf,endcount);
        	LineFeed (fpPrn,1);
        }
        if(fpText){fclose(fpText);}
	}
}

//void DispSumHeader (int nPno,float fColWidth,int nFreqNum)
//Modified by:sdt:09092001:1305
void DispSumHeader (int nPno,float fColWidth,int nFreqNum,int nSwitchMode)
{
	char path[80];//22012001
	float fs,fe,fWidth;
	char ctmp[MAXWIDTH],cbuf[MAXWIDTH];
	int cpi = CPI10,bNewPage, i;
    //char buf[MAXWIDTH];//Added by:sdt:03042001

	//if (m_Page.nCurrentln + 7 >= m_Page.nRowsPP) //5 lines are required for Summ Header & for footer
    if (m_Page.nCurrentln + 10 >= m_Page.nRowsPP) //changed by:sdt:09042001
    //This size is increased for avoiding extra printing in summary report.
	{
		PageEnd();
		PageStart();
	}
    SelectShrink(0);//Added by:sdt:09042001
	fWidth=m_Page.fInchWidth-(m_Page.fleft+m_Page.fright);
	ParamBorder(m_Page.fleft,fWidth-0.1,'-',cpi);
	ClearBuffer (cbuf,MAXWIDTH);
	ClearBuffer (ctmp,MAXWIDTH);
	fs=fe=m_Page.fleft;


    //Following Code added by:sdt:08042001
    //Start :to Print Header in draft mode with 10 cpi.
    cRepChar = ESC;
	fputc (cRepChar,fpPrn);
	cRepChar = 120;
	fputc (cRepChar,fpPrn);
	cRepChar = 0;
	fputc (cRepChar,fpPrn);
    //End

     SelectElite(1);
    SelectCondensed(0);
    SelectShrink(1); //Added by:sdt:08042001
    fputc (ESC,fpPrn);
	fputc (87,fpPrn);
	fputc (1,fpPrn);

    //Condition Added by:sdt:01042001
    if(nPno !=SRNO_CR&& nPno!=SRNO_RU&&nPno!=SRNO_LR)
    {
        //Commented by:sdt:30082001
    	//sprintf(ctmp, "SUMMARY:%s At Freq. %s.", prmarray[nPno].szParamName,SpeckList[nFreqNum]);
        //Following Code Added by:sdt:30082001.
        //For Small Printer "At Freq." this text was going on next line.
        //Also the Impedence values to be added in the summary header.

        sprintf(ctmp, "SUMMARY:%s ", prmarray[nPno].szParamName);
        
        
	int i = 0;
		while(ctmp[i] != NULL)
		{
			ctmp[i] = toupper(ctmp[i]);
			i++;
		}
		PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,cpi,LEFT,endcount);
        WriteStringToFile (fpPrn,cbuf,endcount);
        bNewPage = LineFeed(fpPrn,1);
		if (bNewPage == YES)
		{
			PageEnd();
			PageStart();
			//DispSumHeader (nPno,CPI,nFreqNum);
            //Modified by:sdt:09092001:1625
            DispSumHeader (nPno,CPI,nFreqNum,nSwitchMode);
		}

        //Added by:sdt:09092001:1305
        //start
        //This code will add the switching information in the Summary of
        //Parameter.
        ClearBuffer (cbuf,MAXWIDTH);
        fs=fe=m_Page.fleft;
		//Commented by:sdt:26112005:1915
		//For avoid printing of Intra Unit/ Inter unit Combinations
		//for quad software.

		/*if(!prmarray[nPno].nTestingMethod)
        {
            sprintf(ctmp,"For %s",LimitTypes[nSwitchMode]);
	        PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,cpi,LEFT,endcount);
    	    WriteStringToFile (fpPrn,cbuf,endcount);

	        bNewPage=LineFeed(fpPrn,1);
			if (bNewPage==YES)
			{
				PageEnd();
				PageStart();
			}
			ClearBuffer (cbuf,MAXWIDTH);
			fs=fe=m_Page.fleft;
		}*/
		//end
		//Commented by:sdt:17102005:2315
		//In Quad software it is not neccessary to show impedence in Summary.
		/*if(nPno==SRNO_ATN)
		{
			if(ZchHfAray[0][nFreqNum]!=0)//Added:MOD30072002:jj
				sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[0][nFreqNum]);
			else
				sprintf(ctmp,"At Freq. %s. ",SpeckList[nFreqNum]);
		}
		else
		if(nPno==SRNO_NEXT)
			sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[1][nFreqNum]);
		else
		if(nPno==SRNO_FEXT)
			sprintf(ctmp,"At Freq. %s. At Impedence %d Ohms",SpeckList[nFreqNum],ZchHfAray[4][nFreqNum]);
		else*/
			sprintf(ctmp,"At Freq. %s. ",SpeckList[nFreqNum]);
	}
	else
		sprintf(ctmp, "SUMMARY:%s.", prmarray[nPno].szParamName);

//	int i = 0;
	while(ctmp[i] != NULL)
	{
		ctmp[i] = toupper(ctmp[i]);
		i++;
	}
	PrepareString (fs,m_Page.fInchWidth-m_Page.fright,ctmp,cbuf,cpi,LEFT,endcount);

    WriteStringToFile (fpPrn,cbuf,endcount);
    //commented by:sdt:09042001
    //fputc( 10, fpPrn);
    //m_Page.nCurrentln++;
    fputc (ESC,fpPrn);
	fputc (87,fpPrn);
	fputc (0,fpPrn);
    bNewPage = LineFeed(fpPrn,1);
	if (bNewPage == YES)
	{
		PageEnd();
		PageStart();
		//DispSumHeader (nPno,CPI,nFreqNum);
        //Modified by:sdt:09092001:1625
        DispSumHeader (nPno,CPI,nFreqNum,nSwitchMode);
	}
    //commented by:sdt:09042001
    //fputc( 10, fpPrn);
    //m_Page.nCurrentln++;
    SelectShrink (0);
	ClearBuffer (cbuf,MAXWIDTH);
	fs=fe,fe+=fColWidth;
	fs=fe,fe+=fColWidth;
	//Condition added by:sdt:18102005:2340 for Quad software.
	//Condition removed by:sdt:04082006:2125 for Pair format representation
	//if(nPno==SRNO_NEXT||nPno==SRNO_FEXT)
	{
		PrepareString (fs,fe,"PAIR",cbuf,cpi,CENTER,endcount);
	}
	//else
	//{
	//	PrepareString (fs,fe,"QUAD",cbuf,cpi,CENTER,endcount);
	//}
	fs=fe,fe+=fColWidth;
	PrepareString (fs,fe," SPEC VAL",cbuf,cpi,LEFT,endcount);
	fs=fe+fColWidth,fe=fs+fColWidth*2;

 	if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED )
     	PrepareString (fs,fe-0.5,"MEAS VAL",cbuf,cpi,RIGHT,endcount);
     if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED )
		PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,LEFT,endcount);
     if(m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH )
          PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,CENTER,endcount);
	if(m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM )
     	PrepareString (fs,fe,"MEAS VAL",cbuf,cpi,LEFT,endcount);


	if (m_RepSettings->nRemark==YES)
	{
	  fs=fe,fe+=fColWidth;
	  PrepareString (fs,fe,"Remark",cbuf,cpi,CENTER,endcount);
	}
	WriteStringToFile (fpPrn,cbuf,endcount);
	bNewPage = LineFeed(fpPrn,1);
	if (bNewPage == YES)
	{
		PageEnd();
		PageStart();
		//DispSumHeader (nPno,CPI,nFreqNum);
        //Modified by:sdt:09092001:1625
        DispSumHeader (nPno,CPI,nFreqNum,nSwitchMode);
	}
	//Commented by:sdt:31012007:2320:start
	/*ClearBuffer (cbuf,MAXWIDTH);
	fs=m_Page.fleft+fColWidth,fe=fs+fColWidth;
	//Commented by:sdt:31012007:2250
	//PrepareString (fs,fe,"",cbuf,cpi,CENTER,endcount);
	//fs=fe,fe+=fColWidth;
	//if ( nPno == 0 )
	//sprintf (ctmp,"%s",prmarray[nPno].szUnit);
	//else
	//	sprintf (ctmp,"%s/Km",prmarray[nPno].szUnit);

	//	 //Added:pnb:11122000:1631
	//	 if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)
	//  	PrepareString (fs,fe,ctmp,cbuf,cpi,CENTER,endcount);
	//fs=fe+fColWidth,fe+=fColWidth*2;
	if ( nPno == 0 )									 //Commented by:sdt:31012007:2310
	sprintf (ctmp,"%s",prmarray[nPno].szUnit);
	else                                               //Commented by:sdt:31012007:2255
	sprintf (ctmp,"%s/L",prmarray[nPno].szUnit);

	 //Added:pnb:11122000:1631
	 if(m_RepSettings->nNormUnnorm[nPno] == SUM_UNNORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH || m_RepSettings->nNormUnnorm[nPno] == SUM_NO_NORMUNNORM)
		  PrepareString (fs,fe,ctmp,cbuf,cpi,CENTER,endcount);

	fs=fe,fe+=fColWidth;
	//Commented by:sdt:31012007:2255
	//if ( nPno == 0 )
	//sprintf (ctmp,"%s",prmarray[nPno].szUnit);
	//else
	//sprintf (ctmp,"%s/Km",prmarray[nPno].szUnit);*/
	//Commented by:sdt:31012007:2320:end
	//Added by:sdt:31012007:2320:start
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
	//Added by:sdt:31012007:2320:End
	//Added by:sdt:31012007:2255:start
	sprintf(path,"%s:\\cts.ini",RamDrv);//jj:22012001
	if ( GetPrivateProfileInt("BASIC_INFO",
		"NORMALISE_KM",0,path) )
		sprintf (ctmp,"%s/Km",prmarray[nPno].szUnit);
	//else if ( speckData.fNormLength == (float)0
	//	   || speckData.fNormLength == (float)1000 )
	//Modified by:sdt:31012007:2135
	else if ( speckData.speckparams[nPno].fNormLength == (float)0
		   || speckData.speckparams[nPno].fNormLength ==	(float)1000)

		sprintf (ctmp,"%s/Km",prmarray[nPno].szUnit);
	else
		sprintf (ctmp,"%s/NL=%.0fm",prmarray[nPno].szUnit,
		//				speckData.fNormLength );
		//Modified by:sdt:31012007:2135
						speckData.speckparams[nPno].fNormLength);
	 //Added by:sdt:31012007:2255:end

	 if(m_RepSettings->nNormUnnorm[nPno] == SUM_NORMALISED || m_RepSettings->nNormUnnorm[nPno] == SUM_BOTH)
		PrepareString (fs,fe,ctmp,cbuf,cpi,CENTER,endcount);
	WriteStringToFile (fpPrn,cbuf,endcount);
	bNewPage=LineFeed(fpPrn,1);
	if (bNewPage == YES)
	{
		PageEnd();
		PageStart();
		//DispSumHeader (nPno,CPI,nFreqNum);
        //Modified by:sdt:09092001:1625
        DispSumHeader (nPno,CPI,nFreqNum,nSwitchMode);
	}

}

void ParamBorder (float fs,float fWidth,char ch,int cpi)
{
	char cbuf[MAXWIDTH],ctmp[MAXWIDTH];

	ClearBuffer (cbuf,MAXWIDTH);
	ClearBuffer (ctmp,MAXWIDTH);
	RepeatChar (ch,fWidth*cpi,ctmp);
	PrepareString (fs,fWidth,ctmp,cbuf,cpi,LEFT,endcount);
	WriteStringToFile (fpPrn,cbuf,endcount);
	bNewPage=LineFeed(fpPrn,1);//Added bNewPage :pnb:27121999
	if (bNewPage==TRUE)// Added:pnb:27121999
	{
		PageEnd();
		PageStart();
	}
}


//Added by:sdt:26042001
//int WithinSpeck (int nParam,float fTmp,int nAdjacency)
//Modified by:sdt:30082001
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
	nTChar = nCharAvail = fTArea * CPI; // Calculation for data is done wrt 17 cpi

	if (m_RepSettings->nRemark == YES)
		nCharAvail-=REMARKCOLSZ; // Remark will be displayed in 17 cpi
	nDispMatrix[row][col++] = 'C';
    nFreqMatrix[row][col-1] = 'C';//nFreqNo-1;//Changed by:sdt:28032001
	nCharAvail -= MAXCOLCHAR;
//	int i;
	for (i = 0;nGrParams [i] != PARAMEND; i++)
	{
		nCharReqr = AreainChar(nGrParams[i]);
       // for(int j=0;j<nConfig.Specks.speckparams[nGrParams[i]].nNumofFreqSelected;
       //             j++)

        for(int j=0;j<MAXFREQUENCIES;j++)
        {
            //Added by:sdt:25032001

            //if(nConfig.Specks.speckparams[nGrParams [i]].freq[j]==1)//Commented by:sdt:06042001
            //if(m_FileInfo.cParamTested[nGrParams[i]][j])//MOD02022002:01:sdt:1250
            //if(!strcmp(m_Specks.speckparams[nGrParams [i]].freq[j],"Yes")
			//	&&m_FileInfo.cParamTested[nGrParams [i]][j])//MOD02022002:01:sdt:1250
            //start:MOD28022002:01:sdt:1520
			//if((!strcmp(m_Specks.speckparams[nGrParams [i]].freq[j],"Yes")
			//Modified by:sdt:17102005:0030
			if((((runSetup.ulParamRunSetup[i]&(0x1l<<(((j+1)*3)-1)))!=0)
                ||(i==SRNO_CR)||(i==SRNO_LR)||(i==SRNO_RU))
				&&m_FileInfo.cParamTested[nGrParams [i]][j])
            //end:MOD28022002:01:sdt:1520
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
                //modified by:sdt:14082001
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
					    nCharAvail = fTArea * CPI; // Calculation for data is done wrt 17 cpi

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

				//This will show the readings for the parameter ONEXT,
				// for one frequency in one row.
				//Commented by:sdt:17102005:0030:Temporary
				/*if(nGrParams [i]==SRNO_NEXT)
				{
					//if(m_FileInfo.cParamTested[nGrParams [i]][j+1]&&j<MAXFREQUENCIES)//Commented by:sdt:12082001
					if(MoreFreqSelected(nGrParams [i],j)&&j<MAXFREQUENCIES)//Condition Modified by:sdt:12082001
					{
						nSetStart = 0,nSetEnd = col;
						if (m_RepSettings->nRemark == YES)//Added by:sdt:14082001
							nCharReqr = nTChar-nCharAvail-REMARKCOLSZ;//Added by:sdt:10042001
						else
							nCharReqr = nTChar-nCharAvail;
						temprow=row;tempcol=col;
						RepeatSet (nSetStart,nSetEnd,nFreqMatrix[temprow],&tempcol,nCharAvail,nCharReqr);
						nCharAvail=RepeatSet (nSetStart,nSetEnd,nDispMatrix[row],&col,nCharAvail,nCharReqr);
						if (m_RepSettings->nRemark == YES)//Added by:sdt:14082001
						{
							nDispMatrix[row][col++]='R'; //added by:sdt:27032001
							nFreqMatrix[row][col-1]='R';
						}
						row++;col=0;//added by:sdt:27032001

						nCharAvail = fTArea * CPI;
						if (m_RepSettings->nRemark == YES)
							nCharAvail-=REMARKCOLSZ;
						nDispMatrix[row][col++] = 'C';
						nFreqMatrix[row][col-1] = 'C';
						nCharAvail -= MAXCOLCHAR;
					}

				}*/


			}
		}
	}
	nSetStart = 0,nSetEnd = col;
    //nCharReqr = nTChar-nCharAvail;//Commented by:sdt:10042001
    if (m_RepSettings->nRemark == YES)//Added by:sdt:14082001
    	nCharReqr = nTChar-nCharAvail-REMARKCOLSZ;//Modified by:sdt:10042001
    else
		nCharReqr = nTChar-nCharAvail;

    temprow=row;tempcol=col;
    RepeatSet (nSetStart,nSetEnd,nFreqMatrix[temprow],&tempcol,nCharAvail,nCharReqr);
	nCharAvail=RepeatSet (nSetStart,nSetEnd,nDispMatrix[row],&col,nCharAvail,nCharReqr);

    if (m_RepSettings->nRemark == YES)//Added by:sdt:14082001
    {
    	nDispMatrix[row][col++]='R'; //added by:sdt:27032001
	    nFreqMatrix[row][col-1]='R';
    }
    row++;col=0;//added by:sdt:27032001

}

//======================================================================
//					Modifications
//
// 1 . Date:21062001  Modified by:sdt
// 	This function previously calculating the Area in characters depending
// 	upon the number of frequencies selected. But for frequency indepedent
// 	parameter such as CR it gives area in characters as zero. so this
// 	function modified so that frequency indepedent parameter's Area in
// 	character will be its Total number of characters.
//======================================================================
float AreainChar (int nPno)
{
	//return (float)((m_ParamHeader[nPno].nTChars*nConfig.Specks.speckparams[nPno].nNumofFreqSelected)<=0?m_ParamHeader[nPno].nTChars:
	//(m_ParamHeader[nPno].nTChars*nConfig.Specks.speckparams[nPno].nNumofFreqSelected)) ;
	//Changed by:sdt:16102005:1600
	//As readings for different frequencies is displayed differently
	return (float)(m_ParamHeader[nPno].nTChars) ;
}



int CheckForMultipleFreq(void)
{
	int nFlag=1;
    HFFreqSelected=0;
    LFFreqSelected=0;
     int i;
	for( i=0;i<MAXPARAMETERS;i++)//Modified by:sdt:17102005:2020 //MAXPARAMETERS-1
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
        //for(i=SRNO_ATN;i<=SRNO_ELFEXT;i++)
        for(i=Param.Hf_Param_Start;i<=Param.Hf_Param_End;i++)

        {
        	for(int j=0;j<MAXFREQUENCIES;j++)
            {
            	if(nConfig.Specks.speckparams[i].freq[j])
                 	FreqSelected[j] = 1;
            }
        }
        Count=0,i;
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
//Function modified by:sdt:17102005:0035
void AllocateDispmatrix(void)
{
	int nCharAvail ,nCharReqr;
	float fTArea;


	fTArea = m_Page.fInchWidth - (m_Page.fleft+m_Page.fright);
	nCharAvail = fTArea * CPI17;

	if (m_RepSettings->nRemark == YES)
		nCharAvail-=REMARKCOLSZ; // Remark will be displayed in 17 cpi
	nCharAvail-=MAXCOLCHAR;//for displaying pair number.
	for(int i=0;i<MAXPARAMETERS;i++)//Changed by:sdt:08042001:for Zchms
    {
    	//nCharReqr = AreainChar(i);
        //nArraySize+=int(nCharReqr/nCharAvail)+1;

		//Condition Added by:sdt:17012002 //MOD17012002:01:sdt::1255
        //For ONEXT, to display the Individual Powersum reading for every
		//frequency is displayed separately.
		//Commented by:sdt:12102005:1115 //Condtion removed
		//if(i!=SRNO_NEXT)
		//{
			nCharReqr = AreainChar(i);
			nArraySize+=int(nCharReqr/nCharAvail)+1;
			//Condition added by:sdt:12102005:sdt:1225
			if(nConfig.Specks.speckparams[i].nNumofFreqSelected>1)
			{
				nArraySize+=nConfig.Specks.speckparams[i].nNumofFreqSelected;
			}
			//Condition added by:sdt:12102005:1650
			//Condition added to seperate normalised & Unnormalised readings
			//if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
			//{
			//	nArraySize++;
			//}
		//}
		//else
		//{
		//	nArraySize+=nConfig.Specks.speckparams[i].nNumofFreqSelected;
		//}

	}
	//Condition removed by:sdt:04022007:1215
	//Condition moved here by:sdt:12102005:1650
	//Condition added to seperate normalised & Unnormalised readings
	//if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
	//{
	//	nArraySize*=2;
	//}
	nDispMatrix = (int **)farmalloc(sizeof(int *)*nArraySize);
	nFreqMatrix = (int **)farmalloc(sizeof(int *)*nArraySize);
	nNormUnnormMatrix = (int **)farmalloc(sizeof(int *)*nArraySize); //Added by:sdt:12102005:1650
	int i;
	for(i=0;i<nArraySize;i++)
	{
		nDispMatrix[i] = (int *)farmalloc(sizeof(int)*(MAXWIDTH/MAXCOLCHAR));
		nFreqMatrix[i] = (int *)farmalloc(sizeof(int)*(MAXWIDTH/MAXCOLCHAR));
		nNormUnnormMatrix[i] = (int *)farmalloc(sizeof(int)*(MAXWIDTH/MAXCOLCHAR)); //Added by:sdt:12102005:1650
	}
	for(i=0;i<nArraySize;i++)
	{
		MemSet (nDispMatrix[i],(MAXWIDTH/MAXCOLCHAR),PARAMEND);
		MemSet (nFreqMatrix[i],(MAXWIDTH/MAXCOLCHAR),PARAMEND);
		MemSet (nNormUnnormMatrix[i],(MAXWIDTH/MAXCOLCHAR),PARAMEND);
	}
}

//added by:sdt:03042001
void SetPageLength ()
{
	 cRepChar = ESC;
   fputc (cRepChar,fpPrn);
   cRepChar = 67;
   fputc (cRepChar,fpPrn);
   cRepChar = (char)((int)(m_Page.fInchHeight*6));
   fputc (cRepChar,fpPrn);
}

//added by:sdt:03042001
void SelectElite(char OnOff)
{
		cRepChar = ESC;
		fputc (27 ,fpPrn);
		cRepChar = 77;
		fputc (77 ,fpPrn);
          OnOff;
}


//Added by:sdt:03042001
void SelectCondensed (char OnOff)
{
	if(OnOff==1)
	{
		cRepChar = ESC;
		fputc (27,fpPrn);
		cRepChar = 15;
		fputc (15,fpPrn);
	}
	else
	{
		cRepChar = 18;
		fputc (cRepChar,fpPrn);
	}
}


//Added by:sdt:03042001
void SelectShrink (char OnOff)
{
	if(OnOff==1)
	{
		cRepChar = ESC;
		fputc (cRepChar,fpPrn);
		cRepChar = 103;
		fputc (cRepChar,fpPrn);
	}
	else
	{
		  cRepChar = ESC;
		  fputc (cRepChar,fpPrn);
		  cRepChar = 80;
		  fputc (cRepChar,fpPrn);
  	}
}

//Added by:sdt:08042001
void SelectDraft (void)
{
	cRepChar = ESC;
	fputc (cRepChar,fpPrn);
	cRepChar = 120;
	fputc (cRepChar,fpPrn);
	cRepChar = 0;
	fputc (cRepChar,fpPrn);
	cRepChar = ESC;
	fputc (cRepChar,fpPrn);
    cRepChar = 77;
	fputc (cRepChar,fpPrn);
}

int SelectPrinterMode(void)
{
	//int Mode;
    char path[80];//22012001
	char szStr[10]; // to get float data items

    sprintf(path,"%s:\\disprep.ini",RamDrv);//jj:22012001
	GetPrivateProfileString("PRINTMODE","DRAFT","0",szStr,path,0);
    return(atoi(szStr));
}


//========================================================================//
//	FUNCTION NAME :- PrinterSetup
//	USE :- Reads the Printer setup information form the Disprep.ini.
//		   and initilise the printer for that settings.
//  DATE OF CREATION :- 20062001
//	CALLIE FUNCTIONS :- main
//  CALLED FUNCTION :- GetPrivateProfileString,GetPrivateProfileInt,
//                     PageSetup,SetPageLength.
//  LIBRARY FUNCTIONS :- sprintf
//  AUTHOR NAME :- sdt
// 	MODIFICATIONS :-
//  	MODIFICATION NO:- 1
//	  		DATE :- 20062001
//  		WHO HAS MODIFIED :-sdt
//  		DESCRIPTION :- Condition for selecting CPI is added.
//  	MODIFICATION NO:- 2
//	  		DATE :- 20062001
//  		WHO HAS MODIFIED :-sdt
//  		DESCRIPTION :- Condition for selecting CPI is modified .
//  	MODIFICATION NO:- 3
//	  		DATE :- 21062001
//  		WHO HAS MODIFIED :-sdt
//  		DESCRIPTION :- Printer selection option shifted in Default.ini
//                         from Disprep.ini.Printer Mode selection tag is
//                         modified. It now contain Field as
//						   "Condensed_Mode" instead of"Draft", because
//                         printer is initialised in Draft mode for
//						   both Condensed & Non Condensed mode.
//
//========================================================================//


int PrinterSetup(void)
{
    char path[80];
	sprintf(path,"default.ini");
    char PrintType[30],szString[30];
    int nPrinterSel;
	nPrinterSel=GetPrivateProfileInt("PRINTER_SELECTION","PRINTER_SELECTED",0,path);

    sprintf(szString,"PRINTER_%d",nPrinterSel);//Modified by:sdt:21062001
    GetPrivateProfileString("PRINTER_TYPES",szString,"",PrintType,path,0);

    //Added:jj:08052001
    //sprintf(path,"%s:\\disprep.ini",RamDrv);//jj:22012001//Added by:sdt:21062001
	int nConMode= GetPrivateProfileInt("PRINTMODE","CONDENSED_MODE",0,path);

    //Below condition added by:sdt:20062001
    if(nConMode==1) //Condition for selecting the CPI for the Printer.
    	CPI=17;
	else
       	CPI=12;

	PageSetup(PrintType);
    SetPageLength ();
    //added by:sdt:20062001
    //Code added for initialising the printer in draft mode
    // with 10 CPI.
    cRepChar = ESC;
	fputc (cRepChar,fpPrn);
	cRepChar = 120;
	fputc (cRepChar,fpPrn);
	cRepChar = 0;
	fputc (cRepChar,fpPrn);
	cRepChar = ESC;
	fputc (cRepChar,fpPrn);
    cRepChar = 80;
	fputc (cRepChar,fpPrn);

    return nConMode;
}

//Function Added by:sdt:08082001
void FindMainExeVersion(char *ptrMainExeVer)
{
    char path[80];
	sprintf(path,"%s:\\ver.sig",RamDrv);
    dec(3,"ever.sig",path,"");
    GetPrivateProfileString ("MAIN","VER","�FX10808",ptrMainExeVer,path,0);//"ver.sig");
	remove(path);
}
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
    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
    sprintf(cBuf,"Individual Power Sum for %s at Frequency %s",
				 prmarray[nDispMatrix[index][1]].szParamNameAbr,
                 SpeckList[nFreqMatrix[index][1]]);
    WriteStringToFile (fpPrn,cBuf,endcount);
    ClearBuffer (cBuf,MAXWIDTH);
    bNewPage = LineFeed (fpPrn,1);
	if (bNewPage == YES)
	{
		 PageEnd();
		 PageStart();
    }
    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);
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
	nCharAvail = fTArea * CPI; // Calculation for data is done wrt 17 cpi

    //if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
    //Modified by:sdt:13082001
    if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
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
	fe=m_Page.fleft+((float)MAXCOLCHAR)/CPI;
    while(nCharAvail>=nCharReq)
    {
        nReadperRow++;
       	//sprintf(cTmp,"Pa�r");
        //Modified by:sdt:12102001
        sprintf(cTmp,"Pa�r");
	    PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);
        fs=fe;fe+=((float)MAXCOLCHAR)/CPI;
    	//if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
        //Modified by:sdt:13082001
        if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
	    {
			sprintf(cTmp,"%s",RdTypeHeader(SRNO_NEXT,1));
        	PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);
            fs=fe;fe+=((float)MAXCOLCHAR)/CPI;
            sprintf(cTmp,"%s",RdTypeHeader(SRNO_NEXT,2));
        	PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);
        }
        else
        {
        	sprintf(cTmp,"%s",RdTypeHeader(SRNO_NEXT,m_RepSettings->nNormUnnorm[SRNO_NEXT]));//Modified by:sdt:13082001
        	PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);
        }
    	nCharAvail-=nCharReq;
        fs=fe;fe+=((float)MAXCOLCHAR)/CPI;
    }
    WriteStringToFile (fpPrn,cBuf,endcount);
    ClearBuffer (cBuf,MAXWIDTH);
    bNewPage = LineFeed (fpPrn,1);
    if (bNewPage == YES)
	{
		 PageEnd();
		 PageStart();
    }
    ParamBorder(m_Page.fleft,m_Page.fInchWidth,'-',CPI);

    //End
    //Displaying Values of Ind Pow sum.
    //start

	nReading=0;
    while(nReading<m_FileInfo.cNoofPairs)
    {
        fs=m_Page.fleft;
	    fe=m_Page.fleft+((float)MAXCOLCHAR)/CPI;
        for(int j=0;j<nReadperRow&&nReading<m_FileInfo.cNoofPairs;j++)
        {
        	if(!(nReading%m_FileInfo.cUnitof))
	        {
                if(fs!=m_Page.fleft)
                {
                	WriteStringToFile (fpPrn,cBuf,endcount);
	                ClearBuffer (cBuf,MAXWIDTH);
    	            bNewPage = LineFeed (fpPrn,1);
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
                    //if(m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
                    //Modified by:sdt:13082001
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
                    PrepareString (m_Page.fleft,m_Page.fInchWidth-m_Page.fright,cTmp,cBuf,CPI,LEFT,endcount);
		            WriteStringToFile (fpPrn,cBuf,endcount);
    	            ClearBuffer (cBuf,MAXWIDTH);
        	        bNewPage = LineFeed (fpPrn,1);
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
        	    PrepareString (m_Page.fleft,m_Page.fInchWidth-m_Page.fright,cTmp,cBuf,CPI,CENTER,endcount);
	            WriteStringToFile (fpPrn,cBuf,endcount);
                j=0;//MOD18012002:01:sdt::1130
                fs=m_Page.fleft; //MOD18012002:01:sdt::1130
			    fe=m_Page.fleft+((float)MAXCOLCHAR)/CPI17;//MOD18012002:01:sdt::1130
                ClearBuffer (cBuf,MAXWIDTH);
                bNewPage = LineFeed (fpPrn,1);
    	        if (bNewPage == YES)
				{
					 PageEnd();
					 PageStart();
    			}
        	}
            if((ptrIpsum[nReading].RawVal!=0.0)||(ptrIpsum[nReading].NorVal!=0.0))
            {
	            sprintf(cTmp,"%d",nReading+1);
    	        PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);

    	        //if(m_RepSettings->nNormUnnorm[i]==SUM_UNNORMALISED||m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
                //Modified by:sdt:13082001
                if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_UNNORMALISED||m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
	            {
                    fs=fe;fe+=((float)MAXCOLCHAR)/CPI;
		            sprintf(cTmp,"%.2lf",ptrIpsum[nReading].RawVal);
	    	        PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);
    	        }
	            //fs=fe;fe+=((float)MAXCOLCHAR)/CPI17;
    	        //if(m_RepSettings->nNormUnnorm[i]==SUM_NORMALISED||m_RepSettings->nNormUnnorm[i]==SUM_BOTH)
                //Modified by:sdt:13082001
                if(m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_NORMALISED||m_RepSettings->nNormUnnorm[SRNO_NEXT]==SUM_BOTH)
	            {
                    fs=fe;fe+=((float)MAXCOLCHAR)/CPI;
		            sprintf(cTmp,"%.2lf",ptrIpsum[nReading].NorVal);
	    	        PrepareString (fs,fe,cTmp,cBuf,CPI,CENTER,endcount);
    	        }
        	    fs=fe;fe+=((float)MAXCOLCHAR)/CPI;

                AvgIndPsum.RawVal += ptrIpsum[nReading].RawVal; //Added by:sdt:12082001
                AvgIndPsum.NorVal += ptrIpsum[nReading].NorVal; //Added by:sdt:12082001
                nAvgCount++; //Added by:sdt:12082001
            }
            nReading++;
        }
        WriteStringToFile (fpPrn,cBuf,endcount);
        ClearBuffer (cBuf,MAXWIDTH);
        bNewPage = LineFeed (fpPrn,1);
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
        //Modified by:sdt:13082001
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
	    PrepareString (m_Page.fleft,m_Page.fInchWidth-m_Page.fright,cTmp,cBuf,CPI,LEFT,endcount);
		WriteStringToFile (fpPrn,cBuf,endcount);
	    ClearBuffer (cBuf,MAXWIDTH);
	    bNewPage = LineFeed (fpPrn,1);
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

//Function is Added by:sdt:29082001
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

//Function Added by:sdt:29082001
//this function will convert limits in the character format to float format.
CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit)
{
	CONFIGLIMIT configLimit;
	//Commented by:sdt:17102005:0035
	/*configLimit.lowerlimit=strcmp(specksLimit.lowerlimit,"")?atof(specksLimit.lowerlimit):BLANKVALUE;
	configLimit.upperlimit=strcmp(specksLimit.upperlimit,"")?atof(specksLimit.upperlimit):BLANKVALUE;
	configLimit.averagelow=strcmp(specksLimit.averagelow,"")?atof(specksLimit.averagelow):BLANKVALUE;
	//configLimit.averagehi=strcmp(specksLimit.averagelow,"")?atof(specksLimit.averagehi):BLANKVALUE;
	//MOD17012002:03:sdt::1520
	//Here first "specksLimit.averagehi" variable was bymistakly written as
	//"specksLimit.averagelow"
	configLimit.averagehi=strcmp(specksLimit.averagehi,"")?atof(specksLimit.averagehi):BLANKVALUE;
	configLimit.stdlow=strcmp(specksLimit.stdlow,"")?atof(specksLimit.stdlow):BLANKVALUE;
	configLimit.stdhi=strcmp(specksLimit.stdhi,"")?atof(specksLimit.stdhi):BLANKVALUE;
	configLimit.rmslow=strcmp(specksLimit.rmslow,"")?atof(specksLimit.rmslow):BLANKVALUE;
	configLimit.rmshi=strcmp(specksLimit.rmshi,"")?atof(specksLimit.rmshi):BLANKVALUE;

	//MOD05072002:02:jj
	configLimit.psumlow=strcmp(specksLimit.psumlow,"")?atof(specksLimit.psumlow):BLANKVALUE;
	configLimit.psumhi=strcmp(specksLimit.psumhi,"")?atof(specksLimit.psumhi):BLANKVALUE;

	//MOD23072002:jj
	configLimit.wpnlow=strcmp(specksLimit.wpnlow,"")?atof(specksLimit.wpnlow):BLANKVALUE;
	configLimit.wpnhi=strcmp(specksLimit.wpnhi,"")?atof(specksLimit.wpnhi):BLANKVALUE;*/
	//Added by:sdt:17102005:0035
	configLimit.lowerlimit = specksLimit.lowerlimit;
	configLimit.upperlimit = specksLimit.upperlimit;
	configLimit.averagelow = specksLimit.averagelow;
	configLimit.averagehi = specksLimit.averagehi;
	configLimit.stdlow = specksLimit.stdlow;
	configLimit.stdhi = specksLimit.stdhi;
	configLimit.rmslow = specksLimit.rmslow;
	configLimit.rmshi = specksLimit.rmshi;

	//Grand power sum MOD0507002:01:jj
	configLimit.psumlow = specksLimit.psumlow;
	configLimit.psumhi = specksLimit.psumhi;

	//1%Worst Pair Next MOD06072002:01:jj
	configLimit.wpnlow = specksLimit.wpnlow;
	configLimit.wpnhi = specksLimit.wpnhi;
	configLimit.anyother = specksLimit.anyother;


    return configLimit;

}

//Added by:sdt:09092001:1255
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

//Added by:sdt:09092001:1255
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

//This function added by:sdt:16102005:2320
//To seperate different frequencies of a parameter
void DisplayIndMatrix (int nGrParams, int nFreqNum, int nNormIndex)
{
	int nSetStart ,nSetEnd ,nCharAvail ,nCharReqr ,nTChar;
	float fTArea;
	static int row = 0,col = 0;
	int temprow=0,tempcol=0, tempcol1=0;
	//int nCharUsedCount=0;
	fTArea = m_Page.fInchWidth - (m_Page.fleft+m_Page.fright);
	nTChar = nCharAvail = fTArea * CPI17; // Calculation for data is done wrt 17 cpi

	if (m_RepSettings->nRemark == YES)
		nCharAvail-=REMARKCOLSZ; // Remark will be displayed in 17 cpi
	nDispMatrix[row][col++] = 'C';
	nFreqMatrix[row][col-1] = 'C';//nFreqNo-1;//Changed by:sdt:28032001
	//nNormUnnormMatrix[row][col-1] = 'C'; //Added by:sdt:16102005:1700//Commented by:sdt:04022007:1215
	nCharAvail -= MAXCOLCHAR;

	nCharReqr = AreainChar(nGrParams);
	if(nCharReqr < nCharAvail)
	{
			nDispMatrix[row][col++] = nGrParams ;
			nFreqMatrix[row][col-1] = nFreqNum;//nFreqNo-1;
			//nNormUnnormMatrix[row][col-1] = nNormIndex; //Added by:sdt:16102005:1700//Commneted by:sdt:04022007:1225
			nCharAvail -= m_ParamHeader[nGrParams ].nTChars;
			nCharReqr -=m_ParamHeader[nGrParams ].nTChars;
	}

	nSetStart = 0,nSetEnd = col;
	//nCharReqr = nTChar-nCharAvail;//Commented by:sdt:10042001
	if (m_RepSettings->nRemark == YES) //Condition Added by:sdt:14082001
		nCharReqr = nTChar-nCharAvail-REMARKCOLSZ;//Added by:sdt:10042001
	else
		nCharReqr = nTChar-nCharAvail;
	temprow=row;tempcol=col;
	tempcol1 = col; //Added by:sdt:12102005:1815
	//RepeatSet (nSetStart,nSetEnd,nNormUnnormMatrix[temprow],&tempcol1,nCharAvail,nCharReqr);//Added by:sdt:12102005:1815//Commented by:sdt:04022007:1220
	RepeatSet (nSetStart,nSetEnd,nFreqMatrix[temprow],&tempcol,nCharAvail,nCharReqr);
	nCharAvail=RepeatSet (nSetStart,nSetEnd,nDispMatrix[row],&col,nCharAvail,nCharReqr);

	if (m_RepSettings->nRemark == YES) //Condition Added by:sdt:14082001
	{
		nDispMatrix[row][col++]='R'; //added by:sdt:27032001
		nFreqMatrix[row][col-1]='R';
		//nNormUnnormMatrix[row][col-1] = 'R'; //Added by:sdt:16102005:1700//Commented by:sdt:04022007:1220
	}
	row++;col=0;//added by:sdt:27032001

}
