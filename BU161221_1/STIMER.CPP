#include "borland_compat.h"

#include <dos.h>
#include "uses.h"
#include "stimer.h"

//	Interrupt 0x15 function 0x83 for count down in microseconds.
/*	Start countdown timer for n microseconds.
	Return 0 = timer started, or > 0 = timer not started because
	another is already running.	*/
//
unsigned int startTimer( unsigned long timeValue, unsigned char *pdoneFlag )
{
	union REGS 		regs;
	struct SREGS 	sregs;

	*pdoneFlag = 0;				// flag starts clear
	regs.h.ah = 0x83;			// AH = 83h - start/cancel timer
	regs.h.al = 0;				// AL = 0 to set a timer
								// CX = high 16 bits of timeout
	regs.x.cx = (unsigned int) (((((unsigned long)(timeValue)) >> 16) & 0xffff));
								// DX = low 16 bits of timeout
	regs.x.dx = (unsigned int) ((unsigned long)(timeValue));
	regs.x.bx = FP_OFF( pdoneFlag );	// BX = flag offset
	sregs.es = FP_SEG( pdoneFlag );		// ES = flag segment
	int86x( 0x15, &regs, &regs, &sregs );	// call BIOS

	return( regs.x.cflag );		// returns Carry flag
}

//
// cancel timer
//
void cancelTimer( void )
{
	union REGS regs;

	regs.h.ah = 0x83;			// AH = 83h - start/cancel timer
	regs.h.al = 1;				// AL = 1 cancel any active timer

	int86( 0x15, &regs, &regs );
}
