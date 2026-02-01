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
char szListItemPair[7]; //Added by:sdt:07092005:2220
char szListItemUnit[7]; //Added by:sdt:07092005:2220
//Boolean         bSpeckChanged = False; //Added by:sdt:24112005:2005

void getConfigSetup(Config& config)
{

	//char	szOldSeqFile[13]; //Added by:sdt:24112005:2005
	TConfig *pConfig = new TConfig( TRect( 13, 1, 67, 23 ), "Configuration" );

	TScrollBar *psb = new TScrollBar( TRect( 32, 2, 33, 5 ) );
	pConfig->plb = new TSpeckListBox( TRect( 2, 2, 32, 5 ), 1, psb );
	pConfig->insert( psb );
	pConfig->pc = new TSpeckCollection(10,5);
	if(!findSpeckFiles(pConfig->pc))
	{
		SpeckObject *sd = new SpeckObject;
		sd->fileNumber = 1;
		sd->speckName = newStr( "temp" );
		pConfig->pc->insert( sd );
		pConfig->pc->atFree( 0 );
	}

	pConfig->plb->newList( pConfig->pc );
	pConfig->insert( pConfig->plb );

	// specification code
	pConfig->insert( new TLabel( TRect( 2, 1, 21, 2 ), "Specification Code", pConfig->plb ));

	pConfig->insert( new TButton( TRect( 2, 6, 16, 8 ), "No.Of ~P~airs", cmNoOfPairs, bfNormal )); //Modified by:sdt:01082006:2240
	pConfig->ppair = new TInputLine( TRect(17, 6, 23, 7), 6);
	pConfig->insert( pConfig->ppair );
	pConfig->ppair->setState(sfDisabled,True);

	pConfig->insert( new TButton( TRect( 26, 6, 40, 8 ), "Pairs/U~n~it", cmPairsPerUnit, bfNormal ));//Modified by:sdt:01082006:2240
	pConfig->punit = new TInputLine( TRect(41, 6, 48, 7), 6);
	pConfig->insert( pConfig->punit );
	pConfig->punit->setState(sfDisabled,True);

	//cable type _ cable guage
	pConfig->insert( new TStaticText( TRect( 2, 8, 12, 9 ), "Cable Type" ));
	pConfig->pcbltp = new TInputLine( TRect( 13, 8, 25, 9 ), 25 );
	pConfig->insert( pConfig->pcbltp );
	pConfig->pcbltp->setState(sfDisabled,True);

	//cable sequence name
	pConfig->insert( new TStaticText( TRect( 27, 8, 37, 9 ), "Cable Seq." ));
	pConfig->pseq = new TInputLine( TRect( 38, 8, 50, 9 ), 25 );
	pConfig->insert( pConfig->pseq );
	pConfig->pseq->setState(sfDisabled,True);

	// initial length marking
	pConfig->insert( new TStaticText( TRect( 2, 10, 5, 11 ), "ILM" ));
	pConfig->pilm = new TInputLine( TRect( 5, 10, 13, 11 ), 6 );
	pConfig->insert( pConfig->pilm );
	// final length marking
	pConfig->insert( new TStaticText( TRect( 15, 10, 18, 11 ), "FLM" ));
	pConfig->pflm = new TInputLine( TRect( 19, 10, 27, 11 ), 6 );
	pConfig->insert( pConfig->pflm );
	// length in meters
	pConfig->insert( new TStaticText( TRect( 29, 10, 48, 11 ), "Length          mts" ));
	pConfig->pl = new TInputLine( TRect( 36, 10, 44, 11 ), 6 );
	pConfig->insert( pConfig->pl );
	// temperature
	pConfig->insert( new TStaticText( TRect( 2, 12, 19, 13 ), "Temp          �C" ));
	pConfig->pt = new TInputLine( TRect( 7, 12, 15, 13 ), 6 );
	pConfig->insert( pConfig->pt );
	pConfig->insert( new TStaticText( TRect( 21, 12, 37, 13 ), "Start Cond From" ));
	pConfig->ps = new TInputLine( TRect( 38, 12, 44, 13 ), 4 );
	pConfig->insert( pConfig->ps );
	//pcu = new TInputLine( TRect( 2, 14, 20, 15 ), MAX_NAMELEN );
	//ptb = new TInputLine( TRect( 27, 14, 45, 15 ), MAX_NAMELEN );
	//insert( pcu );
	//insert( ptb );
	// User Entries
	BYTE count = 0;
	BYTE row = 13;
	BYTE factor = 0;
	for ( int i = 0; i < MAX_USERENTRIES; i++)
	{
		if ( config.m_UserTitles[count][0] != '\0' )
		{
			pConfig->insert( new TStaticText( TRect( 2+factor*25, row,
				2+factor*17+MAX_USERENTRY_LEN, row+1), config.m_UserTitles[count] ));
			pConfig->pUe[count] = new TInputLine( TRect(2+factor*25,
			row+1,2+factor*25+MAX_USERENTRY_LEN+3, row+2), MAX_USERENTRY_LEN+1 );
			pConfig->insert( pConfig->pUe[count] );
			count++,factor++;
		}
		if( factor == 2 )
		{
			factor = 0;
			row+=2;
		}
	}
	pConfig->insert( new TButton( TRect( 34, 2, 44, 4 ), " ~O~K ", cmOK, bfDefault ));
	pConfig->insert( new TButton( TRect( 34, 4, 45, 6 ), " ~C~ancel ", cmCancel, bfNormal ));

	//strcpy(szOldSeqFile,config.szCableSequence.FileName);//Added by:sdt:24112005:2005

	pConfig->setData(config);
	TProgram::deskTop->execView( pConfig );
	pConfig->getData(config);
	TDialog::destroy (pConfig);

	//Added by:sdt:24112005:2005
	//if(bSpeckChanged == True)
	//{
	//	strcpy(config.szCableSequence.FileName, speckData.szCableSequence.FileName);
	//}
	//else
	//{
	//	strcpy(config.szCableSequence.FileName, szOldSeqFile);
	//}


}



void TConfig::getData( Config& config )
{
	Config tconfig;
	tconfig.speckNumber = ((SpeckObject*)pc->at(plb->focused))->fileNumber;
	strcpy( tconfig.speckName, ((SpeckObject*)pc->at(plb->focused))->speckName );

	tconfig.m_Temp = atof( pt->data );
	if ( tconfig.m_Temp <= 0 )
		tconfig.m_Temp = config.m_Temp;
	tconfig.m_Ilm = atoi( pilm->data );
	tconfig.m_Flm = atoi( pflm->data );
	tconfig.m_Length = atoi( pl->data );
	tconfig.m_StartFrom = atoi( ps->data );
	if ( tconfig.m_StartFrom <= 0 || tconfig.m_StartFrom > MAX_CONDUCTORS )
		tconfig.m_StartFrom = 1;
	if ( tconfig.m_Length <= (unsigned int)0 )
		tconfig.m_Length = config.m_Length;

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
	//Uncommented by:sdt:02082006:2305
	tconfig.m_Noofpairs = atoi(ppair->data);
	//Commented by:sdt:02082006:2305
	//Modified by:sdt:04122005:2310 //Modified for showing No of pairs as quad
	//tconfig.m_Noofpairs = atoi(ppair->data)*2;
	tconfig.m_Unitof = atoi(punit->data);
	strcpy(tconfig.cabletype, pcbltp->data);
	strcpy(tconfig.szCableSequence.SeqName, pseq->data);
	strcpy( tconfig.m_PrintDate, config.m_PrintDate);
	strcpy( tconfig.m_StartTime, config.m_StartTime );
	strcpy( tconfig.m_StopTime, config.m_StopTime);
	strcpy( tconfig.m_TestDate, config.m_TestDate);
	//Commented by:sdt24112005:2010
	//Added by:sdt:23112005:1905
	//strcpy(tconfig.szCableSequence.FileName, config.szCableSequence.FileName);
	//Added by:sdt:24112005:0715
	//if(bSpeckChanged == True)
	//{
	//	strcpy(tconfig.szCableSequence.FileName, speckData.szCableSequence.FileName);
	//}
	//else
	//{
	//	strcpy(tconfig.szCableSequence.FileName, config.szCableSequence.FileName);
	//}
	config = tconfig;

	FILE *fcnf;
	if ((fcnf = fopen("config.bin", "wb")) != NULL)
	{
		fwrite(&config, sizeof(Config),1,fcnf);
	}
	if(fcnf){fclose(fcnf);}


}

void TConfig::setData( Config& config)
{
	/*FILE *fcnf;
	if ((fcnf = fopen("config.bin", "rb")) != NULL)
	{
		fread(&config, sizeof(Config),1,fcnf);
	}
	if(fcnf){fclose(fcnf);}*/

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

	int i ;
	i = -1;
	int l = pc->getCount();
	while( ++i < l )
		if ( strcmp( ((SpeckObject*)pc->at(i))->speckName, config.speckName ) == 0 )
			break;
	i = i<l?i:0;
	config.speckNumber = ((SpeckObject*)pc->at(i))->fileNumber;
	strcpy( config.speckName, ((SpeckObject*)pc->at(i))->speckName );
	plb->focused = i;     //TScrollBar
	plb->vScrollBar->value += plb->vScrollBar->arStep*i;
	plb->vScrollBar->drawView();
	//Code added for handling No. of pairs & Pairs in unit part
	readSpeck(speckData, config.speckNumber);
	strcpy(pcbltp->data, speckData.cabletype);//Added by:sdt:07092005:1900
	strcpy(pseq->data, speckData.szCableSequence.SeqName);//Added by:sdt:07092005:1900

	strcpy(szSeqFileName, speckData.szCableSequence.FileName);
	//Commented by:sdt:11112006:2100
	/*//Added by:sdt:02092005:2030
	FreeSequenceStructure(ALLPAIRS);
	GetCableCategaryList(speckData.szCableSequence.SeqName);
	//Uncommented by:sdt:02082006:2200 //To represent No. of Pairs in Pair format.
	sprintf(ppair->data,"%d",CableSequence.NumberOfPairs[0]);
	//Modified by:sdt:04122005:2305 //Modified for showing No of pairs as quad
	//Commented by:sdt:02082006:2200
	//sprintf(ppair->data,"%d",(CableSequence.NumberOfPairs[0]/2));
	sprintf(szListItemPair,"%d",(CableSequence.NumberOfPairs[0]));///2));//Modified by:sdt:02082006:2200
	GetCableUnitsList(CableSequence.NumberOfPairs[0]);
	sprintf(punit->data,"%d",CableSequence.TotalPairs.NumberOfUnits[0]);
	sprintf(szListItemUnit,"%d",CableSequence.TotalPairs.NumberOfUnits[0]);
	strcpy(szOldSpeckName,((SpeckObject*)pc->at(plb->focused))->speckName);
	strcpy(szOldPairs,ppalir->data);
	strcpy(szOldUnits,punit->data);*/

	//Start//Added by:sdt:11112006:2100
	//Below changes to avoid resetting of config values when Config Dialog
	//Box is opened.
	sprintf(ppair->data,"%d",config.m_Noofpairs);
	sprintf(szListItemPair,"%d",config.m_Noofpairs);
	sprintf(punit->data,"%d",config.m_Unitof);
	sprintf(szListItemUnit,"%d",config.m_Unitof);
	strcpy(szOldSpeckName,((SpeckObject*)pc->at(plb->focused))->speckName);
	strcpy(szOldPairs,ppair->data);
	strcpy(szOldUnits,punit->data);
	//End//Added by:sdt:11112006:2100
}


void TConfig::handleEvent( TEvent& ev )
{
	TDialog::handleEvent( ev );
	static unsigned int ilm;
	static unsigned int flm;


	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmNoOfPairs)
		{
			selectNoOfPairs(CableSequence.NumberOfPairs);
			strcpy(ppair->data, szListItemPair);
			ppair->drawView();
			return;
		}
		if(ev.message.command == cmPairsPerUnit)
		{
			selectPairsPerUnit(CableSequence.TotalPairs.NumberOfUnits);
			strcpy(punit->data, szListItemUnit);
			punit->drawView();
			return;
		}

	}

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
				//bSpeckChanged = True;//Commented by:sdt:24112005:2010
				readSpeck(speckData, ((SpeckObject*)pc->at(plb->focused))->fileNumber);
				strcpy(pcbltp->data, speckData.cabletype);//Added by:sdt:07092005:1900
				strcpy(pseq->data, speckData.szCableSequence.SeqName);//Added by:sdt:07092005:1900
				pcbltp->drawView();//Added by:sdt:07092005:1900
				pseq->drawView();  //Added by:sdt:07092005:1900
				strcpy(szSeqFileName, speckData.szCableSequence.FileName);
				strcpy(szOldSpeckName,((SpeckObject*)pc->at(plb->focused))->speckName);
				//strcpy(config.szCableSequence.FileName, speckData.szCableSequence.FileName);//Added by:sdt:24112005:0705
				FreeSequenceStructure(ALLPAIRS);
				GetCableCategaryList(speckData.szCableSequence.SeqName);
				//Uncommneted by:sdt:02082006:2230 //For PIJF Cable
				sprintf(ppair->data,"%d",CableSequence.NumberOfPairs[0]);
				sprintf(szListItemPair,"%d",CableSequence.NumberOfPairs[0]);
				//Commented by:sdt:02082006:2230 //For PIJF Cable
				//Modified by:sdt:04122005:2320 //For Quad Cable
				//sprintf(ppair->data,"%d",(CableSequence.NumberOfPairs[0]/2));
				//sprintf(szListItemPair,"%d",(CableSequence.NumberOfPairs[0]/2));
				GetCableUnitsList(CableSequence.NumberOfPairs[0]);
				sprintf(punit->data,"%d",CableSequence.TotalPairs.NumberOfUnits[0]);
				sprintf(szListItemUnit,"%d",CableSequence.TotalPairs.NumberOfUnits[0]);
				ppair->drawView();
				punit->drawView();
				strcpy(szOldPairs,ppair->data);
				strcpy(szOldUnits,punit->data);

			}
			else if(strcmp(szOldPairs,ppair->data)!=0)
			{
				strcpy(szOldPairs,ppair->data);
				FreeSequenceStructure(PAIRSINUNIT);
				GetCableUnitsList(atoi(ppair->data));
				//Modified by:sdt:04122005:2320
				//Commented by:sdt:11112006:2055 // For pair software (*2) not required.
				//GetCableUnitsList(atoi(ppair->data)*2);
				sprintf(punit->data,"%d",CableSequence.TotalPairs.NumberOfUnits[0]);
				sprintf(szListItemUnit,"%d",CableSequence.TotalPairs.NumberOfUnits[0]);
				punit->drawView();
				strcpy(szOldUnits,punit->data);
			}

			clearEvent( ev );

		}

	}
}


void TConfig::selectNoOfPairs(int *iListEntries)
{
	char szTemp[7];
	TPairDialog *pprdlg = new TPairDialog( TRect( 13, 5, 37, 12 ), "Total Pairs" );//Added by:sdt:03082005:0025
	TScrollBar *psb = new TScrollBar( TRect( 8, 2, 9, 5 ) );
	pprdlg->insert( psb );
	pprdlg->plbpair = new TSpeckListBox( TRect( 2, 2, 8, 5 ), 1, psb );
	pprdlg->pcpair = new TSpeckCollection(10,5);
	for(int i=0; iListEntries[i]!=-1;i++)
	{
		SpeckObject *spobj = new SpeckObject;
		spobj->fileNumber = 0;
		//Uncommented by:sdt:02082006:2230
		sprintf(szTemp,"%d",iListEntries[i]);
		//Commented by:sdt:02082006:2230
		//Modified by:sdt:04122005:2315 //For showing number pairs in terms of Quad
		//sprintf(szTemp,"%d",(iListEntries[i]/2));
		spobj->speckName = newStr(szTemp);
		pprdlg->pcpair->insert( spobj );
	}
	pprdlg->plbpair->newList( pprdlg->pcpair );
	pprdlg->insert( pprdlg->plbpair );

	pprdlg->insert( new TButton( TRect( 10, 2, 21, 4 ), " ~O~K ", cmOKPair, bfDefault ));
	pprdlg->insert( new TButton( TRect( 10, 4, 21, 6 ), " ~C~ancel ", cmCancel, bfNormal ));
	pprdlg->plbpair->focused = 0;

	TProgram::deskTop->execView( pprdlg );
	TDialog::destroy (pprdlg);
}

void TConfig::selectPairsPerUnit(int *iListEntries)
{
	char szTemp[7];
	TUnitDialog *putdlg = new TUnitDialog( TRect( 26, 5, 50, 12 ), "Pairs/Unit" );//Added by:sdt:03082005:0025
	TScrollBar *psb = new TScrollBar( TRect( 8, 2, 9, 5 ) );
	putdlg->plbunit = new TSpeckListBox( TRect( 2, 2, 8, 5 ), 1, psb );
	putdlg->insert( psb );
	putdlg->pcunit = new TSpeckCollection(10,5);
	for(int i=0; iListEntries[i]!=-1;i++)
	{
		SpeckObject *spobj = new SpeckObject;
		spobj->fileNumber = 0;
		sprintf(szTemp,"%d",iListEntries[i]);
		spobj->speckName = newStr(szTemp);
		putdlg->pcunit->insert( spobj );
	}
	putdlg->plbunit->newList( putdlg->pcunit );
	putdlg->insert( putdlg->plbunit );

	putdlg->insert( new TButton( TRect( 10, 2, 21, 4 ), " ~O~K ", cmOKUnit, bfDefault ));
	putdlg->insert( new TButton( TRect( 10, 4, 21, 6 ), " ~C~ancel ", cmCancel, bfNormal ));
	putdlg->plbunit->focused = 0;

	TProgram::deskTop->execView( putdlg );
	TDialog::destroy ( putdlg );
}

TConfig::~TConfig()
{
	pc->freeAll();
	delete pc;
}

TPairDialog::~TPairDialog()
{
	pcpair->freeAll();
	delete pcpair;
}

void TPairDialog::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmOKPair)
		{
			plbpair->getText(szListItemPair, plbpair->focused,6);

			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );

		}
	}
}



TUnitDialog::~TUnitDialog()
{
	pcunit->freeAll();
	delete pcunit;
}

void TUnitDialog::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmOKUnit)
		{
			plbunit->getText(szListItemUnit, plbunit->focused,6);

			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );

		}
	}
}




