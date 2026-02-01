#include "stdio.h"

#include "uses.h"
#include "frontpg.h"
#include "ct.h"
#include "utils.h"
#include "funproto.h"
#include "share.h"
//#include "data.h"

//int whichConductor = 0;
//int whichStep = 0;
//extern float CurrRfVal;
//extern int nParamCount;
extern int nIntNo,nDecNo;
extern int nCurrentReadingno;
extern char SpeckList[MAXFREQUENCIES][10];
extern CTPAIR         CtPair;
extern CONFIG nConfig;
extern PRM			*prmarray;
extern DispStruct dispstruct[MAXPARAMETERS];

TFrontPage::TFrontPage(char *aTitle):
			TWindow(TRect(0,0,50,18),aTitle,wnNoNumber),
			TWindowInit(&TFrontPage::initFrame)
{
  TView::state &= ~sfShadow;
  TView::options = (ofCenterX | ofCenterY);

  flags &= ~(wfClose|wfGrow|wfZoom);
  palette = wpGrayWindow;
  TRect viewR = getExtent();
  viewR.a.x += 1;
  viewR.a.y += 1;
  viewR.b.x -= 1;
  viewR.b.y -= 1;
  tp = new TTestProgress(viewR);
  insert(tp);
  updateFrontPage(RESET_STATUS, UPDATE_DEFAULT, 0);
}

TFrontPage::~TFrontPage()
 {

 }

void TFrontPage::hint( ushort hintNo )
{
	ushort helpCtx = TProgram::deskTop->helpCtx;
	TProgram::deskTop->helpCtx = hintNo;
 	TProgram::statusLine->update();
	TProgram::deskTop->helpCtx = helpCtx;
}

void TFrontPage::updateFrontPage( BYTE status, BYTE updateType, int nFreqNum )
{
	tp->updateType = updateType;
	tp->status = status;
	tp->nFreqNum = nFreqNum;
	message( (TTestProgress*)tp, evBroadcast, cmDraw, 0 );
}


void TFrontPage::draw()
{
	TWindow::draw();
}

TTestProgress::TTestProgress( TRect& r ):
				TView( r )
{
	/*int x = 8,y= 1;
	for(int i=0;i<MAX_CONDUCTORS;i++)
	 {
	  conductor_status[i].conductor_x = x;
	  conductor_status[i].conductor_y = y;
	  conductor_status[i].status = NOTTESTED;
	  x = x + 3;
	  if (x >= 20)
	   {
	  x = 8;
	  y = y + 2;
	   }
	}*/
}

TTestProgress::~TTestProgress()
 {
	 //memset(&conductor_status,0,sizeof(CONDUCTOR_STATUS));
 }

void TTestProgress::handleEvent( TEvent& ev )
{
	TView::handleEvent( ev );
	if ( ev.what == evBroadcast )
		if ( ev.message.command == cmDraw )
		{
/*			if ( updateType == UPDATE_RESULT )
				infoPtr = (char*)ev.message.infoPtr;*/
			drawView();
		}
}

/*void TTestProgress::showConductorNo(int no,int step)
 {
  char cond_number[30];
  char rfvalline[15];
  sprintf(cond_number,"Conductor No: %2d   Step:-%2d",no,step);
  writeStr(11,13,(cond_number),1);
  sprintf(rfvalline,"Current RF : %0.2f",CurrRfVal);
  writeStr(16,15,(rfvalline),2);
 }*/

void TTestProgress::showCurrentReading()
{
	char str[60], str1[10] ;
	float	fRu_Reading;
	float fRa,fRb;
	static float fRaOriginal,fRbOriginal;

	sprintf(str,"Test Frequency :: %s",SpeckList[nFreqNum]);
	writeStr(2,2,(str),6);
	//sprintf(str,"Unit Under Test ::%d",CtPair.UnitSrNo);
	if(!prmarray->nTestingMethod)//Added:jj:17042001
		sprintf( str, "Unit Under Test :: %d-%d",((CtPair.Pair1-1)/atoi(nConfig.szUnitof))+1,((CtPair.Pair2-1)/atoi(nConfig.szUnitof))+1);
	else
	{
		sprintf( str, "Unit Under Test :: %d   ",CtPair.UnitSrNo);
	}
	writeStr(2,3,(str),6);
	sprintf(str,"Pair Under Test ::%d",CtPair.Pair1);
	if(CtPair.nTest==SRNO_CR)
	{
		if( CtPair.TipOrRing == TIP)
		{
			strcat( str, "(T)");
			fRaOriginal=CtPair.norval;
		}
		else
		{
			strcat( str, "(R)");
			fRbOriginal=CtPair.norval;
			fRb=fRbOriginal;
			fRa=fRaOriginal;
			if(fRa<fRb)
			{
				fRu_Reading=fRa;
				fRa=fRb;
				fRb=fRu_Reading;
			}
		}
	}
	else if( (!prmarray->nTestingMethod)&&(CtPair.nTest!=SRNO_ZCAL) )
	{
		sprintf(str1,"-%d    ",CtPair.Pair2);
		strcat( str, str1);
	}
	writeStr(2,4,(str),6);

	sprintf (str, "Raw Reading ::%*.*f ",nIntNo,nDecNo, CtPair.val);
	strcat( str, prmarray->szUnit);
	writeStr(2,5,(str),6);

	sprintf (str, "Normalised Reading ::%*.*f ",nIntNo,nDecNo, CtPair.norval);
	strcat( str, prmarray->szUnit);
	//strcat( str, "/km");
	strcat( str, "/NL");
	writeStr(2,6,(str),6);

	//Uncommented by:sdt:02112006:2225
	//Now RU parameter required in Pair Software.
	//Commented by:sdt:01102005:1330
	//RU Display removed, as it not required in Quad.
	if(CtPair.nTest==SRNO_CR && (CtPair.TipOrRing == RING))
	{
		//fRa=User_ceil(fRa,2);
		//fRb=User_ceil(fRb,2);
		int uRuMmtMethod = (unsigned char) GetPrivateProfileInt("MEASUREMENT_OPTIONS",
				"RU_MMT_FORMULA_NO",1,"default.ini");
		fRu_Reading= CalculateRu( fRa, fRb, uRuMmtMethod );//pr
		int nprTemp1;
		nprTemp1 =  FindNewDecimal(fRu_Reading,dispstruct[SRNO_RU].Decimal[0]);
		sprintf(str,"%s : %4.*f %s ","RU",nprTemp1,fRu_Reading,dispstruct[SRNO_RU].szUnit);
		//i_row += yChar;
		//outtextxy (i-16, i_row, str);
		writeStr(2,7,(str),6);
	}
	else
	{
		strcpy(str,"");
		writeStr(2,7,(str),6);
	}

	//sprintf(str,"Unit Size ::%s",nConfig.szUnitof);
	//writeStr(2,7,(str),6);


}

void TTestProgress::showProgress(int progress )
{

   char str[60] ;
   for(int i  = 0;i<60;i++)
	  str[i] = NULL;

 int i;
   for(i=0;i< progress;i++)
	 {
	   strcat(str,"л");
	 }

   writeStr(12,14,(str),6);
   sprintf(str,"%d",nCurrentReadingno);
   writeStr(24, 15, str,2);

}



/*void TTestProgress::showLegend( TPoint size, char color )
{
	TDrawBuffer buf;
	int indent = size.x-strlen("Legends  ")-2;

	buf.moveStr( 0, "Legends:-", color );
	writeLine( indent - 10, 0, strlen("Legends:-"), 1, buf );
	// legend colors
	//buf.moveCStr( 0, "\xDB HV", LEGEND_HV|color );

	buf.moveCStr( 0, "\xDB Pass", LEGEND_PASS | color );
	writeLine( indent, 1, strlen("\xDB Pass"), 1, buf );
	buf.moveCStr( 0, "\xDB Fail", LEGEND_FAIL | color );
	writeLine( indent, 3, strlen("\xDB Fail"), 1, buf );
	buf.moveCStr( 0, "\x4 Tip", color );
	writeLine( indent, 5, strlen("\xDB Tip"), 1, buf );
	buf.moveCStr( 0, "\xA Ring", color );
	writeLine( indent, 7, strlen("\xDB Ring"), 1, buf );
	char temp[4];
	int j = 1;
	for(int i=1;i<12;i=i+2)
	  {
		sprintf(temp,"Q%d",j++);
		writeStr(2,i,temp,2);
	  }
	writeStr(11,14,"л",2);
	writeStr(37,14,"л",2);

	//buf.moveCStr( 0, "\xDB Sheath", color );лллллллллл
	//writeLine( indent, 9, strlen("\xDB Sheath"), 1, buf );
}*/

void TTestProgress::draw()
{
	TDrawBuffer buf;

	char colorDefault = getColor( 6 );
	//if ( updateType == UPDATE_DEFAULT )
	//{
	for(int y = 0;y < 8; y++)
	buf.moveChar(y,' ', colorDefault, size.x);
	writeLine(0,0,50,16,buf);
	buf.putAttribute(0, getColor(2) );
	buf.moveChar( 0, 'Ф', getColor( 2 ), size.x );
	writeLine( 0, 12, size.x, 1, buf);
	writeStr(11,14,"л",2);
	writeStr(37,14,"л",2);
	writeStr(11,15,"0",2);
	char str[60] ;
	sprintf(str,"%d",nConfig.Max[CtPair.nTest]);
	writeStr(37,15,str,2);
	//}
	if ( updateType != UPDATE_DEFAULT )//Condition moved here by:sdt:01112005:1630
	{
		showProgress((nCurrentReadingno*25)/nConfig.Max[CtPair.nTest]);
		showCurrentReading();
	}
}

