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
int GetParamReadingsCUPS (int);//modified by:sdt:20032001
int SelectFixtureCUPS (int nStart);
float GetRawReadingCUPS (int nRef);
float normaliseCUPS (float fReading);

extern Calibration  	Cal;
//extern void		SelectSplitForCal( );
//extern int     	SetParamFreq();
//extern float GetAverageReadingSync(int nRange,int nRef);
//extern int SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
//extern void InitialiseCALarray(void);
//extern void ExchangeDelayInfo (int nExachange);
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001

//Commented:jj:11042001
//extern float                    cupgPosOff[MAXFIXCOMB][MAXPAIRS];

/*extern unsigned long  far       ALocationSys[1000];
extern float far                ASysOffsetVal[1000];*/
extern int          gflagReplace1, gflagReplace2 ;
extern   			SPECIFICATION  	speck;
extern int      	graphisuptodate;
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
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
extern INFORMATION  m_FileInfo;
extern CONFIG       nConfig;
extern int 			GShort;
extern char 		cRamDrive[2];
extern int 			noofparameters;
extern int      	bSync;

//Commented:jj:18052001
//extern FILE  *fpData[MAXPARAMETERS][MAXFREQUENCIES];

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

//commented by:sdt:01042001
/*extern float far	fNorVal[MAX_NO_READINGS];
extern float far	fRawVal[MAX_NO_READINGS];*/

extern char 		bIncOkRej;
extern int			nCurrentRange;
extern char 		SpeckList[10][30]; //Added by:sdt:20032001
extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:20032001
extern STRUCT_SPECKS speckData;

extern TFrontPage *fPage; //Added by:sdt:27092005:2300

int CupsTest()
{
	char	path[80];
	nCount=0;
	int nReturnVal=0;
	int nFreqNum = 0; //added by:sdt:20032001

	nParamCount=SRNO_CUPS;
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

	FillDispStruct(prmarray,nParamCount);

	//Commented by:sdt:20032001
	//if(prmarray->nParamType)
	//	m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
	//else
	//	m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

	prmarray->max=nConfig.Max[nParamCount];

	CtPair.nTest=nParamCount;

	//Commented:jj:08042001
	//graphicsinit();//Added:jj:05042001

	InitialiseCALarray();

	 //Commented:jj:06042001
	//CheckForDefaultFreq(nParamCount,prmarray->nParamType);//Added by:sdt:23032001
	//Added by:sdt:28092005:1455
	fPage = (TFrontPage *)(new TFrontPage( "CUPS TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}

	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:20032001
	{
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:28092005:1455
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

			/*for(int j=0;j<MAX_NO_READINGS;j++)
			{
				fNorVal[j]=(float)BLANKVALUE;
				fRawVal[j]=(float)BLANKVALUE;
			}*/

			if ( atof(nConfig.szLength) >= 2501.0 )
			{
				 ExchangeDelayInfo (TRUE);

				//strcpy(nConfig.szLFreq,"80Hz");
				//For loop added by sdt:20032001

//Commented:jj:24032001
/*                for(int i=0;(i<LFFrequency.nTotalFrequencies)&&(strcmp(SpeckList[i],"0.08 KZ"));i++)
				nConfig.nLFrequency=i;
*/
				nConfig.nLFrequency=0;//Added:jj:24032001
				nFreqNum=LFFrequency.nTotalFrequencies;//For immediate termination of loop

				SetLowFreq(nConfig.nLFrequency);
				//Post Frequency Delay Added by:sdt:03082001
			   delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );
			}

//Commented:jj:24032001
/*            else
			{
				if ( strcmp(SpeckList[nFreqNum],"0.08 Hz") == 0 )
				{
					//strcpy(nConfig.szLFreq,"1KHz");
					for(int i=0;(i<LFFrequency.nTotalFrequencies)&&(strcmp(SpeckList[i],"1 KZ"));i++)
					nConfig.nLFrequency=i;
					SetLowFreq(nConfig.nLFrequency);
				}
			}
*/

			CtPair.nTest=nParamCount;
			SetLowFreq(nConfig.nLFrequency);
			//Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

			SetHighFreq(nConfig.nHFrequency);
			//setupbargraph(nParamCount,nFreqNum);//Modified by:sdt:20032001
			//setupbargraph(nParamCount,nConfig.nLFrequency);//Commented by:sdt:28092005:1445//Modified:jj:24032001 for display of correct freq in case of length >2500
			sprintf(path,"%s:\\cts.ini",cRamDrive);

			factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);
			CalibrateOneParam();
			nCurrentRange =0;

    		SetLowFreq(nConfig.nLFrequency);
            //Post Frequency Delay Added by:sdt:03082001
			delay( rdCtrl.m_PostFreqDelay[nConfig.nLFrequency] );

		    nTested[nParamCount]=TRUE;

            //m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			fPage->hint(hcStartTesting);//Added by:sdt:28092005:1455
			nReturnVal=GetParamReadingsCUPS(nFreqNum);
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			if ( atof(nConfig.szLength) >= 2501.0 )
				 ExchangeDelayInfo (FALSE);

			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//Changed by:sdt:21032001
			}
			fPage->hint(hcStopTesting);//Added by:sdt:28092005:1455
		}
        }
    	if(nReturnVal ==-1)
        	break;
	}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}

int SelectFixtureCUPS (int nStart)
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


int GetParamReadingsCUPS (int nFreqNum)
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1635
    FILE *fpData;//Added:jj:18052001

    //Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
    unsigned long         ALocationSys;
	float                 ASysOffsetVal;

    //Added:sdt:01042001
    //Modifid by:sdt:02042001
    float 	far *fNorVal;//[MAX_NO_READINGS];
	float 	far *fRawVal;//[MAX_NO_READINGS];

	char    path[80];//22012001
	int     cKey,first1=TRUE;//,first2=TRUE;//Commented by:sdt:07052001
    int 	cupsFrst = TRUE;
    int 	cupsNord;

	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass=0;
	int     nSumCount;//, NotValid = 0;//Commented by:sdt:07052001

	float   fRawReading = 0.0, fSysOffset=0.0;

	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING  InfoReading;
	struct  time prevtime;

	char    sztmp[20]; // for %d.Dat File
    SSysOffsetVal m_SysOffsetVal;
    //Added by:sdt:02042001
    fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));

    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1635

	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	cupsNord = GetPrivateProfileInt("CUPS_RDGS","NORD",2,path);
	cupsFrst = cupsNord;

	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum); //Changed by:sdt:20032001
	//remove (sztmp);
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
		SelectRange(nCurrentRange,ON);

    delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	nBypass=SelectFixtureCUPS(1);

	if(nBypass)
	{
		  while(nBypass)
		 {
				nBypass=SelectFixtureCUPS(0);
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
                 //Added by:sdt:02042001
			    farfree(fNorVal);
			    farfree(fRawVal);

				flagReset = TRUE;
				return -1;
		 case  F8:
				//StopPrint ();//Commented Temporary by:sdt:28092005:1445
				break;
		 default :
				break;
		 }
		 gettime(&currenttime);
		 //showtime(&currenttime);//Commented by:sdt:28092005:1445

		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{
			fRawReading=GetRawReadingCUPS(FALSE);
			if (first1 )
			{
				first1 = FALSE;
				fRawReading=GetRawReadingCUPS(FALSE);
			}

            SendSegmentData(CtPair,m_FileInfo);
			fRawReading=User_ceil(fRawReading,prmarray->Decimal[0]);
			//fSysOffset = GetSysOffsetFromArray (nParamCount);
            memset (&m_SysOffsetVal,0,sizeof (m_SysOffsetVal));

			m_SysOffsetVal.m_SysOffsetLocation.FixNo = m_FileInfo.cFixtureNo + 1;
			m_SysOffsetVal.m_SysOffsetLocation.Freq = nConfig.nLFrequency;
			m_SysOffsetVal.m_SysOffsetLocation.Freq += 1;
			m_SysOffsetVal.m_SysOffsetLocation.ParamAbr = nParamCount - 1;

			m_SysOffsetVal.m_SysOffsetLocation.Range = nCurrentRange + 1;
            //commented by:sdt:02042001
            /*int j=0;
			for (j = 0; j<1000;j ++)
				 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
					 break;
            fSysOffset =ASysOffsetVal [j];*/
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

			fSysOffset = User_ceil(fSysOffset, prmarray->Decimal[0]);
			fRawReading -= fSysOffset;
			fRawReading = User_ceil(fRawReading, prmarray->Decimal[0]);

//Commented:jj:11042001
/*			float fTempCupgOff=cupgPosOff[m_FileInfo.cFixtureNo][CtPair.Pair1+m_FileInfo.cStartFixPos-2];
			fTempCupgOff=User_ceil(fTempCupgOff, prmarray->Decimal[0]);
			fRawReading -= fTempCupgOff;
 */
			CtPair.norval=normaliseCUPS(fRawReading);
			CtPair.norval=User_ceil(CtPair.norval, prmarray->Decimal[0]);

			//graphisuptodate=0;//Commented by:sdt:28092005:1445

			if ( !cupsFrst )
			{
				//graphicsupdate(nFreqNum);//Modified by:sdt:20032001
				//graphicsupdate(nFreqNum,fNorVal);//Modified:sdt:01042001//Commented by:sdt:28092005:1445
				//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:28092005:1445
				//nValidReading=CheckHiLoLimits();
				fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:28092005:1455
                nValidReading=CheckHiLoLimits(nFreqNum);//Modified by:sdt:23082001
		  	}

			switch(nValidReading)
			{
				case  F7 :
                      //Added by:sdt:02042001
					 farfree(fNorVal);
				     farfree(fRawVal);

					 flagReset = TRUE;
					 return -1;
				case  F8:
					 //StopPrint ();//Commented Temporary by:sdt:28092005:1445
					 break;
				default :
					 break;
			}

			if ( cupsFrst )
			{
				 cupsFrst--;
				 if ( cupsFrst == -1 )
					nValidReading = 1;
				 else
					nValidReading = 0;
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

		dRawAvg+=fabs((double)fRawReading);
		dNorAvg+=fabs((double)CtPair.norval);
		dRawRms+=fabs((double)(fRawReading*fRawReading));
		dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));
		if(i==1)
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		}
		SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1645
		//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
		//Modified by:sdt:03092001:1645
		if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
		//Modified by:sdt:03092001:1645
		if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		//statusdisplay(nParamCount);
		//statusdisplay(nParamCount,nFreqNum);//Modified by:sdt:21032001//Commented by:sdt:28092005:1459
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

		nBypass=SelectFixtureCUPS(0);
		if(nBypass)
		{
			 while(nBypass)
			 {
					nBypass=SelectFixtureCUPS(0);
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
	//Commented by:sdt:03092001:1645
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
	//Added by:sdt:03092001:1645
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

	//Added by:sdt:02042001
	farfree(fNorVal);
	farfree(fRawVal);
	return nReadingno;
}

float GetRawReadingCUPS(int nRef)
{
	int         Huntup = 0, Huntlow = 0;
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

			if ( fReading == prmarray->fStartofscale[nCurrentRange] )
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

float normaliseCUPS (float fReading)
{
	char    path[80];
	float   fReturn;//fRoomTemp;
	unsigned int		nCableLength;
	float 	nor_length;

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
	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commeted by sdt:28092005:1735
	//As fRoomTemp is not used in any calculations.
	fReturn=fReading;
	fReturn *=(float)(nor_length/( (nCableLength) ? nCableLength : 1 )); // Substituted nor_length instead 1000
	return fReturn;
}

//#endif //Added by:sdt:21082001