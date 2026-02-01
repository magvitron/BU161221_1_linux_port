/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision 1.0                                      */
/*   Turbo Vision FileViewer Demo Support File             */
/*   Copyright (c) 1991 by Borland International           */
/*                                                         */
/*---------------------------------------------------------*/
/*---------------------------------------------------------*/
#undef __INC_TOBJSTRM_H
#define Uses_TStreamable
#define Uses_TStreamableClass
#define Uses_TObjStrm

#define Uses_MsgBox
#define Uses_TKeys
#define Uses_TScroller
#define Uses_TDrawBuffer
#define Uses_TRect
#define Uses_TProgram
#define Uses_TDeskTop
#define Uses_TStreamableClass
#define Uses_TObjStrm
#define Uses_TWindowInit

#include <tvision/tv.h>
#include <tvision/tobjstrm.h>   // keep this for clarity
#include "fileview.h"


#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <fstream>

__link(RScroller)
__link(RScrollBar)

//const char * const TFileViewer::name = "TFileViewer";
static int winNumber = 0;
//static_assert(sizeof(ipstream) > 0, "ipstream incomplete");
//static_assert(sizeof(opstream) > 0, "opstream incomplete");


TFileViewer::TFileViewer( const TRect& bounds,
                          TScrollBar *aHScrollBar,
                          TScrollBar *aVScrollBar,
                          const char *aFileName) :
    TScroller( bounds, aHScrollBar, aVScrollBar )
{
    growMode = gfGrowHiX | gfGrowHiY;
    isValid = True;
	fileName = 0;
	//TVMemMgr *memMgr = new TVMemMgr();
	//memMgr->resizeSafetyPool(2048000l);
    readFile( aFileName );
}

TFileViewer::~TFileViewer()
{
     delete[] fileName;
     destroy (fileLines);
}

void TFileViewer::draw()
{
    char *p;

	ushort c =  getColor(0x0301);
	for( int i = 0; i < size.y; i++ )
        {
        TDrawBuffer b;
	b.moveChar( 0, ' ', c, size.x );

        if( delta.y + i < fileLines->getCount() )
            {
			char s[256];
            p = (char *)( fileLines->at(delta.y+i) );
            if( p == 0 || strlen(p) < delta.x )
                s[0] = EOS;
            else
		{
		strncpy( s, p+delta.x, size.x );
		if( strlen( p + delta.x ) > size.x )
                    s[size.x] = EOS;
                }
            b.moveStr( 0, s, c );
            }
        writeBuf( 0, i, size.x, 1, b );
	   }
}

void TFileViewer::scrollDraw()
{
    TScroller::scrollDraw();
    draw();
}

void TFileViewer::readFile( const char *fName )
{
    delete[] fileName;

    limit.x = 0;
    fileName = newStr( fName );
    fileLines = new TLineCollection(5, 5);
    std::ifstream fileToView( fName );
    if( !fileToView )
        {
        messageBox( "Invalid drive or directory", mfError | mfOKButton );
        isValid = False;
        }
    else
        {
		char line[256+1];
while ( !lowMemory() && fileToView.getline(line, sizeof line) )
{
    limit.x = max(limit.x, (int)strlen(line));
    fileLines->insert(newStr(line));
}

/*        while( !lowMemory() &&
               !fileToView.eof() && 
               fileToView.get( line, sizeof line ) != 0 
             )
            {
            char c;
            fileToView.get(c);      // grab trailing newline
            limit.x = max( limit.x, strlen( line ) );
            fileLines->insert( newStr( line ) );
            }
*/
        isValid = True;
        }
    limit.y = fileLines->getCount();
}

void TFileViewer::setState( ushort aState, Boolean enable )
{
    TScroller::setState( aState, enable );
    if( enable && (aState & sfExposed) )
        setLimit( limit.x, limit.y );
}

Boolean TFileViewer::valid( ushort )
{
    return isValid;
}
/*
void *TFileViewer::read(ipstream& is)
{
    char *fName;

    TScroller::read(is);
    fName = is.readString();
    fileName = 0;
    readFile(fName);
    delete[] fName; 
    return this;
}

void TFileViewer::write(opstream& os)
{
    TScroller::write(os);
    os.writeString(fileName);
}


TStreamable *TFileViewer::build()
{
    return new TFileViewer( streamableInit );
}


TStreamableClass RFileView( TFileViewer::name,
                            TFileViewer::build,
                              __DELTA(TFileViewer)
                          );



static int winNumber = 0;

TFileWindow::TFileWindow( const char *fileName ) :
	TWindow( TProgram::deskTop->getExtent(), "QUAD Test Report", winNumber++ ),
    TWindowInit( &TFileWindow::initFrame )
{
    options |= ofTileable;
    TRect r( getExtent() );
    r.grow(-1, -1);
    insert(new TFileViewer( r,
                            standardScrollBar(sbHorizontal | sbHandleKeyboard),
                            standardScrollBar(sbVertical | sbHandleKeyboard),
                            fileName) );
}

*/
TFileWindow::TFileWindow( const char *fileName, const char *Title ) :
	TWindow( TProgram::deskTop->getExtent(), Title, winNumber++ ),
	TWindowInit( &TFileWindow::initFrame )
{
	palette = wpCyanWindow;
	options |= ofTileable;
    TRect r( getExtent() );
    r.grow(-1, -1);
    insert(new TFileViewer( r,
                            standardScrollBar(sbHorizontal | sbHandleKeyboard),
                            standardScrollBar(sbVertical | sbHandleKeyboard),
                            fileName) );
}
