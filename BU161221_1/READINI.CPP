#include <stdio.h>
#include <alloc.h>
#include <time.h>

#include "ct.h"
#include "share.h"
#include "utils.h"
#include "cconfig.h"
#include "funproto.h"
#include "run.h"
#include "setup.h"

//Global Variable//Start
extern char cRamDrive[2];
extern FreqStruct HFFrequency,LFFrequency;
extern Config config;
extern int LfIndex;    		//Added by:sdt:09092005:2300
extern int HfIndex;			//Added by:sdt:09092005:2300
extern char SpeckList[MAXFREQUENCIES][10]; //Added by:sdt:09092005:2300
extern int  nGiveAlarms;
extern int  nTempUnitOf;
extern int  ElfextLength;
extern char gMaxPinsReplaced;
extern float    fAtnRefStd[MAXFREQUENCIES];

extern PARAM_DETAILS Param;
//extern REPLACE     	pinReplace; //cOMMENTED BY:SDT:26042015:1700
extern RMPINIINFO  	RAMPINI;
extern REPINIINFO  	RepIni;
extern INFORMATION 	m_FileInfo;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
extern HFHHFLFREQLINES HfhHflFreqLines;
extern HFFREQUECYINFO HFfreqInfo;
extern STRUCT_SPECKS speckData;
extern char 	szSeqFileName[15];
extern SEQUENCE CableSequence;
extern RunSetup runSetup,TempRunSetup;//Added by:sdt:16102005:0835
extern ReportSetup reportSetup;
//Global Variable//End


//functions added for reading the frequency information
//from cts.ini
//Read the High frequency information
//Modified by:sdt:18032001
void ReadHFFreqInfo()
{
	char path[80];
	char szStr[100],szTmp[100];
	char *pStrPtr,*pStrPtr1,*pTempPtr;
	int nTempNo;

	sprintf(path,"%s:\\cts.ini",cRamDrive);

	strcpy(szStr,"");
	GetPrivateProfileString ("HF_FREQ_DETAILS_START","NO_OF_FREQLINES",szStr,"4",path);
	HFFrequency.nTotalFreqLines = atoi(szStr);

	strcpy(szStr,"");
	GetPrivateProfileString ("HF_FREQ_DETAILS_START","NO_OF_FREQUENCIES",szStr,"10",path);
	HFFrequency.nTotalFrequencies = atoi(szStr);

	strcpy(szStr,"");
	GetPrivateProfileString ("HF_FREQ_DETAILS_START","DEFAULT_FREQ",szStr,"7",path);
	HFFrequency.nDefualtFreq = atoi(szStr)-1;//Modified:jj:10042001

	HFFrequency.nFreqLines = (int *)farmalloc(sizeof(int)*HFFrequency.nTotalFreqLines);

	for (int i=0;i<HFFrequency.nTotalFreqLines;i++)
	{
		strcpy(szStr,"");
		strcpy(szTmp,"");
		sprintf(szTmp,"LINE_%d",i+1);
		GetPrivateProfileString ("HF_FREQ_LINES_START",szTmp,szStr,"0",path);
		HFFrequency.nFreqLines[i] = atoi(szStr);
	}

	HFFrequency.nStatusofFreqLines =  (STATUSOFFREQLINE *)
		farmalloc(sizeof(STATUSOFFREQLINE)*HFFrequency.nTotalFrequencies);
    int i;
    for (i=0;i<HFFrequency.nTotalFrequencies;i++)
    {
    	strcpy(szStr,"");
        strcpy(szTmp,"");
        sprintf(szTmp,"FREQ_%d",i+1);
		GetPrivateProfileString ("HF_FREQ_LINES_STATUS_START",szTmp,szStr,"0",path);
        pTempPtr = strdup(szStr);

        HFFrequency.nStatusofFreqLines[i].nFreqLines = (int *)farmalloc
				(sizeof(int)*HFFrequency.nTotalFreqLines);

        for (int j=0;j<HFFrequency.nTotalFreqLines;j++)
		{
			pStrPtr = strdup(pTempPtr);
			pStrPtr1 = strtok(pStrPtr,",");
			nTempNo= *pStrPtr1-'0';
			HFFrequency.nStatusofFreqLines[i].nFreqLines[j] = nTempNo > 1 ? 0 : nTempNo;
			(pTempPtr = strchr(pTempPtr,','))++;
			free(pStrPtr); //Added by:sdt:26042015:1700
		}
		free(pTempPtr);//Added by:sdt:6042015:1700

	}
}

//Read the Low frequency information
void ReadLFFreqInfo()
{
	char path[80];
    char szStr[100],szTmp[100];
    char *pStrPtr,*pStrPtr1,*pTempPtr;
    int nTempNo;

	sprintf(path,"%s:\\cts.ini",cRamDrive);

	strcpy(szStr,"");
	GetPrivateProfileString ("LF_FREQ_DETAILS_START","NO_OF_FREQLINES",szStr,"4",path);
	LFFrequency.nTotalFreqLines = atoi(szStr);

	strcpy(szStr,"");
	GetPrivateProfileString ("LF_FREQ_DETAILS_START","NO_OF_FREQUENCIES",szStr,"4",path);
	LFFrequency.nTotalFrequencies = atoi(szStr);

	strcpy(szStr,"");
	GetPrivateProfileString ("LF_FREQ_DETAILS_START","DEFAULT_FREQ",szStr,"3",path);
	LFFrequency.nDefualtFreq = atoi(szStr)-1;//Modified:jj:10042001

	LFFrequency.nFreqLines = (int *)farmalloc(sizeof(int)*LFFrequency.nTotalFreqLines);


	for (int i=0;i<LFFrequency.nTotalFreqLines;i++)
	{
		strcpy(szStr,"");
		strcpy(szTmp,"");
		sprintf(szTmp,"LINE_%d",i+1);
		GetPrivateProfileString ("LF_FREQ_LINES_START",szTmp,szStr,"0",path);
		LFFrequency.nFreqLines[i] = atoi(szStr);
	}

	LFFrequency.nStatusofFreqLines =  (STATUSOFFREQLINE *)
		farmalloc(sizeof(STATUSOFFREQLINE)*LFFrequency.nTotalFrequencies);

	for (int i=0;i<LFFrequency.nTotalFrequencies;i++)
    {
    	strcpy(szStr,"");
        strcpy(szTmp,"");
		sprintf(szTmp,"FREQ_%d",i+1);
		GetPrivateProfileString ("LF_FREQ_LINES_STATUS_START",szTmp,szStr,"0",path);
        pTempPtr = strdup(szStr);

        LFFrequency.nStatusofFreqLines[i].nFreqLines = (int *)farmalloc
				(sizeof(int)*LFFrequency.nTotalFreqLines);

        for (int j=0;j<LFFrequency.nTotalFreqLines;j++)
        {
			pStrPtr = strdup(pTempPtr);
			pStrPtr1 = strtok(pStrPtr,",");
			nTempNo= *pStrPtr1-'0';
			LFFrequency.nStatusofFreqLines[i].nFreqLines[j] = nTempNo > 1 ? 0 : nTempNo;
			(pTempPtr = strchr(pTempPtr,','))++;
			free(pStrPtr); //Added by:sdt:26042015:1700
	   }
	   free(pTempPtr); //Added by:sdt:26042015:1700

	}
}


//Function used to free the memory allocated by the Frequency structures
void freeFreqStruct()
{
    int i=0;

    for (i=0;i<HFFrequency.nTotalFreqLines;i++)
	{
		farfree(HFFrequency.nStatusofFreqLines[i].nFreqLines);

    }
    farfree(HFFrequency.nStatusofFreqLines);
    farfree(HFFrequency.nFreqLines);

    for (i=0;i<LFFrequency.nTotalFreqLines;i++)
    {
		farfree(LFFrequency.nStatusofFreqLines[i].nFreqLines);

	}
	farfree(LFFrequency.nStatusofFreqLines);
	farfree(LFFrequency.nFreqLines);
}

//functions added for default configuration from Default.ini
//Added by:sdt:26082005:1315
void ReadConfigInfo(void)
{
	char szTemp[80];
	time_t timer;
	struct tm *tlocal;
	FILE *fcnf;
	if ((fcnf = fopen("config.bin", "rb")) != NULL)
	{
		fread(&config, sizeof(Config),1,fcnf);
		readSpeck(speckData,config.speckNumber);
		strcpy(config.cabletype,speckData.cabletype);
		config.szCableSequence = speckData.szCableSequence;
		strcpy(szSeqFileName, speckData.szCableSequence.FileName);
		GetCableCategaryList(config.szCableSequence.SeqName);
		//config.m_Noofpairs = CableSequence.NumberOfPairs[0];
		GetCableUnitsList(config.m_Noofpairs);
		//config.m_Unitof=CableSequence.TotalPairs.NumberOfUnits[0];
	}
	else
	{

		config.speckNumber = GetPrivateProfileInt( "BASIC_INFO", "SPECK_NO", 1, "default.ini" );
		GetPrivateProfileString( "BASIC_INFO","SPECK_NAME", config.speckName,
			" ",	"default.ini" );
		config.m_Noofpairs=GetPrivateProfileInt( "BASIC_INFO", "NOOFPAIRS", 0, "default.ini" );
		config.m_Unitof=GetPrivateProfileInt( "BASIC_INFO", "UNITOF", 0, "default.ini" );
		config.m_Ilm=GetPrivateProfileInt( "BASIC_INFO", "ILM", 0, "default.ini" );
		config.m_Flm=GetPrivateProfileInt( "BASIC_INFO", "FLM", 0, "default.ini" );
		config.m_Length=GetPrivateProfileInt( "BASIC_INFO", "CABLE_LENGTH", 500, "default.ini" );
		config.m_StartFrom=GetPrivateProfileInt( "BASIC_INFO", "STARTFIXPOS", 1, "default.ini" );
		GetPrivateProfileString( "BASIC_INFO", "ROOM_TEMP", szTemp, "30.0", "default.ini" );
		config.m_Temp = atof( szTemp );
		for ( int i=1; i<=MAX_USERENTRIES; i++ )
		{
			sprintf( szTemp, "TITLE%d", i );
			GetPrivateProfileString( "BASIC_INFO", szTemp, config.m_UserTitles[i-1], "", "default.ini" );
			memset( &config.m_UserEntries[i-1], 0, MAX_USERENTRY_LEN );
		}

		readSpeck(speckData,config.speckNumber);
		strcpy(config.cabletype,speckData.cabletype);
		config.szCableSequence = speckData.szCableSequence;
		strcpy(szSeqFileName, speckData.szCableSequence.FileName);
		GetCableCategaryList(config.szCableSequence.SeqName);
		config.m_Noofpairs = CableSequence.NumberOfPairs[0];
		GetCableUnitsList(CableSequence.NumberOfPairs[0]);
		config.m_Unitof=CableSequence.TotalPairs.NumberOfUnits[0];
	}
	if(fcnf){fclose(fcnf);}

	if ( config.m_StartFrom <= 0 || config.m_StartFrom > MAX_CONDUCTORS )
		config.m_StartFrom = 1;
	// set date and time for config
	timer = time(NULL);
	tlocal = localtime( &timer );

	sprintf( config.m_PrintDate, "%02d/%02d/%04d", tlocal->tm_mday, tlocal->tm_mon, tlocal->tm_year+1900 );
	strcpy( config.m_StartTime, "00:00" );
	strcpy( config.m_StopTime, "00:00" );
	strcpy( config.m_TestDate, "00/00/0000" );




}

//Function used to read frequency list from Cts.ini and store in a
//"SpeckList" array.
void ReadFreqList(void)
{
	char path[80];
	char list[10];
	sprintf(path,"%s:\\cts.ini",cRamDrive);
	for (int f=0;f<MAXFREQUENCIES;f++)
	{
		sprintf(list,"FREQ_%d",f+1);
		GetPrivateProfileString ("FREQLIST",list,SpeckList[f],"",path);
		for( int j=1; j<=5; j++)
			SpeckList[f][20+j] = '\0';

		//Added:jj:24032001
		if(stricmp(SpeckList[f],"1 KHZ")==0)
			LfIndex=f;
		if(stricmp(SpeckList[f],"150 KHZ")==0)
			HfIndex=f;

	}
}


void InitializeINIInfo ()
{
	char path[80];//22012001

	char   szStdVal[120]; //, NoReplace=FALSE;
	int    ntmp[MAXFRCLINES],i,j,k,l;
	char   sztStr[MAXSTRINGLENGTH1];
	char   szStr[MAXSTRINGLENGTH1], szTmp[120];
    int    pinrep[4][2],pin=0,temp;
	InitSummaryFile();//Added by:sdt:03092001

	//Added:jj:16032001
	sprintf(path,"%s:\\cts.ini",cRamDrive);

	for(i=0;i<MAXFRCLINES;i++)
		ntmp[i] = 0;
	/*GetPrivateProfileString("BASIC_INFO","ROOM_TEMP",nConfig.szRoomtemp,"25.0","default.ini");
	GetPrivateProfileString("BASIC_INFO","CABLE_LENGTH","1000",nConfig.szLength,"default.ini");
	GetPrivateProfileString("BASIC_INFO","SPECK_ID","1",nConfig.szSpeckName,"default.ini");
	GetPrivateProfileString("BASIC_INFO","NOOFPAIRS","100",nConfig.szNoofpairs,"default.ini");
	GetPrivateProfileString("BASIC_INFO","UNITOF","20",nConfig.szUnitof,"default.ini");
	GetPrivateProfileString("BASIC_INFO","STARTFIXPOS","1",nConfig.szFixPos,"default.ini");
	GetPrivateProfileString("BASIC_INFO","FIXTURE","Inner",nConfig.szFixtures,"default.ini");
	GetPrivateProfileString("BASIC_INFO","CABLE_TYPE","PIJF_0.5",nConfig.Specks.cabletype,"default.ini");*/
	RepIni.nTopMargin=GetPrivateProfileInt("REPINFO","TOPMARGIN",5,"default.ini");
	RepIni.nBottomMargin=GetPrivateProfileInt("REPINFO","BOTTOMMARGIN",5,"default.ini");
	RepIni.nPaperInches=GetPrivateProfileInt("REPINFO","PAPERSIZE",12,"default.ini");
	RepIni.nPrinterTime=GetPrivateProfileInt("REPINFO","PRINTTIME",10,"default.ini");

	/*GetPrivateProfileString("BASIC_INFO","SPECK_NO","",szTmp,"default.ini");
	if ( strlen(szTmp) == 0 )
	{
		GetPrivateProfileString("BASIC_INFO","SPECK_NAME","",nConfig.szSpeckName,"default.ini");
		GetPrivateProfileString("BASIC_INFO","CABLE_TYPE","PIJF_0.5",nConfig.Specks.cabletype,"default.ini");
		SPECIFICATION spk;
		int reply = findspeck(nConfig.szSpeckName,nConfig.Specks.cabletype,&spk);
		if ( reply > 0 )
			sprintf(nConfig.Specks.specksnumber, "%d", reply );
	}
	else
	{
		strcpy(nConfig.Specks.specksnumber, szTmp );
		verifyspeckno( szTmp );
	}*/

	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	nGiveAlarms=GetPrivateProfileInt("BASIC_INFO","ALARMS",1,path);
	m_FileInfo.cVersion=GetPrivateProfileInt("BASIC_INFO","VERSION",1,path);

	//san 21-11-97 For appending of the remaining pairs
	//nTempUnitOf = atoi (nConfig.szUnitof);//Modified by:sdt:12092005:2235
	nTempUnitOf = config.m_Unitof;
	m_FileInfo.nAppend = GetPrivateProfileInt("BASIC_INFO","APPEND",0,path);
	//san

	/*for(i=0;i<MAXTITLES;i++)
	{
		sprintf(sztStr,"TITLE%d", i+1);
		GetPrivateProfileString("BASIC_INFO",sztStr,"",m_FileInfo.ENTRY[i].szTitle,"default.ini");
		for( int j=1; j<=5; j++)
			m_FileInfo.ENTRY[i].szTitle[20+j] = '\0';
		strcpy(m_FileInfo.ENTRY[i].szEntry,"");
		SetConfig(i+1,m_FileInfo.ENTRY[i].szTitle);
	}*/
	for(i=0;i<MAXRANGES;i++)
	{
		//for(j=0;j<MAX_FRC_PINS;j++)//commented by:sdt:28072001
		for(j=0;j<MAX_CTRL_LINES;j++)//Modified by:sdt:28072001
		{
			RAMPINI.nCtrlLinesOffset[i][j]=0;
			RAMPINI.nCtrlLinesStdval[i][j]=0;
			RAMPINI.nCtrlLinesRange[i][j]=0;
		}
	}
	for(i=0;i<MAXREADINGPOS;i++)
	{
		//for(j=0;j<MAX_FRC_PINS;j++)//commented by:sdt:28072001
        for(j=0;j<MAX_CTRL_LINES;j++)//Modified by:sdt:28072001
			RAMPINI.nCtrlLinesReading[i][j]=0;
	}
    //for(j=0;j<MAX_FRC_PINS;j++)//commented by:sdt:28072001
    for(j=0;j<MAX_CTRL_LINES;j++)//Modified by:sdt:28072001
		RAMPINI.nCtrlLinesCommon[j]=0;

	sprintf(sztStr,"RAMP");
	 sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	GetPrivateProfileString("STD_VAL",sztStr,szStdVal,"",path);
	strcpy(szTmp,szStdVal);
	j=0;  k=0; l=0;
	while(k<strlen(szTmp))
	{
		for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
			szStr[j] = szTmp[k++];
		szStr[j]=NULL;
		k++;
		RAMPINI.fStdVal[l++]=(float)atof(szStr);
	}
	//getting deviation reading for calibration purpose
	sprintf(sztStr,"RAMP");
	 sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	GetPrivateProfileString("DEVIATION_READING",sztStr,szStdVal,"",path);
	strcpy(szTmp,szStdVal);
	j=0;  k=0; l=0;
	while(k<strlen(szTmp))
	{
		for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
			szStr[j] = szTmp[k++];
		szStr[j]=NULL;
		k++;
		RAMPINI.fDeviationVal[l++]=(float)atof(szStr);
	}
	sprintf(sztStr,"RAMP");
	GetPrivateProfileString("DEVIATION_FS",sztStr,szStdVal,"",path);
	strcpy(szTmp,szStdVal);
	j=0;  k=0; l=0;
	while(k<strlen(szTmp))
	{
		for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
			szStr[j] = szTmp[k++];
		szStr[j]=NULL;
		k++;
		RAMPINI.fDeviationFs[l++]=(float)atof(szStr);
	}

	 //above for calibration purpose
	/*
	strcpy(sztStr,"RAMP_UNIT");
	GetPrivateProfileString("SYS_OFFSET_UNIT",sztStr,"0.0,0.0,0.0,0.0",szOffsetVal,"g:\\prog.ini");
	strcpy(szTmp, szOffsetVal);
	j = 0; k = 0; l = 0;
	while(k < strlen(szTmp))
	{
		for(j = 0; szTmp[k]!=',' && k < strlen(szTmp); j++)
			szStr[j] = szTmp[k++];
		szStr[j] = NULL;
		k++;
		RAMPINI.fSysOffsetUnit[l++] = (float)atof(szStr);
	}

	strcpy(sztStr,"RAMP_READING");
	GetPrivateProfileString("SYS_OFFSET_READING",sztStr,"0.0,0.0,0.0,0.0",szOffsetVal,"g:\\cts.ini");
	strcpy(szTmp,szOffsetVal);
	j = 0;  k = 0;  l = 0;
	while(k < strlen(szTmp))
	{
		for(j = 0; szTmp[k]!=',' && k < strlen(szTmp);j++)
			szStr[j] = szTmp[k++];
		szStr[j] = NULL;
		k++;
		RAMPINI.fSysOffsetRead[l++] = (float)atof(szStr);
	}
	*/
	strcpy(sztStr,"RAMP");
	 sprintf(path,"%s:\\prog.ini",cRamDrive);//22012001
	GetPrivateProfileString("COMMON_LINES", sztStr,szStdVal,0,path);
	strcpy(szTmp,szStdVal);
	j=0;  k=0;  l=0;
	while(k<strlen(szTmp))
	{
		for(j = 0; szTmp[k]!=',' &&  k < strlen(szTmp); j++)
			szStr[j] = szTmp[k++];
		szStr[j] = NULL;
		k++;
		RAMPINI.nCtrlLinesCommon[l++] = atoi(szStr);
	}

	for(i=0;i<2;i++)
	{
		sprintf(sztStr,"RAMP_RANGE%d",i+1);
		  sprintf(path,"%s:\\prog.ini",cRamDrive);//22012001
		 GetPrivateProfileString("OFFSET_LINES",sztStr,szStdVal,0,path);
		 strcpy(szTmp,szStdVal);
		 j=0;  k=0;  l=0;
		 while(k < strlen(szTmp))
		 {
			 for(j = 0; szTmp[k]!=',' && k < strlen(szTmp);j++)
				 szStr[j] = szTmp[k++];
			 szStr[j] = NULL;
			 k++;
			 RAMPINI.nCtrlLinesOffset[i][l++]=atoi(szStr);
		 }
		sprintf(sztStr,"RAMP_RANGE%d",i+3);
		 GetPrivateProfileString("RANGE_LINES",sztStr,szStdVal,0,path);
		 strcpy(szTmp, szStdVal);
		 j=0;  k=0;  l=0;
		 while(k<strlen(szTmp))
		 {
			 for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
				 szStr[j] = szTmp[k++];
			 szStr[j] = NULL;
			 k++;
			 RAMPINI.nCtrlLinesRange[i][l++] = atoi(szStr);
		 }
		 sprintf(sztStr,"RAMP_RANGE%d",i+3);
		 GetPrivateProfileString("STD_LINES",sztStr,szStdVal,0,path);
		 strcpy(szTmp, szStdVal);
		 j=0;  k=0;  l=0;
		 while(k<strlen(szTmp))
		 {
			 for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
				 szStr[j] = szTmp[k++];
			 szStr[j] = NULL;
			 k++;
			 RAMPINI.nCtrlLinesStdval[i][l++] = atoi(szStr);
		 }
	}
	for(i=0;i<MAXREADINGPOS;i++)
	{
		sprintf(sztStr,"RAMP_POS%d",i+1);
		  sprintf(path,"%s:\\prog.ini",cRamDrive);//22012001
		GetPrivateProfileString("MEASUREMENT_LINES",sztStr,szStdVal,0,path);
		strcpy(szTmp, szStdVal);
		j=0;  k=0;  l=0;
		while(k<strlen(szTmp))
		{
			for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
				szStr[j] = szTmp[k++];
			szStr[j] = NULL;
			k++;
			RAMPINI.nCtrlLinesReading[i][l++] = atoi(szStr);
		}
	}
	 sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	RAMPINI.nAverageof=GetPrivateProfileInt("AVERAGE","RAMP_AVG",250,path);

	RAMPINI.nLength=GetPrivateProfileInt("RAMP_LENGTH","LIMIT",2500,path);
	ElfextLength=GetPrivateProfileInt("ELFEXT_LENGTH","ELFEXT_LIMIT",2500,path);

	//pin replacement start
	GetPrivateProfileString ("REPLACE_FIX_POS","PIN_NO",szStdVal,"","default.ini");
	strcpy(szTmp, szStdVal);
	memset( pinrep, 0, sizeof(pinrep) );
	pin = 0;
	j=0;  k=0;  l=0;
	while(k<strlen(szTmp)&&pin<MAXREPLACELEN) //4 //Updated by:sdt:30112014:1610
	{
		for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
			szStr[j] = szTmp[k++];
		szStr[j] = NULL;
		k++;
		temp = atoi(szStr);
		if ( temp>0&&temp<=101 )
			pinrep[pin++][0] = temp;
	}
	GetPrivateProfileString ("REPLACE_FIX_POS","REP_WITH",szStdVal,"","default.ini");
	strcpy(szTmp, szStdVal);
	pin = 0;
	j=0;  k=0;  l=0;
	while(k<strlen(szTmp)&&pin<MAXREPLACELEN) //4 //Updated by:sdt:30112014:1610
	{
		for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
			szStr[j] = szTmp[k++];
		szStr[j] = NULL;
		k++;
		temp = atoi(szStr);
//		if ( temp>100&&temp<=104 )
			pinrep[pin++][1] = temp;
	}
	//Commented by:sdt:30112014:1615
	/*for(i=0;i<MAXPAIRS;i++)
	{
		for( j=0;j<4; j++ )
			if ( pinrep[j][0] == i+1 )
				pinReplace.nPinsReplace[i]=pinrep[j][0],pinReplace.nPinsReplacewith[i]=pinrep[j][1];
	}*/
	//commented by:sdt:26042015:1700//Modified & Added by:sdt:30112014:1615
	//for(i=0;i<MAXREPLACELEN;i++)
	//{
	//	pinReplace.nPinsReplace[i]=pinrep[i][0],pinReplace.nPinsReplacewith[i]=pinrep[i][1];
	//}

/*	for(i=0;i<MAXPAIRS;i++)
	{
		 sprintf(sztStr,"PIN_%d",i+1);
		 pinReplace.nPinsReplace[i]=GetPrivateProfileInt("REPLACE_FIX_POS",sztStr,-1,"g:\\default.ini");
		 if ((pinReplace.nPinsReplace[i] != -1) &&
			(pinReplace.nPinsReplace[i]>=1 && pinReplace.nPinsReplace[i]<=MAXPAIRS))
		 {
			for (j = 0;j < i;j++)
				if (pinReplace.nPinsReplace[j]==i || pinReplace.nPinsReplacewith[j]==i)
					NoReplace=TRUE;
			if (NoReplace==FALSE)
				pinReplace.nPinsReplacewith[i]=i+1;
			else
				NoReplace=FALSE;
		 }
		 else
			pinReplace.nPinsReplace[i]=0;
	}
	*/
	gMaxPinsReplaced=i;
	//pin replacement end
	for(i = 0; i < MAXFREQUENCIES; i++)
	{
		sprintf(sztStr,"%sRef_FREQ%d", paraminfo[SRNO_ATN].szParamAbr, i+1);
		  sprintf(path,"%s:\\cts.ini",cRamDrive);
		GetPrivateProfileString("STD_VAL", sztStr,szStdVal,"",path);
		fAtnRefStd[i] = atof(szStdVal);
	}

	GetParamDetails();//Added:jj:02052001

	ReadHFHLFreqInfo(); //Added by:sdt:17052001
	sprintf(path,"%s:\\cts.ini",cRamDrive);
	//Commented by:sdt:28082001
	//HFLHFLfreqLine = GetPrivateProfileInt("HFH_HFL_LINE_INFO","LINENO",0,path);;
	HfhHflFreqLines.VIunit = GetPrivateProfileInt("HFH_HFL_LINE_INFO","LINENO_FOR_VIUNIT",0,path);
	HfhHflFreqLines.HFunit = GetPrivateProfileInt("HFH_HFL_LINE_INFO","LINENO_FOR_HFUNIT",0,path);
}

void GetParamDetails()
{
	 char path[MAXPATH];
	 sprintf(path,"%s:\\cts.ini",cRamDrive); //added by:sdt:18062001
     Param.Param_Start = GetPrivateProfileInt("PARAMS","PARAM_START",0,path);
     Param.Param_End = GetPrivateProfileInt("PARAMS","PARAM_END",0,path);
     Param.Lf_Param_Start = GetPrivateProfileInt("PARAMS","LF_PARAM_START",0,path);
     Param.Lf_Param_End = GetPrivateProfileInt("PARAMS","LF_PARAM_END",0,path);
     Param.Hf_Param_Start = GetPrivateProfileInt("PARAMS","HF_PARAM_START",0,path);
     Param.Hf_Param_End = GetPrivateProfileInt("PARAMS","HF_PARAM_END",0,path);
}


//This function will read the starting & ending frequency numbers
// for HF Low & HF High frequencies from cts.ini.

void ReadHFHLFreqInfo()
{
	char Path[MAXPATH];
	sprintf(Path,"%s:\\cts.ini",cRamDrive);
    HFfreqInfo.HFLstart = GetPrivateProfileInt("HF_LOW_FREQUENCY","START_FREQ_NUMBER",0,Path);
    HFfreqInfo.HFLend = GetPrivateProfileInt("HF_LOW_FREQUENCY","END_FREQ_NUMBER",0,Path);
    HFfreqInfo.HFHstart = GetPrivateProfileInt("HF_HIGH_FREQUENCY","START_FREQ_NUMBER",0,Path);
    HFfreqInfo.HFHend = GetPrivateProfileInt("HF_HIGH_FREQUENCY","END_FREQ_NUMBER",0,Path);
}

void ReadRunSetupINI(void)
{
	char path[80], szStr[25], *p;
	sprintf(path,"%s:\\runsetup.ini",cRamDrive);
	for(int i=0;i<MAXPARAMETERS;i++)
	{
		GetPrivateProfileString ("RUN_SETUP",paraminfo[i].szParamAbr,szStr,"\0",path);
		if(strcmp(szStr,"\0")!=0)
		{
			p = strtok (szStr,",");
			runSetup.ulParamRunSetup[i]|=PARAMSELECTIONMASK*(BYTE)((atoi(p)!=0&&atoi(p)!=1)?0:atoi(p));
			//runSetup.mode |= 0x10*(BYTE)((i!=0&&i!=1)?0:i);
			p = strtok (NULL,",");
			for(int j=1; (j<=MAXFREQUENCIES)&&(p!=NULL);j++)
			{
				runSetup.ulParamRunSetup[i]|=(0x1l<<(((j)*3)-1))*(BYTE)((atoi(p)!=0&&atoi(p)!=1)?0:atoi(p));
				p = strtok (NULL,",");
			}
		}
		//TempRunSetup.ulParamRunSetup[i] =  runSetup.ulParamRunSetup[i];//Added by:sdt:06022007:2215
	}
	TempRunSetup =  runSetup;//Added by:sdt:06022007:2215

}

//Function added by:sdt:16102005:1420
//This function will read default Report Setup from CTS.INI
void ReadReportSetup()
{
	FILE *frpstp;
	if ((frpstp = fopen("rpsetup.bin", "rb")) != NULL) //Added by:sdt:15122016:2130
	{
		fread(&reportSetup, sizeof(ReportSetup),1,frpstp); //Added by:sdt:15122016:2130
	}
	else
	{
		int j;
		char path[80];
		sprintf(path,"%s:\\cts.ini",cRamDrive);
		for(int i=0;i<MAXPARAMETERS;i++)
		{
			j = GetPrivateProfileInt( "REPORT_SETUP", paraminfo[i].szParamAbr, 0, path );
			reportSetup.m_Mode |= (0x01<<i)*(BYTE)((j!=0&&j!=1)?0:j);
		}
		j = GetPrivateProfileInt( "REPORT_SETUP", "NORMALISED", 0, path );
		reportSetup.m_Type |= 0x01*(BYTE)((j!=0&&j!=1)?0:j);
		j = GetPrivateProfileInt( "REPORT_SETUP", "UNNORMALISED", 0, path );
		reportSetup.m_Type |= 0x02*(BYTE)((j!=0&&j!=1)?0:j);
		j = GetPrivateProfileInt( "REPORT_SETUP", "REMARK", 0, path );
		if(j==1)
		{
			reportSetup.m_Remark = True;
		}
		else
		{
			reportSetup.m_Remark = False;
		}
		j = GetPrivateProfileInt( "REPORT_SETUP", "DETAILED", 0, path );
		if(j==1)
		{
			reportSetup.m_SumDet = True;
		}
		else
		{
			reportSetup.m_SumDet = False;
		}
	}
	if(frpstp){fclose(frpstp);}
}
