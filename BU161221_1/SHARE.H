#if !defined( __SHARE_H )
#define __SHARE_H

//Added by:sdt:27092005:1800
#define TIMER_START     0

#define INNER_DATALINE_IDX 		0
#define OUTER_DATALINE_IDX 		4
#define INNER_PAIR1LATCH_IDX 	3
#define INNER_PAIR2LATCH_IDX 	0
#define OUTER_PAIR1LATCH_IDX 	6
#define OUTER_PAIR2LATCH_IDX 	9
#define PAIR1					0
#define PAIR2					1

//Commented by:sdt:24072005:1000
//#define SECURITY //Added by:sdt:17092001

//#define MAXSUMMARYTYPES		2	//Added by:sdt:29082001;//
#define MAXSUMMARYTYPES		3	//MOD08072002


//type 1-Adjacent pair/unit type 2- NonAdjacent pair/unit.
#define MAX_DIRECT_LINES	1000 //Added by:sdt:14082001
//Added :sdt:19072001
#define OPEN_CIRCUIT 0
#define SHORT_CIRCUIT 1//0 //Modified by :sdt:28072001
//Added:jj:11042001
#define X_OFFSET	2
#define Y_OFFSET	5//3
#define YPOS_PARAM  8

/*#define OFFSET_PARAM	 		0
#define OFFSET_FREQ	 			OFFSET_PARAM  + 6//15
#define OFFSET_RANGE	 		OFFSET_FREQ + 8//8
#define OFFSET_STANDARD	 		OFFSET_RANGE + 10
#define OFFSET_MEASURED			OFFSET_STANDARD + 10
#define OFFSET_DEVIATION		OFFSET_MEASURED+ 10
#define OFFSET_DEVIATION_MIN	OFFSET_DEVIATION+ 10
#define OFFSET_DEVIATION_MAX	OFFSET_DEVIATION_MIN+ 10
#define OFFSET_CALFACTOR		OFFSET_DEVIATION_MAX+10
#define OFFSET_REMARK		 	OFFSET_CALFACTOR+ 10*/



#define 	  PRACTMAXNUMOFPAIRS  101  //Added by :sdt:14032001.
//Added:jj:24012001
#define CL_OUTER                39
#define CL_INNER                0
#define BOTH                    4

#define     DEBUG
#undef		DEBUG

#include <ctype.h>
#define       NEPEAR              8.686
#define       LOW                 0x00
#define       HIGH                0xFF
#define       ON                  1
#define       OFF                 0
#define       MAX_PORTLINES       8
#define       MAX_LINESPERIC      24
#define       MAX_PORTSINIC       3
#define       MAX_DIO_ICS         6
#define       MAX_FRC_PINS        50
#define       MAXFRCLINES         150
#define       MAX_MUX_PINS        720
#define       ALLPORTSOUT         0x80
// For digital ramp
#define       MODE0IO         	  0x88
#define       MODE0CIN            0x89 //
#define 	  MODE0CLIN			  0x81 //Added by:sdt:20062001
#define       WR_HIGH             0xFF
#define       WR_LOW              0x7F
#define       PORT_A              0
#define       PORT_B              1
#define       PORT_C              2
#define       PORT_CWR            3
#define       FRC_PORT2           0
#define       FRC_PORT3           1
#define 	  FRC_PORT4           2
#define 	  MAXLEN_SELECT_PARAM 29//25
#define       DIOBASE             0xE20
#define       CONTROL_PORT        DIOBASE + 12
#define       DATA_PORT           DIOBASE + 8

#define       IC0_PORT_A          DIOBASE + 0
#define       IC0_PORT_B          DIOBASE + 1
#define       IC0_PORT_C          DIOBASE + 2
#define       IC0_PORT_CWR        DIOBASE + 3
#define       IC1_PORT_A          DIOBASE + 4
#define       IC1_PORT_B          DIOBASE + 5
#define       IC1_PORT_C          DIOBASE + 6
#define       IC1_PORT_CWR        DIOBASE + 7
#define       IC2_PORT_A          DIOBASE + 8
#define       IC2_PORT_B          DIOBASE + 9
#define       IC2_PORT_C          DIOBASE + 10
#define       IC2_PORT_CWR        DIOBASE + 11
#define       IC3_PORT_A          DIOBASE + 12
#define       IC3_PORT_B          DIOBASE + 13
#define       IC3_PORT_C          DIOBASE + 14
#define       IC3_PORT_CWR        DIOBASE + 15
#define       IC4_PORT_A          DIOBASE + 16
#define       IC4_PORT_B          DIOBASE + 17
#define       IC4_PORT_C          DIOBASE + 18
#define       IC4_PORT_CWR        DIOBASE + 19
#define       IC5_PORT_A          DIOBASE + 20
#define       IC5_PORT_B          DIOBASE + 21
#define       IC5_PORT_C          DIOBASE + 22
#define       IC5_PORT_CWR        DIOBASE + 23

#define       MUX_DATA_PORT       DIOBASE + 8
#define       MUX_CTRL_PORT       DIOBASE + 12
// sizeof one segment in bytes (approx)
#define       SZ_SEG              64000L
#define       ADCBASE             0x210

#define 	  PT_AVG			  10 // 10 pt averaging
// the time (in micro sec) until which averaging readings are to be taken
#define       AVG_READTIMER       40000L
#define       ADC_READDELAY       100
#define       ADC_PORTLOW         ADCBASE + 4
#define       ADC_PORTHIGH        ADCBASE + 5
#define       ADC_PORTCHANNEL     ADCBASE + 10
#define       ADC_PORTTRIGGER     ADCBASE + 12
#define       ADC_CHANNEL1        0
#define       ADC_CHANNEL2        1
#define       ADC_CHANNEL3        2
#define       ADC_CHANNEL4        3
#define       ADC_CHANNEL5        4
#define       ADC_GNDCHANNELLF    ADC_CHANNEL2
#define       ADC_HFREFCHANNEL    ADC_CHANNEL4
#define       ADC_CHANNEL6        5
#define       ADC_GNDCHANNELHF    ADC_CHANNEL6
#define       ADC_CHANNEL7        6
#define       ADC_CHANNEL8        7
#define       ADC_EOC             15
#define       ADC_TRIGGER         10
#define       ADC_TIMEOUT         50
#define       ADC_NOINPUT         4096
#define 	  ADC_LIMIT			  4054
#define       ADC0CORRESPONDINGGND 2048
#define 	  ADC_READBAND		  5 	// in percentage for removing
												// of spikes
#define       LF_PARAMETER        0
#define       HF_PARAMETER        1

#define       MAX_MUX_ICS         30
#define       MAX_MUX_PORTS       90
#define       MAX_FRCPINS         50
#define       MAX_PORTLINESPERIC  24
#define       MAX_PORTLINES       8
#define       MAX_PORTS           90

#define       SOURCE              0x04
#define       DETECTOR            0x02
#define       GROUND              0x01
#define       MAXPAIRS            104 //Modified for Pair Software
#define       FIXTURE1            0
#define       FIXTURE2            1
#define       FIXTUREBOTH         2
#define       MAXBYPASSPAIRS      10
//#define     MAXLINESPERPAIR     3
#define       MAXLINESPERPAIR     2
#define       MAXFIXCOMB          3
//pr
#define       NOOFFIXTURES        2

//#define       CALLOW              0.7//Commented :jj:31012001
//#define       CALHIGH             1.3

#define       MAXALLOWEDHUNT      2
#define       MAXAVERAGEREADINGS  700

#define       SINGLEPAIR          1
#define       PAIRCOMBINATION     0

//#define       MAXFREQUENCIES      10 //Commented by:sdt:01102005:2145 //As it is already defined in CT.h

#define 	  MAXRANGES           5
#define       HF150KZPINNO        108
#define       HF1024KZPINNO       109
#define       HF552KZPINNO        133
#define       HF772KZPINNO        134
#define       LF1KZPINNO          34
#define       LF800HZPINNO        33

#define       MAXREADINGPOS       20
#define       MAXNAMESIZE         25
#define       MAXENTRIES          8
#define       SPLITTERALLON       0xFF
#define       MAXSUMMOPTIONS      5
#define       DUMMYSIZE           20
#define       INVALID             0
#define       OVERRIDE            1
#define       VALID               2
#define       MAXHEADERLEN        136
#define       MAXREPHEADLINE      3
#define       MAXREPLACELEN       4 //104 //Updated by:sdt:30112014:1610 //Max 4 Pins can be repalced

// This is Used for Subtracting the Fixture Offset from V1 and V2

#define       BLOCKSIZE       	  10
#define       INTERBLOCK          1
#define       INTRABLOCK          0
#define       V1                  0
#define       V2                  1

#define 	  CURRENT         	  0
#define 	  PREV                1
#define 	  NEXT				  2
#define 	  OTHERS			  3

#define 	  POS_LEFT            0
#define 	  POS_CENTER          1
#define 	  POS_RIGHT           2
//#define 	  MAXPRINTCOLS        105//Commented by:sdt:28092005:1130//As it is declared in "rep.h"

#define       MAX_SPECKNAME		  22


enum FileStatus {       fs_new, fs_open, fs_inprocess, fs_executed, fs_bad,
					fs_error, fs_closed, fs_saved, fs_notsaved };

/*
typedef struct tagRepHeader
{
	 char szLin[MAXREPHEADLINE][MAXHEADERLEN];
}REPHEADER;
*/

//amit 1/2/98
/*
typedef struct tagCalInfo
{
	 float          fDeviationVal[MAXFREQUENCIES][MAXRANGES];
	 float          fDeviationFs[MAXFREQUENCIES][MAXRANGES];
	 float          fStdVal[MAXFREQUENCIES][MAXRANGES];
}CALINFO;
*/
//pranav
typedef struct tagCtrlLines
{
  unsigned char nCtrlLine1 : 2;
  unsigned char nCtrlLine2 : 2;
  unsigned char nCtrlLine3 : 2;
  unsigned char nCtrlLine4 : 2;
}CtrlLine;

// NileshB - 25th Aug., 1999 (Start - MZNG MEM)
// New Array = PARAMINI (old) (INIINFO) + parameterarray (old) (PARAMETER)
// prmarray (PRM)
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

    //Commented by:sdt:26042001
    //Added:jj:17042001
    //int 		UnitSwitching; //Intraunit/Interunit
    //int 		PairSwitching; //Adjacent/Nonadjacent/Both

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

typedef struct tagCalibration
{
	CtrlLine Cl[MAXFIXCOMB][NOOFFIXTURES];
}Calibration;

////////

typedef struct tagIniInfo
{
	 float          fStdVal[MAXFREQUENCIES][MAXRANGES];
	 float          fDeviationVal[MAXFREQUENCIES][MAXRANGES];
	 float          fDeviationFs[MAXFREQUENCIES][MAXRANGES];
	 //float          fSysOffsetRead[MAXRANGES];
	 //float          fSysOffsetUnit[MAXRANGES];
///////////pranav
	 CtrlLine			nCurrStatus1[MAXFIXCOMB][NOOFFIXTURES];
// Nilesh - 15th Aug. 1999 - Start (Commented)
//	 CtrlLine			nPrevStatus1[MAXFIXCOMB][NOOFFIXTURES];
//	 CtrlLine			nNextStatus1[MAXFIXCOMB][NOOFFIXTURES];
// Nilesh - 15th Aug. 1999 - End (Commented)
	 CtrlLine			nCurrStatus2[MAXFIXCOMB][NOOFFIXTURES];
// Nilesh - 15th Aug. 1999 - Start (Commented)
//	 CtrlLine			nPrevStatus2[MAXFIXCOMB][NOOFFIXTURES];
//	 CtrlLine			nNextStatus2[MAXFIXCOMB][NOOFFIXTURES];
// Nilesh - 15th Aug. 1999 - End (Commented)
	 CtrlLine			nOtherStatus1[MAXFIXCOMB][NOOFFIXTURES];
	 CtrlLine			nOtherStatus2[MAXFIXCOMB][NOOFFIXTURES];

	 CtrlLine			nCurrSpltPair1[MAXFIXCOMB][NOOFFIXTURES];
// Nilesh - 16th Aug. 1999 - Start (Commented)
/*
	 CtrlLine			nPrevSpltPair1[MAXFIXCOMB][NOOFFIXTURES];
	 CtrlLine			nNextSpltPair1[MAXFIXCOMB][NOOFFIXTURES];
*/
// Nilesh - 16th Aug. 1999 - End (Commented)
	 CtrlLine			nOtherSpltPair1[MAXFIXCOMB][NOOFFIXTURES];

	 CtrlLine			nCurrSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
// Nilesh - 16th Aug. 1999 - start (Commented)
/*
	 CtrlLine			nPrevSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
	 CtrlLine			nNextSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
*/
// Nilesh - 16th Aug. 1999 - End (Commented)
	 CtrlLine			nOtherSpltPair2[MAXFIXCOMB][NOOFFIXTURES];
////////////////////
	 //modification by:sdt:28072001
     //size of Array is reduced from MAX_FRC_PINS to MAX_CTRL_LINES
     //now these arrays size is as per size in PRM structure.
	 int            nCtrlLinesCommon[MAX_CTRL_LINES];//MAX_FRC_PINS];
	 int            nCtrlLinesOffset[MAXRANGES][MAX_CTRL_LINES];//MAX_FRC_PINS];
	 int            nCtrlLinesStdval[MAXRANGES][MAX_CTRL_LINES];//MAX_FRC_PINS];
	 int            nCtrlLinesReading[MAXREADINGPOS][MAX_CTRL_LINES];//MAX_FRC_PINS];
	 int            nCtrlJunction[MAXFIXCOMB][MAX_CTRL_LINES];//MAX_FRC_PINS];
	 int            nCtrlLinesRange[MAXRANGES][MAX_CTRL_LINES];//MAX_FRC_PINS];
	 int            nCtrlLinesFix[MAX_CTRL_LINES];//MAX_FRC_PINS];
	 //int            nSummOptions[MAXSUMMOPTIONS];
	 //int            nAverageof;
	 //REPHEADER      RepNorm;
	 //REPHEADER      RepUnnorm;
	 //REPHEADER      RepBoth;
}INIINFO;

typedef struct tagRampIniInfo
{
	 float          fStdVal[MAXRANGES];
	 float          fSysOffsetRead[MAXRANGES];
	 float          fSysOffsetUnit[MAXRANGES];
	 float          fDeviationVal[MAXRANGES];
	 float          fDeviationFs[MAXRANGES];

     //modification by:sdt:28072001
     //size of Array is reduced from MAX_FRC_PINS to MAX_CTRL_LINES
     //now these arrays size is as per size in PRM structure.
	 int            nCtrlLinesCommon[MAX_CTRL_LINES];//MAX_FRC_PINS];//40];//Modified by:sdt:29062001
	 int            nCtrlLinesOffset[MAXRANGES][MAX_CTRL_LINES];//MAX_FRC_PINS];//40];//Modified by:sdt:29062001
	 int            nCtrlLinesStdval[MAXRANGES][MAX_CTRL_LINES];//MAX_FRC_PINS];//40];//Modified by:sdt:29062001
	 int            nCtrlLinesReading[MAXREADINGPOS][MAX_CTRL_LINES];//MAX_FRC_PINS];//40];//Modified by:sdt:29062001
	 int            nCtrlLinesRange[MAXRANGES][MAX_CTRL_LINES];//MAX_FRC_PINS];//40];//Modified by:sdt:29062001
	 int            nAverageof;
	 unsigned int            nLength; //SDT11122016:0830
}RMPINIINFO;

typedef struct tagReplace
{
	 char nPinsReplace[MAXREPLACELEN];//MAXPAIRS //Updated by:sdt:30112014:1610
	 char nPinsReplacewith[MAXREPLACELEN];//MAXPAIRS //Updated by:sdt:30112014:1610
}REPLACE;

typedef struct tagRepIni
{
	 int            nTopMargin;
	 int            nBottomMargin;
	 int            nPaperInches;
	 int            nPrinterTime;
}REPINIINFO;

typedef struct tagCHIPINFO
{
	 int            PortNumber;
	 int            MaskNumber;
}CHIPINFO;

typedef  struct tagFreq
{
	 int            nFreq;
}FREQ;

typedef struct tagAverageReading
{
	 int            nReadVal;
	 int            nChannel;
}AverageList;

typedef struct tagUserEntry
{
	 char           szTitle[MAXNAMESIZE];
	 char           szEntry[MAXNAMESIZE];
}USERENTRY;

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

//coomented by:sdt:21032001
/*typedef struct tagInformation
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
	float                           fRoomtemp;
	SUMINFO                         STATRAW[MAXPARAMETERS];
	SUMINFO                         STATNORMALISED[MAXPARAMETERS];
	int                             nReadingsOk[MAXPARAMETERS];
	int                             nReadingsNotOk[MAXPARAMETERS];
	int                             nAppend;                //san For appending the remaining pairs
	unsigned char           		cFrequency[MAXPARAMETERS];
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

//Changes done by:sdt:21032001
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
    //Commented by:sdt:02092001
	//SUMINFO  	                    STATRAW[MAXPARAMETERS][MAXFREQUENCIES];
	//SUMINFO  	                  	STATNORMALISED[MAXPARAMETERS][MAXFREQUENCIES];
	int 	                        nReadingsOk[MAXPARAMETERS][MAXFREQUENCIES];
	int     	                    nReadingsNotOk[MAXPARAMETERS][MAXFREQUENCIES];
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

//Added by:sdt:02092001
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

struct SysOffsetLocation
{
	unsigned char FixNo:2;
	unsigned char Freq :4;//3; //MOD18012002:03:sdt::1235
	unsigned char Range:3;
	unsigned char ParamAbr:4;
	unsigned char Method:3;
	unsigned char nPos:4;
};

struct  SSysOffsetVal
{
	float fSysOffsetVal;
	union
	{
		SysOffsetLocation m_SysOffsetLocation;
		unsigned long lLocation;
	};
    int Dummy;//MOD18012002:03:sdt::1235
	//This dummay variable is added to mentain structure compatability with
	//the "Systh" utility.
};

struct FixOffsetLocation
{
   unsigned char ParameterAbr : 5;
   unsigned char Freq : 4;//3; //MOD18012002:03:sdt::1235
   unsigned char Block : 3;
   unsigned char Voltage : 2;
   unsigned char Range : 3;
};

struct SFixOffsetVal
{
   union
   {
	  FixOffsetLocation m_FixOffsetLocation;
	  unsigned int nLocation;
   };
   float fFixOffsetVal;
   int Dummy;//MOD18012002:03:sdt::1235
   //This dummay variable is added to mentain structure compatability with
   //the "fixth" utility.
};
// structure used for storing the values used for ploting the scope
typedef struct tagPoint
{
	float x;
	float y;
}Point;

typedef struct tagDebug
{
	float	fInstOff;
	float	fReadOff;
	float	fSysOff;
	float 	fRead;
	float	fCal;
	int		nRange;
	int		nAdc;
}Debug;



#define			MAX_ARRAY_INDEX 3 // for CR - CR,RU,LR hence max. 3 considered
#define			CR_PARAM_INDEX  2
#define			LR_PARAM_INDEX  1
#define			RU_PARAM_INDEX  0
#define 		LF_OPARAM_INDEX	0
#define			HF_PARAM_INDEX  0
#define 		MAX_NO_READINGS	2310 // 104 Pairs 1036 (last Unit 24 Pairs )


#define RU_FORMULA_1		1
#define RU_FORMULA_2		2
#define RU_FORMULA_3		3
#define RU_FORMULA_4		4

#define CUPP_SQRT_MTH 		1
#define CUPP_LINEAR_MTH 	2
#define CUPP_AVG_MTH 		3



typedef struct tag80HzINFO
{
	 int InstruDelay;
	 int JunctionDelay;
	 int RelayDelay;
	 int PreReadDelay;
	 int RangeDelay;
}Hz_80_INFO;

typedef struct DisplayStruct
{
	int 		pixelstodisplay;
	int 		pixelsdisplayed;
	char   		szUnit[MAXUNITSIZE];
	int 		Decimal[2];

}DispStruct;

//Structures added by:sdt:18032001

typedef struct Statusoffreqline
{
	int far * nFreqLines;
}STATUSOFFREQLINE;



typedef struct FrequencyStruct
{
	int nTotalFreqLines ;
	int nTotalFrequencies ;
	int nDefualtFreq;
	int far * nFreqLines;
	STATUSOFFREQLINE far * nStatusofFreqLines;
}FreqStruct;

//Added:jj:05042001
typedef struct StatusofZmsLines
{
	int far * ZmsLines;
}STATUSOFZMSLINES;

typedef struct ZMS_Sequence_Struct
{
	STATUSOFZMSLINES far *SeqPattern;
    int AutorangeArm; //Added by:sdt:19072001
    int *SelectedPositions;//Added by:sdt:19072001
    int nTotalPositions;
}Zms_Sequence;

typedef struct Zms_Res_Struct
{
	int far *Lines;
}Zms_Resistance;

//Added by:sdt:22042001
typedef struct PairSwitch
{
	int CUPP;
    int CUPPI;
    int ELFEXT;
    int ONEXT;

    //Added:jj:04062001
    int FEXT;
    int INEXT;

}PAIRSWITCH;

//Added by:sdt:22042001

#define ADJPAIRS		0
#define NONADJPAIRS		1
#define BOTH_PAIRS		2
#define ADJUNITS		3
#define NONADJUNITS		4
#define BOTH_UNITS		5
#define ALL_COMBINATIONS 6 //Added by:sdt:01102005:1510
#define ADJPAIRS_INCABLE 7 //MOD2802200601: sdt:28022006:2205


typedef struct ParamDetails
{
	int Param_Start;
    int Param_End;
    int Lf_Param_Start;
	int Lf_Param_End;
    int Hf_Param_Start;
    int Hf_Param_End;
}PARAM_DETAILS;

//Commented by:sdt:04022007:1620
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


//Added by:sdt:17072001
typedef struct hffrequency
{
	int HFLstart;
    int HFLend;
    int HFHstart;
    int HFHend;
}HFFREQUECYINFO;

//Added by:sdt:18072001
typedef struct AutoRangeCurrent
{
	int nCurrentRangeNo;
    int nCurrentVal,nRangeResistorVal,nRangeVoltageVal;
    int nVoltageRange;
};

typedef struct CalibratedImpedabceValues
{
	//float *IdealValue;
    float *ActualValue;
}CALIBRATEDIMPEDANCEVALUES;

typedef struct HfhHflfreqline
{
	int VIunit;
	int HFunit;
}HFHHFLFREQLINES;

//Added by:sdt:16082001
//This will store the number of position where reading is to be taken
//and Autorange Arm number.
typedef struct Zch_meas_sett
{
	int far *Position_Order;
    int Autorangearm;
}ZCH_MEAS_SETTING;


void SetImpedence(int Val);
void GetActualImpedence(); //Added:MOD11082002:02:jj

void	Set_DIOFRC_Pin(int,int);
int     Get_DIOFRC_Pin(int);
//inline void  SetDMuxPin(int,int);
void  	SetDMuxPin(int,int);
void  	SetFrequency(int nDefault=TRUE);
void  	SetSelectFreq(int nFreq);//amit 1/2/98
void  	SetLowFreq(int nFreq);//amit 1/2/98
void  	SetHighFreq(int nFreq);//amit 1/2/98
void  	OnFrequency();
float 	GetAverageReading(int,int);
int   	AreCompatible (float ,float,int);
int   	SetFixture (int,int,int);
void  	ResetSystem();
void  	InitializeDIOcard(void);
int   	InitializeADCcard(void);
int   	GetParamReadingsLF();
int   	GetParamReadingsHF(int *);
float 	GetRawReading(int);
int   	currentdisplay(float reading, int parameternumber);
float 	normalise(float fReading,int nParam);
int   	SelectFixture(int nStart);
void  	FillParamInfo(void);
void  	SwitchCommonLines(int);
void  	SwitchJunctionLines(int);
void  	SelectRange(int,int);
void  	SelectReadingLines(int,int);
void  	SwitchOffsetLines(int,int);
void  	SwitchStdvalLines(int,int);
//int     CheckHiLoLimits(float fRu = 0.0, int bRu = 0);
int     CheckHiLoLimits(int nFreqNum,float fRu = 0.0, int bRu = 0);//Modified by:sdt:23082001
void  	GiveAlarms(int);
void  	SetInfo(void);
void  	WriteInfo(void);
//void  	StoreMinVal(float,int,int);//Changed by:sdt:21032001//Commented by:sdt:04092001
//void  	StoreMaxVal(float,int,int);//Changed by:sdt:21032001//Commented by:sdt:04092001
void  	StoreMinVal(float,int,int,int);//Changed by:sdt:04092001:1025
void  	StoreMaxVal(float,int,int,int);//Changed by:sdt:04092001:1025

void  	ExchangeRampInfo(int);
void  	FindAutoCal(void);
void 	FindAtnRefCalFactor(void);
//amit 1/2/98
int  	CalibrateOneParam();
//void  	AutoCalibrationAllParam();
//void  	GetCalibrationAllValues();
//void  	AutoCalibrationSelectedParam();
//void  	GetCalibrationSelectedValues(int);

//Modified:jj:12042001
//void  	CalibrationReport(void);
//void  	GetCalibrationReportValues(int);
int  	CalibrationReport(void);
int  	GetCalibrationReportValues(int);

void  	FindSysCal(void);
int  	WriteString(char *,int,int,int nLineFeed=TRUE);
void  	StopPrint();
int 	GetFirstAdcValue( int nRange, int *nChannel, char *cChannelIndex );
int 	GetAdcValue( int nChannel, int nRange );
int 	areAdcCompatible( int firstRead, int secondRead );
//FILE*   PrepareFile(void);
FILE * PrepareFile(char *szFileName);
void    RemoveFile(void);
void    RemoveIniFiles(void);
void    SaveFile( void );
// NileshB - 25th Aug., 1999 (Start - MZNG MEM)
int		ReadParamDotCts( char prmAbr[MAXPARAMABRNAME], PRM *prm );
int		ReadParamDotHdr( void );
// To Read the parameter details from PARAM.CTS file ___
// Only one parameters data will be read at a time...
// NileshB - 25th Aug., 1999 (End - MZNG MEM)
void SendDataToDMux (int,int,unsigned char);
void FillDispStruct(PRM *prmarray,int paramno);
void 	InitializeSegmentLines();
/*void 	SendSegmentData(CTPAIR CtPair,INFORMATION m_FileInfo);
void 	DisplayDigit(int Data,int Line);*/
void DisplayDigit( int Digit, int index, int FIXTURE, int PAIR );
void SendSegmentData(CTPAIR CtPair,INFORMATION m_FileInfo);
void Set_Cups_Data(int);
void ReadStdVal(int nParamCount);
void InitialiseCALarray(void);
void SelectSplitForCal();
float GetAverageReadingSync(int nRange,int nRef);
int SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2);
float getCmRamp( int nRange );
float getDpmRamp( int Adc1st, int Adc2nd, int Adc3rd, int nRange );
int GetReplacePairNo( int pnPair1 );
int ReadADC (int nChannelNumber);
float FindCalOffset (int nRange);
void ReadInstrStd (float *pfStd, int nRange, int nFreqIndex);
float FindStdVal (int nRange);
void getPair( int *pnPair1, int *pnPair2 );
void SetPairCtrlLines( int nFixType, int nStatus, int nPair1, int nPair2 );
void SetSpltCtrlLines( int nFixType, int nStatus, int nPair1, int nPair2, int bOnOff );
void getNextPair( int *pnPair1, int *pnPair2 );
float GetFixOffsetFromArray (int param,int pair1,int pair2,int voltage);
#endif