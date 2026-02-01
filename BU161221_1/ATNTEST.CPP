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
//Added by:sdt:02022007
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];

//extern char 	szZchAutoManual[8] ;//Added:MOD11082002:01:jj
//extern int 		ZchHfAray[5][MAXFREQUENCIES];//Added:MOD11082002:01:jj

//Added:MOD29072002:02:jj
extern int 		LfIndex;
extern char 	szLfAtnMode[5];

extern char 		SpeckList[MAXFREQUENCIES][10];
//extern READING far* GetPrevParamVals( READING far *pparamVal, int param, int nFreqNum );
//extern READING far* FreePrevParamVals( READING far *pparamVal, int param );
extern READING far*	fCrPairVal;
extern READING far*	fCmPairVal;


//Added by:sdt:03092001
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);

//Added by:sdt:31072001
//extern float HF_Offset[MAXFREQUENCIES][MAXRANGES];
//extern float HF_CalFact[MAXFREQUENCIES][MAXRANGES];
//extern float HF_RefCalFact[MAXFREQUENCIES];
//Added by:sdt:18062001
//void SetImpedence(int Val);

//Added:jj:09042001
//extern int ZchHfAray[3][MAXFREQUENCIES];
extern int CurrentHfParam;
extern int CurrentFreq;

//Added:jj:06052001
extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];


//Added:jj:05042001
//extern int ZchVal;
//extern int ZchValArray[MAXFREQUENCIES];
//extern getZchVal();

//extern void ReadStdVal(int nParamCount);//Added:jj:26032001
extern char cRamDrive[2];//jj:22012001
extern char bIncOkRej;

// DATA DECLARATION
extern FreqStruct 	HFFrequency,LFFrequency; //Added by:sdt:19032001:1215
extern PARAMINFO	paraminfo[ MAXPARAMETERS+2 ]; //Modified by:sdt:31072001
extern PRM			*prmarray;
extern char     	defaultmessage[];
extern int 			ParamCtsHandle; // File Handle to read PARAM.CTS file.
extern int 			noofparameters;
extern int 			nCurrentGuage; // Serves as the Current Index for Array
extern int 			nNoofSpans;
extern 	ATNVarLenOffs far *AtnVarOff; // Variable dynamically allocated.
extern int			nPrevPair1,nPrevPair2;
extern int      	gflagReplace1,gflagReplace2;
extern int          nLastUnitSize,nUnits,nCount;
extern int          nCurrentRange,nCurrentReadingno;
extern float        fAtnCalRef;
extern float		gfOffset[MAXRANGES];
extern float        fFcal[MAXRANGES];
//extern float       fRefVal[120]; //Commented by:sdt:23072001
//extern float        fRefVal[110][MAXFREQUENCIES];// Added by:sdt:23072001
//Modified by:sdt:16102005:1715 //For 6 Quad cable
extern float          	fRefVal[MAXPAIRS][MAXFREQUENCIES];// Added by:sdt:23072001

//Commented by:sdt:01042001
/*
extern float far    fNorVal[MAX_NO_READINGS];
extern float far    fRawVal[MAX_NO_READINGS];
*/

extern          RdControl       rdCtrl;
//extern          SPECIFICATION    speck;//Commented by:sdt:13112005:1300
extern    int      nTested[MAXPARAMETERS];

extern int      flagReset;

extern int      nParamCount;
extern int      nflagRangeLock;

extern int      nTempUnitOf;
extern int      graphisuptodate;
extern char     szRangeHold[2];
extern int 		factor;

extern struct   time currenttime;
extern          FileStatus fs;

//Commented:jj:18052001
//extern FILE *fpData[MAXPARAMETERS][MAXFREQUENCIES];

//Commented by:sdt:02042001
/*extern unsigned long  far       ALocationSys[1000];
extern float far                ASysOffsetVal[1000];*/

//extern float                    atnPosOff[MAXPAIRS];//Commented:jj:11042001

//  Global structures used in this code file given below
extern INFORMATION      m_FileInfo;
extern CTPAIR           CtPair;
extern CONFIG           nConfig;

//extern void 	ReadInstrStd (float *pfStd, int nRange, int nFreqIndex);
//extern void		SelectSplitForCal( );
//extern int 		SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
//extern float   	GetFixOffsetFromArray (int, int, int, int );
//extern int     	SetParamFreq();
//extern void 	FindAtnRefCalFactor();
//extern void 	InitialiseCALarray(void);

int CalibrateOneParamATN();
int GetParamReadingsATN(int); //modified by:sdt:19032001
//int SelectFixtureATN (int nStart);
float normaliseATN(float fReading);
float GetRawReadingATN (int nRef);
//void FillDispStruct(PRM prmarray,int paramno);//Commented by:sdt:16102005:1840 //As it is no longer used
int GetParamReadingsATNCAL(int nFreqNum) ;
//extern void InitialiseCALarray(void);//14032001
//extern void CheckForDefaultFreq(int nParam,int nParamType);//Added by:sdt:23032001
extern STRUCT_SPECKS speckData;

extern TFrontPage *fPage; //Added by:sdt:27092005:2300

int AtnTest()
{
	//int nRead=1;//Commented by:sdt:07052001
    int nReturnVal =0;
    nParamCount=SRNO_ATN;

    int nFreqNum = 0; //added by:sdt:19032001
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

    lseek ( ParamCtsHandle, sizeof( RdControl ) * nParamCount, SEEK_CUR );
	//nRead = read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );//Commented by:sdt:07052001
    read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
    close( ParamCtsHandle );
	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used

	//Commented by:sdt:19032001
//    if(prmarray->nParamType)
//		m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
//	else
//		m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

    prmarray->max=nConfig.Max[nParamCount];

    InitialiseCALarray(); //Initialise the Cal array

    CtPair.nTest=nParamCount;

    //Commented:jj:08042001
    //graphicsinit();//Added:jj:05042001

     //Commented:jj:06042001
    //CheckForDefaultFreq(nParamCount,prmarray->nParamType); //Added by:sdt:23032001
	//Added by:sdt:02102005:1245
	fPage = (TFrontPage *)(new TFrontPage( "ATN TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}

    for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)  //Added by:sdt:19032001
    {
	fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:02102005:1235
	if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum]) //Added by:sdt:19032001
	   {
		    if(!param_tested[nParamCount][nFreqNum] || gnTest[nParamCount])//Added:jj:06042001
		  {
		  //Below statement added by:sdt:02072001
		  //If Last Unit size different ,below statement will restore the
		  //Original Unit size.
		  itoa (nTempUnitOf ,nConfig.szUnitof ,10);//san 21-11-97 For appending of the remaining pairs
		  nConfig.nHFrequency = nFreqNum; //Added by:sdt:19032001
		  nConfig.nLFrequency = nFreqNum; //Added by:sdt:14072009
		  //Added:jj:09042001
		  //ZchVal=ZchValArray[nFreqNum]; //Commented:MOD11082002:01:jj
		  CurrentHfParam=0;
		  CurrentFreq=nFreqNum;

		  //getZchVal();//Added:jj:05042001

		  //Added by:sdt:19032001
		  if(prmarray->nParamType)
				m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
			else
				m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

			//setupbargraph(nParamCount,nFreqNum);//Commented by:sdt:02102005:1210

		  //Commented:jj:MOD29072002:02

		 // getZchVal();//Added:jj:05042001
		  //Uncommented by:sdt:04102005:1900 //For Quad

			nTested[nParamCount]=TRUE;

			if(strcmp(szLfAtnMode,"CAL")==0 && nFreqNum<LFFrequency.nTotalFrequencies) //Added:MOD29072002:02:jj
			{
				nCurrentRange=0;//Added by:sdt:14072009//As ATN is calculated at LF & there is no ranging of equipment at LF.
				fPage->hint(hcStartTesting);//Added by:sdt:02102005:1245
				nReturnVal=GetParamReadingsATNCAL(nFreqNum);
			}
			else
			{
				//Code for ATN calibration is added here as
				//ATN at LF frequency is calculated parameter
				//and does not requires Calibration.
				SetHighFreq(nConfig.nHFrequency);
				//Post Frequency Delay Added by:sdt:03082001
				delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );

				factor = 100; // for HF parameters 1 % accuracy is to be achived

				SelectReadingLines(0,OFF);//Modified by:sdt:28072001
				// Above line used to select ATN in the VI unit.
				nReturnVal = CalibrateOneParamATN();//Uncommented by:sdt:04102005:1910//Commented by:sdt:31072001
				SetHighFreq(nConfig.nHFrequency);
				//Post Frequency Delay Added by:sdt:03082001
				delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );

			 //Commented by:sdt:02102005:1210
			//ZchVal=ZchValArray[nFreqNum];//Moved:MOD11082002:01:jj
			//if(ZchVal<10 || ZchVal >2550 || !strcmp(szZchAutoManual,"Manual")) //Added:MOD11082002:01:jj
			//	  getZchVal();

			 //GetActualImpedence();  //Added:MOD11082002:02:jj
			 //Added:MOD11082002:01:jj
			 //ZchHfAray[CurrentHfParam][nFreqNum]=ZchVal;
			 //ZchValArray[nFreqNum]=ZchVal;

			SetHighFreq(nConfig.nHFrequency);
			  delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );

				factor = 100; // for HF parameters 1 % accuracy is to be achived
			  SelectReadingLines(0,OFF);
				SetHighFreq(nConfig.nHFrequency);
		   //Post Frequency Delay Added by:sdt:03082001
		    delay( rdCtrl.m_PostFreqDelay[nConfig.nHFrequency] );
			nReturnVal=GetParamReadingsATN(nFreqNum);
			}
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//changed by:sdt:21032001
			}
			fPage->hint(hcStopTesting);//Added by:sdt:02102005:1250
		}
	   if(nReturnVal ==-1)
		break;
	   }
	}
    //SelectReadingLines(nPos,OFF);
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}

int CalibrateOneParamATN()
{
	int      i, nFreqIndex, nRStart, nREnd;
	float    fInstStd;


	//messagewindow("System under calibration");
	//Modified by:sdt:02102005:1210
	fPage->hint(hcCalibrate);

	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);

	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	SelectSplitForCal();

	nRStart = nflagRangeLock==TRUE ? atoi(szRangeHold) - 1 : 0;
	nREnd   = nflagRangeLock==TRUE ? nRStart+1 : prmarray->nRanges;

	//nFreqIndex = SetParamFreq();//Commented by:sdt:19032001
    nFreqIndex = nConfig.nHFrequency; //Added by:sdt:19032001

	for(i=nRStart;i<nREnd;i++)
	{
		ReadInstrStd(&fInstStd, i, nFreqIndex);
	}


	FindAtnRefCalFactor();

	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);

	ResetSystem();

	//messagewindow(defaultmessage);
	//Modified by:sdt:02102005:1210
	fPage->hint(hcCalibrateOver);
	return 0;
}

int GetParamReadingsATN(int nFreqNum)
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1450
	FILE *fpData;//Added:jj:18052001

    //Added:jj:01042001
    float    far *fNorVal;//changed by :sdt:02042001
    float    far *fRawVal;//changed by :sdt:02042001

    //Added:sdt:02042001
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
    unsigned long         ALocationSys;
	float                 ASysOffsetVal;

	int     cKey;
	int     i,nReadingno = 0,nPos=0,nValidReading,nBypass;
	int     nSumCount;
	float   fRawReading,fV1,fV2, fSysOffset, fFixOffset;
	//int     NextTestPair;//19032001
	//int     First = TRUE;//19032001
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;
    double  dNorPowerSum=0.0,dRawPowerSum=0.0;

	unsigned int     nCableLength=atoi(nConfig.szLength),first = TRUE;

	READING InfoReading;
	char sztmp[20];
	struct time prevtime;
	SSysOffsetVal m_SysOffsetVal;
	//Added by:sdt:02022007:2300:start
	float far *OnePerAtnRaw;
	float far *OnePerAtnNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerAtnRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerAtnNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerAtnRaw,0,OnePerCount);
	memset(OnePerAtnNorm,0,OnePerCount);
	//Added by:sdt:02022007:2300:end

    sprintf(sztmp,"%s:\\%dFreq_%d.dat",cRamDrive,nParamCount,nFreqNum); //Added by:sdt:19032001
//	sprintf(sztmp,"%s:\\%d.dat",cRamDrive,nParamCount);
	//remove (sztmp);//Commented temporary by:sdt:02102005:1330

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
    //Added by:sdt:02042001
    fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1450
    //SetImpedence(ZchVal); //commented by:sdt:02102005:1210
    //This function is added for setting impedance while testing.
	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);
	//SelectReadingLines(nPos,ON); //Commented by:sdt:28072001
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


	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );
	//nBypass=SelectFixtureATN(1);
	nBypass=SelectFixtureSinglePair(1);//Added by:sdt:13112005:1515
	if(nBypass)
	{
		 while(nBypass)
		 {
				//nBypass=SelectFixtureATN(0);
				nBypass=SelectFixtureSinglePair(0);//Added by:sdt:13112005:1515
				nConfig.Max[nParamCount]--;
		 }
	}

	delay( rdCtrl.m_anDelay[ DLY_JUNCTION ] );

	//prmarray->max=nConfig.Max[nParamCount];
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamCount][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]=0;
	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);

	//NextTestPair = CtPair.Pair1;//19032001
    nCurrentReadingno=0; //Added by:sdt:20032001
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
				StopPrint (); //Commented Temporary by:sdt:02102005:1215
				break;
		 	default :
				break;
		}
		gettime(&currenttime);
		//showtime(&currenttime);//Commented by:sdt:02102005:1215
		if(bIncOkRej)
			prmarray->ok++;
		nCurrentReadingno++;
		i++;
		nValidReading=0;
		do
		{

			 fV1=GetRawReadingATN(TRUE);
			 fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V1);
			 fV1 -= fFixOffset;
			 fV1*=fAtnCalRef;//Uncommented by:sdt:02102005:1235
			 //Commented by:sdt:02102005:1235
			 //fV1*=HF_RefCalFact[nFreqNum]; //Added by:sdt:31072001
			 fRefVal[CtPair.Pair1][nFreqNum]=fV1;


			fV2=GetRawReadingATN(FALSE); //now
			if (first)
			{
				first = FALSE;
				fV2=GetRawReadingATN(FALSE); //now
			}
			fFixOffset = GetFixOffsetFromArray (nParamCount,CtPair.Pair1,CtPair.Pair2,V2);
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

//			fSysOffset = GetSysOffsetFromArray (nParamCount);
            //getsysoffsetfromarray start



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

            //Commented by:sdt:02042001
			/*for (j = 0; j<1000;j ++)
				 if (ALocationSys [j] == m_SysOffsetVal.lLocation)
				 break;
			fSysOffset=ASysOffsetVal [j];

            //getsysoffsetfromarray end
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

	            //getsysoffsetfromarray end
				fRawReading -= fSysOffset;
            }
		  fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);

			for( int k = 0; k < (nNoofSpans >= MAX_SPANS? MAX_SPANS:nNoofSpans); k++)
			{
				if( ( nCableLength >= AtnVarOff[ k ].nLowerLimit) &&
					( nCableLength <= AtnVarOff[ k ].nUpperLimit) &&
					( nCurrentGuage != -1) )
				{
					fRawReading -= AtnVarOff[k].fOffset[nCurrentGuage];
					break;
				}

			} // End of if statement
			SendSegmentData(CtPair,m_FileInfo);
		    fRawReading=User_ceil(fRawReading, prmarray->Decimal[0]);

            //Commented by:sdt:02042001:because it is giving a
			// problem due to the random value.
			/*if ( nParamCount == SRNO_ATN )
			{
				float fTempAtnOff = atnPosOff[CtPair.Pair1+m_FileInfo.cStartFixPos-2];
				fTempAtnOff = User_ceil(fTempAtnOff, prmarray->Decimal[0]);
				fRawReading -= fTempAtnOff;
			} */
			CtPair.val = fRawReading;//added by:sdt:04102005:2355
			CtPair.norval=normaliseATN(fRawReading);
			CtPair.norval=User_ceil(CtPair.norval,prmarray->Decimal[0]);
			//graphisuptodate=0;//Commented by:sdt:02102005:1215
			//graphicsupdate(nFreqNum);//Modified by:sdt:20032001
		  //graphicsupdate(nFreqNum,fNorVal);//Commented by:sdt:02102005:1215

			//currentdisplay(fRawReading,nParamCount);//Commented by:sdt:02102005:1215
			//nValidReading=CheckHiLoLimits();
			fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:02102005:1250
			nValidReading=CheckHiLoLimits(nFreqNum);//Modified by:sdt:23082001
			switch(nValidReading)
			{
			case  F7 :
			  farfree(fNorVal);
				 farfree(fRawVal);

				 fcloseall();
				 flagReset = TRUE;
				 return -1;
			case  F8:
				 StopPrint ();//Commented Temporary by:sdt:02102005:1215
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
		 dRawAvg+=fabs((double)fRawReading);
		 dNorAvg+=fabs((double)CtPair.norval);

	    //Added:jj:15032001
	    dRawRms+=fabs((double)(fRawReading*fRawReading));
		 dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));

		 if(i==1)
		 {
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		 }
	    SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1455
		 //if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
	    //Modified by:sdt:03092001:1455
	    if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		 //if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
	    //Modified by:sdt:03092001:1455
	    if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		 //statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:02102005:1245
	    //statusdisplay(nParamCount);
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

		//Added by:sdt:02022007:2305:start
		 if(i<OnePerCount+1)
		 {
			OnePerAtnRaw[i-1]=fRawReading;
			OnePerAtnNorm[i-1]=CtPair.norval;
		 }
		 else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220if(i==OnePerCount+1)
		 {
			SortArray(OnePerAtnRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerAtnRaw,OnePerCount);
			SortArray(OnePerAtnNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerAtnNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerAtnRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerAtnNorm,OnePerCount);
		 }
		//Added by:sdt:02022007:2305:end

		//nBypass=SelectFixtureATN(0);
		nBypass=SelectFixtureSinglePair(0);//Added by:sdt:13112005:1515
		if(nBypass)
		{
			while(nBypass)
			{
				//nBypass=SelectFixtureATN(0);
				nBypass=SelectFixtureSinglePair(0);//Added by:sdt:13112005:1515
				nConfig.Max[nParamCount]--;
			}
		}
	}
	//Added by:sdt:02022007:2310:start
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerAtnRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerAtnNorm[OnePerCount-1]);
	farfree(OnePerAtnRaw);
	farfree(OnePerAtnNorm);
	//Added by:sdt:02022007:2310:end


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
    //Commented by:sdt:03092001:1500
    //start
	/*m_FileInfo.STATRAW[nParamCount][nFreqNum].fAvg=(float)fabs(dRawAvg);
	m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fAvg=(float)fabs(dNorAvg);
	m_FileInfo.STATRAW[nParamCount][nFreqNum].fRms=(float)fabs(dRawRms);
	m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fRms=(float)fabs(dNorRms);
	m_FileInfo.STATRAW[nParamCount][nFreqNum].fStddev=(float)fabs(dRawStdDev);
	m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fStddev=(float)fabs(dNorStdDev);
	m_FileInfo.STATRAW[nParamCount][nFreqNum].fPowerSum=dRawPowerSum;
	m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fPowerSum=dNorPowerSum;*/
    //end
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
    //end
    SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	//SelectReadingLines(nPos,OFF);//Commented by:sdt:28072001
    SelectReadingLines(nPos,ON);//Modified by:sdt:28072001

	ResetSystem();

    //Modified:jj:18052001
	//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
    if(fpData){fclose(fpData);}

    param_tested[SRNO_ATN][nFreqNum]=1;//Added:jj:06042001

    farfree(fNorVal);
    farfree(fRawVal);
	return nReadingno;

}

//Commented by:sdt:13112005:1515
/*int SelectFixtureATN (int nStart)
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
		if(!prmarray->nTestingMethod)
		{
			CtPair.Pair2=2;
			nBypass=SetFixture(CtPair.Pair1,CtPair.Pair2,FALSE);
		}
		else
		{
			CtPair.Pair2=0;
			nBypass=SetFixture(CtPair.Pair1,0,FALSE);
		}
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


float GetRawReadingATN (int nRef)
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



	fReading = fReading-gfOffset[nCurrentRange];//Uncommented by:sdt:02102005:1235
	//Commented by:sdt:02102005:1230
    //fReading = fReading-HF_Offset[nConfig.nHFrequency][nCurrentRange];//Modified by:sdt:31072001

	if(!nRef)
	{
		// for digi cm
		fReading *= fFcal[nCurrentRange];//Uncommented by:sdt:02102005:1235
	   //Commented by:sdt:02102005:1230
	   //fReading *= HF_CalFact[nConfig.nHFrequency][nCurrentRange];//Modified by:sdt:31072001

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

float normaliseATN (float fReading)
{
	char 	szTag[50];  //Added by:sdt:04022007:2330
	int 	nMin_length; //Added by:sdt:04022007:2330
	char    path[80];
	float   fRoomTemp,fReturn;/*,fSeq,fLe,fAtnAvg//19032001*/
	unsigned int		nCableLength;
	float 	nor_length;
	//int 	uCuppMmtMethod;//19032001

	//nor_length = atof( speck.speckparams[nParamCount].norlength);
	//Modified by:sdt:02102005:1250
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
	//fRoomTemp=atof(nConfig.szRoomtemp)-20;//Commented by:sdt:13072001
	//Commented by:sdt:02102005:1250
	//if(atof(speck.speckparams[nParamCount].norTemp)!=0)
	//	fRoomTemp=atof(nConfig.szRoomtemp)-atof(speck.speckparams[nParamCount].norTemp);
	//else
	//	fRoomTemp=atof(nConfig.szRoomtemp)-20;
	//Added by:sdt:04022007:2330
	sprintf(szTag,"%s_MIN_LENGTH",paraminfo[nParamCount].szParamAbr);
	nMin_length=GetPrivateProfileInt("MINIMUM_CABLE_LENGTH",szTag,0,path);
	if((nMin_length!=0)&&(nCableLength<nMin_length))
	{
		nCableLength = nMin_length;
	}

	if(speckData.fNormTemp!=0)
		fRoomTemp=atof(nConfig.szRoomtemp)-speckData.fNormTemp;
	else
		fRoomTemp=atof(nConfig.szRoomtemp)-20;

	fReturn=fReading;

	fReturn /=(float)( 1.0 + ( 0.0018 *(fRoomTemp) ));

	fReturn *= (float)(nor_length / ( (nCableLength) ? nCableLength : 1 )); // Substituted nor_length instead 1000

	return fReturn;
}

//Added:MOD29072002:02:jj
int GetParamReadingsATNCAL(int nFreqNum)
{
	//Added:sdt:14072009:Start
	FILE *sys;
	SSysOffsetVal           OffsetValSys;
	unsigned long         ALocationSys;
	float                 ASysOffsetVal,fSysOffset=0.0;
	SSysOffsetVal m_SysOffsetVal;
	//Added:sdt:14072009:End
	unsigned int     nCableLength=atoi(nConfig.szLength);//Added by:sdt:14072009:2345

	SUMMARYINFO SummaryInfo;
	FILE *fpData;
	int nValidReading=0;
	float   far *fNorVal;
	float   far *fRawVal;

	int 	AtnCurrentRdg = 0;
	int     cKey;
	char    path[80];
	int     i,nReadingno = 0;
	int     nSumCount;

	float   fRawReading = 0.0;
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING InfoReading;
	struct  time prevtime;
	char    sztmp[20]; // for %d.Dat File

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

	float far *OnePerAtnRaw;
	float far *OnePerAtnNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerAtnRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerAtnNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerAtnRaw,0,OnePerCount);
	memset(OnePerAtnNorm,0,OnePerCount);
	//Added by:sdt:02022007:2300:end


	//Added by:sdt:04122005:2120:end

	//Commented by:sdt:04122005:2120
	/*p=strtok(freqname,"K");
	if(p==NULL)
    {
		strcpy(freqname,SpeckList[nFreqNum]);
		p=strtok(freqname,"k");
    }
    if(p!=NULL)
	    Curfreq = 1000 * _atold(p);
    //Added by:sdt:02102005:1235 //For lower frequencies
    if(p==NULL)
    {
		strcpy(freqname,SpeckList[nFreqNum]);
		p=strtok(freqname,"HZ");
    }
    if(p==NULL)
    {
		strcpy(freqname,SpeckList[nFreqNum]);
		p=strtok(freqname,"hz");
    }
	if(p!=NULL)
		Curfreq = _atold(p);*/


    fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));

	fCrPairVal = FreePrevParamVals( fCrPairVal, SRNO_CR );
	fCmPairVal = FreePrevParamVals( fCmPairVal, SRNO_CM );
	fCrPairVal = GetPrevParamVals( fCrPairVal, SRNO_CR ,LFFrequency.nDefualtFreq);
	//Added by:sdt:02102005:1240 //For Quad software
	//value of CM at 1Khz is taken
	fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,LFFrequency.nDefualtFreq);
	//Commented by:sdt:02102005:1240 //For Quad Software
    //if(nFreqNum>LFFrequency.nTotalFrequencies-1)
    //{
    //		fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,LfIndex);
    //}
    //else
    //	   fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,nFreqNum);
	AtnCurrentRdg = 0;

    sprintf(path,"%s:\\cts.ini",cRamDrive);
    sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);

	bIncOkRej=1;

    if((fpData = fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		//getch();
		//Modified by:sdt:02102005:1245
		messageBox( "File creation error", mfError|mfOKButton );
		fs = fs_error;
		return 1;
	}
	prmarray->ok=0;
	prmarray->rej=0;
	m_FileInfo.nReadingsOk[nParamCount][nFreqNum]=0;
	m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]=0;

    prmarray->max=nConfig.Max[nParamCount];

	prmarray->pixelstodisplay=0;
	prmarray->pixelsdisplayed=0;
	gettime(&prevtime);

    nCurrentReadingno=0;

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
				StopPrint ();//Commented Temporary by:sdt:02102005:1245
				break;
		 default :
				break;
		 }

	    i++;
	    prmarray->ok++;
		 nCurrentReadingno++;

		 gettime(&currenttime);
		 //showtime(&currenttime);//Commented by:sdt:02102005:1245

	   float temp = (M_PI * Curfreq * fCrPairVal[AtnCurrentRdg].fRawVal*fCmPairVal[AtnCurrentRdg].fRawVal * 1.0e-9 );
	   temp = 8.686 * sqrt(fabs(temp));

	   fRawReading = temp;
	   //Added:sdt:14072009:Start
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

			  //getsysoffsetfromarray end
		fRawReading -= fSysOffset;
	  }
	  //Added:sdt:14072009:End
	  //Added by:sdt:14072009:2345:Start //ATN Lookup Table added for ATN at LF
	  for( int k = 0; k < (nNoofSpans >= MAX_SPANS? MAX_SPANS:nNoofSpans); k++)
	  {
		if( ( nCableLength >= AtnVarOff[ k ].nLowerLimit) &&
			( nCableLength <= AtnVarOff[ k ].nUpperLimit) &&
			( nCurrentGuage != -1) )
		{
			fRawReading -= AtnVarOff[k].fOffset[nCurrentGuage];
			break;
		}

	  }
	  //Added by:sdt:14072009:2345:End
		fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
		CtPair.val = fRawReading;
		CtPair.Pair1 = fCrPairVal[AtnCurrentRdg].cPair1;
		CtPair.Pair2 = fCrPairVal[AtnCurrentRdg].cPair2;
	   if(CtPair.Pair1%atoi(nConfig.szUnitof))
		   CtPair.UnitSrNo=(CtPair.Pair1/atoi(nConfig.szUnitof))+1;
	   else
	    CtPair.UnitSrNo=(CtPair.Pair1/atoi(nConfig.szUnitof));
		CtPair.nTest = SRNO_ATN;

	   //Commented by:sdt:14072009:1305//Added:MOD31072002:01:jj
	   //Now normalised value of ATN is calculated from raw value of ATN
	   //instead of normalised value of CR & CM
	   //temp = (M_PI * Curfreq * fCrPairVal[AtnCurrentRdg].fNormVal*fCmPairVal[AtnCurrentRdg].fNormVal * 1.0e-9 );
	   //temp = 8.686 * sqrt(fabs(temp));

	   //MOD31072002:01:jj
		//CtPair.norval = fRawReading;
	   CtPair.norval = normaliseATN(fRawReading); //14072009//Now Normalised value of ATN calculated from raw ATN value instead of normalised value of CR & CM
	   CtPair.norval = User_ceil(CtPair.norval,prmarray->Decimal[0]); //Modified by:sdt:14072009:1405

		AtnCurrentRdg++;
		delay( 300 );

	   fNorVal[i]=CtPair.norval;
		fRawVal[i]=fRawReading;

	   //graphisuptodate=0;//Commented by:sdt:02102005:1245
	   //graphicsupdate(nFreqNum,fNorVal);//Commented by:sdt:02102005:1245
	   //	currentdisplay(fRawReading,nParamCount);//Commented by:sdt:02102005:1245
	   fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:02102005:1250
	   nValidReading=CheckHiLoLimits(nFreqNum);
			switch(nValidReading)
			{
			case  F7 :
			    farfree(fNorVal);
			    farfree(fRawVal);

				fcloseall();
				 flagReset = TRUE;
				 return -1;
			case  F8:
				 StopPrint ();//Commented Temporary by:sdt:02102005:1245
				 break;
			default :
				 break;
			}

		if(nValidReading==VALID)
			 m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
		else
			 m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;

		dRawAvg+=fabs((double)fRawReading);
		dNorAvg+=fabs((double)CtPair.norval);
		dRawRms+=fabs((double)(fRawReading*fRawReading));
		dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));
		if(i==1)
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		}
	   SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);
	   if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
	   if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		//statusdisplay(nParamCount,nFreqNum);//Commented by:sdt:02102005:1245
		InfoReading.cPair1=CtPair.Pair1;
		InfoReading.cPair2=CtPair.Pair2;
		InfoReading.cParam=nParamCount;
	   InfoReading.nFreq = nFreqNum;
		InfoReading.fRawVal=fRawReading;
		InfoReading.fNormVal=CtPair.norval;
	   InfoReading.nPairAdjacency=-1;
	    InfoReading.nUnitAdjacency=-1;

	   fwrite(&InfoReading,sizeof(InfoReading),1,fpData);

	   //Added by:sdt:02022007:2305:start
	   if(i<OnePerCount+1)
	   {
			OnePerAtnRaw[i-1]=fRawReading;
			OnePerAtnNorm[i-1]=CtPair.norval;
	   }
	   else
	   if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
	   {
			SortArray(OnePerAtnRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerAtnRaw,OnePerCount);
			SortArray(OnePerAtnNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerAtnNorm,OnePerCount);
	   }
	   else
	   {
			InsertNumber(fRawReading,OnePerAtnRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerAtnNorm,OnePerCount);
	   }
	  //Added by:sdt:02022007:2305:end

	}
	//Added by:sdt:02022007:2310:start
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerAtnRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerAtnNorm[OnePerCount-1]);
	farfree(OnePerAtnRaw);
	farfree(OnePerAtnNorm);
	//Added by:sdt:02022007:2310:end

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

    if(fpData){fclose(fpData);}
    param_tested[SRNO_ATN][nFreqNum]=1;

	fCrPairVal = FreePrevParamVals( fCrPairVal,nParamCount );
	fCmPairVal = FreePrevParamVals( fCmPairVal,nParamCount );

    farfree(fNorVal);
    farfree(fRawVal);

	return nReadingno;
}


//#endif //Added by:sdt:21082001