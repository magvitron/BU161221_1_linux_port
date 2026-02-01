#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <io.h>
#include <conio.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <process.h>
#include <math.h>

#include "common.h"
#include "utils.h"
#include "seq.h"
#include "funproto.h"
#include "setup.h"
#include "share.h"
#include "ct.h"
#include "run.h"
#include "cts.h"
#include "key.h"
#include "rep.h"

#include <ctime>
extern int g_argc;
extern char **g_argv;
/*
struct time {
    unsigned char ti_hour;
    unsigned char ti_min;
    unsigned char ti_sec;
    unsigned char ti_hund; // hundredths
};

static inline void gettime(struct time *t)
{
    std::time_t now = std::time(nullptr);
    std::tm lt{};
    localtime_r(&now, &lt);

    t->ti_hour = (unsigned char) lt.tm_hour;
    t->ti_min  = (unsigned char) lt.tm_min;
    t->ti_sec  = (unsigned char) lt.tm_sec;
    t->ti_hund = 0; // you can improve this using gettimeofday/clock_gettime
}
*/


int nRetestflag=0; 						//Added by:sdt:14072009
unsigned long ulParamRunSetup[MAXPARAMETERS];//Added by:sdt:14072009


//float HF_Offset[MAXFREQUENCIES][MAXRANGES];//Commented by:sdt:10112005:1930
extern int 			nCurrentGuage; // Serves as the Current Index for Array
extern float           fAtnRefStd[MAXFREQUENCIES];
extern int            nDC;
extern  char 	Text1[2],Text2[2],Text3[2];//Added:pnb:27052000
int		PARAMEND = MAXPARAMETERS+1;
int 	LoadFlag=0;
//char    summarystr[9]="Detailed";//"Summary"; //Commented by:sdt:16102005:1840 //As it is no longer used
//char	remarkstr[4] ="Yes"; //Commented by:sdt:16102005:1840 //As it is no longer used
//char 	normalstr[15] ="Both";//Commented by:sdt:16102005:1840 //As it is no longer used
struct 	fname fname;
int 	CRTestedFlag=0;
int 	DOT;
extern  char  	szRangeHold[2];
char 	szLfZchMode[5]="CAL"; //MOD29072002:01:jj
char 	szHfZchMode[5]="CAL";

char 	szLfAtnMode[5]="CAL"; //Added:MOD29072002:02:jj
char 	szZchAutoManual[8]="Manual";//Added:MOD11082002:01:jj
extern int     gnTest[MAXPARAMETERS];
extern int		nflagRangeLock;
extern int 	HFCalibrateFlag;

//extern int DisplayArray[MAXPARAMETERS];//Commented by:sdt:16102005:1840 //As it is no longer used
extern int nTempUnitOf;
extern int nCurrentReadingno;
extern char cSinglePair1, cSinglePair2;
extern int nParamCount;
extern int param_tested[MAXPARAMETERS][MAXFREQUENCIES];
extern char	szSeqFileName[15];
extern int Pcm_Cable;//Added:pnb:19111999.
extern char SpeckList[MAXFREQUENCIES][10];//26032001
extern char DataDirectory[MAXPATH];
extern char cRamDrive[2];
extern int 	ParamCtsHandle;
extern int  nLastUnitSize,nUnits,nNeedOfAppend;
extern int nReport;
//extern SEQUENCE CableSequence;
//extern char	bZchMeas=FALSE;
extern int noofparameters;
extern int 		   factor;
extern int bSync;
extern float gfOffset[MAXRANGES];
extern float fFcal[MAXRANGES];
float RAMP_RANGE2_MF;
extern float t1,t2,CALLOW,CALHIGH;//Replaced:jj:14032001
extern float fAtnCalRef;
extern float fReadAtnStd;
extern int  nLineNo,nPrintMod,nColNo,nPageNo;
//Uncommented by:sdt:06112006:2105
float fReadStd[MAXPARAMETERS][MAXRANGES];//Commented by:sdt:10112005:1950 //as it not used

extern FILE *fptr_Report, *RepView;
FILE            *fpTmp;
struct 	time t;
struct 	date d;


extern STRUCT_SPECKS speckData;
extern CONFIG nConfig;
extern PRM *prmarray;
extern Config config;
extern RunSetup runSetup;//Added by:sdt:24082005:2355
extern INFORMATION 	m_FileInfo;
extern FreqStruct HFFrequency,LFFrequency; //Added by:sdt:05082005:2245
extern FileStatus     fs;
extern PARAM_DETAILS Param;
extern REPINIINFO  	RepIni;
extern CTPAIR         CtPair;
extern RMPINIINFO  	RAMPINI;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];
extern BasicInfo   	m_BasicInfo;
//extern WORSTPAIRNEXT Worstpairnext;
//Modified by:sdt:04022007:1630
extern float Worstpairnext[MAXPARAMETERS][MAXFREQUENCIES][2];
extern REPSETINGS m_RepSettings;
extern ReportSetup reportSetup;
extern RdControl       rdCtrl;
extern Pass			Secu;
//Added Temporary by sdt:09112005:0530
int X ;
int Y ;
int Ypos_Start;
int Xpos_Start;
int Ypos_Freq;
int Range_posX;
int Range_posY;
int Ypos_Std;
int Ypos_Dev;
int Ypos_DevMin;
int Ypos_DevMax;
int Ypos_Calfactor;
int Ypos_Remark;
int Ypos_Meas;
int Ypos_Offset;//30042001
int Ypos_fOffset;//30042001

TView *CalReport;


//void run_initial(void)
void copyConfig(void)
{
	//copy config

	nConfig.Specks.nSpeckFileNumber = config.speckNumber;
	strcpy(nConfig.Specks.specksnumber,config.speckName);
	strcpy(nConfig.szSpeckName,config.speckName);
	strcpy(nConfig.Specks.cabletype,config.cabletype);

	nConfig.Specks.szCableSequence = config.szCableSequence;
	sprintf(nConfig.szNoofpairs,"%d",config.m_Noofpairs);
	sprintf(nConfig.szUnitof,"%d",config.m_Unitof);
	nTempUnitOf=config.m_Unitof;//Added by:sdt:27092005:0000
	sprintf(nConfig.szLength,"%u",config.m_Length);  //SDT11122016:0730
	sprintf(nConfig.szIlm,"%u",config.m_Ilm);
	sprintf(nConfig.szFlm,"%u",config.m_Flm);
	//Modified by:sdt:04122005 //instead of %d - %.1f is added.
	//As room temp. is float value it gives 0 by using %d.
	sprintf(nConfig.szRoomtemp,"%.1f",config.m_Temp);
	sprintf(nConfig.szFixPos,"%d",config.m_StartFrom);
	if(runSetup.wFixture==BOTHFIXTURE)
	{
		strcpy(nConfig.szFixtures,"Both");
	}
	else if(runSetup.wFixture==OUTERFIXTURE)
	{
		strcpy(nConfig.szFixtures,"Outer");
	}
	else if(runSetup.wFixture==INNERFIXTURE)
	{
		strcpy(nConfig.szFixtures,"Inner");
	}
	if(runSetup.wShortBreak == NONEBREAKSHORT)
	{
		nConfig.cBreak = 'N';
		nConfig.cShort = 'N';
	}
	else if(runSetup.wShortBreak == ONLYSHORT)
	{
		nConfig.cBreak = 'N';
		nConfig.cShort = 'Y';
	}
	else if(runSetup.wShortBreak == ONLYBREAK)
	{
		nConfig.cBreak = 'Y';
		nConfig.cShort = 'N';
	}
	else if(runSetup.wShortBreak == BOTHBREAKSHORT)
	{
		nConfig.cBreak = 'Y';
		nConfig.cShort = 'Y';
	}
	//read cable sequence fully
	FreeSequenceStructure(ALLPAIRS);
	strcpy(szSeqFileName, config.szCableSequence.FileName);
	GetCableCategaryList(config.szCableSequence.SeqName);
	GetCableUnitsList(config.m_Noofpairs);
	GetCableStructure(config.m_Unitof);

	for ( int i=0; i<MAX_USERENTRIES; i++ )
	{
		strcpy(m_FileInfo.ENTRY[i].szTitle,config.m_UserTitles[i]);
		strcpy(m_FileInfo.ENTRY[i].szEntry,config.m_UserEntries[i]);
	}

}

void copynConfigtoConfig(void)
{
	//copy config

	config.speckNumber = nConfig.Specks.nSpeckFileNumber;
	strcpy(config.speckName,nConfig.Specks.specksnumber);
	strcpy(config.speckName,nConfig.szSpeckName);
	strcpy(config.cabletype,nConfig.szSpeckName);

	config.szCableSequence = nConfig.Specks.szCableSequence;
	config.m_Noofpairs = atoi(nConfig.szNoofpairs);
	config.m_Unitof = atoi(nConfig.szUnitof);
	//nTempUnitOf=config.m_Unitof;//Added by:sdt:27092005:0000
	config.m_Length = atoi(nConfig.szLength);
	config.m_Ilm = atoi(nConfig.szIlm);
	config.m_Flm = atoi(nConfig.szFlm);

	//Modified by:sdt:04122005 //instead of %d - %.1f is added.
	//As room temp. is float value it gives 0 by using %d.
	config.m_Temp = atof(nConfig.szRoomtemp);
	config.m_StartFrom = atoi(nConfig.szFixPos);

	//if(runSetup.wFixture==BOTHFIXTURE)
	//{
	//	strcpy(nConfig.szFixtures,"Both");
	//}
	//else if(runSetup.wFixture==OUTERFIXTURE)
	//{
	//	strcpy(nConfig.szFixtures,"Outer");
	//}
	//else if(runSetup.wFixture==INNERFIXTURE)
	//{
	//	strcpy(nConfig.szFixtures,"Inner");
	//}
	//if(runSetup.wShortBreak == NONEBREAKSHORT)
	//{
	//	nConfig.cBreak = 'N';
	//	nConfig.cShort = 'N';
	//}
	//else if(runSetup.wShortBreak == ONLYSHORT)
	//{
	//	nConfig.cBreak = 'N';
	//	nConfig.cShort = 'Y';
	//}
	//else if(runSetup.wShortBreak == ONLYBREAK)
	//{
	//	nConfig.cBreak = 'Y';
	//	nConfig.cShort = 'N';
	//}
	//else if(runSetup.wShortBreak == BOTHBREAKSHORT)
	//{
	//	nConfig.cBreak = 'Y';
	//	nConfig.cShort = 'Y';
	//}
	//read cable sequence fully
	//FreeSequenceStructure(ALLPAIRS);
	//strcpy(szSeqFileName, config.szCableSequence.FileName);
	//GetCableCategaryList(config.szCableSequence.SeqName);
	//GetCableUnitsList(config.m_Noofpairs);
	//GetCableStructure(config.m_Unitof);

	for ( int i=0; i<MAX_USERENTRIES; i++ )
	{
		strcpy(config.m_UserTitles[i],m_FileInfo.ENTRY[i].szTitle);
		strcpy(config.m_UserEntries[i],m_FileInfo.ENTRY[i].szEntry);
	}

}

void run_selected(void)
{
	//Copy & modify the code used set condition for xtalk, ZCH & ATN

	char *szTemp, szPairs[MAXBYPASSPAIRLEN];
	int  i=0, nHfSelected=FALSE;//,nFreqNum;
	int nTestNo[MAXPARAMETERS];
	int ntmpRange;
	//int ntmpRange, ntemp;

	for(i=0;i<MAXPARAMETERS;i++)
		nTestNo[i]=0;

	//Uncommented by:sdt:08102005:1445
	if( szRangeHold[0] != NULL )
	{
				nflagRangeLock = TRUE;
				ntmpRange = atoi(szRangeHold);
				if( ntmpRange < 1 )
				{
					//messagewindow ("Invalid Range");
					messageBox( "Invalid Range", mfError|mfOKButton );
					szRangeHold[0] = NULL;
					nflagRangeLock = FALSE;
					//getch();//Commented by:sdt:08102005:1445
					return ;//modified by:sdt:08102005:1445
				}
	}

	memset(m_FileInfo.cPairsBypassed, 127, sizeof(m_FileInfo.cPairsBypassed));
	//strcpy(szPairs, szBypassPairs);
	strcpy(szPairs, runSetup.szBypassPairs);
	szTemp = strtok(szPairs, ",");
	while(szTemp)
	{
		m_FileInfo.cPairsBypassed[i++] = atoi(szTemp) + atoi(nConfig.szFixPos) - 1;
		szTemp = strtok(NULL, ",");
	}

	/*if(bSingleStep==TRUE)
	{
		int j=0;

		cSinglePair1 = 1, cSinglePair2 = 1;
		for(i=0;szRunSelectParam[i]!=' '&&szRunSelectParam[i]!='\0';i++);
		while(szRunSelectParam[i]==' ') i++;
		while(szRunSelectParam[i]!=' '&&szRunSelectParam[i]!='\0')
			szTmp[j++]=szRunSelectParam[i++];
		szTmp[j] = '\0';
		cSinglePair1 = atoi(szTmp);
		j=0;
		while(szRunSelectParam[i]==' '&&szRunSelectParam[i]!='\0') i++;
		while(szRunSelectParam[i]!=' '&&szRunSelectParam[i]!='\0')
			szTmp[j++]=szRunSelectParam[i++];
		szTmp[j] = '\0';
		cSinglePair2 = atoi(szTmp);
		i=0;
		while(szRunSelectParam[i]!=' '&&szRunSelectParam[i]!='\0') i++;
		szRunSelectParam[i] = '\0';
	}*/

		//Commented by:sdt:30092005:2115
		//Commented for Quad software as frequency depenedent settings
		//are not required in Quad software.
		//for(int i=0;szRunSelectParam[i]!=NULL;i++)
		/*for(i=SRNO_CR;i<MAXPARAMETERS-2;i++)
		{
			if((runSetup.ulParamRunSetup[i]&PARAMSELECTIONMASK)!=0)
			{
				if(i==SRNO_ZCAL)//Added:jj:05042001
				{
					for(int x=0;x<4;x++)
					{
						if((runSetup.ulParamRunSetup[i]&(0x1l<<(((x+1)*3)-1)))!=0)
						{
							if(strcmp(szLfZchMode,"CAL"))//This contion added by:sdt:20072001
								nTestNo[SRNO_ZMS]=TRUE;
							else
								nTestNo[SRNO_ZCAL]=TRUE;
						}
					}
					for(x=4;x<MAXFREQUENCIES;x++)
					{
						if((runSetup.ulParamRunSetup[i]&(0x1l<<(((x+1)*3)-1)))!=0)
						{
							if(strcmp(szHfZchMode,"CAL"))//This contion added by:sdt:20072001
								nTestNo[SRNO_ZMS]=TRUE;
							else
								nTestNo[SRNO_ZCAL]=TRUE;
						}
					}
				}
				else
					nTestNo[i]=TRUE;
				if(i>=SRNO_ATN)
					nHfSelected=TRUE;
			}
		}

		//Added by:sdt:04052001
		FillParamInfo();//Commented temporary:23092005:2335

		//for(int pindex=2;pindex<MAXPARAMETERS;pindex++)
		for(int pindex=2;pindex<(MAXPARAMETERS-1);pindex++)//Condition modified by:sdt:06052001
		{
			if(nConfig.Max[pindex]==0)
				nTestNo[pindex]=0;
		}



		if(nTestNo[SRNO_ELFEXT])
		 {
			 //nTestNo[SRNO_ATN]=TRUE;
			 runSetup.ulParamRunSetup[SRNO_ATN] |=(runSetup.ulParamRunSetup[SRNO_ELFEXT] & ALLFREQMASK);
			 //Now Elfext is dependent parameter on FEXT.
			 //Added afterwords:sdt:26092005:1030
			 //Combination bits should set if frequency is selected.
			 //That code is not added at this stage
			 runSetup.ulParamRunSetup[SRNO_FEXT] |= (runSetup.ulParamRunSetup[SRNO_ELFEXT] & FREQANDCMBNMASK);
		}


		//Added:jj:06042001
		for(int z=0;z<MAXPARAMETERS;z++)
			gnTest[z]=nTestNo[z];
		 if(nTestNo[SRNO_ZMS] || nTestNo[SRNO_ZCAL]) //Added:jj:11082002:01:jj
			gnTest[SRNO_ZCAL]=1;



		if (nTestNo[SRNO_FEXT])
		{
			setZchFreqforXtalk(SRNO_FEXT);
		}
		if (nTestNo[SRNO_NEXT])
		{
			setZchFreqforXtalk(SRNO_FEXT);
			if (atoi(nConfig.szLength)<300)
			{
				runSetup.ulParamRunSetup[SRNO_ATN] |= (runSetup.ulParamRunSetup[SRNO_NEXT] & ALLFREQMASK);
				runSetup.ulParamRunSetup[SRNO_ATN] |= (runSetup.ulParamRunSetup[SRNO_INEXT] & ALLFREQMASK);
			}
		}
		if (nTestNo[SRNO_INEXT])
		{
			setZchFreqforXtalk(SRNO_INEXT);
		}

		if((nTestNo[SRNO_ATN] || nTestNo[SRNO_ELFEXT]) && !strcmp(szLfAtnMode,"MEAS"))
		{
			setZchFreqforXtalk(SRNO_ATN);
		}

		if((nTestNo[SRNO_ATN] || nTestNo[SRNO_ELFEXT]) && !strcmp(szLfAtnMode,"CAL"))
		{
			runSetup.ulParamRunSetup[SRNO_CM]|=(runSetup.ulParamRunSetup[SRNO_ATN]&LFFREQMASK);
			if((runSetup.ulParamRunSetup[SRNO_ATN]&LFFREQMASK)!=0)
			{
				runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
			}
			if((runSetup.ulParamRunSetup[SRNO_ATN]&(0x1l<<(((LFFrequency.nTotalFrequencies)*3)-1)))!=0)
			{
				runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((LFFrequency.nTotalFrequencies)*3)-1));
				runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
			}

			if(!strcmp(szHfZchMode,"CAL"))
			{
				runSetup.ulParamRunSetup[SRNO_ZCAL]|=(runSetup.ulParamRunSetup[SRNO_ATN]&0x24920000l);
				if((runSetup.ulParamRunSetup[SRNO_ATN]&0x24920000l)!=0)
				{
					runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
				}
			}
			else
			{
				runSetup.ulParamRunSetup[SRNO_ZCAL]|=(runSetup.ulParamRunSetup[SRNO_ATN]&0x24920000l);
			}
		}
		if(nTestNo[SRNO_ZCAL])
		{
			runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
			if(!strcmp(szLfZchMode,"CAL"))
			{
				runSetup.ulParamRunSetup[SRNO_CM]|=(runSetup.ulParamRunSetup[SRNO_ZCAL]&LFFREQMASK);
			}
			if(!strcmp(szHfZchMode,"CAL"))
			{
				if((runSetup.ulParamRunSetup[SRNO_ZCAL]&HFFREQMASK)!=0)
				{
					runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
				}
			}
		}

		if(!strcmp(szHfZchMode,"CAL")&& nTestNo[SRNO_ZCAL])
		{
			if((runSetup.ulParamRunSetup[SRNO_ZCAL]&HFFREQMASK)!=0)
			{
				runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
				runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
			}
		}


		//Stop on 26092005:sdt:2335

		//Added here by:sdt:22072001
		FillParamInfo();

		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_INEXT])
		{
			selectZchForXtalk(nTestNo);
		}
		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_NEXT])
		{
			selectZchForXtalk(nTestNo);
			if (atoi(nConfig.szLength)<300)
			{
				if((runSetup.ulParamRunSetup[SRNO_ATN]&ALLFREQMASK)!=0&&(nTestNo[SRNO_ATN] != TRUE))
				{
					for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
					{
						if ( param_tested[SRNO_ATN][nFreqNum] == 1)
							nTestNo[SRNO_ATN]=FALSE;
						else
						{
							nTestNo[SRNO_ATN]=TRUE;
							break;
						}
					}
				}
			}
		}
		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_ELFEXT])
		{
			if((runSetup.ulParamRunSetup[SRNO_ATN]&ALLFREQMASK)!=0&&(nTestNo[SRNO_ATN] != TRUE))
			{
				for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
				{
					if ( param_tested[SRNO_ATN][nFreqNum] == 1)
						nTestNo[SRNO_ATN]=FALSE;
					else
					{
						nTestNo[SRNO_ATN]=TRUE;
						break;
					}
				}
			}
			if((runSetup.ulParamRunSetup[SRNO_FEXT]&ALLFREQMASK)!=0&&(nTestNo[SRNO_FEXT] != TRUE))
			{
				for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
				{
					if ( param_tested[SRNO_FEXT][nFreqNum] == 1)
						nTestNo[SRNO_FEXT]=FALSE;
					else
					{
						nTestNo[SRNO_FEXT]=TRUE;
						break;
					}
				}
			}
		}
		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_FEXT])
		{
			selectZchForXtalk(nTestNo);
		}
		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_ATN] && !strcmp(szLfAtnMode,"MEAS"))
		{
			selectZchForXtalk(nTestNo);
		}
		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_ATN] && !strcmp(szLfAtnMode,"CAL"))
		{
			if(runSetup.ulParamRunSetup[SRNO_CR]&(0x1l<<(((nConfig.nLFrequency+1)*3)-1))&&(nTestNo[SRNO_CR]!= TRUE))
			{
				if ( param_tested[SRNO_CR][nConfig.nLFrequency] == 1 )
					nTestNo[SRNO_CR]=FALSE;
				else
				{
					 nTestNo[SRNO_CR]=TRUE;
				}
			}
			if((runSetup.ulParamRunSetup[SRNO_CM]&LFFREQMASK)!=0&& (nTestNo[SRNO_CM] != TRUE))
			{
				for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
				{
					if ( param_tested[SRNO_CM][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
						nTestNo[SRNO_CM]=FALSE;
					else
					{
						nTestNo[SRNO_CM]=TRUE;
						break;
					}
				}
			}
			if(runSetup.ulParamRunSetup[SRNO_ATN]&(0x1l<<(((LFFrequency.nTotalFrequencies)*3)-1))&&!param_tested[SRNO_CM][nConfig.nLFrequency])
			{
				runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
				nTestNo[SRNO_CM]=TRUE;
			}

			if(!strcmp(szHfZchMode,"CAL"))
			{
				if((runSetup.ulParamRunSetup[SRNO_ZCAL]&HFFREQMASK)!=0&&(nTestNo[SRNO_ZCAL] != TRUE))
				{
					for(nFreqNum=LFFrequency.nTotalFrequencies+1;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
					{
						if ( param_tested[SRNO_ZCAL][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
							nTestNo[SRNO_ZCAL]=FALSE;
						else
						{
							nTestNo[SRNO_ZCAL]=TRUE;
							break;
						}
					}
				}
			}
			else
			{
				if((runSetup.ulParamRunSetup[SRNO_ZCAL]&HFFREQMASK)!=0&&(nTestNo[SRNO_ZMS] != TRUE))
				{
					for(nFreqNum=LFFrequency.nTotalFrequencies+1;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
					{
						if ( param_tested[SRNO_ZCAL][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
							nTestNo[SRNO_ZMS]=FALSE;
						else
						{
							nTestNo[SRNO_ZMS]=TRUE;
							break;
						}
					}
				}
			}
		}

		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_ZCAL])
		{
			 if ( param_tested[SRNO_CR][nConfig.nLFrequency] == 1 && nTestNo[SRNO_CR] != TRUE )
				nTestNo[SRNO_CR]=FALSE;
			else
			{
				nTestNo[SRNO_CR]=TRUE;
				runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
			}
			if((runSetup.ulParamRunSetup[SRNO_CM]&LFFREQMASK)!=0)
			{
				for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
				{
					if ( param_tested[SRNO_CM][nFreqNum] == 1 && nTestNo[SRNO_CM] != TRUE )
						nTestNo[SRNO_CM]=FALSE;
					else
					{
						nTestNo[SRNO_CM]=TRUE;
						break;
					}
				}
			}
		}*/

		//Commented by:sdt:27292005:1220
		/*for (int par=0;par<MAXPARAMETERS;par++)//Added:jj: 04052001
		{
			for( int fre=0;fre<MAXFREQUENCIES;fre++)
			{
				//Added by:sdt:11052001:jj
				if(par==SRNO_ZCAL && gnTest[SRNO_ZMS])
					continue;
				else
				if(param_tested[par][fre] == 1 && !gnTest[par])
					strcpy(speck.speckparams[par].freq[fre],"No");
			}
		}*/
		//Following added by:sdt:30092005:2130
		//Code added for Quad software.
		for(i=0;i<MAXPARAMETERS;i++)
		{
			if((runSetup.ulParamRunSetup[i]&PARAMSELECTIONMASK)!=0)
			{
				nTestNo[i]=TRUE;
				if(i>=SRNO_ATN)
				{
					//Condition Added by:sdt:03112006:2145
					//This condition will set nHfSelected flag only if
					//HF frequency is selected for parameters
					//ATN, NEXT & FEXT
					if((runSetup.ulParamRunSetup[i]&HFFREQMASK)!=0)
					{
						nHfSelected=TRUE;
					}
				}
			}
		}

		//Added by:sdt:04052001
		FillParamInfo();//Commented temporary:23092005:2335
		for(int z=0;z<MAXPARAMETERS;z++)
			gnTest[z]=nTestNo[z];

		//for(int pindex=2;pindex<MAXPARAMETERS;pindex++)
		for(int pindex=0;pindex<MAXPARAMETERS;pindex++)//Condition modified by:sdt:30092005:2135 for Quad
		{
			if(nConfig.Max[pindex]==0)
				nTestNo[pindex]=0;
		}

		if(nTestNo[SRNO_ZCAL])
		{
			//Conidtions Added by:sdt:03102005:2300
			for(i=0;i<LFFrequency.nTotalFrequencies;i++)
			{
				if(runSetup.ulParamRunSetup[SRNO_ATN]&(0x1l<<(((i+1)*3)-1)))
				{
					runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
					runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
					//Added by:sdt:03102005:2215
					runSetup.ulParamRunSetup[SRNO_CR]|=PARAMSELECTIONMASK;
					runSetup.ulParamRunSetup[SRNO_CM]|=PARAMSELECTIONMASK;
					nTestNo[SRNO_CR]=TRUE;
					nTestNo[SRNO_CM]=TRUE;
				}
			}
		}

		if( (fs == fs_inprocess||fs == fs_notsaved||fs==fs_new) && nTestNo[SRNO_ZCAL])//Changed:Added:P.N.B.(07101999)
		{
		  if (  param_tested[SRNO_CR][nConfig.nLFrequency] ==1  && nTestNo[SRNO_CR] != TRUE )
				nTestNo[SRNO_CR]=FALSE;
		  else
			nTestNo[SRNO_CR]=TRUE;
		  if ( param_tested[SRNO_CM][nConfig.nLFrequency]==1 && nTestNo[SRNO_CM] != TRUE )
				nTestNo[SRNO_CM]=FALSE;
			else
			nTestNo[SRNO_CM]=TRUE;
		}
		//Added by:sdt:03102005:2215
		//For ATN at Low frequency
		if(nTestNo[SRNO_ATN])
		{
			for(i=0;i<LFFrequency.nTotalFrequencies;i++)
			{
				if(runSetup.ulParamRunSetup[SRNO_ATN]&(0x1l<<(((i+1)*3)-1)))
				{
					runSetup.ulParamRunSetup[SRNO_CR]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
					runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
					//Added by:sdt:03102005:2215
					runSetup.ulParamRunSetup[SRNO_CR]|=PARAMSELECTIONMASK;
					runSetup.ulParamRunSetup[SRNO_CM]|=PARAMSELECTIONMASK;
					nTestNo[SRNO_CR]=TRUE;
					nTestNo[SRNO_CM]=TRUE;
				}
			}
		}

		if (nTestNo[SRNO_NEXT])//Added INEXT:pnb:171199.
			if (atoi(nConfig.szLength)<300)
				nTestNo[SRNO_ATN]=TRUE;


		if(nHfSelected)
		{
			 if(strcmpi(nConfig.szFixtures,"Both")==0)
			 {
					//if(szRunSelectParam[0]!=NULL)//Temporary commented by:sdt:27092005:2435
					{
						 SetSpeckInfo();
						 SetLowFreq(nConfig.nLFrequency);
						 SetHighFreq(nConfig.nHFrequency);
						 OnRunFullsequence(nTestNo);
						 if (fs != fs_bad && fs != fs_error)
							fs = fs_executed;
						 memset(szRangeHold,NULL,sizeof(szRangeHold));
						 nflagRangeLock=FALSE;
						 SetLowFreq(nConfig.nLFrequency);
						 SetHighFreq(nConfig.nHFrequency);
					}
			 }
			 else
			 {
					//messagewindow("Select both fixture to test HF parameters");
					//getch ();
					messageBox( "Select both fixture to test HF parameters", mfError|mfOKButton );
			 }
		}
		else
		{
			 //if(szRunSelectParam[0]!=NULL)//Temporary commented by:sdt:27092005:2435
			 {
					SetSpeckInfo();
					SetLowFreq(nConfig.nLFrequency);
					SetHighFreq(nConfig.nHFrequency);
					OnRunFullsequence(nTestNo);
					/*if (fs != fs_bad && fs != fs_error && fs != fs_inprocess)
						fs = fs_executed;*/
					memset (szRangeHold,NULL,sizeof (szRangeHold));
					nflagRangeLock=FALSE;
					SetLowFreq(nConfig.nLFrequency);
					SetHighFreq(nConfig.nHFrequency);
			 }
			 //else //commented temporary:sdt::27092005:1500
			 //{
			 //		//messagewindow("Select parameters to be tested first");
			 //		//getch ();
			 //		messageBox( "Select parameters to be tested first", mfError|mfOKButton );
			 //}
		}
		//while(kbhit())//Commented by:sdt:05102005:2235
		//	key = getch();//Commented by:sdt:05102005:2235

		//Added by:sdt:14072009:1205:Start
		if(nRetestflag==1)
		{
			//nConfig = tempConfig;
			for(i=0;i<MAXPARAMETERS;i++)
			runSetup.ulParamRunSetup[i]|=ulParamRunSetup[i];

		}
		//Added by:sdt:14072009:1205:End
		char szString[80];
		if( (fs == fs_executed || fs == fs_notsaved) && fs != fs_inprocess)
		{
			if ( messageBox( "Do you want to save the file",
					mfYesButton|mfNoButton ) == cmYes )
			{
				SaveFile();
				sprintf( szString, "File Saved as %s\\%s", DataDirectory, nConfig.szFileName );//Added:jj:18012001:1045
				messageBox(szString,mfInformation|mfOKButton);
				RemoveFile();
			}
			else
			{
				fs = fs_notsaved;
				//Added by:sdt:14072009:1205:Start
				for(i=0;i<MAXPARAMETERS;i++)
					ulParamRunSetup[i]=runSetup.ulParamRunSetup[i];
				nRetestflag=1;
				//Added by:sdt:14072009:1205:End
			}
			//messagewindow("F10: Main Menu    F6: Quit    F7: Reset");//Commented by:sdt:06102005:0625
		}


}

//Commented by:sdt:13112005:1440
/*void setZchFreqforXtalk(int iParam)
{
	if(runSetup.bZchMeas==TRUE)
	{
		if(!strcmp(szLfZchMode,"CAL"))
		{
			runSetup.ulParamRunSetup[SRNO_ZCAL]|=(runSetup.ulParamRunSetup[iParam]&LFFREQMASK);
			runSetup.ulParamRunSetup[SRNO_CM]|=(runSetup.ulParamRunSetup[iParam]&LFFREQMASK);
		}
		else
		{
			runSetup.ulParamRunSetup[SRNO_ZCAL]|=(runSetup.ulParamRunSetup[iParam]&LFFREQMASK);
		}

		if(!strcmp(szHfZchMode,"CAL"))
		{
			runSetup.ulParamRunSetup[SRNO_ZCAL]|=(runSetup.ulParamRunSetup[iParam]&HFFREQMASK);
			if((runSetup.ulParamRunSetup[iParam]&HFFREQMASK)!=0)
			{
				runSetup.ulParamRunSetup[SRNO_CM]|=(0x1l<<(((nConfig.nLFrequency+1)*3)-1));
			}
		}
		else
		{
			runSetup.ulParamRunSetup[SRNO_ZCAL]|=(runSetup.ulParamRunSetup[iParam]&HFFREQMASK);
		}
	}
}*/

//Commented by:sdt:13112005:1440
/*void selectZchForXtalk(int *nTestNo)
{
	if(!strcmp(szLfZchMode,"CAL"))
	{
		if((runSetup.ulParamRunSetup[SRNO_ZCAL]&LFFREQMASK)!=0&&(nTestNo[SRNO_ZCAL] != TRUE))
		{
			for(int nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
			{
				if ( param_tested[SRNO_ZCAL][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
					nTestNo[SRNO_ZCAL]=FALSE;
				else
				{
					nTestNo[SRNO_ZCAL]=TRUE;
					break;
				}
			}
		}
	}
	else
	{
		if((runSetup.ulParamRunSetup[SRNO_ZCAL]&LFFREQMASK)!=0&&(nTestNo[SRNO_ZMS] != TRUE))
		{
			for(int nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
			{
				if ( param_tested[SRNO_ZCAL][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
					nTestNo[SRNO_ZMS]=FALSE;
				else
				{
					nTestNo[SRNO_ZMS]=TRUE;
					break;
				}
			}
		}
	}

	if(!strcmp(szHfZchMode,"CAL"))
	{
		if((runSetup.ulParamRunSetup[SRNO_ZCAL]&HFFREQMASK)!=0&&(nTestNo[SRNO_ZCAL] != TRUE))
		{
			for(int nFreqNum=LFFrequency.nTotalFrequencies-1;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
			{
				if ( param_tested[SRNO_ZCAL][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
					nTestNo[SRNO_ZCAL]=FALSE;
				else
				{
					nTestNo[SRNO_ZCAL]=TRUE;
					break;
				}
			}
		}
	}
	else
	{
		if((runSetup.ulParamRunSetup[SRNO_ZCAL]&HFFREQMASK)!=0&&(nTestNo[SRNO_ZMS] != TRUE))
		{
			for(int nFreqNum=LFFrequency.nTotalFrequencies-1;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
			{
				if ( param_tested[SRNO_ZCAL][nFreqNum] == 1)// && nTestNo[SRNO_ZCAL] != TRUE )
					nTestNo[SRNO_ZMS]=FALSE;
				else
				{
					nTestNo[SRNO_ZMS]=TRUE;
					break;
				}
			}
		}
	}
}*/


void SetSpeckInfo()
{

	//Added:jj:10042001
	int LfFreq;
	int HfFreq;

	int   i,nCount;
	char  nTemp[100];
	//char  *p;
	int temp[MAXFREQUENCIES],count,a;
	float Guage=0.0;//Added by:sdt:27112005:0915

	//strcpy(nConfig.Specks.specksnumber,speck.speckno);//Commented by:sdt:27092005:1440
	//strcpy(nConfig.szSpeckName,speck.speckname);//Commented by:sdt:27092005:1440
	for(i=0;nConfig.Specks.cabletype[i]!='_'&&i < strlen(nConfig.Specks.cabletype);i++)
		nTemp[i]=nConfig.Specks.cabletype[i];
	nTemp[i++]='\0';
//	strcpy(nConfig.Specks.cabletype,nTemp);
	nCount=0;
	for(;nConfig.Specks.cabletype[i]!=NULL;i++)
		nTemp[nCount++]=nConfig.Specks.cabletype[i];
	nTemp[nCount]='\0';
	strcpy(nConfig.Specks.guage,nTemp);
	//Added by:sdt:27112005:0915:start
	// Substrating the Offset for the cable guage 0.4, 0.5 or 0.63
	// nCurrentGuage will be the index of the AtnVarOff array.
	Guage = atof(nConfig.Specks.guage); // Getting the Guage.
	if ( Guage == (float)0.4 )
		nCurrentGuage = 0;
	else if ( Guage == (float)0.5 )
		nCurrentGuage = 1;
	 else if ( Guage == (float)0.63 )
		nCurrentGuage = 2;
	 else if ( Guage == (float)0.9 )
		nCurrentGuage = 3;
	else
		nCurrentGuage = -1;
	//Added by:sdt:27112005:0915:end

	//if ( strcmpi(speckData.Type,"YES") == 0 )
	if(speckData.Type==1)
		nConfig.Specks.Type = 1;
	else
		nConfig.Specks.Type = 0;

	DOT=1; //24032001
	for(a=0;a<MAXFREQUENCIES;a++)//Added:jj:26032001
		temp[a]=0;

	//for(i=0;i<MAXPARAMETERS-1;i++) //-1 Added:jj:12042001
	for(i=0;i<MAXPARAMETERS;i++) //-1 removed by:sdt:30092005:2135 for Quad software
	{
		if(i==SRNO_ATN)//Added:jj:12042001
			for(a=0;a<MAXFREQUENCIES;a++)
				temp[a]=0;


		nConfig.Specks.speckparams[i].nNumofFreqSelected=0;
		for(int x=0;x<MAXFREQUENCIES;x++)
		{
			//if(strcmp(speck.speckparams[i].freq[x],"Yes")==0  )
			if((runSetup.ulParamRunSetup[i]&(0x1l<<(((x+1)*3)-1)))!=0)
			{
				nConfig.Specks.speckparams[i].freq[x]=1;
				nConfig.Specks.speckparams[i].nNumofFreqSelected++;
				temp[x]=1;//26032001

				//Modified:jj:02052001
				//if(i==SRNO_CR || i== SRNO_CM || i== SRNO_CUPG || i== SRNO_CUPP)
				if(i>=Param.Lf_Param_Start && i<=Param.Lf_Param_End)
					LfFreq=x;
				//if(i==SRNO_ATN || i== SRNO_NEXT || i== SRNO_ELFEXT)
				if(i>Param.Hf_Param_Start)
					HfFreq=x;

			}
			else
				nConfig.Specks.speckparams[i].freq[x]=0;
		}

		if(i>=Param.Lf_Param_Start && i<=Param.Lf_Param_End)
		{
			if(nConfig.Specks.speckparams[i].nNumofFreqSelected==1)
				DOT=DOT & 1;
			 else
				DOT=DOT & 0;
		}

		if(i>Param.Hf_Param_Start)
		{
			if(nConfig.Specks.speckparams[i].nNumofFreqSelected==1)
				DOT=DOT & 1;
			else
				DOT=DOT & 0;
		}
		count=0;


		if(i==Param.Lf_Param_End)
		{
			for(a=0;a<MAXFREQUENCIES;a++)
				if(temp[a])
					count++;
		   if(count==1)
				DOT=DOT & 1;
		   else
				DOT=DOT & 0;

		   for(a=0;a<MAXFREQUENCIES;a++)
				temp[a]=0;
		}

		if(i==Param.Hf_Param_End)
		{
			for(a=0;a<MAXFREQUENCIES;a++)
				if(temp[a])
					count++;
		   if(count==1)
				DOT=DOT & 1;
		   else
				DOT=DOT & 0;

		   for(a=0;a<MAXFREQUENCIES;a++)
				temp[a]=0;

		}
		//End:jj:26032001
	}

	//Added:jj:18052001
	DOT=1;
	for(i=Param.Lf_Param_Start+1 ; i<MAXPARAMETERS ; i++) //-1 removed by:sdt:30092005:2135 for Quad
	{
		 if(nConfig.Specks.speckparams[i].nNumofFreqSelected==1)
			DOT=DOT & 1;
		 else
			DOT=DOT & 0;
	}


	if ( nConfig.Specks.Type == 1 )
	{
		//messagewindow("Pcm Specification Selected...");
		messageBox( "Pcm Specification Selected...", mfError|mfOKButton );
		//delay(100);
		Pcm_Cable = 1;
		//Set_Cups_Data(1);//Commented:jj:24032001
	}
	else
	{
		Pcm_Cable = 0;
		//Set_Cups_Data(0);//Commented:jj:24032001
	}

	if(DOT)
	{
		//Modified:jj:10042001 LfIndex to LfFreq ,similar for HF
		strcpy(nConfig.szLFreq,SpeckList[LfFreq]);
		strcpy(nConfig.szHFreq,SpeckList[HfFreq]);
	}

}


void OnRunFullsequence (int nTestNo[MAXPARAMETERS])
{
	int i,nReturnVal=0;//,ret;//,flag=0;//Change:Added:ret,flag:P.N.B.(07101999)

	if (fs != fs_bad)
			fs = fs_inprocess;
	fcloseall ();
	/*for (i=0;i<MAXPARAMETERS;i++)
		gnTest[i] = nTestNo[i];*/
	SetInfo();
	//if(m_FileInfo.nLength>=RAMPINI.nLength) //Commented by:sdt:18122019:1800
	//	 ExchangeRampInfo(TRUE);             //Commented by:sdt:18122019:1800
	FillParamInfo();
	for(i=0;i<MAXPARAMETERS;i++)
	{
		if(nTestNo[i])
		{
			nParamCount=i;
			i=MAXPARAMETERS;
		}
	}
	if ( nConfig.Specks.Type == 1 )
		nConfig.Specks.Type = atoi(nConfig.szNoofpairs);
	i=0;

    //if(nParamCount==SRNO_ZMS) //03052001
    //Below condition will set status bar on Parameter Zch when Zch
    //is to be first parameter to be tested.
    if(nParamCount>SRNO_ZCAL)//Modified by:sdt:13082001
    {
        if(nTestNo[SRNO_ZMS]) //Added by:sdt:13082001
			CtPair.nTest=SRNO_ZCAL;
    }
    else
		CtPair.nTest=nParamCount;
	CtPair.Pair1=1;
	//if(bSingleStep==TRUE)
	//Modified by:sdt:27092005:1528
	if(runSetup.bSingleStep==TRUE)
		CtPair.Pair1=cSinglePair1;

    if(nTestNo[SRNO_CR])
    {
		//DisplayArray[SRNO_CR] = nConfig.Max[SRNO_CR];//Commented by:sdt:16102005:1840 //As it is no longer used
		//DisplayArray[SRNO_LR] = nConfig.Max[SRNO_LR];//Commented by:sdt:16102005:1840 //As it is no longer used
		//DisplayArray[SRNO_RU] = nConfig.Max[SRNO_RU];//Commented by:sdt:16102005:1840 //As it is no longer used
    }
    //Added by:sdt:07052001
    if(nTestNo[SRNO_CR])
		CRTestedFlag=1;
    else
	    CRTestedFlag=0;

	//graphicsinit();//Commented:sdt:27092005:1515


	do
	{
		nCurrentReadingno=0;
		switch(i)
		{
			case SRNO_CR:   if(nTestNo[SRNO_CR])
							{
								nReturnVal=CrTest();   //13072009
							}
							break;

			case SRNO_CM:   if(nTestNo[SRNO_CM])
								nReturnVal=CmTest();   //13072009
							break;
			//Commented by:sdt:30092005:2010 removed for quad software.
			//case SRNO_GM:   if(nTestNo[SRNO_GM])
			//					nReturnVal=GmTest();
			//				break;


			case SRNO_CUPG:   if(nTestNo[SRNO_CUPG])
								nReturnVal=CupgTest(); //13072009
							break;
			//Commented by:sdt:30092005:2010 removed for quad software.
			//case SRNO_CUPS:   if(nTestNo[SRNO_CUPS])
			//					nReturnVal=CupsTest();
			//				break;


			case SRNO_CUPP:   if(nTestNo[SRNO_CUPP])
								nReturnVal=CuppTest(); //Commented termporary by:sdt:23112005:1945
							break;
			case SRNO_CUPPI:   if(nTestNo[SRNO_CUPPI])
								nReturnVal=CUPPITest();//13072009
							break;

			case SRNO_ATN:   if(nTestNo[SRNO_ATN])
							 {
								//if(HFCalibrateFlag==FALSE)   //added by:sdt:31072001
								//CalibrateHFParam();
								nReturnVal=AtnTest();//13072009
							 }
							break;

			case SRNO_ELFEXT:   if(nTestNo[SRNO_ELFEXT])
								{
									//if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
									//CalibrateHFParam();
									//nReturnVal=ElfextTest();
								}
							break;

			case SRNO_NEXT:   if(nTestNo[SRNO_NEXT])
							  {
								//if(HFCalibrateFlag==FALSE)    //added by:sdt:31072001
								//	CalibrateHFParam();
								nReturnVal=NextTest();//13072009
							  }
							break;
			case SRNO_ZCAL:     //Condition Added by:sdt:20062001
								//Commented by:sdt:01102005:2245
								//For quad software ZCH is a calculated parameter.
								/*if(!strcmp(szHfZchMode,"CAL")&&!strcmp(szLfZchMode,"MEAS"))
								{
									//Commented by:sdt:01082001
									//if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
									//CalibrateHFParam();
									//This condition moved in next if {}
									//because when no HF parameter is
									//selected then also it does the
									//calibration.


									if(nTestNo[SRNO_ZMS])
									{
										if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
											CalibrateHFParam();
										nReturnVal=ZmsTest();
									}

									if(nTestNo[SRNO_ZCAL])
									{
										if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
											CalibrateHFParam();
										nReturnVal=ZCalTest();
									}
								}
								else
								{
									//Commented by:sdt:01082001
									//if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
									//CalibrateHFParam();
									//This condition moved in next if {}
									//because when no HF parameter is
									//selected then also it does the
									//calibration.

									if(nTestNo[SRNO_ZCAL])
									{
										//Added by:sdt:01082001
										if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
											CalibrateHFParam();
										nReturnVal=ZCalTest();
									}

								//Added:jj:05042001
								   if(nTestNo[SRNO_ZMS])
								   {
										//Added by:sdt:01082001
										if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
											CalibrateHFParam();
										nReturnVal=ZmsTest();
								   }
								}*/
							if(nTestNo[SRNO_ZCAL])
							{
								nReturnVal=ZCalTest();   //13072009
							}
							break;

			//Added:jj:03062001
			case SRNO_FEXT:   if(nTestNo[SRNO_FEXT])
							  {
								//if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
								//	CalibrateHFParam();

								nReturnVal=FextTest();//13072009
							  }
							break;
			case SRNO_INEXT:   if(nTestNo[SRNO_INEXT])
							   {
								//if(HFCalibrateFlag==FALSE) //added by:sdt:31072001
								//	CalibrateHFParam();

								//nReturnVal=InextTest();
							   }
							break;


			default:		break;
		}
		if(nReturnVal==-1)
		 i=MAXPARAMETERS;
		i++;
		itoa (nTempUnitOf ,nConfig.szUnitof ,10);//san 21-11-97 For appending of the remaining pairs
	}while(i<MAXPARAMETERS);//-1 by:sdt:30092005:2135 for Quad software

	//Added by:sdt:04112005:1500
	//This will restore default frequencies in nConfig variables
	//which might be chaged during different parameters.
	nConfig.nLFrequency=LFFrequency.nDefualtFreq;//1Khz
	nConfig.nHFrequency=HFFrequency.nDefualtFreq;//150Khz

	WriteInfo();

	//Added by:sdt:14082001//This code will restore the specification
	//after competing testing of selected parameter or if system
	//is reseted while testing of parameter is going on.
	//nSpeckFileNo=atoi(nConfig.Specks.specksnumber);
	//Modified by:sdt:27092005:1525
	//nSpeckFileNo = nConfig.Specks.nSpeckFileNumber;//Commented by:sdt:27092005:1530
	//freadspeck(&speck);
	readSpeck(speckData,nConfig.Specks.nSpeckFileNumber);

	if(nReturnVal==-1)
	{
		 fs = fs_inprocess;
		 fcloseall ();
		 CtPair.nTest = 0;
		 //graphicsinit();//Commented by:sdt:29052005:1525
		 ResetSystem ();
		 HFCalibrateFlag=FALSE;//Added by:sdt:13082001.
		 //This will reset flag for calibration for HF parameter.
	}
	else
	{
		 //WriteInfo();
		 fs = fs_executed;
	}
	//if(m_FileInfo.nLength>=RAMPINI.nLength) //Commented by:sdt:18122019:1800
	//	 ExchangeRampInfo(FALSE);            //Commented by:sdt:18122019:1800
}

void SetInfo(void)
{
	struct date d;
	struct time t;
	//int    start = 2;//Commented by:sdt:07052001

	getdate(&d);
	gettime(&t);
	strcpy(m_FileInfo.szHeader,
			// "Multitest (c) 1998 Sivananda Electronics, Nashik");  //header
			 "ePoint Technologies, Nashik");  //header
	m_FileInfo.cDummyEnd=26;                   //End of file(dummy)
	m_FileInfo.cVersion=0x10;                  //version
	m_FileInfo.FileOpenDate.cDay=d.da_day;       //File open date
	m_FileInfo.FileOpenDate.cMon=d.da_mon;
	m_FileInfo.FileOpenDate.nYear=d.da_year;

	m_FileInfo.FileOpenTime.cHour=t.ti_hour;     //File open time
	m_FileInfo.FileOpenTime.cMin=t.ti_min;
	m_FileInfo.FileOpenTime.cSec=t.ti_sec;
	strcpy(m_FileInfo.szCableType,nConfig.Specks.cabletype);
	strcpy(m_FileInfo.szSpecificationNo,nConfig.Specks.specksnumber);
	m_FileInfo.fCableGuage=atof(nConfig.Specks.guage);

	m_FileInfo.cNoofPairs=atoi(nConfig.szNoofpairs);   //Number of pairs
	m_FileInfo.cUnitof=atoi(nConfig.szUnitof);         //Unit of
	m_FileInfo.nLength=atoi(nConfig.szLength);         //Cable length
	m_FileInfo.nIlm=atoi(nConfig.szIlm);         		//Cable inital length Mark //Added by:sdt:09102005:1330
	m_FileInfo.nFlm=atoi(nConfig.szFlm);         		//Cable final length mark //Added by:sdt:09102005:1330
	m_FileInfo.fRoomtemp=atof(nConfig.szRoomtemp);     //Room Temperature
	m_FileInfo.cStartFixPos=atoi(nConfig.szFixPos);    //Start fix position

	if(strcmpi(nConfig.szFixtures,"Inner")==0)
		m_FileInfo.cFixtureNo=0;
	else if(strcmpi(nConfig.szFixtures,"Outer")==0)
		m_FileInfo.cFixtureNo=1;
	else
		m_FileInfo.cFixtureNo=2;

	gettime(&t);
	strcpy(m_FileInfo.nSpeckId,nConfig.szSpeckName);        //Specification number
	m_FileInfo.StartTime.cHour=t.ti_hour;       //Start time
	m_FileInfo.StartTime.cMin=t.ti_min;
	m_FileInfo.StartTime.cSec=t.ti_sec;
	//This two paramters taken directly from Default.INI i.e. kept in Titles
	//strcpy(m_FileInfo.szDrumNo,nConfig.szDrumNo); //Super Unit number
	//strcpy(m_FileInfo.szTestedBy,nConfig.szTestedBy);   //Tested by
}

void WriteInfo(void)
{
	struct date d;
	struct time t;

	getdate(&d);
	gettime(&t);
	m_FileInfo.StopTime.cHour=t.ti_hour;       //Stop time
	m_FileInfo.StopTime.cMin=t.ti_min;
	m_FileInfo.StopTime.cSec=t.ti_sec;
	m_FileInfo.SaveTime.cHour=t.ti_hour;       //Save time
	m_FileInfo.SaveTime.cMin=t.ti_min;
	m_FileInfo.SaveTime.cSec=t.ti_sec;
	m_FileInfo.SaveDate.cDay=d.da_day;         //Save date
	m_FileInfo.SaveDate.cMon=d.da_mon;
	m_FileInfo.SaveDate.nYear=d.da_year;
}


void SaveFile()
{
	char szTmp[64];//, szTmp1[64];
	unsigned int nfileno;

	nfileno = atoi (nConfig.szFileName);
	//nfileno = ReadStartup ();
	m_FileInfo.nFileNumber=nfileno;
	//Added by:sdt:06102005:2025
	sprintf (szTmp, "%s:\\%s",cRamDrive,nConfig.szFileName);//22012001
	PrepareFile(szTmp);

	//sprintf (szTmp,"copy g:\\%s %d.use > message",nConfig.szFileName, nfileno);//Commented:jj:18012001:1025

//Added:start:sdt:17092001:1510
#ifdef SECURITY
   if(onexit())
   {
		//Commented by:sdt:06102005:2025
		//sprintf (szTmp,"copy %s:\\%s %s\\%d.use > message",cRamDrive,nConfig.szFileName, DataDirectory, nfileno);
		//system (szTmp);
		//Added by:sdt:06102005:2025
		sprintf (szTmp, "%s\\%d.use",DataDirectory,nfileno);
		PrepareFile(szTmp);

		itoa(nfileno, nConfig.szFileName, 10);
		nfileno++;
		WriteStartup(nfileno);
		fs = fs_saved;
   }
   else
		fs = fs_notsaved;
#else
	//Commented by:sdt:06102005:1900
	//sprintf (szTmp,"copy %s:\\%s %s\\%d.use > message",cRamDrive,nConfig.szFileName, DataDirectory, nfileno);
	//system (szTmp);//commented by:sdt:06102005:0745
	//Added by:sdt:06102005:1900
	//sprintf(szTmp,"%s:\\%s",cRamDrive,nConfig.szFileName);
	//sprintf(szTmp1,"%s\\%d.use",DataDirectory,nfileno);
	//execlp("copy.exe", "copy.exe", szTmp, szTmp1, NULL);
	//Added by:sdt:06102005:2025
	sprintf (szTmp, "%s\\%d.use",DataDirectory,nfileno);
	PrepareFile(szTmp);

	itoa(nfileno, nConfig.szFileName, 10);
	nfileno++;
	WriteStartup(nfileno);
	fs = fs_saved;
#endif
//end:sdt:17092001:1510
	RemoveFile();
	//Added by:sdt:14072009:1205:Start
	nRetestflag=0;
	for(int i=0;i<MAXPARAMETERS;i++)
		ulParamRunSetup[i]=0;
	//Added by:sdt:14072009:1205:End
	//currentdisplay(0, -1);//Commented by:sdt:06102005:0625

	return;
}


FILE * PrepareFile(char *szFileName)
{
	FILE *fpSummary;//Added by:sdt:04092001:1250
	SUMMARYINFO SummaryInfo;//Added by:sdt:04092001:1250
	SPECKLIMIT specksLimit;//Added by:sdt:02092001
	FILE *fpLimitFile;//Added by:sdt:02092001
	FILE *fpData;//Added:jj:18052001

	char    	szbuf[20];
	READING 	Reading;
	FILE    	*fpCurFile;
	struct 		stat statbuf;
	int			nLRSelected,nRUSelected;
	fcloseall();
	//sprintf (szbuf, "%s:\\\%s",cRamDrive,nConfig.szFileName);//22012001
	//remove(szbuf);
	//if ((fpCurFile = fopen(szbuf, "wb")) == NULL )
	//   return NULL;
	remove(szFileName);
	if ((fpCurFile = fopen(szFileName, "wb")) == NULL )
	   return NULL;

	nLRSelected=GetPrivateProfileInt("REPORT","LR",0,
											"default.INI");//22012001
	nRUSelected=GetPrivateProfileInt("REPORT","RU",0,
											"default.INI");
	//Added by:sdt:23032001
	if(nLRSelected)
	{
		for(int i=0;i<MAXFREQUENCIES;i++)
		{
        	m_FileInfo.cParamTested[SRNO_LR][i]=m_FileInfo.cParamTested[SRNO_CR][i];
        }
    }
    else
    {
    	for(int i=0;i<MAXFREQUENCIES;i++)
        {
        	m_FileInfo.cParamTested[SRNO_LR][i]=0;
        }
    }

    //Added by:sdt:23032001
    if(nRUSelected)
    {
    	for(int i=0;i<MAXFREQUENCIES;i++)
        {
        	m_FileInfo.cParamTested[SRNO_RU][i]=m_FileInfo.cParamTested[SRNO_CR][i];
        }
	}
	else
	{
		for(int i=0;i<MAXFREQUENCIES;i++)
		{
			m_FileInfo.cParamTested[SRNO_RU][i]=0;
		}
	}
	fwrite (&nConfig,sizeof(nConfig),1,fpCurFile);
	fwrite (&m_FileInfo,sizeof(m_FileInfo),1,fpCurFile);
	//fwrite (&speck,sizeof(speck),1,fpCurFile);//Commented by:sdt:06102005:0630
	fwrite(&runSetup,sizeof(runSetup),1,fpCurFile); //Added by:sdt:06102005:0630
	fwrite (&speckData,sizeof(speckData),1,fpCurFile);//Added by:sdt:06102005:0630

	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
	{
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		exit( 1 );
	}
	for( int j = 0;j < MAXPARAMETERS; j++)
	{
		//ReadParamDotCts( paraminfo1[j].szParamAbr, &prmarray );
		//Modified by:sdt:06102005:0710
		ReadParamDotCts( paraminfo[j].szParamAbr, prmarray );
		fwrite (prmarray,sizeof(PRM),1,fpCurFile);
	}
	close( ParamCtsHandle );
	fwrite (&CtPair,sizeof(CtPair),1,fpCurFile);
	fwrite (m_BasicInfo.cSerialNo,sizeof(m_BasicInfo.cSerialNo),1,fpCurFile);
//	fwrite (&Decimal,sizeof (Decimal),1,fpCurFile);
	fwrite (&nTempUnitOf,sizeof(nTempUnitOf),1,fpCurFile);
	fwrite (&nLastUnitSize,sizeof(nLastUnitSize),1,fpCurFile);
	fwrite (&nUnits,sizeof(nUnits),1,fpCurFile);
	fwrite (&nNeedOfAppend,sizeof(nNeedOfAppend),1,fpCurFile);

	//Added:jj:09042001
	//fwrite (&ZchHfAray,sizeof(ZchHfAray),1,fpCurFile);//Commented by:sdt:06102005:0630

	//Added:jj:02052001
	fwrite (&Param,sizeof(Param),1,fpCurFile);


	//Added by:sdt:08052001//Commented by:sdt:06062001
	//fwrite (&WorstElfext,sizeof(WorstElfext),1,fpCurFile);
	//fwrite (&WorstOnext,sizeof(WorstOnext),1,fpCurFile);

	//Added by:sdt:06062001
	//fwrite (&Worstpairnext,sizeof(WORSTPAIRNEXT),1,fpCurFile);
	//Modified by:sdt:04022007:1630
	fwrite (&Worstpairnext,sizeof(Worstpairnext),1,fpCurFile);
	//Added:jj:21052001
	fwrite (&SpeckList,sizeof(SpeckList),1,fpCurFile);

	//fwrite (&szOriginalParams,sizeof(szOriginalParams),1,fpCurFile); //Commented by:sdt:06102005:0630
	//Added by:sdt:02092001
	//This code will writes the limit in .USE FIle.
	//start
	sprintf(szbuf,"%s:\\limits.bin",cRamDrive);
	fpLimitFile=fopen(szbuf,"rb");
	memset(&specksLimit,0,sizeof(SPECKLIMIT));
	if(fpLimitFile!=NULL)
	{
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)
		{
			for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
			{
				for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
				{
					fread(&specksLimit, sizeof (SPECKLIMIT), 1, fpLimitFile);
					fwrite (&specksLimit, sizeof (SPECKLIMIT), 1, fpCurFile);
				}
			}
		}
	}
	if(fpLimitFile){fclose(fpLimitFile);}
	//End
	//Added by:sdt:04092001:1450
	//This Loop will write the Summary values present in "Summary.bin"
	//into the ".USE" File.
	//start
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
	sprintf(szbuf,"%s:\\summary.bin",cRamDrive);
	fpSummary=fopen(szbuf,"rb");
	if(fpSummary!=NULL)
	{
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)
		{
			for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
			{
				for(int nSumType=0;nSumType<MAXSUMMARYTYPES;nSumType++)
				{
					fread(&SummaryInfo, sizeof(SUMMARYINFO),1,fpSummary);
					fwrite(&SummaryInfo,sizeof(SUMMARYINFO),1,fpCurFile);
				}
			}
		}
	}
	if(fpSummary){fclose(fpSummary);}
	//end
	for( int i = 0;i < MAXPARAMETERS; i++)
	{
		for (int j=0;j<MAXFREQUENCIES;j++)
		{
			sprintf(szbuf, "%s:\\%dFREQ_%d.dat",cRamDrive,i,j);//22012001

			//Modified:jj:18052001
			//if( (fpData[i][j] = fopen(szbuf, "rb")) == NULL)
			if( (fpData = fopen(szbuf, "rb")) == NULL)

				continue;

			//Modified:jj:18052001
			//int handle = fileno (fpData[i][j]);
			int handle = fileno (fpData);

			fstat (handle, &statbuf);
			if (statbuf.st_size == 0)
			{
				//Modified:jj:18052001
				//if(fpData[i][j]){fclose(fpData[i][j]);}
				if(fpData){fclose(fpData);}

				//nTested[i] = 0;
				m_FileInfo.cParamTested[i][j] = 0;
				continue;
			}

			//Modified:jj:18052001
			//while(fread(&Reading,sizeof(Reading),1,fpData[i][j]) > 0)
			while(fread(&Reading,sizeof(Reading),1,fpData) > 0)

				fwrite(&Reading,sizeof(Reading),1,fpCurFile);

			//Modified:jj:18052001
			//if(fpData[i][j]){fclose(fpData[i][j]);}
			if(fpData){fclose(fpData);}

		}
	}
	if(fpCurFile){fclose(fpCurFile);}
	return fpCurFile;
}


void RemoveFile()
{
	char szbuf[100];
	fcloseall();
	for( int i = 0;i < MAXPARAMETERS; i++)
	{
		for (int j=0;j<MAXFREQUENCIES;j++)
        {
			sprintf(szbuf, "%s:\\%dFREQ_%d.dat",cRamDrive,i,j);
			remove(szbuf);
        }
	}
	sprintf (szbuf, "%s:\\%s", cRamDrive,nConfig.szFileName);
	remove(szbuf);
	remove("message");
}


void createReport(void)
{
	char path[80];
	char 	szAddress[50];

	sprintf(path,"%s:\\sat.txt",cRamDrive);//22012001
	remove (path);
	if (fs == fs_open||fs == fs_closed)
	{
		for (int i = 0;i < fname.count;)
		{
			//Commented by:sdt:08102005:1615
			//sprintf (szTmp, "Please wait preparing Report");//added by:sdt:03042001
			//messagewindow (szTmp);
			for (int j = 0; j < MAXPARAMETERS+1; j++)
				m_RepSettings.nParams[j]=PARAMEND;
			if (GetConfigStructure (fname.name[i]) == fs_error)
			{
				i++;
				continue;
			}
			PrepareFileForPrint ();
			sprintf (szAddress,"%p",&m_RepSettings);
			if ( nConfig.Specks.Type )
			{
				//sprintf (szTmp,"disprep1 %s %s g:\\\sat.txt %s %s %s %s",szAddress,fname.name[i], runSetup.szBypassPairs,Text1,Text2,Text3);
				spawnlp(P_WAIT,"disprep1.exe","disprep1.exe",szAddress,fname.name[i],"g:\\sat.txt",Text1,Text2,Text3,NULL);
			}
			else
			{
				sprintf(path,"%s:\\sat.txt",cRamDrive);
				spawnlp(P_WAIT,"disprep.exe","disprep.exe",szAddress,fname.name[i],path,Text1,Text2,Text3,NULL);
				//sprintf (szTmp,"disprep %s %s %s:\\sat.txt %s %s %s %s",szAddress,fname.name[i],cRamDrive,runSetup.szBypassPairs,Text1,Text2,Text3);
				//system (szTmp);
			}
			//Commented by:sdt:08102005:2305
			//closegraph(); //Added by:sdt:31032001
			//sprintf(path,"view %s:\\sat.txt",cRamDrive);//22012001
			//system (path);

			//sprintf(path,"%s:\\\sat.txt",cRamDrive);
			//remove (path);

			//InitialiseGraphics();
			//clearmenu();
			//Added by:sdt:09052001
			if(LoadFlag)
			{
				for (int i = 0;i < MAXPARAMETERS;i ++)
				{
					//DisplayArray[i] = 0;//Commented by:sdt:16102005:1840 //As it is no longer used
					for(int j=0;j<MAXFREQUENCIES;j++)
					{
						m_FileInfo.nReadingsOk[i][j]=0;
						m_FileInfo.nReadingsNotOk[i][j]=0;
						m_FileInfo.cParamTested[i][j]=FALSE;
					}
				}
			}
			//graphicsinit();
			i++;
			//Commented Temporary by:sdt:08102005:1735
			//while (getkey () != 0);
			//while (i < fname.count)
			//{
			//	sprintf (szTmp, "view %s : Esc->Abort N->No Enter->Yes", fname.name[i]);
			//	messagewindow (szTmp);
			//	while ((key = getkey ()) == 0);
			//	if (key == ESCAPE)
			//		return;
			//	else if (key == ENTER)
			//		break;
			//	else if (key == 'n' || key == 'N')
			//		i++;
			//}
		}
	}
	else
	{
		for (int j = 0; j < MAXPARAMETERS+1; j++)
			m_RepSettings.nParams[j]=PARAMEND;
		if (fs == fs_executed || fs == fs_notsaved || fs == fs_bad || fs == fs_inprocess)
		{
			sprintf (path, "%s:\\%s",cRamDrive,nConfig.szFileName);//22012001
			PrepareFile(path);
		}
		PrepareFileForPrint ();
		sprintf (szAddress,"%p",&m_RepSettings);

		char szFileName[MAXPATH];//Added:pnb:jj:28122000:1410
		sprintf(szFileName," %s\\%s", DataDirectory, nConfig.szFileName );//Added:pnb:jj:28122000:1410//Modified:jj:18012001:1200


		if (fs == fs_saved)
		{
			if ( nConfig.Specks.Type )
			{
			  //sprintf (szTmp,"disprep1 %s %s.use g:\\\sat.txt %s %s %s %s",szAddress, szFileName, runSetup.szBypassPairs,Text1,Text2,Text3);//Modified:jj:18012001:1055
			  sprintf(path,"%s.use",szFileName);
			  spawnlp(P_WAIT,"disprep1.exe","disprep1.exe",szAddress,path,"g:\\sat.txt",Text1,Text2,Text3,NULL);
			}
			else
			{
			  //sprintf (szTmp,"disprep %s %s.use %s:\\\sat.txt %s %s %s %s",szAddress, szFileName,cRamDrive,runSetup.szBypassPairs,Text1,Text2,Text3);//Modified:jj:18012001:1055
			  sprintf(path,"%s.use",szFileName);
			  char path1[80];
			  sprintf(path1,"%s:\\sat.txt",cRamDrive);
			  spawnlp(P_WAIT,"disprep.exe","disprep.exe",szAddress,path,path1,Text1,Text2,Text3,NULL);
			}
		}
		else
		{
			if ( nConfig.Specks.Type )
			{
				//sprintf (szTmp,"disprep1 %s g:\\\%s g:\\\sat.txt %s %s %s %s",szAddress, nConfig.szFileName, runSetup.szBypassPairs,Text1,Text2,Text3);
				sprintf(path,"g:\\%s",nConfig.szFileName);
				spawnlp(P_WAIT,"disprep1.exe","disprep1.exe",szAddress,path,"g:\\sat.txt",Text1,Text2,Text3,NULL);
			}
			else
			{
				//sprintf (szTmp,"disprep %s %s:\\\%s %s:\\\sat.txt %s %s %s %s",szAddress,cRamDrive,nConfig.szFileName,cRamDrive,runSetup.szBypassPairs,Text1,Text2,Text3);
				sprintf(path,"%s:\\%s",cRamDrive,nConfig.szFileName);
				char path1[80];
				sprintf(path1,"%s:\\sat.txt",cRamDrive);
				spawnlp(P_WAIT,"disprep.exe","disprep.exe",szAddress,path,path1,Text1,Text2,Text3,NULL);
			}
		}

		//system (szTmp);//Commented by:sdt:14102005:1110
		//Commented by:sdt:08102005:2300
		//closegraph ();
		//sprintf(path,"view %s:\\\sat.txt",cRamDrive);//22012001
		//system (path);
		//sprintf(path,"%s:\\\sat.txt",cRamDrive);//22012001
		//remove (path);
		//InitialiseGraphics();
	}
	//clearmenu();
	//graphicsinit();
	//messagewindow("F10: Main Menu    F6: Quit    F7: Reset");

}




FileStatus GetConfigStructure (char *caption)
{
	char FileName[MAXPATH];//Added:jj:18012001:1120
	FILE *t_File;
	char *p;
	//int i;
	int done;
	struct ffblk ffblk;
	//char szParamSel[30];
	//char szTmp[2];

	//strcpy(szParamSel,paramnostr);//Commented by:sdt:08102005:1820
	caption = strupr (caption);
	p = strtok (caption, ".");
	strcpy (nConfig.szFileName, p);

	if (strstr (caption, ".USE") == NULL)
		strcat (caption, ".use");

	if (strcmp (strupr(caption), ".USE") == 0)
		strcpy (caption, " ");

	//Start:jj:18012001:1110
	char *m = strstr (caption,DataDirectory );
	if ( m == NULL )
		sprintf(FileName, "%s\\%s",DataDirectory, caption );
	else
		sprintf(FileName, "%s", caption );
	//End:jj:18012001:1110

	done = findfirst(FileName,&ffblk,0);//Modified:jj:18012001:1110 caption to FileName
	if (done != 0)
	{
		//messagewindow ("File Not Found");
		//getch ();
		//messagewindow("F10: Main Menu    F6: Quit    F7: Reset");
		messageBox("File Not Found", mfError|mfOKButton );
		fs = fs_error;
		return fs_error;
	}
	//strcpy(fname.name[0],caption);//Commented:jj:18012001:1110
	 strcpy(fname.name[0],FileName);//Modified:jj:18012001:1110

	fname.count = 1;
	//t_File = fopen (caption,"rb");//Commented:jj:18012001:1110
	 t_File = fopen (fname.name[0],"rb");//Modified:jj:18012001:1110

	fseek (t_File, 0, SEEK_SET);
	fread (&nConfig,sizeof(CONFIG),1,t_File);
	fread (&m_FileInfo,sizeof(INFORMATION),1,t_File);
	//MOD02022002:01:sdt:1015:start
	//fread (&speck,sizeof(speck),1,t_File);//Commented by:sdt:08102005:1630
	fread(&runSetup,sizeof(runSetup),1,t_File); //Added by:sdt:06102005:0630
	fread (&speckData,sizeof(speckData),1,t_File);//Added by:sdt:06102005:0630

	for( int j = 0;j < MAXPARAMETERS; j++)
	{
		fread (prmarray,sizeof(PRM),1,t_File); //sizeof(prmarray) modified by:sdt:01112005:1630
	}
	fread (&CtPair,sizeof(CtPair),1,t_File);
	fread (m_BasicInfo.cSerialNo,sizeof (m_BasicInfo.cSerialNo),1,t_File);
	fread (&nTempUnitOf,sizeof(nTempUnitOf),1,t_File);
	fread (&nLastUnitSize,sizeof(nLastUnitSize),1,t_File);
	fread (&nUnits,sizeof(nUnits),1,t_File);
	fread (&nNeedOfAppend,sizeof(nNeedOfAppend),1,t_File);
	//fread (&ZchHfAray,sizeof(ZchHfAray),1,t_File); //Commented by:sdt:08102005:1635
	fread (&Param,sizeof(Param),1,t_File);
	//fread (&Worstpairnext,sizeof(WORSTPAIRNEXT),1,t_File);
	//Modified by:sdt:04022007:1630
	fread (&Worstpairnext,sizeof(Worstpairnext),1,t_File);
	fread (&SpeckList,sizeof(SpeckList),1,t_File);
	//fread (&szOriginalParams,sizeof(szOriginalParams),1,t_File); //Commented by:sdt:08102005:1635
	//MOD02022002:01:sdt:1015:end
	//strcpy (nConfig.szDrumNo,m_FileInfo.szDrumNo);//Commented by:sdt:08102005:1635
	//strcpy (nConfig.szTestedBy,m_FileInfo.szTestedBy);//Commented by:sdt:08102005:1635

	sprintf (nConfig.szFixPos,"%d",m_FileInfo.cStartFixPos);
	if(m_FileInfo.cFixtureNo==0)
		strcpy(nConfig.szFixtures,"Inner");

	if(m_FileInfo.cFixtureNo==1)
		strcpy(nConfig.szFixtures,"Outer");

	if(m_FileInfo.cFixtureNo==2)
		strcpy(nConfig.szFixtures,"Both");

	sprintf (nConfig.szSpeckName,"%s",m_FileInfo.nSpeckId);
	sprintf (nConfig.szNoofpairs,"%d",m_FileInfo.cNoofPairs);
	sprintf (nConfig.szUnitof,"%d",m_FileInfo.cUnitof);
	sprintf (nConfig.szRoomtemp,"%2.2f",m_FileInfo.fRoomtemp);
	sprintf (nConfig.szLength,"%u",m_FileInfo.nLength);//SDT11122016:0800

	if(t_File){fclose(t_File);}
	fs = fs_open;
	return fs_open;
}


void PrepareFileForPrint()
{
	char path[80];
	int j=0;//nCount,ntmp;
	//char nTmp[10];
	//int x=0;
	//Commented by:sdt:08102005:1705
	//if (fs == fs_bad || fs == fs_inprocess || fs == fs_notsaved || fs == fs_executed || fs == fs_saved)
	//{
	//	char szParamSel[30];
	//	char szTmp[10];//Array size increased to 3:jj:17042001
		//Array size increased to 10:jj:27042001
		//MOD01022002:01:sdt
	//	if(strlen(paramnostr)==0)
	//	{
	//		strcpy(paramnostr,szOriginalParams);
	//	}
	//}
	for(int i=0;i<MAXPARAMETERS;i++)
	{
		if((runSetup.ulParamRunSetup[i]&PARAMSELECTIONMASK)!=0)
		{
			for(int k=0;k<MAXFREQUENCIES;k++)
			{
				if (m_FileInfo.cParamTested[i][k]!=TRUE)
					continue;
				else
				{
					//Condition Added by:sdt:16102005:1430
					if(reportSetup.m_Mode & (0x01<<i))
					{
						m_RepSettings.nParams[j++]=i;
					}
					break;
				}
			}
			//Condition uncommneted by:sdt:03112006:2115
			//This code required to include Parameter RU in report.
			//Condition commented by:sdt:16102005:1445
			if(i == SRNO_CR)
			{
				//Uncommented by:sdt:03112006:2120
				//Following for loop commented by:sdt:14102005:2325
				//This loop commented to avoid RU parameter in report
				//Commented by:sdt:09112006:2320
				//This was causes ptinting / display of RU parameters
				//two times in a report. hence this part is commented.
				/*for(int k=0;k<MAXFREQUENCIES;k++)
				{
					if (m_FileInfo.cParamTested[SRNO_RU][k]!=TRUE)
						continue;
					else
					{
						m_RepSettings.nParams[j++]=SRNO_RU;
						break;
					}
				}*/
				//Commented by:sdt:03112006:2120
				//Added by:sdt:23032001
				//for(int l=0;l<MAXFREQUENCIES;l++)
				//{
				//	if (m_FileInfo.cParamTested[SRNO_LR][l]!=TRUE)
				//		continue;
				//	else
				//	{
				//		//Condition Added by:sdt:16102005:1430
				//		if(reportSetup.m_Mode & (0x01<<SRNO_LR))
				//		{
				//			m_RepSettings.nParams[j++]=SRNO_LR;
				//		}
				//		break;
				//	}
				//}
			}

		}
	}

	sprintf(path,"%s:\\disprep.ini",cRamDrive);
	m_RepSettings.nGroupRep = GetPrivateProfileInt("REPORT","GROUP_REP",0,path);
	//=ntmp;

	m_RepSettings.nNormUnnorm[SRNO_RU] = 0;

	//if(!strcmp(summarystr,"Detailed"))
	//Condition modified by:sdt:16102005:1430
	if(reportSetup.m_SumDet==True)
	{
		m_RepSettings.nSumm_Detailed=SUM_DETAILED;
		nReport=DETAILEDREP;
	}
	else
	{
		m_RepSettings.nSumm_Detailed=SUM_SUMMARY;
		nReport=SUMMARYREP;
	}

	//if(!strcmp(remarkstr,"Yes"))
	//Condition modified by:sdt:16102005:1430
	if(reportSetup.m_Remark==True)
		m_RepSettings.nRemark=TRUE;
	else
		m_RepSettings.nRemark=FALSE;

	//if(!strcmp(normalstr,"Both"))
	//Condition modified by:sdt:16102005:1430
	if((reportSetup.m_Type==0x03)||(reportSetup.m_Type==0x00))
		for (j=0;j<MAXPARAMETERS+1;j++)
			m_RepSettings.nNormUnnorm[j]=SUM_BOTH;
	//else if(!strcmp(normalstr,"Normalised"))
	//Condition modified by:sdt:16102005:1430
	else if(reportSetup.m_Type==0x01)
		for (j=0;j<MAXPARAMETERS+1;j++)
			m_RepSettings.nNormUnnorm[j]=SUM_NORMALISED;

	//else if(!strcmp(normalstr,"Unnormalised"))
	//Condition modified by:sdt:16102005:1430
	else if(reportSetup.m_Type==0x02)
	{
		for (j=0;j<MAXPARAMETERS+1;j++)
			m_RepSettings.nNormUnnorm[j]=SUM_UNNORMALISED;
		m_RepSettings.nRemark=FALSE;
	}
	m_RepSettings.nNormUnnorm[SRNO_RU] = 0;
}

//Added by:sdt:17102005:1300
void printReport(void)
{
	int Result;
	char szFileName[MAXPATH];

	ushort helpCtx = TProgram::deskTop->helpCtx;
	TProgram::deskTop->helpCtx = hcReportPrint;
	TProgram::statusLine->update();
	TProgram::deskTop->helpCtx = helpCtx;

	if (fs == fs_error || fs == fs_executed)
	{
		messageBox( "This file cannot be printed", mfError|mfOKButton );
	}
	else if (fs == fs_open)
	{
		for (int i = 0; i < fname.count; i++)
		{
			if (GetConfigStructure (fname.name[i]) == fs_error)
				continue;
			Result = SendFileInQueue(fname.name[i]);
		}
	}
	else if (fs == fs_saved)
	{
	   char FileName[MAXPATH];//Added:jj:18012001:1105

		sprintf (szFileName,"%s.use",nConfig.szFileName);
	   //Start:jj:18012001:1105
		char *m = strstr (szFileName,DataDirectory );
	   if ( m == NULL )
			sprintf(FileName, "%s\\%s",DataDirectory, szFileName );
		else
			sprintf(FileName, "%s", szFileName );
			//End:jj:18012001:1105

		Result = SendFileInQueue(FileName);//Modified:jj:18012001:1105 szFileName to FileName
		if (Result == FALSE)
			messageBox( "Please Wait. Print Queue Overflow", mfError|mfOKButton );
	}
	else if (fs == fs_executed || fs == fs_notsaved || fs == fs_bad || fs == fs_inprocess)
	{
		char szString[80];
		if ( messageBox( "Do you want to save the file",
					mfYesButton|mfNoButton ) == cmYes )
		{
			SaveFile();
			sprintf( szString, "File Saved as %s\\%s", DataDirectory, nConfig.szFileName );//Added:jj:18012001:1045
			messageBox(szString,mfInformation|mfOKButton);
			RemoveFile();
		}
		else
		{
			fs = fs_notsaved;
			return;
		}
		//Commented by:sdt:25112005:2305
		//sprintf (szFileName, "%s:\\\%s",cRamDrive,nConfig.szFileName);
		//PrepareFile (szFileName);
		//Result = SendFileInQueue(szFileName);
		//Added by:sdt:25112005:2305
		char FileName[MAXPATH];//Added:jj:18012001:1105
		sprintf (szFileName,"%s.use",nConfig.szFileName);
	   //Start:jj:18012001:1105
		char *m = strstr (szFileName,DataDirectory );
	   if ( m == NULL )
			sprintf(FileName, "%s\\%s",DataDirectory, szFileName );
		else
			sprintf(FileName, "%s", szFileName );
			//End:jj:18012001:1105
		Result = SendFileInQueue(FileName);//Modified:jj:18012001:1105 szFileName to FileName
		if (Result == FALSE)
			messageBox( "Please Wait. Print Queue Overflow", mfError|mfOKButton );
	}
	else
	{
		messageBox( "No valid file available for printing", mfError|mfOKButton );
	}
	//messagewindow("F10: Main Menu    F6: Quit    F7: Reset");

}


int SendFileInQueue(char *szFileName)
{
	int i, j;//, Result;
	char szAddress[50];//szTmp[100];

	for(i=0;i<MAXFILEINQUEUE;i++)
	{
		for(j=0;j<MAXPARAMETERS+1;j++)
		m_RepSettings.nParams[j]=PARAMEND;
		PrepareFileForPrint ();
		sprintf (szAddress,"%p",&m_RepSettings);
		if ( nConfig.Specks.Type )
		{
			//sprintf (szTmp,"prnrep1 %s %s PRN %s %s %s %s",szAddress,szFileName, szBypassPairs,Text1,Text2,Text3);
			spawnlp(P_WAIT,"prnrep1.exe","prnrep1.exe",szAddress,szFileName,"PRN",Text1,Text2,Text3,NULL);
		}
		else
		{
			  //sprintf (szTmp,"prnrep %s %s PRN %s %s %s %s %s",szAddress,szFileName,cRamDrive,Text1,Text2, Text3,szBypassPairs);//Modified:jj:jp:18022001:1030
			  spawnlp(P_WAIT,"prnrep.exe","prnrep.exe",szAddress,szFileName,"PRN",cRamDrive,Text1,Text2,Text3,NULL);

		}
		return TRUE;
	}
	return FALSE;
}

void viewCalibrationReport(void)
{

	nDC=SCREEN;//Added:jj:13042001
	SetSpeckInfo();
	CalibrationReport();
	CtPair.nTest = 0;
}

int  CalibrationReport()
{
	int bHFCalFlag=0;
	//char szCaliSelectParam[MAXLEN_SELECT_PARAM], nTmp[5];//[2];Modified:jj:02052001
	static int nParCount;
	int nRead=1;
	char path[80];

	if(nDC==PRINTER)
	{//Modified into loop :jj:13042001
		fpTmp=fptr_Report;
		//messagewindow("Generating Calibration Report");//Temporarry commented by:sdt:08112005:2000
	}
	//else				//Temporarry commented by:sdt:08112005:2000
	//	closegraph();
	//strcpy(szCaliSelectParam,szRunSelectParam);
	if(nDC==SCREEN)
	{
		CalReport = (TView *)(new TWindow(TRect( 0, 0, 79, 22 ), "Calibration Report" ,wnNoNumber));
		TProgram::deskTop->insert(CalReport);
	}

	for(int i=0;i<MAXPARAMETERS;i++)
	{
		//Added by:sdt:09112005:1930 // ZCH is a calculated parameter.
		if(i==SRNO_ZCAL)
			continue;

		if((runSetup.ulParamRunSetup[i]&PARAMSELECTIONMASK)!=0)
		{
			nParCount = i;

			if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
			{
				clrscr();
				printf( "\nError Opening File PARAM.CTS File for Reading..." );
				//RemoveIniFiles();
				exit( 1 );
			}

			if(nParCount==SRNO_CR)
			{
				nConfig.Specks.speckparams[SRNO_CR].freq[LFFrequency.nDefualtFreq]=1;
				nConfig.Specks.speckparams[SRNO_LR].freq[LFFrequency.nDefualtFreq]=1;
				nConfig.Specks.speckparams[SRNO_RU].freq[LFFrequency.nDefualtFreq]=1;
			}


			ReadParamDotCts( paraminfo[nParCount].szParamAbr , prmarray);

			//Added by:sdt:07082001
			if(prmarray->nParamType ==1)
			{
				ReadParamDotCts( paraminfo[SRNO_ATN].szParamAbr, prmarray);
				nParCount = SRNO_ATN;
			}

			//Added:jj:26032001
			if ( toupper(prmarray->nTesting) != 'N' )
				ReadStdVal(nParCount);

			lseek ( ParamCtsHandle, 0L, SEEK_SET );
			lseek ( ParamCtsHandle, sizeof( PRM ) * noofparameters, SEEK_CUR );
			lseek ( ParamCtsHandle, sizeof( RdControl ) * nParCount, SEEK_CUR );
			nRead = read( ParamCtsHandle, &rdCtrl, sizeof(RdControl) );
			close( ParamCtsHandle );
			InitialiseCALarray();

			//if(nParCount >= SRNO_ATN) //Commented by:sdt:07082001
			if(prmarray->nParamType==1)  //Added by:sdt:07082001
				factor = 100;
			else
			if( nParCount == SRNO_CM && ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) ))
			{
				ExchangeRampInfo(TRUE);
				sprintf(path,"%s:\\cts.ini",cRamDrive);
				factor = GetPrivateProfileInt("COMPATIBILITY","RAMP_FACTOR",500,path);
			}
			else
			{
				sprintf(path,"%s:\\cts.ini",cRamDrive);
				factor = GetPrivateProfileInt("COMPATIBILITY","FACTOR",1000,path);
			}

			if ( (nParCount == SRNO_CUPP||nParCount == SRNO_CUPG)&&(atof(nConfig.szLength) >= 2501.0) ) //Delay exchnged
			{
				ExchangeDelayInfo (TRUE);
			}
			if(bHFCalFlag)//Added by:sdt:07082001
				continue;

			if(!GetCalibrationReportValues(nParCount))//Modified:jj:12042001
			{
				TProgram::deskTop->remove(CalReport);//Added by:sdt:10112005:1945
				TWindow::destroy (CalReport);//Added by:sdt:10112005:1945
				return FALSE;
			}
			if(nParCount ==SRNO_ATN)//Added by:sdt:07082001
				bHFCalFlag=1;
			if (nParCount==SRNO_NEXT || nParCount==SRNO_FEXT)
				break;

			if ( (nParCount == SRNO_CUPP||nParCount == SRNO_CUPG)&&(atof(nConfig.szLength) >= 2501.0) ) //Delay exchnged
			{
				ExchangeDelayInfo (FALSE);
			}

			if( nParCount == SRNO_CM && ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) ))
				ExchangeRampInfo(FALSE);
		}

	}
	if(nDC==SCREEN)//Added:jj:13042001
	{
		messageBox( "End of Calibration Report", mfOKButton );
		//EndCalDisplay();//Commented Temporary:sdt:08112005:2025
		//getch(); //Commented Temporary:sdt:08112005:2025
		//InitialiseGraphics();//Added:jj:12042001
		TProgram::deskTop->remove(CalReport);
		TWindow::destroy (CalReport);
	}
	//messagewindow(defaultmessage);//Commented Temporary:sdt:08112005:2025
		//return;
		return TRUE;
}





//Added:jj:18052001
int GetCalibrationReportValues(int nParC) //Modified for Multiple freq calibration 24032001:jj:
{
	int HFfreqSelected[MAXFREQUENCIES]; //Added by:sdt:07082001
	//30042001
	char path[80];
    char callow[5],calhigh[5];

	char buf[180];
    int key;

	int      i, nFreqIndex,nPow;
	float    fInstStd,fMeasuredStd,fDeviationPercentage,fDevFactor,fMinDevFactor,fMaxDevFactor;
	char     szRemark[10];
	int 	 nFreqNum;//Added:jj:24032001


	nParamCount = nParC;
	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);

	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	SelectSplitForCal();


	if (prmarray->nParamType == 0) //Lf
	{
		//for(nFreqNum=0;nFreqNum<LFFrequency.nTotalFrequencies;nFreqNum++)
		//Modified by:sdt:08112005:2230
		nFreqNum = LFFrequency.nDefualtFreq;
	    {
			if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum])
			{
				if(nDC==SCREEN)//Added:jj:13042001
				{
					InitCalDisplay(nFreqNum);//Added:jj:12042001
					Range_posX=X_OFFSET+5;
				}

				SetLowFreq(nFreqNum);
				//Post Frequency Delay Added by:sdt:03082001
			   	delay( rdCtrl.m_PostFreqDelay[nFreqNum] );
				nFreqIndex = nFreqNum;
				for(i=0;i<prmarray->nRanges;i++)
				{
                	if(nDC==SCREEN)//Added:jj:13042001
                    {
                		//Added:jj:12042001
                    	Range_posX+=12;
						//gotoxy(Range_posX, Range_posY);
						sprintf(buf,"Range %d",i+1);
						//cprintf(buf);
						CalReport->writeStr(Range_posX, Range_posY,buf,2);//Added by:sdt:09112005:2200
						X=Range_posX;
					}

					//Condition modified:jj:02052001
					if(nParamCount==SRNO_CM && i == 0 && nDC!=SCREEN)
						continue;
					if(nParamCount==SRNO_CUPP && i == 2 && nDC!=SCREEN)
						continue;
					if(nParamCount==SRNO_CUPG && i == 2 && nDC!=SCREEN)
						continue;
					//Change:Added INEXT with OR in following line:pnb:16111999.
    		        if(nParamCount ==SRNO_CM && ( (m_FileInfo.nLength >= RAMPINI.nLength) || (atoi(nConfig.szLength) >= RAMPINI.nLength) ))
		            {
                        //30042001
        	           	//char path[80];
					    //char callow[5],calhigh[5];

						//02052001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
								//gotoxy(X , Ypos_Std);
								sprintf(buf,"%4.3f",prmarray->fStdVal[nFreqIndex][i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Std,buf,2);//Added by:sdt:09112005:2200
							 }
						 }

						SwitchOffsetLines(i,ON);
						SelectRange(i,ON);

    	        		delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

					    if(bSync==1)
				    	   	gfOffset[i]  = GetAverageReadingSync(i+2,FALSE);
					    else
							gfOffset[i]  = GetAverageReading(i+2,FALSE);

						SwitchOffsetLines(i,OFF);
						SelectRange(i,OFF);

						//02052001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
                	                //12042001
								//gotoxy(X , Ypos_Offset);
								sprintf(buf,"%4.3f",gfOffset[i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Offset,buf,2);//Added by:sdt:09112005:2200
	                         }
    	                 }
				        SwitchStdvalLines(i,ON);
						SelectRange(i,ON);

						delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

				        if(bSync==1)
							fInstStd = GetAverageReadingSync(i+2,FALSE);
				        else
							fInstStd = GetAverageReading(i+2,FALSE);
				        if ( i == 1 )
					    {
							char RampMF[4];
							sprintf(path,"%s:\\cts.ini",cRamDrive);
							GetPrivateProfileString("RAMP_RANGE2_MF","FACTOR",RampMF,"1.0",path);
							RAMP_RANGE2_MF=atof(RampMF);
					   	   	fInstStd = fInstStd * RAMP_RANGE2_MF;
				         }

						SwitchStdvalLines(i,OFF);
						SelectRange(i,OFF);

                        //02052001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
                	                //12042001
								//gotoxy(X , Ypos_Meas);
								sprintf(buf,"%4.3f",fInstStd);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Meas,buf,2);//Added by:sdt:09112005:2200
	                         }
						 }

						fInstStd = fInstStd - gfOffset[i];

						sprintf(path,"%s:\\cts.ini",cRamDrive);
						GetPrivateProfileString("CAL_STD_LIMITS","CALLOW",callow,"0.7",path);
						GetPrivateProfileString("CAL_STD_LIMITS","CALHIGH",calhigh,"1.3",path);

				     	CALLOW=atof(callow);
				     	CALHIGH=atof(calhigh);

						t1 = prmarray->fStdVal[nFreqIndex][i]*(float)CALLOW;
						t2 = prmarray->fStdVal[nFreqIndex][i]*(float)CALHIGH;
						if(fInstStd < t1 || fInstStd > t2)
						{
							fFcal[i] = (float)1.0;
							gfOffset[i]=0.0;
						}
						else
						{
							if (fInstStd == 0)
								fFcal[i] = (float)1.0;
							else
								fFcal[i] = (float)(prmarray->fStdVal[nFreqIndex][i]) / (fInstStd);
						}

                        //02052001		if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
								//gotoxy(X , Ypos_fOffset);
								sprintf(buf,"%4.3f",gfOffset[i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_fOffset,buf,2);//Added by:sdt:09112005:2200

								//gotoxy(X , Ypos_Calfactor);
								sprintf(buf,"%4.3f",fFcal[i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Calfactor,buf,2);//Added by:sdt:09112005:2200
							 }
						 }

						fMeasuredStd = fInstStd;
						//Below line Uncommented by:sdt:06112006:2105
						fReadStd[nParamCount][i] = fInstStd;//Modified:jj:09052001
						fDeviationPercentage = (float) fabs( ( ( prmarray->fStdVal[nFreqIndex][i] - fMeasuredStd ) / prmarray->fStdVal[nFreqIndex][i])*100);
						//This is for Ranges
						//nPow = i;

						nPow= i - 1;

						fDevFactor = ( (prmarray->fStdVal[nFreqIndex][i] * (prmarray->fDeviationVal[nFreqIndex][i]/100))
									 + ( pow(10,nPow) * (prmarray->fDeviationFs[nFreqIndex][i]/100) ) );
						fMinDevFactor = (float) fabs((prmarray->fStdVal[nFreqIndex][i] - fDevFactor));
						fMaxDevFactor = (float) fabs((prmarray->fStdVal[nFreqIndex][i] + fDevFactor));
						if(( fMeasuredStd >= fMinDevFactor ) && ( fMeasuredStd <= fMaxDevFactor ))
							strcpy(szRemark,"Ok");
						else
							strcpy(szRemark,"NotOk");

    	                //02052001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
								//gotoxy(X , Ypos_DevMin);
								sprintf(buf,"%4.3f",fMinDevFactor);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_DevMin,buf,2);//Added by:sdt:09112005:2200

								//gotoxy(X , Ypos_DevMax);
								sprintf(buf,"%4.3f",fMaxDevFactor);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_DevMax,buf,2);//Added by:sdt:09112005:2200

								//gotoxy(X , Ypos_Remark);
								//cprintf(szRemark);
								//Condition Added by:sdt:05122005:0030
								if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
								{
									CalReport->writeStr(X , Ypos_Remark,szRemark,2);//Added by:sdt:09112005:2200
								}

								//Condition modified by:sdt:05122005:0030
								//This will control remark display through CTS.ini
								if((!strcmp(szRemark,"NotOk"))&&( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) ))
								{
								   //gotoxy(10 ,22 );
								   //cprintf("Out of Limit: Press any key to Continue :ESC to quit  ");
								   CalReport->writeStr(10, 19,"Out of Limit: Press any key to Continue :ESC to quit  ",2);//Added by:sdt:09112005:2200
								   while ((key = getkey ()) == 0);
								   //gotoxy(10 ,22 );
								   //cprintf("                                                      ");
								   CalReport->writeStr(10, 19,"                                                      ",2);//Added by:sdt:09112005:2200
								}
								else
								{
								   //gotoxy(10 ,22 );
								   //cprintf("Press : ESC to quit                                    ");
								   CalReport->writeStr(10, 19,"Press : ESC to quit                                    ",2);//Added by:sdt:09112005:2200
								}

							}
							if(nDC==PRINTER)//Added:jj:09052001
								  fprintf(fpTmp,"%-10s%-15s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);

						}
						else//Added:jj:09052001
						{
							if(nDC==PRINTER)
							{
								if(DOT)
								{

									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )

											fprintf(fpTmp,"%-10s%-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
										 else
											fprintf(fpTmp,"%-10s%-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
									}
									else
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
											fprintf(fpTmp,"%-10s%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
										else
											fprintf(fpTmp,"%-10s%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);

											/*fprintf(fpTmp,"%-10s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);*/
									}
								}
								else
								{
									//Commented by:sdt:08112006:2135
									/*if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )

											fprintf(fpTmp,"%-10s%-15s%-10d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);
										 else
											fprintf(fpTmp,"%-10s%-15s%-10d%8.2f%12.2f%12.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor);
									}
									else
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
											fprintf(fpTmp,"%-10s%-15s%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);
										else
											fprintf(fpTmp,"%-10s%-15s%8.2f%12.2f%12.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor);

									}*/

								}
							}
						}
					}
					else //if((nParamCount!=SRNO_NEXT)&&nParamCount!=SRNO_ELFEXT)//Commented jj:24032001
					{
						//30042001
						//ReadInstrStd( &fInstStd, i, nFreqIndex);

						//30042001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
								if(nDC==SCREEN)
								{
									//gotoxy(X , Ypos_Std);
									sprintf(buf,"%4.3f",prmarray->fStdVal[nFreqIndex][i]);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_Std,buf,2);//Added by:sdt:09112005:2200
								 }
						 }

						SwitchOffsetLines(i,ON);
						SelectRange(i,ON);

						delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

						gfOffset[i]  = GetAverageReading(i,FALSE);

						SwitchOffsetLines(i,OFF);
						SelectRange(i,OFF);

							//30042001
if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
							   //12042001
								//gotoxy(X , Ypos_Offset);
								sprintf(buf,"%4.3f",gfOffset[i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Offset,buf,2);//Added by:sdt:09112005:2200
							 }
						 }

						SwitchStdvalLines(i,ON);
						SelectRange(i,ON);

						delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
						fInstStd = GetAverageReading(i,FALSE);

						SwitchStdvalLines(i,OFF);
						SelectRange(i,OFF);

						//30042001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
								//12042001
								//gotoxy(X , Ypos_Meas);
								sprintf(buf,"%4.3f",fInstStd);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Meas,buf,2);//Added by:sdt:09112005:2200
							 }
						 }
						fInstStd = fInstStd - gfOffset[i];
						sprintf(path,"%s:\\cts.ini",cRamDrive);
						GetPrivateProfileString("CAL_STD_LIMITS","CALLOW",callow,"0.7",path);
						GetPrivateProfileString("CAL_STD_LIMITS","CALHIGH",calhigh,"1.3",path);

						CALLOW=atof(callow);
						CALHIGH=atof(calhigh);
						t1 = prmarray->fStdVal[nFreqIndex][i]*(float)CALLOW;
						t2 = prmarray->fStdVal[nFreqIndex][i]*(float)CALHIGH;
						if(fInstStd < t1 || fInstStd > t2)
						{
							fFcal[i] = (float)1.0;
							gfOffset[i]=0.0;
						}
						else
						{
							if (fInstStd == 0)
								fFcal[i] = (float)1.0;
							else
								fFcal[i] = (float)(prmarray->fStdVal[nFreqIndex][i]) / (fInstStd);
						}

						//30042001
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
//						if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
						{
							if(nDC==SCREEN)
							{
								//gotoxy(X , Ypos_fOffset);
								sprintf(buf,"%4.3f",gfOffset[i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_fOffset,buf,2);//Added by:sdt:09112005:2200
								//gotoxy(X , Ypos_Calfactor);
								sprintf(buf,"%4.3f",fFcal[i]);
								//cprintf(buf);
								CalReport->writeStr(X , Ypos_Calfactor,buf,2);//Added by:sdt:09112005:2200
							 }

						 }

						fMeasuredStd = fInstStd;
						//Below line Uncommented by:sdt:06112006:2105
						fReadStd[nParamCount][i] = fInstStd;//Modified:jj:09052001
						fDeviationPercentage = (float) fabs( ( ( prmarray->fStdVal[nFreqIndex][i] - fMeasuredStd ) / prmarray->fStdVal[nFreqIndex][i])*100);
						//This is for Ranges
						nPow = i;
						if(nParamCount==SRNO_CR)
						nPow= i + 1;
						if(nParamCount==SRNO_CM)
						nPow= i - 1;
						if(nParamCount==SRNO_CUPP || nParamCount==SRNO_CUPG)
						nPow= i + 2;
						fDevFactor = ( (prmarray->fStdVal[nFreqIndex][i] * (prmarray->fDeviationVal[nFreqIndex][i]/100))
									 + ( pow(10,nPow) * (prmarray->fDeviationFs[nFreqIndex][i]/100) ) );
						fMinDevFactor = (float) fabs((prmarray->fStdVal[nFreqIndex][i] - fDevFactor));
						fMaxDevFactor = (float) fabs((prmarray->fStdVal[nFreqIndex][i] + fDevFactor));
						if(( fMeasuredStd >= fMinDevFactor ) && ( fMeasuredStd <= fMaxDevFactor ))
							strcpy(szRemark,"Ok");
						else
							strcpy(szRemark,"NotOk");
					 }
					if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)

//					if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
					{
						if(nDC==SCREEN)
						{
							//gotoxy(X , Ypos_DevMin);
							sprintf(buf,"%4.3f",fMinDevFactor);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_DevMin,buf,2);//Added by:sdt:09112005:2200
							//gotoxy(X , Ypos_DevMax);
							sprintf(buf,"%4.3f",fMaxDevFactor);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_DevMax,buf,2);//Added by:sdt:09112005:2200

							//gotoxy(X , Ypos_Remark);
							//cprintf(szRemark);
							//Condition Added by:sdt:05122005:0030
							if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
							{
								CalReport->writeStr(X , Ypos_Remark,szRemark,2);//Added by:sdt:09112005:2200
							}
						   //Added:jj:02052001
							//Condition modified by:sdt:05122005:0030
							//This will control remark display through CTS.ini
							if((!strcmp(szRemark,"NotOk"))&&( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) ))
							{
							   //gotoxy(10 ,22 );
							   //cprintf("Out of Limit: Press any key to Continue :ESC to quit  ");
							   CalReport->writeStr(10,19,"Out of Limit: Press any key to Continue :ESC to quit  ",2);//Added by:sdt:09112005:2200
							   while ((key = getkey ()) == 0);
							   //gotoxy(10 ,22 );
							   //cprintf("                                                      ");
							   CalReport->writeStr(10,19,"                                                      ",2);//Added by:sdt:09112005:2200
							}
							else
							{
							   //gotoxy(10 ,22 );
							   //cprintf("Press : ESC to quit                                    ");
							   CalReport->writeStr(10,19,"Press : ESC to quit                                    ",2);//Added by:sdt:09112005:2200
							}

						}
						if(nDC==PRINTER)
						{
							  fprintf(fpTmp,"%-10s%-15s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
															 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);
						}

					}
					else//Added:jj:09052001
					{
						if(DOT)
						{

							if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
							{
								if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
										fprintf(fpTmp,"%-10s%-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,i+1,
															 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
								 else
									fprintf(fpTmp,"%-10s%-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
							}
							else
							{
								if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
									fprintf(fpTmp,"%-10s%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
								else
									fprintf(fpTmp,"%-10s%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
							}
						}
						else
						{
							//Commented by:sdt:08112006:2135
							/*if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
							{
								if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
										fprintf(fpTmp,"%-10s%-15s%-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
								else
										fprintf(fpTmp,"%-10s%-15s%-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
							}
							else
							{
								if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
									fprintf(fpTmp,"%-10s%-15s%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
												 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
								else
									fprintf(fpTmp,"%-10s%-15s%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
												 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);

							}*/
					   }
					}
					if(nDC==SCREEN)//Added:jj:13042001
					{

						//12042001
						if(strcmp(szRemark,"NotOk"))//02052001
							key=getkey();
						switch(key)
						{
							case ESCAPE :return FALSE;
							 default: break;
						}
					}
			   }
			   if(nDC==SCREEN)//Added:jj:13042001
			   {
				   //gotoxy(10 ,22 );
				   //cprintf("Press : Enter Key to Continue : ESC to quit");
				   CalReport->writeStr(10,19,"Press : Enter Key to Continue : ESC to quit",2);//Added by:sdt:09112005:2200

				   while ((key = getkey ()) == 0);
					switch (key)
					{
							 case ESCAPE :return FALSE;
								default: break;
					}
					if(key!=ENTER)
						while ((key = getkey ()) !=ENTER );
			   }
		   }
	   }
	}
	if (prmarray->nParamType == 1) //Hf
	{
		//Commented by:sdt:101112005:1930
		//sprintf(path,"%s:\\cts.ini",cRamDrive);
		//if(!GetPrivateProfileInt("HF_OFFSET","MEASURE_HF_OFFSETS",0,path))
		//	ReadHFOffsets();

		memset(HFfreqSelected,0,sizeof(int)*MAXFREQUENCIES);
		for(int i=SRNO_ATN;i<MAXPARAMETERS;i++)
		{
			for(int j=0;j<MAXFREQUENCIES;j++)
			{
				if(nConfig.Specks.speckparams[i].freq[j]==1)
				{
					HFfreqSelected[j]=1;
				}
			}
		}
		for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
		//Uncommented by sdt:15032015:1605//Condition modified by:sdt:08112005:2250
		//nFreqNum = HFFrequency.nDefualtFreq;
		{
			//Modified by:sdt:07082001
			//if(nConfig.Specks.speckparams[nParamCount].freq[nFreqNum])
			if(HFfreqSelected[nFreqNum])
			{
					//Commented by:sdt:07082001
					//Added:jj:08042001
					//if(nParamCount==SRNO_ZCAL  && nFreqNum <LFFrequency.nTotalFrequencies)
					//	continue;

				if(nDC==SCREEN)//Added:jj:13042001
				{
					//Added:jj:12042001
						InitCalDisplay(nFreqNum);
						Range_posX=X_OFFSET+5;
				}
				SetHighFreq(nFreqNum);
					//Post Frequency Delay Added by:sdt:03082001
				delay( rdCtrl.m_PostFreqDelay[nFreqNum] );

				nFreqIndex =nFreqNum;
				nConfig.nHFrequency=nFreqNum;//for AtnRef

				for(i=0;i<prmarray->nRanges;i++)
				{
				   if(nDC==SCREEN)//Added:jj:13042001
				   {
							//12042001
							Range_posX+=12;
							//gotoxy(Range_posX, Range_posY);
							sprintf(buf,"Range %d",i+1);
							//cprintf(buf);
							CalReport->writeStr(Range_posX, Range_posY,buf,2);//Added by:sdt:09112005:2200
							X=Range_posX;
				   }
					   //Commented by:sdt:07082001
					   //Modified:jj:08042001
					   //if(nParamCount==SRNO_ATN  || nParamCount==SRNO_ZCAL)
					   if(nParamCount==SRNO_ATN) //Added by:sdt:07082001
					   {
							//here//02052001
							//ReadInstrStd(&fInstStd, i, nFreqIndex);

							//Added:Start:jj:02052001
							if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
							//if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
							{
								if(nDC==SCREEN)
								{
									//gotoxy(X , Ypos_Std);
									sprintf(buf,"%4.3f",prmarray->fStdVal[nFreqIndex][i]);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_Std,buf,2);//Added by:sdt:09112005:2200
								 }
							 }

							SelectRange(i,ON);
							delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

							SwitchOffsetLines(i,ON);

							//commented by:sdt:10112005:1930
							//sprintf(path,"%s:\\cts.ini",cRamDrive);
							//if(GetPrivateProfileInt("HF_OFFSET","MEASURE_HF_OFFSETS",0,path))
									gfOffset[i]  = GetAverageReading(i,FALSE);
							//else
							//		gfOffset[i]  =HF_Offset[nFreqIndex][i];

							SwitchOffsetLines(i,OFF);
							SelectRange(i,OFF);
//

//							if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
							if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
							{
								if(nDC==SCREEN)
								{
									//gotoxy(X , Ypos_Offset);
									sprintf(buf,"%4.3f",gfOffset[i]);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_Offset,buf,2);//Added by:sdt:09112005:2200
								}
							}

							SelectRange(i,ON);
							delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

							SwitchStdvalLines(i,ON);

							fInstStd = GetAverageReading(i,FALSE);

							SwitchStdvalLines(i,OFF);
							SelectRange(i,OFF);
							if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
//							if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
							{
								if(nDC==SCREEN)
								{
									//gotoxy(X , Ypos_Meas);
									sprintf(buf,"%4.3f",fInstStd);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_Meas,buf,2);//Added by:sdt:09112005:2200
								 }
							}

							fInstStd = fInstStd - gfOffset[i];

							sprintf(path,"%s:\\cts.ini",cRamDrive);
							GetPrivateProfileString("CAL_STD_LIMITS","CALLOW",callow,"0.7",path);
							GetPrivateProfileString("CAL_STD_LIMITS","CALHIGH",calhigh,"1.3",path);

							CALLOW=atof(callow);
							CALHIGH=atof(calhigh);

							t1 = prmarray->fStdVal[nFreqIndex][i]*(float)CALLOW;
							t2 = prmarray->fStdVal[nFreqIndex][i]*(float)CALHIGH;
							if(fInstStd < t1 || fInstStd > t2)
							{
								fFcal[i] = (float)1.0;
								gfOffset[i]=0.0;
							}
							else
							{
								if (fInstStd == 0)
									fFcal[i] = (float)1.0;
								else
									fFcal[i] = (float)(prmarray->fStdVal[nFreqIndex][i]) / (fInstStd);
							}
							if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
							//if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) )
							{
								if(nDC==SCREEN)
								{
									//gotoxy(X , Ypos_fOffset);
									sprintf(buf,"%4.3f",gfOffset[i]);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_fOffset,buf,2);//Added by:sdt:09112005:2200

									//gotoxy(X , Ypos_Calfactor);
									sprintf(buf,"%4.3f",fFcal[i]);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_Calfactor,buf,2);//Added by:sdt:09112005:2200
								 }

							 }
							 //End:jj:02052001

							fMeasuredStd = fInstStd;
							//Below line Uncommented by:sdt:06112006:2105
							fReadStd[nParamCount][i] = fInstStd;//Modified:jj:09052001
							fDeviationPercentage = (float) fabs( ( ( prmarray->fStdVal[nFreqIndex][i] - fMeasuredStd ) / prmarray->fStdVal[nFreqIndex][i])*100);
							 //This is for Ranges
							//if(nParamCount==SRNO_V1V2) //Commented by:sdt:10112005:1950
							if(nParamCount==SRNO_ATN)//UnCommented by:sdt:10112005:1950
								nPow= i - 1;
							else
								nPow = i;
							fDevFactor = ( (prmarray->fStdVal[nFreqIndex][i] * (prmarray->fDeviationVal[nFreqIndex][i]/100))
										 + ( pow(10,nPow) * (prmarray->fDeviationFs[nFreqIndex][i]/100) ) );
							fMinDevFactor = (float) fabs((prmarray->fStdVal[nFreqIndex][i] - fDevFactor));
							fMaxDevFactor = (float) fabs((prmarray->fStdVal[nFreqIndex][i] + fDevFactor));
							if(( fMeasuredStd >= fMinDevFactor ) && ( fMeasuredStd <= fMaxDevFactor ))
								strcpy(szRemark,"Ok");
							else
								strcpy(szRemark,"NotOk");
						}
						//if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1))
						if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
						{
							if(nDC==SCREEN)
							{
								if(nDC==SCREEN)
								{
									//gotoxy(X , Ypos_DevMin);
									sprintf(buf,"%4.3f",fMinDevFactor);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_DevMin,buf,2);//Added by:sdt:09112005:2200

									//gotoxy(X , Ypos_DevMax);
									sprintf(buf,"%4.3f",fMaxDevFactor);
									//cprintf(buf);
									CalReport->writeStr(X , Ypos_DevMax,buf,2);//Added by:sdt:09112005:2200

									//gotoxy(X , Ypos_Remark);
									//cprintf(szRemark);
									//Condition added by:sdt:05122005:0045
									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
									{
										CalReport->writeStr(X , Ypos_Remark,szRemark,2);//Added by:sdt:09112005:2200
									}

								   //Added:jj:02052001
									//Condition modified by:sdt:05122005:0045
									if((!strcmp(szRemark,"NotOk"))&&( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) ))
									{
									   //gotoxy(10 ,22 );
									   //cprintf("Out of Limit: Press any key to Continue :ESC to quit  ");
									   CalReport->writeStr(10,19,"Out of Limit: Press any key to Continue :ESC to quit  ",2);//Added by:sdt:09112005:2200
									   while ((key = getkey ()) == 0);
									   //gotoxy(10 ,22 );
									   //cprintf("                                                      ");
									   CalReport->writeStr(10,19,"                                                      ",2);//Added by:sdt:09112005:2200
									}
									else
									{
									   //gotoxy(10 ,22 );
									   //cprintf("Press : ESC to quit                                    ");
									   CalReport->writeStr(10,19,"Press : ESC to quit                                    ",2);//Added by:sdt:09112005:2200
									}

								}

							}
							if(nDC==PRINTER)
								 /*fprintf(fpTmp,"%-10s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,i+1,
															 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);*/
								 fprintf(fpTmp,"%-10s%-15s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
															 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);

						}
						else//Added:jj:09052001
						{

								if(DOT)
								{

									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )

											fprintf(fpTmp,"%-10s%-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
										 else
											fprintf(fpTmp,"%-10s%-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
									}
									else
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
											fprintf(fpTmp,"%-10s%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
										else
											fprintf(fpTmp,"%-10s%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
									}
								}
								else
								{
									//Commented by:sdt:08112006:2140
									/*if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )

											fprintf(fpTmp,"%-10s%-15s%-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
										 else
											fprintf(fpTmp,"%-10s%-15s%-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);
									}
									else
									{
										if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
											fprintf(fpTmp,"%-10s%-15s%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,szRemark);
										else
											fprintf(fpTmp,"%-10s%-15s%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
																 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage);

									}*/
							   }
							/*if(nDC==PRINTER)
							{
								if(DOT)
									fprintf(fpTmp,"%-10s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,i+1,
															 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);
								 else
									 fprintf(fpTmp,"%-10s%-15s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],i+1,
															 prmarray->fStdVal[nFreqIndex][i],fMeasuredStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);
						   }  */
						}
					}

					//if (nParamCount == SRNO_V1V2) //Commented by:sdt:10112005:1950
					if (nParamCount == SRNO_ATN) //UnCommented by:sdt:10112005:1950
						FindAtnRefCalFactor();
					//commented by :sdt:07082001
					//if(nDC==SCREEN && nParamCount == SRNO_V1V2) //Commented by:sdt:10112005:1950
					if(nDC==SCREEN && nParamCount == SRNO_ATN)//UnCommented by:sdt:10112005:1950
					{
						Range_posX+=10;
						//gotoxy(Range_posX, Range_posY);
						//cprintf("V1 Cal.");
						CalReport->writeStr(Range_posX, Range_posY,"V1 Cal.",2);//Added by:sdt:09112005:2200
						X=Range_posX;
					}
					//Modified by:sdt:07082001
					//Modified:jj:02052001
					//if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) && (nParamCount >= SRNO_ATN))
					//if( strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1) && (nParamCount == SRNO_V1V2))//Commented by:sdt:10112005:1950
					if ((g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0) && (nParamCount == SRNO_ATN))//UnCommented by:sdt:10112005:1950
					{
						//Check for printer or screen is not available.
						//Check added by :sdt:12032001

						fDeviationPercentage = (float) fabs( ( ( fAtnRefStd[nConfig.nHFrequency]) - fReadAtnStd ) / fAtnRefStd[nConfig.nHFrequency]*100);
						fDevFactor = ( (fAtnRefStd[nConfig.nHFrequency] * (prmarray->fDeviationVal[nFreqIndex][1]/100))
										 + ( pow(10,1) * (prmarray->fDeviationFs[nFreqIndex][1]/100) ) );
						fMinDevFactor = (float) fabs((fAtnRefStd[nConfig.nHFrequency] - fDevFactor));
						fMaxDevFactor = (float) fabs((fAtnRefStd[nConfig.nHFrequency] + fDevFactor));

						if(nDC==SCREEN) //changed by:sdt:28022001
						{
								//12042001
							//gotoxy(X , Ypos_Std);
							sprintf(buf,"%4.3f",fAtnRefStd[nConfig.nHFrequency]);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_Std,buf,2);//Added by:sdt:09112005:2200

							//gotoxy(X , Ypos_Meas);
							sprintf(buf,"%4.3f",fReadAtnStd);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_Meas,buf,2);//Added by:sdt:09112005:2200

							if(( fReadAtnStd >= fMinDevFactor ) && ( fReadAtnStd <= fMaxDevFactor ))
								strcpy(szRemark,"Ok");
							else
								strcpy(szRemark,"NotOk");

						   //12042001
							//gotoxy(X , Ypos_DevMin);
							sprintf(buf,"%4.3f",fMinDevFactor);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_DevMin,buf,2);//Added by:sdt:09112005:2200

							//gotoxy(X , Ypos_DevMax);
							sprintf(buf,"%4.3f",fMaxDevFactor);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_DevMax,buf,2);//Added by:sdt:09112005:2200

							//gotoxy(X , Ypos_Calfactor);
							sprintf(buf,"%4.3f",fAtnCalRef);
							//cprintf(buf);
							CalReport->writeStr(X , Ypos_Calfactor,buf,2);//Added by:sdt:09112005:2200

							//gotoxy(X , Ypos_Remark);
							//cprintf(szRemark);
							//Condition modified by:sdt:05122005:0050
							if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
							{
								CalReport->writeStr(X , Ypos_Remark,szRemark,2);//Added by:sdt:09112005:2200
							}

							//Condition modified by:sdt:05122005:0050
							if((!strcmp(szRemark,"NotOk"))&&( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) ))
							{
							   //gotoxy(10 ,22 );
							   //cprintf("Out of Limit: Press any key to Continue :ESC to quit  ");
							   CalReport->writeStr(10 , 22,szRemark,2);//Added by:sdt:09112005:2200
							   while ((key = getkey ()) == 0);
							   //gotoxy(10 ,22 );
							   //cprintf("                                                      ");
							   CalReport->writeStr(10 , 22,"                                                      ",2);//Added by:sdt:09112005:2200

							}
							else
							{
							   //gotoxy(10 ,22 );
							   //cprintf("Press : ESC to quit                                    ");
							   CalReport->writeStr(10 , 22,"Press : ESC to quit                                    ",2);//Added by:sdt:09112005:2200
							}

						}
						if(nDC==PRINTER)//Added by:sdt:12032001
						{
						   //Modified by:sdt:07082001
						   //fprintf(fpTmp,"%sRef.                  %-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,1,
						   fprintf(fpTmp,"%sRef.  %-15s%-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],1,
								 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);

						}

					   if(nDC==SCREEN)//Added:jj:13042001
					   {

							//12042001
							if(strcmp(szRemark,"NotOk"))//02052001
								key=getkey();
							switch(key)
							{
							  case ESCAPE :return FALSE;
							  default: break;
							}
						}
					}

					//if( strcmp(_argv[1], Secu.szArgv)!=0 && (_argc == 1) && (nParamCount == SRNO_V1V2))//Commented by:sdt:10112005:1950
					if ((g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0) && (nParamCount == SRNO_ATN))//UnCommented by:sdt:10112005:1950
					{
						if(nDC==PRINTER)//Added by:sdt:12032001
						{
						   /*fprintf(fpTmp,"%sRef.                  %-8d%8.2f%12.2f%12.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,1,
								 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage,fMinDevFactor,fMaxDevFactor,szRemark);*/
							if(DOT)
							{
								if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
								{
									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
										fprintf(fpTmp,"%sRef.   %-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,1,
															 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage,szRemark);
									else
										fprintf(fpTmp,"%sRef.   %-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,1,
																 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage);
								}
								else
								{
									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
										fprintf(fpTmp,"%sRef.   %8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,
															 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage,szRemark);
									else
										fprintf(fpTmp,"%sRef.   %8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,
															 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage);
								 }

							}
							else
							{
								//Commented by:sdt:08112006:2140
								/*if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
								{
									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
										fprintf(fpTmp,"%sRef   %-15s%-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],1,
										//Commented by:sdt:07082001
										//fprintf(fpTmp,"%sRef.                  %-10d%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,1,
														 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage,szRemark);
									else
										fprintf(fpTmp,"%sRef   %-15s%-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],1,
										//Commented by:sdt:07082001
										//fprintf(fpTmp,"%sRef.                  %-10d%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,1,
																	 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage);
								}
								else
								{
									if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
										fprintf(fpTmp,"%sRef   %-15s%8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
														fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage,szRemark);
										//Commented by:sdt:07082001
										//fprintf(fpTmp,"%sRef.                  %8.2f%12.2f%12.2f%10s\n",prmarray->szParamNameAbr,

									else
										fprintf(fpTmp,"%sRef   %-15s%8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,SpeckList[nFreqNum],
										//Commented by:sdt:07082001
										//fprintf(fpTmp,"%sRef.                  %8.2f%12.2f%12.2f\n",prmarray->szParamNameAbr,
															 fAtnRefStd[nConfig.nHFrequency],fReadAtnStd,fDeviationPercentage);
								 }*/
							 }
						}
					}

				   if(nDC==SCREEN)//Added:jj:13042001
				   {

						//gotoxy(10 ,22 );
						//cprintf("Press : Enter Key to Continue : ESC to quit");
						CalReport->writeStr(10 , 22,"Press : Enter Key to Continue : ESC to quit",2);//Added by:sdt:09112005:2200
						while ((key = getkey ()) == 0);
						switch (key)
						{
							 case ESCAPE :return FALSE;
							 default: break;
						}
						if(key!=ENTER)
							while ((key = getkey ()) !=ENTER );
				   }
				}
			 }
		}
		SwitchCommonLines(OFF);
		SwitchJunctionLines(OFF);
		SelectReadingLines(0,OFF);
		ResetSystem();
	//return;
		return TRUE;
}

void InitCalDisplay(int nFreqNum)
{

	//FILE  *fp;
	//char str[180];
	//int i;
	//char cCh;
	//int x=5,y=1;
	char buf[180];
	X = X_OFFSET;
	Y= 1;//Y_OFFSET;//Modified by:sdt:09112005:2310

	//clrscr(); //Commented by:sdt:09112005:2200
	//window(1,1,80,25);//Commented by:sdt:09112005:2200
	//textbackground( LIGHTCYAN);//Commented by:sdt:09112005:2200
	//textcolor( WHITE);//Commented by:sdt:09112005:2200
	//clrscr(); //Commented by:sdt:09112005:2200
	//DrawBox(1,1,80,25);//Commented Temporary by:sdt:09112005:1855
	//window( 2,2,79,24); //Commented by:sdt:09112005:2200
	CalReport->draw();
	//Commented by:sdt:09112005:2310:start
	/*fp=fopen("title.inf","r");
	if(fp!=NULL)
	{
		while(!feof(fp))
		{
			i=0;
			cCh=fgetc(fp);
			if (cCh==EOF)
				break;
			while(cCh!='\n')
			{
				str[i++]=cCh;
				cCh=fgetc(fp);
			}
			if(i>70)
				str[70]='\0';
			else
				str[i]='\0';
			//gotoxy(x,y++);//Commented by:sdt:09112005:2200
			//cprintf(str);//Commented by:sdt:09112005:2200
			CalReport->writeStr(x,y++,str,2);//Added by:sdt:09112005:2200
		}
	}
	if (fp) if(fp){fclose(fp);}*/
	//Commented by:sdt:09112005:2310:end
	gettime(&t);
	getdate(&d);
	//gotoxy(X + 20,Y++);//Commented by:sdt:09112005:2200
	//cprintf("CALIBRATION REPORT");//Commented by:sdt:09112005:2200
	CalReport->writeStr(X + 20,Y++,"CALIBRATION REPORT",2);//Added by:sdt:09112005:2200
	//gotoxy(X + 20,Y++);//Commented by:sdt:09112005:2200
	sprintf(buf,"Time.   %2d:%02d:%02d",t.ti_hour,t.ti_min,t.ti_sec);
	//cprintf(buf);//Commented by:sdt:09112005:2200
	CalReport->writeStr(X + 20,Y++,buf,2);//Added by:sdt:09112005:2200
	//gotoxy(X + 20,Y++);//Commented by:sdt:09112005:2200
	sprintf(buf,"Date.   %d/%d/%d",d.da_day,d.da_mon,d.da_year);
	//cprintf(buf);//Commented by:sdt:09112005:2200
	CalReport->writeStr(X + 20,Y++,buf,2);//Added by:sdt:09112005:2200

	//gotoxy(X+20,Y++);
	//cprintf(SpeckList[nFreqNum]);
	CalReport->writeStr(X + 20,Y++,SpeckList[nFreqNum],2);//Added by:sdt:09112005:2200

	Y++;
	//gotoxy(X , Y++);//Commented by:sdt:09112005:2200
	sprintf(buf,"Parameter  %s",prmarray->szParamNameAbr);
	//cprintf(buf);//Commented by:sdt:09112005:2200
	CalReport->writeStr(X, Y++,buf,2);//Added by:sdt:09112005:2200
	Range_posY=Y;

	Y++;
	if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
	//if( strcmp(_argv[1], Secu.szArgv)==0 && _argc > 1  )
	{
			//gotoxy(X ,Y++);
			sprintf(buf,"------------------------------------------------------------------------");
			//cprintf(buf);
			CalReport->writeStr(X, Y++,buf,2);//Added by:sdt:09112005:2200
//	    	Y++;

			Ypos_Std=Y;
			//gotoxy(X , Y++);
			//cprintf("Standard");
			CalReport->writeStr(X, Y++,"Standard",2);//Added by:sdt:09112005:2200

			Ypos_Offset=Y;
			//gotoxy(X , Y++);
			//cprintf("Meas Offset");
			CalReport->writeStr(X, Y++,"Meas Offset",2);//Added by:sdt:09112005:2200

			Ypos_Meas=Y;
			//gotoxy(X , Y++);
			//cprintf("Meas Std");
			CalReport->writeStr(X, Y++,"Meas Std",2);//Added by:sdt:09112005:2200

			Ypos_DevMin=Y;
			//gotoxy(X , Y++) ;
			//cprintf("DevMin");
			CalReport->writeStr(X, Y++,"DevMin",2);//Added by:sdt:09112005:2200

			Ypos_DevMax=Y;
			//gotoxy(X , Y++) ;
			//cprintf("DevMax");
			CalReport->writeStr(X, Y++,"DevMax",2);//Added by:sdt:09112005:2200

			Ypos_fOffset=Y;
			//gotoxy(X , Y++);
			//cprintf("Final Offset");
			CalReport->writeStr(X, Y++,"Final Offset",2);//Added by:sdt:09112005:2200

			Ypos_Calfactor=Y;
			//gotoxy(X , Y++) ;
			//cprintf("CalFactor");
			CalReport->writeStr(X, Y++,"CalFactor",2);//Added by:sdt:09112005:2200

			Ypos_Remark=Y;
			//gotoxy(X , Y++) ;
			//cprintf("Remark");
			CalReport->writeStr(X, Y++,"Remark",2);//Added by:sdt:09112005:2200
			//gotoxy(X ,Y++);
			sprintf(buf,"------------------------------------------------------------------------");
			//cprintf(buf);
			CalReport->writeStr(X, Y++,buf,2);//Added by:sdt:09112005:2200
		}
	//gotoxy(10 ,22 );
	//cprintf("Press : ESC to quit");
	CalReport->writeStr(10, 19,"Press : ESC to quit",2);//Added by:sdt:09112005:2200
}


void printCalibrationReport()
{
	FILE *fTitle;
	char  cCh,szStr[200];//,szFreq[20]//commented by: sdt:07052001
	char path[80];

	nLineNo=1;
	nColNo=1;
	nPageNo=1;

	nDC=PRINTER;
	InitialisePrinter();
	if ((fptr_Report = fopen ("PRN","w"))==NULL)
	{
		//messagewindow("Cannot Open Printer...");
		messageBox("Cannot Open Printer...",mfError|mfOKButton );
		delay(250);
		return ;
	}
	SetSpeckInfo(); //Added by:sdt:07082001
	SelectNlq(FALSE);//TRUE);//Modified by:sdt:28062001
	SelectShrink(TRUE);
	SelectElite();//Added by:sdt:28062001
	SelectCondensed(TRUE); //Added bysdt:29062001
	nPrintMod=150;//MAXPRINTCOLS-19;//Modified:jj:08052001
	SkipTopMargin();
	fTitle=fopen("title.inf","r");
	if(nDC==PRINTER)
	{
		if(fTitle!=NULL)
		{
			while(!feof(fTitle))
			{
				cCh=fgetc(fTitle);
				fputc(cCh,fptr_Report);
			}
			nLineNo+=4;
		}
	}
	if(fTitle){fclose(fTitle);}
	gettime(&t);
	getdate(&d);
	sprintf(szStr,"\nCALIBRATION REPORT\n");
	WriteString(szStr,POS_CENTER,nPrintMod);
	memset(szStr,NULL,sizeof(szStr));
	sprintf(szStr,"Time.   %2d:%02d:%02d\n",t.ti_hour,t.ti_min,t.ti_sec);
	WriteString(szStr,POS_CENTER,nPrintMod);
	memset(szStr,NULL,sizeof(szStr));
	sprintf(szStr,"Date.   %d/%d/%d\n\n",d.da_day,d.da_mon,d.da_year);
	WriteString(szStr,POS_CENTER,nPrintMod);
	memset(szStr,NULL,sizeof(szStr));

	if(DOT)//Added:jj:09052001
	{
		sprintf(szStr,"Frequencies	Low: %s   High: %s\n\n",nConfig.szLFreq,nConfig.szHFreq);
		WriteString(szStr,POS_CENTER,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
	}
//	if( strcmp(_argv[1],Secu.szArgv)==0 && _argc > 1  )
//	if (strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1))
	if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
	{
		sprintf(szStr,"-----------------------------------------------------------------------------------------------------\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		//sprintf(szStr,"Param     Range      Standard     Measured   Deviation(%)    DevMin    DevMax     Remark\n");
		sprintf(szStr,"Param      Freq      Range      Standard     Measured   Deviation(%)    DevMin    DevMax     Remark\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		sprintf(szStr,"---------------------------------------------------------------------------------------------------\n\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
	}
	else
	{
		sprintf(szStr,"----------------------------------------------------------------------------------------\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
		if(DOT)//09052001
		{
			if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
			{
				if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
					sprintf(szStr,"Param     Range     Standard     Measured    Deviation    Remark\n");
				else
					sprintf(szStr,"Param     Range     Standard     Measured    Deviation   \n");
			}
			else
			{
				if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
					sprintf(szStr,"Param     Standard     Measured     Deviation     Remark\n");
				else
					sprintf(szStr,"Param     Standard     Measured     Deviation   \n");
			}
		}
		else
		{
			if ( GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path) )
			{
				if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
					sprintf(szStr,"Param       Freq         Range        Standard     Measured   Deviation   Remark\n");
				else
					sprintf(szStr,"Param       Freq         Range        Standard     Measured   Deviation   \n");
			}
			else
			{
				if ( GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path) )
					sprintf(szStr,"Param        Freq       Standard      Measured     Deviation  Remark\n");
				else
					sprintf(szStr,"Param        Freq       Standard      Measured     Deviation   \n");
			}

		}

		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		sprintf(szStr,"---------------------------------------------------------------------------------------------------\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
	}
	CalibrationReport();
	//if( strcmp(_argv[1], Secu.szArgv)==0 && _argc > 1  )
//	if (strcmp(_argv[1], Secu.szArgv)==0 && (_argc > 1))
	if (g_argc > 1 && strcmp(g_argv[1], Secu.szArgv) == 0)
	{
		sprintf(szStr,"---------------------------------------------------------------------------------------------------\n\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		sprintf(szStr,"End of Calibration Report\n");
		WriteString(szStr,POS_CENTER,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
	}
	else
	{
		//Uncommented by:sdt:08112006:2050
		PrintAutocalRep();//Function added by:sdt:06112006:2115
		sprintf(szStr,"-------------------------------------------------------------------------------------------------------\n\n");
		WriteString(szStr,POS_LEFT,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
		sprintf(szStr,"End of Calibration Report\n");
		WriteString(szStr,POS_CENTER,nPrintMod);
		memset(szStr,NULL,sizeof(szStr));
	}
	EndReport();

}

//Function Added by:sdt:06112006:2100
void PrintAutocalRep()
{
	//Added:jj:09052001
	char tag1[50];
	char tag2[50];
    int nFreqNum;

	char path[80];//22012001
	int remark=0,range=0,nParam,nRange;
	char szTmp[50],szTmp1[50];
	float dev1,dev2,range_std,readstd;
	int cupp_cupg=0;
	//char nTmp[5];

	sprintf(path,"%s:\\cts.ini",cRamDrive);
	remark = GetPrivateProfileInt("CAL_REPORT","SHOW_REMARK",0,path);
	range = GetPrivateProfileInt("CAL_REPORT","SHOW_RANGE",0,path);


	for(int i=0;i<MAXPARAMETERS;i++)
	{
		if(i==SRNO_ZCAL)
			continue;
		if((runSetup.ulParamRunSetup[i]&PARAMSELECTIONMASK)!=0)
		{

			nParam = i;

		//Modified:jj:02052001
		//if ( nParam<SRNO_CR||nParam>SRNO_ATN )

		//Commented by:sdt:06112006:2130
		//if ( nParam<Param.Lf_Param_Start||nParam>Param.Hf_Param_Start )
		//	continue;

		if ( (nParam==SRNO_CUPP||nParam==SRNO_CUPG) && cupp_cupg == 1)
			continue;

		//Conditions added by:sdt:08112006:2050
		if (nParam>=Param.Lf_Param_Start&&nParam<=Param.Lf_Param_End) //Lf
			nFreqNum = LFFrequency.nDefualtFreq;
		if (nParam>=Param.Hf_Param_Start&&nParam<=Param.Hf_Param_End) //Hf
			nFreqNum = HFFrequency.nDefualtFreq;

		//Commented by:sdt:08112006:2050
		//for(nFreqNum=0;nFreqNum<HFFrequency.nTotalFrequencies;nFreqNum++)
		{
			//Commented by:sdt:06112006:2130
			//if(nParam<=Param.Lf_Param_End && nFreqNum>LFFrequency.nTotalFrequencies-1)
			//	continue;

			if(nConfig.Specks.speckparams[nParam].freq[nFreqNum])
			{//End:jj:09052001

				for( nRange=0;nRange<4; nRange++ )
				{
					memset( szTmp, '\0', sizeof(szTmp) );
					sprintf( szTmp, "%s_R%d_STD",paraminfo[nParam].szParamAbr, \
					nRange+1 );

					sprintf( tag1, "CALREP_STD_FREQ%d",nFreqNum);//Added:jj:09052001

					//Modified:jj:09052001
					//GetPrivateProfileString ("CALREP_STD",szTmp,"",szTmp1,path);
					GetPrivateProfileString (tag1,szTmp,szTmp1,"",path);

					range_std = atof(szTmp1);
					if ( range_std != (float)0.0 )
					{
						char *p;
						memset( szTmp, '\0', sizeof(szTmp) );
						sprintf( szTmp, "%s_R%d_DEV",paraminfo[nParam].szParamAbr, \
						nRange+1 );

	                    sprintf( tag2, "CALREP_DEVIATION_FREQ%d",nFreqNum);//Added:jj:09052001

						//Modified:jj:09052001
						//GetPrivateProfileString ("CALREP_DEVIATION",szTmp,"0.0",szTmp1,path);
						GetPrivateProfileString (tag2,szTmp,szTmp1,"0.0",path);

						p = strtok( szTmp1, "-" );
						dev1 = atof(p);
						p = strtok(NULL, "-");
						dev2 = atof(p);
						if ( nParam == SRNO_CUPP )
						{
							fprintf(fpTmp, "%-5s%-5s", paraminfo[nParam].szParamAbr,"/G");
							cupp_cupg = 1;
						}
						else if ( nParam == SRNO_CUPG )
						{
							fprintf(fpTmp, "%-5s%-5s", paraminfo[nParam].szParamAbr,"/P");
							cupp_cupg = 1;
						}
						else if ( nParam == SRNO_ATN )
							fprintf(fpTmp, "%-5s%-5s", paraminfo[nParam].szParamAbr,"/XT");
						else
							fprintf(fpTmp, "%-10s", paraminfo[nParam].szParamAbr);
						//fprintf(fpTmp, "   ") ;

						if(DOT!=1)//Added:jj:09052001
							fprintf(fpTmp, "%-15s", SpeckList[nFreqNum]);
						//fprintf(fpTmp, "   ") ;

						if ( range )
                		{
							strcpy( szTmp, getRangeText( nRange+1, nParam ) );
							fprintf(fpTmp, "%-10s", szTmp );
                		}
						//fprintf(fpTmp, "   " );
						fprintf(fpTmp, "%-12s", ntos(range_std,5,2,0) );
						readstd = fReadStd[nParam][nRange];
						fprintf(fpTmp, "%-12s", ntos(readstd,5,2,0) ); // ntos(readstd,5,2,0) );
						//fprintf(fpTmp, "%-12s", ntos(fabs(readstd-range_std),5,2,0) );
						//Modified by:sdt:08112006:2120//For displaying deviation in percentage.
						fprintf(fpTmp, "%-12s", ntos(fabs(((readstd-range_std)/range_std)*100),5,2,0) );
						if ( remark )
						{
							if ( (readstd>=dev1) && (readstd<=dev2) )
								fprintf(fpTmp, "  Ok" );
							else
								fprintf(fpTmp, " NotOk" );
						}
						fprintf( fpTmp, "\n" );
					}
				}
			}
		}
		}
	}
}

//Function Added by:sdt:06112006:2100
char* ntos( double val, int dec, int prec, int round )
{
	char szTemp[100], *p, s[100];
	memset( s, '\0', 100 );
	sprintf( szTemp, "%f", val );
	p = strtok(szTemp, ".");
	if ( p == NULL )
		return NULL;
	if ( strlen( p ) > dec )
		sprintf( s, "%s.", p );
	else
	{
		int i;
		for( i = 0; i < (dec-strlen(p)); i++ )
			strcat( s, " " );
		strcat( s, p );
		strcat( s, "." );
	}
	p = strtok(NULL, ".");
	if ( p == NULL )
		return s;
	if ( strlen( p ) >= prec )
	{
		int i;
		for( i = 0; (i<prec)&&p; i++ )
		{
			char sztmp[50];
			if ( i == (prec-1) && round )
			{
				if ( *(p+1) >= 53 )
					*p += 1;
			}
			sprintf( sztmp, "%c", *p++ );
			strcat( s, sztmp );
		}
	}
	return s;
}



//Function Added by:sdt:06112006:2100
char * getRangeText( int range, int nParam )
{

	//Added:jj:10052001
	char szTag[50];
	char path[80];

	char szTmp[50];

	memset( szTmp, '\0', sizeof(szTmp) );

	//Added:jj:10052001
	sprintf(path,"%s:\\cts.ini",cRamDrive);
	sprintf(szTag,"%s_R%d_TEXT",paraminfo[nParam].szParamAbr,range);
	GetPrivateProfileString ("CAL_RANGE_TEXT",szTag,szTmp,"",path);

	return szTmp;
}
