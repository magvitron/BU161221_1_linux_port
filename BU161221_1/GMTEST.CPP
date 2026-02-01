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
#include "funproto.h"
#include "frontpg.h"
#include "setup.h"

extern RunSetup runSetup; //Added by:sdt:28042015:2100


//Added by:sdt:03092001
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);

//Added:jj:06052001
extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];


//extern void ReadStdVal(int nParamCount);//Added:jj:26032001
int GetParamReadingsGM (int); //modified by:sdt:20032001
int SelectFixtureGM (int nStart);
float GetRawReadingGM (int nRef);
float normaliseGM (float fReading);

extern Calibration  	Cal;
//extern void		SelectSplitForCal( );
//extern int     	SetParamFreq();
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
extern float 		User_ceil(float fFlotVal,int nNo);
extern  int        	nTested[MAXPARAMETERS];
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

//Commented by:sdt:01042001
/*extern float far    fNorVal[MAX_NO_READINGS];
extern float far 	fRawVal[MAX_NO_READINGS];*/
extern char			bIncOkRej;
extern char 		SpeckList[10][30]; //Added by:sdt:20032001
extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:20032001
extern STRUCT_SPECKS speckData;

extern TFrontPage *fPage; //Added by:sdt:27092005:2300

int GmTest()
{
	char	path[80];
	int nFreqNum = 0; //added by:sdt:20032001
	nCount=0;
	int nReturnVal=0;

	nParamCount=SRNO_GM;
	//memset( prmarray, 0, sizeof( prmarray ) );
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

	prmarray->fAccuracyfsv = 0.05;
	FillDispStruct(prmarray,nParamCount);
	prmarray->max=nConfig.Max[nParamCount];

	CtPair.nTest=nParamCount;
	InitialiseCALarray();
	//Added by:sdt:28092005:1150
	fPage = (TFrontPage *)(new TFrontPage( "GM TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}

	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:20032001
	{
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:28092005:1150
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
			//setupbargraph(nParamCount,nFreqNum);//Modified by:sdt:20032001//Commented by:sdt:28092005:1205
			sprintf(path,"%s:\\cts.ini",cRamDrive);
			factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);
			CalibrateOneParam();
			nCurrentRange =0;

			SetLowFreq(nConfig.nLFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			nTested[nParamCount]=TRUE;

			//m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			fPage->hint(hcStartTesting);//Added by:sdt:28092005:1150
			nReturnVal=GetParamReadingsGM(nFreqNum);
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			GiveAlarms(nFreqNum);//changed by:sdt:21032001
			fPage->hint(hcStopTesting);//Added by:sdt:28092005:1150
		}
		if(nReturnVal ==-1)
			break;
	   }
	}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}


int GetParamReadingsGM (int nFreqNum)
{
	SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1650
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

	int     cKey,first1=TRUE;//,first2=TRUE;//19032001
	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount;//, NotValid = 0;//19032001

	float   fRawReading = 0.0, fSysOffset=0.0;
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING InfoReading;//TipInfoReading,//19032001
	struct  time prevtime;

	char    sztmp[20];

	SSysOffsetVal m_SysOffsetVal;

	fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1650
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);//added by:sdt:20032001
	//	sprintf(sztmp,"%s:\\%d.dat",RamDrv,nParamCount);
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
	SelectRange(nCurrentRange,ON);

	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	nBypass=SelectFixtureGM(1);

	if(nBypass)
	{
		  while(nBypass)
		 {
				nBypass=SelectFixtureGM(0);
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
					//StopPrint ();//Commented Temporary by:sdt:28092005:1205
					break;
		default :
					break;
		}
		gettime(&currenttime);
		//showtime(&currenttime);//Commented by:sdt:28092005:1205
		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			fRawReading=GetRawReadingGM(FALSE);
			if (first1 )   // all cm readings taken twice 2/12/98
			{
					first1 = FALSE;
					fRawReading=GetRawReadingGM(FALSE);
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
			 CtPair.norval=normaliseGM(fRawReading);
			 CtPair.norval=User_ceil(CtPair.norval, prmarray->Decimal[0]);

			 //graphisuptodate=0;//Commented by:sdt:28092005:1200
			 //graphicsupdate(nFreqNum);//Modified by:sdt:20032001
			 //graphicsupdate(nFreqNum,fNorVal);//Modified:jj:01042001//Commented by:sdt:28092005:1200
			 //currentdisplay(fRawReading,nParamCount);//Commented by:sdt:28092005:1200
			 //nValidReading=CheckHiLoLimits();
			 fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:28092005:1150
			 nValidReading=CheckHiLoLimits(nFreqNum);//Modified by:sdt:23082001
			 switch(nValidReading)
			 {
				case  F7 :
					 farfree(fNorVal);
					 farfree(fRawVal);

					 flagReset = TRUE;
					 return -1;
				case  F8:
					 //StopPrint ();//Commented Temporary by:sdt:28092005:1205
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
		SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1650
		//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
		//Modified by:sdt:03092001:1650
		if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
		//Modified by:sdt:03092001:1650
		if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		//statusdisplay(nParamCount);
		//statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:28092005:1210
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

		nBypass=SelectFixtureGM(0);
		if(nBypass)
		{
			 while(nBypass)
			 {
					nBypass=SelectFixtureGM(0);
					nConfig.Max[nParamCount]--;
			 }
		}
	}
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
	//Commented by:sdt:03092001
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
	//Added by:sdt:03092001:1655
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

	param_tested[SRNO_GM][nFreqNum]=1;//Added:jj:06042001

	//Added by:sdt:02042001
	farfree(fNorVal);
	farfree(fRawVal);
	return nReadingno;
}

float GetRawReadingGM (int nRef)
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
	return fReading;
}


float normaliseGM (float fReading)
{
	char    path[80];
	float   fReturn;//fRoomTemp;
	unsigned int		nCableLength;
	float 	nor_length;

	//nor_length = atof( speck.speckparams[nParamCount].norlength);//Commented by:sdt:28092005:1200
	//nor_length = speckData.fNormLength;
	//Modified by:sdt:31012007:2140
	nor_length = speckData.speckparams[nParamCount].fNormLength;

	 sprintf(path,"%s:\\cts.ini",cRamDrive);
	if ( GetPrivateProfileInt("BASIC_INFO",
			"NORMALISE_KM",0,path) )
		nor_length = 1000.0;
	else if ( nor_length == (float)0.0 )
			nor_length = 1000.0;

	nCableLength=atoi(nConfig.szLength);
	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commeted by sdt:28092005:1735
	//As fRoomTemp is not used in any calculations.
	fReturn=fReading;
	fReturn *=(float)(nor_length/( ((unsigned int)nCableLength) ? (unsigned int)nCableLength : 1 )); //SDT11122016:0700
	return fReturn;
}

int SelectFixtureGM (int nStart)
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
}


//#endif //Added by:sdt:21082001