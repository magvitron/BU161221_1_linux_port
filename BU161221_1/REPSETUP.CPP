#include "uses.h"   // make this first (defines macros, pulls tv.h, etc.)

// Forward declarations needed because tobjstrm.h uses these before defining them
class ipstream;
class opstream;
class TStreamableTypes;

#include <stdio.h>
#include <tobjstrm.h>
#include <strstream.h>          // ostrstream()

//#include "uses.h"
#include "ct.h"
#include "share.h"
#include "repsetup.h"


//Global Variable //Start
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2140 for Quad
extern ReportSetup reportSetup;

//Global Variable //end


void getReportSetup()
{
	TCommandSet temp = TProgram::deskTop->curCommandSet;
	TProgram::deskTop->enableCommand( cmOK );

	TReportSetupDlg *prepdlg = new TReportSetupDlg( TRect( 19, 1, 61, 15 ), "Report Setup" );

	prepdlg->insert( new TLabel( TRect( 2, 1, 20, 2 ), "Parameters", prepdlg) );
	prepdlg->pParam  = new TCheckBoxes(TRect(2,2,35,6 ),new TSItem(paraminfo[0].szParamAbr,
											new TSItem(paraminfo[1].szParamAbr,
											new TSItem(paraminfo[2].szParamAbr,
											new TSItem(paraminfo[3].szParamAbr,
											new TSItem(paraminfo[4].szParamAbr,
											new TSItem(paraminfo[5].szParamAbr,
											new TSItem(paraminfo[6].szParamAbr,
											new TSItem(paraminfo[7].szParamAbr,
											new TSItem(paraminfo[8].szParamAbr,
											new TSItem(paraminfo[9].szParamAbr,
											new TSItem(paraminfo[10].szParamAbr,0))))))))))));
	prepdlg->insert(prepdlg->pParam);

	prepdlg->insert( new TLabel( TRect( 2, 6, 15, 7 ), "Type", prepdlg) );
	//prepdlg->pType  = new TRadioButtons(TRect(2,7,20,9 ),new TSItem("NORMALISE",
	//														 new TSItem("UNNORMALISE",0)));
	//Modified by:sdt:03022007:2230
	prepdlg->pType  = new TCheckBoxes(TRect(2,7,20,9 ),new TSItem("NORMALISE",
															 new TSItem("UNNORMALISE",0)));

	prepdlg->insert(prepdlg->pType);
	prepdlg->insert( new TLabel( TRect( 25, 8, 40, 9 ), "Remark", prepdlg) );
	prepdlg->pRemark  = new TCheckBoxes(TRect(25,9,40,10 ),new TSItem("REMARK",0));
	prepdlg->insert(prepdlg->pRemark);
	prepdlg->insert( new TLabel( TRect( 25, 6, 40, 7 ), "Report Type", prepdlg) );
	prepdlg->pRepType = new TCheckBoxes(TRect(25,7,40,8 ),new TSItem("DETAILED",0));
	prepdlg->insert(prepdlg->pRepType);

	prepdlg->insert(new TButton( TRect( 10, 11 , 18, 13 ), "~O~K", cmOK, bfDefault ));
	prepdlg->insert(new TButton( TRect( 25, 11 ,35 ,13  ), "~C~ancel", cmCancel, bfNormal ));

	//prepdlg->pParam->setData(reportSetup.m_Mode);
	//prepdlg->pType->setData(reportSetup.m_Type);
	//prepdlg->pRemark->setData(reportSetup.m_Remark);
	//prepdlg->pRepType->setData(reportSetup.m_SumDet);

	for ( int i = 0; i < MAXPARAMETERS ; i++ )
		if ( reportSetup.m_Mode & (0x01<<i) )
			prepdlg->pParam->press(i);

	for (int i = 0; i < 2; i++ )
		if( reportSetup.m_Type & (0x01<<i) )
			prepdlg->pType->press(i);

	if ( reportSetup.m_SumDet == True )
		prepdlg->pRepType->press(0);

	if ( reportSetup.m_Remark == True )
		prepdlg->pRemark->press(0);


	if(TProgram::deskTop->execView( prepdlg )==cmOK)
	{
		for ( int i = 0; i < MAXPARAMETERS; i++ )
		{
			if ( prepdlg->pParam->mark(i) == True )
				reportSetup.m_Mode |= 0x01<<i;
			else
				reportSetup.m_Mode &= ~((unsigned int)0x01<<i);
		}

		for ( int i = 0; i < 2; i++ )
		{
			if ( prepdlg->pType->mark(i) == True )
				reportSetup.m_Type |= 0x01<<i;
			else
				reportSetup.m_Type &= ~((unsigned int)0x01<<i);
		}
		if ( prepdlg->pRepType->mark(0) == True )
			reportSetup.m_SumDet = True;
		else
			reportSetup.m_SumDet = False;

		if ( prepdlg->pRemark->mark(0) == True )
			reportSetup.m_Remark = True;
		else
			reportSetup.m_Remark = False;
		//Added by:sdt:15122016:2145:start
		FILE *frpstp;
		if ((frpstp = fopen("rpsetup.bin", "wb")) != NULL) //Added by:sdt:15122016:2130
		{
			fwrite(&reportSetup, sizeof(ReportSetup),1,frpstp); //Added by:sdt:15122016:2130
		}
		if(frpstp){fclose(frpstp);}
		//Added by:sdt:15122016:2145:end
	}
	TDialog::destroy (prepdlg);

	if ( temp != TProgram::deskTop->curCommandSet )
		TProgram::deskTop->disableCommand( cmOK );

}


TReportSetupDlg::~TReportSetupDlg()
{
}

