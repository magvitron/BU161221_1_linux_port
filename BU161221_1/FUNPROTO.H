#if !defined(__FUNPROTO_H)
#define __FUNPROTO_H

#include "speck.h"
#include "seq.h"
#include "cconfig.h"
#include "share.h"
//cts.cpp
void ReadAtnVarOffs (void); // Function to Read Atn from .ini file

//Limits.cpp
void ReadLimitFromSivFile(int SpeckFileNo);
CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit);
//Share.cpp
int SelectFixturePairtoPair (int nStart,int nCombnType);
float CalculateRu( float dRaNor, float dRbNor, unsigned char uRuMmtMethod );
int FindNewDecimal(float fFlotVal,int nNo);
void ExchangeDelayInfo (int nExchange);
void RemoveFilesFromRamDrive(void);
SEQCOUNT CountPairsandUnits();
PAIRSWITCH ReadPairSwitchingOptions();
SEQCOUNT CountPairsandUnitCmbLastUnit(int UnitSize,int nLastUnit);
SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType);
void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo);
int CrTest();
int CmTest();
int CupgTest();
int CupsTest();
int CuppTest();
int CUPPITest();//Added by:sdt:22042001
int ElfextTest();
int AtnTest();
int NextTest();
int  ZCalTest();
int  ZmsTest();
int  GmTest();
int FextTest();//Added:jj:03062001
int InextTest();
int GetLimitIndex(int Param);

//Speck.cpp
Boolean readSpeck( STRUCT_SPECKS& ,int fileNumber );
void WriteLimitInSivFile(int SpeckFileNo,int bNewFile);
void ReadLimitFromSivFile(int SpeckFileNo);
SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex,int bCreateSpeckFlag);
void ModifyIntermediateLimits(int nParam,int nFreq,int nLimitIndex,SPECKLIMIT specksLimit);
void CopyLimitsToIntmLim(void);
int findSpeckFiles(TSpeckCollection *pc);
//Cts.cpp
void getRunSetup(void);
void RemoveFilesFromRamDrive(void);
void getConfigSetup(Config& config);
unsigned int ReadStartup ();
void WriteStartup (unsigned int);
void freeZmsResLines();
void GetZmsResLines();
void InitializeZchArray(void);

//Readini.cpp
void ReadHFFreqInfo(); //Fsor HF Frequencies
void ReadLFFreqInfo(); //For LF Frequencies
void freeFreqStruct();
void ReadRunSetup(void);//Added by:sdt:25082005:0050
void ReadConfigInfo(void);
void ReadFreqList(void);
void InitSummaryFile(void);
void GetParamDetails();
void ReadHFHLFreqInfo();
//Utils1.cpp
char *NewGetPrivateProfileString (char szAppName[10][30],char *szkeyname,char *szDefault
									  ,char *szStr,char *FileName,int nDefault);
int CharOccurInString(char *String,char ch);
FILE *ReturnFilePtr(char szAppName[10][30],char *szkeyname,char *FileName);
//CBLSEQ.cpp
void GetCableStructure(int Unit);
void FreeSequenceStructure(int bFlag);
SEQPAIR *ReadPairsFromFile(FILE *fpPtr);
void GetCableCategaryList(char *SeqTitle);
void GetCableUnitsList(int Categary);
//RUN.CPP
void run_initial(void);
void run_selected(void);
void createReport(void);
void setZchFreqforXtalk(int iParam);
void selectZchForXtalk(int *nTestNo);
READING far* GetPrevParamVals( READING far *pparamVal, int param ,int nFreqNum);
READING far* FreePrevParamVals( READING far *pparamVal, int param );
FileStatus	GetConfigStructure (char *);
void		PrepareFileForPrint(void );
void ReadRunSetupINI(void);
void getReportSetup();
void ReadReportSetup();
int			SendFileInQueue(char *szFileName);
void printReport(void);
void newDatFile(void );
void copyConfig(void);
void InitCalDisplay(int nFreqNum);
void ReadHFOffsets();
void viewCalibrationReport(void);
void printCalibrationReport();
int SelectFixtureSinglePair (int nStart);
float 	GetRawReadingCUPP (int nRef);
float 	normaliseCUPP(float fReading);
void vfCreateFormulaeDatFile(void);
void PrintFormulae(void);
void EndReport ();
//Added by:sdt:19062006:2225
int nfXTalkAtLowFreq(int, int);
int CalculateReadingsXTALK(int, int);
//Added by:sdt:06112006:2105
char* ntos( double val, int dec, int prec, int round );
char * getRangeText( int range, int nParam );
void PrintAutocalRep();
void SortArray(float *Array,int Arraysize);
void InsertNumber(float Val,float *Array,int Arraysize);
int Search(int ArrayIndex,float *Array,float Val,int Arraysize);

void copynConfigtoConfig(void);
void  PinReplacement(void); //Added by:sdt:26042015:1700
#endif