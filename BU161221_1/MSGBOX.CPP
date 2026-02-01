#if !defined( __USES_H )
#include "uses.h"
#endif

#if !defined( __COMMON_H )
#include "common.h"
#endif

#if !defined( __DEFTYPE_H )
#include "deftype.h"
#endif

#if !defined( __STDARG_H )
#include <stdarg.h>
#endif  // __STDARG_H

#if !defined( __STDIO_H )
#include <stdio.h>
#endif  // __STDIO_H

#if !defined( __STRING_H )
#include <string.h>
#endif  // __STRING_H

//extern BYTE		SystemStatus;//Commeneted by:sdt:16012006:2150
BYTE				commandOKDisable = False;//,lastStatus;//Commeneted by:sdt:16012006:2150

static const char *buttonName[] =
{
	MsgBoxText::yesText,
	MsgBoxText::noText,
	MsgBoxText::okText,
	MsgBoxText::cancelText,
	// Hvir Button Text

	"R~e~set",
	"~R~epeat",
	"~S~kip",
	"Con~t~inue",
};

static ushort commands[] =
{
	cmYes,
	cmNo,
	cmOK,
	cmCancel,
	// Hvir specific commands. They are equal to cmYes, cmNo, cmOK resp.
	cmReset,
	cmRepeat,
	cmSkip,
	cmContinue,
};

static const char *Titles[] =
{
	MsgBoxText::warningText,
	MsgBoxText::errorText,
	MsgBoxText::informationText,
	MsgBoxText::confirmText
};

ushort messageBoxRect(const TRect &r,
                      ushort aOptions,
                      const char *msg, ...) noexcept

{
	TDialog *dialog;
	short i, x, buttonCount;
	TView* buttonList[6];//5];
	ushort ccode;

	//dialog = new TSDialog( r, Titles[aOptions & 0x3] );
	dialog = new TDialog( r, Titles[aOptions & 0x3] );
	dialog->insert(
		new TStaticText(TRect(3, 2, dialog->size.x - 2, dialog->size.y - 3),
						msg) );

	//for( i = 0, x = -2, buttonCount = 0; i < 4; i++ )
	for( i = 7, x = -2, buttonCount = 0; i >=0; i-- ) //i=6
		{
		if( (aOptions & (0x0100 << i)) != 0)
			{
			buttonList[buttonCount] =
				new TButton( TRect(0, 0, 12, 2), buttonName[i], commands[i], bfNormal );
			x += buttonList[buttonCount++]->size.x + 2;
			}
		}

	x = (dialog->size.x - x) / 2;

	for( i = 0; i < buttonCount; i++ )
		{
		dialog->insert(buttonList[i]);
		buttonList[i]->moveTo(x, dialog->size.y - 3);
		x += buttonList[i]->size.x + 2;
		}

	dialog->selectNext(False);

	ccode = TProgram::deskTop->execView(dialog);

	TObject::destroy( dialog );

	//::SystemStatus = lastStatus;//Commented by:sdt:16012006:2150

	if ( commandOKDisable == True )
		TProgram::deskTop->disableCommand( cmOK );
	return ccode;
}

//ushort messageBoxRect( const TRect &r,
//					   ushort aOptions,
//					   const char *fmt,
//					   ... )
//{
//	va_list argptr;
//	va_start( argptr, aOptions );
//
//    char msg[256];
//    vsprintf( msg, fmt, argptr );
//
//	if ( aOptions & mfError )
//	{
//	printf("\a");
//	fflush(stdout);
//
//	}
//    return messageBoxRect( r, msg, aOptions );
//}

static TRect makeRect()
{
	//TRect r( 0, 0, 40, 9 );
	TRect r( 0, 0, 45, 10 );
    r.move((TProgram::deskTop->size.x - r.b.x) / 2,
           (TProgram::deskTop->size.y - r.b.y) / 2);
    return r;
}

ushort messageBox( const char *msg, ushort aOptions )
{
	TCommandSet temp = TProgram::deskTop->curCommandSet;
	TProgram::deskTop->enableCommand( cmOK );
	if ( temp != TProgram::deskTop->curCommandSet )
		commandOKDisable = True;

	//lastStatus = ::SystemStatus;//Commented by:sdt:16012006:2150
	//::SystemStatus = STATUS_OTHERVIEW;//Commented by:sdt:16012006:2150
    return messageBoxRect( makeRect(), msg, aOptions );
}

ushort messageBox( ushort aOptions, const char *fmt, ... )
{
    va_list argptr;
	va_start( argptr, aOptions );

    char msg[256];
	vsprintf( msg, fmt, argptr );

	//lastStatus = ::SystemStatus;//Commented by:sdt:16012006:2150
	//::SystemStatus = STATUS_OTHERVIEW;//Commented by:sdt:16012006:2150
    return messageBoxRect( makeRect(), msg, aOptions );
}

ushort inputBox( const char *Title, const char *aLabel, char *s, uchar limit )
{
    TRect r(0, 0, 60, 8);
    r.move((TProgram::deskTop->size.x - r.b.x) / 2,
           (TProgram::deskTop->size.y - r.b.y) / 2);
	return inputBoxRect(r, Title, aLabel, s, limit);
}

ushort inputBoxRect( const TRect &bounds,
                     const char *Title,
                     const char *aLabel,
                     char *s,
					 uchar limit )
{
	TDialog *dialog;
	TView* control;
	TRect r;
	ushort c;

	dialog = new TDialog(bounds, Title);

    r = TRect( 4 + strlen(aLabel), 2, dialog->size.x - 3, 3 );
    control = new TInputLine( r, limit );
    dialog->insert( control );

	r = TRect(2, 2, 3 + strlen(aLabel), 3);
    dialog->insert( new TLabel( r, aLabel, control ) );

	r = TRect( dialog->size.x - 24, dialog->size.y - 4,
               dialog->size.x - 14, dialog->size.y - 2);
    dialog->insert( new TButton(r, MsgBoxText::okText, cmOK, bfDefault));

	r.a.x += 12;
    r.b.x += 12;
    dialog->insert( new TButton(r, MsgBoxText::cancelText, cmCancel, bfNormal));

    r.a.x += 12;
    r.b.x += 12;
    dialog->selectNext(False);
	dialog->setData(s);
    c = TProgram::deskTop->execView(dialog);
    if( c != cmCancel )
        dialog->getData(s);
    TObject::destroy( dialog );
    return c;
}


