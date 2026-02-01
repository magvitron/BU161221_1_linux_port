#if !defined (__FRONTPG_h)
 #define __FRONTPG_H

#include "uses.h"
#include "deftype.h"
#include "common.h"
//#include "data.h"
//#define cmFrontPage 123 //Commented by:sdt:28092005:1120 //As it is declared in Common.h

#define cmDraw    0x1000
#define NOTTESTED 0x01
//#define PASS      0x40 //Commented by:sdt:28092005:1120 //As it is declared in Common.h
//#define FAIL      0x80 //Commented by:sdt:28092005:1120 //As it is declared in Common.h
//#define TIP       0//Commented by:sdt:27092005:2305: as it is declaredin ct.h
//#define RING      1//Commented by:sdt:27092005:2305: as it is declaredin ct.h

#define LEGEND_PASS   14
#define LEGEND_FAIL    5

#define LEGEND_TIP     1
#define LEGEND_RING   10

#define UPDATE_PASS    3
#define UPDATE_FAIL    4
#define UPDATE_DEFAULT 5
#define UPDATE_RESULT 0X10

//#define RESET_STATUS  0x00 //As it is declared in Common.h

//#define MAX_CONDUCTORS 24 //Commented as it is no longer used

class TTestProgress : public TView
 {
   private:
	  //CONDUCTOR_STATUS conductor_status[MAX_CONDUCTORS];
   public:
	  BYTE  updateType;
	  BYTE  status;
	  int  nFreqNum;
//     void  *infoPtr;
//      int   cond[MAX_CONDUCTORS];
   public:
	  TTestProgress (TRect &r);
	  ~TTestProgress();
	  void handleEvent( TEvent &ev);
	  virtual void draw();
	  //void showConductorNo(int,int);
	  void showProgress(int );
	  //void showLegend(TPoint size,char color);
	  void showCurrentReading(void);
 };

class TFrontPage : public TWindow
 {
  protected:

  public:
     TTestProgress *tp;
     TFrontPage(char *aTitle);
	 ~TFrontPage();

	 void updateFrontPage(BYTE status=RESET_STATUS,BYTE updateType=UPDATE_DEFAULT, int nFreqNum=0 );
     void hint(ushort hintNo);
     virtual void draw();
 };

#endif

