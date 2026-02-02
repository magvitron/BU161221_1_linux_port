#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <io.h>
#include <cstring>

#include "ct.h"
#include "password.h"
#include "common.h"
#include "prodini.h"
#include "deftype.h"


Boolean writeFile( int noofUsers, User *user, char *lastLogin )
{
	Header Hdr = { SIGNATURE, COMPANY_NAME };
	fstream s( "rest.inf", ios::binary | ios::in | ios::out );
	if ( s.bad() )
	{
		messageBox( "Cannot open Security File...", mfError|mfOKButton );
		::remove( "hvir.ini" );
		exit(1);
	}
	s.seekg( ios::beg );
	if ( s.bad() || s.eof() || s.fail() )
	{
		s.close();
		return False;
	}
	s.write( (char *)((Header*)&Hdr), sizeof( Header ) );
	if ( s.bad() || s.eof() || s.fail() )
	{
		s.close();
		return False;
	}
	for( int i = 0; i < noofUsers; i++ )
	{
		s.write( (char *)((User*)&user[i]), sizeof( User ) );
		if ( s.bad() || s.eof() || s.fail() )
		{
			s.close();
			return False;
		}
	}
	s.write( (char *)lastLogin, MAX_LOGIN_LEN );
	if ( s.bad() || s.eof() || s.fail() )
	{
		s.close();
		return False;
	}
	return True;
}

Boolean readFile( int noofUsers, User *user, char *lastLogin )
{
	Header Hdr = { SIGNATURE, COMPANY_NAME };
	fstream s( "rest.inf", ios::binary | ios::in );
	if ( s.bad() )
	{
		messageBox( "Cannot open Security File...", mfError|mfOKButton );
		::remove( "hvir.ini" );
		exit(1);
	}
	s.seekg( ios::beg );
	if ( s.bad() || s.eof() || s.fail() )
	{
		s.close();
		return False;
	}
	s.read( (char *)((Header*)&Hdr), sizeof( Header ) );
	if ( s.bad() || s.eof() || s.fail() )
	{
		s.close();
		return False;
	}
	for( int i = 0; i < noofUsers; i++ )
	{
		s.read( (char *)((User*)&user[i]), sizeof( User ) );
		if ( s.bad() || s.eof() || s.fail() )
		{
			s.close();
			return False;
		}
	}
	s.read( (char *)lastLogin, MAX_LOGIN_LEN );
	if ( s.bad() || s.eof() || s.fail() )
	{
		s.close();
		return False;
	}
	return True;
}

Boolean getPassword( User& currentUser )
{
	BYTE userType = ACCESS_DENIED;
	char lgn[MAX_LOGIN_LEN+1], pwd[MAX_PASSWORD_LEN+1];

	User users[MAX_USERS];
	char lastLogin[MAX_LOGIN_LEN+1];

	memset(lgn, 0, sizeof(lgn));
	memset(pwd, 0, sizeof(pwd));
	memset( users, 0, sizeof( users ) );
	printf(lgn);
	printf(pwd);
	// printf(users[0]);
	dec( 3, "erest.inf", "rest.inf", "" );
	memset( lastLogin, 0, MAX_LOGIN_LEN+1 );
	readFile( MAX_USERS, users, lastLogin );

	TDialog *pdlg = new TDialog( TRect( 10, 5, 55, 14 ), "Entry" );
	pdlg->options = ofCentered;
	pdlg->insert( new TButton( TRect( 14, 6, 21, 8 ), " ~O~K ", cmOK, bfDefault ));
	pdlg->insert( new TButton( TRect( 23, 6, 34, 8 ), " ~C~ancel ", cmCancel, bfNormal ));

	TStaticText *stlogin = new TStaticText( TRect( 5, 2, 20, 3 ), "Login Name : " );
	pdlg->insert( stlogin );

	TStaticText *stpassword = new TStaticText( TRect( 5, 4, 20, 5), "  Password : " );
	pdlg->insert( stpassword );

	TInputLine *login = new TInputLine( TRect( 20, 2, 40, 3 ), MAX_LOGIN_LEN+1 );
	login->setData( (void *)lastLogin );
	pdlg->insert( login );

	TPWDInputLine *password = new TPWDInputLine( TRect( 20, 4, 40, 5 ), MAX_PASSWORD_LEN, ""  );
	pdlg->insert( password );
	password->setData( (void*)"" );   // empty string, valid pointer

	password->select();
	int cmd = TProgram::deskTop->execView(pdlg);
	if ( cmd == cmOK )
	{
		login->getData( (void*)lgn );
		password->getData( (void*)pwd );
		TProgram::deskTop->destroy( pdlg );
		for( int i = 0; i < MAX_USERS; i++ )
		{
			if ( (strcmp(users[i].loginName, lgn) == 0) &&
				 (strcmp(users[i].Password, pwd) == 0) )
			{
				currentUser.userType = userType = i;
				strcpy( currentUser.loginName, lgn );
				strcpy( currentUser.Password, pwd );
				if ( userType != U_SIVAENGG )
					strcpy( lastLogin, lgn );
				else
					strcpy( lastLogin, users[U_SUPERUSER].loginName );
				break;
			 }
		}
		if ( userType != ACCESS_DENIED )
		{
			writeFile( MAX_USERS, users, lastLogin );
			::enc( 3, "rest.inf", "erest.inf", "" ); //Uncommented by:sdt:08122005:1230
			::remove( "rest.inf" );
			return True;
		}
	}
	::remove( "rest.inf" );
	return False;
}

Boolean getExitPassword( const User& currentUser )
{
	char pwd[MAX_PASSWORD_LEN];

	TDialog *pdlg = new TDialog( TRect( 0, 0, 30, 5 ), "Exit" );
	pdlg->options = ofCentered;

	TStaticText *txt = new TStaticText( TRect( 2, 2, 12, 3 ), "Password :"  );
	pdlg->insert( txt );

	TPWDInputLine *password = new TPWDInputLine( TRect( 13, 2, 27, 3 ), MAX_PASSWORD_LEN, ""  );
	pdlg->insert( password );
	password->setData( (void*)"" );   // empty string, valid pointer
	password->select();
	int cmd = TProgram::deskTop->execView(pdlg);
	if ( cmd == cmOK )
	{
		password->getData( (void*)pwd );
		TProgram::deskTop->destroy( pdlg );
		if ( strcmp(currentUser.Password, pwd) == 0 )
			return True;
		else
			return False;
	}
	else
		return (Boolean)cmCancel;
}


void TPWDInputLine::handleEvent( TEvent& ev )
{
	static char *pwd = new char[ maxLen ];

	TView::handleEvent( ev );
	if ( ev.what == evMouseDown )
	{
		clearEvent( ev );
		return;
	}
	if ( ev.what == evKeyboard )
	{
		strcpy( pwd, passEntered );
		strcpy( data, passEntered );
		if ( ev.message.command == kbEnter )
		{
			if ( strncmp( pwd, passWord, maxLen ) != 0 )
			{
				ev.what = evCommand;
				ev.message.command = cmOK;
				putEvent( ev );
			}
			else
			{
				ev.what = evCommand;
				ev.message.command = cmCancel;
				putEvent( ev );
			}
			clearEvent( ev );
		}
		else if ( ev.message.command != kbEsc )
		{
			TInputLine::handleEvent( ev );
			strcpy( pwd, data );
			memset( data, '*', strlen(data) );
			//strset( data, '*' );
			drawView();
			if ( (ev.message.command != kbTab) && (ev.message.command != kbShiftTab) )
			{
				strcpy( data, pwd );
			}
			strcpy( passEntered, pwd );
		}
		else if ( ev.message.command == kbEsc )
		{
			ev.what = evCommand;
			ev.message.command = cmNo;
			putEvent( ev );
		}
		memset( data, '*', strlen(data) );
		
//strset( data, '*' );
	}
}

void TPWDInputLine::getData( void *rec )
{
   strcpy( (char*)rec, passEntered );
}

void TPWDInputLine::setData(void *rec)
{
    // If rec is NULL, treat it as empty string.
    const char *src = (rec != nullptr) ? static_cast<const char*>(rec) : "";

    // If passEntered wasn't allocated, bail out safely.
    if (!passEntered)
        return;

    // Copy safely into passEntered (limit comes from TInputLine::maxLen typically).
    // If you don't have access to maxLen, store `limit` in your class as a member.
    int cap = this->maxLen;          // or whatever TInputLine uses
    if (cap < 0) cap = 0;

    std::strncpy(passEntered, src, cap);
    passEntered[cap] = '\0';

    // Optional: also keep passWord consistent if you use it as masked display
    if (passWord) {
        std::memset(passWord, '*', std::strlen(passEntered));
        passWord[std::strlen(passEntered)] = '\0';
    }

    // If you want the underlying input line to display '*' masking,
    // you'd typically set TInputLine's internal buffer too.
    // But at minimum: do not read NULL, do not overflow.
}


