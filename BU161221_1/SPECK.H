//
// Speck.H - Declaration for class TSpeck*
//			for management of specification
//
#if !defined( __SPECK_H )
#define __SPECK_H

#define MAXL_SPECKSNUMBER	25 /* Shrinivas Atre 16 th June 1996 */
#define MAXL_CABLETYPE		15
#define MAXL_PARAMETERNAME	15
#define MAXL_GUAGE			10
#define MAX_LIMITS			2 //Added:san:03032000 .


#include "deftype.h"
#include "uses.h"
#include "common.h"
#include "password.h"
#include "ct.h"
//#include "config.h"

#define cmCreateNew			2001
#define cmEdit				2002
#define cmRemove			2003
#define cmExitSpeck			2004
#define cmPrint				2005

#define cmSave				2011
#define cmSaveas			2012
#define cmEditParameter     2013
#define cmSelectParameter   2014
#define cmSaveParameter     2015
#define cmOKEdit            2016
#define cmSelectSequence	   2017
#define cmOKSelectSequence  2018
#define cmEditFrequency	   2019
#define cmOKFrequency		2020
#define cmEditCombination   2021
#define cmOKCombination		2022

#define MAX_GUAGES			6
#define MAX_FILELEN			255
#define MAX_SPECKNAMELEN	35
#define MAX_CABLETYPELEN    20

#define CONST_SAVE			0
#define CONST_SAVEAS		1

__link(RFCollection);

struct SpeckObject
{
	ushort fileNumber;
	char *speckName;
	~SpeckObject();
};

#define SPECK_FILENAME		"SPECKS.INI"

class TSpeckCollection : public TCollection
{
public:

	TSpeckCollection( ccIndex aLimit, ccIndex aDelta ) :
			TCollection( aLimit, aDelta ) {}
	static const char * const name;

	SpeckObject *at(ccIndex index) {
	  return (SpeckObject*)TCollection::at(index);
    }

	void atInsert(ccIndex index, SpeckObject *item) {
      TCollection::atInsert(index, item);
    }

	void atPut(ccIndex index, SpeckObject *item) {
	  TCollection::atPut(index, item);
    }

    void atRemove(ccIndex index) {
      TCollection::atRemove(index);
    }

    //static void error(ccIndex code, ccIndex info) {
     // TCollection::error(code, info);
    //}

	SpeckObject *firstThat(ccTestFunc Test, void *arg) {
	  return (SpeckObject*)TCollection::firstThat(Test, arg);
	}

	void forEach(ccAppFunc action, void *arg) {
	  TCollection::forEach(action, arg);
	}

	void free(SpeckObject *item) {
	  TCollection::free(item);
	}

	void freeAll() {
	  TCollection::freeAll();
	}

	virtual ccIndex indexOf(SpeckObject *item) {
	  return TCollection::indexOf(item);
	}

	void insert(SpeckObject *item) {
	  TCollection::insert(item);
	}

	SpeckObject *lastThat(ccTestFunc Test, void *arg) {
	  return (SpeckObject*)TCollection::lastThat(Test, arg);
	}

	void pack() {
	  TCollection::pack();
	}

	void remove(SpeckObject *item) {
	  TCollection::remove(item);
	}

	void removeAll() {
	  TCollection::removeAll();
	}

	virtual void setLimit(ccIndex aLimit) {
	  TCollection::setLimit(aLimit);
	}

	static TStreamable *build();
	int getCount() { return count; }

private:

	virtual void freeItem(void *item);

	virtual const char *streamableName() const
		{ return name; }

	virtual void *readItem( ipstream& );
	virtual void writeItem( void *, opstream& );

protected:

	TSpeckCollection( StreamableInit ) : TCollection( streamableInit ) {}
};

class TSpeckListBox : public TListBox
{

public:
	TSpeckListBox( const TRect&, ushort, TScrollBar *);

	virtual void getText( char *, short, short );
};



class TSpeckBuild : public TDialog
{
private:
//  ParameterChar *ParaCharacter[10];
//    STRUCT_SPECKS    	speckData;
protected:
	void 		handleEvent( TEvent& ev );

protected:
	BYTE		m_nUserType;

public:

	TSpeckBuild( BYTE userType = U_OPERATOR );
	TSpeckBuild();
	~TSpeckBuild();

	void 	createSpeck( Boolean );

	void    selectParameter();
	void    editParameter();
	void    selectCableSequence(); //Added by:sdt:28072005:0055
	void	   selectFrequency();  //Added by:sdt:31072005:1220
	//void	   selectCombination(); //Commented by:sdt:15112005:1650

	BYTE		getUserType() { return m_nUserType; }
	Boolean	saveSpeck( TDialog *d, ushort type );
	Boolean	getData( TDialog *d );
	void		getAccessState( ushort& first, ushort& second );
	void 	setAccessState( ushort& first, ushort& second );
	//int		findSpeckFiles(void); //added by:sdt:27072005:1905

public:
	TSpeckCollection	*pc;
	TSpeckListBox		*pl;
	TView				*vc;
	TView				*vr;

};

class TSpeckDataDlg : public TDialog
{
protected:
  void handleEvent( TEvent& ev );
public:
	Boolean newSpeck;
	TSpeckBuild *ps;
	TInputLine *pn;					// speck name
	TInputLine *pct;				// cable type
	TInputLine *pseq;				//Cable sequence//Added by:sdt:28072005:1050
	TInputLine *pntemp;				// Normalised Temperature
	//TInputLine *pnlen;				// Normalised Length //Commented by:sdt:31012007:2115


public :
	TSpeckDataDlg(const TRect& r, char *atitle ) : TDialog( r, atitle ) ,
						TWindowInit( TSpeckDataDlg::initFrame )
	{
	}
};

struct parameterselect
 {
    ushort select;
 };



class  TSelectParameter : public TDialog
{
protected:
   void handleEvent(TEvent &ev);

public:

   TLabel        *pLabel;
   TRadioButtons *pParameter;

public:

    TSpeckDataDlg   *psdlg;
	TSelectParameter(const TRect& r,char *atitle): TDialog(r,atitle),
							TWindowInit(TSelectParameter::initFrame)
							{
					   }
	~TSelectParameter();

};

class TEditParameter : public TDialog
{
protected:
	void handleEvent( TEvent& ev );

public:
	Boolean newSpeck;
	TSelectParameter *pselect;

	TInputLine *pLowerLimit;			// Absolute Lower Limit
	TInputLine *pUpperLimit;			// Absolute Upper Limit
	TInputLine *pAverageLow;			// Absolute Average Limit
	TInputLine *pAverageHigh;		// Absolute Average Limit
	TInputLine *pStdLow;			// Standard
	TInputLine *pStdHigh;			// Standard
	TInputLine *pRMSLow;			// RMS
	TInputLine *pRMSHigh;			// RMS
	TInputLine *pPSumLow;			// Power Sum
	TInputLine *pPSumHigh;			// Power Sum
	TInputLine *pWpnLow;			// Worst Pair Next
	TInputLine *pWpnHigh;			// Worst Pair Next
	TInputLine *pAnyOther;			// AnyOther
	TInputLine *pnlen;				// Normalised Length //Added by:sdt:31012007:2115


public :
	TEditParameter(const TRect& r, char *atitle ) : TDialog( r, atitle ) ,
						TWindowInit( TEditParameter::initFrame )
	{
	}
};

//Added by:sdt:28072005:2330
class  TSelectSequence : public TDialog
{
protected:
   void handleEvent(TEvent &ev);

public:

   TSpeckListBox		*pcblseq;
   TSpeckCollection		*pseqcol;

public:

	TSpeckDataDlg   *psdlg;
	TSelectSequence(const TRect& r,char *atitle): TDialog(r,atitle),
						TWindowInit(TSelectSequence::initFrame)
							{
							}
	~TSelectSequence();

};



//Added by:sdt:31072005:1230
class  TSelectFrequency : public TDialog
{
protected:
   void handleEvent(TEvent &ev);

public:

   TLabel        *pLabel;
   TRadioButtons   *pLFFrequency;
   TRadioButtons   *pHFFrequency;

public:

	TSpeckDataDlg   *psdlg;
	TSelectFrequency(const TRect& r,char *atitle): TDialog(r,atitle),
						TWindowInit(TSelectFrequency::initFrame)
							{
					   }

};


//Commented by:sdt:15112005:1550
//Added by:sdt:31072005:1530
/*class  TSelectCombination : public TDialog
{
protected:
   void handleEvent(TEvent &ev);

public:

   TLabel        *pLabel;
   //TCheckBoxes   *pPairCombinations;
   TRadioButtons *pPairCombinations;
   TRadioButtons   *pUnitCombinations;

public:

	TSpeckDataDlg   *psdlg;
	TSelectCombination(TRect& r,char *atitle): TDialog(r,atitle),
						TWindowInit(TSelectCombination::initFrame)
					   {
					   }

};*/


inline ipstream& operator >> ( ipstream& is, TSpeckCollection& cl )
	{ return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TSpeckCollection*& cl )
	{ return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TSpeckCollection& cl )
	{ return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TSpeckCollection* cl )
	{ return os << (TStreamable *)cl; }

#endif
