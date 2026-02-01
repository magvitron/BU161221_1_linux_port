#if !defined( __RSETUP_H )
#define __RSETUP_H
#include "setup.h"

#define cmEditRunParameter 	3001
#define cmOKRunParameter    3002
#define cmEditRunFrequency	3003
#define cmOKRunFrequency	3004
#define cmEditRunCombination	3005
#define cmOKRunCombination	3006


struct run_param
{
 ushort select;
};//run_parameter;


class TDerivedCheckBoxes : public TCheckBoxes//TCluster
{
	protected :
		//void handleEvent(TEvent &ev);
	public :
		//void draw();
		int *iSel;
		//TDerivedCluster(const TRect& bounds, TSItem *aStrings);
		TDerivedCheckBoxes(const TRect& bounds, TSItem *aStrings): TCheckBoxes(bounds,aStrings)
					   {
							iSel = &sel;
					   }


};

class TRunSetupDlg : public TDialog
{
protected:
    void handleEvent(TEvent& ev);

public:
    run_param *runParam;
    run_param *runLFParam;
    run_param *runHFParam;

    TDerivedCheckBoxes *pLFParam;
    TDerivedCheckBoxes *pHFParam;
    TRadioButtons *pFixtures;
    TDerivedCheckBoxes *pFanout;
    TCheckBoxes *pSingleStep;
    TCheckBoxes *pZchMeas;
    TInputLine *pBypassPairs;
    TCheckBoxes *pAlarmsOff;

public:
    TRunSetupDlg(const TRect& r, const char *atitle)
        : TWindowInit(&TDialog::initFrame)          // <-- construct *virtual* base here
        , TDialog(r, TStringView(atitle))
    {
        runParam   = new run_param;
        runLFParam = new run_param;
        runHFParam = new run_param;
    }

    ~TRunSetupDlg();

    void selectRunFrequency();
    void getParameterBits();
    void setParameterBits();
};



class TRunSetupFrequency : public TDialog
{
protected:
    void handleEvent(TEvent &ev);

public:
    run_param *runFreq;
    TDerivedCheckBoxes *pLFFrequency;
    TDerivedCheckBoxes *pHFFrequency;

public:
    TRunSetupDlg *prsdlg;

    TRunSetupFrequency(const TRect& r, const char *atitle)
        : TWindowInit(&TDialog::initFrame)
        , TDialog(r, TStringView(atitle))
    {
        runFreq = new run_param;
    }

    ~TRunSetupFrequency();

    void getFrequencyBits();
    void setFrequencyBits();
};




// class TRunSetupDlg : public TDialog
// {
// protected:
//   void handleEvent( TEvent& ev );
// public:
// //	int iSelRunParam;
// //	int iSelRunFreq;
// public:
// 	//RunSetup *runSetup;
// 	run_param *runParam;
// 	run_param *runLFParam;// = new run_param;
// 	run_param *runHFParam;
// 	TDerivedCheckBoxes *pLFParam;					// Parameter Selection
// 	TDerivedCheckBoxes *pHFParam;					// Parameter Selection
// 	TRadioButtons *pFixtures;					// Fixture Selection
// 	TDerivedCheckBoxes *pFanout;				// Break / Short Selection
// 	TCheckBoxes *pSingleStep;					// Single Step Selection
// 	TCheckBoxes *pZchMeas;						// ZCH Measure Selection
// 	TInputLine *pBypassPairs;					// Bypass Pairs Selection
// 	TCheckBoxes *pAlarmsOff;					// No alarms will be shown //Added by:sdt:28042015:2100

// public :
// 	//TRunSetupDlg( TRect& r, char *atitle ) : TDialog( r, atitle ) ,
// 	//TRunSetupDlg(const TRect& r, const char *atitle)
// 	//	:  TWindowInit(&TDialog::initFrame)
// 	TRunSetupDlg(const TRect& r, const char *atitle)
// 	    : TDialog(r, TStringView(atitle))
// 	{
// 		runParam = new run_param;
// 		runLFParam = new run_param;
// 		runHFParam = new run_param;
// 	}

// 	~TRunSetupDlg();

// 	void selectRunFrequency();
// 	//void selectRunCombination();//Commented by:sdt:15112005:1545
// 	void getParameterBits();
// 	void setParameterBits();
// 	//void getFrequencyBits();
// 	//void setFrequencyBits();
// 	//void getCombinationBits();
// 	//void setCombinationBits();

// };

// class  TRunSetupFrequency : public TDialog
// {
// protected:
//    void handleEvent(TEvent &ev);

// public:

//    run_param *runFreq;// = new run_param;
//    TDerivedCheckBoxes *pLFFrequency;					// LF Frequency Selection
//    TDerivedCheckBoxes *pHFFrequency;					// LF Frequency Selection

// public:

// 	TRunSetupDlg   *prsdlg;
// 	//TRunSetupFrequency(const TRect& r, const char *atitle)
// 	//	: TWindowInit(&TDialog::initFrame)
// 	TRunSetupFrequency(const TRect& r, const char *atitle)
// 	        : TDialog(r, TStringView(atitle))

// 	{
// 		runFreq = new run_param;
// 	}

// 	~TRunSetupFrequency();

// 	void getFrequencyBits();
// 	void setFrequencyBits();



// };

//Commented by:sdt:15112005:1545
/*class  TRunSetupCmbn : public TDialog
{
protected:
   void handleEvent(TEvent &ev);

public:
   run_param *runCmbn;// = new run_param;
   TDerivedCheckBoxes *pPairCombinations;					// Pair to Paor Combination Selection
   TDerivedCheckBoxes *pUnitCombinations;					// Combination Selection

public:

	TRunSetupDlg   *prsdlg;
	TRunSetupCmbn(TRect& r,char *atitle): TDialog(r,atitle),
							TWindowInit(TRunSetupCmbn::initFrame)
	{
		runCmbn = new run_param;
	}
	~TRunSetupCmbn();

	void getCombinationBits();
	void setCombinationBits();

};*/

#endif
