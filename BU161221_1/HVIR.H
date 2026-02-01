//
// Hvir.H - Main File
// declaration for HVIR system which includes menus and submenu's
// declaration for providing dynamic status line for providing
// hint for menus
// Turbo Vision related classes starts with T
// HVIR System  related classes starts with C
//


#if !defined( __HVIR_H )
#define __HVIR_H

#include "common.h"
#include "uses.h"
#include "hvbd.h"
#include "ir.h"
#include "info.h"
#include "bns.h"
#include "statuspg.h"

#define VT_VIEW			0  // Report View Type
#define VT_PRINTER			1

#if !defined( __PASSWORD_H )
#include "password.h"
#endif

#include <stdarg.h>

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
//#define cmResetSystem  151
#define cmCalibrate		152
#define cmPause	    	153

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
// HELP Menu
#define hcHelpMenu		0x7000
#define hcHelpContents	0x7001
#define hcHelpAbout		0x7002

class TCompany;
class TProgress;
class THeapView;
class TFrontPage;

class THvir : public TApplication
{
protected:
	TCompany 		*pCompany;
	THeapView		*pHeap;
	TFrontPage	*fPage;
	CHvbd		*hvbd;
	CIr			*ir;
	CBnS			*bns;
	TStatusPage	*sPage;

	Config		config;
	SpeckData		speckData;
	TestSetup		testSetup;
	RunSetup    	runSetup;
	ReportSetup	reportSetup;
	InfoList		info;
	enum 		{ Saved=0, New=1, Open, InProcess, Processed }fileStatus;
	unsigned long	fileNumber;

	char 		m_SerialNo[20];
	char 		m_Version[20];
	User 		currentUser;

public:
	THvir();	// the constructor
	~THvir();	// the destructor
	virtual void idle();

	void 	resetSystem( void );
	void 	resetAfterTest( void );
	static 	TMenuBar* 	initMenuBar( TRect r );
	static 	TStatusLine* initStatusLine( TRect r );
	static	TDeskTop* initDeskTop( TRect r );
	void 	initSystem();
	virtual 	void draw();
	friend 	int getFileStatus();

protected:
	void 	handleEvent( TEvent& ev );
	void 	getEvent( TEvent& ev );
	void 	aboutDlgBox( void );
	void 	exitSystem();
	void 	speckManagement();
	void 	runHvirAll( void );
	void 	runHvirSelected( void );
	void 	fileOpen( void );
	void 	fileSave( void );
	void 	updateLogAll( void );
	void 	readLogAll( void );
};

// global pointer to application's object
// Use in case required
//THvir *pHvir;

class THvirStatusLine : public TStatusLine
{
public:
	THvirStatusLine( TRect& r, TStatusDef& d ) :
		TStatusLine( r, d )
	{
		return;
	};
	const char* hint( ushort u );
};

class THvirDeskTop : public TDeskTop
{
public:
	THvirDeskTop( TRect& r);
	static TBackground* initBackground( TRect r);
	void draw();
};

class THvirBackground : public TBackground
{
public:
	THvirBackground( TRect& r, char aPattern );
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

#endif

