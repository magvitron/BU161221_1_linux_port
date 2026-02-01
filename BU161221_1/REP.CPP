
//=================================================================
//				Modifications
//	Date :29062001
//	1. LineFeed function modified, for detecting end of page.
//	2. Initializeprinter() function modified to set printers
//	   Page length.
//	3. PrintSpeck() & PrintFormulae() function modified, to
//     print the report in Draft Condensed form.
//=================================================================
#include "common.h"
#include <math.h>
#include <string.h>
#include "rep.h"
#include "utils.h"
#include <ctime>
//#include "common.h"
#define MAXBYPASSPAIRLEN  25

SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex,int bCreateSpeckFlag);//MOD11072002:02:jj
int newpage;//added:sdt:11092001:0935
extern char SpeckList[MAXFREQUENCIES][10];//26032001

extern char cRamDrive[2]; //22012001
//#ifdef EXECUTE //Added by:sdt:21082001

//Added:sdt:28062001
extern PARAM_DETAILS Param;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//Modified by:sdt:31072001



//  File pointers used in this code file
extern FILE *fptr_Report,*RepView;

//  Variables used in this code file declared below
int  nLineNo,nPrintMod,nColNo,nPageNo,nPrevStrLen;

char        cRepChar;
char        szGlobStr[120],szRFName[50];

//extern      struct time t;
extern      struct date d;
//static int  RepFileNo=0;

//    Structures used in this code file
extern REPINIINFO       RepIni;

//    External variables used in this code file
extern      int  nReport;
extern      int  nSpeckFileNo,nDC;
extern      int  flagFileLoaded;
extern      int  nLastUnitSize,nUnits,nNeedOfAppend;
extern      int  nTempUnitOf;
extern      char szBypassPairs[MAXBYPASSPAIRLEN];

void EndReport ()
{
//	char szTemp[80];
    if(nDC==SCREEN)
	  if(RepView){fclose(RepView);}
    if (nDC==PRINTER)
    {
        if(fptr_Report){fclose(fptr_Report);}
//        sprintf(szTemp, "print %s > message",szRFName);
//        system (szTemp);
    }
    SelectShrink(FALSE);
    SelectNlq(FALSE);
    SelectCondensed(TRUE);
    nPrintMod=CONDENSEDMODE;
    FormFeed();
    nReport=FALSE;
}

void SetPageLength ()
{
   if(nDC!=PRINTER)
	  return;
   cRepChar = ESC;
   fputc (cRepChar,fptr_Report);
   cRepChar = 67;
   fputc (cRepChar,fptr_Report);
   cRepChar = (char)RepIni.nPaperInches*6;
   fputc (cRepChar,fptr_Report);
}

int SendtoPrint (char cCh)
{
    int nRet=TRUE;

    if(cCh=='\t')
    {
      cCh=' ';
	  if(nDC==PRINTER)
	      fputc (cCh,fptr_Report);
	  else
		 fputc(cCh,RepView);
    }
    if(nDC==PRINTER)
		fputc (cCh,fptr_Report);
   else
	  fputc(cCh,RepView);
   return nRet;
}

void SelectNlq (char OnOff)
{
  if(nDC!=PRINTER)
	 return;
  cRepChar = ESC;
  fputc (cRepChar,fptr_Report);
  cRepChar = 120;
  fputc (cRepChar,fptr_Report);
  cRepChar = OnOff;
  fputc (cRepChar,fptr_Report);
}

void SelectCondensed (char OnOff)
{
  if(nDC!=PRINTER)
	 return;

	if(OnOff)
	{
		cRepChar = ESC;
		fputc (cRepChar,fptr_Report);
		cRepChar = 15;
		fputc (cRepChar,fptr_Report);
		nPrintMod=CONDENSEDMODE;
	}
	else
	{
		cRepChar = 18;
		fputc (cRepChar,fptr_Report);
	}
}

void SelectDoublewidth (char OnOff)
{
  if(nDC!=PRINTER)
	 return;
  cRepChar = ESC;
  fputc (cRepChar,fptr_Report);
  cRepChar = 87;
  fputc (cRepChar,fptr_Report);
  cRepChar = OnOff;
  fputc (cRepChar,fptr_Report);
  nPrintMod/=2;
}

void SelectSuperscript (char OnOff)
{
  if(nDC!=PRINTER)
	 return;
  if(OnOff)
  {
		cRepChar = ESC;
		fputc (cRepChar,fptr_Report);
		cRepChar = 83;
		fputc (cRepChar,fptr_Report);
		cRepChar = 0;
		fputc (cRepChar,fptr_Report);
  }
  else
  {
		cRepChar = ESC;
		fputc (cRepChar,fptr_Report);
		cRepChar = 84;
		fputc (cRepChar,fptr_Report);
  }
}

void SelectUnderline (char OnOff)
{
  if(nDC!=PRINTER)
	 return;

  cRepChar = ESC;
  fputc (cRepChar,fptr_Report);
  cRepChar = 45;
  fputc (cRepChar,fptr_Report);
  cRepChar = OnOff;
  fputc (cRepChar,fptr_Report);
}

void SelectShrink (char OnOff)
{
  if(nDC!=PRINTER)
	 return;

  if(OnOff)
  {
	  cRepChar = ESC;
	  fputc (cRepChar,fptr_Report);
	  cRepChar = 103;
	  fputc (cRepChar,fptr_Report);
	  nPrintMod=SHRINKMODE;
  }
  else
  {
	  cRepChar = ESC;
	  fputc (cRepChar,fptr_Report);
	  cRepChar = 80;
	  fputc (cRepChar,fptr_Report);
  }
}

int WriteString (char *szStr,int nPosition,int nPrintMod,int nLineFeed)
{
  static int nHeadPos=0;
	char szLine[MAXPRINTCOLS];
	int nStrLen,i;

  if(!nLineFeed)
  {
	 if(nColNo==1)
		nPrevStrLen=strlen(szStr);
	 else
		nPrevStrLen+=strlen(szStr);
  }
	szLine[0]=NULL;
	if(strlen(szStr)>nPrintMod)
		return FALSE;
	nPrintMod/=2;
	if(nPosition==POS_CENTER)
	{
		nStrLen=strlen(szStr);
		nStrLen/=2;
		for(i=0;i<(nPrintMod-nStrLen);i++)
			strcat(szLine," ");
		strcat(szLine,szStr);
		for(i=0;szLine[i]!=NULL;i++)
			SendtoPrint(szLine[i]);
	}
	if(nPosition==POS_LEFT)
	{
		for(i=0;szStr[i]!=NULL;i++)
		{
			SendtoPrint(szStr[i]);
			nHeadPos++;
		}
	}
	if(nPosition==POS_RIGHT)
	{
		nHeadPos=61-nHeadPos;
		for(i=0;i<nHeadPos;i++)
			strcat(szLine," ");
		strcat(szLine,szStr);
		for(i=0;szLine[i]!=NULL;i++)
			SendtoPrint(szLine[i]);
	}
	if(nLineFeed)
	{
		LineFeed();
		nHeadPos=0;
	}
	return TRUE;
}

void RepeatChar (char cCh,int nPrintMod)
{
	int i;

  for(i=0;i<nPrintMod;i++)
  {
   if(nDC==PRINTER)
	    SendtoPrint(cCh);
   else
      fputc((int)cCh,RepView);
  }
  LineFeed();
}

void LineFeed (void)
{
  newpage=0;//Added by:sdt:11092001:0955
  if(nDC==PRINTER)
  {
	   SendtoPrint(13);
	   SendtoPrint(10);
  }
  else
     fputc('\n',RepView);
	nLineNo++;
    //condition Added by:sdt:29062001:Check for the End of Page.
    if((nLineNo >=((RepIni.nPaperInches*6)-5))&&(nDC==PRINTER))
    {
    	FormFeed();
        SkipTopMargin();
        newpage=1;//Added by:sdt:11092001:0955
    }
    //Condition added by:sdt:11092001:0955
    //This condition is used to detect the end of page of the screen.
    if((nLineNo > 23 )&&(nDC==SCREEN))
    {
	   nLineNo=1;
	   nColNo=1;
       newpage=1;
    }
}

void FormFeed (void)
{
   int i;

   for(i=nLineNo;i<(RepIni.nPaperInches*6)-5;i++)
   {
   	   SendtoPrint(13);
	   SendtoPrint(10);
   }

   if(nDC==PRINTER)
	  SendtoPrint(12);
   nLineNo=1;
   nColNo=1;
   nPageNo++;
   return;
}

void SkipTopMargin ()
{
	 int i;
	 for(i=0;i<RepIni.nTopMargin;i++)
			LineFeed();
}
void SelectElite()
{
	if(nDC!=PRINTER)//Added:jj:15032001
	 return;

	cRepChar = ESC;
	fputc (27 ,fptr_Report);
	cRepChar = 77;
	fputc (77 ,fptr_Report);
}



//==========================================================
// Now the formulae are read from the cts.ini & printed.
// Hard coding used for printing thr formulae is removed.
//==========================================================
//Commented by:sdt:10112005:1145
//Uncommented by:sdt:29062006:2310
void PrintFormulae(void) //Added by:sdt:28062001
{
	 char szStr[150],cCh;
	 FILE *fpTmp;
     char *Ptr; //Added by:sdt:28062001
     char ParamFormulae[200];
     char path[80];

	 nLineNo=1;
 	 nColNo=1;
	 nPageNo=1;
	 InitialisePrinter();
	 SelectNlq(FALSE);//TRUE);//Modified by:sdt:28062001
	 SelectShrink(TRUE);
     SelectElite(); //Added by:sdt:28062001
     SelectCondensed(TRUE); //Added by:sdt:29062001
     nPrintMod=MAXPRINTCOLS-19;
	 SkipTopMargin();
	 if(nDC==PRINTER)
	 {
         fpTmp=fopen("title.inf","r");
         if(fpTmp!=NULL)
		 {
		    while(!feof(fpTmp))
            {
              	cCh=fgetc(fpTmp);
                fputc(cCh,fptr_Report);
            }
		 }
         if(fpTmp){fclose(fpTmp);}
         nLineNo+=4;//Lines used for Title.
     }
	time_t now = time(nullptr);
	tm *t = localtime(&now);
//	struct time t;
//	gettime(&t);
//	gettime(&t);
	getdate(&d);
	sprintf(szStr,"List Of Formulae used\n");
    WriteString(szStr,POS_CENTER,nPrintMod);
	printf("%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
//	sprintf(szStr,"Time.   %2d:%02d:%02d",t.ti_hour,t.ti_min,t.ti_sec);
	WriteString(szStr,POS_CENTER,nPrintMod);
	sprintf(szStr,"Date.   %d/%d/%d\n",d.da_day,d.da_mon,d.da_year);
    WriteString(szStr,POS_CENTER,nPrintMod);

    RepeatChar('-',nPrintMod);

    for(int nParamno =Param.Param_Start; nParamno<=Param.Param_End;nParamno++)
    {
        Ptr = ParamFormulae;
		for(int i=1;i<=4;i++)
		{
			sprintf(szStr,"%s_FOR%d", paraminfo[nParamno].szParamAbr,i);
			sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
			GetPrivateProfileString("FORMULAS",szStr,Ptr+((i-1)*50),"",path);
		}
        sprintf(szStr,"[%d] Formule used to calculate ::  %s",nParamno+1,paraminfo[nParamno].szParamAbr);
		WriteString(szStr,POS_LEFT,nPrintMod);
        for(int i=0;i<4;i++)
		{
			strcpy(szStr,Ptr);
			WriteString(szStr,POS_LEFT,nPrintMod);
			Ptr+=50;
        }
    }
	sprintf(szStr,"       Legends :");
	WriteString(szStr,POS_LEFT,nPrintMod);
	sprintf(szStr,"       	NL - Normalisation Length");
	WriteString(szStr,POS_LEFT,nPrintMod);
	sprintf(szStr,"       	CL - Actual Cable Length");
	WriteString(szStr,POS_LEFT,nPrintMod);

	RepeatChar('-',nPrintMod);
     FormFeed();//Added by:sdt:29062001
     SelectNlq(TRUE); //Added by:sdt:29062001
	 SelectShrink(FALSE); //Added by:sdt:29062001
     SelectCondensed(FALSE); //Added by:sdt:29062001

}

void ShiftString (char szTmp[],int nTmp)
{
	int nStrlen,i;

	nStrlen=strlen(szTmp);
	szGlobStr[0]=NULL;
	for(i=nStrlen;i<nTmp;i++)
		strcat(szGlobStr," ");
	strcat(szGlobStr,szTmp);
}

int InitialisePrinter ()
{
  if(nDC!=PRINTER)
	 return TRUE;

/*  if(nDC!=PRINTER)
	 return TRUE;
  if (RepFileNo >= MAXFILEINQUEUE)
  {
	 messagewindow ("Files in queue exceed the capacity !!!");
	 getch ();
  }
	sprintf (szRFName,"g:\\Rep%d.prn",RepFileNo++);
  if ((fptr_Report = fopen (szRFName,"wb"))==NULL)
  {
	 messagewindow ("Drive Not Available");
	 getch ();
	 return -1;
  }  */
  SetPageLength ();//Uncommented by:sdt:29062001
  return 0;
}

//#endif //Added by:sdt:21082001
