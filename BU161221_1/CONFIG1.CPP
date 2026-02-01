//
// Config.Cpp - Implementation file for class TConfig
//			builds config dialog for taking configuration of the cable
//			from the user
//
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "common.h"
#include "cconfig.h"
#include "utils.h"
#include "seq.h"
#include "funproto.h"

//Global variable
extern STRUCT_SPECKS speckData;
extern char 	szSeqFileName[15];
extern SEQUENCE CableSequence;

TConfig::TConfig():TDialog( TRect( 13, 1, 67, 21 ), "Configuration" ),
				TWindowInit( &TConfig::initFrame )
{
}

void TConfig::buildConfig( Config& config )
{
	TScrollBar *psb = new TScrollBar( TRect( 32, 2, 33, 5 ) );
	plb = new TSpeckListBox( TRect( 2, 2, 32, 5 ), 1, psb );
	insert( psb );
	//Specks.CTS is not used further to find speck files.
	/*Header header;
	ifpstream in( "specks.cts", ios::binary );
	if ( in.good() )
	{
		in.readBytes( (char *)&header, sizeof( Header ) );
		if ( strcmp( header.szSignature, "CTS#SAN#SPK#000" )!=0 ||
		 strcmp( header.szCompany, "Sivananda Electronics" ) !=0 )
		{
			messageBox( mfError|mfOKButton, "Error in file Specks.Cts" );
			return;
		}
		else
		{
			in >> ( TCollection * & )pc;
			in.close();
		}
	}*/
	//Added by:sdt:26082005:1250
	pc = new TSpeckCollection(10,5);
	if(!findSpeckFiles(pc))
	{
		SpeckObject *sd = new SpeckObject;
		sd->fileNumber = 1;
		sd->speckName = newStr( "temp" );
		pc->insert( sd );
		pc->atFree( 0 );
	}

	plb->newList( pc );
	insert( plb );

	// specification code
	insert( new TLabel( TRect( 2, 1, 21, 2 ), "Specification Code", plb ));

	//Total number of pairs
	//Commented by:sdt:28082005:1315
	//insert( new TStaticText( TRect( 2, 6, 15, 7 ), "No. of Pairs" ));
	//TScrollBar *psbpair = new TScrollBar( TRect( 23, 6, 24, 7 ) );
	//plbpair = new TListBox( TRect( 16, 6, 23, 7 ), 1, psbpair );
	//insert( psbpair );

	//pcpair = new TConfigCollection(10,5);
	//int *ilstmbr = new int;
	//*ilstmbr = config.m_Noofpairs;
	//Commented by:sdt:28082008:1305
	//Start
	//ListObject *lsobj = new ListObject;
	//lsobj->listMember = config.m_Noofpairs;
	//pcpair->insert(lsobj);
	//plbpair->newList(pcpair);
	//end
	//insert( plbpair );
	//delete ilstmbr;
	//Pairs in a unit
	//insert( new TStaticText( TRect( 26, 6, 40, 7 ), "Pairs/Unit" ));
	//TScrollBar *psbunit = new TScrollBar( TRect( 48, 6, 49, 7 ) );
	//plbunit = new TListBox( TRect( 41, 6, 48, 7 ), 1, psbunit );
	//insert( psbunit );
	//pcunit = new TConfigCollection(10,5);
	//ilstmbr = new int;
	//*ilstmbr = config.m_Unitof;
	//lsobj->listMember = config.m_Unitof;
	//pcunit->insert(lsobj);
	//plbunit->newList(pcunit);
	//insert( plbunit );
	//delete ilstmbr;

	//Added by:sdt:30082005:1930
	//Start
	insert( new TStaticText( TRect( 2, 6, 15, 7 ), "No. of Pairs" ));
	TScrollBar *psbpair = new TScrollBar( TRect( 23, 6, 24, 7 ) );
	plbpair = new TSpeckListBox( TRect( 16, 6, 23, 7 ), 1, psbpair );
	insert( psbpair );
	pcpair = new TSpeckCollection(10,5);
	//SpeckObject *spobj = new SpeckObject;
	//spobj-> = config.m_Noofpairs;
	//pcpair->insert(lsobj);
	//plbpair->newList(pcpair);
	insert( plbpair );
	//Pairs in a unit
	insert( new TStaticText( TRect( 26, 6, 40, 7 ), "Pairs/Unit" ));
	TScrollBar *psbunit = new TScrollBar( TRect( 48, 6, 49, 7 ) );
	plbunit = new TSpeckListBox( TRect( 41, 6, 48, 7 ), 1, psbunit );
	insert( psbunit );
	pcunit = new TSpeckCollection(10,5);
	//*ilstmbr = config.m_Unitof;
	//lsobj->listMember = config.m_Unitof;
	//pcunit->insert(lsobj);
	//plbunit->newList(pcunit);
	insert( plbunit );
	//end:308082005:1930


	//cable type _ cable guage

	//cable sequence name
	//insert( new TStaticText( TRect( 27, 8, 40, 9 ), "Cable Sequence" ));
	//pseq = new TInputLine( TRect( 41, 8, 55, 9 ), 25 );
	//insert( pseq );

	// initial length marking
	insert( new TStaticText( TRect( 2, 8, 5, 9 ), "ILM" ));
	pilm = new TInputLine( TRect( 5, 8, 13, 9 ), 6 );
	insert( pilm );
	// final length marking
	insert( new TStaticText( TRect( 15, 8, 18, 9 ), "FLM" ));
	pflm = new TInputLine( TRect( 19, 8, 27, 9 ), 6 );
	insert( pflm );
	// length in meters
	insert( new TStaticText( TRect( 29, 8, 48, 9 ), "Length          mts" ));
	pl = new TInputLine( TRect( 36, 8, 44, 9 ), 6 );
	insert( pl );
	// temperature
	insert( new TStaticText( TRect( 2, 10, 19, 11 ), "Temp          øC" ));
	pt = new TInputLine( TRect( 7, 10, 15, 11 ), 6 );
	insert( pt );
	insert( new TStaticText( TRect( 21, 10, 37, 11 ), "Start Cond From" ));
	ps = new TInputLine( TRect( 38, 10, 44, 11 ), 4 );
	insert( ps );
	//pcu = new TInputLine( TRect( 2, 14, 20, 15 ), MAX_NAMELEN );
	//ptb = new TInputLine( TRect( 27, 14, 45, 15 ), MAX_NAMELEN );
	//insert( pcu );
	//insert( ptb );
	// User Entries
	BYTE count = 0;
	BYTE row = 11;
	BYTE factor = 0;
	for ( int i = 0; i < MAX_USERENTRIES; i++)
	{
		if ( config.m_UserTitles[count][0] != '\0' )
		{
			insert( new TStaticText( TRect( 2+factor*25, row,
				2+factor*17+MAX_USERENTRY_LEN, row+1), config.m_UserTitles[count] ));
			pUe[count] = new TInputLine( TRect(2+factor*25,
			row+1,2+factor*25+MAX_USERENTRY_LEN+3, row+2), MAX_USERENTRY_LEN+1 );
			insert( pUe[count] );
			count++,factor++;
		}
		if( factor == 2 )
		{
			factor = 0;
			row+=2;
		}
	}
	insert( new TButton( TRect( 34, 2, 44, 4 ), " ~O~K ", cmOK, bfDefault ));
	insert( new TButton( TRect( 34, 4, 45, 6 ), " ~C~ancel ", cmCancel, bfNormal ));

	setData( config );
}

void TConfig::getData( Config& config )
{
	Config tconfig;
	tconfig.speckNumber = ((SpeckObject*)pc->at(plb->focused))->fileNumber;
	strcpy( tconfig.speckName, ((SpeckObject*)pc->at(plb->focused))->speckName );

	//tconfig.m_BunchSize = (BYTE)atoi( pbs->data );
	//if ( tconfig.m_BunchSize <= 0 )
	//	tconfig.m_BunchSize = config.m_BunchSize;
	tconfig.m_Temp = atof( pt->data );
	if ( tconfig.m_Temp <= 0 )
		tconfig.m_Temp = config.m_Temp;
	tconfig.m_Ilm = atoi( pilm->data );
	tconfig.m_Flm = atoi( pflm->data );
	tconfig.m_Length = atoi( pl->data );
	tconfig.m_StartFrom = atoi( ps->data );
	if ( tconfig.m_StartFrom <= 0 || tconfig.m_StartFrom > MAX_CONDUCTORS )
		tconfig.m_StartFrom = 1;
	if ( tconfig.m_Length <= 0 )
		tconfig.m_Length = config.m_Length;
	//strncpy( tconfig.m_Customer, pcu->data, MAX_NAMELEN );
	//strncpy( tconfig.m_TestedBy, ptb->data, MAX_NAMELEN );

	BYTE count = 0;
	for ( int i = 0; i < MAX_USERENTRIES; i++ )
	{
		if ( config.m_UserTitles[count][0] != '\0' )
		{
			strncpy( tconfig.m_UserEntries[count], pUe[ count ]->data,
				MAX_USERENTRY_LEN );
			strncpy( tconfig.m_UserTitles[count], config.m_UserTitles[count],
				MAX_USERTITLE_LEN );
		}
		else
		{
			memset( tconfig.m_UserTitles[count], 0, MAX_USERTITLE_LEN );
			memset( tconfig.m_UserEntries[count], 0, MAX_USERENTRY_LEN );
		}
		count++;
	}
	tconfig.m_Noofpairs = atoi(((SpeckObject*)pcpair->at(plbpair->focused))->speckName);
	tconfig.m_Unitof = atoi(((SpeckObject*)pcunit->at(plbunit->focused))->speckName);
	config = tconfig;
}

void TConfig::setData( Config& config)
{
	//sprintf( pbs->data, "%d", config.m_BunchSize );
	//strncpy( pcu->data, config.m_Customer, MAX_NAMELEN );
	//strncpy( ptb->data, config.m_TestedBy, MAX_NAMELEN );

	sprintf( pilm->data, "%u", config.m_Ilm );
	sprintf( pflm->data, "%u", config.m_Flm );
	sprintf( pl->data, "%u", config.m_Length ); //SDT11122016:0730
	sprintf( ps->data, "%d", config.m_StartFrom );
	ftos( config.m_Temp, pt->data, 4, -1, 1 );
	// for user defined entries
	BYTE count = 0;
	for ( int i = 0; i < MAX_USERENTRIES; i++ )
		if ( config.m_UserTitles[count][0] != '\0' )
		{
			strncpy( pUe[ count ]->data, config.m_UserEntries[count],
				MAX_USERENTRY_LEN );
			count++;
		}

	i = -1;
	int l = pc->getCount();
	while( ++i < l )
		if ( strcmp( ((SpeckObject*)pc->at(i))->speckName, config.speckName ) == 0 )
			break;
	i = i<l?i:0;
	config.speckNumber = ((SpeckObject*)pc->at(i))->fileNumber;
	strcpy( config.speckName, ((SpeckObject*)pc->at(i))->speckName );
	plb->focused = i;
	plb->vScrollBar->value += plb->vScrollBar->arStep*i;
	plb->vScrollBar->drawView();
	//Code added for handling No. of pairs & Pairs in unit part
	readSpeck(speckData, config.speckNumber);
	strcpy(szSeqFileName, speckData.szCableSequence.FileName);

	setDataPairs();
	plbpair->newList(pcpair);
	plbpair->focused = 0;

	setDataUnits(CableSequence.NumberOfPairs[0]);
	plbunit->newList(pcunit);
	plbunit->focused = 0;

	//Code Shifted into functio setDataPairsAndUnits::sdt:300820052250
	/*readSpeck(speckData, config.speckNumber);
	strcpy(szSeqFileName, speckData.szCableSequence.FileName);
	GetCableCategaryList(speckData.szCableSequence.SeqName);

	for(i=0;CableSequence.NumberOfPairs[i]!=-1;i++)
	{
		SpeckObject *spobj = new SpeckObject;
		spobj->fileNumber = 0;
		spobj->speckName = (char *)malloc(sizeof(char)*7);//size of int
		sprintf(spobj->speckName,"%d",CableSequence.NumberOfPairs[i]);
		pcpair->insert( spobj );
	}
	plbpair->newList(pcpair);
	plbpair->focused = 0;
	GetCableUnitsList(CableSequence.NumberOfPairs[0]);
	for(i=0;CableSequence.TotalPairs.NumberOfUnits[i]!=-1;i++)
	{
		SpeckObject *spobj = new SpeckObject;
		spobj->fileNumber = 0;
		spobj->speckName = (char *)malloc(sizeof(char)*7);//size of int
		sprintf(spobj->speckName,"%d",CableSequence.TotalPairs.NumberOfUnits[i]);
		pcunit->insert( spobj );
	}
	plbunit->newList(pcunit);
	plbunit->focused = 0;*/

	strcpy(szOldSpeckName,((SpeckObject*)pc->at(plb->focused))->speckName);
	strcpy(szOldPairs,((SpeckObject*)pcpair->at(plbpair->focused))->speckName);
	strcpy(szOldUnits,((SpeckObject*)pcunit->at(plbunit->focused))->speckName);
}

void TConfig::setDataPairs(void)
{
	GetCableCategaryList(speckData.szCableSequence.SeqName);

	for(int i=0;CableSequence.NumberOfPairs[i]!=-1;i++)
	{
		SpeckObject *spobj = new SpeckObject;
		spobj->fileNumber = 0;
		spobj->speckName = (char *)malloc(sizeof(char)*7);//size of int
		sprintf(spobj->speckName,"%d",CableSequence.NumberOfPairs[i]);
		pcpair->insert( spobj );
	}

}
void TConfig::setDataUnits(int iNoOfPairs)
{
	GetCableUnitsList(iNoOfPairs);
	for(int i=0;CableSequence.TotalPairs.NumberOfUnits[i]!=-1;i++)
	{
		SpeckObject *spobj = new SpeckObject;
		spobj->fileNumber = 0;
		spobj->speckName = (char *)malloc(sizeof(char)*7);//size of int
		sprintf(spobj->speckName,"%d",CableSequence.TotalPairs.NumberOfUnits[i]);
		pcunit->insert( spobj );
	}

}

void TConfig::handleEvent( TEvent& ev )
{
	TDialog::handleEvent( ev );
	static unsigned int ilm;
	static unsigned int flm;

	if ( ev.what == evBroadcast )
	{
		if ( ev.message.command == cmReleasedFocus ||
				ev.message.command == cmReceivedFocus )
		{
			if ( ilm != atoi(pilm->data) || flm != atoi(pflm->data ) )
			{
				ilm = atoi( pilm->data );
				flm = atoi( pflm->data );

				sprintf( pl->data, "%u", flm - ilm );
				pl->drawView();

			}
			//clearEvent( ev );//Shifted at end of new code added
			//Added by:sdt:30082005:2235
			//For handing of No of Pairs Selection & Pair in unit.
			//Start
			else if(strcmp(szOldSpeckName,((SpeckObject*)pc->at(plb->focused))->speckName)!=0)
			{
				readSpeck(speckData, ((SpeckObject*)pc->at(plb->focused))->fileNumber);
				strcpy(szSeqFileName, speckData.szCableSequence.FileName);
				strcpy(szOldSpeckName,((SpeckObject*)pc->at(plb->focused))->speckName);
				pcpair->removeAll();
				FreeSequenceStructure(ALLPAIRS);
				setDataPairs();
				//plbpair->setData(pcpair);
				pcpair->pack();
				plbpair->focused = 0;
				plbpair->drawView();

				strcpy(szOldPairs,((SpeckObject*)pcpair->at(plbpair->focused))->speckName);
				pcunit->removeAll();
				setDataUnits(CableSequence.NumberOfPairs[0]);
				//plbunit->setData(pcpair);
				pcunit->pack();
				plbunit->focused = 0;
				plbunit->drawView();
				strcpy(szOldUnits,((SpeckObject*)pcunit->at(plbunit->focused))->speckName);

			}
			else if(strcmp(szOldPairs,((SpeckObject*)pcpair->at(plbpair->focused))->speckName)!=0)
			{
				strcpy(szOldPairs,((SpeckObject*)pcpair->at(plbpair->focused))->speckName);
				pcunit->removeAll();
				FreeSequenceStructure(PAIRSINUNIT);
				setDataUnits(atoi(((SpeckObject*)pcpair->at(plbpair->focused))->speckName));
				plbunit->focused = 0;
				plbunit->drawView();
				strcpy(szOldUnits,((SpeckObject*)pcunit->at(plbunit->focused))->speckName);
			}
			/*else if(ev.message.command == cmOK)
			{
				this->pcpair->freeAll();
				this->pcunit->freeAll();
				ev.what = evCommand;
				ev.message.command = cmOK;
				putEvent( ev );
			}
			else if(ev.message.command == cmCancel)
			{
				this->pcpair->freeAll();
				this->pcunit->freeAll();
				ev.what = evCommand;
				ev.message.command = cmCancel;
				putEvent( ev );
			}
			else*/

			clearEvent( ev );

		}
	}
}

TConfig::~TConfig()
{
	/*pc->freeAll();
	delete pc;
	pcpair->freeAll();
	delete pcpair;
	pcunit->freeAll();
	delete pcunit;*/
}

void TConfigCollection::freeItem( void *item )
{
	delete (ListObject *)item;
}

void *TConfigCollection::readItem( ipstream& is )
{
	ListObject *ptr = new ListObject;
	return ptr;
}

void TConfigCollection::writeItem( void *obj, opstream& os )
{
}

const char * const TConfigCollection::name = "TConfigCollection";

TStreamable *TConfigCollection::build()
{
	return new TConfigCollection( streamableInit );
}

TStreamableClass RFCollection1( TConfigCollection::name,
								   TConfigCollection::build,
								   __DELTA(TConfigCollection)
								 );
