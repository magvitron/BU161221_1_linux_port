//
//	Specks.Cpp - Implementation file for class TSpeck*
//			Builds the specification dialog and facilitates the user
//			with creation, deletion, and editing of the specification
//
#include "common.h"
#include "uses.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tobjstrm.h>
#include <strstream.h>          // ostrstream()
#include <io.h>
#include <fcntl.h>
#include <conio.h>


#include "ctype.h"
#include "common.h"
#include "speck.h"
#include "utils.h"
#include "ct.h"
#include "share.h"
#include "key.h"
#include "funproto.h"

using std::ostrstream;
using std::ends;


//Global Variable //Start
extern SPECKLIMIT specksLimit;
extern STRUCT_SPECKS speckData;
extern char cRamDrive[2];
extern PRM *prmarray;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2140 //For quad
extern FreqStruct LFFrequency;
extern int LfIndex;    		//Added by:sdt:09092005:2300
extern int HfIndex;			//Added by:sdt:09092005:2300
extern char SpeckList[MAXFREQUENCIES][10]; //Added by:sdt:09092005:2300
extern int ParamCtsHandle;

static parameterselect *selpara = new parameterselect ;
static parameterselect *selfreq = new parameterselect ;
static parameterselect *selcmbn = new parameterselect ;
//Global Variable //End


SpeckObject::~SpeckObject()
{
	delete speckName;
}

TSpeckBuild::TSpeckBuild( BYTE userType ) :
				TDialog( TRect( 30, 2, 76, 15 ), "Speck Build" ),
				TWindowInit(TSpeckBuild::initFrame)
{
	//Header header;//Commented by:sdt:27072005:1915

	m_nUserType = userType;


	TScrollBar *ps = new TScrollBar( TRect( 33, 2, 34, 10 ) );
	insert( ps );

	pl = new TSpeckListBox( TRect( 2, 2, 33, 10 ), 1, ps );
	pc = new TSpeckCollection(10,5);
	//"speck.cts" file is not further used to find speck files.
	if(!findSpeckFiles(pc))//Added by:sdt:27072005:1905
	{
		//pc = new TSpeckCollection(10,5);
		SpeckObject *sd = new SpeckObject;
		sd->fileNumber = 1;
		sd->speckName = newStr( "temp" );
		pc->insert( sd );
		pc->atFree( 0 );
	}
	pl->newList( pc );
	insert( pl );

	insert( new TLabel( TRect( 2, 1, 10, 2 ), "Names", pl) );

	TView::disableCommand( cmCreateNew );
	vc = new TButton( TRect( 34, 2, 44, 4 ), "~C~reate", cmCreateNew,
												   bfNormal	);
	//vc->state |= sfDisabled;
	insert( new TButton( TRect( 34, 4, 44, 6 ), "~E~dit", cmEdit,
												   bfDefault ) );
	vr = new TButton( TRect( 34, 6, 44, 8 ), "~R~emove", cmRemove,
													bfNormal );
	insert( new TButton( TRect( 34, 8, 44, 10 ), "~P~rint", cmPrint,
													bfNormal ) );
	insert( new TButton( TRect( 34, 10, 44, 12 ), "E~x~it", cmExitSpeck,
													bfNormal ) );
	insert( vc );
	insert( vr );

	selectNext( False );

	if ( userType == U_OPERATOR )
	{
		vr->setState( sfDisabled, True );
		vc->setState( sfDisabled, True );
	}
}

TSpeckBuild::~TSpeckBuild()
{

	pc->freeAll();
	delete pc;
}


void TSpeckBuild::createSpeck( Boolean newSpeck )
{
	memset( &speckData, 0, sizeof( STRUCT_SPECKS ) );

	TSpeckDataDlg *pd = new TSpeckDataDlg( TRect( 15, 3, 60, 20 ), "Specifications" );
	pd->ps = this;
	pd->newSpeck = newSpeck;
	// specification name
	pd->pn = new TInputLine( TRect( 16, 2, 31, 3 ), 28 );
	pd->insert( pd->pn );
	pd->insert( new TLabel( TRect( 2, 2, 15, 3 ), "Spec. Name:",pd->pn ));

 //   TScrollBar *SBar = new TScrollBar(TRect(17,4,18,5));

	//cable type
/*    TCollection	*TC;
	TC->insert(new TSItem("PAIR",new TSItem("QUAD",0)));
	TC->atFree(0);
	TListBox *LB = new TListBox(TRect(16,4,31,5),1,SBar);
    LB->newList(TC);
    pd->insert(LB);  */

	pd->pct = new TInputLine( TRect( 16, 4, 31, 5 ),14 );
	pd->insert( pd->pct );
	pd->insert( new TLabel( TRect( 2, 4, 15, 5 ), "Cable Type:",pd->pct));
	pd->pseq = new TInputLine( TRect( 16, 6, 31, 7), 35); 			//Added by:sdt:28072005:2300
	pd->insert( pd->pseq );                                               //Added by:sdt:28072005:2300
	pd->pseq->setState(sfDisabled,True);
	pd->insert( new TLabel( TRect( 2, 6, 15, 7), "Sequence:", pd->pseq )); //Added by:sdt:28072005:2300
	pd->pntemp = new TInputLine( TRect( 16, 8, 31, 9), 5); 			//Added by:sdt:02082005:1045
	pd->insert(pd->pntemp);											//Added by:sdt:02082005:1045
	pd->insert( new TLabel(TRect(2, 8, 15, 9), "Norm. Temp.", pd->pntemp));	//Added by:sdt:02082005:1045
	//Code for Normalisation length commented by:sdt:31012007:2110
	//pd->pnlen = new TInputLine( TRect( 16, 10, 31, 11), 8); 			//Added by:sdt:02082005:1045
	//pd->insert(pd->pnlen);											//Added by:sdt:02082005:1045
	//pd->insert( new TLabel(TRect(2, 10, 15, 11), "Norm. Length", pd->pnlen));	//Added by:sdt:02082005:1045

	//TView *vseq = new TButton( TRect(30, 9, 40, 11), "Se~l~ect", cmSelectSequence, bfNormal );//Added by:sdt:28072005:2300
	TView *vseq = new TButton( TRect(24, 12, 42, 14), "Cable Se~q~uence", cmSelectSequence, bfNormal );//Added by:sdt:28072005:2300
	pd->insert(vseq);
	TView *vs;
	if ( m_nUserType == U_OPERATOR )
		vs = new TButton( TRect( 2, 14, 10, 16 ), "~S~ave", cmSave, bfNormal );
	else
		vs = new TButton( TRect( 2, 14, 10, 16 ), "~S~ave", cmSave, bfDefault );
	pd->insert(vs);

	TView *vedit = new TButton( TRect(2,12,23,14), " ~E~dit Parameters",cmSelectParameter,bfNormal);
	pd->insert(vedit);

	TView *vsas = new TButton( TRect( 11, 14 ,22, 16 ), "S~a~ve as", cmSaveas, bfNormal );
	pd->insert(vsas);
	pd->insert(new TButton( TRect( 24, 14, 34, 16 ), "~C~ancel", cmCancel, bfNormal ));

	if ( m_nUserType == U_OPERATOR )
	{
		vs->setState( sfDisabled, True );
		vsas->setState( sfDisabled, True );
		vedit->setState( sfDisabled, True );
		vseq->setState( sfDisabled, True);
	}
	if ( newSpeck == False )
	{

		//if ( readSpeck( speckData,speckData.nSpeckFileNumber ) == True )//Modified by:sdt:05082005:2310
		if ( readSpeck( speckData,pl->focused+1 ) == True )
		{
			strcpy( pd->pn->data, speckData.specksnumber );
			strcpy( pd->pct->data, speckData.cabletype );
			strcpy (pd->pseq->data, speckData.szCableSequence.SeqName );
			sprintf (pd->pntemp->data, "%.1f",speckData.fNormTemp);
			//sprintf (pd->pnlen->data, "%.0f",speckData.fNormLength);
		}
		else
		{
			destroy( pd );
			return;
		}
	}
	pd->pn->select();
	TProgram::deskTop->execView( pd );
	destroy( pd );
}



void TSpeckBuild::handleEvent( TEvent& ev )
{
	/*if ( ev.what == evKeyboard )
	{
		switch( ev.keyDown.charScan.charCode )
		{
			case kbAltX:
				ev.what = evCommand;
				ev.message.command = cmExitSpeck;
				putEvent( ev );
				break;
		  }
		  clearEvent( ev );
	}*/

	if ( ev.what == evCommand )
	{
		switch( ev.message.command )
		{
			case cmCreateNew:
				if ( m_nUserType != U_OPERATOR )
				{
					char path[80];
					sprintf(path,"%s:\\intmlim.bin",cRamDrive);//Added by:sdt:23082001
					::remove(path);
					//system(path);

					sprintf(path,"%s:\\limits.bin",cRamDrive);
					::remove(path);
					//system(path);

					createSpeck( True );
				}
				break;
			case cmRemove:
				char fileName[ MAX_FILELEN ];
				if ( m_nUserType != U_OPERATOR )
				{
					sprintf( fileName, "%d.SPK", ((SpeckObject*)pc->at(pl->focused))->fileNumber );
					::remove( fileName );
					pc->atFree( pl->focused );
					pl->setRange( pc->getCount() );
					pl->drawView();
				}
				break;
			case cmEdit:
				createSpeck( False );
				break;
			case cmClose:
			case cmExitSpeck:
				if ( pc->getCount() != 0 )
				{
					Header header = {"CTS#SAT#SPK#001", "ePoint Technologies"};
					fpstream *lbs = new fpstream("specks.cts", ios::out|
													 ios::binary|
													 ios::trunc);
					if ( lbs->good() )
					{
						lbs->writeBytes( (char*)&header, sizeof( Header ) );
						*lbs << pc;
						delete lbs;
					}
					else
						messageBox( "Error creating file Specks.Cts", mfError|mfOKButton );
				}
				destroy( pl );
				destroy( this );

				break;
		}
	}
		TDialog::handleEvent( ev );
		clearEvent( ev );
}


void TSpeckCollection::freeItem( void *item )
{
	delete (SpeckObject *)item;
}

void *TSpeckCollection::readItem( ipstream& is )
{
	SpeckObject *ptr = new SpeckObject;
	is >> ptr->fileNumber;
	ptr->speckName = is.readString();
	return ptr;
}

void TSpeckCollection::writeItem( void *obj, opstream& os )
{
	SpeckObject *ptr = (SpeckObject*)obj;

	os << ptr->fileNumber;
	os.writeString((const char*)ptr->speckName);
}

const char * const TSpeckCollection::name = "TSpeckCollection";

TStreamable *TSpeckCollection::build()
{
	return new TSpeckCollection( streamableInit );
}

TStreamableClass RFCollection( TSpeckCollection::name,
								   TSpeckCollection::build,
								   __DELTA(TSpeckCollection)
								 );


TSpeckListBox::TSpeckListBox(const TRect& bounds, ushort aNumCols,
				  TScrollBar *aScrollBar):
	 TListBox(bounds, aNumCols, aScrollBar)
{
}


void TSpeckListBox::getText( char *dest, short item, short maxLen )
{
	cout.setf(ios::right);
	ostrstream(dest, maxLen-1)
	<< ((TSpeckCollection*)list())->at(item)->speckName
	<< ends;
}

void TSpeckBuild::editParameter()
{
  TEditParameter *pEP = new TEditParameter( TRect(5,2 ,63, 22),"Edit Parameters");
  pEP->insert(new TButton( TRect( 33, 16 ,44 ,18  ), " ~C~ancel", cmCancel, bfNormal ));
  pEP->insert(new TButton( TRect( 17, 16, 28, 18), "~O~K", cmOKEdit, bfDefault ));

  pEP->pLowerLimit = new TInputLine(TRect(17,2,25,3),8);
  pEP->insert(pEP->pLowerLimit);
  pEP->insert(new TLabel(TRect( 2,2,14,3),"Lower",pEP->pLowerLimit));
  if(specksLimit.lowerlimit!=(float)BLANKVALUE)  //Condition added by:sdt:04122015:0630
	sprintf(pEP->pLowerLimit->data,"%0.2f",specksLimit.lowerlimit);
  else
	strcpy(pEP->pLowerLimit->data,"");

  pEP->pUpperLimit = new TInputLine(TRect(17,4,25,5),8);
  pEP->insert(pEP->pUpperLimit);
  pEP->insert(new TLabel(TRect( 2,4,14,5),"Upper",pEP->pUpperLimit));
  if(specksLimit.upperlimit!=(float)BLANKVALUE) //Condition added by:sdt:04122015:0630
	sprintf(pEP->pUpperLimit->data,"%0.2f",specksLimit.upperlimit);
  else
	strcpy(pEP->pUpperLimit->data,"");

  pEP->pAverageLow = new TInputLine(TRect(17,6,25,7),8);
  pEP->insert(pEP->pAverageLow);
  pEP->insert(new TLabel(TRect( 2,6,14,7),"Avg. Low",pEP->pAverageLow));
  if(specksLimit.averagelow!=(float)BLANKVALUE) //Condition added by:sdt:04122015:0630
	sprintf(pEP->pAverageLow->data,"%0.2f",specksLimit.averagelow);
  else
	strcpy(pEP->pAverageLow->data,"");

  pEP->pAverageHigh = new TInputLine(TRect(17,8,25,9),8);
  pEP->insert(pEP->pAverageHigh);
  pEP->insert(new TLabel(TRect( 2,8,14,9),"Avg. High",pEP->pAverageHigh));
  if(specksLimit.averagehi!=(float)BLANKVALUE) //Condition added by:sdt:04122015:0630
	sprintf(pEP->pAverageHigh->data,"%0.2f",specksLimit.averagehi);
  else
	strcpy(pEP->pAverageHigh->data,"");

  pEP->pStdLow = new TInputLine(TRect(17,10,25,11),8);
  pEP->insert(pEP->pStdLow);
  pEP->insert(new TLabel(TRect( 2,10,14,11),"Std. Low",pEP->pStdLow));
  if(specksLimit.stdlow!=(float)BLANKVALUE) //Condition added by:sdt:04122015:0630
	sprintf(pEP->pStdLow->data,"%0.2f",specksLimit.stdlow);
  else
	strcpy(pEP->pStdLow->data,"");

  pEP->pStdHigh = new TInputLine(TRect(17,12,25,13),8);
  pEP->insert(pEP->pStdHigh);
  pEP->insert(new TLabel(TRect( 2,12,14,13),"Std. High",pEP->pStdHigh));
  if(specksLimit.stdhi!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pStdHigh->data,"%0.2f",specksLimit.stdhi);
  else
	strcpy(pEP->pStdHigh->data,"");

  pEP->pRMSLow = new TInputLine(TRect(43,2,51,3),8);
  pEP->insert(pEP->pRMSLow);
  pEP->insert(new TLabel(TRect( 27,2,39,3),"RMS Low",pEP->pRMSLow));
  if(specksLimit.rmslow!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pRMSLow->data,"%0.2f",specksLimit.rmslow);
  else
	strcpy(pEP->pRMSLow->data,"");

  pEP->pRMSHigh = new TInputLine(TRect(43,4,51,5),8);
  pEP->insert(pEP->pRMSHigh);
  pEP->insert(new TLabel(TRect( 27,4,39,5),"RMS High",pEP->pRMSHigh));
  if(specksLimit.rmshi!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pRMSHigh->data,"%0.2f",specksLimit.rmshi);
  else
	strcpy(pEP->pRMSHigh->data,"");

  pEP->pPSumLow = new TInputLine(TRect(43,6,51,7),8);
  pEP->insert(pEP->pPSumLow);
  pEP->insert(new TLabel(TRect( 27,6,39,7),"PSUM Low",pEP->pPSumLow));
  if(specksLimit.psumlow!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pPSumLow->data,"%0.2f",specksLimit.psumlow);
  else
	strcpy(pEP->pPSumLow->data,"");

  pEP->pPSumHigh = new TInputLine(TRect(43,8,51,9),8);
  pEP->insert(pEP->pPSumHigh);
  pEP->insert(new TLabel(TRect( 27,8,39,9),"PSUM High",pEP->pPSumHigh));
  if(specksLimit.psumhi!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pPSumHigh->data,"%0.2f",specksLimit.psumhi);
  else
	strcpy(pEP->pPSumHigh->data,"");

  pEP->pWpnLow = new TInputLine(TRect(43,10,51,11),8);
  pEP->insert(pEP->pWpnLow);
  pEP->insert(new TLabel(TRect( 27,10,39,11),"WPN Low",pEP->pWpnLow));
  if(specksLimit.wpnlow!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pWpnLow->data,"%0.2f",specksLimit.wpnlow);
  else
	strcpy(pEP->pWpnLow->data,"");

  pEP->pWpnHigh = new TInputLine(TRect(43,12,51,13),8);
  pEP->insert(pEP->pWpnHigh);
  pEP->insert(new TLabel(TRect( 27,12,39,13),"WPN High",pEP->pWpnHigh));
  if(specksLimit.wpnhi!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pWpnHigh->data,"%0.2f",specksLimit.wpnhi);
  else
	strcpy(pEP->pWpnHigh->data,"");

  pEP->pAnyOther = new TInputLine(TRect(17,14,25,15),8);
  pEP->insert(pEP->pAnyOther);
  pEP->insert(new TLabel(TRect( 2,14,14,15),"Any Other",pEP->pAnyOther));
  if(specksLimit.anyother!=(float)BLANKVALUE)//Condition added by:sdt:05122015:0520
	sprintf(pEP->pAnyOther->data,"%0.2f",specksLimit.anyother);
  else
	strcpy(pEP->pAnyOther->data,"");

  //Added here by:sdt:31012007:2120
  pEP->pnlen = new TInputLine( TRect(43,14,51,15), 8);
  pEP->insert(pEP->pnlen);
  pEP->insert( new TLabel(TRect(27, 14, 39, 15), "Norm. Length", pEP->pnlen));
  sprintf(pEP->pnlen->data,"%.0f",speckData.speckparams[selpara->select].fNormLength);

  pEP->pLowerLimit->select();
  TProgram::deskTop->execView( pEP );
  TDialog::destroy (pEP);
}

void TEditParameter::handleEvent(TEvent &ev)
{
	TDialog::handleEvent(ev);
	if(ev.what == evCommand)
	 {
		if(ev.message.command == cmOKEdit)
		{
			if(strcmp(pLowerLimit->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.lowerlimit =(float)BLANKVALUE;
			else
				specksLimit.lowerlimit = atof( pLowerLimit->data );


			if(strcmp(pUpperLimit->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.upperlimit = (float)BLANKVALUE;
			else
				specksLimit.upperlimit = atof( pUpperLimit->data );

			if(strcmp(pAverageLow->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.averagelow = (float)BLANKVALUE;
			else
				specksLimit.averagelow = atof( pAverageLow->data );

			if(strcmp(pAverageHigh->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.averagehi = (float)BLANKVALUE;
			else
				specksLimit.averagehi = atof( pAverageHigh->data );

			if(strcmp(pStdLow->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.stdlow = (float)BLANKVALUE;
			else
				specksLimit.stdlow = atof( pStdLow->data );

			if(strcmp(pStdHigh->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.stdhi = (float)BLANKVALUE;
			else
				specksLimit.stdhi = atof( pStdHigh->data );

			if(strcmp(pRMSLow->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.rmslow = (float)BLANKVALUE;
			else
				specksLimit.rmslow = atof( pRMSLow->data );

			if(strcmp(pRMSHigh->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.rmshi = (float)BLANKVALUE;
			else
				specksLimit.rmshi = atof( pRMSHigh->data );

			if(strcmp(pPSumLow->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.psumlow = (float)BLANKVALUE;
			else
				specksLimit.psumlow = atof( pPSumLow->data );

			if(strcmp(pPSumHigh->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.psumhi = (float)BLANKVALUE;
			else
				specksLimit.psumhi = atof( pPSumHigh->data );

			if(strcmp(pWpnLow->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.wpnlow = (float)BLANKVALUE;
			else
				specksLimit.wpnlow = atof( pWpnLow->data );

			if(strcmp(pWpnHigh->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.wpnhi = (float)BLANKVALUE;
			else
				specksLimit.wpnhi = atof( pWpnHigh->data );

			if(strcmp(pAnyOther->data,"")==0)//Condition added by:sdt:05122015:0535
				specksLimit.anyother = (float)BLANKVALUE;
			else
				specksLimit.anyother = atof( pAnyOther->data );

			speckData.speckparams[selpara->select].fNormLength = atof ( pnlen->data ); //Added by:sdt:31102007:2125
			//Added by:sdt:03082005:1935
			ModifyIntermediateLimits(selpara->select,selfreq->select,selcmbn->select,specksLimit);

		//Commented by:sdt:27072005:1915
		/*for (int i=0;i<2;i++)
		{
		speckData.speckparams[selpara->select].s[i].lowerlimit = atof(pAbsLowerLimit->data);
		speckData.speckparams[selpara->select].s[i].upperlimit = atof(pAbsUpperLimit->data);
		speckData.speckparams[selpara->select].s[i].averagelow = atof(pAbsAverageLow->data);
		speckData.speckparams[selpara->select].s[i].averagehi = atof(pAbsAverageHigh->data);
		speckData.speckparams[selpara->select].s[i].std = atof(pStd->data);
		speckData.speckparams[selpara->select].s[i].rms = atof(pRMS->data);
		speckData.speckparams[selpara->select].s[i].anyother = atof(pAnyOther->data);

		}*/
			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );
		}
	 }
}


void TSpeckDataDlg::handleEvent( TEvent& ev )
{
	TDialog::handleEvent( ev );
	ushort result = True;

	if ( ev.what == evCommand )
	{
		if( ev.message.command == cmSelectParameter)
		{
			CopyLimitsToIntmLim();//Added by:sdt:03082005:1945
			ps->selectParameter();
			return;
		}
		if ( ev.message.command == cmSave )
		{
			if ( newSpeck == True )
				result = ps->saveSpeck( this, CONST_SAVEAS );
			else
				result = ps->saveSpeck( this, CONST_SAVE );
		}
		if ( ev.message.command == cmSaveas )
			result = ps->saveSpeck( this, CONST_SAVEAS );

		if ( ev.message.command == cmSelectSequence ) //Added by:sdt:28072005:2300
		{
			ps->selectCableSequence();
			strcpy (pseq->data, speckData.szCableSequence.SeqName );
			this->redraw();
			return;
		}
		if ( result == False )
			clearEvent( ev );
		else
		{
			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );
		}
	}
}



void TSpeckBuild::selectParameter()
{
	TSelectParameter *pSP = new TSelectParameter( TRect( 22, 7,60,19),"Select Parameter");
	pSP->pParameter  = new TRadioButtons(TRect(2,2,35,8 ),new TSItem(paraminfo[0].szParamAbr,
											new TSItem(paraminfo[1].szParamAbr,
											new TSItem(paraminfo[2].szParamAbr,
											new TSItem(paraminfo[3].szParamAbr,
											new TSItem(paraminfo[4].szParamAbr,
											new TSItem(paraminfo[5].szParamAbr,
											new TSItem(paraminfo[6].szParamAbr,
											new TSItem(paraminfo[7].szParamAbr,
											new TSItem(paraminfo[8].szParamAbr,
											new TSItem(paraminfo[9].szParamAbr,
											new TSItem(paraminfo[10].szParamAbr,0))))))))))));
											//Modified by:sdt:30092005:2000 for Quad software.
											//new TSItem(paraminfo[11].szParamAbr,
											//new TSItem(paraminfo[12].szParamAbr,
											//new TSItem(paraminfo[13].szParamAbr,
											//new TSItem(paraminfo[14].szParamAbr,0))))))))))))))));





 pSP->insert(pSP->pParameter);
 pSP->insert(new TButton( TRect( 16, 9 ,27 ,11  ), " ~C~ancel", cmCancel, bfNormal ));
 pSP->insert(new TButton( TRect( 5,9 ,14, 11), "~E~dit", cmEditParameter, bfDefault ));
 pSP->pParameter->select();

 TProgram::deskTop->execView( pSP );
 destroy (pSP);
}

void TSelectParameter::handleEvent(TEvent &ev)
{
    TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmEditParameter)
		{
			this->getData(selpara);
			//Added by:sdt:20112005:0100
			if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
			{
				clrscr();
				printf( "\nError Opening File PARAM.CTS File for Reading..." );
				RemoveIniFiles();
				exit( 1 );
			}

			ReadParamDotCts( paraminfo[selpara->select].szParamAbr , prmarray);
			::close( ParamCtsHandle );//Added by:sdt:05012006:1950
			//CopyLimitsToIntmLim();//Added by:sdt:03082005:1945
			if(selpara->select==SRNO_RU||selpara->select==SRNO_LR||selpara->select==SRNO_CR)//Added by:sdt:31072005:1330
			{
				selfreq->select = LFFrequency.nDefualtFreq;
				selcmbn->select = short (0);
				specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
				psdlg->ps->editParameter();

			}
			else
			{
				psdlg->ps->selectFrequency();
			}

		}


	}
	TDialog::handleEvent( ev );
	clearEvent( ev );

}

TSelectParameter::~TSelectParameter()
{

}


Boolean TSpeckBuild::saveSpeck( TDialog *d, ushort type )
{
	int bNewFile=0;
	if ( getData( d ) == False )
		return False;
	// check for Duplicate Specks Names
	int i = -1;
	int l = pc->getCount();
	while( ++i < l )
	{
		if ( i == pl->focused )
			continue;
	}
	SpeckObject *obj;
	if ( type == CONST_SAVEAS )
	{
		obj = new SpeckObject;
		obj->speckName = newStr( speckData.specksnumber );
		obj->fileNumber = pc->getCount() + 1;
		//obj->fileNumber = peckData.nSpeckFileNumber //Changed by:sdt:06082005:1955
		pc->insert( obj );

	}
	else if ( type == CONST_SAVE )
	{
		obj = (SpeckObject*)pc->at(pl->focused);
		obj->speckName = newStr(((TSpeckDataDlg*)d)->pn->data);
		obj->fileNumber = pl->focused + 1;
		pc->atPut( pl->focused, obj );
	}
	// write specification to file
	char fileName[ MAX_FILELEN ];
	Header header;

	sprintf( fileName, "%d.SPK", obj->fileNumber );

	ofstream out( fileName, ios::binary|ios::trunc );
	// make the header
	if(!out.good())
	{
		bNewFile = 1;
	}
	strcpy( header.szSignature, "CTS#SAT#SPK#001" );
	strcpy( header.szCompany, "ePoint Technologies" );
	out.write( (char*)&header, sizeof( Header ) );
	out.write( (char*)&speckData, sizeof( STRUCT_SPECKS ) );
	out.close();

	//if() loop added for following case
	//While speck creation after inserting Speck Name, Cable Type ...etc,
	//user choose option as select parameter instead of Save Speck.
	//In that case speck values inserted by user will be present in
	//intmlim.bin & limits.bin  files. There this should read from that.
	//If bNewFile =1, limits value will be taken as 0.0 in WriteLimitInSivFile()
	//function.
	if(bNewFile)
	{
		sprintf( fileName,"%s:\\intmlim.bin",cRamDrive);
		ifstream in(fileName, ios::binary);
		if(!in.good())
		{
			sprintf( fileName,"%s:\\limits.bin",cRamDrive);
			ifstream inlim(fileName, ios::binary);
			if(!inlim.good())
			{
				bNewFile = 1;
			}
			else
				bNewFile = 0;
			inlim.close();
		}
		else
			bNewFile = 0;
		in.close();
	}
	WriteLimitInSivFile(obj->fileNumber,bNewFile);//Function call added by:sdt:03082005:1925

	pl->setRange( pc->getCount());
	pl->drawView();

	return True;
}

Boolean	TSpeckBuild::getData( TDialog *d )
{
 //	char *endPtr;
	Boolean errorFlag = False;

	if ( strcmp( ((TSpeckDataDlg*)d)->pn->data, "" )==0 )
	{
		messageBox( "Specification Name cannot be Blank", mfError|mfOKButton );
		((TSpeckDataDlg*)d)->pn->select();
		return False;
	}
	strcpy( speckData.specksnumber, ((TSpeckDataDlg*)d)->pn->data );
	strcpy( speckData.cabletype, ((TSpeckDataDlg*)d)->pct->data );
	strcpy (speckData.szCableSequence.SeqName, ((TSpeckDataDlg*)d)->pseq->data);
	speckData.fNormTemp = atof(((TSpeckDataDlg*)d)->pntemp->data);
	//speckData.fNormLength = atof(((TSpeckDataDlg*)d)->pnlen->data);//Commented by:sdt:31012007:2150
	strupr( speckData.specksnumber);
	strupr( speckData.cabletype);
	//strupr( speckData.szCableSequence.SeqName );//Commented by:sdt:28082005:1250

	if(strspn(speckData.cabletype,"PIJF")==4)
	  {
	   speckData.Type = 0;//'P';//Modified by:sdt:08102005:1745
	  strcpy(speckData.guage,strtok(speckData.cabletype,"PIJF_"));

	  }
	if(strspn(speckData.cabletype,"QUAD")==4)
	  {
	   speckData.Type = 0;//'Q';//Modified by:sdt:08102005:1745
	   strcpy(speckData.guage,strtok(speckData.cabletype,"QUAD_"));
	  }

	if ( errorFlag == True )
	{
		messageBox( "Errornous Input for Specification", mfError|mfOKButton );
		return False;
	}
	return True;
}

void TSpeckBuild::getAccessState( ushort& first, ushort& second )
{
	first = vc->state;
	second = vr->state;
}

void TSpeckBuild::setAccessState( ushort& first, ushort& second )
{
	vc->state = first;
	vr->state = second;
}

Boolean readSpeck( STRUCT_SPECKS &speckData, int fileNumber )
{
	char fileName[ MAX_FILELEN ];
	Header header;
	ifstream in;

	sprintf( fileName, "%d.SPK", fileNumber );
	in.open( fileName, ios::binary );
	if ( !in.good() )
	{
		messageBox( mfError|mfOKButton, "Cannot find Speck file %s\n"
										"Please use Remove to delete Specification", fileName );
		return False;
	}
	in.read( (char *)&header, sizeof( Header ) );
	if ( strcmp( header.szSignature, "CTS#SAT#SPK#001" )!=0 ||
		 strcmp( header.szCompany, "ePoint Technologies" ) !=0 )
	{
		messageBox( mfError|mfOKButton, "Error in file %s\n", fileName );
		return False;
	}
	in.read( (char*)&speckData, sizeof(STRUCT_SPECKS ) );
	speckData.nSpeckFileNumber = fileNumber;
	in.close();
	ReadLimitFromSivFile(fileNumber); //Added by:sdt:03082005:1925
	return True;
}

//int TSpeckBuild::findSpeckFiles(void)
// Replaces DOS/borland ios::nocreate usage for Linux/GCC build
int findSpeckFiles(TSpeckCollection *pc)
{
    struct find_t ffblk;
    Header header;

    // Search for specification files present in current directory
    if (!(_dos_findfirst("*.spk", _A_NORMAL, &ffblk)))
    {
        do
        {
            // Open file for reading (nocreate is not available on GCC/libstdc++)
            fpstream *lbs = new fpstream(ffblk.name, ios::in | ios::binary);

            if (lbs->good())
            {
                lbs->readBytes((char *)&header, sizeof(Header));

                if (strcmp(header.szSignature, "CTS#SAT#SPK#001") != 0 ||
                    strcmp(header.szCompany, "ePoint Technologies") != 0)
                {
                    messageBox(mfError | mfOKButton,
                               "Error in file Specification File");
                }
                else
                {
                    // Extract number from "NNN.SPK" safely without multiple strtok() calls
                    char tmpName[MAX_FILELEN];
                    strncpy(tmpName, ffblk.name, sizeof(tmpName) - 1);
                    tmpName[sizeof(tmpName) - 1] = '\0';

                    char *dot = strrchr(tmpName, '.');
                    if (dot)
                        *dot = '\0';

                    int fileNum = atoi(tmpName);

                    if (readSpeck(speckData, fileNum) == True)
                    {
                        SpeckObject *sd = new SpeckObject;
                        speckData.nSpeckFileNumber = fileNum;
                        sd->fileNumber = fileNum;
                        sd->speckName = newStr(speckData.specksnumber);
                        pc->insert(sd);
                    }
                }
            }

            // fpstream cleanup
            delete lbs;

        } while (!(_dos_findnext(&ffblk)));

        return 1;
    }

    return 0;
}


void TSpeckBuild::selectCableSequence()
{
	struct find_t ffblk;
	char fileName[ MAX_FILELEN ];
	TSelectSequence *pselseq = new TSelectSequence( TRect( 30, 2, 76, 11 ), "Cable Sequence" );
	//pselseq->ps = this;

	TScrollBar *pscrl = new TScrollBar( TRect( 33, 2, 34, 8 ) );
	pselseq->insert( pscrl );

	pselseq->pcblseq = new TSpeckListBox( TRect( 2, 2, 33, 8 ), 1, pscrl );

	if (!(_dos_findfirst("*.seq",_A_NORMAL,&ffblk)))
	{
		pselseq->pseqcol = new TSpeckCollection(10,5);
		sprintf( fileName, "%s:\\seq.dat", cRamDrive );
		ofstream out( fileName, ios::binary );
		do
		{
			//fpstream *lbs = new fpstream(ffblk.name, ios::in|
			//							   ios::binary|
			 //							   ios::nocreate
			 //							   );
			//if(lbs->good()&&(strcmpi(ffblk.name,"gen.seq")!=0))
			if(strcmpi(ffblk.name,"gen.seq")!=0)
			{
				GetPrivateProfileString ("SEQ_LIST","SEQUENCE_NAME",speckData.szCableSequence.SeqName,"",ffblk.name);
				strcpy(speckData.szCableSequence.FileName,ffblk.name);
				if(strcmp(speckData.szCableSequence.SeqName,"")!=0)
				{
					SpeckObject *sd = new SpeckObject;
					sd->fileNumber = atoi(strtok(ffblk.name,".seq"));
					sd->speckName = newStr(speckData.szCableSequence.SeqName);
					pselseq->pseqcol->insert( sd );
				}
				out.write( (char*)&speckData.szCableSequence, sizeof( SEQNAMES ) );


			}
		}while(!(_dos_findnext(&ffblk)));
		out.close();
	}
	pselseq->pcblseq->newList( pselseq->pseqcol );
	pselseq->insert(pselseq->pcblseq);
	pselseq->insert(new TButton( TRect( 34, 4, 44, 6), " ~C~ancel", cmCancel, bfNormal ));
	pselseq->insert(new TButton( TRect( 34, 2, 44, 4), "~O~K", cmOKSelectSequence, bfDefault ));
	TProgram::deskTop->execView( pselseq );
	destroy (pselseq);
}

void TSelectSequence::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmOKSelectSequence)
		{
			pcblseq->getText(speckData.szCableSequence.SeqName, pcblseq->focused,30);
			//Added by:sdt:28082005:1230
			//This code will store sequnce file name of selected sequence in
			//speckData Strcture.
			char fileName[ MAX_FILELEN ];
			SEQNAMES sqnmSpeck;
			sprintf( fileName, "%s:\\seq.dat", cRamDrive );
			ifstream in( fileName, ios::binary );
			if(in.good())
			{   while(in.eof()==0)
				{
					in.read( (char *)&sqnmSpeck, sizeof( SEQNAMES ) );
					if(strcmp(sqnmSpeck.SeqName,speckData.szCableSequence.SeqName)==0)
					{
						strcpy(speckData.szCableSequence.FileName,sqnmSpeck.FileName);
						break;
					}
				}
			}
			in.close();
			//Commented by:sdt:06082005:2330
			// write specification to file
			/*char fileName[ MAX_FILELEN ];
			Header header;

			sprintf( fileName, "%d.SPK", speckData.nSpeckFileNumber );
			ofstream out( fileName, ios::binary|ios::trunc );
			// make the header
			strcpy( header.szSignature, "CTS#SAT#SPK#001" );
			strcpy( header.szCompany, "ePoint Technologies" );
			out.write( (char*)&header, sizeof( Header ) );
			out.write( (char*)&speckData, sizeof( STRUCT_SPECKS ) );
			out.close();*/

			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );

		}

	}
	//TDialog::handleEvent( ev );
	//clearEvent( ev );

}


TSelectSequence::~TSelectSequence()
{
	pseqcol->freeAll();
	delete pseqcol;
}


//Added by:sdt:31072005:1250
void TSpeckBuild::selectFrequency()
{

	TSelectFrequency *pSF = new TSelectFrequency( TRect( 22, 7,57,19),"Select Frequency");

	pSF->insert( new TLabel( TRect( 2, 1, 17, 2 ), "LOW FREQ.", pSF) );

	//pSF->pLFFrequency  = new TRadioButtons(TRect(2,2,17,6 ),new TSItem("0.08 KHZ",
	//										new TSItem("0.8 KHZ",
	//										new TSItem("1 KHZ",
	//										new TSItem("1.6 KHZ",0)))));
	//Modified by:sdt:01102005:0930
	//For Quad : LF Frequencied increased to 6 & HF reduced to 2.
	pSF->pLFFrequency  = new TRadioButtons(TRect(2,2,33,5 ),new TSItem(SpeckList[0],
											new TSItem(SpeckList[1],
											new TSItem(SpeckList[2],
											new TSItem(SpeckList[3],
											new TSItem(SpeckList[4],
											new TSItem(SpeckList[5],0)))))));

	pSF->insert(pSF->pLFFrequency);
	pSF->insert( new TLabel( TRect( 2, 6, 17, 7 ), "HIGH FREQ.", pSF) );
	//pSF->pHFFrequency  = new TRadioButtons(TRect(18,2,33,8 ),new TSItem("12 KHZ",
	//											new TSItem("80 KHZ",
	//											new TSItem("150 KHZ",
	//											new TSItem("772 KHZ",
	//											new TSItem("1000 KHZ",
	//											new TSItem("2000 KHZ",0)))))));
	pSF->pHFFrequency  = new TRadioButtons(TRect(2,7,33,8 ),new TSItem(SpeckList[6],
												new TSItem(SpeckList[7],0)));

	pSF->insert(pSF->pHFFrequency);


	pSF->insert(new TButton( TRect( 5, 9 ,14, 11), "~E~dit", cmEditFrequency, bfDefault ));
	//pSF->insert(new TButton( TRect( 12, 9 ,20 ,11  ), "~O~K", cmOKFrequency, bfNormal ));
	pSF->insert(new TButton( TRect( 19, 9 ,32 ,11  ), "~C~ancel", cmCancel, bfNormal ));
	//Added by:sdt:03082005:1925//For Marking Status of selected frequencies.
	//specksLimit=FindLimits(parameternumber,nFreqNum,limitarray,1);

	if(!prmarray->nParamType) //For LF Parameters HF Frequencies are disabled.
	{
		pSF->pHFFrequency->setState( sfDisabled, True );
		pSF->pLFFrequency->select();
	}
	else
	{
		pSF->pHFFrequency->select();
	}

	TProgram::deskTop->execView( pSF );
	destroy (pSF);
}

void TSelectFrequency::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmEditFrequency)
		{
			if(this->pHFFrequency->getState(sfFocused))
			{
				this->pHFFrequency->getData(selfreq);
				//Added by:sdt:09082005:1225
				selfreq->select+=LFFrequency.nTotalFrequencies;//Added as Limits for HF frequencies stored after LF Frequencies
				//Code Added by:sdt:01102005:1410
				//As combination selection is not required in Quad
				selcmbn->select = short (0);
				specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
				psdlg->ps->editParameter();
				//Commented by:sdt:01102005:1410
				/*if(prmarray->nTestingMethod)
				{
					selcmbn->select = short (0);
					specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
					psdlg->ps->editParameter();
				}
				else
				{
					psdlg->ps->selectCombination();
				}*/
			}
			else
			{
				//int i = *(this->pLFFrequency->iSel);
				this->pLFFrequency->getData(selfreq);
				//Condiotion removed by:sdt:01102005:1300 //for Quad software
				//if(prmarray->nParamType && (selfreq->select==0))
				//{
				//	messageBox( "Not valid HF frequency", mfError|mfOKButton );
				//}
				//Code Added by:sdt:01102005:1410
				//As combination selection is not required in Quad
				selcmbn->select = short (0);
				specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
				psdlg->ps->editParameter();
				//Commented by:sdt:01102005:1410
				/*if(prmarray->nTestingMethod)
				{
					specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
					psdlg->ps->editParameter();
				}
				else
				{
					psdlg->ps->selectCombination();
				}*/
			}

		}
		//Added by:sdt:03082005:1940
		/*if(ev.message.command == cmOKFrequency)
		{
			//ModifyIntermediateLimits(parameternumber,selfreq.select,selcmbn.select,specksLimit);
		}*/
	}
	TDialog::handleEvent( ev );
	clearEvent( ev );
}

//Commented by:sdt:15112005:1650
//Added by:sdt:31072005:1520
/*void TSpeckBuild::selectCombination()
{
	TSelectCombination *pSC = new TSelectCombination( TRect( 23, 8, 57,20),"Select Combination");

	pSC->insert( new TLabel( TRect( 2, 1, 32, 2 ), "PAIR TO PAIR", pSC) );
	pSC->pPairCombinations  = new TRadioButtons(TRect( 2, 2, 32, 4 ),	new TSItem("INTRA ADJACENT PAIRS",
														new TSItem("INTRA NONADJACENT PAIRS", 0)));
	//pSC->pPairCombinations  = new TDerivedCluster(TRect( 2, 2, 32, 4 ),	new TSItem("INTRA ADJACENT PAIRS",
	//													new TSItem("INTRA NONADJACENT PAIRS", 0)));

	pSC->insert(pSC->pPairCombinations);
	pSC->insert( new TLabel( TRect( 2, 5, 32, 6 ), "UNIT TO UNIT", pSC) );
	pSC->pUnitCombinations  = new TRadioButtons(TRect( 2, 6, 32, 8 ),	new TSItem("ADJACENT UNITS",
														new TSItem("NON ADJACENT UNITS",0)));
	pSC->insert(pSC->pUnitCombinations);

	pSC->insert(new TButton( TRect( 5 , 9, 14, 11 ), "~E~dit", cmEditCombination, bfDefault ));
	//pSC->insert(new TButton( TRect( 12, 9, 20 , 11 ), "~O~K", cmOKCombination, bfNormal ));
	pSC->insert(new TButton( TRect( 19, 9, 32 , 11 ), "~C~ancel", cmCancel, bfNormal ));

	if(selpara->select==SRNO_CUPPI)//For Unit to Unit OR Pair to Pair Testing Method.
	{
		pSC->pPairCombinations->setState( sfDisabled, True );
		pSC->pUnitCombinations->select();
	}
	else
	{
		pSC->pUnitCombinations->setState( sfDisabled, True );
		pSC->pPairCombinations->select();
	}


	//Added by:sdt:03082005:1925//For Marking Status of selected frequencies.
	//specksLimit=FindLimits(selpara.select,selfreq.select,selcmbn.select,1);

	TProgram::deskTop->execView( pSC );
	destroy (pSC);
}*/

//Commented by:sdt:15112005:1650
/*void TSelectCombination::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmEditCombination)
		{
			//int i = *(this->pPairCombinations->iSel);
			if(this->pPairCombinations->getState(sfFocused))
			{
				this->pPairCombinations->getData(selcmbn);
				specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
				psdlg->ps->editParameter();

			}
			else
			{
				this->pUnitCombinations->getData(selcmbn);
				specksLimit=FindLimits(selpara->select,selfreq->select,selcmbn->select,1);
				psdlg->ps->editParameter();
			}

		}
		//Added by:sdt:03082005:1940
		//if(ev.message.command == cmOKCombination)
		//{
		//	ModifyIntermediateLimits(selpara.select,selfreq.select,selcmbn.select,specksLimit);
		//}
	}
	TDialog::handleEvent( ev );
	clearEvent( ev );

}*/

