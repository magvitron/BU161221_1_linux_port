#if !defined( __REPSETUP_H )
#define __REPSETUP_H
#include "setup.h"

//#define cmEditRunParameter 	3001
//#define cmOKRunParameter    3002
//#define cmEditRunFrequency	3003
//#define cmOKRunFrequency	3004
//#define cmEditRunCombination	3005
//#define cmOKRunCombination	3006






class TReportSetupDlg : public TDialog
{
protected:
  //void handleEvent( TEvent& ev );

public:
	//RunSetup *runSetup;


	TCheckBoxes *pParam;					// Parameter Selection
	//TRadioButtons *pType;					//Normalised / Unnormalised selection
	//Modified by:sdt:03022007:2230
	TCheckBoxes *pType;					//Normalised / Unnormalised selection
	TCheckBoxes *pRepType;					// Summery / Detailed
	TCheckBoxes *pRemark;					//Yes/No

public :
	//TReportSetupDlg( TRect& r, char *atitle ) : TDialog( r, atitle ) ,
	  TReportSetupDlg( const TRect& r, const char *atitle ) : TDialog( r, atitle ),
						TWindowInit( TReportSetupDlg::initFrame )
	{
	}
	~TReportSetupDlg();
};


#endif
