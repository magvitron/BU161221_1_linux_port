
#if !defined( __PRNREP_H )
#define __PRNREP_H

#define SUMMARY_LF_FREQ			0		//ADDED by:sdt:23022006:2230 MOD2302200601
#define SUMMARY_HF_FREQ			1       //ADDED by:sdt:23022006:2230 MOD2302200601

#define MAXBYPASSPAIRLEN        25
#define MAXLEN_SELECT_PARAM 29//25 //MOD02022002:01:sdt:1235
#define MAXSUMMARYTYPES		3//2 MOD10072002:JJ
#define MAXLIMITTYPES	    2//5//4 //Modified by:sdt:16102005:2300
#define ADJ					0   //Added by:sdt:10052001
#define NONADJ				1   //Added by:sdt:10052001
#define BOTHADJ				2   //Added by:sdt:10052001
#define ADJUNITS			3  //Added by:sdt:16102005:2300
#define NONADJUNITS			4  //Added by:sdt:16102005:2300
#define BOTH_UNITS			5  //Added by:sdt:16102005:2300
#define ALL_COMBINATIONS 	6 //Added by:sdt:16102005:2300
#define ADJPAIRS_INCABLE 7 //MOD1103200601: sdt:11032006:2255

#define CABLESEQMNULENGHTH 	8   //Added by:sdt:07052001
#define MAXLENCABLESEQ		30  //Added by:sdt:26042001

//sdt:16102005:2300 for quad software.
#define MAXPARAMETERS           11 //16
#define SRNO_RU                 0
#define SRNO_LR                 1
#define SRNO_CR                 2
#define SRNO_CM                 3
#define SRNO_CUPP               4
#define SRNO_CUPG               5
#define SRNO_CUPPI              6
#define SRNO_ZCAL			  7
#define SRNO_ATN                8
#define SRNO_NEXT               9
#define SRNO_FEXT              10

//Commented by:sdt:16102005:2300
//#define SRNO_RU                 0
//#define SRNO_LR                 1
//#define SRNO_CR                 2
//#define SRNO_CM                 3

//#define SRNO_GM                 4 //Added:jj:11042001

//#define SRNO_CUPG               5//4
//#define SRNO_CUPS               6 //Added by:sdt:18052001
//#define SRNO_CUPP               7//6//5
//#define SRNO_CUPPI              8//7  //Added by:sdt:26042001
//#define SRNO_ATN                10//9//7
//#define SRNO_NEXT               11//10//8
//#define SRNO_ELFEXT             14//12//11//9//Commented by:sdt:04082001
//#define SRNO_ZCAL				9//8//6

//Added:jj:04062001
//#define SRNO_INEXT            13
//#define SRNO_FEXT             12//14 //Commented by:sdt:04082001


#define MAX_SPECKTYPE_SZ		6
#define INTRA_LO_UPSIZE			15

#define MEAN_STD_SIZE 			15
#define LENGTHSIZE 				10
#define MAXFREQUENCIES          8 //10 //Modified by:sdt:16102005:2300
#define MAX_CTRL_LINES 			15
#define MAXFIXCOMB              3
#define NOOFFIXTURES            2
#define MAXREADINGNAME_LEN		30

#define ESC                             27
#define MAXWIDTH        250
#define TIP                             0
//sdt:21062001
//StrWidth & Datawidth modified to make this Utility compatible
//with the 80 column and 132 column printers.
#define STRWIDTH        2//2.35 //Modified by:sdt:21062001
#define DATAWIDTH       1.9//2.1   // In Inches ( To be taken in INI later//Modified by:sdt:21062001
#define FALSE                   0
#define TRUE                    1
#define YES                             1
#define NO                              0
#define LEFT                    0
#define CENTER          1
#define RIGHT           2
#define RPI                             6       // Rows Per Inch
///pranav
		 //All width is made same due to report requirement
#define CPI10                   10
#define CPI15                   17
//#define CPI17                   20
#define CPI17                   17

/////////original data
//#define CPI10                   10
//#define CPI15                   15
//#define CPI17                   17
//////////////////////////
//#define MAXLEVELS               3//Commented by:sdt:10042001
#define MAXLEVELS               4 //Increased by 1 to adjust the Impedance in Parameter header.
#define MAXPARTS                5
//Modified by:sdt:04042001:from 11 to 15.
#define MAXCOLCHAR              15 // maximum characters in one column
#define SUM_NEUTRAL             0
#define MIN                     0x01
#define MAX                     0x02
#define AVG                     0x04
#define RMS                     0x08
#define STDDEV                  0x10
#define MEAN_STDDEV             0x20
//#define MAXSUMOPT               0x40
#define MAXSUMOPT               0x80 //Modified 0x40 to 0x80 by:sdt:08052001
#define WPN						0x40 //Added by:sdt:08052001
#define ANYOTHER				0x80 //Added by:sdt:19102005:0025
#define	    SUM_NO_NORMUNNORM		0//Added:pnb:11122000:1250
#define       SUM_UNNORMALISED          1
#define       SUM_NORMALISED            2
#define       SUM_BOTH                  3
#define       MAXCOLUMNS                25
//#define       MAXNOOFPARTS              4
//#define       MAXNOOFLEVELS             4
#define       NOON(t)      ((TIME)t).cHour > 12 ? "P.M" : "A.M"
#define       HALFCLOCK(t) ((TIME)t).cHour > 12 ? 12 : 0
#define       UNNORMSTR(i) (int)i == SUM_NORMALISED ? "L":"Km"
#define       IfSummBothOrNot(i) (int)i == SUM_BOTH ? 2:1
#define       IFMARK(i) (int)i == TRUE ? ' ' : '*'
#define       MAXINFOLEN                25
#define       MAXREADINGNAMESIZE           80

//#define       MAXPARAMETERS             16//cOMMENTED by:sdt:16102005:2300
#define       PARAMETERNAMESIZE         15
#define       PARAMETERLINESIZE         50
#define       PARAMETERUNITSIZE         10
#define       MAXLENCABLETYPE           15
#define       MAXLENGUAGE                       6
#define       AVERAGESIZE                       10
#define       LIMITSIZE                 10
#define       RMSSIZE                           10
#define       STDSIZE                           10
#define       MAXFILENAME               13
#define       MAXSPECKLEN               22
#define       MAXNOOFPAIRSLEN           4
#define       MAXCABLELEN               6
#define       MAXTEMPLEN                6
#define       MAXUNITOFLEN              3
#define   	  MAXLENSPECKSNAME      	22
#define 	  MAXLENSPECKSNUMBER      	4
#define       PARAMEND                  MAXPARAMETERS+1
#define       MAXLENCABLETYPE           15
#define       MAXNOOFPAIRSLEN           4
#define       MAXCABLELEN               6
#define       MAXTEMPLEN                6
#define       MAXUNITOFLEN              3
#define       BLANKVALUE                99999.99
#define       MAXHEADERSIZE             55
#define       NORMALISED                            1
#define       UNNORMALISED                      2
#define       SUMMARY                           10
#define       REMARK                            3
#define       MAXPAIRS                    104 //12 //for 6 Quad cable
#define       SINGLEPAIR                          1
#define       MAXREADINGPOS               20
#define       MAXNAMESIZE                 25
#define       MAXENTRIES                  8
#define       MAXSUMMOPTIONS              5
#define       DUMMYSIZE                   20
#define       MAXPARAMNAME              55
#define       MAXPARAMABRNAME           12
#define       MAXUNITSIZE               12
#define       MAXRANGES                 5
#define       MAXADCCHANNELS            8
#define       MAXREADINGSPERPARAM       1050
#define       SUM_MIN                   0
#define       SUM_MAX                   1
#define       SUM_AVG                   2
#define       SUM_RMS                   3
#define       SUM_STDDEV                4
#define       SUM_DETAILED              0
#define       SUM_SUMMARY               1
#define       MAXBYPASSPAIRS                      10
#define       MAXLINESPERPAIR                     3
#define       IFMARK(i) (int)i == TRUE ? ' ' : '*'
#define REMARKCOLSZ						6
#define PAIRNOCOLSZ						8

#define       MAX_SPECKNAME		  22

//Added by:sdt:07052001
typedef struct SeqNames
{
	char  FileName[13];
    char  SeqName[32];
}SEQNAMES;

// Structure defination begins.
typedef struct tagTime
{
	 char           cHour;
	 char           cMin;
	 char           cSec;
}TIME;

typedef struct tagDate
{
	 char           cDay;
	 char           cMon;
	 int            nYear;
}DATE;

typedef struct tagRepSettings
{
	int nSumm_Detailed;
	int nRemark;
	int nNormUnnorm[MAXPARAMETERS+1];
	int nParams[MAXPARAMETERS+1];
	int nGroupRep;
}REPSETINGS;

typedef struct tagParam // not used
{
	char   cNull;
	char   szParamName[MAXPARAMNAME];
	char   szParamNameAbr[MAXPARAMABRNAME];
	char   szUnit[MAXUNITSIZE];
	int    nParamType;   //(Lf/Hf)
	int    nTesting;
	int    nTestingMethod;  //(pair to pair /single pair)
    int    nPositions;
    int    nReadings;
    char    szReadingNames [MAXREADINGNAMESIZE];
	int    nChannels;
	int    nRanges;
	float  fAccuracy;
	float  fAccuracyfsv;
	int    nInstruStabiTime;
	int    nRangeStabiTime;
	float  fHysthigh;
	float  fHystlow;
	float  fStartofscale[MAXRANGES];
	int    nAdcMin[MAXRANGES];
	float  fEndofscale[MAXRANGES];
	int    nAdcMax[MAXRANGES];
	int    nAdcChannels[MAXADCCHANNELS];
	int    max;
	int    ok;
	int    rej;
	int    pixelstodisplay;
	int    pixelsdisplayed;
}PARAMETER; // not used
// NileshB - 25th Aug., 1999 (Start - MZNG MEM)
// New Array = PARAMINI (old) (INIINFO) + parameterarray (old) (PARAMETER)
// prmarray (PRM)
typedef struct tagCtrlLines
{
  unsigned char nCtrlLine1 : 2;
  unsigned char nCtrlLine2 : 2;
  unsigned char nCtrlLine3 : 2;
  unsigned char nCtrlLine4 : 2;
}CtrlLine;

typedef struct tagPrm
{
// PARAMINI Array
	float       fStdVal[MAXFREQUENCIES][MAXRANGES];
	float       fDeviationVal[MAXFREQUENCIES][MAXRANGES];
	float       fDeviationFs[MAXFREQUENCIES][MAXRANGES];
	int			nCtrlLinesFix[MAX_CTRL_LINES];
	int         nCtrlLinesStdval[MAXRANGES][MAX_CTRL_LINES];
	int         nCtrlLinesOffset[MAXRANGES][MAX_CTRL_LINES];
	int         nCtrlLinesRange[MAXRANGES][MAX_CTRL_LINES];

	CtrlLine	nCurrStatus1[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nPrevStatus1[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nNextStatus1[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nOtherStatus1[MAXFIXCOMB][NOOFFIXTURES];

	CtrlLine	nCurrStatus2[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nPrevStatus2[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nNextStatus2[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nOtherStatus2[MAXFIXCOMB][NOOFFIXTURES];

	CtrlLine	nCurrSpltPair1[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nPrevSpltPair1[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nNextSpltPair1[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nOtherSpltPair1[MAXFIXCOMB][NOOFFIXTURES];

	CtrlLine	nCurrSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nPrevSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nNextSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
	CtrlLine	nOtherSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
	int         nCtrlJunction[MAXFIXCOMB][MAX_CTRL_LINES];
	int         nCtrlLinesReading[MAXREADINGPOS][MAX_CTRL_LINES];
	int         nCtrlLinesCommon[MAX_CTRL_LINES];

// PARAMETER Array
	char   		szParamName[MAXPARAMNAME];
	char   		szParamNameAbr[MAXPARAMABRNAME];
	char   		szUnit[MAXUNITSIZE];
	int    		nParamType;   //(Lf/Hf)
	int    		nTesting;
	int    		nTestingMethod;  //(pair to pair /single pair)
	int     	nPositions;
	int     	nReadings;
	char    	szReadingNames[MAXREADINGNAME_LEN];
	int    		nChannels;
	int    		nRanges;
	float  		fAccuracy;
	float  		fAccuracyfsv;
	int    		nInstruStabiTime;
	int    		nRangeStabiTime;
	float  		fHysthigh;
	float  		fHystlow;
	float  		fStartofscale[MAXRANGES];
	int    		nAdcMin;
	float  		fEndofscale[MAXRANGES];
	int    		nAdcMax;
	int    		nAdcChannels[MAXADCCHANNELS];
	int    		max;
	int    		ok;
	int    		rej;
	int    		pixelstodisplay;
	int    		pixelsdisplayed;
	int 		Decimal[2];
}PRM;
// NileshB - 25th Aug., 1999 (End - MZNG MEM)

typedef struct tabCTPAIR
{
	short   nTest;
	int     Pair1;
	int     Pair2;
	short   TipOrRing;
	short   UnitSrNo;
	float   val;
	float   norval;
}CTPAIR;
/*typedef struct
{
	char    speckname[MAXLENSPECKSNUMBER];
	char    cabletype[MAXLENCABLETYPE];
    char 	type[MAX_SPECKTYPE_SZ]; // PCM='YES' / NORMAL='NO'
	struct
	{
		char    name[PARAMETERNAMESIZE];
		char    lowerlimit[LIMITSIZE];
		char    upperlimit[LIMITSIZE];
		char    averagelow[AVERAGESIZE];
		char    averagehi[AVERAGESIZE];
		char    stdlow[STDSIZE];
		char    stdhi[STDSIZE];
		char    rmslow[RMSSIZE];
		char    rmshi[RMSSIZE];
		char 	norlength[LENGTHSIZE];
        char 	mean_stddev[MEAN_STD_SIZE];
        char 	intralowerupper[INTRA_LO_UPSIZE];
		//char    cDump[200];
	}speckparams[MAXPARAMETERS];
} SPECIFICATION;*/

//Below structure is Added by:sdt:24082001
//This is a limit structure having the all componentes as string &
//used in menu handling
/*typedef struct
{
	char    lowerlimit[LIMITSIZE];
	char    upperlimit[LIMITSIZE];
	char    averagelow[AVERAGESIZE];
	char    averagehi[AVERAGESIZE];
	char    stdlow[STDSIZE];
	char    stdhi[STDSIZE];
	char    rmslow[RMSSIZE];
	char    rmshi[RMSSIZE];

    //Grand power sum MOD05072002:02:jj
	char    psumlow[RMSSIZE];
	char    psumhi[RMSSIZE];

    //1%Worst Pair  MOD10072002:
	char    wpnlow[RMSSIZE];
	char    wpnhi[RMSSIZE];


}SPECKLIMIT;*/
//Below structure is Added by:sdt:16102005:1215
typedef struct
{
	float    lowerlimit;
	float    upperlimit;
	float    averagelow;
	float    averagehi;
	float    stdlow;
	float    stdhi;
	float    rmslow;
	float    rmshi;

	//Grand power sum MOD0507002:01:jj
	float    psumlow;
	float    psumhi;

	//1%Worst Pair Next MOD06072002:01:jj
	float    wpnlow;
	float    wpnhi;
	float	 anyother;

}SPECKLIMIT;


typedef struct
{
	char    speckno[MAXLENSPECKSNUMBER];
	char    speckname[MAXLENSPECKSNAME];
	char    cabletype[MAXLENCABLETYPE];
    char 	type[MAX_SPECKTYPE_SZ]; // PCM='YES' / NORMAL='NO'
    //char  	szCableSequence[MAXLENCABLESEQ];//Added by:sdt:26042001
    SEQNAMES szCableSequence;//Addedby:sdt:07052001
	struct
	{
		char    name[PARAMETERNAMESIZE];
        //Comemnted by:sdt:24082001
        /*struct
        {
			char    lowerlimit[LIMITSIZE];
			char    upperlimit[LIMITSIZE];
			char    averagelow[AVERAGESIZE];
			char    averagehi[AVERAGESIZE];
			char    stdlow[STDSIZE];
			char    stdhi[STDSIZE];
			char    rmslow[RMSSIZE];
			char    rmshi[RMSSIZE];
        }specklimits[4];//Modified by:sdt:26042001
        */
		char 	norlength[LENGTHSIZE];
        char 	norTemp[LENGTHSIZE]; //added by:sdt:13072001
        //Commented by:sdt:26042001
        //char 	mean_stddev[MEAN_STD_SIZE];
        //char 	intralowerupper[INTRA_LO_UPSIZE];

        //Added:jj:18032001
        char freq[10][4];

		//char    cDump[200];
	}speckparams[MAXPARAMETERS];
} SPECIFICATION;

/*
typedef struct
{
	char    specksnumber[MAXLENSPECKSNUMBER+1];
	char    cabletype[MAXLENCABLETYPE+1];
	char    guage[MAXLENGUAGE+1];
    char 	type; // PCM=1 / Normal=0
	struct
	{
        float   lowerlimit;
		float   upperlimit;
		float   averagelow;
		float   averagehi;
		float   stdlow;
		float   stdhi;
		float   rmslow;
		float   rmshi;
        float   intralower;
        float   intraupper;
	}speckparams[MAXPARAMETERS];
}STRUCT_SPECKS;*/

//Below structure Added by:sdt:24082001
//This is a limit structure having the all componentes as float &
//used in Calculation & Limit checks.
typedef struct
{
	float   lowerlimit;
	float   upperlimit;
	float   averagelow;
	float   averagehi;
	float   stdlow;
	float   stdhi;
	float   rmslow;
	float   rmshi;

    //Grand power sum MOD05072002:02:jj
	float   psumlow;
	float   psumhi;

    //1%Worst Combination  MOD10072002:jj
	float   wpnlow;
	float   wpnhi;
	float	 anyother;

}CONFIGLIMIT;


typedef struct
{
	int	nSpeckFileNumber;
	char    specksnumber[MAXLENSPECKSNAME+1];
	char    cabletype[MAXLENCABLETYPE+1];
	char    guage[MAXLENGUAGE+1];
    char 	type; // PCM=1 / Normal=0
	//char  szCableSequence[MAXLENCABLESEQ];
	SEQNAMES szCableSequence;
	float		fNormTemp;	//Added by:sdt:08102005:2345
	//float		fNormLength; //Added by:sdt:08102005:2345

	struct
	{
       //Commented by:sdt:24082001
       //start
       /* struct
        {
	        float   lowerlimit;
			float   upperlimit;
			float   averagelow;
			float   averagehi;
			float   stdlow;
			float   stdhi;
			float   rmslow;
			float   rmshi;
        }specklimits[4];//Modified into structure by:sdt:26042001
       */
       //end
		//commented by:sdt:26042001
		//float   intralower;
		//float   intraupper;

		//Added:jj:18032001
		int freq[MAXFREQUENCIES];
		//Added by :sdt:21032001
		int nNumofFreqSelected;
		float		fNormLength; //Moved here by:sdt:31012007:2110

	}speckparams[MAXPARAMETERS];
}STRUCT_SPECKS;

typedef struct
{
	char           szFileName[MAXFILENAME];
	STRUCT_SPECKS  Specks;
	char           szSpeckName[MAXSPECKLEN];
	char           szDrumNo[MAXINFOLEN];
	char           szTestedBy[MAXINFOLEN];
	char           szNoofpairs[MAXNOOFPAIRSLEN];
	char           szUnitof[MAXUNITOFLEN];
	char           szLength[MAXCABLELEN];
	char		   szIlm[8];//Added by:sdt:09102005:1320
	char		   szFlm[8];//Added by:sdt:09102005:1320
	char           szRoomtemp[MAXTEMPLEN];
	char           szFixPos[4];
	char           szFixtures[8];
	char           szLFreq[10];
	char           szHFreq[10];
	int            nLFrequency;
	int            nHFrequency;
	int            Max[MAXPARAMETERS];

      //Modifications for Break & Short starts here
      //jj:22012001:1655
     char 		cBreak;
	char 		cShort;
     unsigned char	BSResult;

}CONFIG;

typedef struct tagDummy
{
	 char  szHeader[120];
	 int   nEnd;
}Dummy;


typedef struct tagUserEntry
{
	 char           szTitle[MAXNAMESIZE];
	 char           szEntry[MAXNAMESIZE];
}USERENTRY;


typedef struct tagSummary
{
    char  cMinPair1;
    char  cMinPair2;
    char  cMinTiporRing;
    char  cMaxPair1;
    char  cMaxPair2;
    char  cMaxTiporRing;
	float fMin;
    float fMax;
    float fAvg;
	float fRms;
	float fStddev;
    float fPowerSum;
}SUMINFO;

/*typedef struct tagInformation
{
	char                            szHeader[MAXHEADERSIZE];
	char                            cDummyEnd;
	char                            cVersion;
	unsigned  int           		  nFileNumber;
	DATE                            FileOpenDate;
	TIME                            FileOpenTime;
	char                            szCableType[MAXINFOLEN];
	char                            szSpecificationNo[MAXINFOLEN];
	float                           fCableGuage;
	unsigned char           		cNoofPairs;
	unsigned char           		cUnitof;
	unsigned int            		nLength;
	float                           fRoomtemp;
	SUMINFO                         STATRAW[MAXPARAMETERS];
	SUMINFO                         STATNORMALISED[MAXPARAMETERS];
	int                             nReadingsOk[MAXPARAMETERS];
	int                             nReadingsNotOk[MAXPARAMETERS];
	int                             nAppend;                //san For appending the remaining pairs
	unsigned char           		  cFrequency[MAXPARAMETERS];
	char                            cParamTested[MAXPARAMETERS];
	char                            cFixtureNo;
	char                            cStartFixPos;
	char                            cPairsBypassed[MAXBYPASSPAIRS];
	char           					nSpeckId[MAX_SPECKNAME];
	TIME                            StartTime;
	TIME                            StopTime;
	TIME                            SaveTime;
	DATE                            SaveDate;
	char                            szDrumNo[MAXINFOLEN];
	char                            szTestedBy[MAXNAMESIZE];
	USERENTRY                       ENTRY[MAXENTRIES];
	char                            szModel[MAXINFOLEN];
}INFORMATION;*/

//Changes done by:sdt:03042001
//fields used for maxparameters are made as double dimensional
typedef struct tagInformation
{
	char                            szHeader[MAXHEADERSIZE];
	char                            cDummyEnd;
	char                            cVersion;
	unsigned  int           		nFileNumber;
	DATE                            FileOpenDate;
	TIME                            FileOpenTime;
	char                            szCableType[MAXINFOLEN];
	char                            szSpecificationNo[MAXINFOLEN];
	float                           fCableGuage;
	unsigned char           		cNoofPairs;
	unsigned char           		cUnitof;
	unsigned int            		nLength;
	unsigned int            		nIlm;//Added by:sdt:09102005:1325
	unsigned int            		nFlm;//Added by:sdt:09102005:1325
	float                           fRoomtemp;
    //Commented by:sdt:05092001:0920
	//SUMINFO                         STATRAW[MAXPARAMETERS][MAXFREQUENCIES];
	//SUMINFO                         STATNORMALISED[MAXPARAMETERS][MAXFREQUENCIES];
	int                             nReadingsOk[MAXPARAMETERS][MAXFREQUENCIES];
	int                             nReadingsNotOk[MAXPARAMETERS][MAXFREQUENCIES];
	int                             nAppend;                //san For appending the remaining pairs
	unsigned char           		cFrequency[MAXPARAMETERS];
	char                            cParamTested[MAXPARAMETERS][MAXFREQUENCIES];
	char                            cFixtureNo;
	char                            cStartFixPos;
	char                            cPairsBypassed[MAXBYPASSPAIRS];
	char           					nSpeckId[MAX_SPECKNAME];
	TIME                            StartTime;
	TIME                            StopTime;
	TIME                            SaveTime;
	DATE                            SaveDate;
	char                            szDrumNo[MAXINFOLEN];
	char                            szTestedBy[MAXNAMESIZE];
	USERENTRY                       ENTRY[MAXENTRIES];
	char                            szModel[MAXINFOLEN];
    //Added by:sdt:26042001
	int 		   					nPairAdjacency[MAXPARAMETERS]; //0 - Adjacent  1 - NonAdjacent -1 = not selected.
	int 		   					nUnitAdjacency[MAXPARAMETERS]; //0 - Adjacent  1 - NonAdjacent -1 = not selected.
    //Added by:sdt:09052001
    int 							nMaxAdjNonAdj[MAXPARAMETERS];
	int 							nMinAdjNonAdj[MAXPARAMETERS];


}INFORMATION;
//Added by:sdt:09092001:1250
//This structure used to store Summary values for single parameter at
//a time.
typedef struct //tagSummaryInfo
{
	SUMINFO     STATRAW;
	SUMINFO    	STATNORMALISED;
}SUMMARYINFO;


typedef struct tagReading
{
	unsigned char  cPair1;
	unsigned char  cPair2;
	unsigned char  cParam;
    unsigned int   nFreq; //Added by:sdt:23032001
	float          fRawVal;
	float          fNormVal;
    int 		   nPairAdjacency; //0 - Adjacent  1 - NonAdjacent -1 = not selected.
	int 		   nUnitAdjacency; //0 - Adjacent  1 - NonAdjacent -1 = not selected.

}READING;


typedef struct tagPage
{
		float   fleft;
		float   fright;
		float   ftop;
		float   fbottom;
		float   fInchHeight; //Height in Inches
		float   fInchWidth;
		int     nRowsPP; // Rows Possible Per Page
		int     nCurrentPg; // Current Page Number
		int     nTotalPg; // Total Number of Pages
		int     nCurrentln; // Current line
		char    bHeader;
		char    bFooter;
		char    bHBorder; //Whether Header Border
		char    bFBorder; //Whether Footer Border
		char    bHAllPg; //Whether Header on All Pages
		char    bFAllPg;
		char    bHAllignment; // Header Allignment
		char    bFAllignment;
		char    szHStr  [MAXWIDTH];
		char    szFStr  [MAXWIDTH];
		char    szHBStr [MAXWIDTH]; //Header Border String
		char    szFBStr [MAXWIDTH]; //Footer Border String

}Page;

typedef struct tagParamHeader
{
	int nTChars; //Total characters
	int nCpi;       // Characters per Inch
	int nNoofLevels;
	struct
	{
		int nNoofParts;
		struct
		{
			int nTChars;    //Width of each parts in characters
			int nCpi;
			char szPHeader[MAXHEADERSIZE]; // Header of each part
		}Parts[MAXPARTS];
	}Levels[MAXLEVELS];
}ParamHeader;

//Function declaration begins
/***************************************************************************/
int PrinterSetup(void);//Added by:sdt:21062001
int SelectPrinterMode(void);//Added by:sdt:08042001
void FileOpen (void);
void PageSetup(char *);
void Margins(void);
void PageSize(char *);
void HeaderFooter(void);
void PageStart(void);
int  LineFeed (FILE *fp,int nTimes);
int  LineFeed1 (FILE *fp,int nTimes);

//06052001
//void WriteStringToFile (FILE *fp,char *str);
void WriteStringToFile (FILE *fp,char *str ,int endcount);
//void PrepareString (float Start,float End,char *scr,char *dest,int cpi,int nAllignment);
void PrepareString (float Start,float End,char *scr,char *dest,int cpi,int nAllignment, int &endcount);

void ClearBuffer (char *cBuf,int nlimit);
void SkipTopMargin (void);
void SkipBottomMargin (void);
void MemSet (int *nArray,int nSize,int nValue);
void ReadBasicData (void);
void DisplaySetup (void);
void GroupParams (int *nArray);
void IndividualParams (int *nGrArray ,int *IndArray);
char *RdTypeHeader(char nPNo,char nSumType);
void FillParamHeader(char nPNo,char nLevel,int cpi);
float AreainChar (int nPno);
float RepeatSet (int nSetStart,int nSetEnd,int *Array,int *col,
					int nCharAvail,int nCharReqr);
void RowComplete (int nSStart,int nSEnd,int *row,int *col,int *Array,int nTChar,int nCharAvail);
void DisplayMatrix (int *nGrParams);
void DispDetailedReport(void);
//int TReadings(int nPno);
int TReadings(int nPno,int nFreqNo);//Added by:sdt:03042001
//void DispParamHeader (int *array,int cpi);
//Modified by:sdt:17102005:0045
void DispParamHeader (int *array,int *FreArray, int *nNormArray, int cpi);
READING * GetData (int nPno);
void DispGroupRep (void);
int CheckParamType(int nPno);
void FileClose (void);
void ReportEnd (void);
int FinishRow (int col,int row,float fe,float fs,char *cbuf,int bNotOk);
void DispRepHeader (void);
void DispSummaryRep (void);
void PrepareHeaderData (char szData[20][MAXHEADERSIZE]);
void PrepareHeaderStrings (char szString[20][MAXHEADERSIZE]);
//READING *OneGrReading(int nPno,int nCount,int bGroupParam);
//READING *OneGrReading(int nPno,int nCount,long *ParaFilePos,int nFreq);//Added by:sdt:03042001
//MOD17012002:02:sdt::1525
READING *OneGrReading(int nPno,int nCount,unsigned long *ParaFilePos,int nFreq);
void RepeatChar (char ch,int times,char *s);
//void DispSumHeader (int nPno,float fColWidth);
//void DispSumHeader (int nPno,float fColWidthint,int nFreqNum);//Added by:sdt:030402001
//Modified by:sdt:09092001:1255
void DispSumHeader (int nPno,float fColWidthint,int nFreqNum,int nSwitchMode);

void ParamBorder (float fs,float fWidth,char ch,int cpi);
//int WithinSpeck (int nParam,float fTmp,int nAdjacency);
int WithinSpeck (int nParam,int nFreqNum,float fTmp,int nAdjacency);//Modified by:sdt:30082001
void SelectShrink (char OnOff);
void SelectUnderline (char OnOff);
void SelectSuperscript (char OnOff);
//void SelectDoublewidth (char OnOff);
void SelectDoublewidth( int index, char *cbuf, char OnOff );
void SelectCondensed (char OnOff);
void SelectDraft (void);
//void SelectNlq (char OnOff);
void SelectNlq( float fs, int cpi, char *cbuf, char OnOff );
void SetPageLength ();
float SpeckValue(char *string);
int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szDefault,
				char *szStr,char *FileName,int nDefault);
int GetPrivateProfileInt (char *szAppName,char *szkeyname,int nDefault,char *FileName);
/***************************************************************************/

///////////////pranav

//typedef unsigned int	WORD;
//typedef unsigned int	BOOL;
typedef unsigned char	BYTE;

#define MAX_CONDUCTORSPERPAIR  2
#define OK    0
#define NOTOK 1

#define TT_BREAK                0
#define TT_SHORT                1
#define TT_ALL                  2
#define TT_NONE                 0xFF

void SelectElite(char OnOff);


///////////////
//structure is added by:sdt:03042001.
//this structure is added as smaller option for PRM structure.
typedef struct Rep_tagprm
{
	char   		szParamName[MAXPARAMNAME];
	char   		szParamNameAbr[MAXPARAMABRNAME];
	char   		szUnit[MAXUNITSIZE];
	int    		nParamType;   //(Lf/Hf)
	int    		nTestingMethod;  //(pair to pair /single pair)
	int     	nReadings;
	char    	szReadingNames[MAXREADINGNAME_LEN];
	int 		Decimal[2];
}REPORT_PRM;

//Added:jj:02052001
typedef struct ParamDetails
{
	int Param_Start;
    int Param_End;
    int Lf_Param_Start;
    int Lf_Param_End;
    int Hf_Param_Start;
	int Hf_Param_End;
}PARAM_DETAILS;

//Commented by:sdt:04022007:1610
//Added by:sdt:06062001
/*typedef struct Worstpairnext
{
	float Ru[MAXFREQUENCIES][2];
	float Lr[MAXFREQUENCIES][2];
	float Cr[MAXFREQUENCIES][2];
	float Cm[MAXFREQUENCIES][2];
	float Cupg[MAXFREQUENCIES][2];
	float Cupp[MAXFREQUENCIES][2];
	float Cuppi[MAXFREQUENCIES][2];
	float Zcal[MAXFREQUENCIES][2];
	float Atn[MAXFREQUENCIES][2];
	float Elfext[MAXFREQUENCIES][2];
	float Onext[MAXFREQUENCIES][2];
	float Fext[MAXFREQUENCIES][2];
	float Inext[MAXFREQUENCIES][2];
}WORSTPAIRNEXT;*/

typedef struct Ipsum
{
	double RawVal;
    double NorVal;
}IndPowSum;


//enum Boolean { False, True };
#ifndef BOOL
//typedef unsigned int BOOL;
#endif

#ifndef WORD
//typedef unsigned int WORD;
#endif

struct RunSetup
{
	unsigned long ulParamRunSetup[MAXPARAMETERS];
	WORD	wShortBreak;
	WORD	wFixture;
	Boolean bSingleStep;
	Boolean bZchMeas;
	char szBypassPairs[MAXBYPASSPAIRLEN];
	Boolean bAlarmsOff; //Added by:sdt:18122014:28042015:2100
	//WORD mode;      // 1st BYTE HV, 2nd BYTE IR 3rd BYTE BnS
	//WORD tested;    // 1st BYTE HV, 2nd BYTE IR 3rd BYTE BnS
};

//Added by:sdt:23022006:2230 //MOD2302200601
typedef struct hffrequency
{
	int HFLstart;
    int HFLend;
    int HFHstart;
    int HFHend;
}HFFREQUECYINFO;

#endif
