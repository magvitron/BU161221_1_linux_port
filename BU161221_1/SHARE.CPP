#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include "math.h"
#include "alloc.h"
#include <process.h>
#include <bios.h>
#include "borland_compat.h"

#include "ct.h"
#include "share.h"
#include "copyrite.h"
#include "funproto.h"
#include "rdsch.h"
#include "utils.h"
#include "key.h"
#include "setup.h"
#include "frontpg.h"
//Global Variable //Start
//Global variables of Turbo Vision classes
//start
extern TFrontPage *fPage; //Added by:sdt:27092005:2300
//end
//Global variables for share.cpp
//start
extern int 			nCount;
unsigned int	paramValAF_Status=0;
int nBothFlag;
int             gnPair1,gnPair2;
static int    	nPrevFix[3],nPrevFix1[3];
static int    	nNextFix[3],nNextFix1[3];
float           fAtnCalRef;
float			fAtnOffset;
float			fReadAtnStd;

//float          	fRefVal[110][MAXFREQUENCIES];// Added by:sdt:23072001
//Modified by:sdt:16102005:1715 //For 6 Quad cable
float          	fRefVal[MAXPAIRS][MAXFREQUENCIES];// Added by:sdt:23072001
int 			nIntNo,nDecNo;
int 			NormalReadingsOver;
int 			nAdjNonAdjFlag;
float t1,t2,CALLOW,CALHIGH;//Replaced:jj:14032001
PAIRSWITCH 		PairSwitchOption;//Added by:sdt:22042001
DispStruct dispstruct[MAXPARAMETERS];//Uncommented by:sdt:02112006:1035//Commented by:sdt:16102005:1840 //As it is no longer used
SEQPAIR *SeqPair;
//end
extern int   nCurrentRange;
extern int     over;
extern int     nFixtures;
extern int 		   factor;
extern char szRangeHold[2];
extern int	nflagRangeLock;
extern int  nGiveAlarms;
extern char     defaultmessage[];
extern int      flagReset;
extern int   nParamCount;
//extern int   DisplayArray[MAXPARAMETERS];//Commented by:sdt:16102005:1840 //As it is no longer used
extern float fAtnRefStd[MAXFREQUENCIES];
extern int compartsizeinunits;
extern int Pcm_Cable;
extern int nTempUnitOf;
extern char cRamDrive[2];
extern int ZchVal;//09042001
extern int noofparameters;
extern int ParamCtsHandle;
extern char			bIncOkRej;
extern float			gfOffset[MAXRANGES];
extern float           fFcal[MAXRANGES];
extern int				nPrevPair1,nPrevPair2;
extern int             gflagReplace1,gflagReplace2;
extern int             nLastUnitSize,nUnits,nNeedOfAppend;

extern READING far		*fCrPairVal;
extern READING far		*fCmPairVal;

extern SEQUENCE CableSequence;
extern Hz_80_INFO Hz_80_Info;
extern RunSetup runSetup;
extern CTPAIR         CtPair;
extern REPLACE     	pinReplace[FIXTUREBOTH+1]; //Modified by:sdt:26042015:1700
extern Calibration  	Cal;
extern FIXTURE	Fixture[3];
extern RdControl       rdCtrl;
extern RMPINIINFO  	RAMPINI;
extern INFORMATION 	m_FileInfo;
extern CONFIG nConfig;
extern PRM *prmarray;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2145
extern int bSimulateAdc,nCurrentReadingno;
extern FreqStruct HFFrequency,LFFrequency; //Added by:sdt:19032001
extern HFHHFLFREQLINES HfhHflFreqLines;
extern HFFREQUECYINFO HFfreqInfo;
extern Zms_Resistance Zms_Res;
// For 7-SEGMENT DISPLAY
int SegmentDataLines[8];
int SegmentLatchLines[12];

unsigned char BITS[8]  = {0x01,0x2,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char PORTS[4] = {0xFC,0xFE,0xFD,0xFF};
unsigned char MASK[8]  = {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};
unsigned char STATUS[90];

CHIPINFO CHIPS[MAX_MUX_ICS] =
{
	{IC2_PORT_B, 0}, // 0
	{IC2_PORT_B, 1}, // 1
	{IC2_PORT_B, 2}, // 2
	{IC2_PORT_B, 3}, // 3
	{IC2_PORT_B, 4}, // 4
	{IC2_PORT_B, 5}, // 5
	{IC2_PORT_B, 6}, // 6
	{IC2_PORT_B, 7}, // 7
	{IC2_PORT_C, 0}, // 8
	{IC2_PORT_C, 1}, // 9
	{IC2_PORT_C, 2}, // 10
	{IC2_PORT_C, 3}, // 11
	{IC2_PORT_C, 4}, // 12
	{IC3_PORT_A, 2}, // 13
	{IC3_PORT_A, 3}, // 14
	{IC2_PORT_C, 5}, // 15
	{IC2_PORT_C, 6}, // 16
	{IC2_PORT_C, 7}, // 17
	{IC3_PORT_B, 0}, // 18
	{IC3_PORT_B, 1}, // 19
	{IC3_PORT_B, 2}, // 20
	{IC3_PORT_B, 3}, // 21
	{IC3_PORT_B, 4}, // 22
	{IC3_PORT_B, 5}, // 23
	{IC3_PORT_B, 6}, // 24
	{IC3_PORT_B, 7}, // 25
	{IC3_PORT_C, 2}, // 26
	{IC3_PORT_C, 3}, // 27
	{IC3_PORT_A, 4}, // 28
	{IC3_PORT_A, 5}, // 29
};


//Global Variable //End

int ReadParamDotCts( char szPrmAbr[MAXPARAMABRNAME], PRM *prm )
{
	int i;// ParamCtsHandle=0 ;
	int nParamCount=-1;
	int nRead=0;

	//Commented by:sdt:31072005:2330
	if ( ParamCtsHandle <= 0 ){
		clrscr();
		printf( "\nError File Handle NULL Used for Reading PARAM.CTS File..." );
		RemoveIniFiles();
		exit( 1 );
	}
	for( i = 0; i < noofparameters; i++ ){
	//Modifications done :jj:05042001
		//if ( strcmp( paraminfo[i].szParamAbr, szPrmAbr ) == 0 ){
	   if ( strcmp( paraminfo[i].szParamAbr, szPrmAbr ) == 0 ){
			nParamCount = paraminfo[i].ParamNo;
			break;
		}
	}
	if ( nParamCount == -1 ){
		clrscr();
		printf( "\n%s - Not Found in PARAM.CTS File...", szPrmAbr );
		RemoveIniFiles();
		exit( 1 );
	}
	lseek ( ParamCtsHandle, 0L, SEEK_SET );
	if ( nParamCount )
		lseek ( ParamCtsHandle, sizeof( PRM ) * nParamCount, SEEK_CUR );
	nRead = read( ParamCtsHandle, prm, sizeof( PRM ) );
	if ( !nRead ){
		clrscr();
		printf( "\nError Reading PARAM.CTS File for parameter - %s", szPrmAbr );
		RemoveIniFiles();
		exit( 1 );
	}
	return nRead;
}

int ReadParamDotHdr( void )
{
	int i =0;
	int nRead=1;//, ParamCtsHandle;
	FILE *fpHdr;
	//FILE *fpTmp;

	memset( &paraminfo, 0, sizeof( paraminfo ) );
	fpHdr = fopen( "PARAM.HDR", "rb" );
	if ( fpHdr == NULL ){
		clrscr();
		printf( "\nCannot Open PARAM.HDR File..." );
		RemoveIniFiles();
		exit( 1 );
	}
	eatcopyright (fpHdr);
	//	fpTmp = fopen( "HDR.TXT", "w" );//Change:Remove Immegiately
	fread( &noofparameters, sizeof( noofparameters ), 1, fpHdr );
	//fprintf(fpTmp,"No of parameters : %d\n",noofparameters);
//	fwrite(&noofparameters,sizeof( noofparameters ), 1, fpTmp);
	if ( noofparameters <= 0 ){
		clrscr();
		printf( "\nNo. of Parameters = 0 in PARAM.HDR File..." );
		RemoveIniFiles();
		exit( 1 );
	}
	if ( noofparameters < MAXPARAMETERS ){
		clrscr();
		printf( "\nNo. of Parameters Less than MAXPARAMETERS in PARAM.HDR File..." );
		RemoveIniFiles();
		exit( 1 );
	}

    //Added:jj:05042001
	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
	{
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		RemoveIniFiles();
		exit( 1 );
	}


	for(int i = 0; i < noofparameters && nRead; i++)
		{
			nRead = fread(&paraminfo[i],	sizeof(PARAMINFO), 1,fpHdr);
		  //paraminfo1[i]=paraminfo[i];//Added:jj:05042001//Commented by:sdt:31072005:2300
		  //Added:jj:05042001 for mutiple mmt for same parameter eg. Zch
		  //ReadParamDotCts( paraminfo[i].szParamAbr , &prmarray);

		  ReadParamDotCts( paraminfo[i].szParamAbr , prmarray);//Modified:jj:05042001
		  strcpy(paraminfo[i].szParamAbr,prmarray->szParamNameAbr);

		}
	if ( !nRead ){
		clrscr();
		printf( "\nError Reading Parameter Information from PARAM.HDR File..." );
		RemoveIniFiles();
		exit( 1 );
	}
	if(fpHdr){fclose(fpHdr);}
    close( ParamCtsHandle );
//	if(fpTmp){fclose(fpTmp);}
// if successfull Open ParamCtsHandle file handle


	//Commented:jj:05042001
	if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1){
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		RemoveIniFiles();
		exit( 1 );
	}
	/////////////// remove immegiatelly
   /*	ReadParamDotCts( "CR", &prmarray[0] );
	fpTmp=fopen("cts.txt","w");
	fwrite(&prmarray[0],sizeof(PRM),1,fpTmp);
	if(fpTmp){fclose(fpTmp);}*/
	//////////////
	return i;
}


void InitializeDIOcard ()
{
	int i;

    //Bellow changes done by:sdt:18062001
    //P2 Port initialization method changed.
	//IC 1 is initialized as All Ports in Output Mode.
	//IC 2 is initialized in Input/Output Mode.
    outportb (IC0_PORT_CWR, ALLPORTSOUT);//MODE0IO);//write control byte on IC P0.
    //Modified by:sdt:19062001
    //Port P2's IC 2 initialized in All Port Output Mode.
	//outportb (IC1_PORT_CWR, ALLPORTSOUT);//MODE0IO);//MODE0CIN) ;//write control byte on IC P1.
    //Above statement commented by:sdt:20062001.

    outportb (IC1_PORT_CWR, MODE0IO); //Added by:sdt:20062001
    //Port P2's Line no 50,49 used as input for Break & short
	// and Ramp Sync respectively. Due to this in Port P2 8255 IC2's
	// Port C (Lower) is Kept in Input mode.
    //Above statement modified by:sdt:28062001
    //As line numbers 47,48,49 & 50 of port2 are the Port c's Higher
    //bits. So now initialization mode is changes & now Port c's
    //Higher bits are initialized in Input Mode.
	//
	outportb(IC2_PORT_CWR, ALLPORTSOUT);    // Write Control byte to IC P2
	outportb(IC3_PORT_CWR, ALLPORTSOUT);    // Write Control byte to IC P3
	outportb(IC4_PORT_CWR, ALLPORTSOUT);    // Write Control byte to IC P4
	outportb(IC5_PORT_CWR, ALLPORTSOUT);    // Write Control byte to IC P5

	 //  Initialization of 30 DMux Ics  and Send data to all ICs
	for(i=0;i<MAX_MUX_ICS;i++)
	{
		SendDataToDMux(i,PORT_CWR,ALLPORTSOUT);
		SendDataToDMux(i,PORT_A,LOW);
		SendDataToDMux(i,PORT_B,LOW);
		SendDataToDMux(i,PORT_C,LOW);
	}
	for (i=0;i<90;i++)
		STATUS[i] = LOW;
	// For Digital Ramp

}

void SendDataToDMux (int nChipno,int nPortno,unsigned char cData)
{
	unsigned char   bits,controlword;
	unsigned char   cMask=0x00;

	outportb(DATA_PORT, 0x00);                // write data
	bits = HIGH & MASK[CHIPS[nChipno].MaskNumber];
	if(prmarray->nCtrlLinesFix[0])
	{
		if(prmarray->nCtrlLinesFix[0] == 93)
			cMask |= 0x01;
		if(prmarray->nCtrlLinesFix[0] == 94)
			cMask |= 0x02;
		if(prmarray->nCtrlLinesFix[0] == 97)
			cMask |= 0x10;
	}
	if(prmarray->nCtrlLinesFix[1])
	{
		if(prmarray->nCtrlLinesFix[1] == 93)
			cMask |= 0x01;
		if(prmarray->nCtrlLinesFix[1] == 94)
			cMask |=0x02;
		if(prmarray->nCtrlLinesFix[1] == 97)
			cMask |= 0x10;
	}
	if(prmarray->nCtrlLinesFix[2])
	{
		if(prmarray->nCtrlLinesFix[2] == 93)
			cMask |= 0x01;
		if(prmarray->nCtrlLinesFix[2] == 94)
			cMask |= 0x02;
		if(prmarray->nCtrlLinesFix[2] == 97)
			cMask |= 0x10;
	}
	if(nChipno > 25 && nChipno < 28)
	{
		bits &= 0xEC;
		bits |=cMask;
	}
	if(CHIPS[nChipno].PortNumber == CONTROL_PORT)
		controlword = bits & WR_HIGH & PORTS[nPortno];
	else
	{
		controlword = HIGH & WR_HIGH & PORTS[nPortno];
		outportb(CHIPS[nChipno].PortNumber, bits);    // chip selection
	}
	outportb(CONTROL_PORT, controlword);
	outportb(DATA_PORT, cData);                // write data
	outportb(CONTROL_PORT, controlword & WR_LOW);
	outportb(CONTROL_PORT, controlword & WR_HIGH);
	bits |= 0xEC;
	if(nChipno > 25 && nChipno < 28)
		outportb(CHIPS[nChipno].PortNumber, bits);      // Chip Select High
	else
		outportb(CHIPS[nChipno].PortNumber, HIGH);      // Chip Select High
}

void SetDMuxPin (int nPinNumber,int bOnOff)
{
	int             nChipNumber,nPortNumber,nBitPosition;
	unsigned char   cData;

	if(nPinNumber > 720)
		return;
	nChipNumber =nPinNumber/MAX_LINESPERIC;
	nPortNumber =(nPinNumber/MAX_PORTLINES)%MAX_PORTSINIC;
	nBitPosition=nPinNumber%MAX_PORTLINES;
	if(bOnOff)
		cData = STATUS[nChipNumber*MAX_PORTSINIC+nPortNumber]|(0x1<<nBitPosition);
	else
		cData=STATUS[nChipNumber*MAX_PORTSINIC+nPortNumber]&~(0x1<<nBitPosition);
	SendDataToDMux(nChipNumber,nPortNumber, cData);
	STATUS[nChipNumber*MAX_PORTSINIC+nPortNumber] = cData;
}

void Set_DIOFRC_Pin (int nLineNumber,int bOnOff)
{
	int             nDIOFRCNumber,nPortNumber,nBitPosition;
	unsigned char   cData;

	//if(nLineNumber > MAXFRCLINES)//Commented by:sdt:14082001
    //Following Added by:sdt:14082001.
    //Now user can use Dmux Lines in the MMTs. To avoid the confusion
	//between Demux & Direct lines, Demux lines are started with the
	//1001. Here 1001 => demux line no.1 ,1002 => Demux line no.2 and so on.
	//Direct Lines are starts with 1. 1 => direct line 1. and so on.
    //Lines between 150-1000 are considered as direct lines.
    if(nLineNumber > MAX_DIRECT_LINES)
    {
        (nLineNumber -=MAX_DIRECT_LINES)--;
		SetDMuxPin(nLineNumber, bOnOff);
    }
	else
	{
		--nLineNumber;
		if(nLineNumber/MAX_FRC_PINS == FRC_PORT2)
			nDIOFRCNumber = FRC_PORT2;
		else
			 if(nLineNumber/MAX_FRC_PINS == FRC_PORT3)
			 {
				nDIOFRCNumber = FRC_PORT3;
				nLineNumber -= MAX_FRC_PINS;
			 }
			 else
				if(nLineNumber/MAX_FRC_PINS == FRC_PORT4)
				{
					nDIOFRCNumber = FRC_PORT4;
					nLineNumber -= (MAX_FRC_PINS*2);
				}
		if(nLineNumber > MAX_LINESPERIC)
			nLineNumber -= 2;
		nPortNumber  = nLineNumber/MAX_PORTLINES + nLineNumber/MAX_LINESPERIC;
		nBitPosition = nLineNumber%MAX_PORTLINES;
		cData = inportb(DIOBASE+nPortNumber+nDIOFRCNumber*MAX_PORTLINES);
		if(bOnOff)
			cData = cData | BITS[nBitPosition];
		else
			cData = cData & (HIGH ^ BITS[nBitPosition]);
		outportb(DIOBASE+nPortNumber+nDIOFRCNumber*MAX_PORTLINES,cData);
	}
}

int Get_DIOFRC_Pin ( int nLineNumber)
{
	int             nDIOFRCNumber,nPortNumber,nBitPosition;
	unsigned char   cData;

	--nLineNumber;
		if(nLineNumber/MAX_FRC_PINS == FRC_PORT2)
			nDIOFRCNumber = FRC_PORT2;
		else
			 if(nLineNumber/MAX_FRC_PINS == FRC_PORT3)
			 {
				nDIOFRCNumber = FRC_PORT3;
				nLineNumber -= MAX_FRC_PINS;
			 }
			 else
				if(nLineNumber/MAX_FRC_PINS == FRC_PORT4)
				{
					nDIOFRCNumber = FRC_PORT4;
					nLineNumber -= (MAX_FRC_PINS*2);
				}
		if(nLineNumber > MAX_LINESPERIC)
			nLineNumber -= 2;
		nPortNumber  = nLineNumber/MAX_PORTLINES + nLineNumber/MAX_LINESPERIC;
		nBitPosition = nLineNumber%MAX_PORTLINES;
		cData = inportb(DIOBASE+nPortNumber+nDIOFRCNumber*MAX_PORTLINES);

		if (cData & BITS[nBitPosition])
			return TRUE;
		return FALSE;
}


int InitializeADCcard ()
{
	int   nEnd,nAdcTimeout;

	outportb(ADC_PORTCHANNEL,ADC_CHANNEL1);
	outportb(ADC_PORTTRIGGER,ADC_TRIGGER);
	nEnd = ADC_EOC + 1;
	nAdcTimeout = 0;
	while(nEnd > ADC_EOC)
	{
		nAdcTimeout++;
		nEnd = inportb(ADC_PORTHIGH);
		if(nAdcTimeout == ADC_TIMEOUT)
		{
			bSimulateAdc = TRUE;
			return FALSE;
		}
		else
			bSimulateAdc = FALSE;
	}
	return TRUE;
}


READING far* GetPrevParamVals( READING far *pparamVal, int param ,int nFreqNum)
{
	FILE *fpParam;
	char fileName[16];
	int nRead=1;
	READING reading,readingr;

	if ( !(paramValAF_Status&(unsigned int)pow(2,param)) )
	{
		pparamVal = (READING far *)farmalloc( sizeof(READING) * \
									atoi(nConfig.szNoofpairs) );
		if ( pparamVal == NULL )
		{
			printf( "\nCannot Allocate Memory for %sReadings...", paraminfo[param].szParamAbr );
			RemoveIniFiles();
			exit(1);
		}
		paramValAF_Status |= (unsigned int)pow(2,param);
	}
	_fmemset( pparamVal, 0, sizeof(READING) * atoi(nConfig.szNoofpairs) );
	//sprintf( fileName, "%s:\\%d.DAT",RamDrv,param );//Commented by:sdt:20032001
    sprintf( fileName, "%s:\\%dFREQ_%d.DAT",cRamDrive,param,nFreqNum);//Added by:sdt:20032001
	fpParam = fopen( fileName, "rb" );
	if ( fpParam == NULL )
	{
		FreePrevParamVals(pparamVal, param);
		printf( "\nCannot Open %s.DAT File...", paraminfo[param].szParamAbr );
		RemoveIniFiles();
		exit(1);
	}
	int noofreadings;

    //for(int bypass=0;(int)m_FileInfo.cPairsBypassed[bypass]!=127;bypass++);
	noofreadings = atoi(nConfig.szNoofpairs);
	fseek( fpParam, 0, SEEK_SET );
	for( int i = 0,j=1,bpass=0; i < noofreadings; i++ ,j++)//Added:pnb:j:17062000
	{
	   if(m_FileInfo.cPairsBypassed[bpass]==j )//&& param==SRNO_ATN)//Added:pnb:for bypass pairs:17062000
	   {
		  reading.cPair1=i+1;
		  reading.cPair2=0;//Storing 0 in bypass pairs.
		  //reading.cParam=SRNO_ATN;
		  reading.cParam=param;
		reading.nFreq=nFreqNum;//Added by:sdt:23032001
		  reading.fRawVal=0.0;
		  reading.fNormVal=0.0;

		  pparamVal[i] = reading;
		  bpass++;
		  continue;
	   }
		nRead = fread( &reading, sizeof(READING), 1, fpParam );
		if ( param == SRNO_CR )
		{
			nRead = fread( &readingr, sizeof(READING), 1, fpParam );
			pparamVal[i].cPair1 = reading.cPair1;
			pparamVal[i].cPair2 = reading.cPair2;
			pparamVal[i].cParam = reading.cParam;
            pparamVal[i].nFreq = reading.nFreq; //Added by:sdt:23032001
			pparamVal[i].fRawVal = reading.fRawVal + readingr.fRawVal;
			pparamVal[i].fNormVal = reading.fNormVal + readingr.fNormVal;
		}
		else
		{
			pparamVal[i] = reading;
		}
	}
	if ( !nRead )
	{
		FreePrevParamVals(pparamVal, param );
		if(fpParam){fclose(fpParam);}
		printf( "\nError Reading %s.DAT File...", paraminfo[param].szParamAbr );
		RemoveIniFiles();
		exit(1);
	}
	if(fpParam){fclose(fpParam);}
	return pparamVal;
}

READING far* FreePrevParamVals( READING far *pparamVal, int param)
{
	if ( paramValAF_Status&(unsigned int)pow(2,param) )
	{
		farfree( pparamVal );
		paramValAF_Status &= (unsigned int)!(pow(2,param));
		return pparamVal;
	}
	else
		return NULL;
}


float CalculateRu( float dRaNor, float dRbNor, unsigned char uRuMmtMethod )
{
	float dRu=0.0;
	switch((unsigned int)uRuMmtMethod)
	{
	case RU_FORMULA_1 :
		dRu=(dRaNor>dRbNor?dRaNor:dRbNor)-(dRaNor>dRbNor?dRbNor:dRaNor);
		dRu /= ((dRaNor+dRbNor)?dRaNor+dRbNor:1);
		dRu *= 100;
		break;
	case RU_FORMULA_2 :
		dRu=(dRaNor>dRbNor?dRaNor:dRbNor)-(dRaNor>dRbNor?dRbNor:dRaNor);
		{float f1 = ((dRaNor<=dRbNor)?dRaNor:dRbNor);
		 dRu /= (f1?f1:1.0);}
		dRu *= 100;
		break;
	case RU_FORMULA_3 :
		dRu=(dRaNor>dRbNor?dRaNor:dRbNor)-(dRaNor>dRbNor?dRbNor:dRaNor);
		{float f1 = ((dRaNor>=dRbNor)?dRaNor:dRbNor);
		 dRu /= ((f1)?f1:1.0);}
		dRu *= 100;
		break;
	case RU_FORMULA_4 :
		dRu=(dRaNor>dRbNor?dRaNor:dRbNor)-(dRaNor>dRbNor?dRbNor:dRaNor);
		break;
	}
	return ( fabs((double)dRu) );
}

void InitializeSegmentLines()
{
	char path[80];//22012001
	int i;
	char TmpBuf[40];
	for(i=0;i<4;i++)
	{
		sprintf(TmpBuf,"LINE_%d",i+1);
		sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
		SegmentDataLines[i]=GetPrivateProfileInt ("SEVEN_SEGMENT_DATA_LINES_FIX_1",TmpBuf,800,path);
	}
	for(i=0;i<4;i++)
	{
		sprintf(TmpBuf,"LINE_%d",i+1);
		SegmentDataLines[i+4]=GetPrivateProfileInt ("SEVEN_SEGMENT_DATA_LINES_FIX_2",TmpBuf,800,path);
	}
	for(i=0;i<6;i++)
	{
		sprintf(TmpBuf,"LINE_%d",i+1);
		SegmentLatchLines[i]=GetPrivateProfileInt ("SEVEN_SEGMENT_LATCH_LINES_FIX_1",TmpBuf,800,path);
	}
	for(i=0;i<6;i++)
	{
		sprintf(TmpBuf,"LINE_%d",i+1);
		SegmentLatchLines[i+6]=GetPrivateProfileInt ("SEVEN_SEGMENT_LATCH_LINES_FIX_2",TmpBuf,800,path);
	}
}

//Commented by:sdt:13112005:1430
/*void SetImpedence(int Val)
{
	//int RestoreZch = ZchVal; //Commented by:sdt:08082001
	//ZchVal/=2;//Added by:sdt:30072001//Commented by:sdt:08082001
	int temp=(ZchVal%10);//Added by:sdt:29072001
	int temp1,i,BinPos=1;
	unsigned int ValueToOut;
	temp1=10-temp;
	if(temp<5)
		ZchVal=ZchVal-temp;
	else
    	ZchVal=ZchVal+temp1;
	ValueToOut=unsigned (ZchVal/10);

    //temp3=~(ValueToOut); //Modified by:sdt:28062001.
    // In hardware the impedance logic is set in Inverted mode.
    // For this Above modification is done.i.e. if impedance 20� is set
    // then LED for 20� is OFF & others will be ON.
    // sdt:08082001
	// Modification done in hardware & inverted impedence logic is
	// removed. For this modification done as follows.
    for(i=0;i<8;i++)
    {
        //Modification done by:sdt:08082001
        //if(~(ValueToOut) & BinPos)
        if(ValueToOut & BinPos)
            Set_DIOFRC_Pin(Zms_Res.Lines[i],ON);
	    else
            Set_DIOFRC_Pin(Zms_Res.Lines[i],OFF);
	    //temp3=ValueToOut;
		BinPos=BinPos<<1;
    }
    //ZchVal = RestoreZch; //Added by:sdt:31072001 //Commented by:sdt:08082001
    //As ZchVal is global variable, one call of function setimedence()
	//will make its value half. Next call to one forth. So to reserve
	//original value above statement is used.
    //sdt:08082001
    //Modification done in hardware. Now logic of making impedence value
	// half is not required.
}

void GetActualImpedence()
{
	int temp=(ZchVal%10);
	int temp1;
	temp1=10-temp;
	if(temp<5)
		ZchVal=ZchVal-temp;
	else
		ZchVal=ZchVal+temp1;
}*/

//Function Added by:sdt:18032001
//Modified by:sdt:19032001
void SetLowFreq (int nFreq)
{
    int i,nPinno=0;
    for (i=0;i<LFFrequency.nTotalFreqLines;i++)
    {
	    nPinno=LFFrequency.nFreqLines[i];
        if(LFFrequency.nStatusofFreqLines[nFreq].nFreqLines[i]==1)
    	   	Set_DIOFRC_Pin(nPinno,ON);
        else
			Set_DIOFRC_Pin(nPinno,OFF);
    }
}


//Function Added by:sdt:18032001
//Modified by:sdt:19032001
void SetHighFreq (int nFreq)
{
    int i,nPinno=0;
    if(HFfreqInfo.HFLstart <=(nFreq+1)  && HFfreqInfo.HFLend >= (nFreq+1))
    {
        //nPinno = HFLHFLfreqLine;//Commented by:sdt:28072001
        //Modification done by:sdt:28072001
        nPinno = HfhHflFreqLines.VIunit; //Select HFL freq in VI unit.
    	Set_DIOFRC_Pin(nPinno,ON);
        nPinno = HfhHflFreqLines.HFunit; //Select HFL Freq in HFL unit.
    	Set_DIOFRC_Pin(nPinno,ON);
    }
    else
    {
        //nPinno = HFLHFLfreqLine;//Commented by:sdt:28072001
        //Modification done by:sdt:28072001
        nPinno = HfhHflFreqLines.VIunit; //Select HFH freq in VI unit.
    	Set_DIOFRC_Pin(nPinno,OFF);
        nPinno = HfhHflFreqLines.HFunit; //Select HFH Freq in HFH unit.
        Set_DIOFRC_Pin(nPinno,OFF);
    }

    for (i=0;i<HFFrequency.nTotalFreqLines;i++)
    {
	    nPinno=HFFrequency.nFreqLines[i];
        if(HFFrequency.nStatusofFreqLines[nFreq].nFreqLines[i]==1)
    	   	Set_DIOFRC_Pin(nPinno,ON);
        else
			Set_DIOFRC_Pin(nPinno,OFF);
    }
}

void ResetSystem ()
{
	int i;

	//  Initialization of 30 DMux Ics  and Send data to all ICs
	for(i=0;i<MAX_MUX_ICS;i++)
	{
		SendDataToDMux(i,PORT_CWR,ALLPORTSOUT);
		SendDataToDMux(i,PORT_A,LOW);
		SendDataToDMux(i,PORT_B,LOW);
		SendDataToDMux(i,PORT_C,LOW);
	}
	for (i=0;i<90;i++)
		STATUS[i] = LOW;
	for(i=0;i<50;i++)
        //Commented by:sdt:16082001
		//if(i!=39&&i!=HF1024KZPINNO&&i!=HF772KZPINNO&&i!=HF150KZPINNO&&i!=LF1KZPINNO
        if(i!=HF1024KZPINNO&&i!=HF772KZPINNO&&i!=HF150KZPINNO&&i!=LF1KZPINNO
			&&i!=LF800HZPINNO )
			Set_DIOFRC_Pin(i, OFF);
	for(i=100;i<=150;i++)
		if(i!=HF1024KZPINNO&&i!=HF772KZPINNO&&i!=HF150KZPINNO&&i!=LF1KZPINNO
			&&i!=LF800HZPINNO )
			Set_DIOFRC_Pin(i, OFF);

	// For Digital Ramp

	SetLowFreq(nConfig.nLFrequency);
	SetHighFreq(nConfig.nHFrequency);

	Set_DIOFRC_Pin(93,OFF);
	Set_DIOFRC_Pin(94,OFF);
	Set_DIOFRC_Pin(97,OFF);
}


void SwitchCommonLines (int nOnOff)
{
	for(int i=0;prmarray->nCtrlLinesCommon[i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesCommon[i],nOnOff);
}

void SwitchJunctionLines (int nOnOff)
{
	for(int i=0;prmarray->nCtrlJunction[m_FileInfo.cFixtureNo][i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlJunction[m_FileInfo.cFixtureNo][i],nOnOff);
}

void SelectRange (int nRange,int nOnOff)
{
	for(int i=0;prmarray->nCtrlLinesRange[nRange][i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesRange[nRange][i],nOnOff);
}

void SelectReadingLines (int nPosition,int nOnOff)
{
	for( int i=0;prmarray->nCtrlLinesReading[nPosition][i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesReading[nPosition][i],nOnOff);
}

void SwitchOffsetLines (int nRange,int nOnOff)
{
	for( int i=0;prmarray->nCtrlLinesOffset[nRange][i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesOffset[nRange][i],nOnOff);
}

void SwitchStdvalLines (int nRange,int nOnOff)
{
	for( int i=0;prmarray->nCtrlLinesStdval[nRange][i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesStdval[nRange][i],nOnOff);
}

int SetFixture ( int nPair1,int nPair2,int bReadOffset)
{
	int     i, nBypass=FALSE;

	getPair( &nPair1, &nPair2 );

    Set_DIOFRC_Pin(93,OFF);
	Set_DIOFRC_Pin(94,OFF);
	Set_DIOFRC_Pin(97,OFF);
	for(i=0;prmarray->nCtrlLinesFix[i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesFix[i],ON);

	if(!bReadOffset)
	{
// Nilesh - 16th Aug. 1999 - Start (Commented)
		SetPairCtrlLines( m_FileInfo.cFixtureNo, PREV, nPrevPair1, nPrevPair2 );
		SetPairCtrlLines( m_FileInfo.cFixtureNo, NEXT, nPair1, nPair2 );
// Nilesh - 13th Aug. 1999 - End (Commented)
		SetPairCtrlLines( m_FileInfo.cFixtureNo, OTHERS, nPair1, nPair2 );
		SetPairCtrlLines( m_FileInfo.cFixtureNo, CURRENT, nPair1, nPair2 );

// Nilesh - 16th Aug. 1999 - Start (Commented)
		SetSpltCtrlLines( m_FileInfo.cFixtureNo, PREV, nPrevPair1, nPrevPair2, ON );
		SetSpltCtrlLines( m_FileInfo.cFixtureNo, NEXT, nPair1, nPair2, ON );
// Nilesh - 16th Aug. 1999 - End (Commented)
		SetSpltCtrlLines( m_FileInfo.cFixtureNo, OTHERS, nPair1, nPair2 ,ON );
		SetSpltCtrlLines( m_FileInfo.cFixtureNo, CURRENT, nPair1, nPair2, ON );
	}

	for(i=0;i<MAXBYPASSPAIRS;i++)
	{
		 if((int)m_FileInfo.cPairsBypassed[i]==nPair1||
				 (int)m_FileInfo.cPairsBypassed[i]==nPair2)
				nBypass=TRUE;
	}
	return nBypass;
}


float FindCalOffset (int nRange)
{
	float  fOffset;

    //Sequence of switching Range Line & Offset Line is swapped by
    //sdt:03082001
    SelectRange(nRange,ON);
	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

	SwitchOffsetLines(nRange,ON);

	fOffset = GetAverageReading(nRange,FALSE);

	SwitchOffsetLines(nRange,OFF);
	SelectRange(nRange,OFF);
	return fOffset;
}

float FindStdVal (int nRange)
{

	float      fCal;

	//Sequence of switching Range Line & Offset Line is swapped by
	//sdt:03082001

	SelectRange(nRange,ON);
	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );

	SwitchStdvalLines(nRange,ON);


	fCal = GetAverageReading(nRange,FALSE);

	SwitchStdvalLines(nRange,OFF);
	SelectRange(nRange,OFF);
	return fCal;
}

void ReadInstrStd (float *pfStd, int nRange, int nFreqIndex)
{
	char path[80];
	char callow[5],calhigh[5];

	//float t1,t2,CALLOW,CALHIGH;//Commented:jj:14032001

	gfOffset[nRange] = FindCalOffset( nRange );

	*pfStd = FindStdVal(nRange);
	*pfStd = *pfStd - gfOffset[nRange];

	 sprintf(path,"%s:\\cts.ini",cRamDrive);
	 GetPrivateProfileString("CAL_STD_LIMITS","CALLOW",callow,"0.7",path);
	 GetPrivateProfileString("CAL_STD_LIMITS","CALHIGH",calhigh,"1.3",path);

     CALLOW=atof(callow);
     CALHIGH=atof(calhigh);

	t1 = prmarray->fStdVal[nFreqIndex][nRange]*(float)CALLOW;
	t2 = prmarray->fStdVal[nFreqIndex][nRange]*(float)CALHIGH;
	if(*pfStd < t1 || *pfStd > t2)
	{
		fFcal[nRange] = (float)1.0;
		gfOffset[nRange]=0.0;
	}
	else
	{
		if (*pfStd == 0)
			fFcal[nRange] = (float)1.0;
		else
			fFcal[nRange] = (float)(prmarray->fStdVal[nFreqIndex][nRange]) / (*pfStd);
	}
}


int CalibrateOneParam( )
{
	int      i, nFreqIndex, nRStart, nREnd;
	float    fInstStd;
	//int      cpParamCount;//Commented by:sdt:13112005:1405

	//messagewindow("System under calibration");
	//messageBox("System under calibration",mfError|mfOKButton );
	fPage->hint(hcCalibrate);//Added by:sdt:28092005:1105

	//cpParamCount = nParamCount;//Added by:sdt:02102005:1200 //For Quad

	//if (nParamCount == SRNO_NEXT)//Added by:sdt:02102005:1200 //For Quad
	//	nParamCount = SRNO_ATN;

	SwitchCommonLines(ON);
	SwitchJunctionLines(ON);

	delay( rdCtrl.m_anDelay[ DLY_INSTRUMENT ] );

	SelectSplitForCal( );

	nRStart = nflagRangeLock==TRUE ? atoi(szRangeHold) - 1 : 0;
	nREnd   = nflagRangeLock==TRUE ? nRStart+1 : prmarray->nRanges;

	//nFreqIndex = SetParamFreq();//Commented by:sdt:20032001
	if(prmarray->nParamType) //Added by:sdt:20032001
		nFreqIndex = nConfig.nHFrequency;
	else
	   nFreqIndex = nConfig.nLFrequency;

	for(i=nRStart;i<nREnd;i++)
			ReadInstrStd(&fInstStd, i, nFreqIndex);

	//if (nParamCount >= SRNO_ATN) //Added by:sdt:02102005:1200 //For Quad
	//	FindAtnRefCalFactor();


	SwitchCommonLines(OFF);
	SwitchJunctionLines(OFF);
	//nParamCount = cpParamCount; //Added by:sdt:02102005:1200 //For Quad
	ResetSystem();
	//messagewindow(defaultmessage);
	//messageBox(defaultmessage,mfError|mfOKButton );//
	fPage->hint(hcCalibrateOver);//Added by:sdt:28092005:1105
	return 0;
}

void FindAtnRefCalFactor()
{
	int        bCompatible = 0;
	float      fCal1,fCal2;

	SwitchStdvalLines(3, ON);
	SelectRange(3,ON);
	//No Need
	delay( rdCtrl.m_anDelay[ DLY_RANGE ] );
	delay(100);

	fCal1 = GetAverageReading(3,TRUE);
	while(!bCompatible)
	{
		fCal2 = GetAverageReading (3,TRUE);
		if(AreCompatible(fCal1,fCal2,3))
			bCompatible = TRUE;
		fCal1 = fCal2;
	}
	SwitchStdvalLines(3,OFF);
	SelectRange(3,OFF);
	fAtnOffset = 0.0;
	fReadAtnStd	= fCal1;
	fAtnCalRef = (float)(fAtnRefStd[nConfig.nHFrequency])/fCal1;
}


void SelectSplitForCal( )
{
	int nPinNo;
	for(int i=0;i<NOOFFIXTURES;i++)
	{
		for(int j=0;j<Fixture[i].m_nNoofUnits;j++)
		{
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[0]-1;
			if( Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine1 != 3)
				SetDMuxPin( nPinNo, Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine1 );
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[1]-1;
			if( Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine2 != 3)
				SetDMuxPin( nPinNo, Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine2 );
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[2]-1;
			if( Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine3 != 3)
				SetDMuxPin( nPinNo, Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine3 );
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[3]-1;
			if( Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine4 != 3)
				SetDMuxPin( nPinNo, Cal.Cl[m_FileInfo.cFixtureNo][i].nCtrlLine4 );
		}
	}
}

void SelectSplitOff( void )
{
	int nPinNo;
	for(int i=0;i<NOOFFIXTURES;i++)
	{
		for(int j=0;j<Fixture[i].m_nNoofUnits;j++)
		{
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[0]-1;
			SetDMuxPin( nPinNo, OFF );
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[1]-1;
			SetDMuxPin( nPinNo, OFF );
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[2]-1;
			SetDMuxPin( nPinNo, OFF );
			nPinNo = Fixture[ i ].Splt[ j ].m_nLineNo[3]-1;
			SetDMuxPin( nPinNo, OFF );
		}
	}
}

float GetAverageReadingSync(int nRange,int nRef)
{
	char        cChannelIndex = 0;
	//int         nAdcChannel;//Commented by:sdt:07052001

    float 		fRaw1,fRaw;

	delay( rdCtrl.m_anDelay[ DLY_PREREAD ] );

	if(nRef)
		//nAdcChannel=ADC_HFREFCHANNEL+1;//Commented by:sdt:07052001
        ADC_HFREFCHANNEL+1;
	else
		//nAdcChannel=prmarray->nAdcChannels[cChannelIndex];//Commented by:sdt:07052001
		prmarray->nAdcChannels[cChannelIndex];
	fRaw = getCmRamp( nRange );
	while( 1 )
	{
		fRaw1 = getCmRamp( nRange );
		//if ( AreCompatible( fRaw, fRaw1, nRange-2 ) )//Commented:jj:12032001
        if ( AreCompatible( fRaw, fRaw1, nRange ) )//Modified:jj:12032001
				break;
			fRaw = fRaw1;
	}
		return float((fRaw+fRaw1)/2);
}

float GetAverageReading (int nRange,int nRef)
{
	float       fPerChannel,fStart,fEnd,fRaw,nRangeBand;
	char        cChannelIndex = 0;
	int         nAdcChannel;
	int         firstAdc, secondAdc;
	AverageList	L;
//    float 		fRaw1;

	delay( rdCtrl.m_anDelay[ DLY_PREREAD ] );

	if(nRef)
		nAdcChannel=ADC_HFREFCHANNEL+1;
	else
		nAdcChannel=prmarray->nAdcChannels[cChannelIndex];

	firstAdc = GetFirstAdcValue( nRange, &nAdcChannel, &cChannelIndex );
	while( 1 )
	{
		secondAdc = GetAdcValue( nAdcChannel, nRange );
		if ( areAdcCompatible( firstAdc, secondAdc ) )
			break;
		else
			firstAdc = secondAdc;
	}
     L.nReadVal = (firstAdc+secondAdc)/2;

	L.nChannel = cChannelIndex;

#ifndef DEBUG
	cancelTimer();
#endif
	nRangeBand=prmarray->fEndofscale[nRange]-
		prmarray->fStartofscale[nRange];
	fPerChannel = nRangeBand / (prmarray->nChannels);

	if(L.nChannel)
		fStart=prmarray->fStartofscale[nRange]+(L.nChannel*fPerChannel)+(float)0.01;
	else
		fStart=prmarray->fStartofscale[nRange]+(L.nChannel*fPerChannel);
	fEnd = fStart + fPerChannel;
	fRaw = fEnd - fStart;
	fRaw *=(float)(L.nReadVal-prmarray->nAdcMin);
	fRaw /=(float)(prmarray->nAdcMax-
						prmarray->nAdcMin);
	fRaw +=fStart;
	return (float)fRaw;
}

int GetAdcValue( int nChannel, int nRange )
{
//	char path[80];
	int 	i = 0;
	int     nAdcVal = 0,nAdcGndVal = 0, nAdcValminusGnd;
	long    AdcValue = 0;
     nRange += 0;

	do
	{
		nAdcVal=ReadADC(nChannel-1);

		if(prmarray->nParamType)
			nAdcGndVal=ReadADC(ADC_GNDCHANNELHF);
		else
			nAdcGndVal=ReadADC(ADC_GNDCHANNELLF);

		nAdcValminusGnd = (nAdcVal - nAdcGndVal);
		AdcValue += nAdcValminusGnd;
		i++;
	}while( i < rdCtrl.m_nAvgof );

	AdcValue = (int)(AdcValue / i);

	nAdcValminusGnd = (int)AdcValue + ADC0CORRESPONDINGGND;
	if ( nAdcValminusGnd > prmarray->nAdcMax )
		nAdcValminusGnd = prmarray->nAdcMax;
	if ( nAdcValminusGnd < prmarray->nAdcMin )
		nAdcValminusGnd = prmarray->nAdcMin;

	return nAdcValminusGnd;
}


int GetFirstAdcValue( int nRange, int *nChannel, char *cChannelIndex )
{
	int     bStoreVal = FALSE;
	int     nAdcVal = 0,nAdcGndVal = 0, nAdcValminusGnd;
	char    cHuntlow = 0,cHuntup = 0;

	nRange+=0;
	do
	{
		nAdcVal=ReadADC((*nChannel)-1);

		if(prmarray->nParamType)
			nAdcGndVal=ReadADC(ADC_GNDCHANNELHF);
		else
			nAdcGndVal=ReadADC(ADC_GNDCHANNELLF);

		nAdcValminusGnd = (nAdcVal - nAdcGndVal) + ADC0CORRESPONDINGGND;

		if(nAdcValminusGnd<prmarray->nAdcMax &&
			nAdcValminusGnd>prmarray->nAdcMin)
			bStoreVal = TRUE;
		else
		{

			if(nAdcValminusGnd < prmarray->nAdcMin )
			{
				if(*cChannelIndex > 0 && (cHuntup+cHuntlow) !=MAXALLOWEDHUNT)
				{
					*nChannel=prmarray->nAdcChannels[--(*cChannelIndex)];
					cHuntup = 1;
				}
				else
					bStoreVal = TRUE;
			}
			else
			{
				if(*cChannelIndex < prmarray->nChannels-1 &&
					(cHuntup+cHuntlow) !=MAXALLOWEDHUNT)
				{
					*nChannel=prmarray->nAdcChannels[++(*cChannelIndex)];
					cHuntlow = 1;
				}
				else
					bStoreVal = TRUE;
			}
		}
	}while( bStoreVal != TRUE );

	if ( nAdcValminusGnd > prmarray->nAdcMax )
		nAdcValminusGnd = prmarray->nAdcMax;
	if ( nAdcValminusGnd < prmarray->nAdcMin )
		nAdcValminusGnd = prmarray->nAdcMin;

	return nAdcValminusGnd;
}

int areAdcCompatible( int firstRead, int secondRead )
{
	char path[80];
	int large = firstRead > secondRead ? firstRead : secondRead;
	int small = firstRead < secondRead ? firstRead : secondRead;

	 sprintf(path,"%s:\\cts.ini",cRamDrive);//Added:jj:28012001:1600

	if ( (large-small) <= firstRead/factor )
		return TRUE;
	else
		return FALSE;
}

int AreCompatible (float fReading1,float fReading2,int nRange)
{
	float fLarge = fReading2>fReading1 ? fReading2 : fReading1;
	float fSmall = fReading2>fReading1 ? fReading1 : fReading2;

	if((fLarge-fSmall)<=(prmarray->fAccuracyfsv*pow(10,nRange)))
		 return TRUE;

    if(fSmall==0.0)
        fSmall=0.0001;
	else if((fLarge/fSmall)<=(1+(prmarray->fAccuracy/100)))
		 return TRUE;

	return FALSE;
}



void SendSegmentData(CTPAIR CtPair,INFORMATION m_FileInfo)
{
	int index=0;
	int Data = 0,Digit,i,k;

	for (i=0;i<12;i++)
		SetDMuxPin(SegmentLatchLines[i]-1, 1);
	for (i=0;i<8;i++)
		SetDMuxPin(SegmentDataLines[i]-1, 0);

	if ( CtPair.Pair1!=0 )
	{
		if ( GetReplacePairNo(CtPair.Pair1+ m_FileInfo.cStartFixPos - 1) )
			Data = GetReplacePairNo(CtPair.Pair1+ m_FileInfo.cStartFixPos - 1);
		else
			Data = CtPair.Pair1+ m_FileInfo.cStartFixPos - 1;
		index = 0;
		do{
			Digit = Data % 10;
			if ( m_FileInfo.cFixtureNo == FIXTUREBOTH )
				for( k=0; k<2; k++ )
					DisplayDigit( Digit, index, k, PAIR1 );
			else
				DisplayDigit( Digit, index, m_FileInfo.cFixtureNo, PAIR1 );
			Data /= 10;
			index++;
		}while( Data != 0 );
		for( ;index < 3; index++ )
			if ( m_FileInfo.cFixtureNo == FIXTUREBOTH )
				for( k=0; k<2; k++ )
					DisplayDigit( 0, index, k, PAIR1 );
			else
				DisplayDigit( 0, index, m_FileInfo.cFixtureNo, PAIR1 );
	}
	if ( CtPair.Pair2!=0 )
	{
		if ( GetReplacePairNo(CtPair.Pair2+ m_FileInfo.cStartFixPos - 1) )
			Data = GetReplacePairNo(CtPair.Pair2+ m_FileInfo.cStartFixPos - 1);
		else
			Data = CtPair.Pair2+ m_FileInfo.cStartFixPos - 1;
		index = 0;
		do{
            Digit = Data % 10;
            if ( m_FileInfo.cFixtureNo == FIXTUREBOTH )
            	for( k=0; k<2; k++ )
                	DisplayDigit( Digit, index, k, PAIR2 );
            else
            	DisplayDigit( Digit, index, m_FileInfo.cFixtureNo, PAIR2 );
            Data /= 10;
            index++;
        }while( Data != 0 );
        for( ;index < 3; index++ )
        	if ( m_FileInfo.cFixtureNo == FIXTUREBOTH )
            	for( k=0; k<2; k++ )
                	DisplayDigit( 0, index, k, PAIR2 );
            else
        		DisplayDigit( 0, index, m_FileInfo.cFixtureNo, PAIR2 );
    }
}

void DisplayDigit( int Digit, int index, int FIXTURE, int PAIR )
{
    int latchIdx=0,i;
    switch( FIXTURE )
    {
    	case FIXTURE1 :
        	if ( PAIR == PAIR1 )
            	latchIdx = INNER_PAIR1LATCH_IDX+index;
           	else
               	latchIdx = INNER_PAIR2LATCH_IDX+index;
            SetDMuxPin(SegmentLatchLines[latchIdx]-1,0);
            for( i = 0; i < 4; i++ )
            {
                if ( Digit&(1<<i) )
            		SetDMuxPin(SegmentDataLines[INNER_DATALINE_IDX+i]-1, 1);
                else
                	SetDMuxPin(SegmentDataLines[INNER_DATALINE_IDX+i]-1, 0);
            }
            SetDMuxPin(SegmentLatchLines[latchIdx]-1,1);
        	break;
		case FIXTURE2 :
        	if ( PAIR == PAIR1 )
            	latchIdx = OUTER_PAIR1LATCH_IDX+index;
           	else
               	latchIdx = OUTER_PAIR2LATCH_IDX+index;
            SetDMuxPin(SegmentLatchLines[latchIdx]-1,0);
            for( i = 0; i < 4; i++ )
            {
                if ( Digit&(1<<i) )
            		SetDMuxPin(SegmentDataLines[OUTER_DATALINE_IDX+i]-1, 1);
                else
                	SetDMuxPin(SegmentDataLines[OUTER_DATALINE_IDX+i]-1, 0);
            }
            SetDMuxPin(SegmentLatchLines[latchIdx]-1,1);
        	break;
    }
}

int GetReplacePairNo( int pnPair1 )
{
	for(int i=0;i<MAXREPLACELEN;i++ )
	{
		 if(pnPair1==pinReplace[m_FileInfo.cFixtureNo].nPinsReplace[i]) //Modified by:sdt:26042015:1700
	    {
			return( pinReplace[m_FileInfo.cFixtureNo].nPinsReplacewith[i] ); //Modified by:sdt:26042015:1700
	    }
    }
    return FALSE;
}

void StoreMinVal(float fRawReading,int nPno,int nFreqNum,int nSumType)
{
     SUMMARYINFO Summary;
     memset(&Summary,0,sizeof(SUMMARYINFO));
     Summary=ReadSummary(nPno,nFreqNum,nSumType);//Added by:sdt:05092001:1300
	 Summary.STATRAW.cMinPair1=CtPair.Pair1;
	 Summary.STATRAW.cMinPair2=CtPair.Pair2;
	 Summary.STATRAW.fMin=(float)fabs((double)fRawReading);
	 Summary.STATRAW.cMinTiporRing=CtPair.TipOrRing;
	 Summary.STATNORMALISED.cMinPair1=CtPair.Pair1;
	 Summary.STATNORMALISED.cMinPair2=CtPair.Pair2;
	 Summary.STATNORMALISED.cMinTiporRing=CtPair.TipOrRing;
	 Summary.STATNORMALISED.fMin=(float)fabs((double)CtPair.norval);
     WriteSummary(nPno,nFreqNum,nSumType,Summary);
}

//Added by:sdt:03092001:sdt:0850
//This function will store the Maximum value measured in the Summary
//structure.
void StoreMaxVal(float fRawReading,int nPno,int nFreqNum,int nSumType)
{
     SUMMARYINFO Summary;
     memset(&Summary,0,sizeof(SUMMARYINFO));
     Summary=ReadSummary(nPno,nFreqNum,nSumType);//Added by:sdt:05092001:1300
	 Summary.STATRAW.cMaxPair1=CtPair.Pair1;
	 Summary.STATRAW.cMaxPair2=CtPair.Pair2;
	 Summary.STATRAW.cMaxTiporRing=CtPair.TipOrRing;
	 Summary.STATRAW.fMax=(float)fabs((double)fRawReading);
	 Summary.STATNORMALISED.cMaxPair1=CtPair.Pair1;
	 Summary.STATNORMALISED.cMaxPair2=CtPair.Pair2;
	 Summary.STATNORMALISED.cMaxTiporRing=CtPair.TipOrRing;
	 Summary.STATNORMALISED.fMax=(float)fabs((double)CtPair.norval);
     WriteSummary(nPno,nFreqNum,nSumType,Summary);
}


void ExchangeRampInfo (int nDirect)
{
	 int 			i,j;
	 static INIINFO	TmpIni;
	 int            temp_nAverageof=0;
	 if(nDirect)
	 {
			for(i=0;i<MAXFREQUENCIES;i++)
			{
				 for(j=0;j<MAXRANGES;j++)
				 {
						TmpIni.fStdVal[i][j]=prmarray->fStdVal[i][j];
						prmarray->fStdVal[i][j]=RAMPINI.fStdVal[j];
				 }
			}
			for(i=0;i<MAXRANGES;i++)
			{
				 for(j=0;j<MAX_CTRL_LINES;j++)
				 {
						TmpIni.nCtrlLinesOffset[i][j]=prmarray->nCtrlLinesOffset[i][j];
						prmarray->nCtrlLinesOffset[i][j]=RAMPINI.nCtrlLinesOffset[i][j];
						TmpIni.nCtrlLinesStdval[i][j]=prmarray->nCtrlLinesStdval[i][j];
						prmarray->nCtrlLinesStdval[i][j]=RAMPINI.nCtrlLinesStdval[i][j];
						TmpIni.nCtrlLinesRange[i][j]=prmarray->nCtrlLinesRange[i][j];
						prmarray->nCtrlLinesRange[i][j]=RAMPINI.nCtrlLinesRange[i][j];
				 }
			}
			//char szmsg[80];
			//sprintf(szmsg,"RMPINI_avg=%d, rdCtrl_avg=%d",RAMPINI.nAverageof,rdCtrl.m_nAvgof);
			//messageBox( szmsg, mfOKButton );

			temp_nAverageof = rdCtrl.m_nAvgof;
			rdCtrl.m_nAvgof = RAMPINI.nAverageof;
			RAMPINI.nAverageof = temp_nAverageof;

			//sprintf(szmsg,"RMPINI_avg=%d, rdCtrl_avg=%d",RAMPINI.nAverageof,rdCtrl.m_nAvgof);
			//messageBox( szmsg, mfOKButton );

			for(i=0;i<MAX_CTRL_LINES;i++)
			{
				 TmpIni.nCtrlLinesCommon[i]=prmarray->nCtrlLinesCommon[i];
				 prmarray->nCtrlLinesCommon[i]=RAMPINI.nCtrlLinesCommon[i];
				 TmpIni.nCtrlLinesReading[0][i]=prmarray->nCtrlLinesReading[0][i];
				 prmarray->nCtrlLinesReading[0][i]=RAMPINI.nCtrlLinesReading[0][i];
			}
			prmarray->nRanges-=2;
	 }
	 else
	 {
			for(i=0;i<MAXFREQUENCIES;i++)
			{
				 for(j=0;j<MAXRANGES;j++)
						prmarray->fStdVal[i][j]=TmpIni.fStdVal[i][j];
			}
			for(i=0;i<MAXRANGES;i++)
			{
				 for(j=0;j<MAX_CTRL_LINES;j++)
				 {
						prmarray->nCtrlLinesOffset[i][j]=TmpIni.nCtrlLinesOffset[i][j];
						prmarray->nCtrlLinesStdval[i][j]=TmpIni.nCtrlLinesStdval[i][j];
						prmarray->nCtrlLinesRange[i][j]=TmpIni.nCtrlLinesRange[i][j];
				 }
			}
			temp_nAverageof = RAMPINI.nAverageof;
			RAMPINI.nAverageof = rdCtrl.m_nAvgof;
			rdCtrl.m_nAvgof = temp_nAverageof;

			for(i=0;i<MAX_CTRL_LINES;i++)
			{
				 prmarray->nCtrlLinesCommon[i]=TmpIni.nCtrlLinesCommon[i];
				 prmarray->nCtrlLinesReading[0][i]=TmpIni.nCtrlLinesReading[0][i];
			}
			prmarray->nRanges+=2;
	 }
}


void FillParamInfo()
{
	//int i;//Commented by:sdt:13112005:1405
	SEQCOUNT SeqCountofLastUnit;
	SEQCOUNT Seqcount; //Added by:sdt:22042001

	nNeedOfAppend = FALSE;

	itoa (nTempUnitOf,nConfig.szUnitof,10);
	nUnits=atoi(nConfig.szNoofpairs)/atoi(nConfig.szUnitof);
	if(!Pcm_Cable)//Addedpnb:only if()
		if (atoi(nConfig.szNoofpairs)%atoi(nConfig.szUnitof))
			nNeedOfAppend = TRUE;

	if (m_FileInfo.nAppend && nNeedOfAppend)
	{
		if(atoi(nConfig.szNoofpairs)%atoi(nConfig.szUnitof))
		{
			// check is Added by:sdt:13032001
			// For the no of pairs other than 101 the odd combination pairs
			// treated as the new unit.
			// In 101 pairs the last pair is concidered as pair of the last
			// unit and not as separate unit.
			if( atoi(nConfig.szNoofpairs) == PRACTMAXNUMOFPAIRS)//Added:reference fxs14032001.sdt
			{
				nLastUnitSize = atoi(nConfig.szUnitof) + atoi(nConfig.szNoofpairs)%atoi(nConfig.szUnitof);
			}
			else
            {
                nLastUnitSize =  atoi(nConfig.szNoofpairs)%atoi(nConfig.szUnitof);
                nUnits++;
            }

			if (nUnits==1)  //only one unit
				itoa (nLastUnitSize ,nConfig.szUnitof ,10);
		}
		else
		{
			nLastUnitSize = 2*atoi(nConfig.szUnitof);
			nUnits --;
		}
	}
	else
		nLastUnitSize = atoi(nConfig.szUnitof);

	Seqcount = CountPairsandUnits();//Added by:sdt:22042001
	PairSwitchOption=ReadPairSwitchingOptions();//Added by:sdt:22042001
	SeqCountofLastUnit=CountPairsandUnitCmbLastUnit(nLastUnitSize,nUnits);//Added by:sdt:02072001

	nConfig.Max[SRNO_CR]=atoi(nConfig.szNoofpairs)*2;
	nConfig.Max[SRNO_CM]=atoi(nConfig.szNoofpairs);
	//nConfig.Max[SRNO_GM]=atoi(nConfig.szNoofpairs);//Commented by:sdt:30092005:2015:for quad software.

	switch(PairSwitchOption.CUPP)
    {
    	case ADJPAIRS :
        	nConfig.Max[SRNO_CUPP]=Seqcount.Adj_Pairs;
			break;
        case NONADJPAIRS:
            nConfig.Max[SRNO_CUPP]=Seqcount.NonAdj_Pairs;
            break;
        case BOTH_PAIRS :
            //Added by:sdt:04052001
            if(Seqcount.NonAdj_Pairs==0&&Seqcount.Adj_Pairs!=0)
            {
            	PairSwitchOption.CUPP=ADJPAIRS;
            }
            if(Seqcount.NonAdj_Pairs!=0&&Seqcount.Adj_Pairs==0)
            {
            	PairSwitchOption.CUPP=NONADJPAIRS;
            }
            nConfig.Max[SRNO_CUPP]=Seqcount.NonAdj_Pairs+Seqcount.Adj_Pairs;
            break;
    }
    //Condition Added by:sdt:02072001
	if(m_FileInfo.nAppend && nNeedOfAppend)
	{
         //Commented by:sdt:02072001.
		 /*nConfig.Max[SRNO_CUPP]=(atoi(nConfig.szUnitof)*(atoi(nConfig.szUnitof)-1))/2;
		 nConfig.Max[SRNO_CUPP]*=(nUnits-1);
		 nConfig.Max[SRNO_CUPP]+=(nLastUnitSize*(nLastUnitSize-1))/2;*/
         //SeqCountofLastUnit=CountPairsInLastUnit(UnitSize,PairSwitchOption.CUPP)
        int nPairsinLastUnit;
        switch(PairSwitchOption.CUPP)
	    {
    		case ADJPAIRS :
        		nPairsinLastUnit=SeqCountofLastUnit.Adj_Pairs;
	            break;
    	    case NONADJPAIRS:
        	    nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs;
            	break;
	        case BOTH_PAIRS :
                nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs+SeqCountofLastUnit.Adj_Pairs;
				break;
		 }

         nConfig.Max[SRNO_CUPP]*=(nUnits-1);
         nConfig.Max[SRNO_CUPP]+=nPairsinLastUnit;
	}
    else
	nConfig.Max[SRNO_CUPP]*=nUnits;


    switch(PairSwitchOption.CUPPI)
    {
    	case ADJUNITS :
        	nConfig.Max[SRNO_CUPPI]=Seqcount.Adj_Units;
            break;
        case NONADJUNITS:
            nConfig.Max[SRNO_CUPPI]=Seqcount.NonAdj_Units;
            break;
		case BOTH_UNITS :
            //Added by:sdt:04052001
            if(Seqcount.NonAdj_Units==0&&Seqcount.Adj_Units!=0)
            {
            	PairSwitchOption.CUPPI=ADJUNITS;
            }
            if(Seqcount.NonAdj_Units!=0&&Seqcount.Adj_Units==0)
            {
            	PairSwitchOption.CUPPI=NONADJUNITS;
            }
            nConfig.Max[SRNO_CUPPI]=Seqcount.NonAdj_Units+Seqcount.Adj_Units;
            break;
    }

    //Condition Added by:sdt:02072001
    if(m_FileInfo.nAppend && nNeedOfAppend)
	{
        int nCombWtLastUnit;
		switch(PairSwitchOption.CUPPI)
		{
			case ADJUNITS :
				nCombWtLastUnit=SeqCountofLastUnit.Adj_Units;
				break;
			case NONADJUNITS:
				nCombWtLastUnit=SeqCountofLastUnit.NonAdj_Units;
				break;
			case BOTH_UNITS :
				nCombWtLastUnit=SeqCountofLastUnit.NonAdj_Units+SeqCountofLastUnit.Adj_Units;
				break;
		 }
		 nConfig.Max[SRNO_CUPPI]-=nCombWtLastUnit;
		 nConfig.Max[SRNO_CUPPI]*=atoi(nConfig.szUnitof)*atoi(nConfig.szUnitof);
		 nConfig.Max[SRNO_CUPPI]+=((atoi(nConfig.szUnitof)*nLastUnitSize)*nCombWtLastUnit);
	}
	else
	nConfig.Max[SRNO_CUPPI]*=atoi(nConfig.szUnitof)*atoi(nConfig.szUnitof);

	nConfig.Max[SRNO_ZCAL]=atoi(nConfig.szNoofpairs);//Change:Added:(08101999)



	nConfig.Max[SRNO_CUPG]=atoi(nConfig.szNoofpairs);
	//nConfig.Max[SRNO_CUPS]=atoi(nConfig.szNoofpairs); //Commented by:sdt:30092005:2015:for quad software.
	nConfig.Max[SRNO_ATN]=atoi(nConfig.szNoofpairs);

	switch(PairSwitchOption.ONEXT)
	{
		case ADJPAIRS :
			nConfig.Max[SRNO_NEXT]=Seqcount.Adj_Pairs;
			break;
		case NONADJPAIRS:
			nConfig.Max[SRNO_NEXT]=Seqcount.NonAdj_Pairs;
			break;
		case BOTH_PAIRS :
			//Added by:sdt:04052001
			if(Seqcount.NonAdj_Pairs==0&&Seqcount.Adj_Pairs!=0)
			{
				PairSwitchOption.ONEXT=ADJPAIRS;
			}
			if(Seqcount.NonAdj_Pairs!=0&&Seqcount.Adj_Pairs==0)
			{
				PairSwitchOption.ONEXT=NONADJPAIRS;
			}
			nConfig.Max[SRNO_NEXT]=Seqcount.NonAdj_Pairs+Seqcount.Adj_Pairs;
			break;
		//Condtion added by:sdt:01102005:1520
		//As Xtalk parameters considers all combinations
		case ALL_COMBINATIONS :
			if(Seqcount.All_Combinations!=0 )
			{
				nConfig.Max[SRNO_NEXT]=Seqcount.All_Combinations;
			}
			break;
		//MOD2802200601:2210
		//As Xtalk parameters considers adjacent Pairs in the Cable
		case ADJPAIRS_INCABLE :
			if(Seqcount.Adj_Pairs_InCable!=0 )
			{
				nConfig.Max[SRNO_NEXT]=Seqcount.Adj_Pairs_InCable;
			}
			break;
	}
	//nConfig.Max[SRNO_NEXT]=(atoi(nConfig.szUnitof)*(atoi(nConfig.szUnitof)-1))/2;
	//Condition Added by:sdt:02072001
	//Condtion added by:sdt:01102005:1520
	//As Xtalk parameters considers all combinations
	//if(PairSwitchOption.ONEXT!=ALL_COMBINATIONS)
	//MOD2802200601:2215
	if((PairSwitchOption.ONEXT!=ALL_COMBINATIONS)&&(PairSwitchOption.ONEXT!=ADJPAIRS_INCABLE))
	{
		if(m_FileInfo.nAppend && nNeedOfAppend)
		{
			int nPairsinLastUnit;
			switch(PairSwitchOption.ONEXT)
			{
				case ADJPAIRS :
					nPairsinLastUnit=SeqCountofLastUnit.Adj_Pairs;
					break;
				case NONADJPAIRS:
					nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs;
					break;
				case BOTH_PAIRS :
					nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs+SeqCountofLastUnit.Adj_Pairs;
					break;
			 }
			 nConfig.Max[SRNO_NEXT]*=(nUnits-1);
			 nConfig.Max[SRNO_NEXT]+=nPairsinLastUnit;
		}
		else
		nConfig.Max[SRNO_NEXT]*=nUnits;
	}

	//Added:jj:03082001
	PairSwitchOption.ELFEXT = PairSwitchOption.FEXT ;

	switch(PairSwitchOption.ELFEXT)
	{
		case ADJPAIRS :
			nConfig.Max[SRNO_ELFEXT]=Seqcount.Adj_Pairs;
			break;
		case NONADJPAIRS:
			nConfig.Max[SRNO_ELFEXT]=Seqcount.NonAdj_Pairs;
			break;
		case BOTH_PAIRS :
			//Added by:sdt:04052001
			if(Seqcount.NonAdj_Pairs==0&&Seqcount.Adj_Pairs!=0)
			{
				PairSwitchOption.ELFEXT=ADJPAIRS;
			}
			if(Seqcount.NonAdj_Pairs!=0&&Seqcount.Adj_Pairs==0)
			{
				PairSwitchOption.ELFEXT=NONADJPAIRS;
			}
			nConfig.Max[SRNO_ELFEXT]=Seqcount.NonAdj_Pairs+Seqcount.Adj_Pairs;
			break;
	}
	//nConfig.Max[SRNO_ELFEXT]=(atoi(nConfig.szUnitof)*(atoi(nConfig.szUnitof)-1))/2;
	//Condition Added by:sdt:02072001
	if(m_FileInfo.nAppend && nNeedOfAppend)
	{
		int nPairsinLastUnit;
		switch(PairSwitchOption.ELFEXT)
		{
			case ADJPAIRS :
				nPairsinLastUnit=SeqCountofLastUnit.Adj_Pairs;
				break;
			case NONADJPAIRS:
				nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs;
				break;
			case BOTH_PAIRS :
				nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs+SeqCountofLastUnit.Adj_Pairs;
				break;
		 }

		 nConfig.Max[SRNO_ELFEXT]*=(nUnits-1);
		 nConfig.Max[SRNO_ELFEXT]+=nPairsinLastUnit;
	}
	else
		nConfig.Max[SRNO_ELFEXT]*=nUnits;


	//Added:jj:03062001
	switch(PairSwitchOption.FEXT)
	{
		case ADJPAIRS :
			nConfig.Max[SRNO_FEXT]=Seqcount.Adj_Pairs;
			break;
		case NONADJPAIRS:
			nConfig.Max[SRNO_FEXT]=Seqcount.NonAdj_Pairs;
			break;
		case BOTH_PAIRS :
			if(Seqcount.NonAdj_Pairs==0&&Seqcount.Adj_Pairs!=0)
			{
				PairSwitchOption.FEXT=ADJPAIRS;
			}
			if(Seqcount.NonAdj_Pairs!=0&&Seqcount.Adj_Pairs==0)
			{
				PairSwitchOption.FEXT=NONADJPAIRS;
			}
			nConfig.Max[SRNO_FEXT]=Seqcount.NonAdj_Pairs+Seqcount.Adj_Pairs;
			break;
		//Condtion added by:sdt:01102005:1520
		//As Xtalk parameters considers all combinations
		case ALL_COMBINATIONS :
			if(Seqcount.All_Combinations!=0 )
			{
				nConfig.Max[SRNO_FEXT]=Seqcount.All_Combinations;
			}
			break;
				//MOD2802200601:2210
		//As Xtalk parameters considers adjacent Pairs in the Cable
		case ADJPAIRS_INCABLE :
			if(Seqcount.Adj_Pairs_InCable!=0 )
			{
				nConfig.Max[SRNO_FEXT]=Seqcount.Adj_Pairs_InCable;
			}
			break;


	}
	//Condition Added by:sdt:02072001
	//Condtion added by:sdt:01102005:1520
	//As Xtalk parameters considers all combinations
	//if(PairSwitchOption.FEXT!=ALL_COMBINATIONS)
	//MOD2802200601:2220
	if((PairSwitchOption.FEXT!=ALL_COMBINATIONS)&&(PairSwitchOption.FEXT!=ADJPAIRS_INCABLE))
	{
		if(m_FileInfo.nAppend && nNeedOfAppend)
		{
			int nPairsinLastUnit;
			switch(PairSwitchOption.FEXT)
			{
				case ADJPAIRS :
					nPairsinLastUnit=SeqCountofLastUnit.Adj_Pairs;
					break;
				case NONADJPAIRS:
					nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs;
					break;
				case BOTH_PAIRS :
					nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs+SeqCountofLastUnit.Adj_Pairs;
					break;
			 }

			 nConfig.Max[SRNO_FEXT]*=(nUnits-1);
			 nConfig.Max[SRNO_FEXT]+=nPairsinLastUnit;
		}
		else
			nConfig.Max[SRNO_FEXT]*=nUnits;
	}

	switch(PairSwitchOption.INEXT)
	{
		case ADJPAIRS :
			nConfig.Max[SRNO_INEXT]=Seqcount.Adj_Pairs;
			break;
		case NONADJPAIRS:
			nConfig.Max[SRNO_INEXT]=Seqcount.NonAdj_Pairs;
			break;
		case BOTH_PAIRS :
			if(Seqcount.NonAdj_Pairs==0&&Seqcount.Adj_Pairs!=0)
			{
				PairSwitchOption.INEXT=ADJPAIRS; //Modified by:sdt:10062001: Inext is mistacally written as Onext.
			}
			if(Seqcount.NonAdj_Pairs!=0&&Seqcount.Adj_Pairs==0)
			{
				PairSwitchOption.INEXT=NONADJPAIRS;
			}
			nConfig.Max[SRNO_INEXT]=Seqcount.NonAdj_Pairs+Seqcount.Adj_Pairs;
			break;
	}

	//Condition Added by:sdt:02072001
	if(m_FileInfo.nAppend && nNeedOfAppend)
	{
		int nPairsinLastUnit;
		switch(PairSwitchOption.INEXT)
		{
			case ADJPAIRS :
				nPairsinLastUnit=SeqCountofLastUnit.Adj_Pairs;
				break;
			case NONADJPAIRS:
				nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs;
				break;
			case BOTH_PAIRS :
				nPairsinLastUnit=SeqCountofLastUnit.NonAdj_Pairs+SeqCountofLastUnit.Adj_Pairs;
				break;
		 }

		 nConfig.Max[SRNO_INEXT]*=(nUnits-1);
		 nConfig.Max[SRNO_INEXT]+=nPairsinLastUnit;
	}
	else
	nConfig.Max[SRNO_INEXT]*=nUnits;



	if(Pcm_Cable)    //Added:pnb:19111999.
	{
		nConfig.Max[SRNO_CR]=atoi(nConfig.szNoofpairs)*2;
        nConfig.Max[SRNO_RU]=nConfig.Max[SRNO_CR]/2;
        nConfig.Max[SRNO_LR]=nConfig.Max[SRNO_CR]/2;
		nConfig.Max[SRNO_CM]=atoi(nConfig.szNoofpairs);
		nConfig.Max[SRNO_ZCAL]=atoi(nConfig.szNoofpairs);
		nConfig.Max[SRNO_CUPG]=atoi(nConfig.szNoofpairs);
		nConfig.Max[SRNO_ATN]=atoi(nConfig.szNoofpairs);
        switch(compartsizeinunits)
        {
        	case 1 :
            	nConfig.Max[SRNO_CUPP]=2 * compartsizeinunits * \
						atoi(nConfig.szUnitof) * (atoi(nConfig.szUnitof)-1) / 2;
				nConfig.Max[SRNO_ELFEXT]=2 * compartsizeinunits * \
						atoi(nConfig.szUnitof) * (atoi(nConfig.szUnitof)-1) / 2;
                NormalReadingsOver = nConfig.Max[SRNO_ELFEXT];
               	break;
         	case 2 :
               	nConfig.Max[SRNO_CUPP]=2 * compartsizeinunits * \
						atoi(nConfig.szUnitof) * (atoi(nConfig.szUnitof)-1) / 2;
				nConfig.Max[SRNO_ELFEXT]=2 * compartsizeinunits * \
						atoi(nConfig.szUnitof) * (atoi(nConfig.szUnitof)-1) / 2;
                NormalReadingsOver = nConfig.Max[SRNO_ELFEXT];
				nConfig.Max[SRNO_CUPP]+=compartsizeinunits * \
						atoi(nConfig.szUnitof) * atoi(nConfig.szUnitof);
				nConfig.Max[SRNO_ELFEXT]+=compartsizeinunits * \
						atoi(nConfig.szUnitof) * atoi(nConfig.szUnitof);
               	break;
        	case 4 :
               	nConfig.Max[SRNO_CUPP]= 2 * compartsizeinunits * \
						atoi(nConfig.szUnitof) * (atoi(nConfig.szUnitof)-1) / 2;
				nConfig.Max[SRNO_ELFEXT]=2 * compartsizeinunits * \
						atoi(nConfig.szUnitof) * (atoi(nConfig.szUnitof)-1) / 2;
                NormalReadingsOver = nConfig.Max[SRNO_ELFEXT];
                nConfig.Max[SRNO_CUPP]+= (2 * compartsizeinunits * (compartsizeinunits-1)/2) * \
					atoi(nConfig.szUnitof) * atoi(nConfig.szUnitof);
				nConfig.Max[SRNO_ELFEXT]+=(2 * compartsizeinunits * (compartsizeinunits-1)/2) * \
					atoi(nConfig.szUnitof) * atoi(nConfig.szUnitof);
               	break;
        }
		nConfig.Max[SRNO_NEXT]=compartsizeinunits*atoi(nConfig.szUnitof)*compartsizeinunits*atoi(nConfig.szUnitof);
	}

	//for(i=0;i<MAXPARAMETERS;i++)//Commented by:sdt:16102005:1840 //As it is no longer used
	//	 DisplayArray[i] = nConfig.Max[i]*nConfig.Specks.speckparams[i].nNumofFreqSelected;

}

void ReadStdVal(int nParamCount)
{
	char path[80];
	char   	szStdVal[50];
	char 	szStr[50], szTmp[50];
	char   	sztStr[50];
	int 	i,j,k,l;


	sprintf(path,"%s:\\std.ini",cRamDrive);
	for(i=0;i<MAXFREQUENCIES;i++)
	{
		sprintf(sztStr,"%s_FREQ%d",paraminfo[nParamCount].szParamAbr,i+1);
		GetPrivateProfileString("STD_VAL",sztStr,szStdVal,"",path);
		strcpy(szTmp,szStdVal);

		j=0;  k=0; l=0;
		float temp;
		while(k<strlen(szTmp))
		{
			for(j=0;szTmp[k]!=','&&k<strlen(szTmp);j++)
				szStr[j] = szTmp[k++];
			szStr[j]=NULL;
			k++;
			temp = (float)atof(szStr);
			prmarray->fStdVal[i][l++]=temp;
		}
	}
}

float getCmRamp( int nRange )
{
	int Adc1st = 0, Adc2nd = 0, Adc3rd = 0;
	long count = 0;
	double dpm = 0;

	while( (inportb(0xe26)&0x40) == 0 );
	while( (inportb(0xe26)&0x40) != 0 )
	{
		Adc1st = ReadADC(prmarray->nAdcChannels[0]-1);
		Adc2nd = ReadADC(prmarray->nAdcChannels[1]-1);
		Adc3rd = ReadADC(prmarray->nAdcChannels[2]-1);

		dpm += getDpmRamp( Adc1st, Adc2nd, Adc3rd, nRange );
		count ++;
        if(count>100) //Added:jj:12032001
        	break;
	}
	while( (inportb(0xe26)&0x40) != 64 )
	{
		Adc1st = ReadADC(prmarray->nAdcChannels[0]-1);
		Adc2nd = ReadADC(prmarray->nAdcChannels[1]-1);
		Adc3rd = ReadADC(prmarray->nAdcChannels[2]-1);
		dpm += getDpmRamp( Adc1st, Adc2nd, Adc3rd, nRange );
		count ++;
        if(count>200)//Added:jj:12032001
        	break;
	}
	dpm /= count;
	return dpm;
}

float getDpmRamp( int Adc1st, int Adc2nd, int Adc3rd, int nRange )
{
	long a, b, c;
	int gnd;

	nRange += 0;
	a = ( Adc1st < 41 ) ? -50:Adc1st;
	a = ( Adc1st > 4054 ) ? 300:Adc1st;
	b = ( Adc2nd < 41 ) ? 0:Adc2nd;
	b = ( Adc2nd > 4054 ) ? 350:Adc2nd;
	c = ( Adc3rd < 41 ) ? 0:Adc3rd;
	c = ( Adc3rd > 4054 ) ? 350:Adc3rd;

	gnd = ReadADC(ADC_GNDCHANNELLF);

	if ( Adc1st > 41 && Adc1st < 4054 )
	{
		Adc1st = Adc1st - gnd + ADC0CORRESPONDINGGND;
		a = float(double(double(Adc1st-41)*350)/float(4013.0)) + -50;
		//a = float( double((Adc1st-41)*(350))/float(4013.0)) + -50;
	}
	if ( Adc2nd > 41 && Adc2nd < 4054 )
	{
		Adc2nd = Adc2nd - gnd + ADC0CORRESPONDINGGND;
		b = float(double(double(Adc2nd-41)*350)/float(4013.0)) + 0;
	}
	if ( Adc3rd > 41 && Adc3rd < 4054 )
	{
		Adc3rd = Adc3rd - gnd + ADC0CORRESPONDINGGND;
		c = float(double(double(Adc3rd-41)*350)/float(4013.0)) + 0;
	}
	else if ( Adc3rd >= 4054 )
	{
		Adc3rd = Adc3rd - gnd + ADC0CORRESPONDINGGND;
		c = float(double(double(Adc3rd-41)*350)/float(4013.0)) + 0;
		c += 1;	// range out
	}
	return (a+b+c);
}

int ReadADC (int nChannelNumber)
{
	unsigned      int wData, wCount = 0;
	int 					nAdcHi, nAdcLo;
	int           nInvalidCount = 0;

	outportb(ADC_PORTCHANNEL, nChannelNumber);
	outportb(ADC_PORTTRIGGER, ADC_TRIGGER);

	while(1)
	{
		nAdcHi = inportb(ADC_PORTHIGH);
		if ( (nAdcHi & 0x10) == 0 )	// high byte is low if conversion is completed
			break;
		wCount++;
		if(wCount >= ADC_READDELAY)
		{
			nInvalidCount++;
			return ADC_NOINPUT;
		}
	}
	wData = nAdcHi & 0xf;
	wData <<= 8;
	wData &= 0x0fff;    // mask the upper 4-bits of high byte (since 12-bit ADC)
	nAdcLo = inportb(ADC_PORTLOW);
	wData |= nAdcLo & 0xff;

	return wData;
}


int SetPair (int nPair1,int nPair2,int nPrPair1,int nPrPair2)
{
	int        i;
	int        nBypass=FALSE;

	nPrevPair1 = nPrPair1;
	nPrPair1 +=(m_FileInfo.cStartFixPos-1);
	if(nPair2)
	{
		nPrevPair2 = nPrPair2;
		nPrPair2 +=(m_FileInfo.cStartFixPos-1);
	}
	if (gflagReplace1 == TRUE)
	{
		 gflagReplace1 = FALSE;
		 nPrPair1 = gnPair1;
	}
	if (gflagReplace2 == TRUE)
	{
		 gflagReplace2 = FALSE;
		 nPrPair2 = gnPair2;
	}

	getPair( &nPair1, &nPair2 );

	SetPairCtrlLines( m_FileInfo.cFixtureNo, PREV, nPrPair1, nPrPair2 );
	SetPairCtrlLines( m_FileInfo.cFixtureNo, NEXT, nPair1, nPair2 );
	SetPairCtrlLines( m_FileInfo.cFixtureNo, OTHERS, nPair1, nPair2 );
	SetPairCtrlLines( m_FileInfo.cFixtureNo, CURRENT, nPair1, nPair2 );

	SetSpltCtrlLines( m_FileInfo.cFixtureNo, PREV, nPrevPair1, nPrevPair2, ON );
	SetSpltCtrlLines( m_FileInfo.cFixtureNo, NEXT, nPair1, nPair2, ON );
	SetSpltCtrlLines( m_FileInfo.cFixtureNo, OTHERS, nPair1, nPair2, ON );
	SetSpltCtrlLines( m_FileInfo.cFixtureNo, CURRENT, nPair1, nPair2, ON );

	for(i=0;prmarray->nCtrlLinesFix[i]!=0;i++)
		Set_DIOFRC_Pin(prmarray->nCtrlLinesFix[i], ON);

	for(i=0;i<MAXBYPASSPAIRS;i++)
	{
		 if((int)m_FileInfo.cPairsBypassed[i]==nPair1||
				 (int)m_FileInfo.cPairsBypassed[i]==nPair2)
				nBypass=TRUE;
	}
	return nBypass;
}

void getPair( int *pnPair1, int *pnPair2 )
{
	*pnPair1 +=(m_FileInfo.cStartFixPos-1);
	if(*pnPair2)
		*pnPair2 +=(m_FileInfo.cStartFixPos-1);
	for( int i=0;i<MAXREPLACELEN;i++ )
	{
		 if(*pnPair1==pinReplace[m_FileInfo.cFixtureNo].nPinsReplace[i]) //Updated by:sdt:26042015:1700
		 {
			*pnPair1=pinReplace[m_FileInfo.cFixtureNo].nPinsReplacewith[i]; //Updated by:sdt:26042015:1700
				/* This is Used for Pins Replace */
			gnPair1=*pnPair1;
			gflagReplace1 = TRUE;
		 }
		 if (*pnPair2)
		 {
			 if(*pnPair2==pinReplace[m_FileInfo.cFixtureNo].nPinsReplace[i]) //Updated by:sdt:26042015:1700
			 {
				*pnPair2=pinReplace[m_FileInfo.cFixtureNo].nPinsReplacewith[i]; //Updated by:sdt:26042015:1700
				/* This is Used for Pins Replace */
				gnPair2=*pnPair2;
				gflagReplace2 = TRUE;
			 }
		 }
	}
}

void SetSpltCtrlLines( int nFixType, int nStatus, int nPair1, int nPair2, int bOnOff )
{
	int   i;
	int   nPinNo, nCtrlLine;
	int   nCurrUnit1, nCurrUnit2,nUnit;
	int   nNextPair1, nNextPair2;

	bOnOff += 0;
	if ( nPair1 < m_FileInfo.cStartFixPos )
		return;

	nNextPair1 = nPair1, nNextPair2 = nPair2;
	getNextPair( &nNextPair1, &nNextPair2 );

	if ( ( nPair1 >= m_FileInfo.cStartFixPos + m_FileInfo.cNoofPairs -1 ) ||
		 (  nPair2 >= m_FileInfo.cStartFixPos + m_FileInfo.cNoofPairs -1 ) )
	{
	 nNextPair1 = -99;
	}

	if( nPair1 )
	{
	if ( nStatus == OTHERS )
	{
	  for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	   {
		nCurrUnit1 = Fixture[ nFixNo ].Pair[nPair1-1].m_nUnit;
		nNextFix[nFixNo] = nCurrUnit1 + 1;
		nPrevFix[nFixNo] = nCurrUnit1;
		for( i = 1; i <= Fixture[ nFixNo ].m_nNoofUnits; i++ )
		{
			if ( i==nCurrUnit1 || i==nPrevFix[nFixNo] ||
						i==nNextFix[nFixNo] )
			continue;
			nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[0]-1;
			if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine1 == 1)
				SetDMuxPin( nPinNo, ON );
			 else
			 if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine1 == 0)
				SetDMuxPin( nPinNo, OFF );
			 nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[1]-1;
			if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine2 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine2 == 0)
				SetDMuxPin( nPinNo, OFF );
			nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[2]-1;
			if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine3 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine3 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[3]-1;
			if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine4 == 1)
				SetDMuxPin( nPinNo, ON );
			 else
			 if (prmarray->nOtherSpltPair1[nFixType][nFixNo].nCtrlLine4 == 0)
				SetDMuxPin( nPinNo, OFF );
		}
	  }
	}
	else
	if ( nStatus == CURRENT )
	{
		for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
		{
			nUnit = nCurrUnit1 = Fixture[ nFixNo ].Pair[nPair1-1].m_nUnit;
			nNextFix[nFixNo] = nCurrUnit1 + 1;
			nPrevFix[nFixNo] = nCurrUnit1;
			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[0]-1;
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine1 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine1 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[1]-1;
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine2 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine2 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[2]-1;
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine3 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine3 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[3]-1;
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine4 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nCurrSpltPair1[nFixType][nFixNo].nCtrlLine4 == 0)
				SetDMuxPin( nPinNo, OFF );
		}
	 }
	 else
	 if ( nStatus == PREV )
	 {
		for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
		{
			nUnit = nPrevFix[nFixNo];
			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[0]-1;
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine1 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine1 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[1]-1;
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine2 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine2 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[2]-1;
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine3 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine3 == 0)
				SetDMuxPin( nPinNo, OFF );

			nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[3]-1;
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine4 == 1)
				SetDMuxPin( nPinNo, ON );
			else
			if (prmarray->nPrevSpltPair1[nFixType][nFixNo].nCtrlLine4 == 0)
				SetDMuxPin( nPinNo, OFF );
		}
	 }
	 else
	 if ( nStatus == NEXT )
	 {
		if (nNextPair1 == -99)
			return;
		//nPair1 = nNextPair1;
		//nPair2 = nNextPair2;

		for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
		{
		nUnit = nNextFix[nFixNo];
        if ( nUnit == 0 )
        	continue;
		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[0]-1;
		nCtrlLine = prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine1;
		if (nCtrlLine == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (nCtrlLine == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[1]-1;
		if (prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[2]-1;
		if (prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[3]-1;
		if (prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (prmarray->nNextSpltPair1[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	 }
	}
	}
	if(nPair2)
	{
	if ( nStatus == OTHERS )
	{
	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		nCurrUnit1 = Fixture[ nFixNo ].Pair[nPair1-1].m_nUnit;
		nCurrUnit2 = Fixture[ nFixNo ].Pair[nPair2-1].m_nUnit;
		nNextFix[nFixNo] = nCurrUnit1 + 1;
		nNextFix1[nFixNo] = nCurrUnit2+1;
		nPrevFix[nFixNo] = nCurrUnit1;
		nPrevFix1[nFixNo] = nCurrUnit2;

	 for( i = 1; i <= Fixture[ nFixNo ].m_nNoofUnits; i++ )
	 {
		if(i==nCurrUnit1      || 	i==nCurrUnit2 ||
			i==nPrevFix[nFixNo]  ||    i==nPrevFix1[nFixNo]  ||
			i==nNextFix[nFixNo]  ||    i==nNextFix1[nFixNo]  )
			continue;

		 nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[0]-1;
		 if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[1]-1;
		if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[2]-1;
		if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ i-1 ].m_nLineNo[3]-1;
		if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		else
		if (prmarray->nOtherSpltPair2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	}
   }
   }
	 else
	 if ( nStatus == CURRENT )
	 {
	for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	{
		nUnit = nCurrUnit2 = Fixture[ nFixNo ].Pair[nPair2-1].m_nUnit;
		nNextFix1[nFixNo] = nCurrUnit2+1;
		nPrevFix1[nFixNo] = nCurrUnit2;
		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[0]-1;
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[1]-1;
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[2]-1;
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[3]-1;
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrSpltPair2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	}
	}
	 else
	 if ( nStatus == PREV )
	 {
	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		nUnit = nPrevFix1[nFixNo];
		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[0]-1;
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[1]-1;
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[2]-1;
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[3]-1;
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevSpltPair2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	 }
	 }
	 else
	 if ( nStatus == NEXT )
	 {
	 nPair1 = nNextPair1, nPair2 = nNextPair2;
	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		nUnit = nNextFix1[nFixNo];
		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[0]-1;
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[1]-1;
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[2]-1;
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		nPinNo = Fixture[ nFixNo ].Splt[ nUnit-1 ].m_nLineNo[3]-1;
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextSpltPair2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );

	 }
	 }
	 }
}

void SetPairCtrlLines( int nFixType, int nStatus, int nPair1, int nPair2 )
{
	int             i,nPinNo;
	int             nNextPair1, nNextPair2;

	if ( nPair1 < m_FileInfo.cStartFixPos )
		return;
	nNextPair1 = nPair1, nNextPair2 = nPair2;
	getNextPair( &nNextPair1, &nNextPair2 );

	if( nPair1 )
	{
	 if ( nStatus == OTHERS )
	 {
		for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
		for( i = 0; i <= Fixture[ nFixNo ].m_nNoofPairs; i++ )
		{
		if ( i==nPair1 || i==nPrevPair1 || (i-1) < 0 ) //  || i==nNextPair1 )
			continue;
		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[0]-1;
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[1]-1;
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[2]-1;
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[3]-1;
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus1[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
		}
	 }
	 else
	 if ( nStatus == CURRENT )
	 {
	for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	{
		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[0]-1;
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[1]-1;
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[2]-1;
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[3]-1;
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus1[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	}
	 }
	 if ( nStatus == PREV )
	 {
	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[0]-1;
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[1]-1;
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[2]-1;
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[3]-1;
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus1[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	 }
	 }
	 else
	 if ( nStatus == NEXT )
	 {
		 nPair1 = nNextPair1;

		 if ( nPair1 > Fixture[ nFixType ].m_nNoofPairs )
			return;

	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[0]-1;
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[1]-1;
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[2]-1;
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair1-1 ].m_nLineNo[3]-1;
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus1[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	 }
	 }
	}

	if(nPair2)
	{
	 if ( nStatus == OTHERS )
	 {
	for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	for( i = 1; i <= Fixture[ nFixNo ].m_nNoofPairs; i++ )
	{
		 if ( i==nPair1 || i==nPrevPair1  //|| i==nNextPair1
			|| i==nPair2 || i==nPrevPair2) //  || i==nNextPair2)
			 continue;
		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[0]-1;
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[1]-1;
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[2]-1;
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ i-1 ].m_nLineNo[3]-1;
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nOtherStatus2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	}
	 }
	 else
	 if ( nStatus == CURRENT )
	 {
	for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	{
		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[0]-1;
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[1]-1;
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[2]-1;
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[3]-1;
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nCurrStatus2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	}
	 }
	 else
	 if ( nStatus == PREV )
	 {
	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[0]-1;
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[1]-1;
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[2]-1;
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[3]-1;
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nPrevStatus2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );
	 }
	 }
	 else
	 if ( nStatus == NEXT )
	 {
	 nPair2 = nNextPair2;

	 for (int nFixNo=0; nFixNo<nFixtures; nFixNo++)
	 {
		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[0]-1;
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine1 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine1 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[1]-1;
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine2 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine2 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[2]-1;
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine3 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine3 == 0)
			SetDMuxPin( nPinNo, OFF );

		 nPinNo = Fixture[ nFixNo ].Pair[ nPair2-1 ].m_nLineNo[3]-1;
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine4 == 1)
			SetDMuxPin( nPinNo, ON );
		 else
		 if (prmarray->nNextStatus2[nFixType][nFixNo].nCtrlLine4 == 0)
			SetDMuxPin( nPinNo, OFF );

	 }
	 }
	}
}

void getNextPair( int *pnPair1, int *pnPair2 )
{
	(*pnPair1)++;

	if(*pnPair2)
		(*pnPair2)++;

	for( int i=0;i<MAXREPLACELEN;i++ )
	{
		 if(*pnPair1==pinReplace[m_FileInfo.cFixtureNo].nPinsReplace[i]) //Updated by:sdt:26042015:1700
			*pnPair1=pinReplace[m_FileInfo.cFixtureNo].nPinsReplacewith[i]; //Updated by:sdt:26042015:1700
		 if (*pnPair2)
			 if(*pnPair2==pinReplace[m_FileInfo.cFixtureNo].nPinsReplace[i]) //Updated by:sdt:26042015:1700
				*pnPair2=pinReplace[m_FileInfo.cFixtureNo].nPinsReplacewith[i];//Updated by:sdt:26042015:1700
	}
}


//Commented by:sdt:16102005:1840 //As it is no longer used
//Uncommented by:sdt:02112006:2240
void FillDispStruct(PRM *prmarray,int paramno)
{
	int i;
	dispstruct[paramno].pixelstodisplay =prmarray->pixelstodisplay;
	dispstruct[paramno].pixelsdisplayed =prmarray->pixelsdisplayed;
	strcpy(dispstruct[paramno].szUnit , prmarray->szUnit);
	for(i=0;i<2;i++)
		dispstruct[paramno].Decimal[i] =prmarray->Decimal[i] ;
}

void InitialiseCALarray(void)
{
	int j,k;
	char path[80];
	char   sztStr[MAXSTRINGLENGTH1];
	char   szStdVal[120];
	char*  Ptr;

	for( j = 0; j < MAXFIXCOMB; j++ )
	{
		for( k = 0; k < NOOFFIXTURES; k++ )
		{
			sprintf( sztStr, "%s_COMB%d_SPLT%d", prmarray->szParamNameAbr,j+1, k+1 );
			sprintf(path,"%s:\\prog.ini",cRamDrive);

			GetPrivateProfileString( "CALIBRATION", sztStr, szStdVal, " ", path );
			Ptr = strtok( szStdVal, "," );
			Cal.Cl[j][k].nCtrlLine1 = (((strcmp(Ptr,"0")==0)||(strcmp(Ptr,"1")==0)||strcmp(Ptr, NULL)==0))?atoi( Ptr ):3;
			Ptr = strtok( NULL, "," );
			Cal.Cl[j][k].nCtrlLine2 = (((strcmp(Ptr,"0")==0)||(strcmp(Ptr,"1")==0)||strcmp(Ptr, NULL)==0))?atoi( Ptr ):3;
			Ptr = strtok( NULL, "," );
			Cal.Cl[j][k].nCtrlLine3 = (((strcmp(Ptr,"0")==0)||(strcmp(Ptr,"1")==0)||strcmp(Ptr, NULL)==0))?atoi( Ptr ):3;
			Ptr = strtok( NULL, "," );
			Cal.Cl[j][k].nCtrlLine4 = (((strcmp(Ptr,"0")==0)||(strcmp(Ptr,"1")==0)||strcmp(Ptr, NULL)==0))?atoi( Ptr ):3;
			Ptr = strtok( NULL, "," );
		}
	}
}

int CheckHiLoLimits (int nFreqNum,float fRu, int bRu)
{
	float fNor;
	int nReturnVal=VALID, nCh=FALSE, nPId;
	int nLimitIndex;
	CONFIGLIMIT configLimit;//Added by:sdt:23082001
	//char buf[80];

	fNor = bRu == TRUE ? fRu : CtPair.norval;
	nPId = bRu == TRUE ? SRNO_RU : nParamCount;

	//Config struct has changed :specklimits struct added:jj:18042001

	nLimitIndex=GetLimitIndex(nPId); //Added by:sdt:11052001
	//Added by:sdt:23082001
	//start
	configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nPId,nFreqNum,nLimitIndex,0));
	if(configLimit.lowerlimit!=(float)BLANKVALUE)
	{
		if(fabs(fNor)<configLimit.lowerlimit)
			nReturnVal=INVALID;
	}
	if(configLimit.upperlimit!=(float)BLANKVALUE)
	{
		if(fabs(fNor)>configLimit.upperlimit)
			nReturnVal=INVALID;
	}
	//End
	//Following Loop Commented by:sdt:23082001
	/*if(nConfig.Specks.speckparams[nPId].specklimits[nLimitIndex].lowerlimit!=(float)BLANKVALUE)
	{
		if(fabs(fNor)<nConfig.Specks.speckparams[nPId].specklimits[nLimitIndex].lowerlimit)
			nReturnVal=INVALID;
	}
	if(nConfig.Specks.speckparams[nPId].specklimits[nLimitIndex].upperlimit!=(float)BLANKVALUE)
	{
		if(fabs(fNor)>nConfig.Specks.speckparams[nPId].specklimits[nLimitIndex].upperlimit)
			nReturnVal=INVALID;
	}*/

	//if(!nReturnVal)
	//Condition Modified by:sdt:28042015:2100
	if((!nReturnVal)&&(runSetup.bAlarmsOff == False))
	{
		//messagewindow("Out of limit.. Press ESC: Override, ANY OTHER KEY: Fresh reading");
		//messageBox( "Out of limit.. Press ESC: Override, ANY OTHER KEY: Fresh reading", mfError|mfOKButton );
		char msg[100];
		//Condition Added by:sdt:05112006:1720
		//Condition added to indicate different messages for Single pair
		//parameter & Pair to Pair parameter.
		if (prmarray->nTestingMethod==SINGLEPAIR)
		{
			sprintf(msg, "Out of limit....\nPair No. = %d\nSpeck Values = %0.2f-%0.2f\nNormalise Value = %0.2f",
				  CtPair.Pair1,configLimit.lowerlimit,configLimit.upperlimit,fNor);
		}
		else
		{
			sprintf(msg, "Out of limit....\nPair No. =%d-%d\nSpeck Values = %0.2f-%0.2f\nNormalise Value = %0.2f",
				  CtPair.Pair1,CtPair.Pair2,configLimit.lowerlimit,configLimit.upperlimit,fNor);
		}
		ushort u = messageBox( mfError|mfContinueButton|mfRepeatButton|mfResetButton,
									"%s", msg );
		sound (BEEP);
		delay (250);
		nosound ();
		//Start::Commented by:sdt:16012006:2205
		/*nCh = FALSE;
		while (kbhit())
			getch ();
		while(!nCh)
			nCh=getkey();
		switch(nCh)
		{
			case SPACEBAR :
				break;

			 case ESCAPE :
					nReturnVal=OVERRIDE;
					break;
			 case F7 :
					flagReset = TRUE;
					nReturnVal=F7;
					break;
			 case F8 :
					nReturnVal=F8;
			 default :
					break;
		}*/
		//End::Commented by:sdt:16012006:2205
		switch(u)
		{
			case cmReset : flagReset = TRUE;
							nReturnVal=F7;
							break;
			case cmRepeat : break;
			case cmContinue : nReturnVal=OVERRIDE;
							  break;
			case cmCancel : nReturnVal=OVERRIDE;
							  break;
			default :
					break;


		}
	}
	else if(runSetup.bAlarmsOff == True) //Condition Aded by:sdt:28042015:2100
	{
		nReturnVal=VALID;
	}
	//messagewindow(defaultmessage);
	//messageBox( defaultmessage, mfError|mfOKButton );//Commented temporary:sdt:27092005:2320
	if(runSetup.bSingleStep==TRUE&&nReturnVal==VALID)
	{
		//sound(BEEP);
		nCh = FALSE;
		//delay (250);
		//nosound ();
		while(!nCh)
			nCh=getkey();
		switch(nCh)
		{
			case SPACEBAR :
				break;
			 case ESCAPE :
				nReturnVal=OVERRIDE;
				break;
			 case F7 :
				flagReset = TRUE;
				nReturnVal=F7;
				break;
			 case F8 :
				nReturnVal=F8;
			 default :
				break;
		}
	}
	return nReturnVal;
}


void GiveAlarms (int nFreqNum)//Changed by:sdt:21032001
{
	int  nAlarm=FALSE;
	int nLimitIndex=0;//Added by:sdt:23082001
	CONFIGLIMIT configLimit;//Added by:sdt:23082001
	int nIterationCount=1;//Added by:sdt:04092001:0940
	SUMMARYINFO SummaryInfo;//Added by:sdt:04092001:0950
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));//Added by:sdt:04092001:0950
	//Following Code Added by:sdt:23082001
    //start
    //Following Code Modified by:sdt:04092001:1000
    //start
    //This switch() loop will findout the actual Switching mode
    //of a given parameter.
    //Commented by:sdt:01102005:1715
    //As Combinations are not considered hence for Quad software.
    //All limits will be stored at Limit Index=0.
    /*switch(nParamCount)
    {
		case SRNO_CUPP: nLimitIndex=PairSwitchOption.CUPP;
						break;
	   case SRNO_CUPPI: nLimitIndex=PairSwitchOption.CUPPI;
						break;
	   case SRNO_NEXT: nLimitIndex=PairSwitchOption.ONEXT;
						break;
	   case SRNO_FEXT: nLimitIndex=PairSwitchOption.FEXT;
						break;
	   case SRNO_INEXT: nLimitIndex=PairSwitchOption.INEXT;
						break;
	   case SRNO_ELFEXT: nLimitIndex=PairSwitchOption.ELFEXT;
						break;
	  default : nLimitIndex = 0;
    }
    //If the Parameter is tested in Both mode we have to check
    //Limits is two cases i). Adjacent, ii). Nonadjacent.
    if(nLimitIndex==BOTH_PAIRS)//||nLimitIndex==BOTH_UNITS) //MOD11072002.02:jj
    {
	   //nIterationCount=2;
	   nIterationCount=3;//MOD11072002.02:jj
	}
    if(nLimitIndex==BOTH_UNITS) //MOD11072002.02:jj
	nIterationCount=2;

    for(int i=0;i<nIterationCount;i++)
    {

	    //nLimitIndex=GetLimitIndex(nParamCount);
	   //Following Condition will find out the correct Limits &
	   //Summary value for a given Parameter.
	   //start
	   if(nParamCount==SRNO_CUPPI&&nLimitIndex!=BOTH_UNITS)
	   {
		//configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,(nLimitIndex-1),0));
		  configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,(nLimitIndex),0)); //MOD11072002.02:jj
		  SummaryInfo=ReadSummary(nParamCount,nFreqNum,(nLimitIndex-3));
	   }
	   else
	   if(nParamCount==SRNO_CUPPI&&nLimitIndex==BOTH_UNITS)
	   {
		//configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,i+2,0));
		  configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,i+3,0));//MOD11072002.02:jj
		  SummaryInfo=ReadSummary(nParamCount,nFreqNum,i);
	   }
	   if(nParamCount!=SRNO_CUPPI&&nLimitIndex!=BOTH_PAIRS)
	   {
		  configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,nLimitIndex,0));
		  SummaryInfo=ReadSummary(nParamCount,nFreqNum,nLimitIndex);
	   }
	   else
	   if(nParamCount!=SRNO_CUPPI&&nLimitIndex==BOTH_PAIRS)
	   {
		configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,i,0));
		  SummaryInfo=ReadSummary(nParamCount,nFreqNum,i);
	   }
	   //end

	   //Below conditions will check for the alarm
	   //these are modified by:sdt:04092001:1010.
	   //start
	    if(configLimit.averagelow!=(float)BLANKVALUE)
	{
		 //if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fAvg<configLimit.averagelow)
		   if(SummaryInfo.STATNORMALISED.fAvg<configLimit.averagelow)
			nAlarm=TRUE;
	    }
	if(configLimit.averagehi!=(float)BLANKVALUE)
	    {
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fAvg>configLimit.averagehi)
		  if(SummaryInfo.STATNORMALISED.fAvg>configLimit.averagehi)
			nAlarm=TRUE;
	}
	    if(configLimit.stdlow!=(float)BLANKVALUE)
	{
			//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fStddev<configLimit.stdlow)
		  if(SummaryInfo.STATNORMALISED.fStddev<configLimit.stdlow)
			nAlarm=TRUE;
	    }
	if(configLimit.stdhi!=(float)BLANKVALUE)
	    {
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fStddev>configLimit.stdhi)
		  if(SummaryInfo.STATNORMALISED.fStddev>configLimit.stdhi)
			nAlarm=TRUE;
	}
	    if(configLimit.rmslow!=(float)BLANKVALUE)
	{
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fRms<configLimit.rmslow)
		  if(SummaryInfo.STATNORMALISED.fRms<configLimit.rmslow)
			nAlarm=TRUE;
	    }
	if(configLimit.rmshi!=(float)BLANKVALUE)
	    {
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fRms>configLimit.rmshi)
		  if(SummaryInfo.STATNORMALISED.fRms>configLimit.rmshi)
			nAlarm=TRUE;
	}
	   //end
    }*/
    //end
    //End
	//Added by:sdt:01102005:1725
	//Hence forth Limit Index is taken as 0 for all parameters.
	//As there are only single pair combinatios are present for all
	//parameters.
	nLimitIndex = 0;
	configLimit=CovertSpeckLimitToConfigLimit(FindLimits(nParamCount,nFreqNum,nLimitIndex,0));
	SummaryInfo=ReadSummary(nParamCount,nFreqNum,nLimitIndex);

	if(configLimit.averagelow!=(float)BLANKVALUE)
	{
		 //if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fAvg<configLimit.averagelow)
		if(SummaryInfo.STATNORMALISED.fAvg<configLimit.averagelow)
			nAlarm=TRUE;
	}
	if(configLimit.averagehi!=(float)BLANKVALUE)
	{
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fAvg>configLimit.averagehi)
		if(SummaryInfo.STATNORMALISED.fAvg>configLimit.averagehi)
			nAlarm=TRUE;
	}
	if(configLimit.stdlow!=(float)BLANKVALUE)
	{
			//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fStddev<configLimit.stdlow)
		if(SummaryInfo.STATNORMALISED.fStddev<configLimit.stdlow)
			nAlarm=TRUE;
	}
	if(configLimit.stdhi!=(float)BLANKVALUE)
	{
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fStddev>configLimit.stdhi)
		if(SummaryInfo.STATNORMALISED.fStddev>configLimit.stdhi)
			nAlarm=TRUE;
	}
	if(configLimit.rmslow!=(float)BLANKVALUE)
	{
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fRms<configLimit.rmslow)
		if(SummaryInfo.STATNORMALISED.fRms<configLimit.rmslow)
			nAlarm=TRUE;
	}
	if(configLimit.rmshi!=(float)BLANKVALUE)
	{
		//if(m_FileInfo.STATNORMALISED[nParamCount][nFreqNum].fRms>configLimit.rmshi)
		  if(SummaryInfo.STATNORMALISED.fRms>configLimit.rmshi)
			nAlarm=TRUE;
	}


	if(!nGiveAlarms)
		 nAlarm=FALSE;
	if(nAlarm)
	{
		 do
		 {
				//messagewindow("Parameter fail to match stat. specifications!! press Escape to continue");
				messageBox("Parameter fail to match stat. specifications!! press Escape to continue",mfError|mfOKButton );
				sound(BEEP);
				if(kbhit())
				{
					 nAlarm=getch();
					 if(nAlarm==NULL)
							nAlarm=getch();
				}
				delay(50);
				nosound();
				delay(50);
		 }while(nAlarm!=ESCAPE);
	}
}


float User_ceil(float fFlotVal,int nNo)
{
	float fRoundVal;
	long  lRoundVal;
	int   i;

	nNo =  FindNewDecimal(fFlotVal,nNo);

	fRoundVal = (fFlotVal)-(int)(fFlotVal);
	for(i=1;i<=nNo+1;i++)
		fRoundVal *=10;
	lRoundVal = (long)fRoundVal;
	fRoundVal = (float)lRoundVal;
	if(((long)fRoundVal%10)>4)
	{
		fRoundVal /= 10;
		fRoundVal ++;
	}
	else
		fRoundVal /= 10;
	lRoundVal = (long)fRoundVal;
	fRoundVal = (float)lRoundVal;
	for(i=nNo;i>0;i--)
		fRoundVal /=10;
	fRoundVal = fRoundVal + (int)fFlotVal;
	return fRoundVal;
}

int FindNewDecimal(float fFlotVal,int nNo)
{
  int nLen;
  char szRaw[15];

  nLen = strlen(itoa(fFlotVal,szRaw,10));
  if (prmarray->Decimal[1] == 0)
  {
	nIntNo = nLen;
	nDecNo = nNo = 0;
	return(nNo);
  }

  if (nLen < nNo)
	 nNo = nNo - nLen;
  else
	 nNo = 0;
	if (fFlotVal == 0.0)
		nNo = 0;
	nIntNo = nLen;
	nDecNo = nNo;

 return(nNo);
}

/*void InitSummaryFile(void)
{
	char path[MAXPATH];
	SUMMARYINFO SummaryInfo;
	//SUM Summary;
	FILE *fpSummary;
	memset(&SummaryInfo,0,sizeof(SummaryInfo));
	sprintf(path,"%s:\\summary.bin",cRamDrive);
	remove(path);
	fpSummary=fopen(path,"wb");
	for(int nParam=0;nParam<MAXPARAMETERS;nParam++)
	{
		for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
        {
        	for(int nSumType=0;nSumType<MAXSUMMARYTYPES;nSumType++)
            {
            	fwrite(&SummaryInfo,sizeof(SUMMARYINFO),1,fpSummary);
            }
        }
    }
}

//Added by:sdt:02092001
void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo)
{
	char path[MAXPATH];
    FILE *fpSummary;
	sprintf(path,"%s:\\summary.bin",cRamDrive);
	fpSummary=fopen(path,"rb+");
	fseek(fpSummary,sizeof(SUMMARYINFO)*((nParam*MAXFREQUENCIES*MAXSUMMARYTYPES)+(nFreqNum*MAXSUMMARYTYPES)),SEEK_SET);
	fseek(fpSummary,sizeof(SUMMARYINFO)*(nSumType),SEEK_CUR);
	fwrite(&SummaryInfo, sizeof(SUMMARYINFO), 1, fpSummary);
	if(fpSummary){fclose(fpSummary);}
}

//Added by:sdt:03092001
SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType)
{
	char path[MAXPATH];
	FILE *fpSummary;
	SUMMARYINFO SummaryInfo;
	memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
	sprintf(path,"%s:\\summary.bin",cRamDrive);
	fpSummary=fopen(path,"rb");
	fseek(fpSummary,sizeof(SUMMARYINFO)*((nParam*MAXFREQUENCIES*MAXSUMMARYTYPES)+(nFreqNum*MAXSUMMARYTYPES)),SEEK_SET);
	fseek(fpSummary,sizeof(SUMMARYINFO)*(nSumType),SEEK_CUR);
	fread(&SummaryInfo, sizeof(SUMMARYINFO), 1, fpSummary);
	if(fpSummary){fclose(fpSummary);}
	return SummaryInfo;
}*/

int GetLimitIndex(int Param)
{
	//int SwitchMode;//Commented by:sdt:13112005:1405
	//static SwitchModeFlag=0;
	 return 0;//Code shifted here by:sdt:01102005:1715
	//Commented by:sdt:01102005:1715
	//As Combinations are not considered hence for Quad software.
	//All limits will be stored at Limit Index=0.
	/*if(prmarray->nTestingMethod)
	{
		return 0;
	}
	else
	{
		if(Param == SRNO_CUPP)
			SwitchMode = PairSwitchOption.CUPP;

		if(Param == SRNO_CUPPI)
			SwitchMode = PairSwitchOption.CUPPI;

		if(Param == SRNO_ELFEXT)
			SwitchMode = PairSwitchOption.ELFEXT;

		if(Param == SRNO_NEXT)
			SwitchMode = PairSwitchOption.ONEXT;

		//Added:jj:03062001
		if(Param == SRNO_FEXT)
			SwitchMode = PairSwitchOption.FEXT;
		if(Param == SRNO_INEXT)
			SwitchMode = PairSwitchOption.INEXT;


		if(Param !=SRNO_CUPPI)
		{
			switch(SwitchMode)
			{
				 case ADJPAIRS : return 0;
				 case NONADJPAIRS :	return 1;
				 case BOTH_PAIRS : return nAdjNonAdjFlag;

			}
			return 0;
		}
		else
		{
			switch(SwitchMode)
			{
				//MOD11072002:02:jj
				//case ADJUNITS : return 2;
				//case NONADJUNITS :return 3;
				//case BOTH_UNITS : return (nAdjNonAdjFlag+2);
				case ADJUNITS : return 3;
				case NONADJUNITS :return 4;
				case BOTH_UNITS : return (nAdjNonAdjFlag+3);
			}
			return 2;
		}
	}*/

}

void ExchangeDelayInfo (int nExchange)
{
	 //int 			i;//19032001
	 //static INIINFO	TmpIni;
	 static RdControl temp_rdctrl;
	 char path[MAXPATH];

	 if(nExchange)
	 {

		for (int i=0; i<MAX_DELAYS;i++)
		{
			temp_rdctrl.m_anDelay[i] = rdCtrl.m_anDelay[i];

		}
		temp_rdctrl.m_nAvgof = rdCtrl.m_nAvgof;

		rdCtrl.m_anDelay[DLY_INSTRUMENT] = Hz_80_Info.InstruDelay;
        rdCtrl.m_anDelay[DLY_JUNCTION] = Hz_80_Info.JunctionDelay;
		rdCtrl.m_anDelay[DLY_RELAY] = Hz_80_Info.RelayDelay;
        rdCtrl.m_anDelay[DLY_PREREAD] = Hz_80_Info.PreReadDelay;
        rdCtrl.m_anDelay[DLY_RANGE] = Hz_80_Info.RangeDelay;

		sprintf(path,"%s:\\cts.ini",cRamDrive);
		rdCtrl.m_nAvgof=GetPrivateProfileInt("80HZ_CTDELAY_INFO","AVERAGE_OF_READINGS",400,
											path);
	 }
	 else
	 {
		for (int i=0; i<MAX_DELAYS;i++)
		{
			rdCtrl.m_anDelay[i] = temp_rdctrl.m_anDelay[i];
		}
		rdCtrl.m_nAvgof = temp_rdctrl.m_nAvgof;
	 }
}


int SelectFixturePairtoPair (int nStart,int nCombnType)
{
	int nBypass;
	if(nStart)
	{
		gflagReplace1 = 0;
		gflagReplace2 = 0;

		if(nCombnType==ADJPAIRS)
		{
			SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
			nAdjNonAdjFlag=0;
        }
        if(nCombnType==NONADJPAIRS)
        {
			SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
            nAdjNonAdjFlag=1;
        }
        if(nCombnType==BOTH_PAIRS)
		{
        	SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
            nAdjNonAdjFlag=0;
            nBothFlag=1;
        }

       	CtPair.Pair1=SeqPair->Pairno1;
        CtPair.Pair2=SeqPair->Pairno2;
		//nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		nPrevPair2=0;
		nPrevPair1=0;
		nBypass=SetFixture(CtPair.Pair1,CtPair.Pair2,FALSE);

		CtPair.UnitSrNo=1;
		nPrevPair1 = CtPair.Pair1;
		nPrevPair2 = CtPair.Pair2;
	}
	else
	{
		if(SeqPair!=NULL)
        {
            SeqPair=SeqPair->link;
            if(SeqPair!=NULL)
            {

                 //Added by:sdt:03072001
		         //for Selecting the pairs lie in the Last Unit only.
                 if(CtPair.UnitSrNo==nUnits)
                 {
					while(((SeqPair->Pairno1 >nLastUnitSize)||(SeqPair->Pairno2 >nLastUnitSize))
	        			   && (SeqPair!=NULL))
	    		     {
			        	SeqPair= SeqPair->link;
        			 }
                 }
		    	//CtPair.Pair1=SeqPair->Pairno1;
	            //CtPair.Pair2=SeqPair->Pairno2;
                //Modified by:sdt:24042001:for the unit proper unit no. display.

				 CtPair.Pair1=SeqPair->Pairno1+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
				 CtPair.Pair2=SeqPair->Pairno2+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
            }
        }
        if(SeqPair==NULL)
		{
        	CtPair.UnitSrNo++;//Added:jj:29042001
			if(nCombnType==ADJPAIRS)
            {
				SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
            	nAdjNonAdjFlag=0;
            }
			if(nCombnType==NONADJPAIRS)
		    {
			   	SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
                nAdjNonAdjFlag=1;
            }
            if(nCombnType==BOTH_PAIRS)
			{
              	if(nBothFlag)
                {
                  	SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
                    nBothFlag=0;
                    nAdjNonAdjFlag=1;
                    CtPair.UnitSrNo--;//Added:jj:29042001

                }
                else
				{
                   	SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
                    nBothFlag=1;
                    nAdjNonAdjFlag=0;
                }

             }
             //CtPair.UnitSrNo++;//Commented:jj:29042001

			 //Added by:sdt:03072001
		     //for Selecting the pairs lie in the Last Unit only.
             if(CtPair.UnitSrNo==nUnits)
             {
             		while(((SeqPair->Pairno1 >nLastUnitSize)||(SeqPair->Pairno2 >nLastUnitSize))
	        			   && (SeqPair!=NULL))
		    	    {
				        	SeqPair= SeqPair->link;
        	        }
             }
			 //Modified by:sdt:24042001:for the unit proper unit no. display.
			 CtPair.Pair1=SeqPair->Pairno1+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));
			 CtPair.Pair2=SeqPair->Pairno2+((CtPair.UnitSrNo-1)*atoi(nConfig.szUnitof));

             //Commented by:sdt:03072001
             /*if(CtPair.UnitSrNo == nUnits)
			 {
				nTempUnitOf = atoi(nConfig.szUnitof);
				itoa(nLastUnitSize,nConfig.szUnitof,10);
			 }*/
		 //nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		}
		nBypass=SetPair(CtPair.Pair1,CtPair.Pair2,nPrevPair1,nPrevPair2);
		over++;
		nPrevPair1=CtPair.Pair1;
		nPrevPair2=CtPair.Pair2;
	}
	return nBypass;
}


int SelectFixtureSinglePair (int nStart)
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
		CtPair.Pair2=0;
		nBypass=SetFixture(CtPair.Pair1,0,FALSE);
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
}



float GetFixOffsetFromArray (int param,int pair1,int pair2,int voltage)
{
	SFixOffsetVal m_FixOffsetVal;

    //Added:jj:29032001
    unsigned long      ALocationFix;
	float              AFixOffsetVal;
    FILE *sys;
	SFixOffsetVal           OffsetValFix;   // End of Storing the System Offset


	memset (&m_FixOffsetVal,0,sizeof (m_FixOffsetVal));

	//m_FixOffsetVal.m_FixOffsetLocation.ParameterAbr = param - 6;//Commented by:sdt:17052001
    m_FixOffsetVal.m_FixOffsetLocation.ParameterAbr = param - SRNO_ATN; //Added by:sdt:17052001
	m_FixOffsetVal.m_FixOffsetLocation.Range = nCurrentRange;
	m_FixOffsetVal.m_FixOffsetLocation.Voltage = voltage;
	m_FixOffsetVal.m_FixOffsetLocation.Freq = nConfig.nHFrequency+1;

	if (voltage == V1)
		m_FixOffsetVal.m_FixOffsetLocation.Block = INTERBLOCK; // Changed 13th May 2000,SEPL
//		 m_FixOffsetVal.m_FixOffsetLocation.Block = INTRABLOCK;
	else if (voltage == V2 && pair2 != 0)
	{
//		m_FixOffsetVal.m_FixOffsetLocation.Block=(pair%BLOCKSIZE)? INTERBLOCK:INTRABLOCK;
	int u1=0,u2=0;
		//Modified by:sdt:04122005:1925
		//Hard coded unit size 10 is replaced by nConfig.szUnitof
		if ( pair1%atoi(nConfig.szUnitof) != 0 )
		{
			u1 = pair1 + ( atoi(nConfig.szUnitof)-(pair1%atoi(nConfig.szUnitof)) );
			u1 /= atoi(nConfig.szUnitof);
		}
		else
			u1 = pair1 / atoi(nConfig.szUnitof);
		if ( pair2%atoi(nConfig.szUnitof) != 0 )
		{
			u2 = pair2 + ( atoi(nConfig.szUnitof)-(pair2%atoi(nConfig.szUnitof)) );
			u2 /= atoi(nConfig.szUnitof);
		}
		else
			u2 = pair2 / atoi(nConfig.szUnitof);
		if ( u1 == u2 )
			m_FixOffsetVal.m_FixOffsetLocation.Block=INTERBLOCK;
		else
			m_FixOffsetVal.m_FixOffsetLocation.Block=INTRABLOCK;
	}
	else
		m_FixOffsetVal.m_FixOffsetLocation.Block=INTERBLOCK;

	//Commented:jj:29032001
	/*for (i = 0; i<1000;i ++)
		 if (ALocationFix [i] == m_FixOffsetVal.nLocation)
			 return AFixOffsetVal [i];*/


	//Added:jj:29032001
	 if ((sys = fopen ("fix.off","rb")) == NULL)
		return 0;
	 while (fread (&OffsetValFix,sizeof (OffsetValFix),1,sys) > 0)
	 {
		AFixOffsetVal  = OffsetValFix.fFixOffsetVal;
		ALocationFix  = OffsetValFix.nLocation;

		if (ALocationFix  == m_FixOffsetVal.nLocation)
		{
			 if(sys){fclose(sys);}
			 return AFixOffsetVal ;
		}

	 }
	 if(sys){fclose(sys);}

	return 0;
}

//Added by:sdt:04102005:2020
//This function will select fixture for Xtalk parameters
int SelectFixtureXTalkInQuad (int nStart,int nCombnType)
{
	int nBypass;
	if(nStart)
	{
		gflagReplace1 = 0;
		gflagReplace2 = 0;

		if(nCombnType==ADJPAIRS)
			SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs;
		if(nCombnType==NONADJPAIRS)
			SeqPair=CableSequence.TotalPairs.Units.NonAdj_Pairs;
		if(nCombnType==ALL_COMBINATIONS)
			SeqPair=CableSequence.TotalPairs.Units.All_Combinations;
		//MOD2802200601:2220
		if(nCombnType==ADJPAIRS_INCABLE)
			SeqPair=CableSequence.TotalPairs.Units.Adj_Pairs_InCable;


		CtPair.Pair1=SeqPair->Pairno1;
		CtPair.Pair2=SeqPair->Pairno2;
		//nCount = CtPair.Pair1 + (atoi(nConfig.szUnitof) - 1);
		nPrevPair2=0;
		nPrevPair1=0;
		nBypass=SetFixture(CtPair.Pair1,CtPair.Pair2,FALSE);

		CtPair.UnitSrNo=1;
		nPrevPair1 = CtPair.Pair1;
		nPrevPair2 = CtPair.Pair2;
	}
	else
	{
		if(SeqPair!=NULL)
		{
			SeqPair=SeqPair->link;
			if(SeqPair!=NULL)
			{
				CtPair.Pair1=SeqPair->Pairno1;
				CtPair.Pair2=SeqPair->Pairno2;
			}
		}
		nBypass=SetPair(CtPair.Pair1,CtPair.Pair2,nPrevPair1,nPrevPair2);

		nPrevPair1=CtPair.Pair1;
		nPrevPair2=CtPair.Pair2;
	}
	return nBypass;
}


void StopPrint ()
{
	//char path[80];//Commented by:sdt:13112005:1405

   //system( "c:\\pcache\\pcache /c" );//Modified:Given complete path:pnb:16122000:1500
   //Modified by:sdt:04112005:1230//As system command not working in TV version.
   spawnlp(P_WAIT,"c:\\pcache\\pcache.exe","c:\\pcache\\pcache.exe","/c",NULL);
   biosprint (1,0x00,0);
   //Commented by:sdt:04122005:1230 //As no "*.prn" is created, hence no question of deletion.
   //sprintf(path,"deltree /Y %s:\\*.prn > message",RamDrv);//22012001
   //system (path);
}

