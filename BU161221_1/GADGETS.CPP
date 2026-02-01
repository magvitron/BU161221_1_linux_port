/*-------------------------------------------------------------------*/
/*                                                                   */
/*   Turbo Vision 1.0                                                */
/*   Turbo Vision Demo                                               */
/*   Copyright (c) 1991 by Borland International                     */
/*                                                                   */
/*   Gadgets.cpp:  Gadgets for the Turbo Vision Demo.  Includes a    */
/*        heap view and a clock view which display the clock at the  */
/*        right end of the menu bar and the current heap space at    */
/*        the right end of the status line.                          */
/*-------------------------------------------------------------------*/
#include "uses.h"

#define Uses_TRect
#define Uses_TView
#define Uses_TDrawBuffer

//#include <tv.h>
#include <tvision/tv.h>
#include <tvision/tobjstrm.h>   // <-- REQUIRED here

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strstrea.h>
#include <iomanip.h>
#include <alloc.h>
#include <time.h>

#include "gadgets.h"

#include <climits>

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

//
// ------------- Heap Viewer functions
//

THeapView::THeapView(TRect& r) : TView( r )
{
    oldMem = 0;
    newMem = heapSize();
}


void THeapView::draw()
{
    TDrawBuffer buf;
    char c = getColor(2);

    buf.moveChar(0, ' ', c, size.x);
    buf.moveStr(0, heapStr, c);
    writeLine(0, 0, size.x, 1, buf);
}


void THeapView::update()
{
    if( (newMem = heapSize()) != oldMem )
        {
        oldMem = newMem;
        drawView();
        }
}



long THeapView::heapSize()
{
#ifdef __linux__
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        unsigned long long freeBytes =
            (unsigned long long) info.freeram *
            (unsigned long long) info.mem_unit;

        if (freeBytes > (unsigned long long)LONG_MAX)
            return LONG_MAX;

        return (long) freeBytes;
    }
    return 0;
#else
    // Original Borland DOS heap code (kept for reference, not used on Linux)
    long total = farcoreleft();
    struct farheapinfo heap;
    switch (farheapcheck())
    {
        case _HEAPEMPTY:   break;
        case _HEAPCORRUPT: break;
        case _HEAPOK:
            while (farheapwalk(&heap) != _HEAPEND)
                if (heap.in_use == 0)
                    total += heap.size;
            break;
    }
    return total;
#endif
}

//
// -------------- Clock Viewer functions
//

TClockView::TClockView( TRect& r ) : TView( r )
{
    strcpy(lastTime, "        ");
    strcpy(curTime, "        ");
}


void TClockView::draw()
{
    TDrawBuffer buf;
    char c = getColor(2);

    buf.moveChar(0, ' ', c, size.x);
    buf.moveStr(0, curTime, c);
    writeLine(0, 0, size.x, 1, buf);
}

void TClockView::update()
{
    time_t t = time(0);
    char *date = ctime(&t);

    date[19] = '\0';
    strcpy(curTime, &date[11]);        /* Extract time. */

    if( strcmp(lastTime, curTime) )
        {
        drawView();
        strcpy(lastTime, curTime);
        }
}

