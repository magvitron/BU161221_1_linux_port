//
//	Uses.H - #define all required Uses_ and #include of tv.h
//			used for Turbo Vision
//
#if !defined( __USES_H )

#define __USES_H
#ifndef _NEAR
#define _NEAR
#endif

#ifndef _FAR
#define _FAR
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif
#ifndef O_TEXT
#define O_TEXT 0
#endif

#define Uses_TApplication
#define Uses_ofpstream
#define Uses_ifpstream
#define Uses_TStreamableClass
#define Uses_TListBox
#define Uses_TCheckBoxes
#define Uses_TScrollBar
#define Uses_TCollection
#define Uses_TProgram
#define Uses_TPoint
#define Uses_TCollection
#define Uses_TLabel
#define Uses_TProgInit
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDeskTop
#define Uses_TMenuBar
#define Uses_TMenu
#define Uses_TMenuItem
#define Uses_TSubMenu
#define Uses_MsgBox
#define Uses_TKeys
#define Uses_TDialog
#define Uses_TStaticText
#define Uses_TButton
#define Uses_TView
#define Uses_TEvent
#define Uses_TWindow
#define Uses_TRect
#define Uses_TInputLine
#define Uses_TRadioButtons
#define Uses_TBackground
#define Uses_TFileDialog
#define Uses_TEditWindow
#define Uses_TSItem
#define Uses_TTerminal
#define Uses_otstream
#define Uses_fpstream
#define Uses_ipstream
#define Uses_TPalette
#define Uses_TDrawBuffer

#include <tvision/tv.h>
#include <tvision/tobjstrm.h>   // <-- REQUIRED here
//#include <tvision/pstream.h>
//#include <tv.h>
#include <strings.h>
#ifndef strcmpi
#define strcmpi(a,b) strcasecmp((a),(b))
#endif

inline void biosprint(int, int, int) {}

#ifndef LEFT
#define LEFT   0
#endif
#ifndef CENTER
#define CENTER 1
#endif
#ifndef RIGHT
#define RIGHT  2
#endif


#endif

