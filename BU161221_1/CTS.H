//
// CTS.H - Main File
// declaration for CTS system which includes menus and submenu's
// declaration for providing dynamic status line for providing
// hint for menus
// Turbo Vision related classes starts with T
// CTS System  related classes starts with C
//


#if !defined( __CTS_H )
#define __CTS_H

#include "uses.h" //Commented by:sdt:25072005:1935
#include "stdio.h"

#include "ct.h" //Commented by:sdt:26072005:0030
#include "setup.h"//Added by:sdt:24082005:2345
//#include "common.h"//Commented by:sdt:25072005:1935
//#include "config.h"//Commented by:sdt:25072005:1935
//#include "info.h"  //Commented by:sdt:25072005:1935
#if !defined( __PASSWORD_H )
#include "password.h"
#endif

//#define	DEBUG

#define MAXSTRINGLENGTH1	50


#define FRONTPAGE		1
#define STATUSPAGE		2
// Commands for the Menu
// FILE
#define cmNewDatFile	101
#define cmOpenDatFile	102
#define cmSaveDatFile	103
#define cmExit			104
#define cmEntry		105
// RUN
#define cmRunAll		111
#define cmRunSelected	112
// SETUP
#define cmHvSetup		121
#define cmIrSetup		122
#define cmRunSetup		123
#define cmConfigSetup	124
#define cmReportSetup	125
// SPECKS
#define cmManagement	131
// REPORT
#define cmViewReport	141
#define cmPrintReport	142
// UTILS
#define cmResetSystem   151
#define cmCalibrate		152
#define cmPause	    	153
#define cmResume        154
#define cmPrintCalibrate 155
#define cmFormulae		156
#define cmPrintFormulae		157
#define cmPinReplace	158 //Added by:sdt:26042015:1700
// HELP
#define cmContents		161
#define cmAbout		162
// INITIALIZE
#define cmInitSystem	163

// Context-Sensitive Help
// FILE Menu
#define hcFileMenu		0x2000
#define hcFileNew   	0x2001
#define hcFileOpen		0x2002
#define hcFileSave		0x2003
#define hcFileExit		0x2004
// RUN Menu
#define hcRunMenu		0x3000
#define hcRunAll		0x3001
#define hcRunSelected	0x3002
// SETUP Menu
#define hcSetupMenu		0x4000
#define hcSetupTest		0x4001
#define hcSetupRun		0x4002
#define hcSetupConfig	0x4003
#define hcSetupReport	0x4004
// SPECK MENU
#define hcSpecksMenu    	0x4005
#define hcManagement	0x4006
// REPORT Menu
#define hcReportMenu	0x5000
#define hcReportView	0x5001
#define hcReportPrint	0x5002
// UTILS Menu
#define hcUtilsMenu		0x6000
#define hcUtilsReset	0x6001
#define hcUtilsCal		0x6002
#define hcUtilsPause	0x6003
#define hcUtilsRestart	0x6004
#define hcUtilsPrintCal 0x6005
#define hcFormulae		0x6006
#define hcPrintFormulae	0x6007
#define hcPinReplace     0x6008 //Added by:sdt:26042015:1700
// HELP Menu
#define hcHelpMenu		0x7000
#define hcHelpContents	0x7001
#define hcHelpAbout		0x7002

//
#define hcExitInsuffMem  0xB001

class TCompany;
class TProgress;
class THeapView;
class TFrontPage;

class TCts : public TApplication
{
protected:
	User 		   currentUser;

	//SPECIFICATION  *speckptr;
	//RunSetup 	runSetup;
public:
	THeapView *heap;                  // Heap view

public:
	TCts();	// the constructor
	~TCts();	// the destructor
	static 	TMenuBar* 	initMenuBar( TRect r );
	static 	TStatusLine* initStatusLine( TRect r );
	static	TDeskTop* initDeskTop( TRect r );
	virtual 	void draw();


protected:
	void 	handleEvent( TEvent& ev );
	void 	getEvent( TEvent& ev );
	void 	aboutDlgBox( void );
	void 	fileOpen( void );
	void 	fileSave( void );
	void 	speckManagement();
	void    	InitSystem();
	void 	copyInisToRamdrive( void );
	void 	exitSystem();
	void 	formulaeDlgBox( void );
};


// global pointer to application's object
// Use in case required

class TCtsStatusLine : public TStatusLine
{
public:
	TCtsStatusLine( TRect& r, TStatusDef& d ) :
		TStatusLine( r, d )
	{
		return;
	};
	const char* hint( ushort u );
};

class TCtsDeskTop : public TDeskTop
{
public:
	TCtsDeskTop( TRect& r);
	static TBackground* initBackground( TRect r);
	void draw();
};

class TCtsBackground : public TBackground
{
public:
	TCtsBackground( TRect& r, char aPattern );
	void draw();
};

class TCompany : public TView
{
public:
	TCompany( TRect& r ) : TView( r )
	{
	}
	virtual void draw();
};

class TSpeckList : public TCollection
{
public:
	TSpeckList( ccIndex aLimit, ccIndex aDelta ) :
	TCollection( aLimit, aDelta ) {};
};


//Class Added by:sdt:26042015:1700
class TPinReplacement : public TDialog
{
protected:
	void handleEvent( TEvent& ev );

public:

	TInputLine *pInnerPin1;			// Absolute Lower Limit
	TInputLine *pInnerPin2;			// Absolute Upper Limit
	TInputLine *pInnerPin3;			// Absolute Average Limit
	TInputLine *pInnerPin4;		// Absolute Average Limit
	TInputLine *pReplacePin1;			// Standard
	TInputLine *pReplacePin2;			// Standard
	TInputLine *pReplacePin3;			// RMS
	TInputLine *pReplacePin4;			// RMS
	TInputLine *pOuterPin1;			// Absolute Lower Limit
	TInputLine *pOuterPin2;			// Absolute Upper Limit
	TInputLine *pOuterPin3;			// Absolute Average Limit
	TInputLine *pOuterPin4;		// Absolute Average Limit
	TInputLine *pReplOutPin1;			// Standard
	TInputLine *pReplOutPin2;			// Standard
	TInputLine *pReplOutPin3;			// RMS
	TInputLine *pReplOutPin4;			// RMS
	TInputLine *pBothPin1;			// Absolute Lower Limit
	TInputLine *pBothPin2;			// Absolute Upper Limit
	TInputLine *pBothPin3;			// Absolute Average Limit
	TInputLine *pBothPin4;		// Absolute Average Limit
	TInputLine *pReplBothPin1;			// Standard
	TInputLine *pReplBothPin2;			// Standard
	TInputLine *pReplBothPin3;			// RMS
	TInputLine *pReplBothPin4;			// RMS


public :
	//TPinReplacement( TRect& r, char *atitle ) : TDialog( r, atitle ) ,
	  TPinReplacement(const TRect& r, char *atitle) : TDialog(r, atitle),

						TWindowInit( TPinReplacement::initFrame )
	{
	}
};



#endif
