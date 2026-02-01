//#ifdef EXECUTE //Added by:sdt:21082001
//Modifications
//19062001 : 0950
//1. GetParamReading() function modified : freq of 1 khz was used in formula
//	 for calculation of zch.Variable Curfreq stores the current freq value in hz.

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

//Added by:sdt:03022007:2215
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];

//Added by:sdt:03092001
//SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
//void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);

//Commented:jj:19062001
//#define ZCH_FRQ_1KHZ		1000.0

//Added:jj:10042001
extern int 		LfIndex;
//extern char 	szHfZchMode[5];//Commented by:sdt:16102005:1840 //As it is no longer used
//extern char 	szLfZchMode[5];//Commented by:sdt:16102005:1840 //As it is no longer used

//Added:jj:06052001
extern int gnTest[MAXPARAMETERS];
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];
//Commented by:sdt:01102005:2200
//extern PARAMINFO	paraminfo1[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
//extern int ZchValArray[MAXFREQUENCIES];//Commented by:sdt:16102005:1840 //As it is no longer used

//extern READING far* GetPrevParamVals( READING far *pparamVal, int param, int nFreqNum );
//extern READING far* FreePrevParamVals( READING far *pparamVal, int param );
extern READING far*	fCrPairVal;
extern READING far*	fCmPairVal;

extern char 		cRamDrive[2];
extern int 			cr_tested;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
extern PRM			*prmarray;
extern int 			ParamCtsHandle; // File Handle to read PARAM.CTS file.
extern int 			noofparameters;

extern int          gnTest[MAXPARAMETERS];
extern int          nCurrentRange,nCurrentReadingno;
extern int         	nCount;
extern char 		bIncOkRej;

//Commented by:sdt:01042001 
/*extern float far    fNorVal[MAX_NO_READINGS];
extern float far    fRawVal[MAX_NO_READINGS];*/
//extern          	SPECIFICATION    speck;
extern  int        	nTested[MAXPARAMETERS];
extern int      	flagReset;
extern int 			nParamCount;
extern int      	nGiveAlarms;
extern int      	nflagRangeLock;
extern int      	graphisuptodate;
extern char     	szRangeHold[2];
extern char     	szFreq[10];
extern char     	szRunSelectParam[MAXLEN_SELECT_PARAM];
extern char     	defaultmessage[];

extern struct   	time currenttime;
extern              FileStatus fs;

//Modified:jj:18052001
//extern FILE 		*fpData[MAXPARAMETERS][MAXFREQUENCIES];

extern INFORMATION   m_FileInfo;
extern CTPAIR        CtPair;
extern CONFIG        nConfig;
extern char 		SpeckList[MAXFREQUENCIES][10];
extern FreqStruct 	HFFrequency,LFFrequency;

int 	GetParamReadingsZCAL(int);
//void 	FillDispStruct(PRM prmarray,int paramno);//Commented by:sdt:16102005:1840 //As it is no longer used
//extern void CheckForDefaultFreq(int nParam,int nParamType);

extern TFrontPage *fPage; //Added by:sdt:27092005:2300

int  ZCalTest()
{
//    memset( prmarray, 0, sizeof( prmarray ) );
    //int nRead=1;
    int nFreqNum = 0;
    nCount=0;

    int nReturnVal=0;
    nParamCount=SRNO_ZCAL;

	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
    {
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}

	ReadParamDotCts( paraminfo[nParamCount].szParamAbr, prmarray );
	//FillDispStruct(prmarray,nParamCount);//Commented by:sdt:16102005:1840 //As it is no longer used

    close( ParamCtsHandle );

	prmarray->max=nConfig.Max[nParamCount];
    CtPair.nTest=nParamCount;

    //Commented:jj:08042001
    //graphicsinit();//Added:jj:05042001


	//CheckForDefaultFreq(nParamCount,prmarray->nParamType);//Commented:jj:05042001
    /*for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
    {//Modified for LF :jj:05042001*/

    //Modified:for Zch Cal at HF freq:jj:10042001
	//int freqlimit;//Commented by:sdt:16102005:1840 //As it is no longer used
	//Commented by:sdt:01102005:2130 //ZCH is a calculated parameter for
	//Quad software
	/*if(strcmp(szHfZchMode,"CAL")==0)
	 freqlimit=HFFrequency.nTotalFrequencies;
	else
	 freqlimit=LFFrequency.nTotalFrequencies;*/
	//freqlimit=LFFrequency.nTotalFrequencies;//Commented by:sdt:16102005:1840 //As it is no longer used

	//int freqstart;//Commented by:sdt:16102005:1840 //As it is no longer used
	//if(strcmp(szLfZchMode,"CAL")==0)
	//	 freqstart=0;
	//else
	//	 freqstart=4;

	//Added by:sdt:28092005:2358
	fPage = (TFrontPage *)(new TFrontPage( "ZCH TEST" ));
	if(fPage != 0)
	{
		//sprintf( Rf_File_Data.m_TestDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon+1, tlocal->tm_year+1900 );
		//sprintf( Rf_File_Data.m_StartTime, "%02d:%02d", tlocal->tm_hour, tlocal->tm_min );
		//::fPageCreated = True;
		TProgram::deskTop->insert( fPage );
	}

	for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
    {
		fPage->updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);//Added by:sdt:29092005:0000
		if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum] )
        {
		    if(!param_tested[nParamCount][nFreqNum] || gnTest[nParamCount])//Added:jj:06042001
            {
            nConfig.nLFrequency = nFreqNum;

            //Below condition commented by:sdt:17072001
            // Because Zch calculated is to calculate Zch at Low frequency
			// using values of low frequency parameter.

			/*if(prmarray->nParamType)
				m_FileInfo.cFrequency[nParamCount]=nConfig.nHFrequency;
			else*/
			m_FileInfo.cFrequency[nParamCount]=nConfig.nLFrequency;

			//setupbargraph(nParamCount,nFreqNum);//Commented by:sdt:28092005:1150

			/*for(int j=0;j<MAX_NO_READINGS;j++)
			{
				fNorVal[j]=(float)BLANKVALUE;
				fRawVal[j]=(float)BLANKVALUE;
			} */

			SetLowFreq(nConfig.nLFrequency);
			CtPair.nTest=nParamCount;
			nTested[nParamCount]=TRUE;

			//messagewindow(defaultmessage);//Added:jj:08042001//Commented by:sdt:28092005:1150
			fPage->hint(hcStartTesting);//Added by:sdt:29092005:0000
			nReturnVal=GetParamReadingsZCAL(nFreqNum);
			m_FileInfo.cParamTested[nParamCount][nFreqNum]=TRUE;
			//Condtion Added by:sdt:28042015:2100
			if(runSetup.bAlarmsOff==False)
			{
				GiveAlarms(nFreqNum);//Changed by:sdt:21032001
			}
			fPage->hint(hcStopTesting);//Added by:sdt:29092005:0000
        }
        }
    	if(nReturnVal ==-1)
        	break;
	}
	TView::destroy(fPage->tp);
	TWindow::destroy(fPage);

	return nReturnVal;
}

int GetParamReadingsZCAL(int nFreqNum)
{
    SUMMARYINFO SummaryInfo;//Added by:sdt:03092001:1655
	FILE *fpData;//Added:jj:18052001
    int nValidReading=0;
	//Commented:sdt:01042001
    //modified by :sdt:02042001
	float   far *fNorVal;//[MAX_NO_READINGS];
 	float   far *fRawVal;//[MAX_NO_READINGS];

	int 	zchCurrentRdg = 0;
    int     cKey;
	char    path[80];//22012001
	int     i,nReadingno = 0;
	int     nSumCount;

	float   fRawReading = 0.0;
	double  dRawAvg=0.0,dRawRms=0.0,dRawStdDev=0.0;
	double  dNorAvg=0.0,dNorRms=0.0,dNorStdDev=0.0;

	READING InfoReading;
	struct  time prevtime;

	char    sztmp[20]; // for %d.Dat File
	//Start:Added by:sdt:03022007:2215
	float far *OnePerZcalRaw;
	float far *OnePerZcalNorm;
	int OnePerCount;
	OnePerCount=ceil(float(nConfig.Max[nParamCount])/100);
	OnePerZcalRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	OnePerZcalNorm = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerZcalRaw,0,OnePerCount);
	memset(OnePerZcalNorm,0,OnePerCount);
	//End:Added by:sdt:03022007:2215


	//Addded:Start:jj:19062001
	double Curfreq;
	char freqname[10], tfreqname[10];
	char *p;
	strcpy(freqname,SpeckList[nFreqNum]);
	p=strtok(freqname,"HZ");
	//Condition modified by:sdt:27112005:1500
	strcpy(tfreqname, freqname);
	p=strtok(freqname,"K");
	if(!(strcmp(freqname, tfreqname)))
	{
		Curfreq = _atold(p);
	}
	else
		Curfreq = 1000 * _atold(p);

	//Commented by:sdt:27112005:1500
	//if(Curfreq==0)
	//{
	//	p=strtok(freqname,"K");
	//	if(p!=NULL)
	//		Curfreq = 1000 * _atold(p);
	//}
	//if((p!=NULL)&&(!strcmp(freqname,SpeckList[nFreqNum])))
	//{
	//	p=strtok(freqname,"HZ");
	//	if(p!=NULL)
	//		Curfreq = _atold(p);
	//}
   //	else if(p!=NULL)
	//	Curfreq = 1000 * _atold(p);
	//if(p==NULL)
	//{
	//	strcpy(freqname,SpeckList[nFreqNum]);
	//	p=strtok(freqname,"HZ");
	//}
	//if(p==NULL)
	//{
	//	strcpy(freqname,SpeckList[nFreqNum]);
	//	p=strtok(freqname,"hz");
	//}
	//if(p!=NULL)
	//	Curfreq = _atold(p);
	//End :jj:19062001


	//Added by:sdt:02042001
    fNorVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    fRawVal=(float *)farmalloc(sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fNorVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(fRawVal,0,sizeof(float)*(nConfig.Max[nParamCount]+1));
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:03092001:1655

   	fCrPairVal = FreePrevParamVals( fCrPairVal, SRNO_CR );
	fCmPairVal = FreePrevParamVals( fCmPairVal, SRNO_CM );
	fCrPairVal = GetPrevParamVals( fCrPairVal, SRNO_CR ,LFFrequency.nDefualtFreq);
	//Added by:sdt:01102005:2130 //For Quad software
	//value of CM at 1Khz is taken
	fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,LFFrequency.nDefualtFreq);
    //Commented by:sdt:01102005:2130 //For Quad Software
    /*if(!strcmp(szHfZchMode,"CAL") && nFreqNum>LFFrequency.nTotalFrequencies-1)//Modified:jj:03052001
    {
		fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,LfIndex);
    }
    else
		fCmPairVal = GetPrevParamVals( fCmPairVal, SRNO_CM ,nFreqNum);*/
	zchCurrentRdg = 0;

	sprintf(path,"%s:\\cts.ini",cRamDrive);
	sprintf(sztmp,"%s:\\%dFREQ_%d.dat",cRamDrive,nParamCount,nFreqNum);

	bIncOkRej=1;

    //Modified:jj:18052001
	//if((fpData[nParamCount][nFreqNum] = fopen(sztmp,"wb"))==NULL)
    if((fpData = fopen(sztmp,"wb"))==NULL)
	{
		//messagewindow("File creation error");
		//getch();
		messageBox("File creation error",mfError|mfOKButton);
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
                 //Added by:sdt:02042001
			    farfree(fNorVal);
			    farfree(fRawVal);

				flagReset = TRUE;
				return -1;
		 case  F8:
				StopPrint ();//commented Temporary by:sdt:28092005:2350
				break;
		 default :
				break;
		 }

         //Added:jj:25032001
         i++;
		 prmarray->ok++;
		 nCurrentReadingno++;

		 gettime(&currenttime);
		 //showtime(&currenttime);//commented by:sdt:28092005:2350

		//Modified:jj:19062001
		//float temp = (2.0 * M_PI * ZCH_FRQ_1KHZ * fCmPairVal[zchCurrentRdg].fRawVal * 1.0e-9 );
		float temp = (2.0 * M_PI * Curfreq * fCmPairVal[zchCurrentRdg].fRawVal * 1.0e-9 );
		temp = 1.0 / (temp ? temp : 1 );
		fRawReading = sqrt(fabs( fCrPairVal[zchCurrentRdg].fRawVal * temp ) ) -40.0; // sdx for polycab daman
		fRawReading = User_ceil(fRawReading,prmarray->Decimal[0]);
		CtPair.val = fRawReading;
		CtPair.Pair1 = fCrPairVal[zchCurrentRdg].cPair1;
		CtPair.Pair2 = fCrPairVal[zchCurrentRdg].cPair2;
		if(CtPair.Pair1%atoi(nConfig.szUnitof))
			CtPair.UnitSrNo=(CtPair.Pair1/atoi(nConfig.szUnitof))+1;//Added by:sdt:23042001
		else
			CtPair.UnitSrNo=(CtPair.Pair1/atoi(nConfig.szUnitof));
		CtPair.nTest = SRNO_ZCAL;
		CtPair.norval = fRawReading;
		zchCurrentRdg++;
		delay( 300 );

		//Added by:sdt:02042001
		fNorVal[i]=CtPair.norval; // LF Params CM,CUPG,GM
		fRawVal[i]=fRawReading; // LF Params CM,CUPG,GM


		//Added:jj:25032001
		//graphisuptodate=0;//commented by:sdt:28092005:2350
		//graphicsupdate(nFreqNum);
		//graphicsupdate(nFreqNum,fNorVal);//commented by:sdt:28092005:2350
		//currentdisplay(fRawReading,nParamCount);//commented by:sdt:28092005:2350
		//Added by:sdt:23082001
		//Start
		fPage->updateFrontPage(PASS,UPDATE_RESULT, nFreqNum);//Added by:sdt:29092005:0002
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
				 StopPrint ();//commented Temporary by:sdt:28092005:2350
				 break;
			default :
				 break;
			}

		if(nValidReading==VALID)
			 m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;
		else
			 m_FileInfo.nReadingsNotOk[nParamCount][nFreqNum]++;
        //End
        //m_FileInfo.nReadingsOk[nParamCount][nFreqNum]++;//Commented by:sdt:23082001

        //Commented by:sdt:04022001
        //fNorVal[i]=CtPair.norval; // LF Params CM,CUPG,GM
		//fRawVal[i]=fRawReading; // LF Params CM,CUPG,GM

		dRawAvg+=fabs((double)fRawReading);
		dNorAvg+=fabs((double)CtPair.norval);
		dRawRms+=fabs((double)(fRawReading*fRawReading));
		dNorRms+=fabs((double)(CtPair.norval*CtPair.norval));
		if(i==1)
		{
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		}
        SummaryInfo=ReadSummary(nParamCount,nFreqNum,0);//Added by:sdt:03092001:1700
		//if(fabs((double)CtPair.norval)<fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMin))
        //Modified by:sdt:03092001:1700
        if(fabs((double)CtPair.norval)<fabs((double)SummaryInfo.STATNORMALISED.fMin))
			 StoreMinVal(fRawReading,nParamCount,nFreqNum,0);
		//if(fabs((double)CtPair.norval)>fabs((double)m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fMax))
        //Modified by:sdt:03092001:1700
        if(fabs((double)CtPair.norval)>fabs((double)SummaryInfo.STATNORMALISED.fMax))
			 StoreMaxVal(fRawReading,nParamCount,nFreqNum,0);
		//statusdisplay(nParamCount,nFreqNum);//commented by:sdt:28092005:2350
		InfoReading.cPair1=CtPair.Pair1;
		InfoReading.cPair2=CtPair.Pair2;
		InfoReading.cParam=nParamCount;
		InfoReading.nFreq = nFreqNum; //Added by:sdt:31032001
		InfoReading.fRawVal=fRawReading;
		InfoReading.fNormVal=CtPair.norval;
		InfoReading.nPairAdjacency=-1; //Added by:sdt:21052001
		InfoReading.nUnitAdjacency=-1;//Added by:sdt:21052001

		//Modified:jj:18052001
		//fwrite(&InfoReading,sizeof(InfoReading),1,fpData[nParamCount][nFreqNum]);
		fwrite(&InfoReading,sizeof(InfoReading),1,fpData);

		//Start:Added by:sdt:03022007:2220
		//Worst pair next calculations
		 if(i<OnePerCount+1)
         {
			OnePerZcalRaw[i-1]=fRawReading;
			OnePerZcalNorm[i-1]=CtPair.norval;
         }
         else
		 if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
         {
			SortArray(OnePerZcalRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerZcalRaw,OnePerCount);
			SortArray(OnePerZcalNorm,OnePerCount);
			InsertNumber(CtPair.norval,OnePerZcalNorm,OnePerCount);
		 }
		 else
		 {
			InsertNumber(fRawReading,OnePerZcalRaw,OnePerCount);
			InsertNumber(CtPair.norval,OnePerZcalNorm,OnePerCount);
		 }
		 //End:Added by:sdt:03022007:2220

	}
	//Start:Added by:sdt:03022007:2210
	Worstpairnext[nParamCount][nFreqNum][0]=(float)fabs(OnePerZcalRaw[OnePerCount-1]);
	Worstpairnext[nParamCount][nFreqNum][1]=(float)fabs(OnePerZcalNorm[OnePerCount-1]);
	farfree(OnePerZcalRaw);
	farfree(OnePerZcalNorm);
	//End:Added by:sdt:03022007:2210


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
	//Commented by:sdt:03092001:1700
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
	//Added by:sdt:03092001:1700
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

	//Added:jj:05042001//Modified by:sdt:04092001:1010
	//ZchValArray[nFreqNum]=int(m_FileInfo.STATRAW[nParamCount][nFreqNum].fAvg);
	//ZchValArray[nFreqNum]=int(SummaryInfo.STATRAW.fAvg);//Commented by:sdt:16102005:1840 //As it is no longer used

	ResetSystem();

	//Modified:jj:18052001
	//if(fpData[nParamCount][nFreqNum]){fclose(fpData[nParamCount][nFreqNum]);}
	if(fpData){fclose(fpData);}

	param_tested[SRNO_ZCAL][nFreqNum]=1;//Added:jj:06042001

	fCrPairVal = FreePrevParamVals( fCrPairVal,nParamCount );
	fCmPairVal = FreePrevParamVals( fCmPairVal,nParamCount );
	//Added by:sdt:02042001
	farfree(fNorVal);
	farfree(fRawVal);

	return nReadingno;
}

//#endif //Commented by:sdt:21082001