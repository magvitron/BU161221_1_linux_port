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
#include "seq.h" //Added by:sdt:23042001
#include "funproto.h"
#include "frontpg.h"
#include "utils.h" //Added by:sdt:05012006:1930
#include "setup.h"

extern RunSetup runSetup; //Added by:sdt:28042015:2100


//Added by:sdt:26072006:2255
extern READING far*	fCrPairVal;
extern READING far*	fCmPairVal;
extern char	SpeckList[MAXFREQUENCIES][10];
//Added by:sdt:18062006:1825
//Commented by:sdt:19062006:222
//int nfFextAtLowFreq(int);
//int CalculateReadingsFEXT(int);
//extern char 	szZchAutoManual[8] ;//Commented by:sdt:04102005:2245
//extern int ZchHfAray[5][MAXFREQUENCIES];//Commented by:sdt:04102005:2245

//Commented by:sdt:04102005:2245
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);
//Commented by:sdt:04102005:2245
//extern float HF_Offset[MAXFREQUENCIES][MAXRANGES];
//extern float HF_CalFact[MAXFREQUENCIES][MAXRANGES];
//extern float HF_RefCalFact[MAXFREQUENCIES];

//Commented by:sdt:04102005:2245
//void SetImpedence(int Val);
//Added by:sdt:06062001
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];


//Added:jj:30052001
extern int 		CalibrateOneParamATN();


extern float    fAtnCalRef;
//void ChangeLimits(int nParam,float fNorVal[]);
//Commented by:sdt:04102005:2245
//void ChangeLimits(int nParam,int nFreqNum,float fNorVal[]);//Modified by:sdt:23082001
//int GetLimitIndex(int Param);

void InsertNumber(float Val,float *Array,int Arraysize);
void SortArray(float *Array,int Arraysize);
//float WorstFext[MAXFREQUENCIES][2];//Commented by:sdt:06062001
extern int nAdjNonAdjFlag; //0-Adjacent 1-NonAdjacent.

//int SelectFixturePairtoPair (int nStart,int nCombnType);
int SelectFixtureXTalkInQuad (int nStart,int nCombnType);
extern SEQPAIR *SeqPair;
extern PAIRSWITCH PairSwitchOption;
extern SEQUENCE CableSequence;
extern int nBothFlag;

extern int CurrentHfParam;
extern int CurrentFreq;
//Commented by:sdt:04102005:2245
//extern int ZchVal;
//extern int ZchValArray[MAXFREQUENCIES];
//extern getZchVal();


extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];

//extern void ReadStdVal(int nParamCount);
int GetParamReadingsFEXT (int);
//int SelectFixtureFEXT (int nStart);
float GetRawReadingFEXT (int nRef);
float normaliseFEXT (float fReading);

//Commented by:sdt:04102005:2245
//extern READING far* GetPrevParamVals( READING far *pparamVal, int param, int nFreqNum );
//extern READING far* FreePrevParamVals( READING far *pparamVal, int param );

extern Calibration  	Cal;
//Commented by:sdt:04102005:2245
//extern void		SelectSplitForCal( );
//extern int     	SetParamFreq();
//extern int SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
//extern void InitialiseCALarray(void);
//extern float   	GetFixOffsetFromArray (int, int, int, int );
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001

extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:19032001:1640
//extern READING far		*fAtnPairVal;
//extern float          	fRefVal[120];//commented by:sdt:23072001
extern float          	fRefVal[MAXPAIRS][MAXFREQUENCIES];// Added by:sdt:25112005:2230
extern int            ElfextLength;
extern int          gnTest[MAXPARAMETERS];
extern int          gflagReplace1, gflagReplace2 ;
//extern   			SPECIFICATION  	speck;
//extern int      	graphisuptodate;
extern int          nCurrentReadingno;
extern struct   	time currenttime;
extern char     	defaultmessage[];
extern char     	szRangeHold[2];
extern int      	nflagRangeLock;
extern float 		User_ceil(float fFlotVal,int nNo);
extern  int        	nTested[MAXPARAMETERS];
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

extern char 		bIncOkRej;
extern int			nCurrentRange;

extern STRUCT_SPECKS speckData;

extern TFrontPage *fPage; //Added by:sdt:27092005:2300



int FextTest()
{
	char	path[80];
	nCount=0;
	int nReturnVal=0;
	int nFreqNum=0; //added by:sdt:19032001

	nParamCount=SRNO_FEXT;
	//Added by:sdt:04102005:2320
	fPage = (TFrontPage *)(new TFrontPage( "FEXT TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}
	//Added by:sdt:17062006:start
	//As Fext at low frequency is calculated from CUPP, ATN, ZCH.
	//Hence calibration and other code is not neccessary.
	//therefore this code is at the starting only.
	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
	{
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum])
		{
			//nReturnVal = nfFextAtLowFreq(nFreqNum);
			//Modified by:sdt:19062006:2225
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

	//Added by:sdt:17062006:end

	nParamCount=SRNO_FEXT;
	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
	{
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	ReadParamDotCts( paraminfo[nParamCount].szParamAbr, prmarray );
	ReadStdVal(nParamCount); //Added:jj:26032001

	lseek ( ParamCtsHandle, 0L, SEEK_SET );
	lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );

	lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount , SEEK_CUR );
	read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
	close( ParamCtsHandle );

	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used
	prmarray->max=nConfig.Max[nParamCount];

	CtPair.nTest=nParamCount;

	InitialiseCALarray();

	//Added:jj:30052001
	SetHighFreq(nConfig.nHFrequency);
	//Post Frequency Delay Added by:sdt:03082001
	delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );
	factor = 100;
	//SelectReadingLines(0,ON);//Commented by:sdt:28072001
	SelectReadingLines(0,OFF);//Modified by:sdt:28072001
	// Above line used to select ATN in the VI unit.
	//Moved after frontpage display by:sdt:14102005:1300
	//nReturnVal = CalibrateOneParamATN();//Uncommented by:sdt:04102005:2300



	//****Temporary commented by:sdt:27072006:1855*****//
	nReturnVal = CalibrateOneParamATN();//Uncommented by:sdt:04102005:2300
	//for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:19032001
	for(nFreqNum=LFFrequency.nTotalFrequencies;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)  //Modified by:sdt:17062006
	{
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:04102005:2320
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum]) //Added by:sdt:19032001
		{
		if(!param_tested[nParamCount][nFreqNum] || gnTest[nParamCount])//Added:jj:06042001
		{
			//Below statement added by:sdt:02072001
			//If Last Unit size different ,below statement will restore the
			//Original Unit size.
			itoa (nTempUnitOf ,nConfig.szUnitof ,10);//san 21-11-97 For appending of the remaining pairs

			nConfig.nHFrequency = nFreqNum; //Added by:sdt:19032001

			//ZchVal=ZchValArray[nFreqNum]; //Commented:MOD11082002:01:jj
			CurrentHfParam=4;
			CurrentFreq=nFreqNum;
			if(prmarray->nParamType)
				m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
			else
				m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

			//setupbargraph(nParamCount,nFreqNum);//Commented by:sdt:04102005:2245
			//Commented by:sdt:04102005:2245:start
			//ZchVal=ZchValArray[nFreqNum];//Moved:MOD11082002:01:jj
			//if(ZchVal<10 || ZchVal >2550 || !strcmp(szZchAutoManual,"Manual")) //Added:MOD11082002:01:jj
			//	getZchVal();
			//GetActualImpedence(); //Added:MOD11082002:02:jj
			//ZchValArray[nFreqNum]=ZchVal;
			//ZchHfAray[CurrentHfParam][nFreqNum]=ZchVal;//Added:MOD11082002:01:jj
			//Commented by:sdt:04102005:2245:end
			SetHighFreq(nConfig.nHFrequency);
            //Post Frequency Delay Added by:sdt:03082001
		    delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );

			sprintf(path,"%s:\\cts.ini",cRamDrive);
		    factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);

			nTested[nParamCount]=TRUE;
			fPage->hint(hcStartTesting);//Added by:sdt:04102005:2320
		   	nReturnVal=GetParamReadingsFEXT(nFreqNum);//modified by:sdt:19032001
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;

			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//Changed by:sdt:21032001
			}
			fPage->hint(hcStopTesting);//Added by:sdt:04102005:2320
    	}
		}
    	if(nReturnVal ==-1)
        	break;
    }

	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}

int GetParamReadingsFEXT (int nFreqNum)
{
	SUMMARYINFO SummaryInfo;//Added by:sdt:03092001
	char *szSwitchOption;//Added by:sdt:03092001
	//int nAdjCount=0,nNonAdjCount=0;//Commented by:sdt:04102005:2330
	FILE *fpData;
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
	unsigned long         ALocationSys;
	float                 ASysOffsetVal;

	float 	far *fNorVal;//[MAX_NO_READINGS];
 	float 	far *fRawVal;//[MAX_NO_READINGS];

	int     cKey;
	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass;
	int     nSumCount;
	float   fRawReading,fV1,fV2, fSysOffset, fFixOffset;
	//Uncommented by:sdt:04102005:2250
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;
	double  dNorPowerSum=0.0,dRawPowerSum=0.0;

	//Commented by:sdt:04102005:2250
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


	int     first = TRUE;//nCableLength=atoi(nConfig.szLength);

	READING InfoReading;
	char sztmp[20];
	struct time prevtime;
	SSysOffsetVal m_SysOffsetVal;
	int	StartSwitchMode;//=GetLimitIndex(nParamCount); //added by:11052001//Commented by:sdt:03092001

	float fAdjPairsOffset=0.0;   //added by:sdt:05012006:1930 //For Adj pair offset
	char  strAdjPairsOffset[10]; //added by:sdt:05012006:1930 //For Adj pair offset


	float far *OnePerFextRaw;
	float far *OnePerFextNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerFextRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerFextNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerFextRaw,0,OnePerCount);
	memset(OnePerFextNorm,0,OnePerCount);
	//memset(Worstpairnext.Fext,0,MAXFREQUENCIES*2);//Modified by:sdt:06062001// Commented:MOD24072002:jj

	//Added by:sdt:02042001
	fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));

	//Added by:sdt:03092001
	//start
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
	szSwitchOption=(char *)farmalloc(sizeof(char)*nConfig.Max[nParamCount]);
	memset(szSwitchOption,0,sizeof(char)*nConfig.Max[nParamCount]);
	//end

	//start:added by:sdt:05012006:1930 //For reading offset for Adjacent Pairs
	char path[80];
	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	GetPrivateProfileString ("FEXT_OFFSET","ADJ_PAIR_FEXT_OFFSET_VOLTAGE",strAdjPairsOffset,"0.0",path);
	fAdjPairsOffset=atof(strAdjPairsOffset);
	//start:added by:sdt:05012006:1930 //For reading offset for Adjacent Pairs


	sprintf(sztmp,"%s:\\%dFreq_%d.dat",cRamDrive,nParamCount,nFreqNum); //Added by:sdt:19032001
	remove (sztmp);

	if((fpData= fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		//getch();
		messageBox( "File creation error", mfError|mfOKButton );
		fs = fs_error;
		return 1;
	}

	//SetImpedence(ZchVal); //Commented by:sdt:04102005:2250
    //This function is added for setting impedance while testing.
	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);
	//SelectReadingLines(nPos,ON);//Commented by:sdt:28072001
    SelectReadingLines(nPos,OFF);//Modified by:sdt:28072001

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
	//Condition added by:sdt:12032006:2235
	if((PairSwitchOption.FEXT==ADJPAIRS)||(PairSwitchOption.FEXT==NONADJPAIRS))
		nBypass=SelectFixturePairtoPair(1,PairSwitchOption.FEXT);//Commented by:sdt::04102005:2305
	else
		nBypass=SelectFixtureXTalkInQuad (1,PairSwitchOption.FEXT);//Added by:sdt::04102005:2305
	if(nBypass)
	{
		 while(nBypass)
		 {
				//Condition added by:sdt:12032006:2235
				if((PairSwitchOption.FEXT==ADJPAIRS)||(PairSwitchOption.FEXT==NONADJPAIRS))
					nBypass=SelectFixturePairtoPair(0,PairSwitchOption.FEXT);//Commented by:sdt::04102005:2305
				else
					nBypass=SelectFixtureXTalkInQuad (0,PairSwitchOption.FEXT);//Added by:sdt::04102005:2305
				nConfig.Max[nParamCount]--;
		 }
	}
     //No Need
	delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );

    StartSwitchMode=GetLimitIndex(nParamCount);//Added by:sdt:03092001

	prmarray->max=nConfig.Max[nParamCount];
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamCount][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]=0;
	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);


//	fAtnPairVal = FreePrevParamVals( fAtnPairVal, SRNO_ATN );
//	fAtnPairVal = GetPrevParamVals( fAtnPairVal, SRNO_ATN, nFreqNum );//Modified:pnb:jj:1600:Removed Comments:Not subtracting
												//ATN from FEXT
    nCurrentReadingno=0;
	for(i=0;i<nConfig.Max[nParamCount];)
	{
		cKey=getkey();
		switch(cKey)
		{
			case  F7 :
			    farfree(fNorVal);
			    farfree(fRawVal);

				fcloseall();
				flagReset = TRUE;
				return -1;
		 	case  F8:
				StopPrint ();//Commented Temporary by:sdt:04102005:2255
				break;
			default :
				break;
		}
		gettime(&currenttime);
		//showtime(&currenttime);//Commented by:sdt:04102005:2255
		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			//Conditions uncommented by:sdt:25102005:2035
			if (gnTest[SRNO_ATN] != TRUE )
			{
				fV1=GetRawReadingFEXT(TRUE);
				if (first == TRUE)
				{
					first = FALSE;
					fV1=GetRawReadingFEXT(TRUE);

				}
				//Moved here by:sdt:25112005:2035
				fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V1);
				fV1 -= fFixOffset;
				//fV1*=HF_RefCalFact[nFreqNum];//Commented by:sdt:04102005:2255
				fV1*=fAtnCalRef;//Uncommented by:sdt:04102005:2255
			}
			else
				fV1=fRefVal[CtPair.Pair1][nFreqNum];//Added by:sdt:25112005:2225
//			fV1=fRefVal[CtPair.Pair1];
			fV2=GetRawReadingFEXT(FALSE); //now
			if (first)
			{
				first = FALSE;
				fV2=GetRawReadingFEXT(FALSE); //now
			}
			//Commented by:sdt:05012006:1935
			//fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V2);
			//start:05012006:1935
			//Condition added for testing pairs are ajacent
			if(CtPair.Pair1==(CtPair.Pair2-1))
			{
				fFixOffset = fAdjPairsOffset;
			}
			else
			{
				fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V2);
			}
			//End:05012006:1935
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
			m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nHFrequency;
			m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
			m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;

			m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;
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
        	    fSysOffset=ASysOffsetVal ;
				fRawReading -= fSysOffset;
            }

			fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
		  	SendSegmentData(CtPair,m_FileInfo);
			fRawReading=User_ceil(fRawReading, prmarray->Decimal[0]);
/*		 	if(nCableLength < ElfextLength)
				fRawReading-=fAtnPairVal[CtPair.Pair1-1].fRawVal;
			else
				fRawReading-=fAtnPairVal[CtPair.Pair1-1].fNormVal;
*/
			fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);
			CtPair.val = fRawReading; //Added by:sdt:04102005:2345
			CtPair.norval=normaliseFEXT(fRawReading);
			CtPair.norval=User_ceil(CtPair.norval,prmarray->Decimal[0]);
			//Commented by:sdt:04102005:2330
			//if(StartSwitchMode !=nAdjNonAdjFlag)
			//{
			//	//ChangeLimits(nParamCount,fNorVal);
			//   ChangeLimits(nParamCount,nFreqNum,fNorVal);//Modified by:sdt:23082001
			//    StartSwitchMode=nAdjNonAdjFlag;
			//}
			//graphisuptodate=0;//Commented by:sdt:04102005:2255
			//graphicsupdate(nFreqNum,fNorVal);//Commented by:sdt:04102005:2255
			//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:04102005:2255
			fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:04102005:2320
			nValidReading=CheckHiLoLimits(nFreqNum);
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
				 StopPrint ();//Commented temporary by:sdt:04102005:2255
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


		//Added by:sdt:04102005:2310:start
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
		//Added by:sdt:04102005:2310:end





		//Commented by:sdt:04102005:sdt:2310:start
		/*//Below condition is Added by:sdt:03092001:1040
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
		 SummaryInfo=ReadSummary(nParamCount,nFreqNum,nAdjNonAdjFlag);//Added by:sdt:03092001
		 //if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
		 //Modified by:sdt:03092001:1105
		 if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
		 {
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			 m_FileInfo.nMinAdjNonAdj[nParamCount]=nAdjNonAdjFlag;
		 }
		 //if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
		 //Modified by:sdt:03092001:1105
		 if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
		 {
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			 m_FileInfo.nMaxAdjNonAdj[nParamCount]=nAdjNonAdjFlag;
		 }
		 //statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:04102005:2255
		 InfoReading.cPair1=CtPair.Pair1;
		 InfoReading.cPair2=CtPair.Pair2;
		 InfoReading.cParam=nParamCount;
		 InfoReading.nFreq = nFreqNum;//Added by:sdt:23032001
		 InfoReading.fRawVal=fRawReading;
		 InfoReading.fNormVal=CtPair.norval;
		 InfoReading.nPairAdjacency=nAdjNonAdjFlag; //Added by:sdt:26042001
		 InfoReading.nUnitAdjacency=-1;

		 szSwitchOption[i-1]=nAdjNonAdjFlag;//Added by:sdt:03092001
		 fwrite(&InfoReading,sizeof(READING),1,fpData);*/
		 //Commented by:sdt:04102005:sdt:2310:end

		 //Added :by:sdt:08052001
		//Worst pair next calculations
		 if(i<OnePerCount+1)
		 {
			OnePerFextRaw[i-1]=fRawReading;
			OnePerFextNorm[i-1]=CtPair.norval;
		 }
		 else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
		 {
			SortArray(OnePerFextRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerFextRaw,OnePerCount);
			SortArray(OnePerFextNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerFextNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerFextRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerFextNorm,OnePerCount);
		 }

		//Condition added by:sdt:12032006:2235
		if((PairSwitchOption.FEXT==ADJPAIRS)||(PairSwitchOption.FEXT==NONADJPAIRS))
			nBypass=SelectFixturePairtoPair(0,PairSwitchOption.FEXT);//Commented by:sdt:04102005:2305
		else
			nBypass=SelectFixtureXTalkInQuad (0,PairSwitchOption.FEXT);//Added by:sdt::04102005:2305
		if(nBypass)
		{
			while(nBypass)
			{
				//nBypass=SelectFixtureFEXT(0);
				//Condition added by:sdt:12032006:2235
				if((PairSwitchOption.FEXT==ADJPAIRS)||(PairSwitchOption.FEXT==NONADJPAIRS))
					nBypass=SelectFixturePairtoPair(0,PairSwitchOption.FEXT);//Commented by:sdt:04102005:2305
				else
					nBypass=SelectFixtureXTalkInQuad (0,PairSwitchOption.FEXT);//Added by:sdt::04102005:2305
				nConfig.Max[nParamCount]--;
			}
		}
	}
	//Added by:sdt:08052001:modifiedm by:sdt:06062001
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerFextRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerFextNorm[OnePerCount-1]);
    farfree(OnePerFextRaw);
    farfree(OnePerFextNorm);

	//Added by:sdt:04102005:2315:start
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
	//Added by:sdt:04102005:2315:end



	 //Commented by:sdt:04102005:2315:start//For Quad Software
	/*//MOD08072002
	if(PairSwitchOption.FEXT==BOTH_PAIRS){//== 28072002
		dRawAvgAllComb=(dRawAvgAdj+dRawAvgNonAdj)/(nAdjCount+nNonAdjCount);
		dNorAvgAllComb=(dNorAvgAdj+dNorAvgNonAdj)/(nAdjCount+nNonAdjCount);
		dRawRmsAllComb=(dRawRmsAdj+dRawRmsNonAdj)/(nAdjCount+nNonAdjCount);
		dNorRmsAllComb=(dNorRmsAdj+dNorRmsNonAdj)/(nAdjCount+nNonAdjCount);
		dRawRmsAllComb=fabs(10*log10(dRawRmsAllComb));
		dNorRmsAllComb=fabs(10*log10(dNorRmsAllComb));
		dRawPowerSumAllComb=fabs( 10 * log10(dRawPowerSumAdj+dRawPowerSumNonAdj) );
		dNorPowerSumAllComb=fabs( 10 * log10(dNorPowerSumAdj+dNorPowerSumNonAdj) );
	}

	//Modified by:sdt:03092001:1105
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
         //Below Condition Added by:sdt:03092001
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
	     if(PairSwitchOption.FEXT==BOTH_PAIRS){ //MOD08072002//== 28072002
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
     if(PairSwitchOption.FEXT==BOTH_PAIRS){ //MOD08072002 //== 28072002
		dRawStdDevAllComb/=(nAdjCount+nNonAdjCount);
		if(dRawStdDevAllComb)
			 dRawStdDevAllComb=sqrt(dRawStdDevAllComb);
		dNorStdDevAllComb/=(nAdjCount+nNonAdjCount);
		if(dNorStdDevAllComb)
			 dNorStdDevAllComb=sqrt(dNorStdDevAllComb);
	 }

	//End
	//Added by:sdt:03092001:1115
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



	if(PairSwitchOption.FEXT==BOTH_PAIRS){//== 28072002
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
	else if(PairSwitchOption.FEXT==ADJPAIRS){//Added:jj:MOD31072002:01:jj
		 SUMMARYINFO SummaryInfo1=ReadSummary(nParamCount,nFreqNum,0);
		 WriteSummary(nParamCount,nFreqNum,2,SummaryInfo1);
	}
	else if(PairSwitchOption.FEXT==NONADJPAIRS){
		 SUMMARYINFO SummaryInfo1=ReadSummary(nParamCount,nFreqNum,1);
		 WriteSummary(nParamCount,nFreqNum,2,SummaryInfo1);
	}*/
	//Commented by:sdt:04102005:2315:end//For Quad Software

	/*else{ //Added:MOD24072002:jj //Commented:jj:28072002
		if(PairSwitchOption.FEXT==ADJPAIRS){//== 28072002
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

	//Repositioned :jj:MOD24072002
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgAllComb);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgAllComb);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsAllComb);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsAllComb);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevAllComb);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevAllComb);
	SummaryInfo.STATRAW.fPowerSum=dRawPowerSumAllComb;
	SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSumAllComb;
	WriteSummary(nParamCount,nFreqNum,2,SummaryInfo); */

	//End

	m_FileInfo.nPairAdjacency[nParamCount]=PairSwitchOption.FEXT;
	m_FileInfo.nUnitAdjacency[nParamCount]=-1;

	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	//SelectReadingLines(nPos,OFF);//Commented by:sdt:28072001
	SelectReadingLines(nPos,ON);//Modified by:sdt:28072001


	ResetSystem();
    if(fpData){fclose(fpData);}

//	fAtnPairVal = FreePrevParamVals( fAtnPairVal,nParamCount );
    farfree(fNorVal);
    farfree(fRawVal);
    free(szSwitchOption);//MOD11072001:02:jj:
	return nReadingno;
}

float GetRawReadingFEXT (int nRef)
{
	int         Huntup = 0, Huntlow = 0;//,nCalNo;19032001
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
	//Uncommented by:sdt:04102005:2300
	fReading = fReading-gfOffset[nCurrentRange];// atn offset shold be used
	//fReading = fReading-HF_Offset[nConfig.nHFrequency][nCurrentRange];//Commented by:sdt:04102005:2300

	if(!nRef)
	{
		// for digi cm
		fReading *= fFcal[nCurrentRange];//Uncommented by:sdt:04102005:2300
		//fReading *= HF_CalFact[nConfig.nHFrequency][nCurrentRange]; //Commented by:sdt:04102005:2300
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

float normaliseFEXT (float fReading)
{
	char 	szTag[50];  //Added by:sdt:04022007:2330
	int 	nMin_length; //Added by:sdt:04022007:2330

	char    path[80];//22012001
	float   fReturn,fLe;//fRoomTemp;
	unsigned int		nCableLength;   //SDT11122016:0715
	float 	nor_length;

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
	if((nMin_length!=0)&&(nCableLength<nMin_length))
	{
		nCableLength = nMin_length;
	}

	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commented by:sdt:04102005:2318
	fReturn=fReading;
	fLe = nCableLength/( (nor_length) ? nor_length : 1 );  // Substituted nor_length instead 1000
	fReturn = ( fReturn + ( 10 * log10(fLe)));
	return fReturn;
}       // End of normalise


//Added by:sdt:17062006:2145:start
//int nfFextAtLowFreq(int nFreqNum)
//Modified by:sdt:19062006:2225
int nfXTalkAtLowFreq(int nParamNum, int nFreqNum)
{
	int nReturnVal =0;
	char    path[80];

	nParamCount=SRNO_CUPP;
	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
	{
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	ReadParamDotCts( paraminfo[nParamCount].szParamAbr , prmarray);
	ReadStdVal(nParamCount); //Added:jj:26032001

	lseek ( ParamCtsHandle, 0L, SEEK_SET );
	lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );

	lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount , SEEK_CUR );
	//nRead = read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );//Commented by:sdt:07052001
	read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
	close( ParamCtsHandle );


	CtPair.nTest=nParamNum; /*******Keep Watch on it**********/

	prmarray->max=nConfig.Max[nParamNum];

	InitialiseCALarray(); //Initialise the Cal array

	fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:28092005:1720

	if(!param_tested[nParamNum][nFreqNum]  || gnTest[nParamNum])//Added:jj:06042001
	{
		itoa (nTempUnitOf ,nConfig.szUnitof ,10);//san 21-11-97 For appending of the remaining pairs
		//nConfig.nLFrequency = nFreqNum; //Added by:sdt:20032001
		//Modified by:sdt:06102006
		//As Cupp must be measured at 1Khz i.e default freq.
		nConfig.nLFrequency = LFFrequency.nDefualtFreq;

		if(prmarray->nParamType)
			m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
		else
			m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

		if ( atof(nConfig.szLength) >= 2501.0 ) //Delay exchnged
		{
			ExchangeDelayInfo (TRUE);
			//Added:jj:24032001
			nConfig.nLFrequency=0;
			//Commented by:sdt:16022007:1345
			//Following line is commented as it sets nFreqNum to LFFrequency.nTotalFrequencies
			//which prints the testing freq. as 150 KHz.
			//nFreqNum=LFFrequency.nTotalFrequencies;//For immediate termination of loop

			SetLowFreq(nConfig.nLFrequency);
			//Post Frequency Delay Added by:sdt:03082001
		   delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );
		}

		CtPair.nTest=nParamNum; /*******Keep Watch on it**********/
		SetLowFreq(nConfig.nLFrequency);
		//Post Frequency Delay Added by:sdt:03082001
		delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

		SetHighFreq(nConfig.nHFrequency);
		//setupbargraph(nParamCount,nConfig.nLFrequency);//Commented by:sdt:28092005:1715

		sprintf(path,"%s:\\cts.ini",cRamDrive);
		factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,
											path);

		nReturnVal = CalibrateOneParam();

		SetLowFreq(nConfig.nLFrequency);
		//Post Frequency Delay Added by:sdt:03082001
		delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

		nTested[nParamNum]=TRUE;

		fPage->hint(hcStartTesting);//Added by:sdt:28092005:1620

		//nReturnVal=CalculateReadingsFEXT(nFreqNum);
		//Modified by:sdt:19062006:2227
		nReturnVal=CalculateReadingsXTALK(nParamNum,nFreqNum);
		m_FileInfo.cParamTested[nParamNum][nFreqNum]=TRUE;
		//Condtion Added by:sdt:28042015:2100
		if(runSetup.bAlarmsOff==False)
		{
			GiveAlarms(nFreqNum);//Changed by:sdt:21032001
		}
		fPage->hint(hcStopTesting);//Added by:sdt:28092005:1620
		if ( atof(nConfig.szLength) >= 2501.0 ) //Delay exchnged
		{
			ExchangeDelayInfo (FALSE);
		}
	}

	//if(nReturnVal ==-1)
	//	break;
	nParamCount=nParamNum;
	return nReturnVal;

}

//int CalculateReadingsFEXT(int nFreqNum)
//Modified by:sdt:19062006:2225
int CalculateReadingsXTALK(int nParamNum, int nFreqNum)
{
	float fCrRawVal=0.0,fCrNorVal=0.0;//Added by:sdt:16102006:1155
	float fCmRawVal=0.0,fCmNorVal=0.0;//Added by:sdt:05102006:2240
	char sztmpLFXT[50], sztmpLFXTVal[10];//Added by:sdt:28072006:2125
	float fLfxtOffext, fLfxtMultFact;//Added by:sdt:28072006:2125
	int nPairSwitch;
	SUMMARYINFO SummaryInfo;//Added by:sdt:03092001
	char *szSwitchOption;//Added by:sdt:03092001
	//int nAdjCount=0,nNonAdjCount=0;//Commented by:sdt:05102005:0020
	FILE *fpData;//Added:jj:18052001

	//Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
	unsigned long         ALocationSys;
	float                 ASysOffsetVal;

	//Added:sdt:01042001
	float  	far *fNorVal;//[MAX_NO_READINGS];
	float  	far *fRawVal;//[MAX_NO_READINGS];
//	char    path[80];//22012001//commented by: sdt:19032001
	int     cKey;

	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount;//, NotValid = 0;//commented by: sdt:07052001

	float   fRawReading = 0.0, fSysOffset=0.0;

	//Uncommented by:sdt:05102005:0020
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;
	double  dNorPowerSum=0.0,dRawPowerSum=0.0;


	READING InfoReading;
	struct  time prevtime;

	char    sztmp[20]; // for %d.Dat File
	int 	StartSwitchMode;//=GetLimitIndex(nParamCount); //added by:11052001//Commented by:sdt:03092001
	SSysOffsetVal m_SysOffsetVal;
	//Added by:sdt:02042001

	double Curfreq;
	char freqname[10],tfreqname[10];
	char *p;
	strcpy(freqname,SpeckList[nFreqNum]);
	//Added by:sdt:04122005:2120:start
	p=strtok(freqname,"HZ");
	strcpy(tfreqname, freqname);
	p=strtok(freqname,"K");
	if(!(strcmp(freqname, tfreqname)))
	{
		Curfreq = _atold(p);
	}
	else
		Curfreq = 1000 * _atold(p);



	float far *OnePerFextRaw;
	float far *OnePerFextNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamNum])/100);
	OnePerFextRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerFextNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerFextRaw,0,OnePerCount);
	memset(OnePerFextNorm,0,OnePerCount);



	fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamNum]+1));
	fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamNum]+1));
	memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamNum]+1));
	memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamNum]+1));
	//sprintf(sztmp,"%s:\\%d.dat",cRamDrive,nParamCount);//Commented by:sdt:20032001
	//Added by:sdt:03092001:1415
	//start
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
	szSwitchOption=(char *)farmalloc(sizeof(char)*nConfig.Max[nParamNum]);
	memset(szSwitchOption,0,sizeof(char)*nConfig.Max[nParamNum]);
	//end
	//Added by:sdt:26072006:2215:start
	fCrPairVal = FreePrevParamVals( fCrPairVal, SRNO_CR );
	fCmPairVal = FreePrevParamVals( fCmPairVal, SRNO_CM );
	fCrPairVal = GetPrevParamVals( fCrPairVal, SRNO_CR ,LFFrequency.nDefualtFreq);
	fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,LFFrequency.nDefualtFreq);
	//Added by:sdt:26072006:2215:end
	//Added by:sdt:05082006:2215:start
	char path[80];
	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	sprintf(sztmpLFXT,"LFXT_%s_INT_DEC",paraminfo[nParamNum].szParamAbr);
	prmarray->Decimal[0]=GetPrivateProfileInt("LFXT_SETUP",sztmpLFXT,3,path);//3;
	sprintf(sztmpLFXT,"LFXT_%s_FRACTION_DEC",paraminfo[nParamNum].szParamAbr);
	prmarray->Decimal[1]=GetPrivateProfileInt("LFXT_SETUP",sztmpLFXT,1,path);//1;
	//As prmarray contains value for CUPP parameter. It is showing
	//unit as "pf". But Xtalks unit is a "db", hence it is taken from cts.ini
	GetPrivateProfileString ("LFXT_SETUP","LFXT_UNIT",sztmpLFXTVal,"db",path);
	strcpy(prmarray->szUnit,sztmpLFXTVal);
	//Added by:sdt:05082006:2215:end



	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamNum,nFreqNum);

	bIncOkRej=1;

	//Modified:jj:18052001
	//if((fpData[nParamCount][nFreqNum] = fopen(sztmp,"wb"))==NULL)
	if((fpData= fopen(sztmp,"wb"))==NULL)
	{
		messageBox( "File creation error", mfError|mfOKButton );
		fs = fs_error;
		return 1;
	}
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamNum][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamNum][nFreqNum]=0;

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

	if(nParamNum == SRNO_FEXT)
	{
		nPairSwitch = PairSwitchOption.FEXT;
	}
	else if(nParamNum == SRNO_NEXT)
	{
		nPairSwitch = PairSwitchOption.ONEXT;
	}
	if((nPairSwitch==ADJPAIRS)||(nPairSwitch==NONADJPAIRS))
		nBypass=SelectFixturePairtoPair(1,nPairSwitch);//Commented by:sdt::04102005:2305
	else
		nBypass=SelectFixtureXTalkInQuad (1,nPairSwitch);//Added by:sdt::04102005:2305
	if(nBypass)
	{
		 while(nBypass)
		 {
				//Condition added by:sdt:12032006:2235
				if((nPairSwitch==ADJPAIRS)||(nPairSwitch==NONADJPAIRS))
					nBypass=SelectFixturePairtoPair(0,nPairSwitch);//Commented by:sdt::04102005:2305
				else
					nBypass=SelectFixtureXTalkInQuad (0,nPairSwitch);//Added by:sdt::04102005:2305
				nConfig.Max[nParamNum]--;
		 }
	}

	delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );
	StartSwitchMode=GetLimitIndex(nParamNum);//Added by:sdt:03092001:1258
	prmarray->max=nConfig.Max[nParamNum];

	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);
	//Start:Added by:sdt:28072006:2130


	sprintf(sztmpLFXT,"LFXT_%s_MULTIFACT",paraminfo[nParamNum].szParamAbr);
	GetPrivateProfileString ("LFXT_SETUP",sztmpLFXT,sztmpLFXTVal,"0.0",path);
	fLfxtMultFact=atof(sztmpLFXTVal);
	sprintf(sztmpLFXT,"LFXT_%s_OFFSET",paraminfo[nParamNum].szParamAbr);
	GetPrivateProfileString ("LFXT_SETUP",sztmpLFXT,sztmpLFXTVal,"0.0",path);
	fLfxtOffext=atof(sztmpLFXTVal);
	//End:Added by:sdt:28072006:2130



	nCurrentReadingno=0;//added by:sdt:21032001
	for(i=0;i<nConfig.Max[nParamNum];)
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
				StopPrint (); //Commented Temporary by:sdt:28092005:1715
				break;
		 default :
				break;
		 }
		 gettime(&currenttime);
		 //showtime(&currenttime);//Commented by:sdt:28092005:1715



		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			fRawReading=GetRawReadingCUPP(FALSE);

			SendSegmentData(CtPair,m_FileInfo);

			fRawReading=User_ceil(fRawReading,prmarray->Decimal[0]);

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
				//debug.fSysOff = fSysOffset;
				fRawReading -= fSysOffset;
			}
			//Implementing Formula for calculating XTalk at low frequency
			//Added by:sdt:26072006:2305:start
			if(!fRawReading)
			{
				fRawReading=1;
			}
			//Mofication:12102006:Important Modification************
			//For reading CR & CM values from arrays fCrPairVal & fCmPairVal
			//index used was CtPair.Pair1. Which was actually gives CR & CM values for
			//next reading. That causes problem while reading CR & CM for
			//last pair (i.e for reading of combination 12-1). Hence index
			//reduced by 1, which now giving the correct results.
			//Added by:sdt:05102006:2250:start
			fCmRawVal=fCmPairVal[CtPair.Pair1-1].fRawVal<=0?0.01:fCmPairVal[CtPair.Pair1-1].fRawVal;
			fCmNorVal=fCmPairVal[CtPair.Pair1-1].fNormVal<=0?0.01:fCmPairVal[CtPair.Pair1-1].fNormVal;
			//Added by:sdt:05102006:2250:end
			//Added by:sdt:16102006:1200:start
			//In further calculation fCrRawVal variable used.
			fCrRawVal=fCrPairVal[CtPair.Pair1-1].fRawVal<=0?0.2:fCrPairVal[CtPair.Pair1-1].fRawVal;
			fCrNorVal=fCrPairVal[CtPair.Pair1-1].fNormVal<=0?0.2:fCrPairVal[CtPair.Pair1-1].fNormVal;
			//Added by:sdt:16102006:1200:end
			//Changes made by:sdt:05102006:2335
			//Instead to seperate calculation for NEXT & FEXT, initial value
			//calculated commom for both parameters. For FEXT additional factor
			//is added to it.

			//sdt:05102006:2010
			//Cupp value converted into Farads, which was in Pico Farads (pf)
			//previously. (i.e. multiplication factor 1.0e-12 is added.
			//LFXT calculation for NEXT & FEXT
			CtPair.val = 10*log10(2*M_PI*Curfreq*fabs(fRawReading)*1.0e-12*sqrt(fCrRawVal/
							 (2*M_PI*Curfreq*fCmRawVal*1.0e-9)));
			CtPair.norval = 10*log10(2*M_PI*Curfreq*fabs(normaliseCUPP(fRawReading))*1.0e-12*sqrt(fCrNorVal/
								(2*M_PI*Curfreq*fCmNorVal*1.0e-9)));
			//Added by:sdt:28072006:2130
			//y=mx+c //Formula implemented.
			CtPair.val = (fLfxtMultFact*CtPair.val)+fLfxtOffext;
			CtPair.norval = (fLfxtMultFact*CtPair.norval)+fLfxtOffext;
			//Added by:sdt:28072006:1950
			//LFXT calculation for FEXT
			if(nParamNum==SRNO_FEXT)
			{
				CtPair.val = CtPair.val+(8.686*sqrt(M_PI*Curfreq*fCmRawVal*1.0e-9*fCrRawVal));
				CtPair.norval = CtPair.norval+(8.686*sqrt(M_PI*Curfreq*fCmNorVal*1.0e-9*fCrNorVal));
			}


			 //Commented by:sdt:26072006:2320
			//fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);
			//CtPair.val = fRawReading; //Added by:sdt:04102005:2345
			//CtPair.norval=normaliseCUPP(fRawReading);
			CtPair.val=User_ceil(CtPair.val, prmarray->Decimal[0]);
			CtPair.norval=User_ceil(CtPair.norval, prmarray->Decimal[0]);
			fRawReading=CtPair.val;//Added by:sdt:06102006:2300



			if(StartSwitchMode !=nAdjNonAdjFlag)
			{
				StartSwitchMode=nAdjNonAdjFlag;
			}
			fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:28092005:1440
			nParamCount=nParamNum;//Added by:sdt:27072006:1745
			nValidReading=CheckHiLoLimits(nFreqNum);//Modified by:sdt:23082001
			nParamCount=SRNO_CUPP;//Added by:sdt:27072006:1745

			switch(nValidReading)
			{
				case  F7 :
					 //Added by:sdt:02042001
					farfree(fNorVal);
					farfree(fRawVal);

					 flagReset = TRUE;
					 return -1;
				case  F8:
					 StopPrint ();//Commented Temporary by:sdt:28092005:1715
					 break;
				default :
					 break;
			}
		}while(!nValidReading);

		if(nValidReading==VALID)
			 m_FileInfo.nReadingsOk[nParamNum][nFreqNum]++;
		else
			 m_FileInfo.nReadingsNotOk[nParamNum][nFreqNum]++;
		fNorVal[i]=CtPair.norval;
		fRawVal[i]=fRawReading;

		//Added by:sdt:04102005:2310:start
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
			 StoreMinVal(fRawReading,nParamNum,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamNum,nFreqNum,0);
		}
		SummaryInfo=ReadSummary(nParamNum,nFreqNum,0);//Added by:sdt:03092001:1455
		if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamNum,nFreqNum,0);
		if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			StoreMaxVal(fRawReading,nParamNum,nFreqNum,0);
		InfoReading.cPair1=CtPair.Pair1;
		InfoReading.cPair2=CtPair.Pair2;
		InfoReading.cParam=nParamNum;
		InfoReading.nFreq = nFreqNum;//added by:sdt:23032001
		InfoReading.fRawVal=fRawReading;
		InfoReading.fNormVal=CtPair.norval;
		InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
		InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

		//Modified:jj:18052001
		 //fwrite(&InfoReading,sizeof(READING),1,fpData[nParamCount][nFreqNum]);
		fwrite(&InfoReading,sizeof(READING),1,fpData);


		if(i<OnePerCount+1)
		{
			OnePerFextRaw[i-1]=fRawReading;
			OnePerFextNorm[i-1]=CtPair.norval;
		 }
		 else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
		 {
			SortArray(OnePerFextRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerFextRaw,OnePerCount);
			SortArray(OnePerFextNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerFextNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerFextRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerFextNorm,OnePerCount);
		 }

		//Condition added by:sdt:12032006:2235
		if((nPairSwitch==ADJPAIRS)||(nPairSwitch==NONADJPAIRS))
			nBypass=SelectFixturePairtoPair(0,nPairSwitch);//Commented by:sdt:04102005:2305
		else
			nBypass=SelectFixtureXTalkInQuad (0,nPairSwitch);//Added by:sdt::04102005:2305
		if(nBypass)
		{
			while(nBypass)
			{
				//nBypass=SelectFixtureFEXT(0);
				//Condition added by:sdt:12032006:2235
				if((nPairSwitch==ADJPAIRS)||(nPairSwitch==NONADJPAIRS))
					nBypass=SelectFixturePairtoPair(0,nPairSwitch);//Commented by:sdt:04102005:2305
				else
					nBypass=SelectFixtureXTalkInQuad (0,nPairSwitch);//Added by:sdt::04102005:2305
				nConfig.Max[nParamNum]--;
			}
		}
	}

	Worstpairnext[nParamNum][nFreqNum][0]=(float)fabs(OnePerFextRaw[OnePerCount-1]);
	Worstpairnext[nParamNum][nFreqNum][1]=(float)fabs(OnePerFextNorm[OnePerCount-1]);
	farfree(OnePerFextRaw);
	farfree(OnePerFextNorm);

	//Added by:sdt:04102005:2315:start
	dRawAvg/=nConfig.Max[nParamNum];
	dNorAvg/=nConfig.Max[nParamNum];
	dRawRms/=nConfig.Max[nParamNum];
	dNorRms/=nConfig.Max[nParamNum];

	dRawRms=fabs(10*log10(dRawRms));
	dNorRms=fabs(10*log10(dNorRms));
	dRawPowerSum=fabs( 10 * log10(dRawPowerSum) );
	dNorPowerSum=fabs( 10 * log10(dNorPowerSum) );

	for(nSumCount=1;nSumCount<=nConfig.Max[nParamNum];nSumCount++)
	{
		 dRawStdDev+=pow((fabs((double)fRawVal[nSumCount])-dRawAvg),2);
		 dNorStdDev+=pow((fabs((double)fNorVal[nSumCount])-dNorAvg),2);
	}
	dRawStdDev/=nConfig.Max[nParamNum];
	if(dRawStdDev)
		 dRawStdDev=sqrt(dRawStdDev);
	dNorStdDev/=nConfig.Max[nParamNum];
	if(dNorStdDev)
		 dNorStdDev=sqrt(dNorStdDev);
	//Added by:sdt:03092001:1500
	//start
	SummaryInfo=ReadSummary(nParamNum,nFreqNum,0);
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvg);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvg);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRms);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRms);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDev);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDev);
	SummaryInfo.STATRAW.fPowerSum=dRawPowerSum;
	SummaryInfo.STATNORMALISED.fPowerSum=dNorPowerSum;
	WriteSummary(nParamNum,nFreqNum,0,SummaryInfo);

	m_FileInfo.nPairAdjacency[nParamNum]=nPairSwitch;
	m_FileInfo.nUnitAdjacency[nParamNum]=-1;

	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	//SelectReadingLines(nPos,OFF);//Commented by:sdt:28072001
	SelectReadingLines(nPos,ON);//Modified by:sdt:28072001


	ResetSystem();
	if(fpData){fclose(fpData);}

//	fAtnPairVal = FreePrevParamVals( fAtnPairVal,nParamCount );
	farfree(fNorVal);
	farfree(fRawVal);
	free(szSwitchOption);//MOD11072001:02:jj:

	fCrPairVal = FreePrevParamVals( fCrPairVal,nParamNum );
	fCmPairVal = FreePrevParamVals( fCmPairVal,nParamNum );


	return nReadingno;

}

