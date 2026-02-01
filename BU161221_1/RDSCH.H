#if !defined(__RDSCH_H)
#define __RDSCH_H

#define MAX_CTRLLINES       4
#define MAX_LINE			80
#define TRUE				1
#define FALSE				0

typedef	unsigned char	boolean;

typedef struct tagPair
{
	unsigned char m_nUnit;
	int m_nLineNo[ MAX_CTRLLINES ];
}PAIR;

typedef struct tagSplitter
{
	int m_nLineNo[ MAX_CTRLLINES ];
}SPLITTER;

typedef struct tagFixture
{
	unsigned 	char m_nNoofPairs;
	unsigned 	char m_nNoofUnits;
	unsigned 	char m_nPairCtrlLines;
	unsigned 	char m_nSpltCtrlLines;
	PAIR 		far *Pair;
	SPLITTER 	far *Splt;
}FIXTURE;

boolean 	readScheme( void );
void 		errorScheme( int errorNo );
int 		getPairDetails( int handle, int nFixture );
int 		getSpltDetails( int handle, int nFixture );
int 		getFixtureDetails( int handle, int nFixtures );
void 		freeFixture( void );
void		getLine( int handle, char* buf, char delimit );
boolean 	checkString( char* buffer, char* key, char* breakChar );
int 		getStringWithin( int , char *, char *, char *, char * );

#endif