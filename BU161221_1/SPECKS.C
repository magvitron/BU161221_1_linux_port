//=========================================================================//
//						Modifications
//
// date :06072001 time :1220 by:sdt
// 1. For checking the duplicate Sequence names in .SEQ file,
//	  CheckforDuplicateCableSequence() function is added.
//	  this function just called before displaying Cable Sequence List,
//	  through the Specks Menu.
//=========================================================================//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include <io.h>//Added:jj:18042001
#include <fcntl.h>
#include <alloc.h> //Added by:sdt:22042001
#include <dir.h> //Added by:sdt:03052001

#include "ct.h"
#include "share.h"
#include "key.h"
#include "menu.h"
#include "copyrite.h"
#include "seq.h" //added by:sdt:22042001
//Added by:sdt:23082001
extern FreqStruct LFFrequency;
//Added by:sdt:22082001
SPECKLIMIT specksLimit;
void ModifyIntermediateLimits(int nParam,int nFreq,int nLimitIndex,SPECKLIMIT specksLimit);
//Added by:sdt:21082001
void WriteLimitInSivFile(int SpeckFileNo,int bNewFile);
int SpeckFreqLimitinit (int left, int top);
void ReadLimitFromSivFile(int SpeckFileNo);
extern char RamDrv[2];
SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex,int bCreateSpeckFlag);
void CopyLimitsToIntmLim(void);
//Added by:sdt:20082001
int nCurrFreq;
//Added by:sdt:17072001
extern HFFREQUECYINFO HFfreqInfo;
void CheckValidHFFreqInSpecks();
//Added by:sdt:13072001
int verifynormtemp(char *string);
//Added by:sdt:06072001
SEQNAMES * CheckforDuplicateCableSequence(void);
//Added by:sdt:03052001
int CalculateFileNumber(char *FileName);

//Added by:sdt:04052001
SEQNAMES Sequence_Names[CABLESEQMNULENGHTH];

//Added by:sdt:29042001
extern char 	szSeqFileName[15];

//Added by:sdt:22042001
//Start
PULLMENULINE CableSeqLines[8];
PULLMENU cableseqmenu={"Cable Sequence",CableSeqLines,-1};
int nPageCount;
SEQUENCE CableSequence;
//char Sequence_Names[CABLESEQMNULENGHTH][30];
//End

extern int 			ParamCtsHandle;
void specklimitscontinue (int key);

//MOD08072002
//char LimitTypes[4][30]={"Intra-Adjacent Pairs","Intra-Nonadjacent Pairs","Adjacent Units","NonAdjacent Units"};
char LimitTypes[5][30]={"Intra-Adjacent Pairs","Intra-Nonadjacent Pairs","All Combinations","Adjacent Units","NonAdjacent Units"};

extern char SpeckList[10][30];
int stage;

int Pcm_Cable=0;
int SparePairs=0;
int compart=0;
int compartsizeinunits=0;


extern PRM			prmarray;//Uncommented:jj:18042001
extern PARAMINFO	paraminfo[ MAXPARAMETERS+2 ];//Modified by:sdt:31072001
extern PARAMINFO	paraminfo1[ MAXPARAMETERS+2 ];//Modified by:sdt:31072001

int  nSpeckFileNo;
char SpeckNo[20];
BOOL Head = TRUE;  /* variable use to print Title or Specification */

/*
    File pointers used in this code file given below
*/

extern FILE * fptr_Report; //Comemnted by:sdt:21082001:temperarily
// FILE * fptr_Report; //Modified by:sdt:21082001:temperarily
extern FILE * RepView;
int    SearchStrInFile (char *FileName,char *Key);

extern struct time t;
extern struct date d;
extern int error;
extern int nDC;

SPECIFICATION speck;

//Added by:sdt:22042001
//start
void CableSeqEventHandling();
void GetCableSequenceTitles(int Pageno);//Added by:sdt:11042001
void GetCableSequence(char *SeqTitle);//Added by:sdt:12042001
int CharOccurInString(char *String,char ch);//Added by:sdt:12042001
int CableSequenceInit(int left,int top);//Added by:sdt:12042001
char *StringToken(char *s1,char ch);//Added by:sdt:17042001
SEQPAIR *ReadPairsFromFile(FILE *fpPtr);//Added by:sdt:19042001
FILE *ReturnFilePtr(char szAppName[10][30],char *szkeyname,char *FileName);//Added by:sdt:19042001
void CableSeqContinue(int key);
//end

void speckparamcontinue1 (int key);


void SelectNlq(char);
void SelectShrink(char);
void EndReport(void);
int fwritespeck (SPECIFICATION *speckptr);
int speckparaminit (int left, int top);
void speckparamcontinue (int key);
int getspeckrhs (int left, int top, char *string, int maxlen, char *prompt, int (*returnfunction)(char *));
char *ToUpperStr (char *string);

/*
    Function definition starts below
*/
void speckinitialize (void)
{
	char szSpeckFile[20];
	FILE *sFp;

	nSpeckFileNo=1;
	strcpy(szSpeckFile,"1.siv");
	sFp=fopen(szSpeckFile,"rb");
	if(sFp==NULL)
		fwritespeck(&speck);
	else
		eatcopyright (sFp);

	freadspeck (&speck);
	if(sFp){fclose(sFp);}
}

int freadspeck (SPECIFICATION *speckptr)
{
	FILE *sFp;
	char szSpeckFile[20];
	int nRead=0;

	sprintf(szSpeckFile,"%d.siv",nSpeckFileNo);
	sFp=fopen(szSpeckFile,"rb");
	if(sFp!=NULL)
	{
		eatcopyright (sFp);
		nRead=fread (speckptr, sizeof (SPECIFICATION), 1, sFp);
		if(sFp){fclose(sFp);}
        ReadLimitFromSivFile(nSpeckFileNo);//Added by:Sdt:21082001
		return nRead;
	}
	else
		return FALSE;
}

int fwritespeck (SPECIFICATION *speckptr)
{
	SPECIFICATION nullspeck;
	int	param;
	FILE *sFp;
	char szSpeckFile[20];
	int nWrite;
	int bNewFile=0;//Added by:sdt:20082001. This flag will be true
    // is given speck file will not be present.(for new speck file).

	sprintf(szSpeckFile,"%d.siv",nSpeckFileNo);
	sFp=fopen(szSpeckFile,"rb");
	if(sFp==NULL)
	{
    	bNewFile = 1; //Added by:sdt:20082001
		memset (speckptr = &nullspeck, '\0', sizeof (SPECIFICATION));
		sprintf(speckptr->speckname,"%d",nSpeckFileNo);
		sprintf(speckptr->speckno,"%d",nSpeckFileNo);
        strcpy(speckptr->type,"No");

        //Modified:jj:05042001
		//for( param=0;param<MAXPARAMETERS; param++)
   		for( param=0;param<MAXPARAMETERS-1; param++)
		{
           //Below For Loop is Commented by:sdt:20082001
           /*	for(int x=0;x<4;x++)
		   {
		   	strcpy(speckptr->speckparams[param].specklimits[x].lowerlimit,"");
			strcpy(speckptr->speckparams[param].specklimits[x].upperlimit,"");
			strcpy(speckptr->speckparams[param].specklimits[x].averagelow,"");
			strcpy(speckptr->speckparams[param].specklimits[x].averagehi,"");
			strcpy(speckptr->speckparams[param].specklimits[x].stdlow,"");
			strcpy(speckptr->speckparams[param].specklimits[x].stdhi,"");
			strcpy(speckptr->speckparams[param].specklimits[x].rmslow,"");
			strcpy(speckptr->speckparams[param].specklimits[x].rmshi,"");
			//strcpy(speckptr->speckparams[param].specklimits[x].norlength,"");
            //strcpy(speckptr->speckparams[param].specklimits[x].mean_stddev,"");//Commented:jj:18042001:1430
           }*/
            ////Commented:jj:18042001
            //strcpy(speckptr->speckparams[param].intralowerupper,"");

           //Added:jj:29042001
           strcpy(speckptr->speckparams[param].norlength,"");
           strcpy(speckptr->speckparams[param].norTemp,"");

            //Added:jj:16032001

             for(int i=0;i<10;i++)
             	strcpy(speckptr->speckparams[param].freq[i],"No");


		}
	}
	if(sFp){fclose(sFp);}
	sFp=fopen(szSpeckFile,"wb");
	putcopyright (sFp);
	nWrite= fwrite (speckptr, sizeof (SPECIFICATION), 1, sFp);
    if(sFp){fclose(sFp);}

	WriteLimitInSivFile(nSpeckFileNo,bNewFile);//Function call added by:sdt:20082001

	return nWrite;
}



int findspeck (char *specknumber, char *cabletype, SPECIFICATION *speckptr)
{
	int nSaveSpeckNo=nSpeckFileNo;

	if (strlen (specknumber)== 0 )	  /*if no speck is specified then ret 2*/
		return 2;
	ToUpperStr(specknumber);
	ToUpperStr(cabletype);
	for(nSpeckFileNo=1;nSpeckFileNo<40;nSpeckFileNo++)
	{
		if (freadspeck(speckptr)<1)
		{
			nSpeckFileNo=nSaveSpeckNo;
			return 0;
		}
		else
		{
			if (strncmp (ToUpperStr(speckptr->speckname), specknumber,
						  sizeof (speckptr->speckname)) == 0  &&
				strncmp (ToUpperStr(speckptr->cabletype), cabletype,
						  sizeof (speckptr->cabletype)) == 0)
				return 1;
		}
	}
	return 0;
}

#define NUMBER 		1
#define SPECKNUMBER 8
//#define CABLETYPE 	2
#define PARAMS 		3
#define PRINT_SPECK  4
#define GUAGE		 5
#define INTERUNIT    6//Added:pnb:19111999.
#define INTRAUNIT    7

#define CABLESEQUENCE  9//Added:jj:11042001


PULLMENULINE specklines[] =
{
	{"Specification Number",speck.speckno,MAXLENSPECKSNUMBER-1,SPECKNUMBER},
	{"Specification Name",speck.speckname,MAXLENSPECKSNAME-1,NUMBER},
	{"Cable Type_Guage",speck.cabletype,(MAXLENCABLETYPE+MAXLENGUAGE)-1,CABLETYPE},  //##NDG :CableType=Cabletype+CableGauge
	{"Parameters",NULL,0,PARAMS},
	{"Print Specks",SpeckNo,MAXLENSPECKSNAME-1,PRINT_SPECK },
    {"Cable Sequence",speck.szCableSequence.SeqName,20,CABLESEQUENCE },
	//{"Pcm",speck.type,MAX_SPECKTYPE_SZ-3,INTRAUNIT }, //Commented:jj:11042001
	//{"Unit To Unit",InterUnit,MAXLENINTERUNIT-1,INTERUNIT},//Added:pnb:18111999.//Commented:pnb:30121999.
	{{NULL},{NULL},0,-1}
};

PULLMENU speckmenu = {"Edit Specifications", specklines, -1};

int speckmenuinit (int left, int top)
{
	char string[80];

//	sprintf( string, "                         %c    %c    %c    %c   <��"
//					 "  ESC   PgUp    PgDn  F2 ", 24, 25, 26, 27, 27 );
	sprintf( string,"PgUp,PgDn: New  F2: Save");
	messagewindow( string);
	//dnr
	//cont_messageflag = 1;
	menustack[++menustackptr].menufunction = speckcontinue;
	return pullmenuinit (&speckmenu, left, top);
}


void speckcontinue (int key)
{
	int left, top;
	char *s,string[80];
    char path[MAXPATH];
//	sprintf( string, "                         %c    %c    %c    %c   <��"
//					 "  ESC   PgUp    PgDn  F2 ", 24, 25, 26, 27, 27 );
	sprintf( string,"PgUp,PgDn: New  F2: Save");
	messagewindow( string);
	switch (key)
	{
	case PAGEDOWN:
		if(speck.speckname[0]== NULL || speck.cabletype[0] == NULL)
			break;
		nSpeckFileNo++;
		left = topofstack.left;
		top = topofstack.top;
		//topofstack.menufunction (ESCAPE);
		topofstack.menufunction (ESCAPE);
		switch (freadspeck(&speck))
		{
		case 0:
			fwritespeck (NULL);
			freadspeck (&speck);
		case 1:
			break;
		default:
			error++;
			break;
		}
		speckmenuinit (left, top);
		return;
	case PAGEUP:
		nSpeckFileNo--;
		if(nSpeckFileNo<1)
			nSpeckFileNo=1;
		left = topofstack.left;
		top = topofstack.top;
		//topofstack.menufunction (ESCAPE);
		topofstack.menufunction (ESCAPE);
		switch (freadspeck(&speck))
		{
		case 1:
			break;
		default:
			return;
			//error++;
			//break;
		}
		speckmenuinit (left, top);
		return;

    case F2 :		//Added by:sdt:16042001
		fwritespeck(&speck);
		break;

   	case ESCAPE :     //Added by:sdt:22082001
    	sprintf(path,"%s:\\intmlim.bin",RamDrv);
	    remove(path);
        break;


	default:
        //freadspeck(&speck); //Commented by:sdt:17052001
		break;
	}
	left = PMTEXTLEFT;
	top = PMTEXTTOP (topofstack.currentoption)+8+BMYMARGIN;
	s = specklines[topofstack.currentoption].rhs;
	switch (pullmenucontinue (key))
	{
	case 0 :
		break;
	case NUMBER:
		getspeckrhs(left,top,s,MAXLENSPECKSNAME-1,"Specification Name",NULLFUNC);
		break;
	case SPECKNUMBER:
        //Commented by:sdt:30072001
		//getspeckrhs(left,top,s,MAXLENSPECKSNUMBER-1,"Specification Number",NULLFUNC);
		break;
	case CABLETYPE:
		getspeckrhs(left,top,s,(MAXLENCABLETYPE+MAXLENGUAGE)-1,"Cable Type_Guage",NULLFUNC);
		break;
	case PARAMS:
        CopyLimitsToIntmLim();//Added by:sdt:22082001
		speckparaminit (left, top);
		break;
//amit for printing specification
	case PRINT_SPECK :
                // system ("print > message");
/*		        int Result = SearchStrInFile ("message","off-line");
				if (Result == TRUE)
		        {
		          messagewindow ("Please Check the Printer");
		          getch ();
				  break;
		        }
		        else
		        {
			        Result = SearchStrInFile ("message","PRINT queue is empty");
			        if (Result)
						system ("deltree /Y g:\\*.prn > message");
			        else
					{
			          messagewindow ("Wait while printing...");
			          getch ();
					  break;
					}
				}*/
				nDC=PRINTER;
                 if ((fptr_Report = fopen ("PRN","wb"))==NULL)
  				 {
	 				messagewindow ("Drive Not Available");
	 				getch ();
                    return ;
  				}
                topofstack.menufunction(F5);
                topofstack.menufunction(F5);
                #ifdef EXECUTE //Added by:sdt:21082001
                PrintSpeck();
                EndReport();
                #endif //Added by:sdt:21082001
                if(fptr_Report){fclose(fptr_Report);}
		break;
	case INTRAUNIT:
		getspeckrhs(left,top,s,MAX_SPECKTYPE_SZ-3,"Intra Unit Spec.",NULLFUNC);
		break;
    case CABLESEQUENCE : //Added by:sdt:22042001
            //int nTotalSequence;
	        //char strtmp[5];//commented by: sdt:07052001
            SEQNAMES *ptrReturnSeqNames;
	    	nPageCount =0;
            ptrReturnSeqNames=CheckforDuplicateCableSequence();
            if(!strcmp(ptrReturnSeqNames[0].FileName,""))
            {
				GetCableSequenceTitles(0);

    	        //GetPrivateProfileString("SEQ_LIST","TOTAL_NUMBER_OF_SEQUENCES","0",strtmp,"cable.seq");
			    //nTotalSequence=atoi(strtmp);

			    for(int i=0;(Sequence_Names[i].SeqName[0]!=NULL)&&i<(CABLESEQMNULENGHTH-1) ;i++)
			    {
		    		strcpy(CableSeqLines[i].lhs,Sequence_Names[i].SeqName);
				    CableSeqLines[i].rhs=NULL;
				 	CableSeqLines[i].rhslength=0;
				  	CableSeqLines[i].returnvalue=i;//ENTER
		   		}
	            for(;i<CABLESEQMNULENGHTH ;i++)
    	        {
        	        CableSeqLines[i].lhs[0] = NULL;
				    CableSeqLines[i].rhs=NULL;
			  		CableSeqLines[i].rhslength=0;
			   		CableSeqLines[i].returnvalue=-1;
    	        }

	    		CableSequenceInit(left,top);
	        }
            else
            if(strcmp(ptrReturnSeqNames[0].FileName,"")&&
					(!strcmp(ptrReturnSeqNames[0].SeqName,"")))
            {
				topofstack.menufunction(ESCAPE);
                topofstack.menufunction(ESCAPE);
                char szErrorMessage[80];
                sprintf(szErrorMessage,"Put proper Sequence Name in File %s",ptrReturnSeqNames[0].FileName);
                messagewindow(szErrorMessage);
                sound (BEEP);
				delay (250);
				nosound ();
            }
            else
            if((strcmp(ptrReturnSeqNames[0].SeqName,""))&&(strcmp(ptrReturnSeqNames[1].SeqName,"")))
            {
            	topofstack.menufunction(ESCAPE);
                topofstack.menufunction(ESCAPE);
                char szErrorMessage[80];
                sprintf(szErrorMessage,"File %s & %s have duplicate Sequence Names",ptrReturnSeqNames[0].FileName,ptrReturnSeqNames[1].FileName);
                messagewindow(szErrorMessage);
                sound (BEEP);
				delay (250);
				nosound ();
            }
            farfree(ptrReturnSeqNames);
    		break;

	case ESCAPE:
		//dnr
		//cont_messageflag = 0;
		break;
/*	case INTERUNIT :  //Added:pnb:19111999.
			getspeckrhs(left,top,s,(MAXLENINTERUNIT)-1,"Unit To Unit",NULLFUNC);
			if(strcmp(strupr(InterUnit),"YES")==0 || strcmp(strupr(InterUnit),"Y")==0)
				Pcm_Cable=1;
			else
				Pcm_Cable=0;
*/
			/*if(strcmp(szSingleStep,"Yes")==0)
				strcpy(szSingleStep,"No");
			else
				strcpy(szSingleStep,"Yes");*/
			//topofstack.menufunction (F5);
//		break;
	default :
		break;
	}
	return;
}

static int parameternumber;
static int limitarray;


/*PULLMENULINE speckparamlines2[] =
{
	{"Sequence Names", NULL,   PARAMETERNAMESIZE-1, PARAMNAME},
	{"Individual minimum", NULL, LIMITSIZE-1, INDMIN},

    {{NULL},{NULL},0,-1}
};*/



PULLMENULINE speckparamlines[] =
{
	{"Parameter Name", NULL,   PARAMETERNAMESIZE-1, PARAMNAME},

    //{"Limits",NULL,0,LMTS},//Commented by:sdt:20082001
    {"Frequency & Limits",NULL,0,FREQLMTS},//Added by:sdt:20082001
    //Commented:jj:18042001 for transfering the same to different menu
	/*{"Individual minimum", NULL, LIMITSIZE-1, INDMIN},
	{"Individual maximum", NULL, LIMITSIZE-1, INDMAX},
	{"Average minimum", NULL, AVERAGESIZE-1, AVERAGEMIN},
	{"Average maximum", NULL, AVERAGESIZE-1, AVERAGEMAX},
	{"Std Dev minimum", NULL, STDSIZE-1, STDMIN},
	{"Std Dev maximum", NULL, STDSIZE-1, STDMAX},
	{"RMS minimum", NULL, RMSSIZE-1, RMSMIN},
	{"RMS maximum", NULL, RMSSIZE-1, RMSMAX},*/
	{"Norm. Length", NULL, LENGTHSIZE-1, LENGTH_SPECKS},
    {"Norm. Temp.", NULL, LENGTHSIZE-1, TEMP_SPECKS},//Added by:sdt:13072001
    /*{"Mean-StdDev", NULL, MEAN_STD_SIZE-1, MEAN_STD_SPECKS},*/

    //{"Intra Unit Speck.", NULL, INTRA_LO_UPSIZE-1, INTRAUNIT_SPECKS},//Commented:jj:18042001
    //{"Frequency", NULL, 0, FRQ},//Commented by:sdt:20082001

    {{NULL},{NULL},0,-1}
};

PULLMENULINE specklimitlines[] =
{
	{"Limits for", NULL,   30,LIMITCATEGORY },
    {"Selected",NULL,5,FREQSTATUS},//Added by:sdt:20082001
	{"Individual minimum", NULL, LIMITSIZE-1, INDMIN},
	{"Individual maximum", NULL, LIMITSIZE-1, INDMAX},
	{"Average minimum", NULL, AVERAGESIZE-1, AVERAGEMIN},
	{"Average maximum", NULL, AVERAGESIZE-1, AVERAGEMAX},
	{"Std Dev minimum", NULL, STDSIZE-1, STDMIN},
	{"Std Dev maximum", NULL, STDSIZE-1, STDMAX},
	{"RMS minimum", NULL, RMSSIZE-1, RMSMIN},
	{"RMS maximum", NULL, RMSSIZE-1, RMSMAX},

    //MOD0507002:01:jj
	{"P.S minimum", NULL, RMSSIZE-1, PSMIN},
	{"P.S maximum", NULL, RMSSIZE-1, PSMAX},

    //MOD06072002:01:jj
	{"1%W minimum", NULL, RMSSIZE-1, WPNMIN},
	{"1%W maximum", NULL, RMSSIZE-1, WPNMAX},

	//{"Norm. Length", NULL, LENGTHSIZE-1, LENGTH_SPECKS},
    //{"Mean-StdDev", NULL, MEAN_STD_SIZE-1, MEAN_STD_SPECKS},//Commented:jj:18042001:1430

    {{NULL},{NULL},0,-1}
};


PULLMENULINE speckparamlines1[] =
{
    {"", NULL, 10 , FREQ1},
    {"", NULL, 10 , FREQ2},
    {"", NULL, 10 , FREQ3},
    {"", NULL, 10 , FREQ4},
    {"", NULL, 10 , FREQ5},
    {"", NULL, 10 , FREQ6},
    {"", NULL, 10 , FREQ7},
    {"", NULL, 10 , FREQ8},
    {"", NULL, 10 , FREQ9},
    {"", NULL, 10 , FREQ10},

	{{NULL},{NULL},0,-1}
};

void SetSpeckList(int nTmp,char *szTmp)//Added:jj:18032001
{
	strcpy(speckparamlines1[FREQBASE+nTmp].lhs,szTmp);
	for( int i=1; i<=5; i++ )
		speckparamlines1[FREQBASE+nTmp].lhs[20+i] = '\0';
}


PULLMENU speckparammenu={"Parameter Specifications",speckparamlines,-1};
PULLMENU speckparammenu1={"Frequencies",speckparamlines1,-1};

PULLMENU specklimitsmenu={"Limits",specklimitlines,-1};//Added:jj:18042001


int speckparaminit (int left, int top)
{
	char string[80];

	sprintf( string,"PgUp,PgDn: New  F2: Save");
	messagewindow( string);

	speckparamlines[0].rhs = speck.speckparams[parameternumber].name;
	strncpy (speckparamlines[0].rhs, paraminfo[parameternumber].szParamAbr,
			 sizeof (paraminfo[parameternumber].szParamAbr));

    //Modified:jj:29042001
	//speckparamlines[9].rhs = speck.speckparams[parameternumber].norlength;
	speckparamlines[2].rhs = speck.speckparams[parameternumber].norlength;
    speckparamlines[3].rhs = speck.speckparams[parameternumber].norTemp;

    stage=0;

	menustack[++menustackptr].menufunction = speckparamcontinue;
	return pullmenuinit (&speckparammenu, left, top);
}

//Added:jj:18042001
//int specklimitsinit (int left, int top)
int specklimitsinit (int left, int top,int nFreqNum)
{
    //SPECKLIMIT specksLimit; //Added by:sdt:21082001//Commented by:sdt:22082001
	char string[80];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
	sprintf( string,"PgUp,PgDn: New  F2: Save");
	messagewindow( string);


    if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
    {
		clrscr();
		printf( "\nError Opening File PARAM.CTS File for Reading..." );
		//RemoveIniFiles();
		exit( 1 );
	}
    ReadParamDotCts( paraminfo1[parameternumber].szParamAbr , &prmarray);
    close( ParamCtsHandle );

    //Added by:sdt:21082001//Commented by:sdt:23082001
    /*if(paraminfo[parameternumber].nParamType)
    {
       	if((!(HFfreqInfo.HFLstart <= nFreqNum && HFfreqInfo.HFLend >= nFreqNum))&&(!(HFfreqInfo.HFHstart <= nFreqNum && HFfreqInfo.HFHend >= nFreqNum)))
        {
	       //	strcpy(speckparamlines1[8].rhs,"No");
            messagewindow(" Not Valid HF frequency");
        	return 0; //Added by:sdt:23082001
        }

    }
    //Added by:sdt:21082001//Condition modified by:sdt:17072001
    if((!paraminfo[parameternumber].nParamType)&&(LFFrequency.nTotalFrequencies < nFreqNum))
    {
       	messagewindow(" Not Valid LF frequency");
        return 0; //Added by:sdt:23082001
    }*/

    specksLimit=FindLimits(parameternumber,nFreqNum,limitarray,1);//Added by:sdt:21082001

    if(prmarray.nTestingMethod)
    {
    	limitarray=0;
        specklimitlines[0].rhs =NULL;
    }
    else
	    specklimitlines[0].rhs = LimitTypes[limitarray];

    //Condition Added by:sdt:23082001.
    //CR ,LR & RU are Frequency Indepedent Parameters.
    if(parameternumber==SRNO_CR||parameternumber==SRNO_LR||parameternumber==SRNO_RU)
    {
    	specklimitlines[1].rhs = "";//speck.speckparams[parameternumber].freq[nFreqNum];
    }
    else
    {
	    //Added by:sdt:20082001
    	specklimitlines[1].rhs = speck.speckparams[parameternumber].freq[nFreqNum];
    }

	//Added by:sdt:21082001
    specklimitlines[2].rhs = specksLimit.lowerlimit;//,"")?specksLimit.lowerlimit:NULL;
	specklimitlines[3].rhs = specksLimit.upperlimit;//,"")?specksLimit.upperlimit:NULL;
	specklimitlines[4].rhs = specksLimit.averagelow;//,"")?specksLimit.averagelow:NULL;
	specklimitlines[5].rhs = specksLimit.averagehi;//,"")?specksLimit.averagehi:NULL;
	specklimitlines[6].rhs = specksLimit.stdlow;//,"")?specksLimit.stdlow:NULL;
	specklimitlines[7].rhs = specksLimit.stdhi;//,"")?specksLimit.stdhi:NULL;
	specklimitlines[8].rhs = specksLimit.rmslow;//,"")?specksLimit.rmslow:NULL;
	specklimitlines[9].rhs = specksLimit.rmshi;//,"")?specksLimit.rmshi:NULL;

    //MOD0507002:01:jj
    specklimitlines[10].rhs = specksLimit.psumlow ;
	specklimitlines[11].rhs = specksLimit.psumhi ;

    //MOD06072002:01:jj
    specklimitlines[12].rhs = specksLimit.wpnlow ;
	specklimitlines[13].rhs = specksLimit.wpnhi ;


    //Commented by:sdt:21082001
	/*specklimitlines[2].rhs = speck.speckparams[parameternumber].specklimits[limitarray].lowerlimit;
	specklimitlines[3].rhs = speck.speckparams[parameternumber].specklimits[limitarray].upperlimit;
	specklimitlines[4].rhs = speck.speckparams[parameternumber].specklimits[limitarray].averagelow;
	specklimitlines[5].rhs = speck.speckparams[parameternumber].specklimits[limitarray].averagehi;
	specklimitlines[6].rhs = speck.speckparams[parameternumber].specklimits[limitarray].stdlow;
	specklimitlines[7].rhs = speck.speckparams[parameternumber].specklimits[limitarray].stdhi;
	specklimitlines[8].rhs = speck.speckparams[parameternumber].specklimits[limitarray].rmslow;
	specklimitlines[9].rhs = speck.speckparams[parameternumber].specklimits[limitarray].rmshi;*/
	//specklimitlines[9].rhs = speck.speckparams[parameternumber].specklimits[limitarray].norlength;
    //specklimitlines[9].rhs = speck.speckparams[parameternumber].specklimits[limitarray].mean_stddev;//Commented:jj:18042001:1430

    //Commented:jj:18042001
    //speckparamlines[11].rhs = speck.speckparams[parameternumber].intralowerupper;
    nCurrFreq=nFreqNum;
	menustack[++menustackptr].menufunction = specklimitscontinue;
	return pullmenuinit (&specklimitsmenu, left, top);

}


int speckparaminit1 (int left, int top)
{
	char string[80];

	sprintf( string,"PgUp,PgDn: New  F2: Save");
	messagewindow( string);

    stage=1;

    //Added:jj:17032001
     for(int i=0;i<10;i++)
     	speckparamlines1[i].rhs = speck.speckparams[parameternumber].freq[i];

	menustack[++menustackptr].menufunction = speckparamcontinue1;
	return pullmenuinit (&speckparammenu1, 100, 100);
}




void speckparamcontinue1 (int key)
{
	int left, top;
	char *s;
	char path[MAXPATH];
//	switch (key)
//	{
//	case F2 :
//		fwritespeck(&speck);
//		break;
//	default:
//		break;
//	}
    //CopyLimitsToIntmLim(); //Added by:sdt:21082001//Commented by:sdt:22082001
	left = PMTEXTLEFT;
	top = PMTEXTTOP (topofstack.currentoption)+8+BMYMARGIN;
	s = speckparamlines1[topofstack.currentoption].rhs;
	switch (pullmenucontinue (key))
	{
	case 0 :
		break;

   case FREQ1:
   				//Added by:sdt:17072001
                //These conditions added for the checking valid HFfrequency.
                //Commented by:sdt:21082001//uncommented by:sdt:23082001
    			if(paraminfo[parameternumber].nParamType)
                {
                   	if((!(HFfreqInfo.HFLstart <= 1 && HFfreqInfo.HFLend >= 1))&&(!(HFfreqInfo.HFHstart <= 1 && HFfreqInfo.HFHend >= 1)))
                    {
	                	strcpy(speckparamlines1[0].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }
                /*if(strcmp(speckparamlines1[0].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[0].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[0],"No");
				}
				else
				{
					strcpy(speckparamlines1[0].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[0],"Yes");
				}*/

                specklimitsinit(left, top,0);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ2:
                //Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                   	if((!(HFfreqInfo.HFLstart <= 2 && HFfreqInfo.HFLend >= 2))&&(!(HFfreqInfo.HFHstart <= 2 && HFfreqInfo.HFHend >= 2)))
                    {
	                	strcpy(speckparamlines1[1].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }
               /* if(strcmp(speckparamlines1[1].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[1].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[1],"No");
				}
				else
				{
					strcpy(speckparamlines1[1].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[1],"Yes");
				}*/
                specklimitsinit(left, top,1);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ3:
                //Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 3 && HFfreqInfo.HFLend >= 3))&&(!(HFfreqInfo.HFHstart <= 3 && HFfreqInfo.HFHend >= 3)))
                    {
	                	strcpy(speckparamlines1[2].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }
                /*if(strcmp(speckparamlines1[2].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[2].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[2],"No");
				}
				else
				{
					strcpy(speckparamlines1[2].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[2],"Yes");
				}*/
                specklimitsinit(left, top,2);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ4:
                //Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 4 && HFfreqInfo.HFLend >= 4))&&(!(HFfreqInfo.HFHstart <= 4 && HFfreqInfo.HFHend >= 4)))
                    {
	                	strcpy(speckparamlines1[3].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }
                /*if(strcmp(speckparamlines1[3].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[3].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[3],"No");
				}
				else
				{
					strcpy(speckparamlines1[3].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[3],"Yes");
				}*/
                specklimitsinit(left, top,3);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ5:
    			//Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 5 && HFfreqInfo.HFLend >= 5))&&(!(HFfreqInfo.HFHstart <= 5 && HFfreqInfo.HFHend >= 5)))
                    {
	                	strcpy(speckparamlines1[4].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }

                //Condition modified by:sdt:17072001
                if(!paraminfo[parameternumber].nParamType)
                {
                	strcpy(speckparamlines1[4].rhs,"No");
                    messagewindow(" Not Valid LF frequency");
                    break;
                }

    			/*if(strcmp(speckparamlines1[4].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[4].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[4],"No");
				}
				else
				{
					strcpy(speckparamlines1[4].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[4],"Yes");
				}*/
                specklimitsinit(left, top,4);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ6:
                //Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 6 && HFfreqInfo.HFLend >= 6))&&(!(HFfreqInfo.HFHstart <= 6 && HFfreqInfo.HFHend >= 6)))
                    {
	                	strcpy(speckparamlines1[5].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }

                //Condition modified by:sdt:17072001
                if(!paraminfo[parameternumber].nParamType)
                {
                	strcpy(speckparamlines1[5].rhs,"No");
                    messagewindow(" Not Valid LF frequency");
                    break;
                }
    			/*if(strcmp(speckparamlines1[5].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[5].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[5],"No");
				}
				else
				{
					strcpy(speckparamlines1[5].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[5],"Yes");
				}*/
                specklimitsinit(left, top,5);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ7:
    			//Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)//Commented by:sdt:21082001
                {
                  	if((!(HFfreqInfo.HFLstart <= 7 && HFfreqInfo.HFLend >= 7))&&(!(HFfreqInfo.HFHstart <= 7 && HFfreqInfo.HFHend >= 7)))
                    {
	                	strcpy(speckparamlines1[6].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }

                //Condition modified by:sdt:17072001
                if(!paraminfo[parameternumber].nParamType)
    			{
                	strcpy(speckparamlines1[6].rhs,"No");
                    messagewindow(" Not Valid LF frequency");
                    break;
                }
    			/*if(strcmp(speckparamlines1[6].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[6].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[6],"No");
				}
				else
				{
					strcpy(speckparamlines1[6].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[6],"Yes");
				}*/
                specklimitsinit(left, top,6);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ8:
    			//Added by:sdt:17072001
                //Commented by:sdt:21082001 //Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 8 && HFfreqInfo.HFLend >= 8))&&(!(HFfreqInfo.HFHstart <= 8 && HFfreqInfo.HFHend >= 8)))
                    {
	                	strcpy(speckparamlines1[6].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }

                //Condition modified by:sdt:17072001
                if(!paraminfo[parameternumber].nParamType)
                {
                	strcpy(speckparamlines1[7].rhs,"No");
                    messagewindow(" Not Valid LF frequency");
                    break;
                }
    			/*if(strcmp(speckparamlines1[7].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[7].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[7],"No");
				}
				else
				{
					strcpy(speckparamlines1[7].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[7],"Yes");
				}*/
                specklimitsinit(left, top,7);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ9:
    			//Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 9 && HFfreqInfo.HFLend >= 9))&&(!(HFfreqInfo.HFHstart <= 9 && HFfreqInfo.HFHend >= 9)))
                    {
	                	strcpy(speckparamlines1[8].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }

                //Condition modified by:sdt:17072001
                if(!paraminfo[parameternumber].nParamType)
                {
                	strcpy(speckparamlines1[8].rhs,"No");
                    messagewindow(" Not Valid LF frequency");
                    break;
                }
    			/*if(strcmp(speckparamlines1[8].rhs,"Yes")==0)//Commented by:sdt:21082001
		 		{
					strcpy(speckparamlines1[8].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[8],"No");
				}
				else
				{
					strcpy(speckparamlines1[8].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[8],"Yes");
				}*/
                specklimitsinit(left, top,8);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;
    case FREQ10:
    			//Added by:sdt:17072001
                //Commented by:sdt:21082001//Uncommented by:sdt:23082001
                if(paraminfo[parameternumber].nParamType)
                {
                  	if((!(HFfreqInfo.HFLstart <= 10 && HFfreqInfo.HFLend >= 10))&&(!(HFfreqInfo.HFHstart <= 10 && HFfreqInfo.HFHend >= 10)))
                    {
	                	strcpy(speckparamlines1[9].rhs,"No");
    	                messagewindow(" Not Valid HF frequency");
        	            break;
            	    }
                }

                //Condition modified by:sdt:17072001
                if(!paraminfo[parameternumber].nParamType)
                {
                	strcpy(speckparamlines1[9].rhs,"No");
                    messagewindow(" Not Valid LF frequency");
                    break;
                }
    			/*if(strcmp(speckparamlines1[9].rhs,"Yes")==0)
		 		{
					strcpy(speckparamlines1[9].rhs,"No");
                    strcpy(speck.speckparams[parameternumber].freq[9],"No");
				}
				else
				{
					strcpy(speckparamlines1[9].rhs,"Yes");
                    strcpy(speck.speckparams[parameternumber].freq[9],"Yes");
				}*/
                specklimitsinit(left, top,9);//Added by:sdt:21082001
		 		topofstack.menufunction (F5);
    			break;

	default :
		break;
	}

	return;
}



void speckparamcontinue (int key)
{
	int left, top;
	char *s;
    char path[MAXPATH];
    //Commented:jj:05042001
   /*	if ( strcmpi(speck.type,"YES") == 0 )
		Set_Cups_Data(1);
	else
		Set_Cups_Data(0);*/

	switch (key)
	{
	case PAGEDOWN:
        if (++parameternumber >= MAXPARAMETERS-1)//Modified:jj:05042001 -1 added
			parameternumber--;

		left = topofstack.left;
		top = topofstack.top;
		topofstack.menufunction (ESCAPE);
		speckparaminit(left, top);

		//return;
        break;
	case PAGEUP:

		if (--parameternumber < 0)
			parameternumber++;
		left = topofstack.left;
		top = topofstack.top;
		topofstack.menufunction (ESCAPE);
		speckparaminit (left, top);

        //return;
		break;
	case F2 :
		fwritespeck(&speck);
		break;

    //case ESCAPE :     //Added by:sdt:22082001//Commented by:sdt:22082001
    //	sprintf(path,"%s:\\intmlim.bin",RamDrv);
	//    remove(path);
    //    break;


	default:
		break;
	}
	left = PMTEXTLEFT;
	top = PMTEXTTOP (topofstack.currentoption)+8+BMYMARGIN;
	s = speckparamlines[topofstack.currentoption].rhs;
	switch (pullmenucontinue (key))
	{
	case 0 :
		break;
	case PARAMNAME:
		break;

    case FREQLMTS :
         //Below Condtion Added by:sdt:23082001
         //Because CR,RU & LR are frequency Indepedent Parameters.
         if(parameternumber==SRNO_RU||parameternumber==SRNO_LR||parameternumber==SRNO_CR)
         {
            //specklimitsinit(left, top,0);//Added by:sdt:21082001
            specklimitsinit(left, top,LFFrequency.nDefualtFreq);//modified by:sdt:24082001
         	//messagewindow("This is a Frequency Independent Parameter");
         }
         else
         {
         	SpeckFreqLimitinit(left,top);//Added by:sdt:20082001
	         topofstack.menufunction (F5);//Added by:sdt:22082001
         }
    	 break;

    //Commented by:sdt:20082001
	//case LMTS: specklimitsinit(left,top);//Added:jj:18042001
    //	break;

//Commented:jj:18042001
/*	case INDMIN:
		getspeckrhs( left, top, s,  LIMITSIZE-1,
							"Minimum value of Reading",NULLFUNC);
		break;
	case INDMAX:
		getspeckrhs( left, top, s,
		  LIMITSIZE-1,
							"Maximum value of Reading",NULLFUNC);
		break;
	case AVERAGEMIN:
		getspeckrhs( left, top, s,
		  AVERAGESIZE-1,
							"Minimum Average value of Reading",NULLFUNC);
		break;
	case AVERAGEMAX:
		getspeckrhs( left, top, s,
		  AVERAGESIZE-1,
							"Maximum  Average value of Reading",NULLFUNC);
		break;
	case STDMIN:
		getspeckrhs( left, top, s,
		  STDSIZE-1, "Minimum Standard Deviation ",NULLFUNC);
		break;
	case STDMAX:
		getspeckrhs( left, top, s,
		  STDSIZE-1, "Maximum Standard Deviation",NULLFUNC);
		break;
	case RMSMIN:
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Minimum Root Mean Square Value",NULLFUNC);
		break;
	case RMSMAX:
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Maximum Root Mean Square Value",NULLFUNC);
		break;         */
	case LENGTH_SPECKS:
		getspeckrhs( left, top, s,
		  LENGTHSIZE-4,"Norm. Length in Mts.",NULLFUNC);
		break;
    case TEMP_SPECKS:
		getspeckrhs( left, top, s,
		  LENGTHSIZE-4,"Norm. Temperature in �C.",verifynormtemp);
		break;

/*   	case MEAN_STD_SPECKS:
		getspeckrhs( left, top, s,
		  MEAN_STD_SIZE-4,"Mean-StdDev.",NULLFUNC);
		break;*/

    //Commented:jj:18042001
   	/*case INTRAUNIT_SPECKS:
		getspeckrhs( left, top, s,
		  INTRA_LO_UPSIZE-1,"Intra(Lo)-Intra(Up)",NULLFUNC);
		break;*/

     //case FRQ: speckparaminit1(left,top); //Commented by:sdt:21082001
     //   break;

	default :
		break;
	}
	return;
}


//Added:jj:18042001
void specklimitscontinue (int key)
{

	int left, top;
	char *s;

	switch (key)
	{
	case PAGEDOWN:

    	if(!prmarray.nTestingMethod)//For Pair to Pair Parameters
	  	{
        //Below statement added by:sdt:22082001
        ModifyIntermediateLimits(parameternumber,nCurrFreq,limitarray,specksLimit);
        //if (++limitarray >=4 )//Commented by:sdt:24082001
		//	limitarray--;

		left = topofstack.left;
		top = topofstack.top;
		topofstack.menufunction (ESCAPE);
        //Following Condition Added here by:sdt:23082001
        //Previously limitindex gets increamented by 1 before calling the
		//function "topofstack.menufunction (ESCAPE)".This function calls
        //ModifyIntermediateLimits() function which overwrites the
        //Limits of increamented Limitindex.
        //if (++limitarray >=4 )
        if (++limitarray >=MAXLIMITTYPES) //MOD08072002
			limitarray--;
		specklimitsinit(left, top,nCurrFreq);
        }
		//return;
        break;
	case PAGEUP:

    	if(!prmarray.nTestingMethod)//For Pair to Pair Parameters
	  	{
        //Below statement added by:sdt:22082001
        ModifyIntermediateLimits(parameternumber,nCurrFreq,limitarray,specksLimit);
		//if (--limitarray < 0)//commented by:sdt:24082001
		//	limitarray++;
		left = topofstack.left;
		top = topofstack.top;
        topofstack.menufunction (ESCAPE);
        if (--limitarray < 0)//Added by:sdt:24082001//reason is explained above
			limitarray++;
		specklimitsinit (left, top,nCurrFreq);
        }
        //return;
		break;
    case ESCAPE ://Added by:sdt:21082001
    	ModifyIntermediateLimits(parameternumber,nCurrFreq,limitarray,specksLimit);
        break;
	case F2 :
        //Following function call added by:sdt:23082001
        ModifyIntermediateLimits(parameternumber,nCurrFreq,limitarray,specksLimit);
		fwritespeck(&speck);
		break;
	default:
		break;
	}
	left = PMTEXTLEFT;
	top = PMTEXTTOP (topofstack.currentoption)+8+BMYMARGIN;
	s = specklimitlines[topofstack.currentoption].rhs;
	switch (pullmenucontinue (key))
	{
	case 0 :
		break;

	case LIMITCATEGORY:
    	 break;
    case FREQSTATUS : //Added by:sdt:20082001
         //Below Condition Added by:sdt:23082001
         //CR, RU & LR are the frequency Indepedent Parameters.
         if(!(parameternumber==SRNO_CR||parameternumber==SRNO_LR||parameternumber==SRNO_RU))
         {
        	 if(strcmp(specklimitlines[1].rhs,"Yes")==0)
			 {
				 strcpy(specklimitlines[1].rhs,"No");
            	 strcpy(speck.speckparams[parameternumber].freq[nCurrFreq],"No");
			 }
			 else
			 {
		 		 strcpy(specklimitlines[1].rhs,"Yes");
	    	     strcpy(speck.speckparams[parameternumber].freq[nCurrFreq],"Yes");
			 }
	         topofstack.menufunction (F5);
         }
         break;
	case INDMIN:
		getspeckrhs( left, top, s,  LIMITSIZE-1,
							"Minimum value of Reading",NULLFUNC);
		break;
	case INDMAX:
		getspeckrhs( left, top, s,
		  LIMITSIZE-1,
							"Maximum value of Reading",NULLFUNC);
		break;
	case AVERAGEMIN:
		getspeckrhs( left, top, s,
		  AVERAGESIZE-1,
							"Minimum Average value of Reading",NULLFUNC);
		break;
	case AVERAGEMAX:
		getspeckrhs( left, top, s,
		  AVERAGESIZE-1,
							"Maximum  Average value of Reading",NULLFUNC);
		break;
	case STDMIN:
		getspeckrhs( left, top, s,
		  STDSIZE-1, "Minimum Standard Deviation ",NULLFUNC);
		break;
	case STDMAX:
		getspeckrhs( left, top, s,
		  STDSIZE-1, "Maximum Standard Deviation",NULLFUNC);
		break;
	case RMSMIN:
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Minimum Root Mean Square Value",NULLFUNC);
		break;
	case RMSMAX:
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Maximum Root Mean Square Value",NULLFUNC);
		break;

	//MOD0507002:01:jj
	case PSMIN:
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Minimum PowerSum Value",NULLFUNC);
		break;
	case PSMAX:
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Maximum PowerSum Value",NULLFUNC);
		break;

	//MOD06072002:01:jj
	case WPNMIN:
    	if(limitarray==BOTH_PAIRS) //MOD08072002
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Minimum 1%Worst Pair Next Value",NULLFUNC);
          else //MOD15072002
          	messagewindow(" Enabled only for All combinations catergory !");

		break;
	case WPNMAX:
    	if(limitarray==BOTH_PAIRS) //MOD08072002
		getspeckrhs( left, top, s,
		  RMSSIZE-1,"Maximum 1%Worst Pair Next Value",NULLFUNC);
          else //MOD15072002
          	messagewindow(" Enabled only for All combinations catergory !");

		break;

/*	case LENGTH_SPECKS:
		getspeckrhs( left, top, s,
		  LENGTHSIZE-4,"Norm. Length in Mts.",NULLFUNC);
		break;*/
/*   	case MEAN_STD_SPECKS:
		getspeckrhs( left, top, s,
		  MEAN_STD_SIZE-4,"Mean-StdDev.",NULLFUNC);
		break;*///18042001:1445
	default :
		break;
	}

	return;
}


STRINGMENU speckrhsmenu;
int getspeckrhs (int left, int top, char *string, int maxlen, char *prompt, int (*returnfunction)(char *))
{
	speckrhsmenu.prompt = prompt;
	speckrhsmenu.maxlen = maxlen;
	speckrhsmenu.string = string;
	speckrhsmenu.returnfunction = returnfunction;  //(int (*)(char *))NULL;
	menustack[++menustackptr].menufunction = stringmenucontinue;
	return stringmenuinit (&speckrhsmenu, left, top);
}	// End of getspeckrhs


//Added by:sdt:03052001
void GetCableSequenceTitles(int PageNo)
{
	//int TotalSequences;
    char Seq_title[20];
    //char strtmp[5];
    int Pagesize = CABLESEQMNULENGHTH-1;
    struct ffblk ffblk;
    int FileFound;

    if(PageNo)
    	FileFound=findfirst("*.seq",&ffblk,FA_RDONLY);
    for(int i=1;i<Pagesize*PageNo;i++)
    {
    	FileFound=findnext(&ffblk);
        if(!FileFound)
	    {
			if(!strcmpi(ffblk.ff_name,"GEN.SEQ"))
            	i--;
        }
    	else
        	break;
    }
    //Added by:sdt:04052001
    if(!PageNo)
        FileFound=findfirst("*.seq",&ffblk,FA_RDONLY);
    else
       FileFound=findnext(&ffblk);

    if(!FileFound)
    {
   		strcpy(Sequence_Names[0].FileName,ffblk.ff_name);
    }
    int j=0;
    i=(Pagesize*PageNo);
    do
    {
    	if(!FileFound)
        {
            if(!strcmpi(ffblk.ff_name,"GEN.SEQ"))
            {
            	i--;j--;
            }
            else
            {
	        	strcpy(Seq_title,"SEQUENCE_NAME");
    	        GetPrivateProfileString("SEQ_LIST",Seq_title,"",Sequence_Names[j].SeqName,Sequence_Names[j].FileName);//"cable.seq");
            }
        }
        else
        	break;
        i++;j++;
        FileFound=findnext(&ffblk);
        if(!FileFound)
    		strcpy(Sequence_Names[j].FileName,ffblk.ff_name);
        else
        	break;

    }while(i<Pagesize*(PageNo+1));
    Sequence_Names[j].FileName[0]=NULL;
    Sequence_Names[j].SeqName[0]=NULL;
}


//Added by:sdt:20042001
void GetCableCategaryList(char *SeqTitle)
{
	char *StrCategary;
    int nNoOfCategaries;
    char *szpToken;
    char szdpTagTitles[2][30];//Added :sdt:17042001

    strcpy(CableSequence.szSequence_Name,SeqTitle);
    sprintf(szdpTagTitles[0],"CABLESEQ_%s",CableSequence.szSequence_Name);//Added :sdt:17042001
    //strcpy(szdpTagTitles[1],NULL);
    //MOD18012002:02:sdt::1155
    szdpTagTitles[1][0]='\0';
    StrCategary=NewGetPrivateProfileString(szdpTagTitles,"CATEGARY"," ",StrCategary,szSeqFileName,0);//"cable.seq",0);
	nNoOfCategaries=CharOccurInString(StrCategary,',');

 	CableSequence.NumberOfPairs = (int *)farmalloc(sizeof(int)*(nNoOfCategaries+2));
     szpToken=strtok(StrCategary,",");
    for(int i=0;i<nNoOfCategaries+1&&(szpToken!=NULL);i++)
    {
        CableSequence.NumberOfPairs[i]=atoi(szpToken);
        szpToken=strtok(NULL,",");
	}
	CableSequence.NumberOfPairs[i]=-1;
}


void GetCableUnitsList(int Categary)
{
    char szdpTagTitles[3][30],*szpToken;
    char *szpUnits;//modified
    int nTotalUnitTypes;
    int count=0;

    for(int i=0;CableSequence.NumberOfPairs[i]!=-1;i++)
    {
       if(CableSequence.NumberOfPairs[i]==Categary)
        break;
	}
    CableSequence.TotalPairs.nIndexPairsCategary =i;

    sprintf(szdpTagTitles[count],"CABLESEQ_%s",CableSequence.szSequence_Name);//Added :sdt:17042001
    sprintf(szdpTagTitles[++count],"%d_PAIRS",Categary);
    //strcpy(szdpTagTitles[++count],NULL);
    //MOD18012002:02:sdt::1155
    szdpTagTitles[++count][0]='\0';

	szpUnits=NewGetPrivateProfileString(szdpTagTitles,"PAIRSIN_UNIT_CATEGARY"," ",szpUnits,szSeqFileName,0);//"cable.seq",0);
    nTotalUnitTypes=CharOccurInString(szpUnits,',');
    CableSequence.TotalPairs.NumberOfUnits = (int *)farmalloc(sizeof(int)*(nTotalUnitTypes+2));
    szpToken=strtok(szpUnits,",");
    for(i=0;i<nTotalUnitTypes+1&&(szpToken!=NULL);i++)
    {
        CableSequence.TotalPairs.NumberOfUnits[i]=atoi(szpToken);
        szpToken=strtok(NULL,",");
	}
	CableSequence.TotalPairs.NumberOfUnits[i]=-1;
}


void GetCableStructure(int Unit)
{
	char szdpTagTitles[4][30], *SeqFile;
    int count=0;
    FILE *pFile;

    sprintf(szdpTagTitles[count],"CABLESEQ_%s",CableSequence.szSequence_Name);//Added :sdt:17042001
    sprintf(szdpTagTitles[++count],"%d_PAIRS",CableSequence.NumberOfPairs[CableSequence.TotalPairs.nIndexPairsCategary]);
    sprintf(szdpTagTitles[++count],"%d_PAIRSINUNIT",Unit);
    //strcpy(szdpTagTitles[++count],NULL);
    //MOD18012002:02:sdt:1205
    szdpTagTitles[++count][0]='\0';

    SeqFile=szSeqFileName;//"cable.seq";
   	pFile=ReturnFilePtr(szdpTagTitles,"UNIT_TO_UNIT_ADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.Adj_Units=ReadPairsFromFile(pFile);

    pFile=ReturnFilePtr(szdpTagTitles,"UNIT_TO_UNIT_NONADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.NonAdj_Units=ReadPairsFromFile(pFile);

    pFile=ReturnFilePtr(szdpTagTitles,"PAIR_TO_PAIR_ADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.Adj_Pairs=ReadPairsFromFile(pFile);

    pFile=ReturnFilePtr(szdpTagTitles,"PAIR_TO_PAIR_NONADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.NonAdj_Pairs=ReadPairsFromFile(pFile);
}


void FreeSequenceStructure(int bFlag)
{
    SEQPAIR *pTemp,*pCurr;
    if((bFlag==ALLPAIRS)||(bFlag==PAIRSINUNIT)||(bFlag==CBLSTRUCT))
    {
    	pCurr=CableSequence.TotalPairs.Units.Adj_Units;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.Adj_Units=NULL;
        pCurr=CableSequence.TotalPairs.Units.NonAdj_Units;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.NonAdj_Units=NULL;
        pCurr=CableSequence.TotalPairs.Units.Adj_Pairs;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.Adj_Pairs=NULL;
        pCurr=CableSequence.TotalPairs.Units.NonAdj_Pairs;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.NonAdj_Pairs=NULL;
    }


    if((bFlag==ALLPAIRS)||(bFlag==PAIRSINUNIT))
    farfree(CableSequence.TotalPairs.NumberOfUnits);

    if(bFlag==ALLPAIRS)
    farfree(CableSequence.NumberOfPairs);
}

int CableSequenceInit(int left,int top)
{
    menustack[++menustackptr].menufunction = CableSeqContinue;
    return pullmenuinit (&cableseqmenu, left, top);

}

void CableSeqContinue(int key)
{
	int left, top;
	char *s;

	left = PMTEXTLEFT;
	top = PMTEXTTOP (topofstack.currentoption)+8+BMYMARGIN;
	s = CableSeqLines[topofstack.currentoption].rhs;
	pullmenucontinue (key);

	  if(key==ESCAPE)
      {
      }

      if(key==ENTER)
	  {
            //Now it is diplaying current selected Cable sequence in the
            //rhs of "Cable Sequence" Option in the speck menu
      		strcpy(speck.szCableSequence.SeqName,PULLMENUPTR->lines[topofstack.currentoption].lhs);
            strcpy(speck.szCableSequence.FileName,Sequence_Names[topofstack.currentoption].FileName);
            topofstack.menufunction (ESCAPE);
            //commented by:sdt:03052001
            //int i=0;
            //while(strcmp(specklines[i++].lhs,"Cable Sequence"));
            //strcpy(specklines[i-1].rhs,speck.szCableSequence);
            //specklines[i-1].rhslength=strlen(speck.szCableSequence);
            topofstack.menufunction (F5);
       }

       if(key==PAGEDOWN)
       {
       	    int nTotalSequence;
            topofstack.menufunction (ESCAPE);
            //commented by:sdt:03052001
		    //nTotalSequence = GetPrivateProfileInt("SEQ_LIST","TOTAL_NUMBER_OF_SEQUENCES",0,"cable.seq");
            nTotalSequence=CalculateFileNumber("*.seq");
            nTotalSequence--;
            if(nTotalSequence>(CABLESEQMNULENGHTH-1)*(nPageCount+1))
            {
                GetCableSequenceTitles(++nPageCount);
                for(int i=((CABLESEQMNULENGHTH-1)*nPageCount),j=0;i<((CABLESEQMNULENGHTH-1)*(nPageCount+1))&&i<nTotalSequence;i++,j++)
                {
                	strcpy(CableSeqLines[j].lhs,Sequence_Names[j].SeqName);
				    CableSeqLines[j].rhs=NULL;
				 	CableSeqLines[j].rhslength=0;
				  	CableSeqLines[j].returnvalue=i;//ENTER
			   	}
                for(;j<CABLESEQMNULENGHTH;j++)
                {
			   	 	CableSeqLines[j].lhs[0]=NULL;
				    CableSeqLines[j].rhs=NULL;
				  	CableSeqLines[j].rhslength=0;
				 	CableSeqLines[j].returnvalue=-1;
                }
            }
            CableSequenceInit(94,133);//Modified by:sdt:22052001
            //CableSequenceInit(left,top);//Added by:sdt:22052001
       }
       if(key==PAGEUP)
       {
            topofstack.menufunction (ESCAPE);
            if(nPageCount-1>=0)
            {
                GetCableSequenceTitles(--nPageCount);
                for(int i=((CABLESEQMNULENGHTH-1)*nPageCount),j=0;i<((CABLESEQMNULENGHTH-1)*(nPageCount+1));i++,j++)
                {
                	strcpy(CableSeqLines[j].lhs,Sequence_Names[j].SeqName);
				    CableSeqLines[j].rhs=NULL;
				 	CableSeqLines[j].rhslength=0;
				  	CableSeqLines[j].returnvalue=i;//ENTER
			   	}
				for(;j<CABLESEQMNULENGHTH;j++)
                {
			    	CableSeqLines[j].lhs[0]=NULL;
				    CableSeqLines[j].rhs=NULL;
				  	CableSeqLines[j].rhslength=0;
				 	CableSeqLines[j].returnvalue=-1;
                }
            }
            CableSequenceInit(94,133);//Modified by:sdt:22052001
            //CableSequenceInit(left,top);//Added by:sdt:22052001
       }
}


int CharOccurInString(char *String,char ch)
{
    int nCharCount=0;
    if(String[0]=='\0')
    	return -1;
	for(int i=0;String[i]!='\0';i++)
    {
		if(String[i]==ch)
        	nCharCount++;
    }
    return nCharCount;
}

char *StringToken(char *s1,char ch)
{
	char *Token="";
    for(int i=0;*s1!='\0'&&*s1!=ch;i++,s1++)
    {
		Token[i]=*s1;
    }
    Token[i]='\0';
    s1++;
    return Token;
}

SEQPAIR *ReadPairsFromFile(FILE *fpPtr)
{
    int i;
    char cChar,szString[10],SubString[10];
    SEQPAIR *pNewPair,*pCurrPair,*Pairs;
    cChar=fgetc(fpPtr);
    fseek(fpPtr,-(long(sizeof(cChar))),SEEK_CUR);
    Pairs = pCurrPair=NULL;
    while(cChar!='#'&&cChar!=EOF)
    {
        i=-1;
        do
        {
            szString[++i]=fgetc(fpPtr);
            if(szString[i]==' '||szString[i]=='\n'||szString[i]=='\t')
            	i--;

        }while(szString[i]!='#'&&szString[i]!=EOF&&szString[i]!=',');
        cChar=szString[i];
		pNewPair=(SEQPAIR *)malloc(sizeof(SEQPAIR));
        pNewPair->link=NULL;

        for(int j=0;szString[j]!='-'&&szString[j]!='#';j++)
        SubString[j]=szString[j];
        SubString[j]='\0';
        pNewPair->Pairno1=atoi(SubString);
        j++;
        for(int k=0;szString[j]!=','&&szString[j]!='#';j++,k++)
        SubString[k]=szString[j];
        SubString[k]='\0';
        pNewPair->Pairno2=atoi(SubString);

        if(Pairs==NULL)
        {
        	Pairs=pNewPair;
            pCurrPair = pNewPair;
        }
        else
        {
        	pCurrPair->link=pNewPair;
            pCurrPair=pCurrPair->link;
        }

    }
    if(fpPtr){fclose(fpPtr);}
    return Pairs;
}
//Function Added by:sdt:03052001
int CalculateFileNumber(char *FileName)
{
    int FileCount=0;
    int FileFound;
    struct ffblk ffblk;

	FileFound=findfirst(FileName,&ffblk,FA_RDONLY);

    while(!FileFound)
    {
    	FileCount++;
        FileFound=findnext(&ffblk);
    }
    return FileCount;
}

//This function is added by:sdt:06072001
//Purpose :- This function used to check the duplicate Cable sequence
// 			 names present in the Cable Sequence files and also check
//			 NULL Cable Sequence Names.
//			 This will return File & cable sequence names which are
//			 duplicate.
SEQNAMES * CheckforDuplicateCableSequence(void)
{
    int nTotSeqFiles,FileFound;
    SEQNAMES *ptrSeqNames;
    SEQNAMES *ptrReturnSeqNames;
    struct ffblk ffblk;

    nTotSeqFiles=CalculateFileNumber("*.seq");
    ptrSeqNames=(SEQNAMES *)farmalloc(sizeof(SEQNAMES)*nTotSeqFiles);
    ptrReturnSeqNames=(SEQNAMES *)farmalloc(sizeof(SEQNAMES)*2);
    memset(ptrSeqNames,'\0',sizeof(SEQNAMES)*nTotSeqFiles);
    memset(ptrReturnSeqNames,'\0',sizeof(SEQNAMES)*2);
    FileFound=findfirst("*.seq",&ffblk,FA_RDONLY);

    if(!FileFound)
    {
   		strcpy(ptrSeqNames[0].FileName,ffblk.ff_name);
    }

    for(int i=0;i<nTotSeqFiles;i++)
    {
		if(!FileFound)
        {
            if(!strcmpi(ffblk.ff_name,"GEN.SEQ"))
            {
            	i--;
            }
            else
            {
				GetPrivateProfileString("SEQ_LIST","SEQUENCE_NAME","",ptrSeqNames[i].SeqName,ptrSeqNames[i].FileName);//"cable.seq");
            }
        }
        else
        	break;

        FileFound=findnext(&ffblk);
        if(!FileFound)
    		strcpy(ptrSeqNames[i+1].FileName,ffblk.ff_name);
        else
        	break;
    }

    for(i=0;i<nTotSeqFiles;i++)
    {
        if((!strcmp(ptrSeqNames[i].SeqName,""))&&(strcmp(ptrSeqNames[i].FileName,""))&&(strcmpi(ptrSeqNames[i].FileName,"GEN.SEQ")))
        {
            strcpy(ptrReturnSeqNames[0].SeqName,ptrSeqNames[i].SeqName);
            strcpy(ptrReturnSeqNames[0].FileName,ptrSeqNames[i].FileName);
            farfree(ptrSeqNames);
			return ptrReturnSeqNames;
        }

    	for(int j=i+1;j<nTotSeqFiles;j++)
        {
			/*if(!strcmp(ptrSeqNames[i].SeqName,"")&&(strcmp(ptrSeqNames[i].FileName,"")))
            {
                strcpy(ptrReturnSeqNames[0].SeqName,ptrSeqNames[i].SeqName);
                strcpy(ptrReturnSeqNames[0].FileName,ptrSeqNames[i].FileName);
                farfree(ptrSeqNames);
				return ptrReturnSeqNames;
            }*/

            if(!strcmp(ptrSeqNames[j].SeqName,"")&&(strcmp(ptrSeqNames[j].FileName,""))&&(strcmpi(ptrSeqNames[j].FileName,"GEN.SEQ")))
            {
                strcpy(ptrReturnSeqNames[0].SeqName,ptrSeqNames[j].SeqName);
                strcpy(ptrReturnSeqNames[0].FileName,ptrSeqNames[j].FileName);
                farfree(ptrSeqNames);
				return ptrReturnSeqNames;
            }

            if(!strcmp(ptrSeqNames[i].SeqName,ptrSeqNames[j].SeqName))
            {
            	strcpy(ptrReturnSeqNames[0].SeqName,ptrSeqNames[i].SeqName);
                strcpy(ptrReturnSeqNames[0].FileName,ptrSeqNames[i].FileName);
                strcpy(ptrReturnSeqNames[1].SeqName,ptrSeqNames[j].SeqName);
                strcpy(ptrReturnSeqNames[1].FileName,ptrSeqNames[j].FileName);
                farfree(ptrSeqNames);
                return ptrReturnSeqNames;
            }

        }
    }
    farfree(ptrSeqNames);
    return ptrReturnSeqNames;
}

int verifynormtemp(char *string)
{
	float normTemp;
    normTemp = atoi(string);
    if(normTemp>= 0 &&normTemp <=100)
       	return TRUE;
    else
    {
        //string = "";
        strcpy(string,"");
        //strcpy(speckparamlines[3].rhs,"");
        messagewindow("Please Enter the Temparature Between 0� to 100�C");
        return FALSE;
    }
}

//Function Added by:sdt:17072001
//This function will read every specification. If it has some
//Hf frequency out of HF Low & HF High frequency Limits.
void CheckValidHFFreqInSpecks()
{
	 int nSpecks=0;
	 //char szFileName[20];
     int FileFound;
     ffblk ffblk;
	 FILE *Chk;
     SPECIFICATION TempSpeck;

	 nSpeckFileNo=1;

     FileFound=findfirst("*.siv",&ffblk,FA_RDONLY);


     while(!FileFound)
	 {
		Chk=fopen(ffblk.ff_name,"rb");
		if(Chk!=NULL)
		{
			 if(Chk){fclose(Chk);}
             //Added by:sdt:24082001
             //IF nSpeckFileNo variable not initialised every time, it will
             //take value as 1 (as initialised Above) and will open
             // 1.Siv every time.
             nSpeckFileNo=atoi(strtok(ffblk.ff_name,"."));
			 freadspeck(&TempSpeck);
             for(int i=0;i<MAXPARAMETERS-1;i++)
             {
                if(paraminfo[i].nParamType)
                {
             		for(int j=1;j<=MAXFREQUENCIES;j++)
                	{
                       	if((!(HFfreqInfo.HFLstart <= j && HFfreqInfo.HFLend >= j))&&(!(HFfreqInfo.HFHstart <= j && HFfreqInfo.HFHend >= j)))
                        {
                			strcpy(TempSpeck.speckparams[i].freq[j-1],"No");
                        }
                	}
                }
             }
        	fwritespeck(&TempSpeck);
        }
        FileFound=findnext(&ffblk);
     }
}

//Added:jj:20082001
int SpeckFreqLimitinit (int left, int top)
{
    char string[80];

	sprintf( string,"PgUp,PgDn: New  F2: Save");
	messagewindow( string);
     //Added temperarily by:sdt:21082001//Commented by:sdt:23082001
     //start
    /* sprintf(string,"%s:\\cts.ini",RamDrv);
	{
    char list[10];
    for (int f=0;f<10;f++)
    {
        sprintf(list,"FREQ_%d",f+1);
		GetPrivateProfileString ("FREQLIST",list,"",SpeckList[f],string);
        for( int j=1; j<=5; j++)
        	SpeckList[f][20+j] = '\0';
        SetSpeckList(f+1,SpeckList[f]);

    }
    } */
    //End

    stage=1;

    //Added:jj:17032001
     for(int i=0;i<MAXFREQUENCIES;i++)
     	speckparamlines1[i].rhs = speck.speckparams[parameternumber].freq[i];

	menustack[++menustackptr].menufunction = speckparamcontinue1;
	return pullmenuinit (&speckparammenu1, 100, 100);
}

