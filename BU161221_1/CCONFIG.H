//
//	CConfig.H - Declaration file for class TConfig, structure Config
//
#if !defined( __CCONFIG_H )
#define __CCONFIG_H

#include <mem.h>

#include "deftype.h"
#include "speck.h"

#define MAX_SPECKNAMELEN		35
#define MAX_NAMELEN				20
#define MAX_USERENTRIES			7//4
#define MAX_USERENTRY_LEN		20//15
#define MAX_USERTITLE_LEN		20//15

#define cmConfigOK                 301 //101 //Modified by:sdt:28092016
#define cmNoOfPairs				302 //102 //Modified by:sdt:28092016
#define cmPairsPerUnit			303 //103 //Modified by:sdt:28092016
#define cmOKPair				304 //104 //Modified by:sdt:28092016
#define cmOKUnit				305 //105 //Modified by:sdt:28092016



struct Config
{
	//char            szFileName[MAXFILENAME];
	ushort			speckNumber;
	char			speckName[31];
	char    		cabletype[MAXLENCABLETYPE+1];
	SEQNAMES 		szCableSequence;
	char 			m_TestDate[13];
	char 			m_PrintDate[13];
	char			m_StartTime[13];
	char			m_StopTime[13];
	unsigned 		int m_Noofpairs;
	unsigned		int m_Unitof;
	unsigned 		int	m_Ilm;
	unsigned 		int	m_Flm;
	unsigned 		int m_Length;
	float       	m_Temp;
	unsigned 		int	m_StartFrom;
	char 			m_UserEntries[ MAX_USERENTRIES ][ MAX_USERENTRY_LEN ];
	char 			m_UserTitles[ MAX_USERENTRIES ][ MAX_USERTITLE_LEN ];
	//char           szLFreq[10];
	//char           szHFreq[10];
	//int            nLFrequency;
	//int            nHFrequency;

};

class TConfig : public TDialog
{
public:
	//TConfig();
	//TConfig( TRect& r, char *atitle ) : TDialog( r, atitle ) ,
	TConfig(const TRect& r, char *atitle) : TDialog(r, atitle),

						TWindowInit( TConfig::initFrame )
	{
	}
	~TConfig();
	virtual void setData( Config& config );
	virtual void getData( Config& config );
	void handleEvent( TEvent& ev );
	void selectNoOfPairs(int* );
	void selectPairsPerUnit(int* );

public:

	TSpeckCollection *pc;		// specification collection
	TSpeckListBox	*plb;			// drop list for speck name
	TInputLine		*pcbltp;		// Cablr type
	TInputLine		*pseq;			//Cable Sequence Name
	TInputLine		*ppair;			//drop list for total number of pairs
	TInputLine		*punit;			//drop list for pairs in an unit
	TInputLine		*pilm;			// initial Length Marking
	TInputLine		*pflm;			// final Length Marking
	TInputLine		*pt;			// temperature
	TInputLine		*pl;			// length in mts
	TInputLine		*ps;			// starting fixture( cond )
	TInputLine		*pUe[ MAX_USERENTRIES ];	// for user entries
	char 			szOldSpeckName[31];
	char			szOldPairs[7];
	char			szOldUnits[7];


};

class TPairDialog : public TDialog
{
	protected :
		void handleEvent(TEvent &ev);

	public :

		TSpeckCollection 	*pcpair;
		TSpeckListBox		*plbpair;
		TConfig				*pconfig;
	public :
		//TPairDialog( TRect& r, char *atitle ) : TDialog( r, atitle ) ,
		TPairDialog(const TRect& r, char *atitle) : TDialog(r, atitle),
						TWindowInit( TPairDialog::initFrame )
		{
		}
		~TPairDialog();


};

class TUnitDialog : public TDialog
{
	protected :
		void handleEvent(TEvent &ev);

	public :

		TSpeckCollection 	*pcunit;
		TSpeckListBox		*plbunit;
		TConfig				*pconfig;
	public :
		//TUnitDialog( TRect& r, char *atitle ) : TDialog( r, atitle ) ,
		TUnitDialog(const TRect& r, char *atitle) : TDialog(r, atitle),
						TWindowInit( TUnitDialog::initFrame )
		{
		}
		~TUnitDialog();


};
#endif
