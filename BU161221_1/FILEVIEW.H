/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision 1.0                                      */
/*   Copyright (c) 1991 by Borland International           */
/*                                                         */
/*   Fileview.h:  Header file for fileview.cpp.            */
/*---------------------------------------------------------*/

#if !defined( __FILEVIEW_H )
#define __FILEVIEW_H

#define Uses_TCollection
#define Uses_TScroller
#define Uses_TWindow
#define Uses_TScrollBar
#define Uses_TRect
#define Uses_TStreamable
#define Uses_TStreamableClass
#define Uses_TObjStrm
#define Uses_TWindowInit   // <-- IMPORTANT (for TWindowInit used in FILEVIEW.CPP)

#include <tvision/tv.h>

// Forward declare streams (header only needs references).
class ipstream;
class opstream;

const ushort hlChangeDir = cmChangeDir;


class TLineCollection : public TCollection
{

public:

    TLineCollection(short lim, short delta) : TCollection(lim, delta) {}
    virtual void  freeItem(void *p) { delete p; }

private:

    virtual void *readItem( ipstream& ) { return 0; }
    virtual void writeItem( void *, opstream& ) {}

};

class TFileViewer : public TScroller
{

public:

    char *fileName;
    TCollection *fileLines;
    Boolean isValid;
    TFileViewer( const TRect& bounds,
                 TScrollBar *aHScrollBar,
                 TScrollBar *aVScrollBar,
                 const char *aFileName
               );
    ~TFileViewer();
    TFileViewer( StreamableInit ) : TScroller(streamableInit) { };
    void draw();
    void readFile( const char *fName );
    void setState( ushort aState, Boolean enable );
    void scrollDraw();
    Boolean valid( ushort command );

private:

    virtual const char *streamableName() const
        { return name; }

protected:

//    virtual void write( opstream& );
  //  virtual void *read( ipstream& );

public:

    //static const char * const name;
   // static TStreamable *build();

};

class TFileWindow : public TWindow
{

public:

	TFileWindow( const char *fileName );
	TFileWindow( const char *fileName , const char *Title);

};

//extern const int maxLineLength;
//extern const int maxLineLength;

#endif

