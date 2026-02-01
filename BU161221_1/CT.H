#if !defined( __CT_H )
#define __CT_H

#include <stdlib.h>
#include <dir.h>
#include <dos.h>

//#include "share.h"
#include "uses.h"

//#define MAXLIMITTYPES		4 //Added by:sdt:02092001
#define MAXLIMITTYPES		2 //5 //MOD08072002 //Modified 5 to 2 by:sdt:05082005:1030

#define CABLESEQMNULENGHTH 	8   //Added by:sdt:03052001

#define MENULENGTH			8   //Added by:sdt:22042001
#define MAXLENCABLESEQ		30  //Added by:sdt:22042001

#define NO_BREAK			0x04;//Added:jj:22012001:1630
#define NO_SHORT			0x08;//Added:jj:22012001:1630

#define MAXFREQUENCIES      	8//10 Modified by:sdt:01102005:0930 //For Quad
#define PAGESIZE				16384
#define MAX_PHYPGS				4
#define MAX_FILESINLIST			100
#define MAX_FILESEL				15			// max file that can be selected
// NileshB - 25th Aug., 1999 (Start - MZNG MEM)
#define MAX_CTRL_LINES			15
#define MAXREADINGNAME_LEN		30
// NileshB - 25th Aug., 1999 (End - MZNG MEM)
#define MAXREADINGNAMESIZE      80
#define TRUE                    1
#define FALSE                   0
#define MAXINFOLEN              25
#define MAXBYPASSPAIRLEN        25
#define MAXFILENAME             13
#define TIP                     0
#define RING				  1

//#define MAXPARAMETERS           13//12//11//Modified:jj:05042001 10 //9 Modified:jj:25032001
//#define MAXPARAMETERS           10//Changed : P.N.B.(05101999)
//#define MAXPARAMETERS           11//Changed : P.N.B.(07101999)
//Maxparameters & SRNO's modified for following parameters by
//sdt:30092005:2000 for quad software.
#define MAXPARAMETERS           11
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
//Added:jj:11042001
//#define SRNO_GM                 4


//#define SRNO_CUPS               6




//Commented:jj:05042001
//#define SRNO_ZCH				6//Modified:jj:04042001
#define SRNO_ELFEXT             14//12//10//8 Modified:jj:04042001//Modfied by:sdt:04082001


//Added:jj:04062001
#define SRNO_INEXT            13


//Added:jj:05042001

#define SRNO_ZMS				15//Modified :jj: 04062001 13//11

//Commented by:sdt:03082001
//#define SRNO_V1                 16 //Added by:sdt:31072001
//#define SRNO_V2                 17 //Added by:sdt:31072001
#define SRNO_V1V2                 16 //Added by:sdt:03082001

#define SRNO_GROUP              11 //this is not a parameter, use for report only
				//& changed SRNO_GROUP from 9 to 11


#define FRQ_80Hz				2
#define FRQ_800Hz               1
#define FRQ_1KHz                0
#define FRQ_150KHz              0
#define FRQ_552KHz              1
#define FRQ_772KHz              2
#define FRQ_1024KHz             3

#define MAXLENCABLETYPE         15
#define MAXLENGUAGE             6
#define MAXLENINTERUNIT			15
#define MAX_SPECKTYPE_SZ		6



#define AVERAGESIZE             10
#define LIMITSIZE               10
#define RMSSIZE                 10
#define STDSIZE                 10
#define LENGTHSIZE 				10
#define MEAN_STD_SIZE 			15
#define INTRA_LO_UPSIZE			15


#define MAX_MUL_FILES           10
#define MAXSPECKLEN             22
#define MAXNOOFPAIRSLEN         4
#define MAXCABLELEN             6
#define MAXTEMPLEN              6
#define MAXUNITOFLEN            3
#define MAXLENSPECKSNAME      	22
#define MAXLENSPECKSNUMBER      4
//san
#define MAXCOLUMNS              20
#define MAXCOLUMNWIDTH          15
#define MAXSCROLLHEADERLEN      48

#define SRNO_BREAK			  0x04//Added:jj:22012001:1640
#define SRNO_SHORT			  0x08//Added:jj:22012001:1640

//Added:jj:04042001

#define PARAMETERNAMESIZE       15
#define PARAMETERLINESIZE       50
#define PARAMETERUNITSIZE       10
#define OVERRANGEMSGSIZE        40
#define EXPRESSIONSIZE          10
#define MAXFORMULALEN           150

#define TWOREADINGS             0x01
#define TWOPAIRS                0x02
#define HFPARAM                 0x10
#define LFPARAM                 0x00

#define MAXPARAMNO              (2)
#define MAXLENPAIR              (3)
#define MAXLENUNITNO            (2)
#define MAXCAPTION              (MAXPARAMNO+MAXLENPAIR*2+MAXLENUNITNO)
#define TOBYT                   (11-2)
#define INVALIDREADING          0x3fff
#define MAXSTRINGLENGTH1        50
#define MAXTITLES               8
#define ENTRYBASE               8

#define FREQBASE                -1//Added:jj:18032001

#define PARAMNAME               2

#define FREQLMTS				1	//Added by:sdt:17082001
#define FREQSTATUS				28  //Added by:sdt:17082001

//Added:jj:18042001
#define LMTS					26
#define LIMITCATEGORY           27

#define INDMIN                  3
#define INDMAX                  4
#define AVERAGEMAX              5
#define AVERAGEMIN              6
#define STDMIN                  7
#define STDMAX                  8
#define RMSMIN                  9
#define RMSMAX                  10

//MOD0507002:01:jj
#define PSMIN       			24
#define PSMAX                   25

//MOD06072002:01:jj
#define WPNMIN       			30
#define WPNMAX                  31

#define LENGTH_SPECKS			11
#define MEAN_STD_SPECKS			12
#define TEMP_SPECKS             13
//#define INTRAUNIT_SPECKS	   	13//Commented :jj:18042001

#define FRQ 					25
#define FREQ1					14
#define FREQ2					15
#define FREQ3					16
#define FREQ4					17
#define FREQ5					18
#define FREQ6					19
#define FREQ7					20
#define FREQ8					21
#define FREQ9					22
#define FREQ10					23

#define BEEP                    1000
#define BLANKVALUE              99999.99
#define MAXHEADERSIZE           55
#define DETAILEDREP             1
#define SUMMARYREP              2
#define MAXPASSLEN              8
#define SCREEN                  1
#define PRINTER                 2

#define NORMALISED              1
#define UNNORMALISED            2
#define SUMMARY                 10
#define REMARK                  3
#define ALL                     4
#define SELECTEDPARAM           5
#define PRNFILENAME             6
#define STARTPRINT              7
#define STARTDISP               8
#define STOPPRINTING            11
#define TEXT1					30//Added:pnb:26052000
#define TEXT2					31//Added:pnb:26052000
#define TEXT3					32//Added:pnb:26052000


typedef int    BOOL;

#define MAXPARAMNAME				55
#define MAXPARAMABRNAME				12
#define MAXUNITSIZE					12
#define MAXRANGES					5
#define MAXADCCHANNELS				8
#define MAXREADINGSPERPARAM			1050
#define SUM_MIN						0
#define SUM_MAX						1
#define SUM_AVG						2
#define SUM_RMS						3
#define SUM_STDDEV					4
#define SUM_DETAILED				0
#define SUM_SUMMARY					1
#define SUM_UNNORMALISED			1
#define SUM_NORMALISED				2
#define SUM_BOTH					3


#define SELEPARAMSEQ				1
#define FULLSEQ					2
#define BYPASS						3
#define SBRK						4
#define	RANGEHOLD					5
#define SINGLESTEP					6
#define FIXTURE_NO		        		7
#define ZCHFORATNXTS				8	//added by:sdt:12082001
#define SHORT_BREAK					10 //Added:jj:22012001

//#define CRIM						7//Changed:P.N.B.:Added:(08101999)

#define HILIGHT						1
#define LOLIGHT						2
#define GRAPHDIR                  	"\\BORLANDC\\BGI"


// masking for columns used in the file load option
#define M_SPECKNO					0x0001	// mask for specification no.
#define M_TESTEDBY					0x0002	// mask for test by
#define M_NOOFPAIRS					0x0004	// mask for no. of pairs
#define M_PAIRSINUNIT				0x0008	// mask for pairs in unit
#define M_LENGTH					0x0010	// mask for cable length
#define M_DRUMNO					0x0020	// mask for superunit no.
#define M_STARTPOS					0x0040	// mask for starting position
#define M_FIXTYPE					0x0080	// mask for fixture type
#define M_ROOMTEMP					0x0100	// mask for room temp.
#define M_TITLE1					0x0200	// mask for title 1
#define M_TITLE2					0x0300  // mask for title 2
#define M_TITLE3					0x0400  // mask for title 3
#define M_TITLE4					0x0500  // mask for title 4
#define M_TITLE5					0x0600  // mask for title 5
#define M_TITLE6					0x0700  // mask for title 6
#define F_COLUMNS					3		// 3 columns for file loading

#define MAX_DELAYS					5
#define D_SIGNATURE					"CTSAN#333"	// delay signature
#define F_NORDCTRL				1
#define F_INVRDCTRL				2
#define MAX_OTHERINFOLEN		sizeof("Name: ")+sizeof(" Mode: ")+ \
								sizeof(" Freq: ")+sizeof(" Unit: ")+ \
								MAXPARAMNAME+MAXUNITSIZE+13+3

#define DLY_INSTRUMENT	0
#define DLY_JUNCTION	1
#define DLY_RELAY		2
#define DLY_PREREAD		3
#define DLY_RANGE		4
// Nilesh - 13th Aug., 1999 - Start // for atn offsets substraction for
#define MAX_CAB_TYPE			4   // 3 types of Cable 0.4, 0.5, 0.63, 0.9 Guage Cable.
#define MAX_SPANS				20  // Maximum No. of Spans below 1000 for Siva Eng.
// Nilesh - 13th Aug., 1999 - End
//#define DLY_POSTREAD	4

//Added:Start:sdt:17092001:1520
#define IGNORE  0
#define RETRY   1
#define ABORT   2

#define SecuString	"EPOINTECH"
#define LCKM		100000.00

typedef struct key
{
	char	Security[10];
	char 	path[MAXPATH];
	char	exename[13];
	int 	OriginalRunCount;
	int 	InstallCount;
	int 	RunCount;
	long    FloppySrNo;
	long    DiskSrNo;
	double	lckm;	//Added:jj:21082001
}STRUCT_KEY;
long GetSrNo(int nDrive);
//void onexit();
int onexit();
void init();
void install();
void uninstall();
void SetSrNo(int disk);
void GetExeName(char *path);
void getinfo();
//End:sdt:17092001:1520


//Added by:sdt:03052001
typedef struct SeqNames
{
	char  FileName[13];
    char  SeqName[32];
}SEQNAMES;


// NileshB - 25th Aug., 1999 (Start - MZNG MEM)
typedef struct tagParamInfo
{
	int 	ParamNo;
	char	szParamAbr[MAXPARAMABRNAME];
    int 	nParamType;
}PARAMINFO;
// NileshB - 25th Aug., 1999 (End - MZNG MEM)

struct LENGTH_SPAN
{
 int nUpperLimit;
 int nLowerLimit;
 struct tagFactorDetail
 {
   float fFactorVal;
   char  cFactorDetail;
   int   LinesON[3];
   int   LinesOFF[3];
 }FactorDetail[MAXPARAMETERS];
};

typedef struct tagRdControl
{
	int m_anDelay[MAX_DELAYS];	// delays for reading
	int m_nAvgof;				// set of readings for averaging
	int m_PostFreqDelay[MAXFREQUENCIES]; //This are post frequency delays.
	//Added by:sdt:03082001
}RdControl;

//typedef struct tagHeader
//{
//	char szSignature[10];
//	char szReserved[20];
//}Header;

typedef struct tagRepSettings
{
	int 	nSumm_Detailed;
	int 	nRemark;
	int 	nNormUnnorm[MAXPARAMETERS+1];
	int 	nParams[MAXPARAMETERS+1];
	int 	nGroupRep;
}REPSETINGS;

//Commented :MOD11072002:02:jj
/*typedef struct tagParam
{
	char   cNull;
	char   szParamName[MAXPARAMNAME];
	char   szParamNameAbr[MAXPARAMABRNAME];
	char   szUnit[MAXUNITSIZE];
	int    nParamType;   //(Lf/Hf)
	int    nTesting;
	int    nTestingMethod;  //(pair to pair /single pair)
	int     nPositions;
	int     nReadings;
	char    szReadingNames[MAXREADINGNAMESIZE];
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
}PARAMETER;*/

typedef struct tabCTPAIR
{
	short   nTest;
	int         Pair1;
	int       Pair2;
	short   TipOrRing;
	short   UnitSrNo;
	float   val;
	float   norval;
}CTPAIR;
//Below structure is Added by:sdt:20082001
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

    //Grand power sum MOD0507002:01:jj
	char    psumlow[RMSSIZE];
	char    psumhi[RMSSIZE];

    //1%Worst Pair Next MOD06072002:01:jj
	char    wpnlow[RMSSIZE];
	char    wpnhi[RMSSIZE];


}SPECKLIMIT;*/ //Commented by:sdt:03082005:1845

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

//Commented by:sdt:13112005:1255
/*typedef struct
{
	char    	speckno[MAXLENSPECKSNUMBER];
	char    	speckname[MAXLENSPECKSNAME];
	char    	cabletype[MAXLENCABLETYPE];
	char		type[MAX_SPECKTYPE_SZ]; // PCM='YES' / NORMAL='NO'
	//char  szCableSequence[MAXLENCABLESEQ];//Added by:sdt:22042001
	SEQNAMES szCableSequence;//Addedby:sdt:03052001
	struct
	{
		char    name[PARAMETERNAMESIZE];
	   char	norlength[LENGTHSIZE];
	   char norTemp[LENGTHSIZE]; //added by:sdt:13072001
		//Added:jj:18032001
		char freq[10][4];

	}speckparams[MAXPARAMETERS];
} SPECIFICATION;*/

//Below structure Added by:sdt:20082001
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

    //Grand power sum MOD0507002:01:jj
	float   psumlow;
	float   psumhi;

    //1%Worst Pair Next MOD06072002:01:jj
	float   wpnlow;
	float   wpnhi;
	float	 anyother;//Added by:sdt:09102005:1215

}CONFIGLIMIT;

typedef struct
{
	int	nSpeckFileNumber;
	char    specksnumber[MAXLENSPECKSNAME+1];
	char    cabletype[MAXLENCABLETYPE+1];
	char    guage[MAXLENGUAGE+1];
    char 	Type; // PCM=1 / Normal=0
    //char  szCableSequence[MAXLENCABLESEQ];//Added by:sdt:22042001//Commented by:sdt:28072005:1035
	SEQNAMES szCableSequence;
	float		fNormTemp;	//Added by:sdt:02082005:2315
	//float		fNormLength; //Added by:sdt:02082005:2315

	struct
	{

    	//Below structure commented by:sdt:20082001
    	/*struct
        {
	        float   lowerlimit;
			float   upperlimit;
			float   averagelow;
			float   averagehi;
			float   stdlow;
			float   stdhi;
			float   rmslow;
			float   rmshi;
        }specklimits[4];*/
        //Commented:jj:18042001
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

     //Modifications for Break and Short
     //Added:jj:22012001:1555
     char 		cBreak;
	char 		cShort;
     unsigned char	BSResult;

}CONFIG;

typedef struct tagDummy
{
	 char  szHeader[120];
	 int   nEnd;
}Dummy;

//Commented by:sdt:27072005:1915
typedef struct tagPass
{
	 char szSivaEng[MAXPASSLEN];
	 char szEntry[MAXPASSLEN];
	 char szExit[MAXPASSLEN];
	 char szSetup[MAXPASSLEN];
	 char szSpeck[MAXPASSLEN];
	 char szArgv[MAXPASSLEN];
}Pass;//PASS;

typedef struct tagBasicInfo
{
	char cVersion[10];
	char cModel[20];
	char cSerialNo[30];
	char cCompanyName[50];
	char cCompanyCity[25];
}BasicInfo;

struct loadfile
{
	char count;		// max can be MAX_FILES	( 15 )
	char filename[15][10];
};

struct fileinfo
{
	char 	name[13];
	long    size;
	union
	{
		struct {
			unsigned int time;
			unsigned int date;
		}st;
		unsigned long	utime;
	}tm;
	char col[3][13];
	char allign [4];		// allignment with PAGESIZE
};

struct fname
{
	char 	name[MAX_FILESEL][13];
	char 	count;
};
/*
typedef struct tagATNVARLENOFF
{
	int length1;
	int length2;
	float offset;
}AtnVarLenOff;
*/

// Nilesh - 13th Aug. 1999 - Start
// Structure Written to create the ATN Lookup table for the Length Spans...
typedef struct tagATNVarLenOffs
{
 int 	nLowerLimit;
 int 	nUpperLimit;
 float	fOffset[MAX_CAB_TYPE];
}ATNVarLenOffs;
// Nilesh - 13th Aug. 1999 - Start
typedef struct tagCMVarLenMF
{
 int 	nLowerLimit;
 int 	nUpperLimit;
 float	fLenMf;
}CMVarLenMfs;


//Commented:jj:18052001
/*typedef struct tagParamLst
{
	char 	szAbr[MAXPARAMABRNAME];
	char 	szName[MAXPARAMNAME];
	char 	szOtherInfo[MAX_OTHERINFOLEN];
	unsigned short  nIndex;
}ParamLst;*/

typedef struct tagTESTPARAM//Added:jj:22012001:1630:Complete Structure
{       					//for Embeded BnS
    int         Start;
    int         End;
    int         TestsToPerform;
    int         SingleStep;
    short       FixtureNo;
    char        szFilename[20];
} TESTPARAM;


enum SortOption { f_name, f_size, f_time};

const	int gnSzfile = sizeof (struct fileinfo);

void  	fullseqinit (void);
int   	seleparaminit (char *caption);
int   	SearchAndLoadFile(char *caption);
char  	*ToUpperStr (char *string);
int   	messagewindow(char *message);
BOOL  	Report();
void  	InitialiseAutoCal();
BOOL  	ShowSpecks();
//Commented by:sdt:13112005:1300
//int   	findspeck(char *specknumber,char *cabletype,SPECIFICATION *speckptr);
int   	speckmenuinit (int left, int top);
void  	speckcontinue (int key);
void  	speckinitialize (void);
void  	InitialiseGraphics(void);
int   	readpassword (void);
BOOL  	ReadSetUp (void);
//int   	logo (int left, int top, int right, int bottom);//Commented by:sdt:04012006 //As this function is not required.
int   	filemenuinit(int,int);
int   	getrunrhs(int,int,char *,int,char *,int (*)(char *));
int   	getprintrhs(int,int,char *,int,char *,int (*)(char *));
int   	FloatToInt(float);
//int   	GetPrivateProfileString(char *,char *,char *,char *,char *,int nDefault=TRUE);//Commented by:sdt:26072005:0055
int   	GetPrivateProfileInt(char *,char *,int,char *);
float  	GetPrivateProfileFloat(char *,char *,float,char *);//Added by:sdt:19082001
void  	InitializeINIInfo(void);
void  	OnRunFullsequence(int nTestNo[]);
//void  	statusdisplay (int parameternumber,int nFreqNum);//modified by:sdt:21032001
//void  	statusdisplay (int parameternumber);//Commented by:sdt:21032001
void  	SetSpeckInfo(void);
//int   	graphicsinit(void);
//void  	setupbargraph (int,int);
//int   	graphicsupdate (int);
int   	graphicsupdate (int ,float fNorVal[]);//Modified:sdt:01042001
//Commented by:sdt:13112005:1300
//int   	freadspeck (SPECIFICATION *speckptr);
void  	showtime (struct time *timep);
void  	SetConfig(int,char *);

void  	SetSpeckList(int,char *);//Added:jj:18032001

int   	CheckLimits(void);
float 	SpeckValue(char *string);
int   	CheckLimits(void);
int   	InitialisePrinter(void);
void  	PrintDetailedRep(void);
void  	SummaryRep(void);
unsigned int ReadStartup(void);
int   	ReadPassWds(void);
void  	WriteStartup(unsigned int );
void  	PrintRep(void);
float 	User_ceil(float,int);
void  	PrintSpeck(void);
int 	DeleteFiles (char *caption);
void	createSubMenus( int );
void	allocmemory (void);
int		EM_Installed (void);
int		PagesAvailable (void);
int 	PgAlloc (int nArrSize, int nEleSize, int *pPgFrmAddr);
void 	Release (int nHandle);

void 	DecryptIni (void);
void 	ReadProgIni (void);
void 	ReadCtsIni (void);
void 	ReadDefaultIni (void);
int		FindNewDecimal(float fFlotVal,int nNo);
void 	getPosOffset( void );
//Added by:sdt:22042001
char *NewGetPrivateProfileString (char szAppName[10][30],char *szkeyname,char *szDefault,
								  	char *szStr,char *FileName,int nDefault);

// Nilesh - 13th Aug. 1999 - Start (Commented)
// void 	ReadLengthSpan( void );
// Nilesh - 13th Aug. 1999 - End
#endif