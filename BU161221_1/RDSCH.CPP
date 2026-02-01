#include <io.h>
#include <mem.h>
#include <alloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "rdsch.h"
#include "common.h"
extern char cRamDrive[2]; //22012001
extern void RemoveIniFiles(void);

extern FIXTURE Fixture[3];

char    *schemeErr[] = {
						"error: Unable to open Pattern.Log\n",
						"error: Unable to open Scheme file\n",
						"errro: #Scheme_Start missing\n",
						"error: Incomplete fixture details\n",
						"error: Specified key missing\n",
						"error: Incomplete pair information\n",
						"error: Incomplete splitter information\n"
						"error: Incomplete string\n"
						};
//int     handleError;
//MOD18012002:05:sdt:1615
FILE *handleError;

static  int     errorOccured = FALSE;
//char    buf[MAX_LINE];
extern int     nFixtures;// = 0;

boolean readScheme()
{
     char path[80];
	int handle;
	char szTemp[ MAX_LINE ];
	char    buf[MAX_LINE];

	//if ( (handleError = open( "Pattern.Log", O_CREAT|O_TRUNC|O_WRONLY|O_TEXT ))==-1)
    //MOD18012002:05:sdt:1615
    if ( (handleError = fopen( "Pattern.Log","wt"))==NULL)
	{
		errorScheme( 0 );
		return 0;
	}

	 sprintf(path,"%s:\\Pattern.Sch",cRamDrive);//22012001
	if ( (handle = open( path, O_RDONLY )) == -1 )
		{
			errorScheme( 1 );
			return 0;
		}
	getLine( handle, buf, '\n' );
	if ( strcmp( buf, "#SCHEME_START" ) != 0 )
		errorScheme( 2 );

	if (!getStringWithin( handle, NULL, "#SCHEME_END",
											"#NO_OF_FIXTURES", buf ))
	{
		errorScheme( 4 );
		exit( 0 );
	}
	nFixtures = atoi( buf );
//	Fixture = (FIXTURE far *)farmalloc( sizeof( FIXTURE ) * nFixtures );
	if ( !getFixtureDetails (handle, nFixtures) )
		errorScheme( 3 );

	getLine( handle, buf, '\n' );
	while( strcmp( buf, "#SCHEME_END" ) != 0 )
	{
		for( int i = 0; i < nFixtures; i++ )
		{
			sprintf( szTemp, "#FIXTURE_%d_PAIRS_START", i+1 );
			if ( strcmp( buf, szTemp ) == 0 )
				if ( !getPairDetails( handle, i ) )
					errorScheme( 5 );

			sprintf( szTemp, "#FIXTURE_%d_SPLITTER_START", i+1 );
			if ( strcmp( buf, szTemp ) == 0 )
				if (!getSpltDetails( handle, i ) )
					errorScheme( 6 );
		}
		getLine( handle, buf, '\n' );
	}

	//close( handleError );
    //MOD18012002:05:sdt:1615
    if(handleError){fclose(handleError);}

	return errorOccured;
}

int getPairDetails( int handle, int nFixture )
{
	int     nCntCtrlLn = 0, nCntPair = -1;
	boolean bComplete = FALSE;
	char    *p;
	char    buf[ MAX_LINE ];
	char    szTemp1[ MAX_LINE ], szTemp2[ MAX_LINE ];

	sprintf( szTemp1, "FIXTURE_%d_PAIRS_END", nFixture+1 );

	getLine( handle, buf, '\n' );
	while( strcmp( buf, szTemp1 ) != 0 )
	{
		if ( nCntPair >= Fixture[nFixture].m_nNoofPairs-1 )
			break;
		sprintf( szTemp2, "PAIR_%d", nCntPair+2 );
		if ( !checkString( buf, szTemp2, "=" ) )
			getStringWithin( handle, NULL, szTemp1, szTemp2, buf );
		if ( *buf != NULL )
			nCntPair++;
		else
			continue;

		nCntCtrlLn = 0;
		p = strtok( buf, "," );
		while( p != NULL )
		{
			if ( nCntCtrlLn < Fixture[ nFixture ].m_nPairCtrlLines )
				Fixture[ nFixture ].Pair[nCntPair].m_nLineNo[ nCntCtrlLn++ ] = atoi( p );
			else
			{
				Fixture[ nFixture ].Pair[nCntPair].m_nUnit = atoi( p );
				bComplete = TRUE;
			}
			p = strtok( NULL, "," );
		}
		if ( !bComplete )
			errorScheme( 7 );
		getLine( handle, buf, '\n' );
	}
	if ( nCntPair != Fixture[ nFixture ].m_nNoofPairs-1 )
		errorScheme( 5 );
	return 1;
}

int getSpltDetails( int handle, int nFixture )
{
	int     nCntCtrlLn = 0, nCntUnit = -1;
	char    *p;
	char    buf[ MAX_LINE ];
	char    szTemp1[ MAX_LINE ], szTemp2[ MAX_LINE ];

	sprintf( szTemp1, "FIXTURE_%d_SPLITTER_END", nFixture+1 );

	getLine( handle, buf, '\n' );
	while( strcmp( buf, szTemp1 ) != 0 )
	{
		if ( nCntUnit >= Fixture[nFixture].m_nNoofUnits-1 )
			break;
		sprintf( szTemp2, "UNIT_%d", nCntUnit+2 );
		if ( !checkString( buf, szTemp2, "=" ) )
			getStringWithin( handle, NULL, szTemp1, szTemp2, buf );
		if ( *buf != NULL )
			nCntUnit++;
		else
			continue;

		nCntCtrlLn = 0;
		p = strtok( buf, "," );
		while( p != NULL )
		{
			if ( nCntCtrlLn < Fixture[ nFixture ].m_nSpltCtrlLines )
				Fixture[ nFixture ].Splt[nCntUnit].m_nLineNo[ nCntCtrlLn++ ] = atoi( p );
			else
				break;
			p = strtok( NULL, "," );
		}
		getLine( handle, buf, '\n' );
	}
	if ( nCntUnit != Fixture[ nFixture ].m_nNoofUnits-1 )
		errorScheme( 6 );

	return 1;
}

void getLine( int handle, char* buf, char delimit )
{
	char ch;
	int charCount = 0;

	read( handle, &ch, 1 );
	while(ch != delimit )
	{
		buf[charCount++] = ch;
		read( handle, &ch, 1 );
	}
	buf[ charCount ] = '\0';
}

void freeFixture( void )
{
	for( int i = 0; i < nFixtures; i++ )
	{
		farfree( Fixture[i].Pair );
		farfree( Fixture[i].Splt );
	}
}

int getFixtureDetails( int handle, int nFixtures )
{
	char sztemp1[MAX_LINE], sztemp2[MAX_LINE];
	char buf[MAX_LINE];

	for( int i = 0; i < nFixtures; i++ )
	{
		sprintf( sztemp1, "#FIXTURE_%d_DETAILS_START", i+1 );
		sprintf( sztemp2, "#FIXTURE_%d_DETAILS_END", i+1 );
		if (!getStringWithin( handle, sztemp1, sztemp2, "NO_OF_PAIRS", buf ))
			return 0;
		Fixture[i].m_nNoofPairs = atoi( buf );
		Fixture[i].Pair=(PAIR far *)farmalloc(sizeof(PAIR)*Fixture[i].m_nNoofPairs);
		memset( Fixture[i].Pair, -1, sizeof(PAIR)*Fixture[i].m_nNoofPairs );

		if (!getStringWithin( handle, sztemp1, sztemp2, "NO_OF_CTRL_LINES", buf ))
			return 0;
		Fixture[i].m_nPairCtrlLines = atoi( buf );

		if (!getStringWithin( handle, sztemp1, sztemp2, "NO_OF_UNITS", buf ))
			return 0;
		Fixture[i].m_nNoofUnits = atoi( buf );
		Fixture[i].Splt = (SPLITTER far *)farmalloc( sizeof( SPLITTER )*
						Fixture[i].m_nNoofUnits );
		memset( Fixture[i].Splt, -1, sizeof( SPLITTER )*Fixture[i].m_nNoofUnits );
		if (!getStringWithin( handle, sztemp1, sztemp2, "NO_OF_SPLT_CTRL_LINES", buf ))
			return 0;
		Fixture[i].m_nSpltCtrlLines = atoi( buf );
	}
	return 1;
}

int getStringWithin( int handle, char *Start, char *End, char *key, char *buf )
{
	char buffer[ MAX_LINE ];
	long lfileMark;

//	lfileMark = tell( handle );
	lfileMark = lseek(handle, 0, SEEK_CUR);

	if ( Start != NULL )
	{
		while( 1 )
		{
			getLine( handle, buffer, '\n' );
			if ( strstr( buffer, Start ) )
				break;
			if ( eof_fd( handle ) )
			{
				*buf = NULL;
				lseek( handle, lfileMark, SEEK_SET );
				return FALSE;
			}
		}
	}
	while( 1 )
	{
		getLine( handle, buffer, '\n' );
		if ( checkString( buffer, key, "=" ) )
		{
			strcpy( buf, buffer );
			break;
		}
		if ( strstr( buffer, End ) || eof_fd( handle ) )
		{
			*buf = NULL;
			lseek( handle, lfileMark, SEEK_SET );
			return FALSE;
		}
	}
	lseek( handle, lfileMark, SEEK_SET );
	return TRUE;
}

boolean checkString( char* buffer, char* key, char* breakChar )
{
	char* cpMark;

	if ( strstr( buffer, key ) )
	{
		cpMark = strpbrk( buffer, breakChar );
		if ( cpMark )
			cpMark++;
		else
			errorScheme( 5 );
		while( *cpMark == ' ' )
			cpMark++;
		strcpy( buffer, cpMark );
		return TRUE;;
	}
	return FALSE;
}

void errorScheme( int errorNo )
{
	int temp = strlen( schemeErr[ errorNo ] );
    if ( errorNo != 0 )
    {
	//write( handleError, schemeErr[ errorNo ], temp );
    //MOD18012002:05:sdt:1620
    fwrite( schemeErr[ errorNo ],temp,1,handleError);
	errorOccured = TRUE;
  	}
    else
    {
    	#ifdef EXECUTE //Added by:sdt:2108200
        RemoveIniFiles();
        #endif //Added by:sdt:21082001
        printf("error: Unable to open Pattern.Log\n");
        exit(1);
    }
}
