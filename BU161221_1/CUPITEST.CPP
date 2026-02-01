//#ifdef EXECUTE //Added by:sdt:21082001
//======================================================================
//							Modifications
//	Date :- 03072001 time :1230 by:sdt
//	1. SelectFixtureCUPPI function modified for handling the
//	   the last unit with the differnt size.
//======================================================================
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
#include "seq.h"//Added by:sdt:22042001
#include "funproto.h"
#include "frontpg.h"
#include "setup.h"

extern RunSetup runSetup; //Added by:sdt:28042015:2100

//Added by:sdt:03022007:2200
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];

//Added by:sdt:03092001
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);

//Added by:sdt:02092001//Commented by:sdt:04092001:1015
//extern double FinalAverageValue(double dVal,int nAdjCount,int nNonAdjCount);
//extern double FinalSqrtValue(double dVal);
//added by:sdt:11052001
//void ChangeLimits(int nParam,int nFreqNum,float fNorVal[]);
int GetLimitIndex(int Param);

//Added by:sdt:26042001
extern int nAdjNonAdjFlag; //0-Adjacent 1-NonAdjacent.
//Added by:sdt:23042001
extern PAIRSWITCH PairSwitchOption;//Added by:sdt:22042001
//Added by:sdt:22042001
extern SEQUENCE CableSequence;
SEQPAIR *Unitpair;
//Added:jj:17042001
int UnitListCount; //previous version Unit1Count
int Pair1Start;
int Pair2Start;
int Unit2;//1200

extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];

//extern void ReadStdVal(int nParamCount);
extern char 		cRamDrive[2];
extern char 		bIncOkRej;
extern int 			over;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
extern PRM			*prmarray;
extern int 			ParamCtsHandle; // File Handle to read PARAM.CTS file.
extern int 			noofparameters;
extern int			nPrevPair1,nPrevPair2;
extern int          gflagReplace1,gflagReplace2;

extern int 			nCount;
extern int 			nLastUnitSize;
extern int          nCurrentRange,nCurrentReadingno;
extern int 			nUnits;

extern float		gfOffset[MAXRANGES];
extern float        fFcal[MAXRANGES];


extern RdControl    	rdCtrl;
//extern SPECIFICATION    speck;
extern 	int	        	nTested[MAXPARAMETERS];

extern int      flagReset;

extern int 		nParamCount;

extern int      nflagRangeLock;
extern int      nTempUnitOf;
extern int      graphisuptodate;
extern char     szRangeHold[2];
extern struct   time currenttime;
extern          FileStatus fs;

//Commented:jj:18052001
//extern FILE  *fpData[MAXPARAMETERS][MAXFREQUENCIES];


extern INFORMATION      m_FileInfo;
extern CTPAIR           CtPair;
extern CONFIG           nConfig;

extern int 				factor;
//extern char 			SpeckList[10][30]; //Added by:sdt:20032001
extern FreqStruct 		HFFrequency,LFFrequency; //Added by:sdt:20032001
extern STRUCT_SPECKS speckData;

int 	GetParamReadingsCUPPI(int); //Changed by:sdt:20032001
int 	SelectFixtureCUPPI (int nStart);
float 	normaliseCUPPI(float fReading);
float 	GetRawReadingCUPPI (int nRef);

//extern void InitialiseCALarray(void);
//extern void	SelectSplitForCal();
//extern int 	SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);

//extern void ExchangeDelayInfo (int nExachange);//14032001
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001
extern TFrontPage *fPage; //Added by:sdt:28092005:1800

int CUPPITest()
{
    //int nRead=1;//Commented by:sdt:07052001
    int nReturnVal =0;
    char    path[80];
    int nFreqNum = 0; //added by:sdt:20032001

    if(atoi(nConfig.szNoofpairs)==atoi(nConfig.szUnitof))
    	return 0;
    nParamCount=SRNO_CUPPI;
    if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
    {
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	ReadParamDotCts( paraminfo[nParamCount].szParamAbr , prmarray);
    ReadStdVal(nParamCount); //Added:jj:26032001

	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used

    lseek ( ParamCtsHandle, 0L, SEEK_SET );
	lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );

    lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount , SEEK_CUR );
	//nRead = read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );//Commented by:sdt:07052001
    read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
    //read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );//Commented by:sdt:14062001
    close( ParamCtsHandle );

	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used
    CtPair.nTest=nParamCount;

	prmarray->max=nConfig.Max[nParamCount];

    InitialiseCALarray(); //Initialise the Cal array

	//Added by:sdt:28092005:1800
	fPage = (TFrontPage *)(new TFrontPage( "CUPPI TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}

	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:20032001
    {
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:28092005:1800
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum]) //Added by:sdt:20032001
        {
		    if(!param_tested[nParamCount][nFreqNum]  || gnTest[nParamCount])//Added:jj:06042001
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

			CtPair.nTest=nParamCount;
			SetLowFreq(nConfig.nLFrequency);
            //Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			SetHighFreq(nConfig.nHFrequency);
			//setupbargraph(nParamCount,nConfig.nLFrequency);//Commented by:sdt:28092005:1755

			sprintf(path,"%s:\\cts.ini",cRamDrive);
			factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,
											path);

			nReturnVal = CalibrateOneParam();

			SetLowFreq(nConfig.nLFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			nTested[nParamCount]=TRUE;
			fPage->hint(hcStartTesting);//Added by:sdt:28092005:1800
			nReturnVal=GetParamReadingsCUPPI(nFreqNum);
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//Changed by:sdt:21032001
			}
			fPage->hint(hcStopTesting);//Added by:sdt:28092005:1800

			if ( atof(nConfig.szLength) >= 2501.0 ) //Delay exchnged
			{
				ExchangeDelayInfo (FALSE);
			}
		}
	   }
		if(nReturnVal ==-1)
			break;
	}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}




int GetParamReadingsCUPPI(int nFreqNum)
{
	SUMMARYINFO SummaryInfo;
	char *szSwitchOption;//Added by:sdt:02092001
	//double dTempRemider=0.0;//Added by:sdt:02092001//Commented by:sdt:03092001
	//int nAdjCount=0,nNonAdjCount=0;//Commented by:sdt:05102005:0035
	FILE *fpData;//Added:jj:18052001

	//Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
	unsigned long         ALocationSys;
	float                 ASysOffsetVal;

	//Added:sdt:01042001
	float  	far *fNorVal;//[MAX_NO_READINGS];
	float  	far *fRawVal;//[MAX_NO_READINGS];
	int     cKey;

	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount;//, NotValid = 0;//Commented by:sdt:07052001

	float   fRawReading = 0.0, fSysOffset=0.0;
	//Uncommented by:sdt:05102005:0030
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;
	//Commented by:sdt:05102005:0030
	/*double  dRawAvgAdj=0.0,dRawRmsAdj=0.0,dRawStdDevAdj=0.0;
	double  dNorAvgAdj=0.0,dNorRmsAdj=0.0,dNorStdDevAdj=0.0;
	double  dRawAvgNonAdj=0.0,dRawRmsNonAdj=0.0,dRawStdDevNonAdj=0.0;
	double  dNorAvgNonAdj=0.0,dNorRmsNonAdj=0.0,dNorStdDevNonAdj=0.0;*/

	READING InfoReading;
	struct  time prevtime;

	char    sztmp[20]; // for %d.Dat File
	SSysOffsetVal m_SysOffsetVal;
	int	StartSwitchMode;//=GetLimitIndex(nParamCount)-2; //added by:11052001//Commented by:sdt:04092001

	//Added by:sdt:03022007:2205:Start
	float far *OnePerCupiRaw;
	float far *OnePerCupiNorm;
    int OnePerCount;
    OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerCupiRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerCupiNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerCupiRaw,0,OnePerCount);
	memset(OnePerCupiNorm,0,OnePerCount);
	//Added by:sdt:03022007:2205:End

	//Added by:sdt:02042001
	fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001
	//Added by:sdt:02092001
	//start
	szSwitchOption=(char *)farmalloc(sizeof(char)*nConfig.Max[nParamCount]);
	memset(szSwitchOption,0,sizeof(char)*nConfig.Max[nParamCount]);
	//end
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);

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


	nBypass=SelectFixtureCUPPI(1);

	if(nBypass)
	{
		  while(nBypass)
		 {
				nBypass=SelectFixtureCUPPI(0);
				nConfig.Max[nParamCount]--;
  		 }
	}

	delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );

    //StartSwitchMode=GetLimitIndex(nParamCount)-2;//Added by:sdt:04092001
    StartSwitchMode=GetLimitIndex(nParamCount)-3;//MOD11072002:02:jj

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
                //Added by:sdt:02042001
			    farfree(fNorVal);
			    farfree(fRawVal);

				flagReset = TRUE;
				return -1;
		 case  F8:
				StopPrint (); //Commented Temporary by:sdt:28092005:1755
				break;
		 default :
				break;
		 }
		 gettime(&currenttime);
		 //showtime(&currenttime);//Commented by:sdt:28092005:1755



		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			fRawReading=GetRawReadingCUPPI(FALSE);

			SendSegmentData(CtPair,m_FileInfo);

			fRawReading=User_ceil(fRawReading,prmarray->Decimal[0]);

			memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

			m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;

			m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nLFrequency;
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
				fSysOffset = User_ceil(fSysOffset, prmarray->Decimal[0]);
				fRawReading -= fSysOffset;
            }
			fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);
			CtPair.val = fRawReading; //Added by:sdt:04102005:2345
			CtPair.norval=normaliseCUPPI(fRawReading);
			CtPair.norval=User_ceil(CtPair.norval, prmarray->Decimal[0]);

            //Added by:sdt:11052001
            if(StartSwitchMode !=nAdjNonAdjFlag)
            {
				//ChangeLimits(nParamCount,fNorVal);
				//ChangeLimits(nParamCount,nFreqNum,fNorVal); //Modified by:sdt:23082001
                StartSwitchMode=nAdjNonAdjFlag;
            }

			//graphisuptodate=0;//Commented by:sdt:28092005:1755

			//graphicsupdate(nFreqNum,fNorVal);//Commented by:sdt:28092005:1755
			//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:28092005:1755
			//nValidReading=CheckHiLoLimits();
			fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:28092005:1800
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
					 StopPrint ();//Commented Temporary by:sdt:28092005:1755
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

		fNorVal[i]=CtPair.norval; // LF Params CM,CUPG,GM
		fRawVal[i]=fRawReading; // LF Params CM,CUPG,GM

		//Added by:sdt:05102005:0025:start
		//As for Quad software pair combinations i.e. Adjancies etc
		//is not concidered. Hence all summary readings are stored
		//as same as other parameters
		 dRawAvg+=fabs((double)fRawReading);
		 dNorAvg+=fabs((double)CtPair.norval);
		 dRawRms+=fabs((double)(fRawReading*fRawReading));
		 dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));

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
		//Added by:sdt:05102005:0025:end

		//Added by:sdt:03022007:2205:Start
		//Worst pair next calculations
		 if(i<OnePerCount+1)
         {
			OnePerCupiRaw[i-1]=fRawReading;
			OnePerCupiNorm[i-1]=CtPair.norval;
         }
         else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
         {
			SortArray(OnePerCupiRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerCupiRaw,OnePerCount);
			SortArray(OnePerCupiNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCupiNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerCupiRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerCupiNorm,OnePerCount);
		 }
		 //Added by:sdt:03022007:2205:End



		//Commented by:sdt:05102005:sdt:0025:start
		//Added by:sdt:02092001
		//This conditions added for storing Adjacent & NonAdjacent
		//values separately in a single variable.
		/*if(nAdjNonAdjFlag+3==ADJUNITS)
		{
			nAdjCount++;
			dRawAvgAdj+=fabs((double)fRawReading);
			dNorAvgAdj+=fabs((double)CtPair.norval);
			dRawRmsAdj+=fabs((double)(fRawReading*fRawReading));
			dNorRmsAdj+=fabs((double)(CtPair.norval*CtPair.norval));
		}
		else
		{
			nNonAdjCount++;
			dRawAvgNonAdj+=fabs((double)fRawReading);
			dNorAvgNonAdj+=fabs((double)CtPair.norval);
			dRawRmsNonAdj+=fabs((double)(fRawReading*fRawReading));
			dNorRmsNonAdj+=fabs((double)(CtPair.norval*CtPair.norval));
		}
		//if(i==1)
		//Modified by:sdt:07092001:1540
		//This condition added to store Min & Max val of Adjacent &
		//Nonadjacent pairs seperately.
		if(((nAdjNonAdjFlag+3)==ADJUNITS)&&(nAdjCount==1)||((nAdjNonAdjFlag+3)==NONADJUNITS)&&(nNonAdjCount==1))
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
		}
		SummaryInfo=ReadSummary(nParamCount,nFreqNum,nAdjNonAdjFlag);//Added by:sdt:03092001
		//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
		//Modified by:sdt:03092001
		if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
		{
			StoreMinVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			m_FileInfo.nMinAdjNonAdj[nParamCount]=nAdjNonAdjFlag;
		}
		//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
		//Modified by:sdt:03092001
		if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
		{
			StoreMaxVal(fRawReading,nParamCount,nFreqNum,nAdjNonAdjFlag);
			m_FileInfo.nMaxAdjNonAdj[nParamCount]=nAdjNonAdjFlag;
		}
		//statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:28092005:1800
		InfoReading.cPair1=CtPair.Pair1;
		InfoReading.cPair2=CtPair.Pair2;
		InfoReading.cParam=nParamCount;
		InfoReading.nFreq = nFreqNum;//Added by:sdt:23032001
		InfoReading.fRawVal=fRawReading;
		InfoReading.fNormVal=CtPair.norval;
		InfoReading.nPairAdjacency=-1; //Added by:sdt:26042001
		InfoReading.nUnitAdjacency=nAdjNonAdjFlag;//Added by:sdt:26042001

		 szSwitchOption[i-1]=nAdjNonAdjFlag;//Added by:sdt:02092001
		//Modified:jj:18052001
		//fwrite(&InfoReading,sizeof(InfoReading),1,fpData[nParamCount][nFreqNum]);
		fwrite(&InfoReading,sizeof(InfoReading),1,fpData);*/
		//Commented by:sdt:05102005:sdt:0025:end

		nBypass=SelectFixtureCUPPI(0);
		if(nBypass)
		{
			 while(nBypass)
			 {
					nBypass=SelectFixtureCUPPI(0);
					nConfig.Max[nParamCount]--;

			 }
		}

	}

	//Added by:sdt:03022007:2205:Start
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerCupiRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerCupiNorm[OnePerCount-1]);
	farfree(OnePerCupiRaw);
	farfree(OnePerCupiNorm);
	//Added by:sdt:03022007:2205:End


	//Added by:sdt:05102005:0030:start
	dRawAvg/=nConfig.Max[nParamCount];
	dNorAvg/=nConfig.Max[nParamCount];
	dRawRms/=nConfig.Max[nParamCount];
	dNorRms/=nConfig.Max[nParamCount];

	dRawRms=sqrt(dRawRms);
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
	//Added by:sdt:03092001:1500
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
	//Added by:sdt:05102005:0030:end


	//Commented by:sdt:05102005:0030:start//For Quad Software
	/*if(nAdjCount!=0)
	{
		dRawAvgAdj/=nAdjCount;
		dNorAvgAdj/=nAdjCount;
		dRawRmsAdj/=nAdjCount;
		dRawRmsAdj=sqrt(dRawRmsAdj);
		dNorRmsAdj/=nAdjCount;
		dNorRmsAdj=sqrt(dNorRmsAdj);
	}

	if(nNonAdjCount!=0)
	{
		dRawAvgNonAdj/=nNonAdjCount;
		dNorAvgNonAdj/=nNonAdjCount;
		dRawRmsNonAdj/=nNonAdjCount;
		dRawRmsNonAdj=sqrt(dRawRmsNonAdj);
		dNorRmsNonAdj/=nNonAdjCount;
		dNorRmsNonAdj=sqrt(dNorRmsNonAdj);
	}
	//end
	for(nSumCount=1;nSumCount<=nConfig.Max[nParamCount];nSumCount++)
	{
		if(szSwitchOption[nSumCount-1]+3==ADJUNITS)
		{
			dRawStdDevAdj+=pow((fabs((double)fRawVal[nSumCount])-dRawAvgAdj),2);
			dNorStdDevAdj+=pow((fabs((double)fNorVal[nSumCount])-dNorAvgAdj),2);
		}
		else
		{
			dRawStdDevNonAdj+=pow((fabs((double)fRawVal[nSumCount])-dRawAvgNonAdj),2);
			dNorStdDevNonAdj+=pow((fabs((double)fNorVal[nSumCount])-dNorAvgNonAdj),2);
		}

	}
	//start
	if(nAdjCount!=0)
	{
		dRawStdDevAdj/=nAdjCount;
		if(dRawStdDevAdj)
			 dRawStdDevAdj=sqrt(dRawStdDevAdj);
		dNorStdDevAdj/=nAdjCount;
		if(dNorStdDevAdj)
			 dNorStdDevAdj=sqrt(dNorStdDevAdj);
	}
	if(nNonAdjCount!=0)
	{
		dRawStdDevNonAdj/=nNonAdjCount;
		if(dRawStdDevNonAdj)
			 dRawStdDevNonAdj=sqrt(dRawStdDevNonAdj);
		dNorStdDevNonAdj/=nNonAdjCount;
		if(dNorStdDevNonAdj)
			 dNorStdDevNonAdj=sqrt(dNorStdDevNonAdj);
	}

	//Added by:sdt:03092001
	//start
	//For Adjacent Units
	SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgAdj);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgAdj);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsAdj);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsAdj);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevAdj);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevAdj);
	SummaryInfo.STATRAW.fPowerSum=0.0;
	SummaryInfo.STATNORMALISED.fPowerSum=0.0;
	WriteSummary(nParamCount,nFreqNum,0,SummaryInfo);

	//For NonAdjacent Units
	SummaryInfo=ReadSummary(nParamCount,nFreqNum,1);
	SummaryInfo.STATRAW.fAvg=(float)fabs(dRawAvgNonAdj);
	SummaryInfo.STATNORMALISED.fAvg=(float)fabs(dNorAvgNonAdj);
	SummaryInfo.STATRAW.fRms=(float)fabs(dRawRmsNonAdj);
	SummaryInfo.STATNORMALISED.fRms=(float)fabs(dNorRmsNonAdj);
	SummaryInfo.STATRAW.fStddev=(float)fabs(dRawStdDevNonAdj);
	SummaryInfo.STATNORMALISED.fStddev=(float)fabs(dNorStdDevNonAdj);
	SummaryInfo.STATRAW.fPowerSum=0.0;
	SummaryInfo.STATNORMALISED.fPowerSum=0.0;
	WriteSummary(nParamCount,nFreqNum,1,SummaryInfo);*/
	//End
	//Commented by:sdt:05102005:0030:end//For Quad Software


	//Added by:sdt:26042001
	m_FileInfo.nPairAdjacency[nParamCount]=-1;
	//m_FileInfo.nUnitAdjacency[nParamCount]=nAdjNonAdjFlag;//Commented by:sdt:09052001
	m_FileInfo.nUnitAdjacency[nParamCount]=PairSwitchOption.CUPPI-3;//Added by:sdt:09052001

	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	SelectReadingLines(nPos,OFF);
	SelectRange(nCurrentRange,OFF);

	ResetSystem();

	//Modified:jj:18052001
	//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
	if(fpData){fclose(fpData);}

	//Added by:sdt:02042001
	farfree(fNorVal);
	farfree(fRawVal);
	free(szSwitchOption);//MOD11072001:02:jj:
	return nReadingno;
}

float GetRawReadingCUPPI (int nRef)
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
	//amit for locking range
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

			if ( (fReading == prmarray->fStartofscale[nCurrentRange] ) )
					fReading = fabs( fReading );

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


float normaliseCUPPI (float fReading)
{
	char 	szTag[50];  //Added by:sdt:04022007:2330
	int 	nMin_length; //Added by:sdt:04022007:2330

	char    path[80];//22012001
	float   fSeq,fReturn,fLe;//fRoomTemp;
	unsigned int		nCableLength;

	float 	nor_length;
	int 	uCuppMmtMethod;

	//nor_length = atof( speck.speckparams[nParamCount].norlength);
	//nor_length = speckData.fNormLength;
	//Modified by:sdt:31012007:2130
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

	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commeted by sdt:28092005:1735
	//As fRoomTemp is not used in any calculations.
	//if(nCableLength < 100)
	//	nCableLength = 100;
	//Condition changed by:sdt:07012006:2330
	//Requirement of SDX for quad software.
	if(nCableLength < 500)
		nCableLength = 500;

	fReturn=fReading;

	uCuppMmtMethod = (unsigned char) GetPrivateProfileInt("MEASUREMENT_OPTIONS",
					"CUPPI_MMT_METHOD",1,"default.ini");
	if ( uCuppMmtMethod == CUPP_SQRT_MTH )
		fReturn /= sqrt(nCableLength/( (nor_length) ? nor_length : 1 ));
	else if ( uCuppMmtMethod == CUPP_LINEAR_MTH )
			fReturn /= nCableLength/( (nor_length) ? nor_length : 1 );
	else if ( uCuppMmtMethod == CUPP_AVG_MTH )
	{
		fSeq=sqrt(nCableLength/( (nor_length) ? nor_length : 1 ));
		fLe = nCableLength/( (nor_length) ? nor_length : 1 );
		fReturn /= (fSeq+fLe)/2;
	}
	return fReturn;
}

//Added:jj:17042001

int SelectFixtureCUPPI (int nStart)
{
	int nBypass;
	if(nStart)
	{
    	UnitListCount=0;
        if(PairSwitchOption.CUPPI==ADJUNITS||PairSwitchOption.CUPPI==BOTH_UNITS)
        {
	        Unitpair=CableSequence.TotalPairs.Units.Adj_Units;
			nAdjNonAdjFlag=0;
        }
        if(PairSwitchOption.CUPPI==NONADJUNITS)
        {
        	Unitpair=CableSequence.TotalPairs.Units.NonAdj_Units;
            nAdjNonAdjFlag=1;
        }
    	CtPair.UnitSrNo=Unitpair->Pairno1;
        //Unit2=CableSequence.TotalPairs.Units.Adj_Units[UnitListCount][1];
        Unit2=Unitpair->Pairno2;
		gflagReplace1 = 0;
		gflagReplace2 = 0;
        Pair1Start=1;
		CtPair.Pair1=Pair1Start;

        //condition Added by:sdt:03072001
        //Calculation for no of pairs in the first unit.
        if(CtPair.UnitSrNo!=nUnits)
			nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
        else
        	nCount = CtPair.Pair1 + (nLastUnitSize - 1);

		nPrevPair2=0;
		nPrevPair1=0;

        Pair2Start=(Unit2-1)*atoi(nConfig.szUnitof)+1;
		CtPair.Pair2=Pair2Start;

		nBypass=SetFixture(CtPair.Pair1,CtPair.Pair2,FALSE);

		//CtPair.UnitSrNo=1;
		nPrevPair1 = CtPair.Pair1;
		nPrevPair2 = CtPair.Pair2;
	}
	else
	{
	    if((Unit2==nUnits)&&(CtPair.Pair2 < ((atoi(nConfig.szUnitof))*(Unit2-1)+nLastUnitSize)))
           	CtPair.Pair2++;
        else
		if((Unit2!=nUnits)&&(CtPair.Pair2 < (atoi(nConfig.szUnitof))*Unit2))
			CtPair.Pair2++;
		else
		{
			if(CtPair.Pair1 < nCount)//-1)//Modified:jj:17042001:1200
			{
				CtPair.Pair1++;
				CtPair.Pair2=Pair2Start;
			}
			else
			{
            	UnitListCount++;
       	        Unitpair=Unitpair->link;
           	    //Added by:sdt:22042001
               	if(Unitpair ==NULL&&PairSwitchOption.CUPPI==BOTH_UNITS)
	            {
					Unitpair=CableSequence.TotalPairs.Units.NonAdj_Units;
       	        	nAdjNonAdjFlag=1;
           	    }
               	if(Unitpair !=NULL)//Added by:sdt:11052001
	            {
	            	CtPair.UnitSrNo=Unitpair->Pairno1;//Added by:sdt:22042001
    	            Unit2 = Unitpair->Pairno2;
					//CableSequence.TotalPairs.Units.Adj_Units[UnitListCount][0];
					//Unit2=CableSequence.TotalPairs.Units.Adj_Units[UnitListCount][1];
					CtPair.Pair1=(CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof)+1;;
					CtPair.Pair2=(Unit2-1)*atoi(nConfig.szUnitof)+1;

                    //Commented by:sdt:03072001
					//nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);

                    //condition Added by:sdt:03072001
			        //Calculation for no of pairs in the first unit.
			        if(CtPair.UnitSrNo!=nUnits)
						nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
			        else
			        	nCount = CtPair.Pair1 + (nLastUnitSize-1);

	           	    Pair2Start=(Unit2-1)*atoi(nConfig.szUnitof)+1;//Added by:sdt:23042001
    	        }
			}
		}
		nBypass=SetPair(CtPair.Pair1,CtPair.Pair2,nPrevPair1,nPrevPair2);
		over++;
		nPrevPair1=CtPair.Pair1;
		nPrevPair2=CtPair.Pair2;
	}
	return nBypass;
}

//#endif //Added by:sdt:21082001
