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
//#include <graphics.h>


#include "ct.h"
#include "share.h"
//#include "Stimer.h"
#include "rdsch.h"
#include "funproto.h"
#include "utils.h"
#include "key.h"
#include "frontpg.h"
#include <process.h>
//#include "menu.h"
#include "setup.h"

float ReadCM80HzLenMFs( unsigned int unCableLen ); //Added by:sdt:21122012:2000
int bCMRamptest=0; //Added by:sdt:13122016:2145
float fCM80HzMf=1.0;//Added by:sdt:21122016:2130

extern RunSetup runSetup; //Added by:sdt:28042015:2100

//Added by:sdt:03022007:1945
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];
//Added by:sdt:03092001

//Added:jj:06052001
extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];


//extern void ReadStdVal(int nParamCount);//Added:jj:26032001
//int getkey (void);
int CalibrateCmRamp();
int GetParamReadingsCMRamp(int); //modified by:sdt:20032001
int GetParamReadingsCM (int); //modified by:sdt:20032001
//int SelectFixtureCM (int nStart);
float GetRawReadingCM (int nRef);
float normaliseCM (float fReading);
float GetRawReadingCMRamp(int nRef);

extern Calibration  	Cal;
extern RMPINIINFO  	RAMPINI;
extern STRUCT_SPECKS speckData;
//extern void		SelectSplitForCal( );
//extern float GetAverageReadingSync(int nRange,int nRef);
//extern int SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
//extern void InitialiseCALarray(void);
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001

//Commented by:sdt:02042001
/*extern unsigned long  far       ALocationSys[1000];
extern float far                ASysOffsetVal[1000];*/
extern int          gflagReplace1, gflagReplace2 ;
//extern   			SPECIFICATION  	speck;
//extern int      	graphisuptodate;
extern int          nCurrentReadingno;
extern struct   	time currenttime;
extern char     	defaultmessage[];
extern char     	szRangeHold[2];
extern int      	nflagRangeLock;
//extern float 		User_ceil(float fFlotVal,int nNo);
extern int         	nTested[MAXPARAMETERS];
extern CTPAIR       CtPair;
extern RdControl	rdCtrl;
extern PRM			*prmarray;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
extern INFORMATION  m_FileInfo;
extern CONFIG       nConfig;
extern int 			GShort;
extern char 		cRamDrive[2];
extern int 			noofparameters;
extern int      	bSync;

//Commented:jj:18052001
//extern FILE 		*fpData[MAXPARAMETERS][MAXFREQUENCIES];

extern              FileStatus fs;
extern int      	flagReset;
extern int      	nParamCount;

extern int      	nTempUnitOf;
extern int          nLastUnitSize,nUnits;

extern int			nPrevPair1,nPrevPair2;
extern int 			nCount;
extern int 		   	ParamCtsHandle;
extern float		gfOffset[MAXRANGES];
extern float       	fFcal[MAXRANGES];

extern int			nCurrentRange;
extern int 			factor;
extern float 				RAMP_RANGE2_MF;

//Commented by:sdt:01042001
/*extern float far    fNorVal[MAX_NO_READINGS];
extern float far 	fRawVal[MAX_NO_READINGS];*/
extern char			bIncOkRej;
//extern char 		SpeckList[MAXFREQUENCIES][30]; //Added by:sdt:20032001
extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:20032001

extern TFrontPage *fPage; //Added by:sdt:27092005:2300

int CmTest()
{

	char	path[80];
	FILE	*fpSbTest;//Uncommented by:sdt:09112006:2245//Commented by:sdt:13112005:1405
	FILE *BnSResult;//Uncommented by:sdt:09112006:2245//Commented by:sdt:13112005:1405
	TESTPARAM *TestParam;//Uncommented by:sdt:09112006:2245//Commented by:sdt:13112005:1405
	unsigned char BSResult;//Uncommented by:sdt:09112006:2245//Commented by:sdt:13112005:1405
    int nFreqNum = 0; //added by:sdt:20032001
    nConfig.BSResult=0x03;
    nCount=0;
    int nReturnVal=0;

    nParamCount=SRNO_CM;
	//memset( prmarray, 0, sizeof( prmarray ) );
	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
    {
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	//ReadParamDotCts( paraminfo[nParamCount].szParamAbr, &prmarray );
	ReadParamDotCts( paraminfo[nParamCount].szParamAbr, prmarray );
    ReadStdVal(nParamCount); //Added:jj:26032001

    lseek ( ParamCtsHandle, 0L, SEEK_SET );
	lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );

    lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount , SEEK_CUR );
	read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
    close( ParamCtsHandle );

    prmarray->fAccuracyfsv = 0.05;
	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used
	//Commented by:sdt:20032001
//    if(prmarray.nParamType)
//		m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
//	else
//		m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

	prmarray->max=nConfig.Max[nParamCount];

    CtPair.nTest=nParamCount;

    //Commented:jj:08042001
    //graphicsinit();//Added:jj:05042001

    //Commented by:sdt:01042001
    /*for(int j=0;j<MAX_NO_READINGS;j++)
	{
		fNorVal[j]=(float)BLANKVALUE;
		fRawVal[j]=(float)BLANKVALUE;
	}*/

	InitialiseCALarray();
	//Commented Temporary by:sdt:27092005:1630
	//Uncommented by:sdt:09112006:2225
	if(nConfig.cShort=='Y')//GShort) //Condition modified by:sdt:09112006:2230
	{
		//nConfig.cShort='Y'; //Commented by:sdt:09112006:2230
		//closegraph();       //Commented by:sdt:09112006:2230

		if( (fpSbTest=fopen("SbTest.Bin","wb"))!=NULL )
		{
			TestParam = (TESTPARAM *)malloc(sizeof(TESTPARAM));

			TestParam->Start=atoi(nConfig.szFixPos)-1;
			TestParam->End=TestParam->Start+atoi(nConfig.szNoofpairs)-1;
			TestParam->TestsToPerform=SRNO_SHORT;
			TestParam->SingleStep=0;
			if( !strcmp(nConfig.szFixtures,"Inner") )
				TestParam->FixtureNo=CL_INNER;
			if( !strcmp(nConfig.szFixtures,"Outer") )
				TestParam->FixtureNo=CL_OUTER;
			if( !strcmp(nConfig.szFixtures,"Both") )
				TestParam->FixtureNo=BOTH;

			fwrite(TestParam,sizeof(TESTPARAM),1,fpSbTest);
			if(fpSbTest){fclose(fpSbTest);}

			ResetSystem();
			//system("BnS.exe");       //Commented by:sdt:09112006:2230
			//system("del SbTest.Bin");//Commented by:sdt:09112006:2230
			spawnlp(P_WAIT,"BnS.exe","BnS.exe",NULL);//Added by:sdt:09112006:2230
			::remove( "SbTest.Bin" );//Added by:sdt:09112006:2230
			free(TestParam);
		}
		else
		{
			//messagewindow("Can Not Create SBTEST.BIN!!!");//Commented by:sdt:09112006:2230
			messageBox( "Can Not Create SBTEST.BIN!!!", mfError|mfOKButton );//Added by:sdt:09112006:2230
			sound(BEEP);
			delay(1000);
			nosound();
		}

		//InitialiseGraphics();//Commented by:sdt:09112006:2230
		//clearmenu();         //Commented by:sdt:09112006:2230
		//graphicsinit();      //Commented by:sdt:09112006:2230
		//ResetSystem ();      //Commented by:sdt:09112006:2230

		if( (BnSResult=fopen("BSResult.Bin","rb"))!=NULL )
		{
			fread(&BSResult,sizeof(nConfig.BSResult),1,BnSResult);
			nConfig.BSResult&=BSResult;
			if(BnSResult){fclose(BnSResult);}
			::remove( "BSResult.Bin" );//Added by:sdt:09112006:2230
			//system("del BSResult.Bin"); //Commented by:sdt:09112006:2230
		}
	}
	TProgram::application->redraw(); //Added by:sdt:09112006:2230
	 //Commented:jj:06042001
	//CheckForDefaultFreq(nParamCount,prmarray.nParamType);//Added by:sdt:23032001

	//fPage = (TFrontPage *) validView(new TFrontPage( "RF TEST" ));
	fPage = (TFrontPage *)(new TFrontPage( "CM TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}
	sprintf(path,"%s:\\cts.ini",cRamDrive);
	//Added by:sdt:13122016:2145:start
	if ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) )
	{
		//messageBox( "Reading CM Ramp Delays & Ramp test", mfOKButton );
		rdCtrl.m_anDelay[DLY_INSTRUMENT] = GetPrivateProfileInt("CM_80HZ_CTDELAY_INFO","INSTRUMENT_DELAY",1000,path);
		rdCtrl.m_anDelay[DLY_JUNCTION] = GetPrivateProfileInt("CM_80HZ_CTDELAY_INFO","JUNCTION_DELAY",1000,path);
		rdCtrl.m_anDelay[DLY_RELAY] = GetPrivateProfileInt("CM_80HZ_CTDELAY_INFO","RELAY_DELAY",100,path);
		rdCtrl.m_anDelay[DLY_PREREAD] = GetPrivateProfileInt("CM_80HZ_CTDELAY_INFO","PRE_READ_DELAY",100,path);
		rdCtrl.m_anDelay[DLY_RANGE] = GetPrivateProfileInt("CM_80HZ_CTDELAY_INFO","RANGE_DELAY",100,path);
		rdCtrl.m_nAvgof=GetPrivateProfileInt("CM_80HZ_CTDELAY_INFO","AVERAGE_OF_READINGS",400,
											path);
		bCMRamptest=GetPrivateProfileInt ("BASIC_INFO","RAMP_TEST",0,path);

		fCM80HzMf=ReadCM80HzLenMFs(atoi(nConfig.szLength)  );//Added by:sdt:21122016:2130
		//char szStr[80];
		//sprintf(szStr,"Cable Length=%u MF=%f",atoi(nConfig.szLength),fCM80HzMf);
		//messageBox( szStr, mfOKButton );
	}
	//Added by:sdt:13122016:2145:start
	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:20032001
    {
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:27092005:2330
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum]) //Added by:sdt:20032001
		{
		    if(!param_tested[nParamCount][nFreqNum] || gnTest[nParamCount])//Added:jj:06042001
		  {
		  //Below statement added by:sdt:02072001
		  //If Last Unit size different ,below statement will restore the
		  //Original Unit size.
		  itoa (nTempUnitOf ,nConfig.szUnitof ,10);//san 21-11-97 For appending of the remaining pairs

		  nConfig.nLFrequency = nFreqNum; //Added by:sdt:20032001

		  //Added by:sdt:20032001
			if(prmarray->nParamType)
				m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
			else
				m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;


			CtPair.nTest=nParamCount;
			SetLowFreq(nConfig.nLFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			 delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			SetHighFreq(nConfig.nHFrequency);
			//setupbargraph(nParamCount,nFreqNum);//Modified by:sdt:20032001
			sprintf(path,"%s:\\cts.ini",cRamDrive);

			if( ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) )&&bCMRamptest)//Modified by:sdt:13122016:2145
			{
			    //char szStr[80];
			    //sprintf(szStr,"Ramp Avg before exge=%d",rdCtrl.m_nAvgof);
			    //messageBox( szStr, mfOKButton );
			    ExchangeRampInfo(TRUE);//Uncommented by:sdt:18122016:1200//Commented by:sdt:13122016:2145
			    //sprintf(szStr,"Ramp Avg after exge=%d",rdCtrl.m_nAvgof);
			    //messageBox( szStr, mfOKButton );
			   factor = GetPrivateProfileInt("COMPATIBILITY","RAMP_FACTOR",500,path);
			   CalibrateCmRamp();
			   if(bSync==1)
				nCurrentRange=1;
			   else
				nCurrentRange=0;
			SetLowFreq(nConfig.nLFrequency);
			 //Post Frequency Delay Added by:sdt:03082001
				 delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			    nTested[nParamCount]=TRUE;
				fPage->hint(hcStartTesting);//Added by:sdt:28092005:1105
				nReturnVal=GetParamReadingsCMRamp(nFreqNum);
				m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
				//Condtion Added by:sdt:28042015:2100
				if(runSetup.bAlarmsOff==False)
				{
					GiveAlarms(nFreqNum);//changed by:sdt:21032001
				}
				fPage->hint(hcStopTesting);//Added by:sdt:28092005:1105
				ExchangeRampInfo(FALSE); //Added by:sdt:18122016:1550
				//sprintf(szStr,"Ramp Avg after exge=%d",rdCtrl.m_nAvgof);
				//messageBox( szStr, mfOKButton );
		    }
		    else
		    {
				//Added by:sdt:13122016:2200:Start
				//Condition added to test higher length cable@80Hz instead of CM Ramp.
				if ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) )
				{
					nConfig.nLFrequency=0;
					SetLowFreq(nConfig.nLFrequency);
					//Post Frequency Delay Added by:sdt:03082001
					delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

				}
				//Added by:sdt:13122016:2200:End
				factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);
				CalibrateOneParam();
				nCurrentRange =0;

				SetLowFreq(nConfig.nLFrequency);
				//Post Frequency Delay Added by:sdt:03082001
				delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

				nTested[nParamCount]=TRUE;
				fPage->hint(hcStartTesting);//Added by:sdt:28092005:1105
				//m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
				if ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) )//Added by:sdt:18122016:1930:for CM80Hz
				{
					nCurrentRange +=2;
					//prmarray->nRanges-=2;
				}
				nReturnVal=GetParamReadingsCM(nFreqNum);

				if ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) )//Added by:sdt:18122016:1930:for CM80Hz
				{
					nCurrentRange -=2;
					//prmarray->nRanges+=2;
				}
				m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;

				//Condtion Added by:sdt:28042015:2100
				if(runSetup.bAlarmsOff==False)
				{
					GiveAlarms(nFreqNum);//changed by:sdt:21032001
				}
				fPage->hint(hcStopTesting);//Added by:sdt:28092005:1105
			}
		}
	if(nReturnVal ==-1)
		break;
	  }
	}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);
	return nReturnVal;
}

int CalibrateCmRamp()
{
	int      nRange, nFreqIndex, nRStart, nREnd;
	float    fInstStd;
	//int      cpParamCount;//19032001

    char path[80];
    char callow[5],calhigh[5];
	float t1,t2,CALLOW,CALHIGH;

	//messagewindow("System under calibration");
	//messageBox( "System under calibration", mfError|mfOKButton );
	fPage->hint(hcCalibrate);//Added by:sdt:28092005:1105

	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);

	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	SelectSplitForCal( );

	nRStart = nflagRangeLock==TRUE ? atoi(szRangeHold) - 1 : 0;
	nREnd   = nflagRangeLock==TRUE ? nRStart+1 : prmarray->nRanges;

	//nFreqIndex = SetParamFreq();//Commented by:sdt:20032001
    nFreqIndex = nConfig.nLFrequency;

	for(nRange=nRStart;nRange<nREnd;nRange++)
	{
    	SwitchOffsetLines(nRange,ON);
		SelectRange(nRange,ON);

		delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

        if(bSync==1)
        	gfOffset[nRange]  = GetAverageReadingSync(nRange+2,FALSE);
        else
			gfOffset[nRange]  = GetAverageReading(nRange+2,FALSE);

		SwitchOffsetLines(nRange,OFF);
		SelectRange(nRange,OFF);

        SwitchStdvalLines(nRange,ON);
		SelectRange(nRange,ON);

		delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

        if(bSync==1)
			fInstStd = GetAverageReadingSync(nRange+2,FALSE);
        else
        	fInstStd = GetAverageReading(nRange+2,FALSE);
        if ( nRange == 1 )
	    {
			//Commented by:sdt:21122016:2145:Start
			//char RampMF[4];
			//sprintf(path,"%s:\\cts.ini",cRamDrive);
			//GetPrivateProfileString("RAMP_RANGE2_MF","FACTOR",RampMF,"1.0",path);
			//RAMP_RANGE2_MF=atof(RampMF);
			//fInstStd = fInstStd * RAMP_RANGE2_MF;
			//Commented by:sdt:21122016:2145:End
			fInstStd = fInstStd * fCM80HzMf;//Added by:sdt:21122016:2145
          }

		SwitchStdvalLines(nRange,OFF);
		SelectRange(nRange,OFF);

		fInstStd = fInstStd - gfOffset[nRange];

		sprintf(path,"%s:\\cts.ini",cRamDrive);
		GetPrivateProfileString("CAL_STD_LIMITS","CALLOW",callow,"0.7",path);
		GetPrivateProfileString("CAL_STD_LIMITS","CALHIGH",calhigh,"1.3",path);
		CALLOW=atof(callow);
		CALHIGH=atof(calhigh);

		t1 = prmarray->fStdVal[nFreqIndex][nRange]*(float)CALLOW;
		t2 = prmarray->fStdVal[nFreqIndex][nRange]*(float)CALHIGH;
		if(fInstStd < t1 || fInstStd > t2)
		{
			fFcal[nRange] = (float)1.0;
			gfOffset[nRange]=0.0;
		}
		else
		{
			if (fInstStd == 0)
				fFcal[nRange] = (float)1.0;
			else
				fFcal[nRange] = (float)(prmarray->fStdVal[nFreqIndex][nRange]) / (fInstStd);
		}
	}

	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	ResetSystem();
	//messagewindow(defaultmessage);
	//messageBox( defaultmessage, mfError|mfOKButton );//Commented by:sdt:28092005:1110
	return 0;
}

int GetParamReadingsCM (int nFreqNum)
{

	SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1510
	FILE *fpData;//Added:jj:18052001

	//Added:jj:01042001
	//modified by:sdt:02042001
	float   far *fNorVal;//[MAX_NO_READINGS];
	float 	far *fRawVal;//[MAX_NO_READINGS];

	//Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
    unsigned long         ALocationSys;
	float                 ASysOffsetVal;

//	char    path[80];
	int     cKey,first1=TRUE;//,first2=TRUE;//19032001
	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount;//, NotValid = 0;//19032001

	float   fRawReading = 0.0, fSysOffset=0.0;
	//float   NormaliseTip = 0.0,NormaliseRing = 0.0;//19032001
	//float   fRawReadingTip = 0.0,fRawReadingRing = 0.0;////19032001

	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING InfoReading;//TipInfoReading,//19032001
	struct  time prevtime;

	char    sztmp[20];

	SSysOffsetVal m_SysOffsetVal;


	//Added by:sdt:03022007:1945:start
	float far *OnePerCmRaw;
	float far *OnePerCmNorm;
    int OnePerCount;
    OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerCmRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerCmNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerCmRaw,0,OnePerCount);
	memset(OnePerCmNorm,0,OnePerCount);
	//Added by:sdt:03022007:1945:End


    fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1510
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);//added by:sdt:20032001
	//	sprintf(sztmp,"%s:\\%d.dat",RamDrv,nParamCount);
	bIncOkRej=1;

	//Modified:jj:18052001
	//if((fpData[nParamCount][nFreqNum] = fopen(sztmp,"wb"))==NULL)
	if((fpData = fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		messageBox( "File creation error", mfError|mfOKButton );
		//getch();
		fs = fs_error;
		return 1;
	}
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamCount][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]=0;
	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);
	SelectReadingLines(nPos,ON);
	SelectRange(nCurrentRange,ON);

	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	//nBypass=SelectFixtureCM(1);
	nBypass=SelectFixtureSinglePair(1);

	if(nBypass)
	{
		  while(nBypass)
		 {
				//nBypass=SelectFixtureCM(0);
				nBypass=SelectFixtureSinglePair(0);
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
                    farfree(fNorVal);
					farfree(fRawVal);

					flagReset = TRUE;
					return -1;
	    case  F8:
					StopPrint ();//Commented temporary by:sdt:27092005:1655
					break;
		default :
					break;
		}
		gettime(&currenttime);
		//showtime(&currenttime);//Commented by:sdt:27092005:1910
		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			fRawReading=GetRawReadingCM(FALSE);
			if (first1 )   // all cm readings taken twice 2/12/98
			{
					first1 = FALSE;
					fRawReading=GetRawReadingCM(FALSE);
			}

			SendSegmentData(CtPair,m_FileInfo);
			fRawReading=User_ceil(fRawReading,prmarray->Decimal[0]);

			memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

			m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;
			m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nLFrequency;
			m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
			m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;

			m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;

			m_SysOffsetVal.m_SysOffsetLocation.Method = 3;
		  //Commented by:sdt:02042001
		  /*int j=0;
			for (j = 0; j<1000;j ++)
				 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
					 break;
			 //fSysOffset = GetSysOffsetFromArray (nParamCount);
		   fSysOffset =ASysOffsetVal [j];

			 fSysOffset = User_ceil(fSysOffset, prmarray.Decimal[0]);
			 fRawReading -= fSysOffset; */
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


				fSysOffset = User_ceil(fSysOffset, prmarray->Decimal[0]);
			    fRawReading -= fSysOffset;
            }

			 fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);
			 CtPair.val = fRawReading; //Added by:sdt:27092005:2240
			 CtPair.norval=normaliseCM(fRawReading);
			 CtPair.norval=User_ceil(CtPair.norval, prmarray->Decimal[0]);

			 //graphisuptodate=0;//Commented by:sdt:27092005:1655
			 //graphicsupdate(nFreqNum);//Modified by:sdt:20032001
			 //graphicsupdate(nFreqNum,fNorVal);//Modified:jj:01042001//Commented by:sdt:27092005:1655
			 //currentdisplay(fRawReading,nParamCount);//Commented by:sdt:27092005:1655
			 //nValidReading=CheckHiLoLimits();
			 fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:27092005:2300
			 nValidReading=CheckHiLoLimits(nFreqNum); //Added by:sdt:23082001
			 switch(nValidReading)
			 {
				case  F7 :
                     farfree(fNorVal);
				     farfree(fRawVal);

					 flagReset = TRUE;
					 return -1;
				case  F8:
					 StopPrint ();//Commented temporary by:sdt:27092005:1655
					 break;
				default :
					 break;
			 }
	     }while(!nValidReading);

		if(bIncOkRej)
		{
			if(nValidReading==VALID)
				m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
			else
				m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;
		}
		fNorVal[i]=CtPair.norval;
		fRawVal[i]=fRawReading;

		dRawAvg+=fabs((double)fRawReading);
		dNorAvg+=fabs((double)CtPair.norval);
		dRawRms+=fabs((double)(fRawReading*fRawReading));
		dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));
		if(i==1)
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		}
        SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1510
		//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
        //Modified by:sdt:03092001:1510
        if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
        //Modified by:sdt:03092001:1510
        if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		//statusdisplay(nParamCount);
		//statusdisplay(nParamCount,nFreqNum);//modified by:sdt:21032001//Commented by:sdt:27092005:1700
		InfoReading.cPair1=CtPair.Pair1;
		InfoReading.cPair2=CtPair.Pair2;
		InfoReading.cParam=nParamCount;
        InfoReading.nFreq = nFreqNum;//Added by:sdt:23032001
		InfoReading.fRawVal=fRawReading;
		InfoReading.fNormVal=CtPair.norval;
        InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
        InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

        //Modified:jj:18052001
		//fwrite(&InfoReading,sizeof(InfoReading),1,fpData[nParamCount][nFreqNum]);
		fwrite(&InfoReading,sizeof(InfoReading),1,fpData);

		//Added :by:sdt:03022007:1945:start
		//Worst pair next calculations
		 if(i<OnePerCount+1)
         {
			OnePerCmRaw[i-1]=fRawReading;
			OnePerCmNorm[i-1]=CtPair.norval;
         }
         else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
         {
			SortArray(OnePerCmRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerCmRaw,OnePerCount);
			SortArray(OnePerCmNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCmNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerCmRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCmNorm,OnePerCount);
		 }
		 //Added :by:sdt:03022007:1945:End

		//nBypass=SelectFixtureCM(0);
		nBypass=SelectFixtureSinglePair(0);
		if(nBypass)
		{
			 while(nBypass)
			 {
					//nBypass=SelectFixtureCM(0);
					nBypass=SelectFixtureSinglePair(0);
					nConfig.Max[nParamCount]--;
			 }
		}
	}
	//Added by:sdt:03022007:1950:Start
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerCmRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerCmNorm[OnePerCount-1]);
	farfree(OnePerCmRaw);
	farfree(OnePerCmNorm);
	//Added by:sdt:03022007:1950:End


	dRawAvg/=nConfig.Max[nParamCount];
	dNorAvg/=nConfig.Max[nParamCount];
	dRawRms/=nConfig.Max[nParamCount];
	dRawRms=sqrt(dRawRms);
	dNorRms/=nConfig.Max[nParamCount];
	dNorRms=sqrt(dNorRms);
	for(nSumCount=1;nSumCount<=nConfig.Max[nParamCount];nSumCount++)
	{
	 	dRawStdDev+=pow((fabs((double)fRawVal[nSumCount])-dRawAvg),2);
	 	dNorStdDev+=pow((fabs((double)fNorVal[nSumCount])-dNorAvg),2);
	}

	dRawStdDev/=nConfig.Max[nParamCount];
	if(dRawStdDev)
		 dRawStdDev=sqrt(dRawStdDev);
	dNorStdDev/=nConfig.Max[nParamCount];
	if(dNorStdDev)
		 dNorStdDev=sqrt(dNorStdDev);

    //Commented by:sdt:03092001:1515
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
    //Added by:sdt:03092001:1515
    //start
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
	//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
    if(fpData){fclose(fpData);}

    param_tested[SRNO_CM][nFreqNum]=1;//Added:jj:06042001

    //Added by:sdt:02042001
    farfree(fNorVal);
    farfree(fRawVal);
	return nReadingno;
}

float GetRawReadingCM (int nRef)
{
	int         Huntup = 0, Huntlow = 0;//,nCalNo;//19032001
	float       fReading, Hysthigh, Hystlow;
	int         bWithinRange = FALSE;//,nSaveRange;//19032001

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
	fReading = fReading-gfOffset[nCurrentRange];
	if(!nRef)
		fReading *= fFcal[nCurrentRange];
	/*if(nRef)
	{
		 SelectRange(nCurrentRange,OFF);
		 nCurrentRange=nSaveRange;
		 SelectRange(nCurrentRange,ON);

		de lay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	} */
	//Added by:sdt:21122016:2145:start
	if ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) )
	{
		fReading *= fCM80HzMf;
	}
	//Added by:sdt:21122016:2145:end
	return fReading;
}


float normaliseCM (float fReading)
{
	char 	szTag[50];  //Added by:sdt:04022007:2330
	int 	nMin_length; //Added by:sdt:04022007:2330
	char    path[80];
	float   fReturn;//fRoomTemp;
	unsigned int		nCableLength;
	float 	nor_length;

	//nor_length = atof( speck.speckparams[nParamCount].norlength);//Commented by:sdt:27092005:1725
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

	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commeted by sdt:28092005:1735
	//As fRoomTemp is not used in any calculations.
	fReturn=fReading;
	fReturn *=(float)(nor_length/( (nCableLength) ? nCableLength : 1 ));
	return fReturn;
}

//Commented by:sdt:13112005:1515
/*int SelectFixtureCM (int nStart)
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


int GetParamReadingsCMRamp(int nFreqNum)
{
	SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1720
	FILE *fpData; //Added:jj:18052001

	//Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
	unsigned long         ALocationSys;
	float                 ASysOffsetVal;

	//Added:jj:01042001
	//Modified by:sdt:02042001
	float   far *fNorVal;//[MAX_NO_READINGS];
	float 	far *fRawVal;//[MAX_NO_READINGS];

	int     cKey,first1=TRUE;//,first2=TRUE;//19032001
	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount;//, NotValid = 0;//19032001

	float   fRawReading = 0.0, fSysOffset=0.0;
	//float   NormaliseTip = 0.0,NormaliseRing = 0.0;//19032001
	//float   fRawReadingTip = 0.0,fRawReadingRing = 0.0;//19032001

	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING  InfoReading;//TipInfoReading,//19032001
	struct  time prevtime;

	char    sztmp[20];

	SSysOffsetVal m_SysOffsetVal;

	//Added by:sdt:03022007:1945:start
	float far *OnePerCmRaw;
	float far *OnePerCmNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerCmRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerCmNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerCmRaw,0,OnePerCount);
	memset(OnePerCmNorm,0,OnePerCount);
	//Added by:sdt:03022007:1945:End


	//Added by:sdt:02042001
	fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1720
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);//Added by:sdt:20032001
	//	sprintf(sztmp,"%s:\\%d.dat",RamDrv,nParamCount);
	bIncOkRej=1;

	//Modified:jj:18052001
	//if((fpData[nParamCount][nFreqNum] = fopen(sztmp,"wb"))==NULL)
	if((fpData = fopen(sztmp,"wb"))==NULL)
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
	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);
	SelectReadingLines(nPos,ON);
	SelectRange(nCurrentRange,ON);

	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	//nBypass=SelectFixtureCM(1);
	nBypass=SelectFixtureSinglePair(1);
	if(nBypass)
	{
		  while(nBypass)
		 {
				//nBypass=SelectFixtureCM(0);
				nBypass=SelectFixtureSinglePair(0);
				nConfig.Max[nParamCount]--;
		 }
	}

	delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );
	prmarray->max=nConfig.Max[nParamCount];

	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);
	for(i=0;i<nConfig.Max[nParamCount];)
	{
		cKey=getkey();
		switch(cKey)
		{
		case  F7 :
                    //Added by:sdt:02042001
                    farfree(fNorVal);
				    farfree(fRawVal);

					flagReset = TRUE;
					return -1;
	    case  F8:
					StopPrint ();//Commented Temporary by:sdt:27092005:1715
					break;
		default :
					break;
		}
		gettime(&currenttime);
		//showtime(&currenttime);//Commented by:sdt:27092005:1910
		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			fRawReading=GetRawReadingCMRamp(FALSE);
			if (first1 )   // all cm readings taken twice 2/12/98
			{
					first1 = FALSE;
					fRawReading=GetRawReadingCMRamp(FALSE);
			}

            SendSegmentData(CtPair,m_FileInfo);
			fRawReading=User_ceil(fRawReading,prmarray->Decimal[0]);

			memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

			m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;
			m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nLFrequency;
			m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
			m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;

			m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;

			m_SysOffsetVal.m_SysOffsetLocation.Method = 1;
            /*int j=0;
			for (j = 0; j<1000;j ++)
				 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
					 break;
			 //fSysOffset = GetSysOffsetFromArray (nParamCount);
             fSysOffset =ASysOffsetVal [j];

			 fSysOffset = User_ceil(fSysOffset, prmarray.Decimal[0]);
			 fRawReading -= fSysOffset;*/
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
				fSysOffset = User_ceil(fSysOffset, prmarray->Decimal[0]);
				fRawReading -= fSysOffset;
			 }
			 fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);
			 CtPair.val = fRawReading; //Added by:sdt:27092005:2240
			 CtPair.norval=normaliseCM(fRawReading);
			 CtPair.norval=User_ceil(CtPair.norval, prmarray->Decimal[0]);

			 //graphisuptodate=0;
			 //graphicsupdate(nFreqNum);//Modified by:sdt:20032001
			 //graphicsupdate(nFreqNum,fNorVal);//Modified:jj:01042001//commented by:sdt:27092005:1900
			 //currentdisplay(fRawReading,nParamCount);//commented by:sdt:27092005:1900
			 //nValidReading=CheckHiLoLimits();
			 fPage->updateFrontPage(PASS,UPDATE_RESULT,nFreqNum);//Added by:sdt:27092005:2300
			 nValidReading=CheckHiLoLimits(nFreqNum);//Modified by:sdt:23082001
			 switch(nValidReading)
			 {
				case  F7 :

                     //Added by:sdt:02042001
                	 farfree(fNorVal);
				     farfree(fRawVal);

					 flagReset = TRUE;
					 return -1;
				case  F8:
					 StopPrint ();//Commented by:Sdt:27092005:1715
					 break;
				default :
					 break;
			 }
	     }while(!nValidReading);

		if(bIncOkRej)
		{
			if(nValidReading==VALID)
				m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
			else
				m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;
		}
		fNorVal[i]=CtPair.norval;
		fRawVal[i]=fRawReading;

		dRawAvg+=fabs((double)fRawReading);
		dNorAvg+=fabs((double)CtPair.norval);
		dRawRms+=fabs((double)(fRawReading*fRawReading));
		dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));
		if(i==1)
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		}
        SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1510
		//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
        //Modified by:sdt:03092001:1721
        if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
        //Modified by:sdt:03092001:1721
        if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		//statusdisplay(nParamCount);
		//statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:27092005:1715
		InfoReading.cPair1=CtPair.Pair1;
		InfoReading.cPair2=CtPair.Pair2;
		InfoReading.cParam=nParamCount;
		InfoReading.nFreq = nFreqNum; //Added by:sdt:12122016:1530
		InfoReading.fRawVal=fRawReading;
		InfoReading.fNormVal=CtPair.norval;

        //Modified:jj:18052001
		//fwrite(&InfoReading,sizeof(InfoReading),1,fpData[nParamCount][nFreqNum]);
        fwrite(&InfoReading,sizeof(InfoReading),1,fpData);

		//Added :by:sdt:03022007:1945:start
		//Worst pair next calculations
		 if(i<OnePerCount+1)
		 {
			OnePerCmRaw[i-1]=fRawReading;
			OnePerCmNorm[i-1]=CtPair.norval;
         }
         else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
         {
			SortArray(OnePerCmRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerCmRaw,OnePerCount);
			SortArray(OnePerCmNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCmNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerCmRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCmNorm,OnePerCount);
		 }
		 //Added :by:sdt:03022007:1945:End

		//nBypass=SelectFixtureCM(0);
		nBypass=SelectFixtureSinglePair(0);
		if(nBypass)
		{
			 while(nBypass)
			 {
					//nBypass=SelectFixtureCM(0);
					nBypass=SelectFixtureSinglePair(0);
					nConfig.Max[nParamCount]--;
			 }
		}
	}

	//Added by:sdt:03022007:1950:Start
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerCmRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerCmNorm[OnePerCount-1]);
	farfree(OnePerCmRaw);
	farfree(OnePerCmNorm);
	//Added by:sdt:03022007:1950:End

	dRawAvg/=nConfig.Max[nParamCount];
	dNorAvg/=nConfig.Max[nParamCount];
	dRawRms/=nConfig.Max[nParamCount];
	dRawRms=sqrt(dRawRms);
	dNorRms/=nConfig.Max[nParamCount];
	dNorRms=sqrt(dNorRms);
	for(nSumCount=1;nSumCount<=nConfig.Max[nParamCount];nSumCount++)
	{
	 	dRawStdDev+=pow((fabs((double)fRawVal[nSumCount])-dRawAvg),2);
	 	dNorStdDev+=pow((fabs((double)fNorVal[nSumCount])-dNorAvg),2);
	}

	dRawStdDev/=nConfig.Max[nParamCount];
	if(dRawStdDev)
		 dRawStdDev=sqrt(dRawStdDev);
	dNorStdDev/=nConfig.Max[nParamCount];
	if(dNorStdDev)
		 dNorStdDev=sqrt(dNorStdDev);
	//Added by:sdt:03092001:1515
	//start
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
	//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
	if(fpData){fclose(fpData);}

	param_tested[SRNO_CM][nFreqNum]=1;//Added:jj:06042001

	farfree(fNorVal);
	farfree(fRawVal);

	return nReadingno;
}
float GetRawReadingCMRamp(int nRef)
{
	int         Huntup = 0, Huntlow = 0;//,nCalNo;//19032001
	float       fReading, Hysthigh, Hystlow;
	int         bWithinRange = FALSE;//,nSaveRange;//19032001
	do
	{
		if(bSync==1)
			fReading  = GetAverageReadingSync(nCurrentRange+2,nRef);
		else
			fReading = GetAverageReading(nCurrentRange+2,nRef);
		Hystlow=prmarray->fHystlow;
		Hysthigh=prmarray->fHysthigh;
		if ( bSync == 1 )
		{
			Hystlow=-MAXFLOAT;
			Hysthigh=MAXFLOAT;
		}
		else
		{
			Hystlow*=prmarray->fEndofscale[nCurrentRange+2];
			Hysthigh*=prmarray->fEndofscale[nCurrentRange+2];
		}
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

	fReading = fReading-gfOffset[nCurrentRange];

	if(!nRef)
	{
		// for digi cm
		fReading *= fFcal[nCurrentRange];
	}
	/*if(nRef)
	{
		 SelectRange(nCurrentRange,OFF);
		 nCurrentRange=nSaveRange;
		 SelectRange(nCurrentRange,ON);

		 delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	} */

	if ( nCurrentRange == 1 )
	{
		//fReading = fReading * RAMP_RANGE2_MF;
		//Modified by:sdt:21122016:2145
		fReading = fReading * fCM80HzMf;
	}
	return fReading;
}

//Added by:sdt:21122016:2035:Start
float ReadCM80HzLenMFs( unsigned int unCableLen )
{
// Variable     : ATNVarLenOffs  :-  CMVarLenMF[MAX_CAB_TYPE];
// Fields       : nUpperLimit, nLowerLimit - int, fOffset - Float

	char    path[80];
	int     i;
	char    szTmp[MAXSTRINGLENGTH1];
	char    cTempStr[20];
	char    *cTempStrPtr,*cTempPtr;
	int     nTempNo_L, nTempNo_U;
	char    cTempStr1[30];
	int     flag;
	int nCMNoofSpans;
	CMVarLenMfs  far *CMVarLenMF;
	float fLenMF=1.0;

	sprintf(path,"%s:\\cts.ini",cRamDrive);
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
//Added by:sdt:21122016:2035:END



//#endif //Added by:sdt:21082001