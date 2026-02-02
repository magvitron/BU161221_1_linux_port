#if !defined( __PWD_H )
#define __PWD_H
#include <cstring>   // for std::strlen, std::strcpy
#include "uses.h"

#define MAX_LOGIN_LEN		15
#define MAX_PASSWORD_LEN		20
#define MAX_USERS			3

#define ACCESS_DENIED		0xff

#define U_SIVAENGG			0
#define U_SUPERUSER			1
#define U_OPERATOR			2

//#define COMPANY_NAME	"Sivananda Electronics"
//#define SIGNATURE		"HV#SNM#07071999"


struct User
{
	char userType;
	char loginName[MAX_LOGIN_LEN + 1];
	char Password[MAX_PASSWORD_LEN + 1];
};




class TPWDInputLine : public TInputLine
{
public :
	//TPWDInputLine( TRect& r, int nLen, char *tpwd ) : TInputLine( r, nLen )
//	TPWDInputLine( TRect& r, int nLen, char *tpwd )
TPWDInputLine(const TRect &bounds, int limit, char *aText)
    : TInputLine(bounds, limit, nullptr, ilMaxBytes)
{
    // Allocate based on limit (what the control may write), not strlen(aText).
    passEntered = new char[limit + 1];
    passWord    = new char[limit + 1];

    // Clear the whole buffers including the terminating '\0'.
    memset(passEntered, 0, limit + 1);
    memset(passWord,    0, limit + 1);

    // If initial text is provided, copy safely (bounded).
    if (aText)
        strncpy(passEntered, aText, limit);  // leaves null terminator because buffers are zeroed
}

   virtual void getData( void *rec );
   virtual void setData( void *rec );
   ~TPWDInputLine( void )
   {
	delete [] passEntered;
	delete [] passWord;
   }
protected :
	void handleEvent( TEvent& ev );

public :
	char *passWord;
	char *passEntered;
};
Boolean writeFile( int noofUsers,  User *User, char *lastLogin );
Boolean readFile( int noofUsers, User *User, char *lastLogin );
Boolean getPassword( User& currentUser );
//Boolean getExitPassword( const User& currentUser );
Boolean getExitPassword( const User& currentUser );

#endif
