//
//	Utils.Cpp - Implementation file for general utilities
//
#include "common.h"
#include "borland_compat.h"

#include "uses.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include "ct.h"

#include "utils.h"
//#include "common.h"//Commented by:sdt:26072005:0055

static 		const char *const cpDynamic = "\x21";
/*extern*/ 		BYTE timerFlag;
extern 		volatile BYTE SystemStatus;

typedef unsigned char  BYTE;   // added vivek


#define MAXBUFFERSIZE	3000   // added vivek
TWindow *pMessage;

//TDynamicText::TDynamicText (TRect& r, char *aText) : TStaticText (r, aText)
TDynamicText::TDynamicText (const TRect& r, char *aText) : TStaticText (r, aText)

{
	return;
}

void TDynamicText::setText (char *aText)
{
	if (text)
		delete (char *)(text);
	text = newStr (aText);
}

TPalette& TDynamicText::getPalette () const
{
	static TPalette palette (cpDynamic, sizeof(cpDynamic)-1);
	return palette;
}

SetBit::SetBit()
{
	Length	= 0;
	Data	= NULL;
}

SetBit::SetBit( unsigned short size )
{
	unsigned short alloc;

	Length = size;
	alloc = ( Length + 7U ) >> 3U;
	Data = new unsigned char[ (unsigned int)alloc ];
	memset( Data, '\x00', (unsigned int )alloc );
}

SetBit::SetBit( const SetBit& bs )
{
	unsigned short alloc;

	Length = bs.Length;
	alloc = ( bs.Length + 7U ) >> 3U;
	Data = new unsigned char[ (unsigned int)alloc ];
	memcpy( Data, bs.Data, (unsigned int)alloc );
}

SetBit::~SetBit()
{
	if ( Data )
		delete [] Data;
}

unsigned short SetBit::Size() const
{
	return Length;
}

unsigned char SetBit::GetData(int i) const
{
	if ( i > (Length >> 3 ) )
		return 0;
	return Data[i];
}

void SetBit::SetData(int byte, unsigned char data) const
{
	if ( byte > (Length >> 3 ) )
		return;
	Data[ byte ] = data;
}


void SetBit::Include( unsigned short int bit )
{
	if ( bit < Length )
		Data[ bit >> 3 ] |= (unsigned char)( 1 << (bit & 7) );
}

void SetBit::Exclude( unsigned short int bit )
{
	if ( bit < Length )
		Data[ bit >> 3 ] &= ~(unsigned char)( 1 << (bit & 7) );
}

void SetBit::AllOn()
{
	memset( Data, '\xFF', (Length + 7) >> 3 );
}

void SetBit::AllOff()
{
	memset( Data, '\x00', (Length + 7) >> 3 );
}

int SetBit::operator [] ( unsigned short bit ) const
{
	if ( bit < Length )
		return ( Data[ bit >> 3 ] & (1 << (bit & 7) ));
	else
		return 0;
}

void accessMsg( char *msg )
{
	TDialog *pDlg = new TDialog( TRect( 0, 0, 35, 7 ), "Message" );
	pDlg->options = ofCentered;

	TDynamicText* pDt = new TDynamicText (TRect (1, 2, 34, 3), msg);
	pDlg->insert (pDt);

	pDlg->insert( new TButton (TRect (13, 4, 19, 6), "~O~K", cmCancel, bfNormal) );

	TProgram::deskTop->execView(pDlg);

	delete pDlg;
	delete pDt;
}

int getFactor( int formula )
{
	if ( formula == FORMULA_1 )
		return 1;
	else if ( formula == FORMULA_2 )
		return 2;
	else if ( formula == FORMULA_3 )
		return 3;

	return 1;
}

BYTE checkSystem( void )
{
	TEvent ev;
	ev.getKeyEvent();
	if ( ev.keyDown.keyCode == kbF7 )
		return STATUS_RESET;

	return STATUS_OK;
}

//
// function returns the string from the specified file,
// under given application key and against the given key name
//

int GetPrivateProfileString(char *szAppName,char *szKeyName,char *szResult,
							char *szDefault, char *szFileName)
{
	char 	szTmp[80];
	int  	appFound = FALSE;
	int 	keyFound = FALSE;

	//ifstream ins( szFileName, ios::in | ios::nocreate );
	ifstream ins( szFileName, ios::in );
	if (!ins.is_open())
	    return 0;  // or whatever your "file not found" behavior is

	if ( !ins )
		return FALSE;

	while ( !ins.eof() )
	{
		ins.getline( szTmp, 80, '\n' );
		if  ( !appFound )
		{
			if ( strstr( szTmp, szAppName ) )
				appFound = TRUE;
		}
		else
		{
			if ( strstr( szTmp, szKeyName ) )
			{
				keyFound = TRUE;
				break;
			}
			else
				if ( strstr( szTmp, "[" ) )
					break;
		}
	}

	ins.close();
	if ( (appFound == TRUE) && (keyFound == TRUE) )
	{
		int i = 0;
		char *temp = szTmp;
		while( isspace( szTmp[i] ) && szTmp[i] != '\0' )
			temp++;
		if ( temp[0] == ';' )
			return FALSE;
		while( *temp != '=' && *temp!='\0' ) temp++;
		if ( *temp == '=' )
		{
			temp++;
			while( isspace( *temp ) && *temp != '\0' )
				temp++;
			if ( *temp == '\"' )
			{
				temp++;
				for( i=0;temp[i] != '\"' && temp[i] != '\0' && i < 80;i++ )
					szResult[i] = temp[i];
				szResult[i] = '\0';
			}
			else
			{
				if ( !strlen( temp ) )
					strcpy( szResult, szDefault );
				else
					strcpy( szResult, temp );
			}
		   return TRUE;
		}
	}
	strcpy( szResult, szDefault );
	return FALSE;
}


int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szStr,
									  char *szDefault,char *FileName,int nDefault)
{
	FILE  *pFile;
	char	cBuf[250], *szTemp;
	int	nLen, i,TokenFound=0;

	if((pFile=fopen(FileName,"rt"))==NULL)
		return FALSE;
	while(fgets(cBuf,250,pFile)!=NULL)
	{
		szTemp=strstr(cBuf,szAppName);
		if(szTemp!=NULL)
			TokenFound=1;
		szTemp=strstr(cBuf,szkeyname);
		if(szTemp!=NULL&&TokenFound)
		{
			szTemp+=strlen(szkeyname);
			nLen=strlen(szTemp);
			i=0;
			while((nLen)&&(*szTemp!='='))
			{
				nLen--;
				szTemp++;
			}
			if(nLen)
			{
				szTemp++;
				nLen=strlen( szTemp);
				i=0;
				if(nDefault)
				{
					 while((nLen)&&(*szTemp==' '))
					 {
						 nLen--;
						 szTemp++;
					 }
				}
				if(nLen)
				{
					if(pFile){fclose(pFile);}
					for(i=0,nLen=strlen(szTemp);nLen&&(*szTemp!='\n')&&(*szTemp!=0)&&(*szTemp!=EOF);szTemp++,i++)
						szStr[i] = *szTemp;
					szStr[i] = 0;
					return i;
				}
			}
		}
	}
	if(pFile){fclose(pFile);}
	return sprintf(szStr,szDefault);
}	// End of GetPrivateProfileString


//
// function returns the integer value from the specified file,
// under given application key and against the given key name
//
int GetPrivateProfileInt(char *szAppName,char *szKeyName, int nDefault,
						 char *szFileName)
{
	char 	szDef[80];
	char 	szRes[80];

//	strset( szRes, '\0' );
	memset(szRes, 0, sizeof(szRes));

	sprintf( szDef, "%d", nDefault );
	GetPrivateProfileString( szAppName, szKeyName, szRes, szDef, szFileName );
	return atoi( szRes );
}

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

double round( double f, int width, int dec )
{
	char temp[20], str[20];
	int i=0;

	sprintf( str, "%f", f );

	while( str[i] != '.' && str[i] != '\0' && i < width ) temp[i] = str[i++];

	if ( dec != -1 )
		if ( i+dec < width )
			width = i+dec;

	if ( i < width )
	{
		temp[i] = '.';
		while( str[i] != '\0' && i <= width )
			temp[i] = str[i++];
	}
	if ( str[i] != '\0' && round )
		if ( str[i] > 5+'0' )
			temp[i-1] += 1;
	temp[i] = '\0';
	return atof( temp );
}

// double value, total width, remaining decimals, whether to round/truncate

void ftos( double f, char *temp, int width, int dec, char round )
{
	char str[20];
	int i = 0;

	sprintf( str, "%f", f );

	while(str[i] != '.'&&str[i] != '\0'&&i < width) temp[i] = str[i++];

	if ( dec != -1 )
		if ( i+dec < width )
			width = i+dec;

	if ( i < width )
	{
		temp[i] = '.';
		while( str[i] != '\0' && i <= width )	temp[i] = str[i++];
	}
	if ( str[i] != '\0' && round )
		if ( str[i] > 5+'0' )
			temp[i-1] += 1;

	temp[i] = '\0';
	return;
}


void cancelTimer( void )
{
	union REGS regs;

	regs.h.ah = 0x83;			// AH = 83h - start/cancel timer
	regs.h.al = 1;				// AL = 1 cancel any active timer

	int86( 0x15, &regs, &regs );
}

int Remove( char *fileName )
{
	return ::remove( fileName );
}

void Sleep( unsigned int mstime )
{
#ifdef DEBUG
	delay( (unsigned int)mstime );
#else
	timerFlag = 0;
	startTimer( (unsigned long)mstime*1000, &timerFlag );
	while( 1 )
		if ( timerFlag )
		{
			cancelTimer();
			break;
		}
#endif
}

// setBlink - This function will set the blinking on if it is reseted by
//			any another program
void setBlink()
{
	union REGS a;

	a.h.ah = 0x10;
	a.h.al = 0x03;
	a.h.bl = 1;

	int86( 0x10, &a, &a );
}


/************** Vivek below ***************/

void resetMsg( int x11,int y11,int x21,int y21,char *msg,char *headmsg )
{
	pMessage = new TWindow( TRect( x11, y11, x21, y21 ), headmsg ,NULL );

	pMessage->flags &= ~(wfClose|wfGrow|wfZoom);
	if ( !strcmp(headmsg,"Information"))
		pMessage->options = ofCentered;
	else
		pMessage->TView::state &= ~sfShadow;

	TDynamicText *pDt = new TDynamicText (TRect (1, 1, x21-x11-1, y21-y11-1), msg);
	pMessage->insert (pDt);

	TProgram::deskTop-> insert(pMessage);
}

void removeResetMsg()
{
	if ( pMessage != 0 )
		pMessage->close();
}
/************** Vivek above ***************/

TResetWindow::TResetWindow( TRect bounds, char *aTitle ) :
			TWindow( bounds, aTitle, wnNoNumber ),
			TWindowInit( TResetWindow::initFrame )
{
	TView::state &= sfShadow;
	flags &= ~(wfClose|wfGrow|wfZoom);
	palette = wpCyanWindow;
}
TResetWindow::~TResetWindow( void )
{
}
void TResetWindow::drawText( char *text, ushort x, ushort y, ushort attrs )
{
	TDrawBuffer b;
	b.moveStr(0, text, attrs);
	writeLine(x, y, strlen(text), 1, b);
}

Boolean isValidDatFile( char* fileName )
{
	Header header;
	ifstream is( fileName, ios::binary );
	if ( is.bad() || is.fail() )
		return False;
	is.read( (char*)((Header*)&header), sizeof( Header ) );
	if ( is.bad() || is.fail() )
	{
		is.close();
		return False;
	}
	if ( strcmp( header.szSignature, SIGNATURE ) == 0 &&
		strcmp( header.szCompany, COMPANY_NAME ) == 0 )
	{
		is.close();
		return True;
	}
	is.close();
	return False;
}
