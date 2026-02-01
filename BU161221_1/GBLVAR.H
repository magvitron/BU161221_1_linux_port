#if !defined(__GBLVAR_H)
#define __GBLVAR_H

#include "setup.h"
#include "seq.h"
#include "rdsch.h"
//#include "speck.h"
//Created by:sdt:25082005:2330
//CTS.CPP
int 			CurrentHfParam;
int 			CurrentFreq;
int 			nCurrentGuage=1;
int            MaxFileList;            // max no. of files that can be listed in load option
int            ElfextLength, bSync, delay_cm;
int            nParamCount;
int            absolutemaxx, absolutemaxy, maxx, maxy;
int            quit=0, error = 0;
int 		   ParamCtsHandle; // File Handle to read PARAM.CTS file.
int 		   noofparameters;
int			   nReport;
int            nGiveAlarms;
int            nDC;
int            nTempUnitOf;
int            flagReset=FALSE;
int            nTested[MAXPARAMETERS];
char    	   gMaxPinsReplaced;
float    	   fAtnRefStd[MAXFREQUENCIES];
//int 		   ZchValArray[MAXFREQUENCIES];////Commented by:sdt:16102005:1840 //As it is no longer used
int 		   param_tested[MAXPARAMETERS][MAXFREQUENCIES];
int 		   Pcm_Cable;
int 		   compartsizeinunits;
char 		   cSinglePair1, cSinglePair2;
//int            DisplayArray[MAXPARAMETERS]; //Commented by:sdt:16102005:1835 //To free More memmory
int            gflagReplace1 = FALSE,gflagReplace2 = FALSE;
char     	   szRangeHold[2];
int			   nflagRangeLock=FALSE;
int     	   gnTest[MAXPARAMETERS];
int            nLastUnitSize,nUnits,nNeedOfAppend = FALSE;
int			   nPrevPair1,nPrevPair2;
int 		   nCount;
float		   gfOffset[MAXRANGES];
float          fFcal[MAXRANGES];
int			   nCurrentRange;
int 		   factor;
char		   bIncOkRej;
int     	   nFixtures = 0;
int			   over;
//char 		   szZchMeas[4]="No";
//char		   bZchMeas=FALSE;

int ZchVal=120;
int LfIndex;    		//Added by:sdt:09092005:2300
int HfIndex;			//Added by:sdt:09092005:2300
char SpeckList[MAXFREQUENCIES][10]; //Added by:sdt:09092005:2300
char Text1[2]="N",Text2[2]="N",Text3[2]="N";
#ifndef __BORLAND_DOS_TIME_DEFINED__
#define __BORLAND_DOS_TIME_DEFINED__
#ifndef __DOS_TIME_STRUCT__
#define __DOS_TIME_STRUCT__
struct time {
    unsigned char ti_hour;
    unsigned char ti_min;
    unsigned char ti_sec;
    unsigned char ti_hund;
};
#endif
#endif

struct time currenttime;
int  nNoofSpans; 	// for Substracting Atn Offset.
					// for a Particular cable viz., 0.4, 0.5, or 0.63.
//char 	defaultmessage[] = "F10: Main Menu    F6: Quit    F7: Reset";//Commented by:sdt:13112005:1245

ATNVarLenOffs far *AtnVarOff; // Variable dynamically allocated.

Calibration  	Cal;
RdControl       rdCtrl;
PARAM_DETAILS Param;
FREQ     m_Freq[MAXPARAMETERS];
CTPAIR         CtPair;
REPLACE     	pinReplace[FIXTUREBOTH+1]; //Modified by:sdt:20122014:1520
Pass			Secu;
RMPINIINFO  	RAMPINI;
REPINIINFO  	RepIni;
INFORMATION 	m_FileInfo;
BasicInfo   	m_BasicInfo;
FIXTURE	Fixture[3];
Hz_80_INFO Hz_80_Info;//Added:jj:28012001:1430
FileStatus     fs;

RunSetup runSetup,TempRunSetup;//Added by:sdt:24082005:2355
ReportSetup reportSetup;//Added by:sdt:16102005:1330
FreqStruct HFFrequency,LFFrequency; //Added by:sdt:05082005:2245
char cRamDrive[2];//Added by:sdt:25072005:1955
PRM *prmarray; //Added by:sdt:31072005:2245
PARAMINFO	paraminfo[ MAXPARAMETERS ];//Added by:sdt:31072005:2245//+2 removed by:sdt:30092005:2145 //for Quad
//SPECIFICATION speck;//Commented by:sdt:28092005:1130
Config	config;
CONFIG        nConfig,DefConfig;//,  *pconfig;
//SPECIFICATION *speckptr;//Commented by:sdt:13112005:1255
//CONFIGURATION *pConfig;
//PCTPAIR         pCtPair;//Commented by:sdt:26072005:0041
//RUNPARAM        RunParam;//Commented by:sdt:26072005:0041

//PINS_BYPASS     PinsPos ;//Commented by:sdt:26072005:0041

FILE          *speckfp;
SPECKLIMIT specksLimit; //Added by:sdt:03082005:2005
STRUCT_SPECKS speckData;
//Specks.cpp
//Share.cpp
int bSimulateAdc,nCurrentReadingno;
HFHHFLFREQLINES HfhHflFreqLines;
HFFREQUECYINFO HFfreqInfo;
Zms_Resistance Zms_Res;
//Rsetup.cpp
int iSelRunParam;
int iSelRunFreq;
//cblseq.cpp
char 	szSeqFileName[15];
SEQUENCE CableSequence;
//Frontpg.cpp
TFrontPage *fPage;
READING far		*fCrPairVal=NULL;//Commented by:sdt:23032001
READING far		*fCmPairVal=NULL;//Commented by:sdt:23032001
//WORSTPAIRNEXT Worstpairnext;//commented by:sdt:04022007:1635
char DataDirectory[MAXPATH];
REPSETINGS m_RepSettings;
int 	HFCalibrateFlag = FALSE;
int RuncountExpiryFlag = FALSE; //Added by:sdt:05012006:2010
FILE *fptr_Report,*RepView;
//Added by:sdt:04022007:1620
float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];
int nExit_status=0;//Added by:sdt:17122016:1850
#endif
