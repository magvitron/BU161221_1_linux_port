//
// Header file for general Utilities
//
// keyBoard Interrupt for F7
#ifdef __cplusplus
	#define __CPPARGS ...
#else
	#define __CPPARGS
#endif

#if !defined( __USES_H )
#include "uses.h"
#endif

#if !defined( __STDIO_H )
#include <stdio.h>
#endif


#if !defined( __DEFTYPE_H )
#include "deftype.h"
#endif


#if !defined( __UTILS_H )
#define __UTILS_H

#include "password.h"

#include <string.h>

#define Cancel 			2000

#define timerExpired( timerVariable ) ( timerVariable != 0 )

/***************************************************************************/
/*******  vivek below **********/
void 	resetMsg( int x1,int y1,int x2, int y2,char *msg,char *headmsg );
void 	removeResetMsg( void );
/*******  vivek above **********/

void 	accessMsg( char *msg );

void GetSeed(char *pszPassword, BYTE *pucSeed1, BYTE *pucSeed2);  // added vivek
void EncodeFile(FILE *fpSrc, FILE *fpDest); // added vivek
void DecodeFile(FILE *fpSrc, FILE *fpDest); //added vivek

void 	setBlink();
BYTE	checkSystem( void );
int 	getFactor( int formula );
int GetPrivateProfileString(char *szAppName,char *szKeyName,char *szResult,
					char *szDefault, char *szFileName);
int GetPrivateProfileString(char *szAppName,char *szKeyName,char *szResult,
					char *szDefault, char *szFileName, int nDefault);


int     GetPrivateProfileInt(char *szAppName,char *szKeyName, int nDefault,
					 char *szFileName);
unsigned  int startTimer( unsigned long timeValue, unsigned char *pdoneFlag );
void   cancelTimer( void );
void   Sleep( unsigned int mstime );
int 	  Remove( char *fileName );
void	  ftos( double f, char *temp, int width, int dec, char round );
double round( double f, int width, int dec );
Boolean isValidDatFile( char* fileName );

/***************************************************************************/

class TDynamicText : public TStaticText
{
	public :
		TDynamicText (const TRect& r, char *aText);
		void setText (char *aText);
		TPalette& getPalette () const;
};

class SetBit
{
public:
	SetBit( unsigned short size );
	SetBit( const SetBit& bs );
	~SetBit();
	void operator = ( const SetBit& bs );
	unsigned short Size() const;
	unsigned char GetData(int i) const;
	void SetData(int i, unsigned char data) const;
	void Include( unsigned short bit );
	void Exclude( unsigned short bit );
	void AllOn ();
	void AllOff ();
	int operator [] ( unsigned short bit ) const;

protected:
	unsigned short Length;
	unsigned char *Data;
	SetBit();
};

/************** Vivek below ***************/

const int TopLeft = 201,TopRight = 187,BottomLeft = 200, BottomRight = 188;
const int Top = 205,Side = 186;

class myp{
		protected:
			int x,y;
		public :
			myp(){
				y = x = 2;
			}
			myp(int x1){
            y = x = x1;
			}
			myp(int x1,int y1){
				x = x1; y = y1;
			}
			void setpoint(int x1,int y1);
			void setpoint(int x1);
			void getpoint(int& x1,int& y1);
			int getmyx();
			int getmyy();

};
class mywin : public myp {

protected :
          int x1,y1,x2,y2;
          int fColor,bColor;
public :
       void  draw( char * );
       mywin(int x11,int y11,int x21, int y21);
};

/************** Vivek above ***************/

class TResetWindow : public TWindow
{
public:
	TResetWindow( TRect bounds, char *aTitle );
	~TResetWindow( void );
	void drawText( char *text = "Please Wait", ushort x=1, ushort y=1, ushort attrs = 20 );
};

#endif

