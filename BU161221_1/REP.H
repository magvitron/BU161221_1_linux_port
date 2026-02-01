#include<dos.h>
#include<bios.h>
#include<conio.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "ct.h"
#include "share.h"

#define       ESC                       27
#define       PRINTPORT                 0
#define       SENDDATA                  0
#define       INITPRINTER               1
#define       PRINTSTATUS               2
#define       POS_LEFT                  0
#define       POS_CENTER                1
#define       POS_RIGHT                 2

//#define       MAXPRINTCOLS              140//120 MOD05072002:03:jj
#define       MAXPRINTCOLS              160//140 MOD06072002:01:jj

#define       SHRINKMODE                120
#define       CONDENSEDMODE             MAXPRINTCOLS
#define       NORMALMODE                80
#define       MAXCRMEM                  220
#define       MAXGRMEM                  104
#define 	  MAXFILEINQUEUE			15


typedef struct tagRepReadings
{
	int nMaxReadingLen;
	int nMaxColsboth;
	int nMaxColsone;
}READINFO;

typedef struct tagGroupReadings
{
	 char  cPair;
	 float fRawVal;
}GROUPREADINGS;

int  InitialisePrinter();
void SetPageLength();
int  SendtoPrint(char);
void SelectNlq(char);
void SelectCondensed(char);
void SelectDoublewidth(char);
void SelectSuperscript(char);
void SelectUnderline(char);
void PrintHeader(void);
void SelectShrink(char);
int  WriteString(char *,int,int,int nLineFeed);
void RepeatChar(char,int);
void LineFeed(void);
void FormFeed(void);
void PrintParamHeader(int nParam);
void PrintData(char,char,char,float);
void ShiftString(char szTmp[],int nTmp);
void FormFeed(void);
void SkipTopMargin(void);
void InitialiseStatVar(int);
int  WithinSpeck(int,float fTmp);
void ShowRemark(void);
void EndReport(void);
void PrintUnitHeader(int);
void PrintGroupData(char cPair1,float fRa,float fRb,float fRaw1,float fRaw2);
void StopPrint ();
void PrintFormulae(void);
void SelectElite(void); //Added by:sdt:29042001