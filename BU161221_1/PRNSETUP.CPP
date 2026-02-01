#include <math.h>
#include <string.h>
#include "rep.h"

extern FILE *fptr_Report,*RepView;
extern REPINIINFO       RepIni;
extern      int  nDC;
char        cRepChar;

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
