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
#include "key.h"
#include "share.h"
#include "Stimer.h"
#include "rdsch.h"
//#include "menu.h"
#include "seq.h" //added by:sdt:23042001
#include "funproto.h"
#include "frontpg.h"
#include "utils.h" //Added by:sdt:07122005:2230
#include "setup.h"

extern RunSetup runSetup; //Added by:sdt:28042015:2100

extern char 	szZchAutoManual[8] ;//Added:MOD11082002:01:jj
extern int 		ZchHfAray[5][MAXFREQUENCIES];//Added:MOD11082002:01:jj

//Added by:sdt:03092001
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);

//Commented by:sdt:04102005:2230
//extern float HF_Offset[MAXFREQUENCIES][MAXRANGES];
//extern float HF_CalFact[MAXFREQUENCIES][MAXRANGES];
//extern float HF_RefCalFact[MAXFREQUENCIES];
//Added by:sdt:18062001
//void SetImpedence(int Val);

//Added by:sdt:06062001
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];


//added by:sdt:11052001
//void ChangeLimits(int nParam,int nFreqNum,float fNorVal[]);//Modified by:sdt:23082001
int GetLimitIndex(int Param);
//Added by:sdt:08052001
void InsertNumber(float Val,float *Array,int Arraysize);
void SortArray(float *Array,int Arraysize);
//float WorstOnext[MAXFREQUENCIES][2];//Commented by:sdt:06062001

//Added by:sdt:26042001
extern int nAdjNonAdjFlag; //0-Adjacent 1-NonAdjacent.

//Added by:sdt:24042001
int SelectFixturePairtoPair (int nStart,int nCombnType);
int SelectFixtureXTalkInQuad (int nStart,int nCombnType);
//Added by:sdt:23042001
extern SEQPAIR *SeqPair;
extern PAIRSWITCH PairSwitchOption;
extern SEQUENCE CableSequence;
extern int nBothFlag;

//Added:jj:09042001
//extern int ZchHfAray[3][MAXFREQUENCIES];
extern int CurrentHfParam;
extern int CurrentFreq;
//extern int ZchVal;//Commented by:sdt:16102005:1840 //As it is no longer used
//extern int ZchValArray[MAXFREQUENCIES];//Commented by:sdt:16102005:1840 //As it is no longer used
//extern getZchVal();//Commented by:sdt:04102005:1850

//Added:jj:06052001
extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];

//extern void ReadStdVal(int nParamCount);//Added:jj:26032001
int GetParamReadingsNEXT (int); //modified by:sdt:19032001
int SelectFixtureNEXT (int nStart);
float GetRawReadingNEXT (int nRef);
float normaliseNEXT (float fReading,int nFreqNum);


//extern READING far* GetPrevParamVals( READING far *pparamVal, int param );
//extern READING far* FreePrevParamVals( READING far *pparamVal, int param );

extern Calibration  	Cal;
extern float    fAtnCalRef;
extern int 		CalibrateOneParamATN();
//extern void		SelectSplitForCal( );
//extern int     	SetParamFreq();
//extern int 		SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
//extern void 	InitialiseCALarray(void);
//extern float   	GetFixOffsetFromArray (int, int, int, int );
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001

//extern READING far		*fAtnPairVal;
extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:19032001:1215
extern int 			factor;
//extern float       	fRefVal[120];//Commented by:sdt:23072001
//extern float          	fRefVal[110][MAXFREQUENCIES];// Added by:sdt:23072001
//Modified by:sdt:16102005:1715 //For 6 Quad cable
extern float          	fRefVal[MAXPAIRS][MAXFREQUENCIES];// Added by:sdt:23072001

extern int          ElfextLength;
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
extern float 		User_ceil(float fFlotVal,int nNo);
extern   int       	nTested[MAXPARAMETERS];
extern CTPAIR       CtPair;
extern RdControl	rdCtrl;
extern PRM			*prmarray;
extern PARAMINFO	paraminfo[ MAXPARAMETERS+2 ];//Modified by:sdt:31072001
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
extern int 			factor;

//Commented by:sdt:01042001
/*extern float far	fNorVal[MAX_NO_READINGS];
extern float far	fRawVal[MAX_NO_READINGS];*/
extern char 		bIncOkRej;
extern int			nCurrentRange;

extern STRUCT_SPECKS speckData;

extern TFrontPage *fPage; //Added by:sdt:27092005:2300


int NextTest()
{
	char	path[80];
	nCount=0;
	int nReturnVal=0;
	int nFreqNum = 0; //added by:sdt:19032001
	nParamCount=SRNO_NEXT;
	//Moved here by:sdt:26072006:2225
	fPage = (TFrontPage *)(new TFrontPage( "NEXT TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}

	//Added by:sdt:26072006:2225:start
	//As Fext at low frequency is calculated from CUPP, ATN, ZCH.
	//Hence calibration and other code is not neccessary.
	//therefore this code is at the starting only.
	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
	{
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum])
		{
			nReturnVal = nfXTalkAtLowFreq(nParamCount,nFreqNum);
			if(nReturnVal ==-1)
			break;
		}
	}
	//Added by:sdt:28072006:2225
	if(nReturnVal ==-1)
	{
		TView::destroy(fPage->tp);
		TWindow::destroy(fPage);
		return nReturnVal;
	}

	//Added by:sdt:26072006:end

	//memset( prmarray, 0, sizeof( prmarray ) );
	nParamCount=SRNO_NEXT;
	//***********Impoertant Modifification sdt:10102006**************
	//Condition is removed to remove the bug, as explained below.
	//Before measuring NEXT at 150 KHz, we calculates Next for Low Frequencies.
	//While calculating this, CUPP is measured in background & its value used
	//in calculation. While measurement of this CUPP, calibration is done.
	//Due calibration it overwrites the offset measured while measuring ATN.
	//Thus CUPP offsets were used in measurement of Next and giving wrong results.
	//To remove this bug, calibration of ATN done before measurement of NEXT at
	//150 Khz. This will again measures the offset and use it in measurement
	//NEXT at 150KHz.
	//Affected variable gfOffset[]
	//if (gnTest[SRNO_ATN] != 1 )//Commented by:sdt:10102006:2145
	//Modification done by:sdt:23022007:0715
	//For() & if() condition here. This conditions added to solve the problem of
	//unneccessary calibration after testing NEXT at low freq.
	//Reason- When NEXT is Selected only at LF, it will test it. As there was no
	//condition was there for calibration it will calibrate the system for
	//HF, evevnthough any HF Freq not selected. To avoid this situation
	//this condition is added. A "break" statement is added here so that
	//calibration will be done only once.
	for(nFreqNum=LFFrequency.nTotalFrequencies;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
	{
		if(nConfig.Specks.speckparams[SRNO_NEXT].freq[nFreqNum])
		{
			nParamCount=SRNO_ATN;
			if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
			{
				clrscr();
				printf( "\nError Opening File PARAM.CTS File for Reading..." );
				//RemoveIniFiles();
				exit( 1 );
			}


			ReadParamDotCts( paraminfo[SRNO_ATN].szParamAbr, prmarray );
			ReadStdVal(nParamCount); //Added:jj:26032001

			lseek ( ParamCtsHandle, 0L, SEEK_SET );
			lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );

			lseek ( ParamCtsHandle, sizeof( RdControl ) * SRNO_ATN , SEEK_CUR );
			read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
			close( ParamCtsHandle );

			InitialiseCALarray();
			SetHighFreq(nConfig.nHFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );

			factor = 100;
			//SelectReadingLines(0,ON); //Added by:sdt:19072001//Commented by:sdt:28072001
			SelectReadingLines(0,OFF); //Modified by:sdt:28072001
			nReturnVal = CalibrateOneParamATN();//Uncommneted by:sdt:04102005:1920:for Quad //Commented by:sdt:31072001
			break; //Added by:sdt:23022007:0715
		}
	}


	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
	{
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	nParamCount=SRNO_NEXT;
	ReadParamDotCts( paraminfo[nParamCount].szParamAbr, prmarray );
	lseek ( ParamCtsHandle, 0L, SEEK_SET );
	lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );
	lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount , SEEK_CUR );
	read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
	close( ParamCtsHandle );
	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used

	//Uncommented by:sdt:04102005:1910 :for Quad//Commented by:sdt:19032001
	//if(prmarray->nParamType)
	//	m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
	//else
	//	m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

	prmarray->max=nConfig.Max[nParamCount];

	CtPair.nTest=nParamCount;

	//Commented:jj:08042001
	//graphicsinit();//Added:jj:05042001

	 //Commented:jj:06042001
	//CheckForDefaultFreq(nParamCount,prmarray->nParamType);//Added by:sdt:23032001
	//Added by:sdt:04102005:2025

	//for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:19032001
	//Modified by:sdt:26072006:2225
	for(nFreqNum=LFFrequency.nTotalFrequencies;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)  //Modified by:sdt:17062006
	{
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:04102005:2025
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum]  ) //Added by:sdt:19032001
		{
			if(!param_tested[nParamCount][nFreqNum]  || gnTest[nParamCount])//Added:jj:06042001
			{
			//Below statement added by:sdt:02072001
			//If Last Unit size different ,below statement will restore the
			//Original Unit size.
			itoa (nTempUnitOf ,nConfig.szUnitof ,10);//san 21-11-97 For appending of the remaining pairs

			nConfig.nHFrequency = nFreqNum; //Added by:sdt:19032001


			//Added:jj:09042001
			//ZchVal=ZchValArray[nFreqNum];//Commented:11082002:01:jj
			CurrentHfParam=1;
			CurrentFreq=nFreqNum;
			//getZchVal();

			//Added by:sdt:19032001
			if(prmarray->nParamType)
				m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
			else
				m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

			//setupbargraph(nParamCount,nFreqNum);//Commented by:sdt:04102005:1855
			 //Commented by:sdt:04102005:1855:start //For Quad Zch is not required
			//ZchVal=ZchValArray[nFreqNum];//Moved:MOD11082002:01:jj
			//if(ZchVal<10 || ZchVal >2550 || !strcmp(szZchAutoManual,"Manual")) //Added:MOD11082002:01:jj
			//	getZchVal();
			//GetActualImpedence(); //Added:MOD11082002:02:jj

			//ZchHfAray[CurrentHfParam][nFreqNum]=ZchVal;//Added:MOD11082002:01:jj
			//ZchValArray[nFreqNum]=ZchVal;
			//Commented by:sdt:04102005:1855:end
			/*for(int j=0;j<MAX_NO_READINGS;j++)
			{
				fNorVal[j]=(float)BLANKVALUE;
				fRawVal[j]=(float)BLANKVALUE;
			} */
			sprintf(path,"%s:\\cts.ini",cRamDrive);
			factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);

			SetHighFreq(nConfig.nHFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );

			CtPair.nTest=nParamCount;

		   // nCurrentRange =0;

		    nTested[nParamCount]=TRUE;
			fPage->hint(hcStartTesting);//Added by:sdt:04102005:2025
			nReturnVal=GetParamReadingsNEXT(nFreqNum);//modified by:sdt:19032001
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;

			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//Changed by:sdt:21032001
			}
			fPage->hint(hcStopTesting);//Added by:sdt:04102005:2030
		}
        }
    	if(nReturnVal ==-1)
        	break;
	}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}

int GetParamReadingsNEXT (int nFreqNum)
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:03092001
    char *szSwitchOption;//Added by:sdt:03092001
	//int nAdjCount=0,nNonAdjCount=0;//Commented by:sdt:04102005:2230
    FILE *fpData;//Added:jj:18052001

    //Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal         OffsetValSys;
    unsigned long         ALocationSys;
	float                 ASysOffsetVal;
	float 			fAdjPairsOffset=0.0;   //added by:sdt:07122005:1010 //For Adj pair offset
	char				strAdjPairsOffset[10]; //added by:sdt:07122005:1010 //For Adj pair offset

    //Added:sdt:01042001
	float 	far *fNorVal;//[MAX_NO_READINGS];
	float 	far *fRawVal;//[MAX_NO_READINGS];

	int     cKey;
	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass;
	int     nSumCount;
	float   fRawReading,fV1,fV2, fSysOffset, fFixOffset;
	//int     NextTestPair;//19032001
	int     First = TRUE;
	//Uncommented by:sdt:04102005:1930
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;
	double  dNorPowerSum=0.0,dRawPowerSum=0.0;
	//Commented by:sdt:04102005:1930
	/*double  dRawAvgAdj=0.0,dRawRmsAdj=0.0,dRawStdDevAdj=0.0;
	double  dNorAvgAdj=0.0,dNorRmsAdj=0.0,dNorStdDevAdj=0.0;
	double  dNorPowerSumAdj=0.0,dRawPowerSumAdj=0.0;
	double  dRawAvgNonAdj=0.0,dRawRmsNonAdj=0.0,dRawStdDevNonAdj=0.0;
	double  dNorAvgNonAdj=0.0,dNorRmsNonAdj=0.0,dNorStdDevNonAdj=0.0;
	double  dNorPowerSumNonAdj=0.0,dRawPowerSumNonAdj=0.0;

	//MOD08072002
	double  dRawAvgAllComb=0.0,dRawRmsAllComb=0.0,dRawStdDevAllComb=0.0;
	double  dNorAvgAllComb=0.0,dNorRmsAllComb=0.0,dNorStdDevAllComb=0.0;
	double  dNorPowerSumAllComb=0.0,dRawPowerSumAllComb=0.0;*/


	int     /*nCableLength=atoi(nConfig.szLength),//19032001*/first = TRUE;

	READING InfoReading;
	char sztmp[20];
	struct time prevtime;
    SSysOffsetVal m_SysOffsetVal;
	//int	StartSwitchMode;//=GetLimitIndex(nParamCount); //added by:11052001//Commented by:sdt:03092001
	//Added by:sdt:08052001
    float far *OnePerOnextRaw;
    float far *OnePerOnextNorm;
    int OnePerCount;
    OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
    OnePerOnextRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
    OnePerOnextNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
    memset(OnePerOnextRaw,0,OnePerCount);
    memset(OnePerOnextNorm,0,OnePerCount);
    //memset(Worstpairnext.Onext,0,MAXFREQUENCIES*2);//changed by:sdt:06062001// Commented:MOD24072002:jj


    //Added by:sdt:02042001
    fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    //Added by:sdt:03092001:1240
    //start
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
    szSwitchOption=(char *)farmalloc(sizeof(char)*nConfig.Max[nParamCount]);
    memset(szSwitchOption,0,sizeof(char)*nConfig.Max[nParamCount]);
    //end

     //start:added by:sdt:07122005:2215 //For reading offset for Adjacent Pairs
	char path[80];
	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	GetPrivateProfileString ("ONEXT_OFFSET","ADJ_PAIR_ONEXT_OFFSET_VOLTAGE",strAdjPairsOffset,"0.0",path);
	fAdjPairsOffset=atof(strAdjPairsOffset);
	//start:added by:sdt:07122005:2215 //For reading offset for Adjacent Pairs

	sprintf(sztmp,"%s:\\%dFreq_%d.dat",cRamDrive,nParamCount,nFreqNum); //Added by:sdt:19032001
	//sprintf(sztmp,"%s:\\%d.dat",cRamDrive,nParamCount);
	remove (sztmp);

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

	//SetImpedence(ZchVal); //Added by:sdt:18062001//Commented by:sdt:04102005:1850
    //This function is added for setting impedance while testing.
	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);
	//SelectReadingLines(nPos,ON);//commented by:sdt:28072001
    SelectReadingLines(nPos,OFF); //Modified by:sdt:28072001

	if(nflagRangeLock==TRUE)
	{
		nCurrentRange=atoi(szRangeHold);
		--nCurrentRange;
		if(nCurrentRange==0)
			nCurrentRange=0;
		SelectRange(nCurrentRange,ON);
	}
	else
	{
		nCurrentRange=0;
		SelectRange(nCurrentRange,ON);
	}

     //No Need
	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	//nBypass=SelectFixtureNEXT(1);
	//Condition added by:sdt:12032006:2235
	if((PairSwitchOption.ONEXT==ADJPAIRS)||(PairSwitchOption.ONEXT==NONADJPAIRS))
		nBypass=SelectFixturePairtoPair(1,PairSwitchOption.ONEXT);//Commented by:sdt:04102005:2020
	else
		nBypass=SelectFixtureXTalkInQuad (1,PairSwitchOption.ONEXT);//Added by:sdt::04102005:2020
	if(nBypass)
	{
		 while(nBypass)
		 {
				//nBypass=SelectFixtureNEXT(0);
				//Condition added by:sdt:12032006:2235
				if((PairSwitchOption.ONEXT==ADJPAIRS)||(PairSwitchOption.ONEXT==NONADJPAIRS))
					nBypass=SelectFixturePairtoPair(0,PairSwitchOption.ONEXT);//Commented by:sdt:04102005:2020
				else
					nBypass=SelectFixtureXTalkInQuad (0,PairSwitchOption.ONEXT);//Added by:sdt::04102005:2020
				nConfig.Max[nParamCount]--;
		 }
	}
	 //No Need
	delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );

	//StartSwitchMode=GetLimitIndex(nParamCount);//Commented by:sdt:04102005:2240
	prmarray->max=nConfig.Max[nParamCount];
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamCount][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]=0;
	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);

	//NextTestPair = CtPair.Pair1;//19032001

	//fAtnPairVal = FreePrevParamVals( fAtnPairVal, SRNO_ATN );
	//fAtnPairVal = GetPrevParamVals( fAtnPairVal, SRNO_ATN );//Modified:pnb:jj:1600:Removed Comments:Not subtracting
												//ATN from FEXT
	nCurrentReadingno=0;
	for(i=0;i<nConfig.Max[nParamCount];)
	{
		cKey=getkey();
		switch(cKey)
		{
			case  F7 :
			  //Added by:sdt:02042001
			    farfree(fNorVal);
			    farfree(fRawVal);

				fcloseall();
				flagReset = TRUE;
				return -1;
			case  F8:
				StopPrint ();//Commented temporary by:sdt:04102005:1925
				break;
			default :
				break;
		}
		gettime(&currenttime);
		//showtime(&currenttime);//Commented by:Sdt:04102005:1925
		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			if (gnTest[SRNO_ATN] != TRUE )
			{
				fV1=GetRawReadingNEXT(TRUE); //Added by:sdt:22072001
				if (First == TRUE)
				{
					First = FALSE;
					//NextTestPair = CtPair.Pair1;//19032001
					fV1=GetRawReadingNEXT(TRUE);

				//Commented by sdt:22072001
					//fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V1);
					//fV1 -= fFixOffset;
					//fV1*=fAtnCalRef;
				}
				fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V1);
				fV1 -= fFixOffset;
				fV1*=fAtnCalRef;//Uncommented by:sdt:04102005:2230
				//fV1*=HF_RefCalFact[nFreqNum];//Commented by:sdt:04102005:2230
			}
			else
				fV1=fRefVal[CtPair.Pair1][nFreqNum];//Modified by:sdt:23072001
			//DEBUG:sdt:12072009:2200

			fV2=GetRawReadingNEXT(FALSE); //now
			if (first)
			{
				first = FALSE;
				fV2=GetRawReadingNEXT(FALSE); //now
			}
			//Commented by:sdt:07122005:1020
			//fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V2);
			//start:07122005:2230
			//Condition added for testing pairs are ajacent
			if(CtPair.Pair1==(CtPair.Pair2-1))
			{
				fFixOffset = fAdjPairsOffset;
			}
			else
			{
				fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V2);
			}
			//End:07122005:2230
			fV2 -= fFixOffset;
			fV1=(float)fabs((double)fV1);
			fV2=(float)fabs((double)fV2);

			if(fV1 && fV2)
			{
				 fRawReading=(float)log10((double)(fV1/fV2));
				 fRawReading*=20;
			}
			else
				fRawReading = 140 + b_random(6);


			memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

			m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;
	// +1 coz fixture numbering starts from 1 in 'Sys.Off' file
			m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nHFrequency;
			m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
			m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;
	// - 1 since the Parameters starts from CR the number of which is 2
	//and  in the file  Sys.Off it start's from 1

			m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;
	// +1 since the Range numbering start's from 0 and in the file Sys.Off it start's from 1

			/*for (int j = 0; j<1000;j ++)
				 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
					 break;
		//	return ASysOffsetVal [i];

		   fSysOffset = ASysOffsetVal [j];

			//fSysOffset = GetSysOffsetFromArray (nParamCount);
			//debug.fSysOff = fSysOffset;
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
			fRawReading -= fSysOffset;
		  }


			fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
			SendSegmentData(CtPair,m_FileInfo);
			fRawReading=User_ceil(fRawReading, prmarray->Decimal[0]);

			/*if(nCableLength < ElfextLength)
				fRawReading-=fAtnPairVal0[CtPair.Pair1-1].fRawVal;
			else
				fRawReading-=fAtnPairVal[CtPair.Pair1-1].fNormVal;*/
			//fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);

			CtPair.val = fRawReading; //Added by:sdt:04102005:2345

			CtPair.norval=normaliseNEXT(fRawReading,nFreqNum);

			CtPair.norval=User_ceil(CtPair.norval,prmarray->Decimal[0]);
			//Commented by:sdt:04102005:1850
			//if(StartSwitchMode !=nAdjNonAdjFlag)
			//{
			//	//ChangeLimits(nParamCount,fNorVal);
			//    ChangeLimits(nParamCount,nFreqNum,fNorVal);//Modified by:sdt:23082001
			//    StartSwitchMode=nAdjNonAdjFlag;
			//}
			//graphisuptodate=0;//Commented by:sdt:04102005:1925
			//graphicsupdate(nFreqNum);//Modified by:sdt:20032001
			//graphicsupdate(nFreqNum,fNorVal);//Commented by:sdt:04102005:1925
			//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:04102005:1925
			//nValidReading=CheckHiLoLimits();
			fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:04102005:2030
			nValidReading=CheckHiLoLimits(nFreqNum);//Modified by:sdt:23082001
			switch(nValidReading)
			{
			case  F7 :
			  //Added by:sdt:02042001
				farfree(fNorVal);
				 farfree(fRawVal);
			  fcloseall();
				 flagReset = TRUE;
				 return -1;
			case  F8:
				 StopPrint ();//Commented temporary by:sdt:04102005:1925
				 break;
			default :
				 break;
			}
		}while(!nValidReading);
		if(nValidReading==VALID)
			 m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
		else
			 m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;
		fNorVal[i]=CtPair.norval;
		fRawVal[i]=fRawReading;

		//Added by:sdt:04102005:1935:start
		//As for Quad software pair combinations i.e. Adjancies etc
		//is not concidered. Hence all summary readings are stored
		//as same as other parameters
		 dRawAvg+=fabs((double)fRawReading);
		 dNorAvg+=fabs((double)CtPair.norval);

		//Added:jj:15032001
		dRawRms+=pow((double)10.0,(double)-1*(fRawReading/10.0));
		dNorRms+=pow((double)10.0,(double)-1*(CtPair.norval/10.0));
		dRawPowerSum+=pow((double)10.0,(double)-1*(fRawReading/10.0));
		dNorPowerSum+=pow((double)10.0,(double)-1*(CtPair.norval/10.0));

		if(i==1)
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		}
		SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1455
		if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		 InfoReading.cPair1=CtPair.Pair1;
		 InfoReading.cPair2=CtPair.Pair2;
		 InfoReading.cParam=nParamCount;
	    InfoReading.nFreq = nFreqNum;//added by:sdt:23032001
		 InfoReading.fRawVal=fRawReading;
		 InfoReading.fNormVal=CtPair.norval;
		InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
	    InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

		//Modified:jj:18052001
		 //fwrite(&InfoReading,sizeof(READING),1,fpData[nParamCount][nFreqNum]);
		fwrite(&InfoReading,sizeof(READING),1,fpData);
		//Added by:sdt:04102005:1935:end

		//Added :by:sdt:08052001
		//Worst pair next calculations
		if(i<OnePerCount+1)
		{
			OnePerOnextRaw[i-1]=fRawReading;
			OnePerOnextNorm[i-1]=CtPair.norval;
		}
		else
		if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
		{
			SortArray(OnePerOnextRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerOnextRaw,OnePerCount);
			SortArray(OnePerOnextNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerOnextNorm,OnePerCount);
		}
		else
		{
			InsertNumber(fRawReading,OnePerOnextRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerOnextNorm,OnePerCount);
		}



		//Commented by:sdt:04102005:sdt:1945
	   /*	//Below condition is Added by:sdt:03092001:1245
		//These conditions used to calculate summary values for
		//Adjacent & Non Adjacent pairs separately.
	   if(nAdjNonAdjFlag==ADJPAIRS)
	   {
			nAdjCount++;
		    dRawAvgAdj+=fabs((double)fRawReading);
			dNorAvgAdj+=fabs((double)CtPair.norval);
			dRawRmsAdj+=pow((double)10.0,(double)-1*(fRawReading/10.0));
			dNorRmsAdj+=pow((double)10.0,(double)-1*(CtPair.norval/10.0));
			dRawPowerSumAdj+=pow((double)10.0,(double)-1*(fRawReading/10.0));
			dNorPowerSumAdj+=pow((double)10.0,(double)-1*(CtPair.norval/10.0));
	   }
		else
	   {
		  nNonAdjCount++;
		dRawAvgNonAdj+=fabs((double)fRawReading);
		    dNorAvgNonAdj+=fabs((double)CtPair.norval);
			dRawRmsNonAdj+=pow((double)10.0,(double)-1*(fRawReading/10.0));
			dNorRmsNonAdj+=pow((double)10.0,(double)-1*(CtPair.norval/10.0));
			dRawPowerSumNonAdj+=pow((double)10.0,(double)-1*(fRawReading/10.0));
			dNorPowerSumNonAdj+=pow((double)10.0,(double)-1*(CtPair.norval/10.0));
	   }
		 //if(i==1)
		//Modified by:sdt:07092001:1540
	   //This condition added to store Min & Max val of Adjacent &
		//Nonadjacent pairs seperately.
	   if((nAdjNonAdjFlag==ADJPAIRS)&&(nAdjCount==1)||(nAdjNonAdjFlag==NONADJPAIRS)&&(nNonAdjCount==1))
		 {
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
		 }
		 SummaryInfo=ReadSummary(nParamCount,nFreqNum,nAdjNonAdjFlag);//Added by:sdt:03092001:1245
		 //if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
	    //Modified by:sdt:03092001:1245
	    if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
		 {
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
		   m_FileInfo.nMinAdjNonAdj[nParamCount]=nAdjNonAdjFlag;
		 }
		 //if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
	    //Modified by:sdt:03092001:1245
	    if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
		 {
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			 m_FileInfo.nMaxAdjNonAdj[nParamCount]=nAdjNonAdjFlag;
	    }
		 //statusdisplay(nParamCount);
		 //statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:04102005:1930
		 InfoReading.cPair1=CtPair.Pair1;
		 InfoReading.cPair2=CtPair.Pair2;
		 InfoReading.cParam=nParamCount;
	    InfoReading.nFreq = nFreqNum;//added by:sdt:23032001
		 InfoReading.fRawVal=fRawReading;
		 InfoReading.fNormVal=CtPair.norval;
	    InfoReading.nPairAdjacency=nAdjNonAdjFlag;//Added by:sdt:26042001
	    InfoReading.nUnitAdjacency=-1;//Added by:sdt:26042001

		 szSwitchOption[i-1]=nAdjNonAdjFlag;//Added by:sdt:03092001:sdt:1245
	    //Modified:jj:18052001
		 //fwrite(&InfoReading,sizeof(READING),1,fpData[nParamCount][nFreqNum]);
	    fwrite(&InfoReading,sizeof(READING),1,fpData);

	    //Added :by:sdt:08052001
		 if(i<OnePerCount+1)
	    {
		OnePerOnextRaw[i-1]=fRawReading;
		  OnePerOnextNorm[i-1]=CtPair.norval;
	    }
	    else
		 if(i==OnePerCount+1)
	    {
		SortArray(OnePerOnextRaw,OnePerCount);
		  InsertNumber(fRawReading,OnePerOnextRaw,OnePerCount);
		  SortArray(OnePerOnextNorm,OnePerCount);
		  InsertNumber(CtPair.norval,OnePerOnextNorm,OnePerCount);
	    }
		 else
		 {
			InsertNumber(fRawReading,OnePerOnextRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerOnextNorm,OnePerCount);
		 }*/

		//nBypass=SelectFixtureNEXT(0);
		//Condition added by:sdt:12032006:2235
		if((PairSwitchOption.ONEXT==ADJPAIRS)||(PairSwitchOption.ONEXT==NONADJPAIRS))
			nBypass=SelectFixturePairtoPair(0,PairSwitchOption.ONEXT);//Commented by:sdt:04102005:2020
		else
			nBypass=SelectFixtureXTalkInQuad (0,PairSwitchOption.ONEXT);//Added by:sdt::04102005:2020

		if(nBypass)
		{
			while(nBypass)
			{
				//nBypass=SelectFixtureNEXT(0);
				//Condition added by:sdt:12032006:2235
				if((PairSwitchOption.ONEXT==ADJPAIRS)||(PairSwitchOption.ONEXT==NONADJPAIRS))
					nBypass=SelectFixturePairtoPair(0,PairSwitchOption.ONEXT);//Commented by:sdt:04102005:2020
				else
					nBypass=SelectFixtureXTalkInQuad (0,PairSwitchOption.ONEXT);//Added by:sdt::04102005:2020
				nConfig.Max[nParamCount]--;
			}
		}

	}
	//Added by:sdt:08052001:modified by:sdt:06062001
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerOnextRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerOnextNorm[OnePerCount-1]);
    farfree(OnePerOnextRaw);
    farfree(OnePerOnextNorm);

	//Added by:sdt:04102005:1945:start
	dRawAvg/=nConfig.Max[nParamCount];
	dNorAvg/=nConfig.Max[nParamCount];
	dRawRms/=nConfig.Max[nParamCount];
	dNorRms/=nConfig.Max[nParamCount];

	dRawRms=fabs(10*log10(dRawRms));
	dNorRms=fabs(10*log10(dNorRms));
	dRawPowerSum=fabs( 10 * log10(dRawPowerSum) );
	dNorPowerSum=fabs( 10 * log10(dNorPowerSum) );

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
	//Added by:sdt:03092001:1500
	//start
	SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);
    SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvg);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvg);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRms);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRms);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDev);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDev);
	SummaryInfo.STATRAW.fPowerSum=dRawPowerSum;
	SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSum;
    WriteSummary(nParamCount,nFreqNum,0,SummaryInfo);
	//Added by:sdt:04102005:1945:end




	 //Commented by:sdt:04102005:1955:start//For Quad Software
	/*//MOD08072002
	if(PairSwitchOption.ONEXT==BOTH_PAIRS){ //== 28072002
		dRawAvgAllComb=(dRawAvgAdj+dRawAvgNonAdj)/(nAdjCount+nNonAdjCount);
		dNorAvgAllComb=(dNorAvgAdj+dNorAvgNonAdj)/(nAdjCount+nNonAdjCount);
		dRawRmsAllComb=(dRawRmsAdj+dRawRmsNonAdj)/(nAdjCount+nNonAdjCount);
		dNorRmsAllComb=(dNorRmsAdj+dNorRmsNonAdj)/(nAdjCount+nNonAdjCount);
		dRawRmsAllComb=fabs(10*log10(dRawRmsAllComb));
		dNorRmsAllComb=fabs(10*log10(dNorRmsAllComb));
	    dRawPowerSumAllComb=fabs( 10 * log10(dRawPowerSumAdj+dRawPowerSumNonAdj) );
    	dNorPowerSumAllComb=fabs( 10 * log10(dNorPowerSumAdj+dNorPowerSumNonAdj) );
    }

	//Modified by:sdt:03092001:1247
    //start
    //For Adjacent Pairs
    if(nAdjCount!=0)
    {
		dRawAvgAdj/=nAdjCount;
		dNorAvgAdj/=nAdjCount;
		dRawRmsAdj/=nAdjCount;
		dNorRmsAdj/=nAdjCount;
		dRawRmsAdj=fabs(10*log10(dRawRmsAdj));
		dNorRmsAdj=fabs(10*log10(dNorRmsAdj));
	   	dRawPowerSumAdj=fabs( 10 * log10(dRawPowerSumAdj) );
    	dNorPowerSumAdj=fabs( 10 * log10(dNorPowerSumAdj) );
    }

    //For NonAdjacent Pairs
    if(nNonAdjCount!=0)
    {
	    dRawAvgNonAdj/=nNonAdjCount;
		dNorAvgNonAdj/=nNonAdjCount;
		dRawRmsNonAdj/=nNonAdjCount;
		dNorRmsNonAdj/=nNonAdjCount;
		dRawRmsNonAdj=fabs(10*log10(dRawRmsNonAdj));
		dNorRmsNonAdj=fabs(10*log10(dNorRmsNonAdj));
	   	dRawPowerSumNonAdj=fabs( 10 * log10(dRawPowerSumNonAdj) );
    	dNorPowerSumNonAdj=fabs( 10 * log10(dNorPowerSumNonAdj) );
	}
    //end
	for(nSumCount=1;nSumCount<=nConfig.Max[nParamCount];nSumCount++)
	{
		 //Below Condition Added by:sdt:03092001:1250
         //This will calculate Std Dev Values for Adjacent Pairs &
         //Non Adjacent Pairs Separately.
         if(szSwitchOption[nSumCount-1]==ADJPAIRS)
         {
		 	dRawStdDevAdj+=pow((fabs((double)fRawVal[nSumCount])-dRawAvgAdj),2);
		 	dNorStdDevAdj+=pow((fabs((double)fNorVal[nSumCount])-dNorAvgAdj),2);
         }
         else if(szSwitchOption[nSumCount-1]==NONADJPAIRS) //MOD08072002
         {
		 	dRawStdDevNonAdj+=pow((fabs((double)fRawVal[nSumCount])-dRawAvgNonAdj),2);
		 	dNorStdDevNonAdj+=pow((fabs((double)fNorVal[nSumCount])-dNorAvgNonAdj),2);
         }
	     if(PairSwitchOption.ONEXT==BOTH_PAIRS){ //MOD08072002 //== 28072002
		 	dRawStdDevAllComb+=pow((fabs((double)fRawVal[nSumCount])-dRawAvgAllComb),2);
			dNorStdDevAllComb+=pow((fabs((double)fNorVal[nSumCount])-dNorAvgAllComb),2);
         }

	}
	//Modified by:sdt:03092001
    //start
    //For Adjacent Pairs
    if(nAdjCount!=0)
    {
		dRawStdDevAdj/=nAdjCount;
		if(dRawStdDevAdj)
			 dRawStdDevAdj=sqrt(dRawStdDevAdj);
		dNorStdDevAdj/=nAdjCount;
		if(dNorStdDevAdj)
			 dNorStdDevAdj=sqrt(dNorStdDevAdj);
    }

    //For NonAdj Pairs
    if(nNonAdjCount!=0)
	{
	    dRawStdDevNonAdj/=nNonAdjCount;
		if(dRawStdDevNonAdj)
			 dRawStdDevNonAdj=sqrt(dRawStdDevNonAdj);
		dNorStdDevNonAdj/=nNonAdjCount;
		if(dNorStdDevNonAdj)
			 dNorStdDevNonAdj=sqrt(dNorStdDevNonAdj);
    }
    //End

     if(PairSwitchOption.ONEXT==BOTH_PAIRS){ //MOD08072002 //== 28072002
		dRawStdDevAllComb/=(nAdjCount+nNonAdjCount);
		if(dRawStdDevAllComb)
			 dRawStdDevAllComb=sqrt(dRawStdDevAllComb);
		dNorStdDevAllComb/=(nAdjCount+nNonAdjCount);
		if(dNorStdDevAllComb)
			 dNorStdDevAllComb=sqrt(dNorStdDevAllComb);
     }


	//Added by:sdt:03092001:1250
	//Start
	//For Adjacent Pairs
	SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgAdj);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgAdj);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsAdj);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsAdj);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevAdj);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevAdj);
	SummaryInfo.STATRAW.fPowerSum=dRawPowerSumAdj;
	SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSumAdj;
	WriteSummary(nParamCount,nFreqNum,0,SummaryInfo);

	//For NonAdjacent Pairs
	SummaryInfo=ReadSummary(nParamCount,nFreqNum,1);
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgNonAdj);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgNonAdj);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsNonAdj);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsNonAdj);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevNonAdj);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevNonAdj);
	SummaryInfo.STATRAW.fPowerSum=dRawPowerSumNonAdj;
	SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSumNonAdj;
	WriteSummary(nParamCount,nFreqNum,1,SummaryInfo);
	//End

	//MOD08072002
	//For BOTH Pairs

	//Repositioned:MOD24072002:jj
	 //SummaryInfo=ReadSummary(nParamCount,nFreqNum,2);//MOD08072002.02 //Commented:jj:28072002
	 //SUMMARYINFO SummaryInfo1=ReadSummary(nParamCount,nFreqNum,0);
	 //SUMMARYINFO SummaryInfo2=ReadSummary(nParamCount,nFreqNum,1);

	if(PairSwitchOption.ONEXT==BOTH_PAIRS){ //== 28072002
		 SummaryInfo=ReadSummary(nParamCount,nFreqNum,2);//MOD08072002.02
		 SUMMARYINFO SummaryInfo1=ReadSummary(nParamCount,nFreqNum,0);
		 SUMMARYINFO SummaryInfo2=ReadSummary(nParamCount,nFreqNum,1);

		 if(fabs((double)SummaryInfo1.STATNORMALISED.fMin)<fabs((double)SummaryInfo2.STATNORMALISED.fMin))
		 {
			 SummaryInfo.STATRAW.cMinPair1=SummaryInfo1.STATRAW.cMinPair1;
			 SummaryInfo.STATRAW.cMinPair2=SummaryInfo1.STATRAW.cMinPair2;
			 SummaryInfo.STATRAW.fMin=SummaryInfo1.STATRAW.fMin;
			 SummaryInfo.STATRAW.cMinTiporRing=SummaryInfo1.STATRAW.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.cMinPair1=SummaryInfo1.STATNORMALISED.cMinPair1;
			 SummaryInfo.STATNORMALISED.cMinPair2=SummaryInfo1.STATNORMALISED.cMinPair2;
			 SummaryInfo.STATNORMALISED.cMinTiporRing=SummaryInfo1.STATNORMALISED.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.fMin=SummaryInfo1.STATNORMALISED.fMin;
		 }
		 else{
			 SummaryInfo.STATRAW.cMinPair1=SummaryInfo2.STATRAW.cMinPair1;
			 SummaryInfo.STATRAW.cMinPair2=SummaryInfo2.STATRAW.cMinPair2;
			 SummaryInfo.STATRAW.fMin=SummaryInfo2.STATRAW.fMin;
			 SummaryInfo.STATRAW.cMinTiporRing=SummaryInfo2.STATRAW.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.cMinPair1=SummaryInfo2.STATNORMALISED.cMinPair1;
			 SummaryInfo.STATNORMALISED.cMinPair2=SummaryInfo2.STATNORMALISED.cMinPair2;
			 SummaryInfo.STATNORMALISED.cMinTiporRing=SummaryInfo2.STATNORMALISED.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.fMin=SummaryInfo2.STATNORMALISED.fMin;
		 }

		 if(fabs((double)SummaryInfo1.STATNORMALISED.fMax)>fabs((double)SummaryInfo2.STATNORMALISED.fMax))
		 {
			 SummaryInfo.STATRAW.cMaxPair1=SummaryInfo1.STATRAW.cMaxPair1;
			 SummaryInfo.STATRAW.cMaxPair2=SummaryInfo1.STATRAW.cMaxPair2;
			 SummaryInfo.STATRAW.cMaxTiporRing=SummaryInfo1.STATRAW.cMaxTiporRing;
			 SummaryInfo.STATRAW.fMax=SummaryInfo1.STATRAW.fMax;
			 SummaryInfo.STATNORMALISED.cMaxPair1=SummaryInfo1.STATNORMALISED.cMaxPair1;
			 SummaryInfo.STATNORMALISED.cMaxPair2=SummaryInfo1.STATNORMALISED.cMaxPair2;
			 SummaryInfo.STATNORMALISED.cMaxTiporRing=SummaryInfo1.STATNORMALISED.cMaxTiporRing;
			 SummaryInfo.STATNORMALISED.fMax=SummaryInfo1.STATNORMALISED.fMax;
		 }
		 else{
			 SummaryInfo.STATRAW.cMaxPair1=SummaryInfo2.STATRAW.cMaxPair1;
			 SummaryInfo.STATRAW.cMaxPair2=SummaryInfo2.STATRAW.cMaxPair2;
			 SummaryInfo.STATRAW.cMaxTiporRing=SummaryInfo2.STATRAW.cMaxTiporRing;
			 SummaryInfo.STATRAW.fMax=SummaryInfo2.STATRAW.fMax;
			 SummaryInfo.STATNORMALISED.cMaxPair1=SummaryInfo2.STATNORMALISED.cMaxPair1;
			 SummaryInfo.STATNORMALISED.cMaxPair2=SummaryInfo2.STATNORMALISED.cMaxPair2;
			 SummaryInfo.STATNORMALISED.cMaxTiporRing=SummaryInfo2.STATNORMALISED.cMaxTiporRing;
			 SummaryInfo.STATNORMALISED.fMax=SummaryInfo2.STATNORMALISED.fMax;
		 }
		SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgAllComb);
		SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgAllComb);
		SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsAllComb);
		SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsAllComb);
		SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevAllComb);
		SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevAllComb);
		SummaryInfo.STATRAW.fPowerSum=dRawPowerSumAllComb;
		SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSumAllComb;
		WriteSummary(nParamCount,nFreqNum,2,SummaryInfo);

	}
	else if(PairSwitchOption.ONEXT==ADJPAIRS){//Added:MOD31072002:01:jj
		 SUMMARYINFO SummaryInfo1=ReadSummary(nParamCount,nFreqNum,0);
		 WriteSummary(nParamCount,nFreqNum,2,SummaryInfo1);
	}
	else if(PairSwitchOption.ONEXT==NONADJPAIRS){
		 SUMMARYINFO SummaryInfo1=ReadSummary(nParamCount,nFreqNum,1);
		 WriteSummary(nParamCount,nFreqNum,2,SummaryInfo1);
	}*/
	//Commented by:sdt:04102005:1955:end

	/*else{ //Added:MOD24072002:jj //Commented:jj:28072002
		if(PairSwitchOption.ONEXT==ADJPAIRS){ //== 28072002
			 SummaryInfo.STATRAW.cMinPair1=SummaryInfo1.STATRAW.cMinPair1;
			 SummaryInfo.STATRAW.cMinPair2=SummaryInfo1.STATRAW.cMinPair2;
			 SummaryInfo.STATRAW.fMin=SummaryInfo1.STATRAW.fMin;
			 SummaryInfo.STATRAW.cMinTiporRing=SummaryInfo1.STATRAW.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.cMinPair1=SummaryInfo1.STATNORMALISED.cMinPair1;
			 SummaryInfo.STATNORMALISED.cMinPair2=SummaryInfo1.STATNORMALISED.cMinPair2;
			 SummaryInfo.STATNORMALISED.cMinTiporRing=SummaryInfo1.STATNORMALISED.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.fMin=SummaryInfo1.STATNORMALISED.fMin;

			 SummaryInfo.STATRAW.cMaxPair1=SummaryInfo1.STATRAW.cMaxPair1;
			 SummaryInfo.STATRAW.cMaxPair2=SummaryInfo1.STATRAW.cMaxPair2;
			 SummaryInfo.STATRAW.cMaxTiporRing=SummaryInfo1.STATRAW.cMaxTiporRing;
			 SummaryInfo.STATRAW.fMax=SummaryInfo1.STATRAW.fMax;
			 SummaryInfo.STATNORMALISED.cMaxPair1=SummaryInfo1.STATNORMALISED.cMaxPair1;
			 SummaryInfo.STATNORMALISED.cMaxPair2=SummaryInfo1.STATNORMALISED.cMaxPair2;
			 SummaryInfo.STATNORMALISED.cMaxTiporRing=SummaryInfo1.STATNORMALISED.cMaxTiporRing;
			 SummaryInfo.STATNORMALISED.fMax=SummaryInfo1.STATNORMALISED.fMax;

		}
		else{
			 SummaryInfo.STATRAW.cMinPair1=SummaryInfo2.STATRAW.cMinPair1;
			 SummaryInfo.STATRAW.cMinPair2=SummaryInfo2.STATRAW.cMinPair2;
			 SummaryInfo.STATRAW.fMin=SummaryInfo2.STATRAW.fMin;
			 SummaryInfo.STATRAW.cMinTiporRing=SummaryInfo2.STATRAW.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.cMinPair1=SummaryInfo2.STATNORMALISED.cMinPair1;
			 SummaryInfo.STATNORMALISED.cMinPair2=SummaryInfo2.STATNORMALISED.cMinPair2;
			 SummaryInfo.STATNORMALISED.cMinTiporRing=SummaryInfo2.STATNORMALISED.cMinTiporRing;
			 SummaryInfo.STATNORMALISED.fMin=SummaryInfo2.STATNORMALISED.fMin;

			 SummaryInfo.STATRAW.cMaxPair1=SummaryInfo2.STATRAW.cMaxPair1;
			 SummaryInfo.STATRAW.cMaxPair2=SummaryInfo2.STATRAW.cMaxPair2;
			 SummaryInfo.STATRAW.cMaxTiporRing=SummaryInfo2.STATRAW.cMaxTiporRing;
			 SummaryInfo.STATRAW.fMax=SummaryInfo2.STATRAW.fMax;
			 SummaryInfo.STATNORMALISED.cMaxPair1=SummaryInfo2.STATNORMALISED.cMaxPair1;
			 SummaryInfo.STATNORMALISED.cMaxPair2=SummaryInfo2.STATNORMALISED.cMaxPair2;
			 SummaryInfo.STATNORMALISED.cMaxTiporRing=SummaryInfo2.STATNORMALISED.cMaxTiporRing;
			 SummaryInfo.STATNORMALISED.fMax=SummaryInfo2.STATNORMALISED.fMax;
		}
	}

	//Repositioned:MOD24072002:jj
	//SummaryInfo=ReadSummary(nParamCount,nFreqNum,2);
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgAllComb);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgAllComb);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsAllComb);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsAllComb);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevAllComb);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevAllComb);
	SummaryInfo.STATRAW.fPowerSum=dRawPowerSumAllComb;
	SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSumAllComb;
	WriteSummary(nParamCount,nFreqNum,2,SummaryInfo);*/

	//End


	//Added by:sdt:26042001
	//m_FileInfo.nPairAdjacency[nParamCount]=nAdjNonAdjFlag;//Commented by:sdt:09052001
	m_FileInfo.nPairAdjacency[nParamCount]=PairSwitchOption.ONEXT; //Added by:sdt:09052001
	m_FileInfo.nUnitAdjacency[nParamCount]=-1;

	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	//SelectReadingLines(nPos,OFF);//Commented by:sdt:28072001
	SelectReadingLines(nPos,ON);//Modified by:sdt:28072001

	ResetSystem();

	//Modified:jj:18052001
	//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
    if(fpData){fclose(fpData);}

//	fAtnPairVal = FreePrevParamVals( fAtnPairVal,nParamCount );
    //Added by:sdt:02042001
    farfree(fNorVal);
    farfree(fRawVal);
    free(szSwitchOption);//MOD11072001:02:jj:
	return nReadingno;
}

float GetRawReadingNEXT (int nRef)
{
	int         Huntup = 0, Huntlow = 0/*,nCalNo//19032001*/;
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
    //Commented by:sdt:31072001
	fReading = fReading-gfOffset[nCurrentRange];// atn offset shold be used
	//fReading = fReading-HF_Offset[nConfig.nHFrequency][nCurrentRange];//Commented by:sdt:04102005:2230

	if(!nRef)
	{
		// for digi cm
		fReading *= fFcal[nCurrentRange];//Uncommented by:sdt:04102005:2230
		//fReading *= HF_CalFact[nConfig.nHFrequency][nCurrentRange];///Commented by:sdt:04102005:2230
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

//Commented by:sdt:24042001
/*int SelectFixtureNEXT (int nStart)
{
	int nBypass;
	if(nStart)
	{
		gflagReplace1 = 0;
		gflagReplace2 = 0;

        if(PairSwitchOption.ONEXT==ADJPAIRS)
	        SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
        if(PairSwitchOption.ONEXT==NONADJPAIRS)
        	SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
        if(PairSwitchOption.ONEXT==BOTH_PAIRS)
        {
        	SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
            nBothFlag=1;
        }
		CtPair.Pair1=SeqPair->Pairno1;
        CtPair.Pair2=SeqPair->Pairno2;
		//nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		nPrevPair2=0;
		nPrevPair1=0;
		nBypass=SetFixture(CtPair.Pair1,CtPair.Pair2,FALSE);

		CtPair.UnitSrNo=1;
		nPrevPair1 = CtPair.Pair1;
		nPrevPair2 = CtPair.Pair2;
	}
	else
	{
		if(SeqPair!=NULL)
        {
            SeqPair=SeqPair->link;
            if(SeqPair!=NULL)
            {
		    	//CtPair.Pair1=SeqPair->Pairno1;
	            //CtPair.Pair2=SeqPair->Pairno2;
                //Modified by:sdt:24042001:for the unit proper unit no. display.
				CtPair.Pair1=SeqPair->Pairno1+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
				CtPair.Pair2=SeqPair->Pairno2+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
            }
        }
        if(SeqPair==NULL)
		{
			if(PairSwitchOption.ONEXT==ADJPAIRS)
		        SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
			if(PairSwitchOption.ONEXT==NONADJPAIRS)
		       	SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
            if(PairSwitchOption.ONEXT==BOTH_PAIRS)
            {
              	if(nBothFlag)
                {
                  	SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
                    nBothFlag=0;
                }
                else
                {
                   	SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
                    nBothFlag=1;
                }
             }
             CtPair.UnitSrNo++;
             //Modified by:sdt:24042001:for the unit proper unit no. display.
			 CtPair.Pair1=SeqPair->Pairno1+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
			 CtPair.Pair2=SeqPair->Pairno2+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
             if(CtPair.UnitSrNo == nUnits)
			 {
				nTempUnitOf = atoi(nConfig.szUnitof);
				itoa(nLastUnitSize,nConfig.szUnitof,10);
			 }
		 //nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		}
		nBypass=SetPair(CtPair.Pair1,CtPair.Pair2,nPrevPair1,nPrevPair2);

		nPrevPair1=CtPair.Pair1;
		nPrevPair2=CtPair.Pair2;
	}
	return nBypass;
}*/


float normaliseNEXT (float fReading,int nFreqNum)//Modified by:sdt:21032001
{
	char 	szTag[50];  //Added by:sdt:04022007:2330
	int 	nMin_length; //Added by:sdt:04022007:2330

	SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1715
	char    path[80];//22012001
	float   fSeq,fReturn,fLe,fAtnAvg;//fRoomTemp;
	unsigned int		nCableLength; //SDT11122016:0715
// Nilesh - 13th Aug. 1999 - Start
	float 	nor_length;
	//int 	uCuppMmtMethod;//19032001
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1715

	//nor_length = atof( speck.speckparams[nParamCount].norlength);
	//Modified by:sdt:04102005:1955
	//nor_length = speckData.fNormLength;
	//Modified by:sdt:31012007:2140
	nor_length = speckData.speckparams[nParamCount].fNormLength;


	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	if ( GetPrivateProfileInt("BASIC_INFO",
			"NORMALISE_KM",0,path) )
		nor_length = 1000.0;
	else if ( nor_length == (float)0.0 )
			nor_length = 1000.0;

	nCableLength=atoi(nConfig.szLength);
	//Added by:sdt:04022007:2330
	sprintf(szTag,"%s_MIN_LENGTH",paraminfo[nParamCount].szParamAbr);
	nMin_length=GetPrivateProfileInt("MINIMUM_CABLE_LENGTH",szTag,0,path);
	if((nMin_length!=0)&&((unsigned int)nCableLength< (unsigned int)nMin_length)) //SDT:11122016:0700
	{
		nCableLength = nMin_length;
	}


	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commented by:sdt:04102005:2240
	fReturn=fReading;
	if ((unsigned int)nCableLength < 300) //SDT:11122016:0700
	{
			SummaryInfo=ReadSummary(SRNO_ATN,nFreqNum,0);//Added by:sdt:03092001:1715
			//fAtnAvg = m_FileInfo.STATNORMALISED[SRNO_ATN][nFreqNum].fAvg;
		  //Modified by:sdt:03092001:1715
		  fAtnAvg = SummaryInfo.STATNORMALISED.fAvg;
			fAtnAvg /= NEPEAR;  // Some Unit
			fLe = ((unsigned int)nCableLength)/( (nor_length) ? nor_length : 1 );  // Substituted nor_length instead 1000 //SDT:11122016:0700
			fSeq = 1 - exp((-4 * fAtnAvg * fLe));
			fSeq = fabs(fSeq ? (1-exp(-4 * fAtnAvg))/fSeq : 0);
			if(fSeq)
				fReturn = fReturn - (10 * log10(fSeq));
	}

	return fReturn;
}       // End of normalise

//#endif //Added by:sdt:21082001