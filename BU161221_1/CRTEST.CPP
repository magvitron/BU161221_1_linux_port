//#ifdef EXECUTE //Added by:sdt:21082001
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <bios.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <stdlib.h>
#include <values.h>
#include <process.h>
//#include <graphics.h>

#include "ct.h"
#include "key.h"
#include "share.h"
#include "Stimer.h"
#include "rdsch.h"
//#include "menu.h"
#include "funproto.h"
#include "frontpg.h"
#include "setup.h"

extern RunSetup runSetup; //Added by:sdt:28042015:2100

//Added by:sdt:03022007:1955
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];

//Added by:sdt:03092001
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);

//Added by:sdt:23082001
//int GetLimitIndex(int Param);
//CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit);
//SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex,int bCreateSpeckFlag);

//Added:jj:06052001
extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];

//extern void ReadStdVal(int nParamCount);//Added:jj:26032001
extern int 			factor;
extern int 			GBreak;
extern char 		cRamDrive[2];
extern char 		cSinglePair1, cSinglePair2;
extern PARAMINFO	paraminfo[ MAXPARAMETERS];//+2 removed by:sdt:30092005:2145
extern PRM			*prmarray;
extern int 			ParamCtsHandle; // File Handle to read PARAM.CTS file.
extern int 			noofparameters;

extern int			nPrevPair1,nPrevPair2;
extern int          gnTest[MAXPARAMETERS];
extern int          gflagReplace1,gflagReplace2;


extern int 			nLastUnitSize;
extern int          nCurrentRange,nCurrentReadingno;
extern int 			nUnits;

extern float		gfOffset[MAXRANGES];
extern float        fFcal[MAXRANGES];

extern int         	nCount;
extern char 		bIncOkRej;

//Commented by:sdt:01042001
/*extern float far    fNorValCR[MAX_ARRAY_INDEX][MAX_NO_READINGS];
extern float far    fRawValCR[MAX_ARRAY_INDEX][MAX_NO_READINGS];*/

extern          	RdControl       rdCtrl;
//extern          	SPECIFICATION    speck;
extern   int       	nTested[MAXPARAMETERS];
extern int      	flagReset;

extern int 			nParamCount;
extern int      	nGiveAlarms;
extern int      	nflagRangeLock;
extern int      	bSync;
extern int      	nTempUnitOf;
extern int      	graphisuptodate;
extern char     	szRangeHold[2];
extern char     	szFreq[10];
extern char     	szRunSelectParam[MAXLEN_SELECT_PARAM];
extern char     	defaultmessage[];

extern struct   	time currenttime;
extern              FileStatus fs;
extern Calibration 	Cal;

//Commented:jj:18052001
//extern FILE 		*fpData[MAXPARAMETERS][MAXFREQUENCIES];

/*
extern unsigned long  far       ALocationSys[1000];
extern float far                ASysOffsetVal[1000];*/


extern INFORMATION   m_FileInfo;
extern CTPAIR        CtPair;
extern CONFIG        nConfig;

//extern char 		SpeckList[10][30]; //Added by:sdt:20032001
extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:20032001
extern STRUCT_SPECKS speckData;

int 	GetParamReadingsCR(int);//modified by:sdt:20032001
//int 	SelectFixtureCR (int nStart);
float 	normaliseCR(float fReading);
float 	GetRawReadingCR (int nRef);
void 	FillDispStruct(PRM prmarray,int paramno);
//void	SelectSplitForCal();
//int     SetParamFreq(void);
//void    ReadInstrStd (float *pfStd, int nRange, int nFreqIndex);
//extern int 		SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
//extern void 	InitialiseCALarray(void);
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001

extern TFrontPage *fPage; //Added by:sdt:27092005:2300

int  CrTest()
{
//    memset( prmarray, 0, sizeof( prmarray ) );
	char path[80];
    //int nRead=1;
	FILE	*fpSbTest;//Uncommented by:sdt:09112006:2200//Commented by:sdt:13112005:1405
	FILE *BnSResult;//Uncommented by:sdt:09112006:2200//Commented by:sdt:13112005:1405
	int nFreqNum = 0; //added by:sdt:20032001
	nCount=0;

	TESTPARAM *TestParam;//Uncommented by:sdt:09112006:2205//Commented by:sdt:13122005:1305
	unsigned char BSResult;//Uncommented by:sdt:09112006:2205//commented by:sdt:13112005:1305
	int nReturnVal=0;
	nParamCount=SRNO_CR;

	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
	{
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	ReadParamDotCts( paraminfo[SRNO_RU].szParamAbr , prmarray);
	FillDispStruct(prmarray,SRNO_RU);//Commented by:sdt:16102005:1840 //As it is no longer used
	ReadParamDotCts( paraminfo[SRNO_LR].szParamAbr , prmarray);
	//FillDispStruct(prmarray,SRNO_LR);//Commented by:sdt:16102005:1840 //As it is no longer used
	ReadParamDotCts( paraminfo[nParamCount].szParamAbr, prmarray );
	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used
	ReadStdVal(nParamCount); //Added:jj:26032001

	lseek ( ParamCtsHandle, 0L, SEEK_SET );
	lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );

	lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount , SEEK_CUR );
	//nRead = read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );//Commented by:sdt:07052001
	read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
	close( ParamCtsHandle );
	//Commenyed by:sdt:20032001
//    if(prmarray->nParamType)
//		m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
//	else
//		m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

	prmarray->max=nConfig.Max[nParamCount];
	CtPair.nTest=nParamCount;

	//Commented:jj:08042001
	//graphicsinit();//Added:jj:05042001

	//commented by:sdt:01042001
	/*int max_prm=0;
	for (max_prm=0;max_prm < MAX_ARRAY_INDEX;max_prm++)
		for(int j=0;j<MAX_NO_READINGS;j++)
		{
				fNorValCR[max_prm][j]=(float)BLANKVALUE;
				fRawValCR[max_prm][j]=(float)BLANKVALUE;
		}
	 */
	InitialiseCALarray(); //Initialise the Cal array
	//Added by:sdt:04022007:2255
	if (m_FileInfo.cFixtureNo!=FIXTUREBOTH)
	{
		messageBox("Short the other end of conductor & connect to Ground Terminal",mfWarning|mfOKButton);
	}
	//Commented Temporary:sdt:28092005:1550
	//Uncommented by:sdt:09112006:2150 // To implement the Break test
	if(nConfig.cBreak=='Y' )//GBreak ) //Condition modified by:sdt:09112006:2150
	{
		//nConfig.cBreak='Y'; //Commented by:sdt:09112006:2155
		//closegraph();		  //Commented by:sdt:09112006:2155

		if( (fpSbTest=fopen("SbTest.Bin","wb"))!=NULL )
		{
			TestParam = (TESTPARAM *)malloc(sizeof(TESTPARAM));
			TestParam->Start=atoi(nConfig.szFixPos)-1;
			TestParam->End=TestParam->Start+atoi(nConfig.szNoofpairs)-1;
			TestParam->TestsToPerform=SRNO_BREAK;
			TestParam->SingleStep=0;

			if( !strcmp(nConfig.szFixtures,"Inner") )
				TestParam->FixtureNo=CL_INNER;
			if( !strcmp(nConfig.szFixtures,"Outer") )
				TestParam->FixtureNo=CL_OUTER;
			if( !strcmp(nConfig.szFixtures,"Both") )
				TestParam->FixtureNo=BOTH;
			strcpy(TestParam->szFilename,nConfig.szFileName);//Use File
			fwrite(TestParam,sizeof(TESTPARAM),1,fpSbTest);
			if(fpSbTest){fclose(fpSbTest);}
			ResetSystem();

			//system("BnS.exe"); //Commented by:sdt:09112006:2200
			spawnlp(P_WAIT,"BnS.exe","BnS.exe",NULL);//Added by:sdt:09112006:2200
			::remove( "SbTest.Bin" );//Added by:sdt:09112006:2200
			//system("del SbTest.Bin");//Commented by:sdt:09112006:2200
			free(TestParam);
		}
		else
		{
			//messagewindow("Can Not Create SBTEST.BIN!!!"); //Commented by:sdt:09112006:2155
			messageBox( "Can Not Create SBTEST.BIN!!!", mfError|mfOKButton );//Added by:sdt:09112006:2145
			sound(BEEP);
			delay(1000);
			nosound();
		}

		//InitialiseGraphics();//Commented by:sdt:09112006:2155
		//clearmenu();//Commented by:sdt:09112006:2155
		//graphicsinit();//Commented by:sdt:09112006:2155
		//ResetSystem ();//Commented by:sdt:09112006:2155

	   if( (BnSResult=fopen("BSResult.Bin","rb"))!=NULL )
	   {
			fread(&BSResult,sizeof(nConfig.BSResult),1,BnSResult);
			nConfig.BSResult&=BSResult;
			if(BnSResult){fclose(BnSResult);}
			::remove( "BSResult.Bin" );//Added by:sdt:09112006:2200
			//system("del BSResult.Bin");//Commented by:sdt:09112006:2155
	   }
	}
	TProgram::application->redraw(); //Added by:sdt:09112006:2210
	 //Commented by :sdt:31032001
	 //B'cause CR is freq. indepedent parameter.
	 //Also made the changes to make the LR & RU as Freq Independent
	 //Start
//    CheckForDefaultFreq(nParamCount,prmarray->nParamType);//Added by:sdt:23032001
//    for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:20032001
//    {
//    	if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum]) //Added by:sdt:20032001
//        {
	  //End
			//For loop added by:sdt:31032001
			//Added by:sdt:28092005:1635
			fPage = (TFrontPage *)(new TFrontPage( "CR TEST" ));
			if(fPage != 0)
			{
				//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
				//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
				//::fPageCreated = True;
				TProgram::deskTop->insert( fPage );
			}

			for(int i=0;i<LFFrequency.nTotalFrequencies;i++)
			{
				if(i==LFFrequency.nDefualtFreq)
				{
					nConfig.Specks.speckparams[nParamCount].freq[i]=1;
					nConfig.Specks.speckparams[SRNO_LR].freq[i]=1;
					nConfig.Specks.speckparams[SRNO_RU].freq[i]=1;
                }
	            else
                {
    	  			nConfig.Specks.speckparams[nParamCount].freq[i]=0;
                    nConfig.Specks.speckparams[SRNO_LR].freq[i]=0;
                    nConfig.Specks.speckparams[SRNO_RU].freq[i]=0;
                }
      		}

		    if(!param_tested[nParamCount][nFreqNum] || gnTest[nParamCount])//Added:jj:09042001
            {

            nConfig.nLFrequency = LFFrequency.nDefualtFreq;//nFreqNum; //Added by:sdt:20032001
            nFreqNum = LFFrequency.nDefualtFreq;
            //Added by:sdt:20032001
			if(prmarray->nParamType)
				m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
			else
				m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

			//setupbargraph(nParamCount,nFreqNum);//Commented by:sdt:28092005:1555
			//Limit of All Frequency indepedent parameters are stored at
			//index 0.Therefore frequency number passed as 0.

			//Commented by:sdt;01042001
			/*for (max_prm=0;max_prm < MAX_ARRAY_INDEX;max_prm++)
				for(int j=0;j<MAX_NO_READINGS;j++)
				{
					fNorValCR[max_prm][j]=(float)BLANKVALUE;
					fRawValCR[max_prm][j]=(float)BLANKVALUE;
				}
			*/
//Commented:jj:24032001
/*            //Changed by:sdt:20032001
			if ( strcmp(SpeckList[nFreqNum],"0.08 Hz") == 0 )
			{
				//strcpy(nConfig.szLFreq,"1KHz");
				for(int i=0;(i<LFFrequency.nTotalFrequencies)&&(strcmp(SpeckList[i],"1 KZ"));i++)
				nConfig.nLFrequency=i;
			}
*/
			//Commented by:sdt:20032001
			//	if ( strcmp(nConfig.szLFreq,"80Hz") == 0 )
			//    {
			//    	strcpy(nConfig.szLFreq,"1KHz");
			//        nConfig.nLFrequency=0;
			//    }
			//	else if ( strcmp(nConfig.szLFreq,"1KHz") == 0 )
			//       	nConfig.nLFrequency=0;
			//    else if ( strcmp(nConfig.szLFreq,"800Hz") == 0 )
			//		nConfig.nLFrequency=1;

			//SetLowFreq(nConfig.nLFrequency);

			CtPair.nTest=nParamCount;
			SetLowFreq(nConfig.nLFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			 delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			SetHighFreq(nConfig.nHFrequency);

			sprintf(path,"%s:\\cts.ini",cRamDrive);
			factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);
			nReturnVal = CalibrateOneParam();
			SetLowFreq(nConfig.nLFrequency);
            //Post Frequency Delay Added by:sdt:03082001
			 delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			nTested[nParamCount]=TRUE;

		 	nTested[SRNO_LR]=GetPrivateProfileInt("REPORT","LR",0,
											"default.INI");
			nTested[SRNO_RU]=GetPrivateProfileInt("REPORT","RU",0,
											"default.INI");

            //m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			fPage->hint(hcStartTesting);//Added by:sdt:28092005:1635
			nReturnVal=GetParamReadingsCR(nFreqNum);

			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//changed by:sdt:21032001
			}

			m_FileInfo.cParamTested[SRNO_LR][nFreqNum]=TRUE;
			m_FileInfo.cParamTested[SRNO_RU][nFreqNum]=TRUE;
			fPage->hint(hcStopTesting);//Added by:sdt:28092005:1635
//        }
//    	if(nReturnVal ==-1)
//        	break;
		}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}

int GetParamReadingsCR(int nFreqNum)
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1525
    CONFIGLIMIT configLimit;//Added by:sdt:23082001
    int LimitIndex;//Added by:sdt:23082001
    FILE *fpData1, *fpData2 ,*fpData;//Added:jj:18052001

    //Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
    unsigned long         ALocationSys;
	float                 ASysOffsetVal;

	char    path[80];//22012001
	int     nValidReadingTip=FALSE, nValidReadingRing=FALSE;
	int     cKey,first1=TRUE,first2=TRUE;
    int 	crFrst = TRUE;
    int 	crNord;

	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount, NotValid = 0;

	float   fRawReading = 0.0, fSysOffset=0.0;
	float   NormaliseTip = 0.0,NormaliseRing = 0.0;
	float   fRawReadingTip = 0.0,fRawReadingRing = 0.0;

	double  dRu=0.0,dRuAvg=0.0,dRuRms=0.0,dRuStdDev=0.0;
	double  dRaRaw=0,dRaNor=0,dRbRaw=0,dRbNor=0;
	double  dLrRaw=0.0,dLrNor=0.0,dLrAvg=0.0,dLrRms=0.0,dLrStdDev=0.0;
	double  dLrRawAvg=0.0,dLrRawRms=0.0,dLrRawStdDev=0.0;
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING TipInfoReading, InfoReading;
	struct  time prevtime;

	char    sztmp[20]; // for %d.Dat File
	unsigned char uRuMmtMethod=1;
	SSysOffsetVal m_SysOffsetVal;

	//Added by:sdt:03022007:1955:start
	float far *OnePerCrRaw;
	float far *OnePerCrNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerCrRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerCrNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerCrRaw,0,OnePerCount);
	memset(OnePerCrNorm,0,OnePerCount);
	//Added by:sdt:03022007:1955:End

	 //Added:sdt:01042001
	 float  far *fNorValCR[MAX_ARRAY_INDEX];//[MAX_NO_READINGS];
	 float  far  *fRawValCR[MAX_ARRAY_INDEX];//[MAX_NO_READINGS];

	 for(i=0;i<MAX_ARRAY_INDEX;i++)
	 {
		fNorValCR[i]=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
		fRawValCR[i]=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
		memset(fNorValCR[i],0,sizeof(float)*(nConfig.Max[nParamCount]+1));
		memset(fRawValCR[i],0,sizeof(float)*(nConfig.Max[nParamCount]+1));

	 }
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1525
	sprintf(path,"%s:\\cts.ini",cRamDrive);
	crNord = GetPrivateProfileInt("CR_RDGS","NORD",1,path);
	crFrst = crNord;
	uRuMmtMethod = (unsigned char) GetPrivateProfileInt("MEASUREMENT_OPTIONS",
					"RU_MMT_FORMULA_NO",1,"default.ini");

	//sprintf(sztmp,"%s:\\%d.dat",cRamDrive,SRNO_LR);//Commented by:sdt:20032001
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,SRNO_LR,nFreqNum);//Added by:sdt:20032001

    //Modified:jj:18052001
	//if((fpData[SRNO_LR][nFreqNum] = fopen(sztmp,"wb"))==NULL)
    if((fpData2 = fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		//getch();
		messageBox( "File creation error", mfError|mfOKButton );
		fs = fs_error;
		return 1;
	}
	//sprintf(sztmp,"%s:\\%d.dat",cRamDrive,SRNO_RU);//Commented by:sdt:20032001
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,SRNO_RU,nFreqNum);//Added by:sdt:20032001

    //Modified:jj:18052001
	//if((fpData[SRNO_RU][nFreqNum] = fopen(sztmp,"wb"))==NULL)
    if((fpData1 = fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		//getch();
		messageBox( "File creation error", mfError|mfOKButton );
		fs = fs_error;
		return 1;
	}

   // sprintf(sztmp,"%s:\\%d.dat",cRamDrive,nParamCount);//Commented by:sdt:20032001
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);//Added by:sdt:20032001

	bIncOkRej=1;

	//Modified:jj:18052001
	//if((fpData[nParamCount][nFreqNum] = fopen(sztmp,"wb"))==NULL)
	if((fpData= fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		//getch();
		messageBox( "File creation error", mfError|mfOKButton );
		fs = fs_error;
		return 1;
	}
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamCount][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]=0;

    m_FileInfo.nReadingsOk[SRNO_RU][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[SRNO_RU][nFreqNum]=0;
	m_FileInfo.nReadingsOk[SRNO_LR][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[SRNO_LR][nFreqNum]=0;


    SwitchCommonLines(ON);
	SwitchJunctionLines(ON);
	SelectReadingLines(nPos,ON);

	if(nflagRangeLock==TRUE)
	{
		nCurrentRange=(atoi(szRangeHold))-1;
		if(nCurrentRange==0)
			nCurrentRange=0;
		SelectRange(nCurrentRange,ON);
	}
	else
	{
		nCurrentRange=0;
		SelectRange(nCurrentRange,ON);
	}

    delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	//nBypass=SelectFixtureCR(1);
	nBypass=SelectFixtureSinglePair(1);//Added by:sdt:13112005:1515

	if(nBypass)
	{
		  while(nBypass)
		 {
				//nBypass=SelectFixtureCR(0);
				nBypass=SelectFixtureSinglePair(0);//Added by:sdt:13112005:1515
				nConfig.Max[nParamCount]--;
     			nConfig.Max[nParamCount]--;

		 }
	}

    delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );

	prmarray->max=nConfig.Max[nParamCount];

	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);

    nCurrentReadingno=0;//added by:sdt:21032001
    for(i=0;i<nConfig.Max[nParamCount];)
	{
		 cKey=getkey();
		 switch(cKey)
		 {
		 case  F7 :
                for(int Array_index=0;Array_index<MAX_ARRAY_INDEX;Array_index++)
			    {
					farfree(fNorValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
        			farfree(fRawValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
			    }

				flagReset = TRUE;
				return -1;
		 case  F8:
				StopPrint ();//Commented Temporary by:sdt:28092005:1558
				break;
		 default :
				break;
		 }
		 gettime(&currenttime);
		 //showtime(&currenttime);/Commented by:sdt:28092005:1558

		 do
		 {
			if (NotValid)
			{
				 SelectReadingLines(nPos,OFF);
				 if (bIncOkRej)
				 prmarray->ok-=2;
				 nCurrentReadingno-=2;
				 i-=2;
				 if(bIncOkRej)
				 {
					if(nValidReadingTip==VALID)
						m_FileInfo.nReadingsOk[nParamCount][nFreqNum]--;
					else
						m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]--;
				 }
				 dRawAvg-=fabs((double)fRawReadingTip);
				 dNorAvg-=fabs((double)NormaliseTip);
				 dRawRms-=fabs((double)(fRawReadingTip*fRawReadingTip));
				 dNorRms-=fabs((double)(NormaliseTip*NormaliseTip));

				 if(bIncOkRej)
				 {
					if(nValidReadingRing==VALID)
						m_FileInfo.nReadingsOk[nParamCount][nFreqNum]--;
					else
						m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]--;
				 }
				 dRawAvg-=fabs((double)fRawReadingRing);
				 dNorAvg-=fabs((double)NormaliseRing);
			}
			nPos=0;
			if(m_FileInfo.cFixtureNo==FIXTUREBOTH||
				m_FileInfo.cFixtureNo==FIXTURE2)
			{
				SetDMuxPin(312,ON);
				SetDMuxPin(314,ON);
			}
			else if ( m_FileInfo.cFixtureNo==FIXTURE1 )
			{
				SetDMuxPin(312+360,ON);
				SetDMuxPin(314+360,ON);
			}
			SendSegmentData(CtPair,m_FileInfo);
			SelectReadingLines(nPos,ON);
			delay( rdCtrl.m_anDelay[ DLY_RELAY ] );

			CtPair.TipOrRing=TIP;

			if(bIncOkRej)
				prmarray->ok++;
			nCurrentReadingno++;
			i++;
			nValidReadingTip=0;
			do
			{
				fRawReading = GetRawReadingCR (FALSE);
				if (first1)
				{
					first1=FALSE;
					fRawReading = GetRawReadingCR (FALSE);;
				}
				fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
				memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

				m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;
				// +1 coz fixture numbering starts from 1 in 'Sys.Off' file

				m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nLFrequency;
				m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
				m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;
				// - 1 since the Parameters starts from CR the number of which is 2
				//and  in the file  Sys.Off it start's from 1

				m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;
				// +1 since the Range numbering start's from 0 and in the file Sys.Off it start's from 1


				m_SysOffsetVal.m_SysOffsetLocation.nPos = CtPair.TipOrRing + 1;
			// + 1 Since in the file Tip is 1 and Ring is 2

				/*for (j = 0; j<1000;j ++)
					 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
						 break;
				fSysOffset=ASysOffsetVal [j];
				 //Getsysoffsetfromarray ends

				//debug.fSysOff = fSysOffset;
				fRawReading -=fSysOffset;*/
				//Added by:sdt:02042001
				if ((sys = fopen ("sys.off","rb")) != NULL)
				{

					while (fread (&OffsetValSys,sizeof (OffsetValSys),1,sys) > 0)
					{
						ASysOffsetVal  = OffsetValSys.fSysOffsetVal;
						ALocationSys  = OffsetValSys.lLocation;
						if (ALocationSys  == m_SysOffsetVal.lLocation)
							 break;
					}
					if(sys){fclose(sys);}

					//fSysOffset=ASysOffsetVal [j];
					fSysOffset=ASysOffsetVal ;
					//debug.fSysOff = fSysOffset;
					fRawReading -=fSysOffset;
				}
				fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
				fRawReadingTip = fRawReading;
				CtPair.norval=normaliseCR(fRawReading);
				CtPair.norval=User_ceil(CtPair.norval,prmarray->Decimal[0]);
				CtPair.val=fRawReading;
				NormaliseTip = CtPair.norval;
				//graphisuptodate=0;/Commented by:sdt:28092005:1558
				if ( !crFrst )
				{
					//commented by:sdt:01042001
					//graphicsupdate(nFreqNum);//Modified by:sdt:20032001
					//Modified:sdt:01042001
					//added by:sdt:02042001
					fRawValCR[CR_PARAM_INDEX][i]=fRawReading; // SRNO_CR - 2, Tip
					fNorValCR[CR_PARAM_INDEX][i]=CtPair.norval; // SRNO_CR - 2, Tip
					//graphicsupdate(nFreqNum,fNorValCR[CR_PARAM_INDEX]);//Commented by:sdt:28092005:1558
					//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:28092005:1558
					//nValidReadingTip=CheckHiLoLimits();
					//nValidReadingTip=CheckHiLoLimits(0);//Modified by:sdt:23082001
					fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:28092005:1638
					nValidReadingTip=CheckHiLoLimits(nFreqNum);//Modified by:sdt:24082001
				}
				switch(nValidReadingTip)
				{
					case  F7 :
						 for(int Array_index=0;Array_index<MAX_ARRAY_INDEX;Array_index++)
						 {
							farfree(fNorValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
							farfree(fRawValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
						 }
						 fcloseall();
						 flagReset = TRUE;
						 return -1;
					case  F8:
								 StopPrint ();//Commented Temporary by:sdt:28092005:1558
								 break;
						default :
								 break;
				}
				if ( crFrst )
		        {
               	 	crFrst--;
		            if ( crFrst == -1 )
          		      	nValidReadingTip = 1;
		            else
	                 	nValidReadingTip = 0;
     		   	}
		 	}while(!nValidReadingTip);
				if(m_FileInfo.cFixtureNo==FIXTUREBOTH||
					m_FileInfo.cFixtureNo==FIXTURE2)
				{
					SetDMuxPin(312,OFF);
					SetDMuxPin(313,ON);
				}
				else if ( m_FileInfo.cFixtureNo==FIXTURE1 )
				{
					SetDMuxPin(312+360,OFF);
					SetDMuxPin(313+360,ON);
				}
				if(bIncOkRej)
				{
					if(nValidReadingTip==VALID)
						m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
					else
						m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;
				}
				fRawValCR[CR_PARAM_INDEX][i]=fRawReading; // SRNO_CR - 2, Tip
				fNorValCR[CR_PARAM_INDEX][i]=CtPair.norval; // SRNO_CR - 2, Tip
				dRaRaw=CtPair.val;
				dRaNor=CtPair.norval;
				dRawAvg+=fabs((double)fRawReading);
				dNorAvg+=fabs((double)CtPair.norval);
				dRawRms+=fabs((double)(fRawReading*fRawReading));
				dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));

				if(i==1)
				{
					 StoreMinVal((float)fabs((double)fRawReading),nParamCount,nFreqNum,0);
					 StoreMaxVal((float)fabs((double)fRawReading),nParamCount,nFreqNum,0);
				}
				else
				{
                     SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1525
					 //if((float)CtPair.norval<(float)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin)
                     //Modified by:sdt:03092001:1525
			         if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
						StoreMinVal((float)fabs((double)fRawReading),nParamCount,nFreqNum,0);
					 //if((float)CtPair.norval>(float)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax)
                     //Modified by:sdt:03092001:1525
			         if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
						StoreMaxVal((float)fabs((double)fRawReading),nParamCount,nFreqNum,0);
				}
				//statusdisplay(nParamCount);
				//statusdisplay(nParamCount,nFreqNum);//Commented Temporary by:sdt:28092005:1558
				TipInfoReading.cPair1=CtPair.Pair1;
				TipInfoReading.cPair2=CtPair.Pair2;
				TipInfoReading.cParam=nParamCount;
                TipInfoReading.nFreq = nFreqNum;//Added by:sdt:23032001
				TipInfoReading.fRawVal=fRawReading;
				TipInfoReading.fNormVal=CtPair.norval;
                TipInfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
				TipInfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

				//Code for Worst Pair Next added here by:sdt:09022007:2225
				if(i<OnePerCount+1)
				{
					OnePerCrRaw[i-1]=fRawReading;
					OnePerCrNorm[i-1]=CtPair.norval;
				}
				else
				if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
				{
					SortArray(OnePerCrRaw,OnePerCount);
					InsertNumber(fRawReading,OnePerCrRaw,OnePerCount);
					SortArray(OnePerCrNorm,OnePerCount);
					InsertNumber(CtPair.norval,OnePerCrNorm,OnePerCount);
				}
				else
				{
					InsertNumber(fRawReading,OnePerCrRaw,OnePerCount);
					InsertNumber(CtPair.norval,OnePerCrNorm,OnePerCount);
				}



				SelectReadingLines(nPos,OFF);
				nPos=1;
				if(m_FileInfo.cFixtureNo==FIXTUREBOTH||
					m_FileInfo.cFixtureNo==FIXTURE2)
				{
					SetDMuxPin(313,ON);
					SetDMuxPin(314,ON);
				}
		          else if (m_FileInfo.cFixtureNo==FIXTURE1)
          		{
					SetDMuxPin(313+360,ON);
					SetDMuxPin(314+360,ON);
     		     }
				SelectReadingLines(nPos,ON);
				delay( rdCtrl.m_anDelay[ DLY_RELAY ] );

				CtPair.TipOrRing=RING;
				if(bIncOkRej)
					prmarray->ok++;
				nCurrentReadingno++;
				i++;
				nValidReadingRing=0;
				do
				{
					fRawReading=GetRawReadingCR(FALSE);
					if (first2)
					{
						first2=FALSE;
						fRawReading=GetRawReadingCR(FALSE);
					}
					fRawReading=User_ceil(fRawReading,prmarray->Decimal[0]);
					//fSysOffset = GetSysOffsetFromArray (nParamCount); by:sdt:06032001

					//Getsysoffsetfromarray start
					memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

					m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;
					// +1 coz fixture numbering starts from 1 in 'Sys.Off' file

					m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nLFrequency;
					m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
					m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;
					// - 1 since the Parameters starts from CR the number of which is 2
					//and  in the file  Sys.Off it start's from 1

					m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;
					// +1 since the Range numbering start's from 0 and in the file Sys.Off it start's from 1


					m_SysOffsetVal.m_SysOffsetLocation.nPos = CtPair.TipOrRing + 1;
					// + 1 Since in the file Tip is 1 and Ring is 2
					//Commented by:sdt:02042001
					/*for (j = 0; j<1000;j ++)
						 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
						 break;
					fSysOffset=ASysOffsetVal [j];
					//Getsysoffsetfromarray end

					fRawReading -=fSysOffset;*/
					//Added by:sdt:02042001
					if ((sys = fopen ("sys.off","rb")) != NULL)
					{

						while (fread (&OffsetValSys,sizeof (OffsetValSys),1,sys) > 0)
						{
							ASysOffsetVal  = OffsetValSys.fSysOffsetVal;
							ALocationSys  = OffsetValSys.lLocation;
							if (ALocationSys  == m_SysOffsetVal.lLocation)
								 break;
						}
						if(sys){fclose(sys);}

						//fSysOffset=ASysOffsetVal [j];
						fSysOffset=ASysOffsetVal ;
						fRawReading -=fSysOffset;
					}

					fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
					fRawReadingRing = fRawReading;
					CtPair.norval=normaliseCR(fRawReading);

					CtPair.norval=User_ceil(CtPair.norval,prmarray->Decimal[0]);
					CtPair.val=fRawReading;
					NormaliseRing = CtPair.norval;
					//graphisuptodate=0;//Commented Temporary by:sdt:28092005:1558
					//graphicsupdate(nFreqNum);//Modified by:sdt:20032001
					//Modified:sdt:01042001
					//graphicsupdate(nFreqNum,fNorValCR[CR_PARAM_INDEX]);//Commented by:sdt:28092005:1558
					//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:28092005:1558
					//nValidReadingRing=CheckHiLoLimits();
					//nValidReadingRing=CheckHiLoLimits(0);//Modified by:sdt:23082001
					fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:28092005:1640
					nValidReadingRing=CheckHiLoLimits(nFreqNum);//Modified by:sdt:24082001
					switch(nValidReadingRing)
					{
					case  F7 :
							for(int Array_index=0;Array_index<MAX_ARRAY_INDEX;Array_index++)
							{
								farfree(fNorValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
								farfree(fRawValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
							}
							fcloseall();
							flagReset = TRUE;
							 return -1;
					case  F8:
							 StopPrint ();//Commented by:sdt:28092005:1558
							 break;
					default :
							 break;
					}
				}while(!nValidReadingRing);

				if(m_FileInfo.cFixtureNo==FIXTUREBOTH||
					m_FileInfo.cFixtureNo==FIXTURE2)
				{
					SetDMuxPin(313,OFF);
					SetDMuxPin(312,ON);
				}
				  else if ( m_FileInfo.cFixtureNo==FIXTURE1 )
				{
					SetDMuxPin(313+360,OFF);
					SetDMuxPin(312+360,ON);
				}
				if(bIncOkRej)
				{
					if(nValidReadingRing==VALID)
						m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
					else
						m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;
				}
				fNorValCR[CR_PARAM_INDEX][i]=CtPair.norval; // SRNO_CR - 2,Ring
				fRawValCR[CR_PARAM_INDEX][i]=fRawReading; // SRNO_CR - 2,Ring

				dRawAvg+=fabs((double)fRawReading);
				dNorAvg+=fabs((double)CtPair.norval);
				dRbNor=fabs((double)CtPair.norval);
				dRbRaw=fabs((double)CtPair.val);
				dRu = CalculateRu( dRaNor, dRbNor, uRuMmtMethod );
				nValidReading=VALID;
				//nValidReading = CheckHiLoLimits (dRu, 1);
				//nValidReading = CheckHiLoLimits (0,dRu, 1);//Modified by:sdt:23082001
				//Commented by:sdt:23112005:2200 //As RU is not tested for QUAD software.
				//Uncommened by:sdt:02112006:2230 //As RU parameter is required for Pair Software.
				nValidReading = CheckHiLoLimits (nFreqNum,dRu, 1);//Modified by:sdt:23082001

				switch (nValidReading)
				{
					case  F7 :
							 for(int Array_index=0;Array_index<MAX_ARRAY_INDEX;Array_index++)
							 {
								farfree(fNorValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
								farfree(fRawValCR[Array_index]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
							 }
							 fcloseall();
							 flagReset = TRUE;
							 return -1;
					case  F8:
							 StopPrint ();//Commented Temporary by:sdt:28092005:1558
							 break;
					default :
							 NotValid = 1;
							 break;
				 }
			}while (!nValidReading);
			NotValid = 0;

			if(bIncOkRej)
			{
				if(nValidReading==VALID)
					m_FileInfo.nReadingsOk[SRNO_RU][nFreqNum]++;
				else
					m_FileInfo.nReadingsNotOk[SRNO_RU][nFreqNum]++;
			}
			dLrNor=dRaNor+dRbNor;
			dLrRaw=dRaRaw+dRbRaw;
			dLrAvg+=fabs(dLrNor);
			dLrRawAvg+=fabs(dLrRaw);

			dRuAvg+=fabs(dRu);
			dRuRms+=fabs(dRu*dRu);
			dLrRms+=fabs(dLrNor*dLrNor);
			dLrRawRms+=fabs(dLrRaw*dLrRaw);
			fRawValCR[LR_PARAM_INDEX][i]=dLrRaw; // SRNO_LR - 1
			fNorValCR[LR_PARAM_INDEX][i]=dLrNor; // SRNO_LR - 1

			fNorValCR[RU_PARAM_INDEX][i]=dRu; // SRNO_RU 0
			dLrNor=fabs((double)dLrNor);

			if(i==1)
			{
				 StoreMinVal((float)fabs((double)dLrRaw),SRNO_LR,nFreqNum,0);
				 StoreMaxVal((float)fabs((double)dLrRaw),SRNO_LR,nFreqNum,0);
			}
			else
			{
				 SummaryInfo=ReadSummary(SRNO_LR,nFreqNum,0);//Added by:sdt:03092001:1535
				 //if((float)dLrNor<(float)m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMin)
				 if((float)dLrNor<(float)SummaryInfo.STATNORMALISED.fMin)
						StoreMinVal((float)fabs((double)dLrRaw),SRNO_LR,nFreqNum,0);
				 if((float)dLrNor>(float)SummaryInfo.STATNORMALISED.fMax)
						StoreMaxVal((float)fabs((double)dLrRaw),SRNO_LR,nFreqNum,0);
			}
			nValidReading=VALID;
			//following Part Added by:sdt:23082001
			//start
			LimitIndex=GetLimitIndex(SRNO_LR); //Added by:sdt:11052001
			//configLimit=CovertSpeckLimitToConfigLimit(FindLimits(SRNO_LR,0,LimitIndex,0));
			//Modified by:sdt:24082001
			configLimit=CovertSpeckLimitToConfigLimit(FindLimits(SRNO_LR,nFreqNum,LimitIndex,0));
			if(configLimit.lowerlimit!=(float)BLANKVALUE)
			{
				if((float)fabs(dLrNor)<configLimit.lowerlimit)
					nValidReading=INVALID;
			}
			if(configLimit.upperlimit!=(float)BLANKVALUE)
			{
				if((float)fabs(dLrNor)>configLimit.upperlimit)
					nValidReading=INVALID;
			}
			//end
			//Commented by:sdt:23082001
			//start
			//Modified:jj:18042001 :specklimits struct added in config struct
			/*if(nConfig.Specks.speckparams[SRNO_LR].specklimits[0].lowerlimit!=(float)BLANKVALUE)
			{
				if((float)fabs(dLrNor)<nConfig.Specks.speckparams[SRNO_LR].specklimits[0].lowerlimit)
					nValidReading=INVALID;
			}
			if(nConfig.Specks.speckparams[SRNO_LR].specklimits[0].upperlimit!=(float)BLANKVALUE)
			{
				if((float)fabs(dLrNor)>nConfig.Specks.speckparams[SRNO_LR].specklimits[0].upperlimit)
					nValidReading=INVALID;
			}*/
			//end
			if(bIncOkRej)
			{
			if(nValidReading==VALID)
				 m_FileInfo.nReadingsOk[SRNO_LR][nFreqNum]++;
			else
				 m_FileInfo.nReadingsNotOk[SRNO_LR][nFreqNum]++;
			}
			dRawRms+=fabs((double)(fRawReading*fRawReading));
			dNorRms+=(CtPair.norval*CtPair.norval);
			if(i==2)
			{
				//              Store the Minimum and Maximum Values
				 //Commented by:sdt:03092001:1550
				 /*m_FileInfo.STATRAW[SRNO_RU][nFreqNum].cMinPair1=CtPair.Pair1;
				 m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fMin=(float)fabs(dRu);
				 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].cMinPair1=CtPair.Pair1;
				 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fMin=(float)fabs(dRu);
				 m_FileInfo.STATRAW[SRNO_RU][nFreqNum].cMaxPair1=CtPair.Pair1;
				 m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fMax=(float)fabs(dRu);
				 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].cMaxPair1=CtPair.Pair1;
				 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fMax=(float)fabs(dRu);

				 m_FileInfo.STATRAW[SRNO_LR][nFreqNum].cMinPair1=CtPair.Pair1;
				 m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fMin=(float)fabs(dLrRaw);
				 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].cMinPair1=CtPair.Pair1;
				 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMin=(float)fabs(dLrNor);
				 m_FileInfo.STATRAW[SRNO_LR][nFreqNum].cMaxPair1=CtPair.Pair1;
				 m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fMax=(float)fabs(dLrRaw);
				 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].cMaxPair1=CtPair.Pair1;
				 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMax=(float)fabs(dLrNor);*/
				 //SummaryInfo=ReadSummary(SRNO_RU,nFreqNum,0);//Added by:sdt:03092001:1550
				 memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
				 SummaryInfo.STATRAW.cMinPair1=CtPair.Pair1;
				 SummaryInfo.STATRAW.fMin=(float)fabs(dRu);
				 SummaryInfo.STATNORMALISED.cMinPair1=CtPair.Pair1;
				 SummaryInfo.STATNORMALISED.fMin=(float)fabs(dRu);
				 SummaryInfo.STATRAW.cMaxPair1=CtPair.Pair1;
				 SummaryInfo.STATRAW.fMax=(float)fabs(dRu);
				 SummaryInfo.STATNORMALISED.cMaxPair1=CtPair.Pair1;
				 SummaryInfo.STATNORMALISED.fMax=(float)fabs(dRu);
				 WriteSummary(SRNO_RU,nFreqNum,0,SummaryInfo);

				 //SummaryInfo=ReadSummary(SRNO_LR,nFreqNum,0);//Added by:sdt:03092001:1555
				 memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
				 SummaryInfo.STATRAW.cMinPair1=CtPair.Pair1;
				 SummaryInfo.STATRAW.fMin=(float)fabs(dLrRaw);
				 SummaryInfo.STATNORMALISED.cMinPair1=CtPair.Pair1;
				 SummaryInfo.STATNORMALISED.fMin=(float)fabs(dLrNor);
				 SummaryInfo.STATRAW.cMaxPair1=CtPair.Pair1;
				 SummaryInfo.STATRAW.fMax=(float)fabs(dLrRaw);
				 SummaryInfo.STATNORMALISED.cMaxPair1=CtPair.Pair1;
				 SummaryInfo.STATNORMALISED.fMax=(float)fabs(dLrNor);
				 WriteSummary(SRNO_LR,nFreqNum,0,SummaryInfo);
			}
			else
			{
				 SummaryInfo=ReadSummary(SRNO_RU,nFreqNum,0);//Added by:sdt:03092001:1555
				 //if(fabs(dRu)<fabs((double)m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fMin))
				 //Modified by:sdt:03092001:1555
				 if(fabs(dRu)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
				 {
						//Commented by:sdt:03092001:1555
						/*m_FileInfo.STATRAW[SRNO_RU][nFreqNum].cMinPair1=CtPair.Pair1;
						m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fMin=(float)fabs(dRu);
						m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].cMinPair1=CtPair.Pair1;
						m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fMin=(float)fabs(dRu);*/
						//Added by:sdt:03092001:1555
						SummaryInfo.STATRAW.cMinPair1=CtPair.Pair1;
						SummaryInfo.STATRAW.fMin=(float)fabs(dRu);
						SummaryInfo.STATNORMALISED.cMinPair1=CtPair.Pair1;
						SummaryInfo.STATNORMALISED.fMin=(float)fabs(dRu);
				 }
				 //if(fabs(dRu)>fabs((double)m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fMax))
				 //Modified by:sdt:03092001:1555
				 if(fabs(dRu)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
				 {
						//Commented by:sdt:03092001:1555
						/*m_FileInfo.STATRAW[SRNO_RU][nFreqNum].cMaxPair1=CtPair.Pair1;
						m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fMax=(float)fabs(dRu);
						m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].cMaxPair1=CtPair.Pair1;
						m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fMax=(float)fabs(dRu);*/
						//Added by:sdt:03092001:1555
						SummaryInfo.STATRAW.cMaxPair1=CtPair.Pair1;
						SummaryInfo.STATRAW.fMax=(float)fabs(dRu);
						SummaryInfo.STATNORMALISED.cMaxPair1=CtPair.Pair1;
						SummaryInfo.STATNORMALISED.fMax=(float)fabs(dRu);
				 }
				 WriteSummary(SRNO_RU,nFreqNum,0,SummaryInfo);//Added by:sdt:03092001:1600
				 SummaryInfo=ReadSummary(SRNO_LR,nFreqNum,0);//Added by:sdt:03092001:1555
				 //if(fabs(dLrNor)<fabs((double)m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMin))
				 //Modified by:sdt:03092001
				 if(fabs(dLrNor)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
				 {
						/*m_FileInfo.STATRAW[SRNO_LR][nFreqNum].cMinPair1=CtPair.Pair1;
						m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fMin=(float)fabs(dLrRaw);
						m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].cMinPair1=CtPair.Pair1;
						m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMin=(float)fabs(dLrNor);*/
						SummaryInfo.STATRAW.cMinPair1=CtPair.Pair1;
						SummaryInfo.STATRAW.fMin=(float)fabs(dLrRaw);
						SummaryInfo.STATNORMALISED.cMinPair1=CtPair.Pair1;
						SummaryInfo.STATNORMALISED.fMin=(float)fabs(dLrNor);
				 }
				 //if(fabs(dLrNor)>fabs((double)m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMax))
				 //Modified by:sdt:03092001:1605
				 if(fabs(dLrNor)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
				 {
						//Commented by:sdt:03092001
						/*m_FileInfo.STATRAW[SRNO_LR][nFreqNum].cMaxPair1=CtPair.Pair1;
						m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fMax=(float)fabs(dLrRaw);
						m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].cMaxPair1=CtPair.Pair1;
						m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fMax=(float)fabs(dLrNor);*/
						//Added by:sdt:03092001:1607
						SummaryInfo.STATRAW.cMaxPair1=CtPair.Pair1;
						SummaryInfo.STATRAW.fMax=(float)fabs(dLrRaw);
						SummaryInfo.STATNORMALISED.cMaxPair1=CtPair.Pair1;
						SummaryInfo.STATNORMALISED.fMax=(float)fabs(dLrNor);
				 }
				 WriteSummary(SRNO_LR,nFreqNum,0,SummaryInfo);//Added by:sdt:03092001:1600
			}

			SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1535
			//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
			//Modified by:sdt:03092001:1545
			if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
				 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
			//Modified by:sdt:03092001:1545
			if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
				 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
			//statusdisplay(nParamCount);
			//statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:28092005:1558

			InfoReading.cPair1=TipInfoReading.cPair1;
			InfoReading.cPair2=TipInfoReading.cPair2;
			InfoReading.cParam=TipInfoReading.cParam;
			InfoReading.nFreq = nFreqNum; //added by:sdt:23032001
			InfoReading.fRawVal=fRawReadingTip;
			InfoReading.fNormVal=CtPair.norval;
			InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
			InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001


			//Modified:jj:18052001
			//fwrite (&TipInfoReading,sizeof(TipInfoReading),1,fpData[nParamCount][nFreqNum]);
			fwrite (&TipInfoReading,sizeof(TipInfoReading),1,fpData);

			InfoReading.cPair1=CtPair.Pair1;
			InfoReading.cPair2=CtPair.Pair2;
			InfoReading.cParam=nParamCount;
			InfoReading.nFreq = nFreqNum; //Added by:sdt:23032001
			InfoReading.fRawVal=fRawReading;
			InfoReading.fNormVal=CtPair.norval;
			InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
			InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

			//Modified:jj:18052001
			//fwrite(&InfoReading,sizeof(InfoReading),1,fpData[nParamCount][nFreqNum]);
			fwrite(&InfoReading,sizeof(InfoReading),1,fpData);

			InfoReading.cPair1=CtPair.Pair1;
			InfoReading.cPair2=CtPair.Pair2;
			InfoReading.cParam=SRNO_LR;
			InfoReading.nFreq = nFreqNum; //Added by:sdt:23032001
			InfoReading.fRawVal=fRawValCR[LR_PARAM_INDEX][i];
			InfoReading.fNormVal=fNorValCR[LR_PARAM_INDEX][i];
			InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
			InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

			//Modified:jj:18052001
			//fwrite(&InfoReading,sizeof (InfoReading),1,fpData[SRNO_LR][nFreqNum]);
			fwrite(&InfoReading,sizeof (InfoReading),1,fpData2);

			InfoReading.cPair1=CtPair.Pair1;
			InfoReading.cPair2=CtPair.Pair2;
			InfoReading.cParam=SRNO_RU;
			InfoReading.nFreq = nFreqNum;//Added by:sdt:23032001
			//Since for Ru there is Nothing like Norm & UnNorm
			InfoReading.fRawVal=fNorValCR[SRNO_RU][i];
			InfoReading.fNormVal=fNorValCR[SRNO_RU][i];
			InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
			InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

			//Modified:jj:18052001
			//fwrite(&InfoReading,sizeof (InfoReading),1,fpData[SRNO_RU][nFreqNum]);
			fwrite(&InfoReading,sizeof (InfoReading),1,fpData1);

		 //Added :by:sdt:03022007:2000:start
		//Worst pair next calculations
		 if(i<OnePerCount+1)
		 {
			OnePerCrRaw[i-1]=fRawReading;
			OnePerCrNorm[i-1]=CtPair.norval;
		 }
		 else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
		 {
			SortArray(OnePerCrRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerCrRaw,OnePerCount);
			SortArray(OnePerCrNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCrNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerCrRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCrNorm,OnePerCount);
		 }
		 //Added :by:sdt:03022007:2000:End

			SelectReadingLines(nPos,OFF);


			//nBypass=SelectFixtureCR(0);
			nBypass=SelectFixtureSinglePair(0);//Added by:sdt:13112005:1515
			if(nBypass)
			{
				while(nBypass)
				{
					//nBypass=SelectFixtureCR(0);
					nBypass=SelectFixtureSinglePair(0);//Added by:sdt:13112005:1515
					nConfig.Max[nParamCount]--;
					 nConfig.Max[nParamCount]--;
				}
			}
		}

		if (m_FileInfo.cFixtureNo!=FIXTUREBOTH&&
		(gnTest[SRNO_CUPP]==TRUE || gnTest[SRNO_CUPG]==TRUE
		|| gnTest[SRNO_CM]==TRUE))
		{
			//sdt:28012006:Following while() loop is commented as it was
			//acting as indefinite loop.
			//int ch;//Commented by:sdt:28012006
			//while ((ch=getkey())!=27 && ch != 13)//Commented by:sdt:28012006
			{
				messageBox("Open other ends of conductors for C!!!",mfInformation|mfOKButton);
				/*messagewindow ("Open other ends of conductors for C!!!");
				delay (150);
				messagewindow (" ");
				delay (150);*/
			}
		}

		//Added by:sdt:03022007:2000:start
		Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerCrRaw[OnePerCount-1]);
		Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerCrNorm[OnePerCount-1]);
		farfree(OnePerCrRaw);
		farfree(OnePerCrNorm);
		//Added by:sdt:03022007:2000:End


		 dRuAvg/=(nConfig.Max[nParamCount]/2);
		 dRuRms/=(nConfig.Max[nParamCount]/2);
		 dRuRms=sqrt(dRuRms);
		 dLrAvg/=(nConfig.Max[nParamCount]/2);
		 dLrRawAvg/=(nConfig.Max[nParamCount]/2);
		 dLrRms/=(nConfig.Max[nParamCount]/2);
		 dLrRawRms/=(nConfig.Max[nParamCount]/2);
		 dLrRms=sqrt(dLrRms);
		 dLrRawRms=sqrt(dLrRawRms);
	    // End of Code to merge the files storing Lr Ru and Cr


		dRawAvg/=nConfig.Max[nParamCount];
		dNorAvg/=nConfig.Max[nParamCount];
		dRawRms/=nConfig.Max[nParamCount];
		dRawRms=sqrt(dRawRms);
		dNorRms/=nConfig.Max[nParamCount];
		dNorRms=sqrt(dNorRms);

		for(nSumCount=1;nSumCount<=nConfig.Max[nParamCount];nSumCount++)
		{
         		dRawStdDev+=pow((fabs((double)fRawValCR[nParamCount][nSumCount])-dRawAvg),2);
			 	dNorStdDev+=pow((fabs((double)fNorValCR[nParamCount][nSumCount])-dNorAvg),2);
        }


		 for(nSumCount=2;nSumCount<(nConfig.Max[nParamCount]);nSumCount+=2)
				dRuStdDev+=pow((fabs(fNorValCR[RU_PARAM_INDEX][nSumCount])-dRuAvg),2);
		 dRuStdDev/=(nConfig.Max[nParamCount]/2);
		 if(dRuStdDev)
				dRuStdDev/=sqrt(dRuStdDev);
         //Commented by:sdt:03092001:sdt:1610
         //start
		 /*m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fAvg=(float)fabs(dRuAvg);
		 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fAvg=(float)fabs(dRuAvg);
		 m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fRms=(float)fabs(dRuRms);
		 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fRms=(float)fabs(dRuRms);
		 m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fStddev=(float)fabs(dRuStdDev);
		 m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fStddev=(float)fabs(dRuStdDev);
         m_FileInfo.STATRAW[SRNO_RU][nFreqNum].fPowerSum=0.0;
         m_FileInfo.STATNORMALISED[SRNO_RU][nFreqNum].fPowerSum=0.0;*/
         //end
         //Added by:sdt:03092001:sdt:1615
         //start
         SummaryInfo=ReadSummary(SRNO_RU,nFreqNum,0);
         SummaryInfo.STATRAW.fAvg=(float)fabs(dRuAvg);
		 SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dRuAvg);
		 SummaryInfo.STATRAW.fRms=(float)fabs(dRuRms);
		 SummaryInfo.STATNORMALISED.fRms=(float)fabs(dRuRms);
		 SummaryInfo.STATRAW.fStddev=(float)fabs(dRuStdDev);
		 SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dRuStdDev);
         SummaryInfo.STATRAW.fPowerSum=0.0;
         SummaryInfo.STATNORMALISED.fPowerSum=0.0;
         WriteSummary(SRNO_RU,nFreqNum,0,SummaryInfo);
         //end
		 for(nSumCount=2;nSumCount<=(nConfig.Max[nParamCount]);nSumCount+=2)
				dLrStdDev+=pow((fabs(fNorValCR[SRNO_LR][nSumCount])-dLrAvg),2);
		 dLrStdDev/=(nConfig.Max[nParamCount]/2);
		 if(dLrStdDev)
				dLrStdDev/=sqrt(dLrStdDev);

		 for(nSumCount=2;nSumCount<=(nConfig.Max[nParamCount]);nSumCount+=2)
				dLrRawStdDev+=pow((fabs(fRawValCR[LR_PARAM_INDEX][nSumCount])-dLrRawAvg),2);
		 dLrRawStdDev/=(nConfig.Max[nParamCount]/2);
		 if(dLrRawStdDev)
				dLrRawStdDev/=sqrt(dLrRawStdDev);

         //Commented by:sdt:03092001:sdt:1615
         //start
		 /*m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fAvg=(float)fabs(dLrRawAvg);
		 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fAvg=(float)fabs(dLrAvg);
		 // Storing in fCr, LR values
		 //fCr[i]=m_FileInfo.STATRAW[SRNO_LR].fAvg;//Changed:Added:(08101999):P.N.B.

		 m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fRms=(float)fabs(dLrRawRms);
		 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fRms=(float)fabs(dLrRms);
		 m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fStddev=(float)fabs(dLrRawStdDev);
		 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fStddev=(float)fabs(dLrStdDev);
         m_FileInfo.STATRAW[SRNO_LR][nFreqNum].fPowerSum=0.0;
		 m_FileInfo.STATNORMALISED[SRNO_LR][nFreqNum].fPowerSum=0.0;*/
         //end
         //Added by:sdt:03092001:sdt:1615
         //start
         SummaryInfo=ReadSummary(SRNO_LR,nFreqNum,0);
         SummaryInfo.STATRAW.fAvg=(float)fabs(dLrRawAvg);
		 SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dLrAvg);
		 SummaryInfo.STATRAW.fRms=(float)fabs(dLrRawRms);
		 SummaryInfo.STATNORMALISED.fRms=(float)fabs(dLrRms);
		 SummaryInfo.STATRAW.fStddev=(float)fabs(dLrRawStdDev);
		 SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dLrStdDev);
         SummaryInfo.STATRAW.fPowerSum=0.0;
		 SummaryInfo.STATNORMALISED.fPowerSum=0.0;
         WriteSummary(SRNO_LR,nFreqNum,0,SummaryInfo);
         //end
		dRawStdDev/=nConfig.Max[nParamCount];
		if(dRawStdDev)
			 dRawStdDev=sqrt(dRawStdDev);
		dNorStdDev/=nConfig.Max[nParamCount];
		if(dNorStdDev)
			 dNorStdDev=sqrt(dNorStdDev);
        //Commented by:sdt:03092001:1620
        //start
		/*m_FileInfo.STATRAW[nParamCount][nFreqNum].fAvg=(float)fabs(dRawAvg);
		m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fAvg=(float)fabs(dNorAvg);
		m_FileInfo.STATRAW[nParamCount][nFreqNum].fRms=(float)fabs(dRawRms);
		m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fRms=(float)fabs(dNorRms);
		m_FileInfo.STATRAW[nParamCount][nFreqNum].fStddev=(float)fabs(dRawStdDev);
		m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fStddev=(float)fabs(dNorStdDev);
    	m_FileInfo.STATRAW[nParamCount][nFreqNum].fPowerSum=0.0;
		m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fPowerSum=0.0;*/
        //end
        //Added by:sdt:03092001:1620
        //Start
        SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);
        SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvg);
		SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvg);
		SummaryInfo.STATRAW.fRms=(float)fabs(dRawRms);
		SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRms);
		SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDev);
		SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDev);
    	SummaryInfo.STATRAW.fPowerSum=0.0;
		SummaryInfo.STATNORMALISED.fPowerSum=0.0;
        WriteSummary(nParamCount,nFreqNum,0,SummaryInfo);
        //end

		SwitchCommonLines(OFF);
		SwitchJunctionLines(OFF);
		SelectReadingLines(nPos,OFF);
		SelectRange(nCurrentRange,OFF);
		ResetSystem();

        //Modified:jj:18052001
		//if(fpData[SRNO_RU][nFreqNum]){fclose(fpData[SRNO_RU][nFreqNum]);}
        if(fpData1){fclose(fpData1);}
		//if(fpData[SRNO_LR][nFreqNum]){fclose(fpData[SRNO_LR][nFreqNum]);}
        if(fpData2){fclose(fpData2);}
		//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
        if(fpData){fclose(fpData);}

        //Added by:sdt:02042001
        for(i=0;i<MAX_ARRAY_INDEX;i++)
	    {
			farfree(fNorValCR[i]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
        	farfree(fRawValCR[i]);//=(float *)farmalloc(sizeof(float)*nConfig.Max[nParamCount]);
	    }
        param_tested[SRNO_CR][nFreqNum]=1;//Added:jj:06042001

	return nReadingno;
}

//Commented by:sdt:13112005:1515
/*int SelectFixtureCR (int nStart)
{
	int nBypass;
	if(nStart )
	{
		gflagReplace1 = 0;
		gflagReplace2 = 0;
		CtPair.Pair1=1;
		nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		nPrevPair2=0;
		nPrevPair1=0;
		CtPair.Pair2=0;
		nBypass=SetFixture(CtPair.Pair1,0,FALSE);
		CtPair.UnitSrNo=1;
		nPrevPair1 = CtPair.Pair1;
		nPrevPair2 = CtPair.Pair2;
	}
	else
	{
		CtPair.Pair1++;
		if (CtPair.Pair1 > nCount)
		{
			 CtPair.UnitSrNo++;
			 if(CtPair.UnitSrNo == nUnits)
			 {
				 nTempUnitOf = atoi(nConfig.szUnitof);
				 itoa(nLastUnitSize,nConfig.szUnitof,10);
			 }
			 nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		}
		nBypass=SetPair(CtPair.Pair1,0 ,CtPair.Pair1-1,0);
	}
	return nBypass;
}*/

float normaliseCR(float fReading)
{
	char 	szTag[50];  //Added by:sdt:04022007:2330
	int 	nMin_length; //Added by:sdt:04022007:2330

	float   fRoomTemp,fReturn;//,fLe,fSeq,fAtnAvg;//19032001
	unsigned int		nCableLength;

	float 	nor_length;
	//int 	uCuppMmtMethod;//19032001
    char    path[80];

	//nor_length = atof( speck.speckparams[nParamCount].norlength);
	//nor_length = speckData.fNormLength;
	//Modified by:sdt:31012007:2130
	nor_length = speckData.speckparams[nParamCount].fNormLength;
	sprintf(path,"%s:\\cts.ini",cRamDrive);
	if ( GetPrivateProfileInt("BASIC_INFO",
								"NORMALISE_KM",0,path) )
		nor_length = 1000.0;
	else if ( nor_length == (float)0.0 )
		nor_length = 1000.0;

	nCableLength=atoi(nConfig.szLength);

	//Added by:sdt:04022007:2330
	sprintf(szTag,"%s_MIN_LENGTH",paraminfo[nParamCount].szParamAbr);
	nMin_length=GetPrivateProfileInt("MINIMUM_CABLE_LENGTH",szTag,0,path);
	if((nMin_length!=0)&&(nCableLength<nMin_length))
	{
		nCableLength = nMin_length;
	}

	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commented by:sdt:13072001
	//Added by:sdt:13072001

	//Commented by:sdt:28092005:1640
	//if(atof(speck.speckparams[nParamCount].norTemp)!=0)
	//	fRoomTemp=atof(nConfig.szRoomtemp)-atof(speck.speckparams[nParamCount].norTemp);
	//else
	//	fRoomTemp=atof(nConfig.szRoomtemp)-20;
	if(speckData.fNormTemp!=0)
		fRoomTemp=atof(nConfig.szRoomtemp)-speckData.fNormTemp;
	else
		fRoomTemp=atof(nConfig.szRoomtemp)-20;

	fReturn=fReading;

	fReturn /=(float)( 1.0 + ( 0.00393 *(fRoomTemp)));
	fReturn *=(float)(nor_length/( (nCableLength) ? nCableLength : 1 )); // Substituted nor_length

    return  fReturn;
}


float GetRawReadingCR (int nRef)
{
	int         Huntup = 0, Huntlow = 0;//,nCalNo;//19032001
	float       fReading, Hysthigh, Hystlow;
	int         bWithinRange = FALSE,nSaveRange;

	if(nRef)
	{
		 SelectRange(nCurrentRange,OFF);
		 nSaveRange=nCurrentRange;
		 nCurrentRange=3;
		 SelectRange(nCurrentRange,ON);

         delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	}
	if(nflagRangeLock==TRUE && !nRef)
	{
		nCurrentRange=atoi(szRangeHold);
		--nCurrentRange;
		if(nCurrentRange==0)
			nCurrentRange=0;
		SelectRange(nCurrentRange,ON);


		delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

	 	fReading = GetAverageReading(nCurrentRange,nRef);
	}
	else
	{
		do
		{
			fReading = GetAverageReading(nCurrentRange,nRef);
			Hystlow=prmarray->fHystlow;
			Hysthigh=prmarray->fHysthigh;

			Hystlow*=prmarray->fEndofscale[nCurrentRange];
			Hysthigh*=prmarray->fEndofscale[nCurrentRange];


			if(Hystlow < fReading && Hysthigh >fReading)
				bWithinRange = TRUE;
			else
			{
				if(fReading > Hysthigh)
				{
					if(nCurrentRange < prmarray->nRanges-1)
					{
						SelectRange(nCurrentRange,OFF);
						nCurrentRange++;
						SelectRange(nCurrentRange,ON);

            			delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
						Huntup = 1;
					 }
					else
						bWithinRange=TRUE;
				}
				else
				{
					if(nCurrentRange > 0)
					{
						if((Huntup + Huntlow) == MAXALLOWEDHUNT)
							bWithinRange = TRUE;
						else
						{
							Huntlow = 1;
							SelectRange(nCurrentRange,OFF);
							nCurrentRange--;
							SelectRange(nCurrentRange,ON);

								delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
						}
					}
					else
						bWithinRange = TRUE;
				}
			}
		}while(!bWithinRange);
	}
	fReading = fReading-gfOffset[nCurrentRange];

	if(!nRef)
	{
		// for digi cm
		fReading *= fFcal[nCurrentRange];
	}
	if(nRef)
	{
		 SelectRange(nCurrentRange,OFF);
		 nCurrentRange=nSaveRange;
		 SelectRange(nCurrentRange,ON);

		 	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	}
	return fReading;
}

//#endif //added by:sdt:21082001