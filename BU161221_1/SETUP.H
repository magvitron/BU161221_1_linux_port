#if !defined(__SETUP_H)
#define __SETUP_H

#include "deftype.h"
#include "common.h"

#define RUN_IRCNS		0x01
#define RUN_IRTNR		0x02
#define RUN_IRRNT		0x04
#define RUN_IRIND		0x08

#define RUN_HVCNS		0x10
#define RUN_HVTNR		0x20
#define RUN_HVRNT		0x40
#define RUN_HVIND		0x80

#define RUN_BNSBREAK	0x100
#define RUN_BNSSHORT	0x200
#define RUN_BNSSHORTCNS  0x400     // Sannm 24/07/2000 ShortCnS

#define MAX_HEADER1		80
#define MAX_HEADER2		80
#define AL_LEFT		0
#define AL_CENTER		1
#define AL_RIGHT		2


struct TestSetup
{
	struct
	{
		uchar		m_Voltage;				// voltage
		uchar		m_Range;				// default range
		uchar 		m_Mode;					// mode: normal Go/NoGo
		uchar   		m_Span;					// full scale span
		unsigned 	int m_ActDelay;		// activator delay;
	}Ir;
	struct
	{
		uchar		m_TripSetCurr;			// trip set current
		uchar 		m_VoltRiseRate;			// voltage rise rate
		unsigned 	int m_ActDelay;		// activator delay;
	}Hv;
};

struct RunSetup
{
	unsigned long ulParamRunSetup[MAXPARAMETERS];
	WORD	wShortBreak;
	WORD	wFixture;
	Boolean bSingleStep;
	Boolean bZchMeas;
	char szBypassPairs[MAXBYPASSPAIRLEN];
	Boolean bAlarmsOff; //Added by:sdt:28042015:2100
	//WORD mode;      // 1st BYTE HV, 2nd BYTE IR 3rd BYTE BnS
	//WORD tested;    // 1st BYTE HV, 2nd BYTE IR 3rd BYTE BnS
};

// Report Setup
struct Page
{
	int		m_Cpi;
	float 	m_Height;
	float 	m_Width;
	float 	m_Top;
	float 	m_Left;
	float 	m_Right;
	float 	m_Bottom;
};

struct ReportSetup
{
	WORD		m_Mode;		// 0x400-FEXT 0x200-NEXT 0x100 -ATN
							// 0x80-ZCH 0x40-CUPPI, 0x20-CUPP, 0x10-CUPG
							// 0x08-CM 0x04-CR, 0x02-LR, 0x01-RU
	BYTE		m_Type;		// 0x01= Normalised, 0x02 = Unnormalised
	BYTE		m_Remark;	// Remark Yes/No
	BYTE		m_SumDet;	// 0x0 = Summary, 0x01 = Detail
};


void 	getIrSetup( TestSetup& testSetup, BYTE currentUser );
void 	getHvSetup( TestSetup& testSetup, BYTE currentUser );
void    getRunSetup( RunSetup& runSetup );
void 	getReportSetup( ReportSetup& rptSetup );

#endif
