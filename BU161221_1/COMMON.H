#if !defined(__COMMON_H)
#define __COMMON_H
// which session DEBUG or RELEASE
#define	DEBUG
#undef       DEBUG
#include <unistd.h>
// System status
#define STATUS_OK			1
#define STATUS_RESET		2
#define STATUS_PROCESS		3
#define STATUS_OTHERVIEW		4		// a crude method

//#include "ct.h"
// formula for incrementing factor
#define FORMULA_1			1
#define FORMULA_2			2
#define FORMULA_3			3

#define MAX_CONDUCTORS    	202
#define SHEATH				203

//#define RESET_STATUS		RESULT_X | HV_OFF & LV_SET & GD_SET//Commented by:sdt:28092005:1125
#define RESET_STATUS  		0x00

// Status of Control Lines for Conductors
#define HV_SET				0x10
#define LV_SET				0x04
#define GD_SET				0x01
#define HV_OFF				0x20
#define LV_OFF				0x08
#define GD_OFF				0x02
#define HV_X				0x00		// _X - Don't Care
#define LV_X				0x00
#define GD_X				0x00
#define PASS				0x80
#define FAIL				0x40
#define RESULT_X			0x00

#define cmFrontPage			180

#define hcRunAllTnR			0x8001
#define hcTesting			0x8002
#define hcTested			0x8003
#define hcStartTesting		0x8004
#define hcStopTesting		0x8005
#define hcCalibrate			0x8006
#define hcCalibrateOver		0x8007
#define hcDataDirectory		0x8008
#define hcViewReport          0x8009 //Added by :sdt:07122005:0915
// because this can used by many files for disabling
#define cmResume				154

// HELP Ir Test
#define hcAutoRange		0x9000
#define hcAutoCal		0x9001
#define hcFindOffset	0x9002
#define hcIrSelfCheck	0x9003
#define hcRunInd		0x9004
#define hcRunCnS		0x9005


#define cmRepeat			10
#define cmContinue			13
#define cmSkip				12
#define cmReset				12//14

#define mfRepeatButton		0x1000
#define mfContinueButton	0x2000
#define mfSkipButton		0x4000
#define mfResetButton		0x8000

#define cmResetSystem   		151

#define RDWR                  1
#define NOEDIT				2

#define COMPANY_NAME	"ePoint Technologies"
#define SIGNATURE		"CTS#SDT#27072005"

static inline int eof_fd(int fd)
{
    char c;
    ssize_t n = read(fd, &c, 1);
    if (n == 0) return 1;          // EOF
    if (n < 0) return 0;           // error treated as not-eof here
    lseek(fd, -1, SEEK_CUR);       // put byte back
    return 0;
}
struct Header
{
	char szSignature[20];
	char szCompany[22];
};

#endif
