#include <time.h>
#include <alloc.h>
#include <bios.h>
#include <conio.h>
#include <ctype.h>
#include "stdlib.h"
#include "math.h"
#include <io.h>
#include <process.h>

#include "logo.h"
#include "cts.h"
#include "ct.h"
#include "share.h"
#include "run.h"
//#include "HCCts.h"//Commented by:sdt:25072005:2330

#include "utils.h"//Commented by:sdt:25072005:2330
#include "gadgets.h"

//#include "views.h"

#if !defined( __SPECK_H )
   #include "speck.h"
#endif
#include "cconfig.h"
#include "fileview.h"
//#include "param.h"//Commented by:sdt:25072005:2330
#include "prodini.h"
#include "key.h"
#include "gblvar.h"
#include "funproto.h"

extern int g_argc;
extern char **g_argv;

//int _argc = 0;
//char **_argv = nullptr;

//Uncommented by:sdt:04122005:2355
/*#ifndef DEBUG
void 		interrupt (*oldKeyInt)(__CPPARGS);
void interrupt	resetKeyInt(__CPPARGS)
{
	unsigned char key;
	key = inportb( 0x60 );

	if ( key == 65 )		// for F7
	{
		//::SystemStatus = STATUS_RESET;
		outportb( 0x20, 0x20 );
		cancelTimer();

		//timerFlag = 1;//Commented by:sdt:04122005:2355
		//return;
	}
	oldKeyInt();
}
#endif*/



TCts::TCts() :
	TProgInit( 	TCts::initStatusLine,
				TCts::initMenuBar,
				TCts::initDeskTop
			 )
{
	TEvent ev;

	TRect r = getExtent();                            // Create the heap view.
	r.a.x = r.b.x - 13;     r.a.y = r.b.y - 1;
	heap = new THeapView( r );
	insert(heap);

	// display version and copyright information
	// Version Number and Serial Number
	ev.what = evCommand;
	ev.message.command = cmEntry;
	putEvent( ev );
    draw();
}



void TCts::draw()
{
	TView::draw();
}

TCts::~TCts()
{
	close( ParamCtsHandle );
	delete prmarray;
	freeFreqStruct();
	RemoveIniFiles();
}

TMenuBar *TCts::initMenuBar( TRect r )
{
	r.b.y = r.a.y + 1;

	return new TMenuBar( r,

		// FILE Menu
		*new TSubMenu( "~F~ile", kbAltF, hcFileMenu )
			+ *new TMenuItem( "~N~ew", cmNewDatFile, kbCtrlN, hcFileNew, "Ctrl+N" )
			+ *new TMenuItem( "~O~pen", cmOpenDatFile, kbF3, hcFileOpen, "F3" )
			+ *new TMenuItem( "~S~ave", cmSaveDatFile, kbCtrlS, hcFileSave, "Ctrl+S" )
			+ *new TMenuItem( "C~l~ose", cmClose, kbAltF3, hcFileSave, "Alt+F3" )
			+ newLine()
			+ *new TMenuItem( "E~x~it", cmExit, kbAltX, hcFileExit, "Alt+X" )    //cmQuit to kbAltX :modified by:sdt:17122016:1950

		// RUN Menu
		+ *new TSubMenu( "~R~un", kbAltR, hcRunMenu )
			+ *new TMenuItem( "~A~ll", cmRunAll, kbF4, hcRunAll, "F4" )
			+ *new TMenuItem( "S~e~lected", cmRunSelected, kbF5, hcRunSelected, "F5" )

		// SETUP Menu
		+ *new TSubMenu( "Set~u~p", kbAltU, hcSetupMenu )
			//+ *new TMenuItem( "~T~est", kbNoKey, kbCtrlT,hcTest,"Ctrl+T" )//Commented by:sdt:26072005:0041
			+ *new TMenuItem( "~R~un", cmRunSetup, kbCtrlR, hcSetupRun, "Ctrl+R" )
			+ *new TMenuItem( "Confi~g~", cmConfigSetup, kbCtrlG, hcSetupConfig, "Ctrl+G" )
			+ *new TMenuItem( "R~e~port", cmReportSetup, kbCtrlE, hcSetupReport, "Ctrl+E" )

		// SPECKS Menu
		+ *new TSubMenu( "Spec~k~s", kbAltK, hcSpecksMenu )
			+ *new TMenuItem( "~M~anagement", cmManagement, kbAltM, hcManagement, "Alt+M" )

		// REPORT Menu
		+ *new TSubMenu( "Re~p~ort", kbAltP, hcReportMenu )
			+ *new TMenuItem( "~V~iew", cmViewReport, kbAltV, hcReportView, "Alt+V" )
			+ *new TMenuItem( "~P~rint", cmPrintReport, kbCtrlP, hcReportPrint, "Ctrl+P" )

		// UTILS Menu
		+ *new TSubMenu( "U~t~ils", kbAltU, hcUtilsMenu )
			+ *new TMenuItem( "~R~eset", cmResetSystem, kbF7, hcUtilsReset, "F7" )
			+ *new TMenuItem( "View Ca~l~ibration", cmCalibrate, kbAltL, hcUtilsCal, "Alt+L" )
			+ *new TMenuItem( "Prin~t~ Calibration", cmPrintCalibrate, kbAltT, hcUtilsPrintCal, "Alt+T" )
			+ newLine()
			+ *new TMenuItem( "~P~ause", cmPause, kbF8, hcUtilsPause, "F8" )
			+ *new TMenuItem( "R~e~sume", cmResume, kbF9, hcUtilsRestart, "F9" )
			+ newLine()
			+ *new TMenuItem( "~D~isplay Fomulae", cmFormulae, kbNoKey, hcFormulae )
			+ *new TMenuItem( "Pr~i~nt Fomulae", cmPrintFormulae, kbNoKey, hcPrintFormulae )
			+ newLine()
			+ *new TMenuItem( "Pi~n~ Replace", cmPinReplace, kbNoKey, hcPinReplace )   //Added by:sdt:26042015:1700

		// HELP Menu
		+ *new TSubMenu( "~H~elp", kbAltH, hcHelpMenu )
			+ *new TMenuItem( "~C~ontents", cmContents, kbNoKey, hcHelpContents )
			+ newLine()
			+ *new TMenuItem( "~A~bout", cmAbout, kbNoKey, hcHelpAbout )
		);
}

TStatusLine *TCts::initStatusLine( TRect r )
{
	r.a.y = r.b.y - 1;

	return new TCtsStatusLine( r,
		* new TStatusDef( 0, 0 )
			+ *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmExit ) //modified by:sdt:17122016:1950
			+ *new TStatusItem( "~F7~ Reset", kbF7, cmResetSystem )
			+ *new TStatusItem( 0, kbF10, cmMenu )
		// FILE
		+ *new TStatusDef( 0, 0xffff )
			+ *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmExit )   //cmQuit //modified by:sdt:17122016:1950
			+ *new TStatusItem( "~F7~ Reset", kbF7, cmResetSystem )
			+ *new TStatusItem( 0, 0, hcNoContext )
		);
}

TCtsDeskTop::TCtsDeskTop(TRect& r) : TDeskTop( r ),
						TDeskInit( initBackground )
{
}


TDeskTop *TCts::initDeskTop( TRect r )
{
	r.a.y++;
	r.b.y--;
    return ( new TCtsDeskTop( r ) );
}

TBackground* TCtsDeskTop::initBackground( TRect r )
{
	return new TCtsBackground( r, 117 );
}

void TCtsDeskTop::draw()
{
	/*TDrawBuffer b;
	ushort c = getColor( 1 );

	for( int i = 0; i < 22; i++ )
	{
	   b.moveStr( 0,"���������������������������������������������������������������������������������", c );
	   writeLine( 0, i, 80, 1, b );
	}
	c = getColor( 1 );
	b.moveStr( 0,"             ePoint Technologies                                                 ", c );
	writeLine( 0, i, 80, 1, b );*/
	//Modified by:sdt:04012006:2210
	//For drawing desktop as ePT
	TDrawBuffer b;
    ushort c = getColor( 1 );

	for( int i = 0; i <= 23; i++ )
	{
	   b.moveStr( 0, logo[i], c );
	   writeLine( 0, i, strlen(logo[i]), 1, b );
	}


}

TCtsBackground::TCtsBackground( TRect& r, char pattern ) :
			TBackground( r, pattern )
{
}

void TCtsBackground::draw()
{
	/*TDrawBuffer b;
	ushort c = getColor( 1 );

	for( int i = 0; i < 22; i++ )
	{
	   b.moveStr( 0,"���������������������������������������������������������������������������������", c );
	   writeLine( 0, i, 80, 1, b );
	}
	c = getColor( 1 );
	b.moveStr( 0,"             ePoint Technologies                                                 ", c );
	writeLine( 0, i, 80, 1, b );*/
	//Modified by:sdt:04012006:2210
	//For drawing background as ePT
	// TDrawBuffer b;
	// ushort c = getColor( 1 );

	// for(int i= 0; i < size.y; i++)
	// {
	//    for( int j= 0; j < size.x; j++)
	//    {
	// 	  b.moveChar( j, logo[i][j], c, 1 );
	//    }
	//    writeLine( 0, i, size.x, 1, b );
	// }
	static constexpr int LOGO_H = (int)(sizeof(logo) / sizeof(logo[0]));

	TDrawBuffer b;
	ushort c = getColor(1);

	for (int i = 0; i < size.y; ++i)
	{
		const char *row = (i < LOGO_H) ? logo[i] : nullptr;

		int rowLen = row ? (int)strlen(row) : 0;

		for (int j = 0; j < size.x; ++j)
		{
			char ch = (row && j < rowLen) ? row[j] : ' ';
			b.moveChar(j, ch, c, 1);
		}

		writeLine(0, i, size.x, 1, b);
	}

}

void TCts::handleEvent( TEvent& ev )
{
	char path[80];
	TApplication::handleEvent( ev );
	this->heap->update();
	if ( ev.what == evKeyboard )
	{
		switch( ev.keyDown.charScan.charCode )
		{
        case kbF7:
			ev.what = evCommand;
			ev.message.command = cmResetSystem;
			putEvent( ev );
			break;

		case kbCtrlP:		// cmPrintReport
			ev.what = evCommand;
			ev.message.command = cmPrintReport;
			putEvent( ev );
			break;
		case kbCtrlN:		// cmNewDatFile
			ev.what = evCommand;
			ev.message.command = cmNewDatFile;
			putEvent( ev );
			break;
		case kbCtrlS:		// cmSaveDatFile
			ev.what = evCommand;
			ev.message.command = cmSaveDatFile;
			putEvent( ev );
			break;
		case kbCtrlG:
			ev.what = evCommand;
			ev.message.command = cmConfigSetup;
			putEvent( ev );
			break;
		case kbCtrlE:
			ev.what = evCommand;
			ev.message.command = cmReportSetup;
			putEvent( ev );
			break;
		case kbCtrlR:
			ev.what = evCommand;
			ev.message.command = cmRunSetup;
			putEvent( ev );
			break;
	  }

	 clearEvent( ev );
   }
unsigned long ulMinHeapMem = 0;

	if ( ev.what == evCommand )
	{
		switch( ev.message.command )
		{
		case cmResetSystem:
			ResetSystem (); //Added by:sdt:08112005:1950
			//Commented by:sdt:25072005:2355
			/*ProcessTestOver();
			if ( Fileloaded == FALSE )
				Cal_Testtime();

			initializeDIOcard();
			runstate = IDLE;
			DischargePrevCond();
			pCtPair->nTest = 0;
			paramchanged = 1;
			currentreadingnumber = 1;*/
		   //resetparamvalues();
		  //deskTop->insert(new TBackground(TRect(1,0,79,22),'�'));//Commented by:sdt:25112005:2230
		  //clearstatus();
			break;
		case cmNewDatFile:

			enableCommand( cmOK );
			disableCommand( cmSaveDatFile );
			enableCommand( cmRunSelected );
			enableCommand( cmRunAll );
			//::SystemStatus = STATUS_OK;
			//info.resume = FALSE;
			char szMinHeapMemory[12];      //Added by:sdt:12122016:2200
			ulMinHeapMem=0;  //Added by:sdt:12122016:2200
			sprintf(path,"%s:\\cts.ini",cRamDrive);//Added by:sdt:12122016:2200
			GetPrivateProfileString ("BASIC_INFO","MINIMUM_HEAP_MEMORY",szMinHeapMemory,"0",path);//Added by:sdt:12122016:2200
			ulMinHeapMem = atol(szMinHeapMemory);//Added by:sdt:12122016:2200
			if(this->heap->heapSize() > ulMinHeapMem) //Added by:sdt:12122016:2200
				newDatFile();
			else
			{
				nExit_status=1;//Added by:sdt:17122016:1850

				ushort helpCtx = TProgram::deskTop->helpCtx; //Added by:sdt:17122016:2015
				TProgram::deskTop->helpCtx = hcExitInsuffMem; //Added by:sdt:17122016:2015
				TProgram::statusLine->update();               //Added by:sdt:17122016:2015
				TProgram::deskTop->helpCtx = helpCtx;         //Added by:sdt:17122016:2015

				ev.what = evCommand;                 //Added by:sdt:12122016:2200
				ev.message.command = cmExit;         //Added by:sdt:12122016:2200
				putEvent(ev);                        //Added by:sdt:12122016:2200
			}
			//if ( fileStatus == Processed || fileStatus == InProcess )
			//{
			//	if ( messageBox( "Do you want to save the file",
			//		mfYesButton|mfNoButton ) == cmYes )
			//	{
			//		fileSave();
			//	}
			//}
			//lg.reOpen( "Hvir.Log", RDWR );
			//readLogAll();
			//if ( fileStatus != New )
			//{
			//	fileNumber++;
			//	lg.updateFile( SV_FILENO, &fileNumber );
			//}
			//pHeap->showFileNo( fileNumber );
			//fileStatus = New;
			//updateLogAll();

			break;
		case cmSaveDatFile:
			if (fs == fs_executed || fs == fs_notsaved || fs == fs_inprocess)
			{
				SaveFile();
				char szString[80];
				sprintf( szString, "File Saved as %s\\%s", DataDirectory, nConfig.szFileName );//Added:jj:18012001:1045
				messageBox(szString,mfInformation|mfOKButton);

				disableCommand( cmRunSelected );
				disableCommand( cmRunAll );
			}
			else if ( fs == fs_saved )
				messageBox( "File already saved", mfInformation|mfOKButton );
			else if( fs == fs_new )
				messageBox( "No File available !!!",mfInformation|mfOKButton );
			else if( fs == fs_bad)
				messageBox( "This File Cannot be Saved !!!",mfInformation|mfOKButton );
			else if ( fs == fs_error )
				messageBox( "Error in File !!!",mfInformation|mfOKButton );
			break;
		case cmOpenDatFile:
			fileOpen();
			break;
		case cmViewReport:
			if(!RuncountExpiryFlag)//condition added by:sdt:05012006:2020
			{
				//condition added by:sdt:26112005:1945
				if (fs == fs_new || fs == fs_error)
				{
					messageBox( "Select Appropriate File First",mfInformation|mfOKButton );
				}
				else
				{
					createReport(); //Added by:sdt:08102005:1450
					//Added by:sdt:07122005:0945//start //for stauts info for report
					//ushort helpCtx = TProgram::deskTop->helpCtx;
					//TProgram::deskTop->helpCtx = hcViewReport;
					//TProgram::statusLine->update();
					//TProgram::deskTop->helpCtx = helpCtx;
					//Added by:sdt:07122005:0945 //End//for stauts info for report
					TCommandSet temp = deskTop->curCommandSet;
					disableCommands( temp );
					disableCommand( cmClose );

					sprintf(path,"%s:\\sat.txt",cRamDrive);
					spawnlp(P_WAIT,"view.com","view.com",path,NULL);
					//Commented by:sdt:29072006:2145:start
					//TView *w= validView( new TFileWindow(path) );
					//if ( w != 0 )
					//	TProgram::deskTop->insert( w );
					//Commented by:sdt:29072006:2145:end
					::remove( "san.txt" );
					enableCommands( temp );
					redraw(); //Added by:sdt:29072006:2155

				}
			}
			break;

		case cmPrintReport:
			if(!RuncountExpiryFlag)//condition added by:sdt:05012006:2020
			{
				//condition added by:sdt:26112005:1945
				if (fs == fs_new || fs == fs_error)
				{
					messageBox( "Select Appropriate File First",mfInformation|mfOKButton );
				}
				else
				{
					printReport();
				}
			}
			break;
		case cmEntry:
			copyInisToRamdrive(); //Added by:sdt:25072005:1955
			//Added by:sdt:17122016:1900:start
			FILE *fexit;
			if ((fexit = fopen("exit.bin", "rb")) != NULL)
			{
				fread(&nExit_status, sizeof(int),1,fexit);

			}
			if(fexit){fclose(fexit);}
			//Added by:sdt:17122016:1900:end
			if(nExit_status==0)    //Condition added by:sdt:17122016:1905
			{
				aboutDlgBox();
			}
			else
				nExit_status=0; //Condition added by:sdt:17122016:1905

			memset( &currentUser, 0, sizeof( currentUser ) );
			if ( getPassword( currentUser ) == False )
			{
				//accessMsg( "  Violation: CTS Access Denied ");//Commented by:sdt:26072005:0041
				//::remove( "cts.ini" );//Commented by:sdt:04012006:2230
				//It was irrelevent to delete CTS.INI from current directory
				//as it is not holding passwords of entry level.
				exit( 0 );
			}
			//Added by:sdt:08122005:1410
			//For disabling Speck management for operator level.
			if(currentUser.userType==U_OPERATOR)
			{
				disableCommand( cmManagement );
				disableCommand( cmFormulae ); //Added by:sdt:11102006:2155
				disableCommand( cmPrintFormulae ); //Added by:sdt:11102006:2155
				disableCommand( cmPrintCalibrate ); //Added by:sdt:06112006:2210

			}
			ev.what = evCommand;
			ev.message.command = cmInitSystem;
			putEvent(ev);
			break;
		case cmAbout:
			aboutDlgBox();
			break;
		//Added by:sdt:26062006:2300
		case cmFormulae:
			formulaeDlgBox();
			break;
		//Added by:sdt:29062006:2320
		case cmPrintFormulae:
			nDC=PRINTER;
			if ((fptr_Report = fopen ("PRN","w"))==NULL)//Added:jj:31012001
			{
				messageBox( "Cannot Open Printer...",mfInformation|mfOKButton );
				return ;
			}
			PrintFormulae(); //Modified by:sdt:28062001
			EndReport();
			break;

		case cmPinReplace:
			PinReplacement(); //Added by:sdt:26042015:1700
			break;


		case cmManagement:
			speckManagement();
			//Commented by:sdt:08122005:1420
			//Method for restricting Operator from editing speck is modified.
			//Now Speck management command is disabled for Operator Level.
			//Condition added by:sdt:08122005:1240
			//Speck management is accessible only to SuperUser & ePointEngg.
			//if((currentUser.userType==U_SIVAENGG)||(currentUser.userType==U_SUPERUSER))
			//else
			//	messageBox( "Access Denied!!!", mfInformation|mfOKButton );
			break;
		case cmConfigSetup:
			getConfigSetup(config);
			break;
			/*TConfig *pConfig = new TConfig();
			pConfig->buildConfig( config );
			if ( deskTop->execView( pConfig ) == cmOK )
			{
				disableCommand( cmResume );
				//info.resume = False;
				pConfig->getData( config );
				readSpeck( speckData, config.speckNumber );
				//if ( config.m_StartFrom + speckData.m_NoofConds - 1 >= MAX_CONDUCTORS )
				//Condition modified by:sdt:26082005:1530
				if ( config.m_StartFrom + config.m_Noofpairs - 1 >= MAX_CONDUCTORS )
					config.m_StartFrom = 1;
			}
			FreeSequenceStructure(ALLPAIRS);
			pConfig->pc->freeAll();
			delete pConfig->pc;
			destroy( pConfig->plb );
			//pConfig->pcpair->freeAll();
			delete pConfig->pcpair;
			//pConfig->pcunit->freeAll();
			delete pConfig->pcunit;
			destroy( pConfig->plbpair );
			destroy( pConfig->plbunit );
			destroy( pConfig );
			break;*/
			//Commented by:sdt:26072005:0000
			/*TConfig *pConfig = new TConfig();
			pConfig->buildConfig( );       // Set Data
			strcpy(pConfig->pcs->data,config.cable_size );
		  sprintf( pConfig->pl->data, "%0.2f", config.length );
		    sprintf( pConfig->pnop->data, "%d", config.numberofpairs);
			sprintf( pConfig->ppiu->data, "%d", config.unitof);
		    strcpy(pConfig->pguage->data,config.Specks.guage);
		    ftos( config.roomtemp, pConfig->pt->data, 3, -1, 1 );
		  strcpy(pConfig->pdno->data,config.drumnumber );
			strcpy(pConfig->pidno->data,config.rotation  );
		    strcpy(pConfig->psuno->data,config.SUnitNo );
		    strcpy(pConfig->polno->data,config.orderno  );
			if ( deskTop->execView( pConfig ) == cmOK )
			{
				disableCommand( cmResume );
				readSpeck(config.Specks, pConfig->plb->focused+1);
			 itoa(pConfig->plb->focused+1,config.FileName,2);
				strcpy(config.cable_size, pConfig->pcs->data ); // Get Data
				config.length = atof( pConfig->pl->data );
				config.numberofpairs = atoi( pConfig->pnop->data );
				config.roomtemp = atof( pConfig->pt->data );
				strcat(pConfig->pilm->data,"-");
				strcat(pConfig->pilm->data,pConfig->pflm->data);
			    strcpy(config.IlmFlm,pConfig->pilm->data);
				strcpy(config.drumnumber,pConfig->pdno->data );
			    strcpy(config.rotation, pConfig->pidno->data );
			    strcpy(config.SUnitNo, pConfig->psuno->data );
				strcpy(config.orderno,  pConfig->polno->data );
			 if(config.Specks.Type == 'Q')
			   {
					config.unitof =  2 * config.numberofpairs;
				Quad = TRUE;
			   }
			 if(config.Specks.Type == 'P')
			   {
				 Quad = FALSE;
				 config.unitof = atoi(pConfig->ppiu->data);
			   }
			 pconfig = &config;
			 CtPair = *pCtPair;
			}
			destroy( pConfig );*/
		case cmIrSetup:

			break;
		case cmHvSetup:

			break;
		case cmRunSetup:
			getRunSetup();
			break;
		case cmReportSetup:
			getReportSetup();
			break;
		case cmInitSystem:
			 //Commented by:sdt:25072005:1950
			 InitSystem();
			 /*Atn_Freq_ReadFromIni();
			 ReadSetUp();
			 GetAverage();
			 InitialiseHF_Param();
			 GetAtnVarOffs ();
			 pconfig = &config;
			 ProcessCFGNEW();
				InitialiseAutoCal();
			 SetParameterArray ();
			 SetDefaultParamList();
			    SetByPassedPinsPos(&PinsPos);
				config.FixtureNo = 0;
			 pconfig = &config;*/
			break;
		case cmResume:

			break;
		case cmExit:
			exitSystem();
			//Commented by:sdt:26072005:0001
			/*initializeDIOcard ();           //make all control lines inactive
			farfree( config.readingsarray);
			if ( nNoofSpans > 0 )
				farfree( AtnVarOff );*/
		break;
		case cmRunAll:
			 //Condition added by:sdt:08122005:1130
			 //Condition added for saved files should not be tested again.
			 if ( fs == fs_saved )
				messageBox( "File already saved. Select New File for testing", mfInformation|mfOKButton );
			 else
			 {
				enableCommand( cmSaveDatFile );
				readSpeck(speckData,config.speckNumber); //Added by:sdt:24112005:2010
				config.szCableSequence = speckData.szCableSequence;//Added by:sdt:24112005:2010
				copyConfig();
				ReadRunSetupINI(); //Read default setting of runsetup from INI file.
				run_selected();
			 }
		  //run_initial();
		  //clearEvent(ev);
		//runall();
		//	ConfigFlag  = 1;
		  break;
		case cmRunSelected:
		  //clearEvent(ev);
		  //run_initial();//Commented by:sdt:18102005:2015
		  //Condition added by:sdt:08122005:1130
		  //Condition added for saved files should not be tested again.
		  if ( fs == fs_saved )
				messageBox( "File is already saved. Select New File for fresh test", mfInformation|mfOKButton );
		  else
		  {
			enableCommand( cmSaveDatFile );
			readSpeck(speckData,config.speckNumber); //Added by:sdt:24112005:2010
			config.szCableSequence = speckData.szCableSequence;//Added by:sdt:24112005:2010

			copyConfig();
		  //verifyConfig();//Added to later on
		  //verifyRunsetup();//Added to later on

			run_selected();
		 // ConfigFlag = 1;
		  }
			break;
		case cmCalibrate :
			viewCalibrationReport();
			break;
		case cmPrintCalibrate :
			printCalibrationReport();
			break;
		}
		clearEvent( ev );
	}
	//deskTop->disableCommand( cmRemove );
}
void TCts::getEvent( TEvent& ev )
{
	TApplication::getEvent( ev );

	if ( ev.what == evCommand && ev.message.command == cmResetSystem )
	{
	   //	clearEvent( ev );
	}
}

void TCts::InitSystem()
{
	char path[80];
	char szStr[50];
	prmarray = new PRM;

	ReadParamDotHdr();
	ReadRunSetup();//Added by:sdt:25082005:0050
	//ReadRunSetupINI();//Added by:sdt:16102005:0835//Commented by:sdt:15122016:2115
	ReadReportSetup(); //Added by:sdt:16102005:1415
	ReadFreqList();
	ReadHFFreqInfo();
	ReadLFFreqInfo();
	ReadConfigInfo(); //Defined in Readini.cpp

	InitializeDIOcard();
	InitializeADCcard();
	// For 7-SEGMENT DISPLAY
	InitializeSegmentLines();

	ReadStartup();

	if( readScheme() == TRUE )
	{
		sound( BEEP );
		delay( 500 );
		nosound ();
		//printf( "Error in Scheme File. Please read Pattern.Log\n" );
		messageBox( "Error in Scheme File. Please read Pattern.Log", mfError|mfOKButton );

	}
	sprintf(path,"%s:\\pattern.sch",cRamDrive);//22012001
	//system( path );
	::remove( path );
	fs = fs_new;
	memset (nTested, 0, sizeof(nTested));

	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	GetPrivateProfileString ("BASIC_INFO","MODEL_NO",m_BasicInfo.cModel," ",path);
	GetPrivateProfileString ("BASIC_INFO","VERSION",m_BasicInfo.cVersion," ",path);
	GetPrivateProfileString ("BASIC_INFO","SERIAL_NO",m_BasicInfo.cSerialNo," ",path);
	GetPrivateProfileString ("BASIC_INFO","COMPANY_NAME",m_BasicInfo.cCompanyName," ",path);
	GetPrivateProfileString ("BASIC_INFO","COMPANY_CITY",m_BasicInfo.cCompanyCity," ",path);

	GetPrivateProfileString("GUARDLIST","GUD0",Secu.szSivaEng,"siva",path);
	GetPrivateProfileString("GUARDLIST","GUD1",Secu.szEntry,"siva","default.ini");
	GetPrivateProfileString("GUARDLIST","GUD2",Secu.szExit,"siva","default.ini");
	GetPrivateProfileString("GUARDLIST","GUD3",Secu.szSetup,"siva","default.ini");
	GetPrivateProfileString("GUARDLIST","GUD4",Secu.szSpeck,"siva","default.ini");
	GetPrivateProfileString("ARGUMENT","ARGV",Secu.szArgv,"siva",path);

	Hz_80_Info.InstruDelay = GetPrivateProfileInt("80HZ_CTDELAY_INFO","INSTRUMENT_DELAY",1000,path);
	Hz_80_Info.JunctionDelay = GetPrivateProfileInt("80HZ_CTDELAY_INFO","JUNCTION_DELAY",1000,path);
	Hz_80_Info.RelayDelay = GetPrivateProfileInt("80HZ_CTDELAY_INFO","RELAY_DELAY",100,path);
	Hz_80_Info.PreReadDelay = GetPrivateProfileInt("80HZ_CTDELAY_INFO","PRE_READ_DELAY",100,path);
	Hz_80_Info.RangeDelay = GetPrivateProfileInt("80HZ_CTDELAY_INFO","RANGE_DELAY",100,path);

	GetPrivateProfileString("TEXT","TEXT1",Text1,"N","default.ini");
	GetPrivateProfileString("TEXT","TEXT2",Text2,"N","default.ini");
	GetPrivateProfileString("TEXT","TEXT3",Text3,"N","default.ini");

	sprintf(path,"%s:\\cts.ini",cRamDrive);
	bSync = GetPrivateProfileInt("BASIC_INFO","SYNC",0,path);
	bSync = bSync > 1 ? 1 : bSync;
	bSync = bSync < 0 ? 0 : bSync;
	delay_cm = GetPrivateProfileInt("BASIC_INFO","DELAY_CM",0,path);
	delay_cm = delay_cm < 0 ? 0:delay_cm;

	//As ZCH not required in Quad software.
	//GetZmsResLines();//Commented by:sdt:16102005:2245
	//SetImpedence(120);//Sets impedence to 120 Ohm//Commented by:sdt:16102005:2245

	for(int i=0;i<MAXPARAMETERS;i++)
	{
		sprintf(szStr,"%s_FREQ",paraminfo[i].szParamAbr);
		m_Freq[i].nFreq=GetPrivateProfileInt("FREQUENCY",szStr,1,path);
	}
	GetPrivateProfileString("BASIC_INFO","LFFREQ",nConfig.szLFreq,"1KHz",path);
	GetPrivateProfileString("BASIC_INFO","HFFREQ",nConfig.szHFreq,"150KHz",path);


	//amit for frequencies hard coded
	nConfig.nLFrequency=LFFrequency.nDefualtFreq;//1Khz
	nConfig.nHFrequency=HFFrequency.nDefualtFreq;//150Khz
	//OnFrequency();//Commented by:sdt:19032001
	SetHighFreq(nConfig.nHFrequency);
	SetLowFreq(nConfig.nLFrequency);

	InitialisePrinter();
	nReport=FALSE;

	nDC=NULL;

	ReadAtnVarOffs(); //Added by:sdt:27112005:0850
	InitializeINIInfo();
	//Speck initialisation code is not included.

	//InitializeZchArray();//Commented by:sdt:16102005:1840 //As it is no longer used

	CtPair.nTest = 0;

	ResetSystem();//Added:jj:29012001:0850
	copyConfig();//Added by:sdt:18102005:2020
	DefConfig = nConfig;           // store default config
	::remove("SBTest.Bin");//Added:jj:22012001:1610

	  //Added:Start:jj:18012001:1020
	 GetPrivateProfileString("DATA_DIRECTORY","PATH",DataDirectory,".\\DATA","default.ini");
	 {
	  char *temp = strupr( DataDirectory );
	  strcpy( DataDirectory, temp );
	 }

	 if ( access( DataDirectory, 00 ) != 0 )//Added:pnb:jj:28122000:1150
		if ( mkdir( DataDirectory,0755 ) != 0 )
		{
			   ushort helpCtx = TProgram::deskTop->helpCtx;
			   TProgram::deskTop->helpCtx = hcDataDirectory;
			   TProgram::statusLine->update();
			   TProgram::deskTop->helpCtx = helpCtx;
			   //messagewindow("Continuing with .\\DATA .Press Any Key.. ");
			   //sound(1000);//Commented by:sdt:06102005:0650
			   //delay(500);
			   //nosound();
			   //getch();
			   strcpy(DataDirectory,".\\DATA");
			   if ( access( DataDirectory, 00 ) != 0 )
				if ( mkdir( DataDirectory,0755 ) != 0 )
						exit(0);

		}//End:jj:18012001:1020

//Commented for DEBUG by:sdt:11092005:1920
#ifdef SECURITY
	init();  //Comment this line for debug//Add
#endif

	// get default config values from cts.ini
//Commented by:sdt:26072005:0002
/*   char temp[4];
   GetPrivateProfileString("BASIC_INFO","CABLELENGTH",temp,0,"default.ini");
   config.length = atof( temp );
   GetPrivateProfileString("DEFAULTS","ROOMTEMP",temp,0,"default.ini");
   config.roomtemp = atof(temp);
   config.numberofpairs = GetPrivateProfileInt("DEFAULTS","PAIRS",0,"default.ini");
   config.unitof = GetPrivateProfileInt("DEFAULTS","UNITOF",0,"default.ini");
   GetPrivateProfileString("DEFAULTS","SPECKS",config.Specks.specksnumber,"","default.ini");
   GetPrivateProfileString("DEFAULTS","CABLETYPE",config.Specks.cabletype,"","default.ini");
   GetPrivateProfileString("DEFAULTS","GUAGE",config.Specks.guage,"","default.ini");
   config.CUPPFormula = GetPrivateProfileInt("DEFAULTS","CUPP_FORMULA",0,"default.ini");
   strcpy(config.FileName,config.Specks.specksnumber);
   readSpeck(config.Specks,atoi(config.Specks.specksnumber));*/
   //Added by:sdt:05122005:2335
   //Disable the View & Print Cal option if argument is not given
if ((g_argc > 1) && strcmp(g_argv[1], Secu.szArgv) == 0)

   {
		enableCommand(cmCalibrate);
		//enableCommand(cmPrintCalibrate);//Commented by:sdt:06112006:2210
   }
   else
   {
		disableCommand( cmCalibrate );
		//disableCommand( cmPrintCalibrate );//Commented by:sdt:06112006:2210
   }


}

void TCts::exitSystem()
{
	 //char path[80];

	 //Commented by:sdt:22112005:2200
	 //sprintf(path,"%s:\\siv.tmp",cRamDrive);
	 //::remove(path);
	 //sprintf(path,"%s:\\store.stw",cRamDrive);
	 //::remove(path);
	 //::remove("message");
	 //sprintf(path,"%s:\\report.rep",cRamDrive);
	 //::remove(path);
	 //sprintf(path,"%s:\\pattern.sch",cRamDrive);
	 //::remove(path);

	 //Added by:sdt:22112005:2215 //To save file.
	 //Added by:sdt:17122016:1905:start
	FILE *fexit;
	if ((fexit = fopen("exit.bin", "wb")) != NULL)
	{
		fwrite(&nExit_status, sizeof(int),1,fexit);
	}
	if(fexit){fclose(fexit);}
	 //Added by:sdt:17122016:1905:end
	 if( fs == fs_executed || fs == fs_notsaved || fs == fs_bad || fs==fs_inprocess)
	 {
		if ( messageBox( "Do you want to save the file",
				mfYesButton|mfNoButton ) == cmYes )
		{
			SaveFile();
			char szString[80];
			sprintf( szString, "File Saved as %s\\%s", DataDirectory, nConfig.szFileName );//Added:jj:18012001:1045
			messageBox(szString,mfInformation|mfOKButton);
			RemoveFile();
		}
		else
		{
			fs = fs_notsaved;
			RemoveFile ();
		}
	 }

	 if ( nNoofSpans > 0 )
		farfree( AtnVarOff );	 // Free Atn. Look-Up Table Memory
	 freeFixture();	  			 // Free the Fixture Memory.
	 RemoveIniFiles();

	 freeFreqStruct();   //Added by:sdt:19032001
	 //Frees the frequency structure.
	 //Commented by:sdt:13112005:1455
	 //freeZmsResLines();//Added:jj:05042001 for Freeing the structure storing Zms Res Lines
	 FreeSequenceStructure(ALLPAIRS);//Added by:sdt:19042001
	 fcloseall();
	 //Commented by:sdt:22112005:2200
	 //::remove(".\\SBTest.bin");//Added:jj:22012001:1610
	 //::remove(".\\BSResult.bin");//Added:jj:22012001:1610

	 if ( noofparameters > 0 )	 // Close the file handle ___
		close( ParamCtsHandle ); // opened in ReadParamDotHdr();
	 delete prmarray; //Added by:sdt:22112005:2200
	 exit(0); //Added by:sdt:22112005:2200
}



void TCts::speckManagement()
{
	ushort aCreate, aRemove;

	TSpeckBuild *pSpeck = new TSpeckBuild( 0 );
	pSpeck->getAccessState( aCreate, aRemove );
	TProgram::deskTop->insert( (TSpeckBuild *)pSpeck );
	pSpeck->setAccessState( aCreate, aRemove );
	//destroy(pSpeck);
}


void TCts::aboutDlgBox( void )
{
	// About dialog
	TDialog *pAboutBox = new TDialog( TRect( 0, 0, 40, 15 ), "About" );
	char szTemp[200];
	//Contents of dialog box modified //sdt:07122005:2100
	sprintf( szTemp, "\003Cable Test Software\n\003\n\003Version \n\003"
			"\n\003\ BU161221 \n\003"
			"\n\003\Copyright (c) 2005, by\n\003\n"
			"\n\003ePoint Technologies, Deolali");
	TStaticText *pAboutInfo = new TStaticText( TRect( 2, 1, 39, 12 ), szTemp );
	pAboutInfo->options = ofCentered;
	pAboutBox->insert( pAboutInfo );
	pAboutBox->insert(new TButton(TRect(14, 12, 25, 14), " OK", cmOK, bfDefault));
	pAboutBox->options |= ofCentered;
	deskTop->execView( pAboutBox );
	destroy( pAboutBox );
}

//Added by:sdt:25072005:2000
void TCts::copyInisToRamdrive( void )
{
	char path[80];
	dec (3,"ects.ini", "c:\\cts.ini","");
	GetPrivateProfileString ("RAMDRIVE","RAMDRV",cRamDrive,"d","c:\\cts.ini");
	::remove("c:\\cts.ini");

	sprintf(path,"%s:\\cts.ini",cRamDrive);
	dec (3,"ects.ini",path,"");

	sprintf(path,"%s:\\prog.ini",cRamDrive);
	dec (3,"eprog.ini", path,"");

	sprintf(path,"%s:\\disprep.ini",cRamDrive);
	dec (3,"edisprep.ini" ,path,"");

	sprintf(path,"%s:\\pattern.sch",cRamDrive);
	dec (3,"epattern.sch",path,"");

	sprintf(path,"%s:\\std.ini",cRamDrive);
	dec (3,"estd.ini",path,"");

	sprintf(path,"%s:\\runsetup.ini",cRamDrive);
	dec (3,"ernsetup.ini",path,"");

}

const char* TCtsStatusLine::hint( ushort u )
{
	switch( u )
	{
	case hcExitInsuffMem:                    //Added by:sdt:17122016:2020
		return "Refreshing the System...";  //Added by:sdt:17122016:2020
	case hcDataDirectory :
		return "Creating Data Directory";
	case hcCalibrate:
		return "System under calibration";
	case hcCalibrateOver:
		return "Calibration Over";
	case hcStartTesting:
		return "Testing Started";
	case hcStopTesting:
		return "Testing Over";
	case hcTesting:
		return "Please StandBy Testing...";
	case hcTested:
		return "Testing Over";
	case hcRunAllTnR:
		return "Ready for testing all Tips & Rings";
	case hcFileMenu:
		return "File Operations( New, Open, Save, Exit, etc )";
	case hcFileNew:
		return "Creates a New File and refreshes and resets the system";
	case hcFileOpen:
		return "Opens an Existing File";
	case hcFileSave:
		return "Save the results of the Current File";
	case hcFileExit:
		return "Exit from the System and return to the Shell";
	case hcRunMenu:
		return "Execution of Parameters, All or Selected";
	case hcRunAll:
		return "Execute all parameters";
	case hcRunSelected:
		return "Execute only selected parameters from Test Setup";
	case hcSetupMenu:
		return "Setup the Test, Run, Config and Report Options";
	case hcSetupTest:
		return "Set the Test Options ";
	case hcSetupRun:
		return "Set the Run Options ";
	case hcSetupConfig:
		return "Set the Configuration";
	case hcSetupReport:
		return "Set the Report Options";
	case hcSpecksMenu:
		return "Specification Creation";
	case hcManagement:
		return "Modify, Create and Delete Specifications";
	case hcReportMenu:
		return "View and Printing of the Report";
	case hcReportView:
		return "View the report of the current File on the screen";
	case hcReportPrint:
		return "Printing the report of current File";
	case hcUtilsMenu:
		return "System Utilities: Calibration, Reset, Pause/Restart etc.";
	case hcUtilsReset:
		return "Reset the entire QUAD system and refresh the readings";
	case hcUtilsCal:
		return "Calibrate the system and provide Calibration Report";
	case hcUtilsPause:
		return "Pause the execution of the system";
	case hcUtilsRestart:
		return "Restart the system";
	case hcHelpMenu:
		return "Access online Help";
	case hcHelpContents:
		return "Show table of contents for online Help";
	case hcHelpAbout:
		return "Show version and copyright information";
	case hcAutoCal :        // B.Nilesh 23rd Sept. 1999 ( Start )
		return "AutoCalibration in Process...";
	case hcAutoRange :
		return "Please standby AutoRanging...";
	case hcFindOffset :
		return "Finding the Offset for each range...";
	case hcIrSelfCheck :
		return "Self-Checking the System, Please Wait...";
	case hcRunInd :
		return "Ready for testing all Conductors Individually...";
	case hcRunCnS :
		return "Ready for testing all Conductors and Sheath...";
							// B.Nilesh 23rd Sept. 1999 ( End )
	case hcViewReport :
		return "ALT+F3 - To Close the Report Window";
	default:
		return " ";
	}
}


struct run_param1
{
 ushort fixture;
 char start_fix[10];
 char bypass_pair[20];
}*run_parameter1;




void TCts::fileSave()
{
}

void TCts::fileOpen()
{
	char szString[80];
	if( fs == fs_executed || fs == fs_notsaved || fs == fs_bad)
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
			fs = fs_notsaved;
	}

	fs = fs_closed;
	flagReset=FALSE;

	char *p,fileName[MAXPATH],tempFileName[MAXPATH];

	TFileDialog *f = new TFileDialog( "*.use","Open file","~N~ame",fdOpenButton,1 );
	for(;;)
	{
		if ( deskTop->execView( f ) == cmFileOpen )
		{
			f->getFileName( tempFileName );
			p=strtok(tempFileName,"\\");
			while(p)
			{
				strcpy(fileName,p);
				p=strtok(NULL,"\\");
			}
			if ( GetConfigStructure (fileName) == fs_error  )
			{
				messageBox( mfError|mfOKButton, "Invalid file %s", fileName );
				continue;
			}
			//else
			//{
			//	lg.reOpen( fileName, NOEDIT );
			//	readLogAll();
			//	pHeap->showFileNo( fileNumber );
			//	fileStatus = Open;
			//}
			copynConfigtoConfig();//Added by:sdt:26092014:1649
			disableCommand( cmSaveDatFile );
			disableCommand( cmRunSelected );
			disableCommand( cmRunAll );
		}
		break;
	}
	destroy( f );

}
//int _argc;
//char **_argv;

int main( int argc, char *argv[] )
{
  g_argc = argc;
    g_argv = argv;
//_argc = argc;
//_argv = argv;

//Uncommented by:sdt:04122005:2355
/*#ifndef DEBUG
	oldKeyInt = getvect( 9 );
	setvect( 9, resetKeyInt );
	setBlink();
#endif*/

#ifdef SECURITY
	GetExeName(argv[0]);
    if(stricmp(argv[1],"/install")==0)
    {
    	install();
        exit(1);
    }
    if(stricmp(argv[1],"/uninstall")==0)
    {
      	uninstall();
        exit(1);
    }
    if(stricmp(argv[1],"/showinfo")==0)
    {
      	getinfo();
        exit(1);
	}
#endif


	TCts cts;
	cts.run();

//Uncommented by:sdt:04122005:2355
/*#ifndef DEBUG
	setvect(9, oldKeyInt);
#endif*/
return 0;
}


//Commented by:sdt:13112005:1455
/*void RemoveFilesFromRamDrive(void)
{
	char path[80];

	sprintf(path,"%s:\\cts.ini",cRamDrive);
	remove( path );

	sprintf(path,"%s:\\disprep.ini",cRamDrive);
	remove( path );

	 sprintf(path,"%s:\\pattern.sch",cRamDrive);
	remove( path );

	 sprintf(path,"%s:\\prog.ini",cRamDrive);
	remove( path );

	sprintf(path,"%s:\\std.ini",cRamDrive); //Added by:sdt:17072001
	remove( path );

	sprintf(path,"%s:\\intmlim.bin",cRamDrive);//Added by:sdt:23082001
	remove(path);

	sprintf(path,"%s:\\limits.bin",cRamDrive);
	remove(path);

	sprintf(path,"%s:\\summary.bin",cRamDrive);//Added by:sdt:04092001:1110
	remove(path);

	sprintf(path,"%s:\\seq.dat",cRamDrive);
	remove(path);

	sprintf(path,"%s:\\runsetup.ini",cRamDrive);
	remove(path);

	sprintf(path,"%s:\\sat.txt",cRamDrive);
	remove(path);

	//sprintf(path,"deltree /y %s:\\*.dat",cRamDrive);
	//system(path);
}*/


//Added by:sdt:25082005:0050
void ReadRunSetup(void)
{
	FILE *frnstp;
	if ((frnstp = fopen("rnsetup.bin", "rb")) != NULL) //Added by:sdt:15122016:2130
	{
		fread(&runSetup, sizeof(RunSetup),1,frnstp); //Added by:sdt:15122016:2130
		TempRunSetup =  runSetup; //Added by:sdt:15122016:2200
	}
	else
	{
		//Added by:sdt:24082005:2355
		//Start
		for(int i=0;i<MAXPARAMETERS;i++)
		{
			runSetup.ulParamRunSetup[i]=0;
		}
		GetPrivateProfileString("BASIC_INFO","FIXTURE",nConfig.szFixtures,"Inner","default.ini");
		if(strcmpi(nConfig.szFixtures,"BOTH")==0)
		{
			runSetup.wFixture = 2;
		}
		else if(strcmpi(nConfig.szFixtures,"OUTER")==0)
		{
			runSetup.wFixture = 1;
		}
		else if(strcmpi(nConfig.szFixtures,"INNER")==0)
		{
			runSetup.wFixture = 0;
		}
		//runSetup.wShortBreak = 0; //Commented by:sd:11112006:1800
		//Condition added by:sdt:11112006:1800
		struct ffblk ffblk;
		if(!findfirst("bns.exe",&ffblk,0))
		{
			runSetup.wShortBreak = 0x3;//BOTHBREAKSHORT
		}
		else
		runSetup.wShortBreak = 0x0;//NONEBREAKSHORT;
		runSetup.bSingleStep = False;
		runSetup.bZchMeas = False;
		strcpy(runSetup.szBypassPairs,"");
		runSetup.bAlarmsOff = False; //Added by:sdt:28042015:2100
		//End
		ReadRunSetupINI();//Added by:sdt:16102005:0835//Added here by:sdt:15122016:2115
	}
	if(frnstp){fclose(frnstp);}

}

unsigned int ReadStartup ()
{
	FILE    *pStartup;
	char    szHeader[]="ePoint Technologies, Nashik";
	unsigned int    nInfo;

	pStartup=fopen("startup.bin","rb");
	if(pStartup==NULL)
	{
		pStartup=fopen("startup.bin","wb");
		fwrite(&szHeader,sizeof(szHeader),1,pStartup);
		nInfo=26;
		fwrite(&nInfo,sizeof(nInfo),1,pStartup);
		fwrite(&nInfo,sizeof(nInfo),1,pStartup);
		nInfo=1;
		fwrite(&nInfo,sizeof(nInfo),1,pStartup);
	}
	else
	{
		fread(&szHeader,sizeof(szHeader),1,pStartup);
		fread(&nInfo,sizeof(nInfo),1,pStartup);
		fread(&nInfo,sizeof(nInfo),1,pStartup);
		fread(&nInfo,sizeof(nInfo),1,pStartup);
	}
	sprintf(nConfig.szFileName,"%d",nInfo);
	if(pStartup){fclose(pStartup);}
	return nInfo;
}

void WriteStartup (unsigned int nfileno)
{
	FILE *pStartup;
	char szHeader[]="ePoint Technologies, Nashik";
	unsigned int  nInfo;

	pStartup=fopen("startup.bin","wb");
	fwrite(&szHeader,sizeof(szHeader),1,pStartup);
	nInfo=26;
	fwrite(&nInfo,sizeof(nInfo),1,pStartup);
	nInfo=currenttime.ti_min;
	fwrite(&nInfo,sizeof(nInfo),1,pStartup);
	fwrite(&nfileno,sizeof(nfileno),1,pStartup);
	if(pStartup){fclose(pStartup);}
}

//Commented by:sdt:13112005:1445
/*void GetZmsResLines()
{
	char path[80];
    char szStr[100];
    char szTmp[100];
    int temp=10;

	Zms_Res.Lines=(int *) farmalloc(sizeof(int)*8);

	sprintf(path,"%s:\\cts.ini",cRamDrive); //Modified by:sdt:18062001
    // Now ZMS_LINES are read from cts.ini instead of the std.ini.

	for (int i=0;i<8;i++)
    {
    	strcpy(szStr,"");
        strcpy(szTmp,"");
        sprintf(szTmp,"RES_%d�",temp); //Modified by:sdt:18062001
    	GetPrivateProfileString ("ZMS_LINES",szTmp,"0",szStr,path);
    	Zms_Res.Lines[i] = atoi(szStr);
        temp*=2;
	}
}*/

//Commented by:sdt:13112005:1445
//void freeZmsResLines()
//{
//	farfree(Zms_Res.Lines);
//}

void ReadAtnVarOffs( void )
{
// Variable     : ATNVarLenOffs  :-  AtnVarOff[MAX_CAB_TYPE];
// Fields       : nUpperLimit, nLowerLimit - int, fOffset - Float

	char    path[80];
	int     i, j;
	char    szTmp[MAXSTRINGLENGTH1];
	char    cTempStr[20];
	char    *cTempStrPtr,*cTempPtr;
	int     nTempNo_L, nTempNo_U;
	char    cTempStr1[30];
	float   fCableType[MAX_CAB_TYPE] = { 0.4, 0.5, 0.63 , 0.9 };
	int     flag;

	sprintf( path, "%s:\\cts.ini",cRamDrive);//22012001
	nNoofSpans = GetPrivateProfileInt("BASIC_INFO","NOOFSPAN_ATNLKUPTAB",0,path);
	if ( nNoofSpans <= 0 )
	{
		nNoofSpans = 0;
		return;
	}
	AtnVarOff = ( ATNVarLenOffs far *) farmalloc ( sizeof ( ATNVarLenOffs ) *
										(nNoofSpans >= MAX_SPANS? MAX_SPANS:nNoofSpans));
	if ( AtnVarOff == NULL )
	{
		//printf( "Cannot Allocate Memory for AtnVarOff" );
		messageBox( "Cannot Allocate Memory for AtnVarOff", mfError|mfOKButton );
		RemoveIniFiles();
		exit(1);
	}

	flag = 0; // Used for - whether the Current Range should be checked for
	for (i = 0; i < (nNoofSpans >= MAX_SPANS? MAX_SPANS:nNoofSpans); i++)
	{
		AtnVarOff[i].nUpperLimit = 0;
		AtnVarOff[i].nLowerLimit = 0;
		for (j = 0; j < MAX_CAB_TYPE; j++ )
			AtnVarOff[i].fOffset[j] = 0.0;
	}
// ( AtnVarOff[i].nLowerLimit > AtnVarOff[i-1].nUpperLimit )
	for (i = 0; i < (nNoofSpans >= MAX_SPANS? MAX_SPANS:nNoofSpans); i++)
	{
		sprintf(szTmp,"SPAN_%d",i+1);
		GetPrivateProfileString("ATNLKUPTAB_SPAN",szTmp,cTempStr,NULL,path,0);
		if (strcmp( cTempStr,"") == 0 )
			flag = 0;
		else
		{
			cTempStrPtr = strdup(cTempStr);
			nTempNo_L = atoi(strtok(cTempStrPtr,"-"));
			cTempPtr = strchr(cTempStr,'-');
			cTempPtr++;
			nTempNo_U = atoi(cTempPtr);                     // Get the Range Seperated
			free(cTempStrPtr);//added by:sdt:21122016:1900
			if ( flag ) // To be checked
			{
				if ( (nTempNo_L > AtnVarOff[i-1].nUpperLimit) &&
					 (nTempNo_U > nTempNo_L) )
				{
					AtnVarOff[i].nLowerLimit = nTempNo_L;
					AtnVarOff[i].nUpperLimit = nTempNo_U;
					for ( j = 0; j < MAX_CAB_TYPE; j++ )
					{
						sprintf(cTempStr1,"ATN_OFFSET_FOR_%1.2f_CABLE",fCableType[j]);
						GetPrivateProfileString(cTempStr1,szTmp,cTempStr,NULL,path,0);//22012001
						if ( strcmp( cTempStr,"") == 0 )
							AtnVarOff[i].fOffset[j] = 0.0;
						else
							AtnVarOff[i].fOffset[j] = atof( cTempStr );
						flag = 1;   // Check Next Time.
					}
				}
				else
					flag = 0; // Next time to checking & array already Initialise
			}
			else // Not to be checked.
			{
				if ( nTempNo_U > nTempNo_L ) // check if UpperLimit > LowerLimit
				{
					AtnVarOff[i].nLowerLimit = nTempNo_L;
					AtnVarOff[i].nUpperLimit = nTempNo_U;
					for ( j = 0; j < MAX_CAB_TYPE; j++ )
					{
						sprintf(cTempStr1,"ATN_OFFSET_FOR_%1.2f_CABLE",fCableType[j]);
						GetPrivateProfileString(cTempStr1,szTmp,cTempStr,NULL,path,0);//22012001
						if ( strcmp( cTempStr,"") == 0 )
							AtnVarOff[i].fOffset[j] = 0.0;
						else
							AtnVarOff[i].fOffset[j] = atof( cTempStr );
						flag = 1; // Check Next Time
					}
				}
				else
					flag = 0; // Next Time No Check
			}
		}
	} // End of for Loop
} // End of ReadAtnVarOffs()


void RemoveIniFiles(void)
{
	char path[80];

	sprintf(path,"%s:\\cts.ini",cRamDrive);
	remove( path );

	sprintf(path,"%s:\\disprep.ini",cRamDrive);
	remove( path );

	sprintf(path,"%s:\\pattern.sch",cRamDrive);
	remove( path );

	sprintf(path,"%s:\\prog.ini",cRamDrive);
	remove( path );

	sprintf(path,"%s:\\std.ini",cRamDrive); //Added by:sdt:17072001
	remove( path );

	sprintf(path,"%s:\\intmlim.bin",cRamDrive);//Added by:sdt:23082001
	remove(path);

	sprintf(path,"%s:\\limits.bin",cRamDrive);
	remove(path);

	sprintf(path,"%s:\\sat.txt",cRamDrive);
	remove(path);

	sprintf(path,"%s:\\summary.bin",cRamDrive);//Added by:sdt:04092001:1110
	remove(path);

	sprintf(path,"%s:\\seq.dat",cRamDrive);
	remove(path);

	sprintf(path,"%s:\\runsetup.ini",cRamDrive);
	remove(path);

	RemoveFile();//Added by:sdt:30072006:2215

	//Temporary commented by:sdt:11092005:sdt:1945
	//sprintf(path,"deltree /y %s:\\*.dat",cRamDrive);
	//system(path);
}

//Commented by:sdt:13112005:1445
//This will initilize Zch array with value as 120�.
/*void InitializeZchArray()
{
	for(int i=0;i<MAXFREQUENCIES;i++)
	{
		ZchValArray[i]=120;
		ZchValArray[i]=0; //Commented by:sdt:16102005:1840 //As it is no longer used
	}
}*/

void newDatFile(void )
{
	#ifdef SECURITY
		init();  //Comment this line for debug//Add
	#endif

   //Added:jj:06042001
   for(int a=0;a<MAXPARAMETERS;a++)
	   for (int b=0;b<MAXFREQUENCIES;b++)
			param_tested[a][b]=0;

	//Added:MOD24072002:jj
	//memset(Worstpairnext.Onext,0,MAXFREQUENCIES*2);
	//memset(Worstpairnext.Fext,0,MAXFREQUENCIES*2);
	//memset(Worstpairnext.Elfext,0,MAXFREQUENCIES*2);
	//memset(Worstpairnext.Inext,0,MAXFREQUENCIES*2);
	//Modified by:sdt:04022007:1635
	memset(Worstpairnext,0,sizeof(Worstpairnext));

	memset (nTested, 0, sizeof(nTested));

	if( fs == fs_executed || fs == fs_notsaved || fs == fs_bad || fs==fs_inprocess)
	{
		if ( messageBox( "Do you want to save the file",
				mfYesButton|mfNoButton ) == cmYes )
		{
			SaveFile();
			char szString[80];
			sprintf( szString, "File Saved as %s\\%s", DataDirectory, nConfig.szFileName );//Added:jj:18012001:1045
			messageBox(szString,mfInformation|mfOKButton);
			RemoveFile();
		}
		else
		{
			fs = fs_notsaved;
			RemoveFile ();
		}
	}
	for (int i = 0;i < MAXPARAMETERS;i ++)
	{
		 for(int j=0;j<MAXFREQUENCIES;j++)
		 {
			m_FileInfo.nReadingsOk[i][j]=0;
			m_FileInfo.nReadingsNotOk[i][j]=0;
			m_FileInfo.cParamTested[i][j]=FALSE;
		}
	}
	fs = fs_new;
	nConfig = DefConfig;
	RemoveFile();
	flagReset = FALSE;
	ReadStartup();
	m_FileInfo.nFileNumber = 0;
	//LoadFlag=0;
	//nSpeckFileNo=atoi(nConfig.Specks.specksnumber);
	//freadspeck(&speck);
	readSpeck(speckData,nConfig.Specks.nSpeckFileNumber);
	HFCalibrateFlag = FALSE;//Added by:sdt:31072001
	InitSummaryFile();//Added by:sdt:03092001:0905.
}

//sdt:27062006:2230
//Function used to Display the Formulae
void TCts::formulaeDlgBox( void )
{
	vfCreateFormulaeDatFile();

	char path[80];
	sprintf(path,"%s:\\formulae.dat",cRamDrive);
	TView *w= validView( new TFileWindow(path,"Formulae") );
	if ( w != 0 )
	TProgram::deskTop->insert( w );
	::remove(path);
}

//sdt:27062006:2230
//Function used to create Formulae.dat file in Ramdrive.
//This file used to display the formulae on screen.
void vfCreateFormulaeDatFile()
{
	FILE *fp, *fpFrml;
	char path[80];
	char cCh;

	sprintf(path,"%s:\\formulae.dat",cRamDrive);
	fpFrml = fopen(path,"w");

	if ( (fp=fopen("title.inf", "r")) == NULL )
	{
		printf("\nCannot open Title.Inf File...");
		getch();
		exit(1);
	}
	while(!feof(fp))
	{
		cCh=fgetc(fp);
		fputc(cCh,fpFrml);
	}
	if(fp){fclose(fp);}


	char *Ptr;
	char szTmp[60];
	char ParamFor[70];//[200];
	int nParamNum;
	sprintf(path,"%s:\\cts.ini",cRamDrive);//22012001
	for(nParamNum=Param.Param_Start; nParamNum<=Param.Param_End;nParamNum++)
	{
		//Ptr = ParamFor[nParamCount];
		sprintf(szTmp,"%s", paraminfo[nParamNum].szParamAbr);
		fputs(szTmp,fpFrml);
		fputc('\n', fpFrml);
		for(int i=1;i<=4;i++)
		{
			sprintf(szTmp,"%s_FOR%d", paraminfo[nParamNum].szParamAbr,i);
			GetPrivateProfileString("FORMULAS",szTmp,ParamFor,"\0",path);
			//GetPrivateProfileString("FORMULAS",szTmp,"",Ptr+((i-1)*50),path);
			fputs(ParamFor,fpFrml);
			if(ParamFor[0]!='\0')
				fputc('\n', fpFrml);
		}
		fputc('\n', fpFrml);
	}
	if(fpFrml){fclose(fpFrml);}
}

//Added by:sdt:26042015:1700
void PinReplacement()
{
  FILE *fpin;
  if ((fpin = fopen("pinrplc.bin", "rb")) != NULL)
  {
	  fread(pinReplace, (sizeof(REPLACE)*(FIXTUREBOTH+1)),1,fpin);
  }
  if(fpin){fclose(fpin);}

  TPinReplacement *pPR = new TPinReplacement( TRect(7,2 ,70, 18),"Pin Replacement");
  pPR->insert(new TButton( TRect( 33, 13, 44 ,15), "~C~ancel", cmCancel, bfNormal ));
  pPR->insert(new TButton( TRect( 17, 13, 28, 15), "~O~K", cmOKEdit, bfDefault ));

  pPR->insert(new TStaticText(TRect( 5,2,15,3),"INNER FIXT"));
  pPR->insert(new TStaticText(TRect( 25,2,35,3),"OUTER FIXT"));
  pPR->insert(new TStaticText(TRect( 45,2,55,3),"BOTH FIXT"));

  pPR->pInnerPin1 = new TInputLine(TRect(2,4,8,5),4);
  pPR->insert(pPR->pInnerPin1);
  pPR->insert(new TLabel(TRect(2,3,8,4),"Pin1",pPR->pInnerPin1));
  sprintf(pPR->pInnerPin1->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplace[0]);
  pPR->pReplacePin1 = new TInputLine(TRect(11,4,17,5),4);
  pPR->insert(pPR->pReplacePin1);
  pPR->insert(new TLabel(TRect( 11,3,17,4),"Rpin1",pPR->pReplacePin1));
  sprintf(pPR->pReplacePin1->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplacewith[0]);

  pPR->pInnerPin2 = new TInputLine(TRect(2,6,8,7),4);
  pPR->insert(pPR->pInnerPin2);
  pPR->insert(new TLabel(TRect(2,5,8,6),"Pin2",pPR->pInnerPin2));
  sprintf(pPR->pInnerPin2->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplace[1]);
  pPR->pReplacePin2 = new TInputLine(TRect(11,6,17,7),4);
  pPR->insert(pPR->pReplacePin2);
  pPR->insert(new TLabel(TRect( 11,5,17,6),"Rpin2",pPR->pReplacePin2));
  sprintf(pPR->pReplacePin2->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplacewith[1]);

  pPR->pInnerPin3 = new TInputLine(TRect(2,8,8,9),4);
  pPR->insert(pPR->pInnerPin3);
  pPR->insert(new TLabel(TRect(2,7,8,8),"Pin3",pPR->pInnerPin3));
  sprintf(pPR->pInnerPin3->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplace[2]);
  pPR->pReplacePin3 = new TInputLine(TRect(11,8,17,9),4);
  pPR->insert(pPR->pReplacePin3);
  pPR->insert(new TLabel(TRect( 11,7,17,8),"Rpin3",pPR->pReplacePin3));
  sprintf(pPR->pReplacePin3->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplacewith[2]);

  pPR->pInnerPin4 = new TInputLine(TRect(2,10,8,11),4);
  pPR->insert(pPR->pInnerPin4);
  pPR->insert(new TLabel(TRect(2,9,8,10),"Pin4",pPR->pInnerPin4));
  sprintf(pPR->pInnerPin4->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplace[3]);
  pPR->pReplacePin4 = new TInputLine(TRect(11,10,17,11),4);
  pPR->insert(pPR->pReplacePin4);
  pPR->insert(new TLabel(TRect( 11,9,17,10),"Rpin4",pPR->pReplacePin4));
  sprintf(pPR->pReplacePin4->data,"%3d",pinReplace[INNERFIXTURE].nPinsReplacewith[3]);

  pPR->pOuterPin1 = new TInputLine(TRect(22,4,28,5),4);
  pPR->insert(pPR->pOuterPin1);
  pPR->insert(new TLabel(TRect(22,3,28,4),"Pin1",pPR->pOuterPin1));
  sprintf(pPR->pOuterPin1->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplace[0]);
  pPR->pReplOutPin1 = new TInputLine(TRect(31,4,37,5),4);
  pPR->insert(pPR->pReplOutPin1);
  pPR->insert(new TLabel(TRect( 31,3,37,4),"Rpin1",pPR->pReplOutPin1));
  sprintf(pPR->pReplOutPin1->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplacewith[0]);

  pPR->pOuterPin2 = new TInputLine(TRect(22,6,28,7),4);
  pPR->insert(pPR->pOuterPin2);
  pPR->insert(new TLabel(TRect(22,5,28,6),"Pin2",pPR->pOuterPin2));
  sprintf(pPR->pOuterPin2->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplace[1]);
  pPR->pReplOutPin2 = new TInputLine(TRect(31,6,37,7),4);
  pPR->insert(pPR->pReplOutPin2);
  pPR->insert(new TLabel(TRect( 31,5,37,6),"Rpin2",pPR->pReplOutPin2));
  sprintf(pPR->pReplOutPin2->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplacewith[1]);

  pPR->pOuterPin3 = new TInputLine(TRect(22,8,28,9),4);
  pPR->insert(pPR->pOuterPin3);
  pPR->insert(new TLabel(TRect(22,7,28,8),"Pin3",pPR->pOuterPin3));
  sprintf(pPR->pOuterPin3->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplace[2]);
  pPR->pReplOutPin3 = new TInputLine(TRect(31,8,37,9),4);
  pPR->insert(pPR->pReplOutPin3);
  pPR->insert(new TLabel(TRect( 31,7,37,8),"Rpin3",pPR->pReplOutPin3));
  sprintf(pPR->pReplOutPin3->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplacewith[2]);

  pPR->pOuterPin4 = new TInputLine(TRect(22,10,28,11),4);
  pPR->insert(pPR->pOuterPin4);
  pPR->insert(new TLabel(TRect(22,9,28,10),"Pin4",pPR->pOuterPin4));
  sprintf(pPR->pOuterPin4->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplace[3]);
  pPR->pReplOutPin4 = new TInputLine(TRect(31,10,37,11),4);
  pPR->insert(pPR->pReplOutPin4);
  pPR->insert(new TLabel(TRect( 31,9,37,10),"Rpin4",pPR->pReplOutPin4));
  sprintf(pPR->pReplOutPin4->data,"%3d",pinReplace[OUTERFIXTURE].nPinsReplacewith[3]);

  pPR->pBothPin1 = new TInputLine(TRect(42,4,48,5),4);
  pPR->insert(pPR->pBothPin1);
  pPR->insert(new TLabel(TRect(42,3,48,4),"Pin1",pPR->pBothPin1));
  sprintf(pPR->pBothPin1->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplace[0]);
  pPR->pReplBothPin1 = new TInputLine(TRect(51,4,57,5),4);
  pPR->insert(pPR->pReplBothPin1);
  pPR->insert(new TLabel(TRect( 51,3,57,4),"Rpin1",pPR->pReplBothPin1));
  sprintf(pPR->pReplBothPin1->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplacewith[0]);

  pPR->pBothPin2 = new TInputLine(TRect(42,6,48,7),4);
  pPR->insert(pPR->pBothPin2);
  pPR->insert(new TLabel(TRect(42,5,48,6),"Pin2",pPR->pBothPin2));
  sprintf(pPR->pBothPin2->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplace[1]);
  pPR->pReplBothPin2 = new TInputLine(TRect(51,6,57,7),4);
  pPR->insert(pPR->pReplBothPin2);
  pPR->insert(new TLabel(TRect( 51,5,57,6),"Rpin2",pPR->pReplBothPin2));
  sprintf(pPR->pReplBothPin2->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplacewith[1]);

  pPR->pBothPin3 = new TInputLine(TRect(42,8,48,9),4);
  pPR->insert(pPR->pBothPin3);
  pPR->insert(new TLabel(TRect(42,7,48,8),"Pin3",pPR->pBothPin3));
  sprintf(pPR->pBothPin3->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplace[2]);
  pPR->pReplBothPin3 = new TInputLine(TRect(51,8,57,9),4);
  pPR->insert(pPR->pReplBothPin3);
  pPR->insert(new TLabel(TRect( 51,7,57,8),"Rpin3",pPR->pReplBothPin3));
  sprintf(pPR->pReplBothPin3->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplacewith[2]);

  pPR->pBothPin4 = new TInputLine(TRect(42,10,48,11),4);
  pPR->insert(pPR->pBothPin4);
  pPR->insert(new TLabel(TRect(42,9,48,10),"Pin4",pPR->pBothPin4));
  sprintf(pPR->pBothPin4->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplace[3]);
  pPR->pReplBothPin4 = new TInputLine(TRect(51,10,57,11),4);
  pPR->insert(pPR->pReplBothPin4);
  pPR->insert(new TLabel(TRect( 51,9,57,10),"Rpin4",pPR->pReplBothPin4));
  sprintf(pPR->pReplBothPin4->data,"%3d",pinReplace[BOTHFIXTURE].nPinsReplacewith[3]);


  pPR->pInnerPin1->select();
  TProgram::deskTop->execView( pPR );
  TDialog::destroy (pPR);
}

//Added by:sdt:26042015:1700
void TPinReplacement::handleEvent(TEvent &ev)
{
	TDialog::handleEvent(ev);
	if(ev.what == evCommand)
	 {
		if(ev.message.command == cmOKEdit)
		{
			pinReplace[INNERFIXTURE].nPinsReplace[0] = atoi (pInnerPin1->data);
			pinReplace[INNERFIXTURE].nPinsReplacewith[0] = atoi (pReplacePin1->data);
			pinReplace[INNERFIXTURE].nPinsReplace[1] = atoi (pInnerPin2->data);
			pinReplace[INNERFIXTURE].nPinsReplacewith[1] = atoi (pReplacePin2->data);
			pinReplace[INNERFIXTURE].nPinsReplace[2] = atoi (pInnerPin3->data);
			pinReplace[INNERFIXTURE].nPinsReplacewith[2] = atoi (pReplacePin3->data);
			pinReplace[INNERFIXTURE].nPinsReplace[3] = atoi (pInnerPin4->data);
			pinReplace[INNERFIXTURE].nPinsReplacewith[3] = atoi (pReplacePin4->data);

			pinReplace[OUTERFIXTURE].nPinsReplace[0] = atoi (pOuterPin1->data);
			pinReplace[OUTERFIXTURE].nPinsReplacewith[0] = atoi (pReplOutPin1->data);
			pinReplace[OUTERFIXTURE].nPinsReplace[1] = atoi (pOuterPin2->data);
			pinReplace[OUTERFIXTURE].nPinsReplacewith[1] = atoi (pReplOutPin2->data);
			pinReplace[OUTERFIXTURE].nPinsReplace[2] = atoi (pOuterPin3->data);
			pinReplace[OUTERFIXTURE].nPinsReplacewith[2] = atoi (pReplOutPin3->data);
			pinReplace[OUTERFIXTURE].nPinsReplace[3] = atoi (pOuterPin4->data);
			pinReplace[OUTERFIXTURE].nPinsReplacewith[3] = atoi (pReplOutPin4->data);

			pinReplace[BOTHFIXTURE].nPinsReplace[0] = atoi (pBothPin1->data);
			pinReplace[BOTHFIXTURE].nPinsReplacewith[0] = atoi (pReplBothPin1->data);
			pinReplace[BOTHFIXTURE].nPinsReplace[1] = atoi (pBothPin2->data);
			pinReplace[BOTHFIXTURE].nPinsReplacewith[1] = atoi (pReplBothPin2->data);
			pinReplace[BOTHFIXTURE].nPinsReplace[2] = atoi (pBothPin3->data);
			pinReplace[BOTHFIXTURE].nPinsReplacewith[2] = atoi (pReplBothPin3->data);
			pinReplace[BOTHFIXTURE].nPinsReplace[3] = atoi (pBothPin4->data);
			pinReplace[BOTHFIXTURE].nPinsReplacewith[3] = atoi (pReplBothPin4->data);

			FILE *fpin;
			if ((fpin = fopen("pinrplc.bin", "wb")) != NULL)
			{
				fwrite(pinReplace, (sizeof(REPLACE)*(FIXTUREBOTH+1)),1,fpin);
			}
			if(fpin){fclose(fpin);}


			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );
		}
	 }
}
