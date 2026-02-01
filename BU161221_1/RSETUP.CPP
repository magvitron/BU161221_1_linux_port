
#include <stdio.h>
#include "tv_compat.h"
//#include <tobjstrm.h>
#include <strstream.h>          // ostrstream()
#include <io.h>
#include <fcntl.h>
#include <conio.h>

#include "ct.h"
#include "share.h"
#include "rsetup.h"


//Global Variable //Start
extern PARAM_DETAILS Param;
extern PRM *prmarray;
extern PARAMINFO	paraminfo[ MAXPARAMETERS ];//+2 removed by:sdt:30092005:2140 for Quad
extern int iSelRunParam;
extern int iSelRunFreq;
extern RunSetup runSetup, TempRunSetup;
extern FreqStruct LFFrequency;
extern int LfIndex;    		//Added by:sdt:09092005:2300
extern int HfIndex;			//Added by:sdt:09092005:2300
extern char SpeckList[MAXFREQUENCIES][10]; //Added by:sdt:09092005:2300
extern int ParamCtsHandle;
//Global Variable //end


void getRunSetup()
{
	TRunSetupDlg *prsdlg = new TRunSetupDlg( TRect( 11, 1, 69, 17 ), "Run Setup" );
	//prsdlg->runSetup = &runSetup;//Added by:sdt:24082005:2300
	//prsdlg->insert( new TLabel( TRect( 2, 1, 20, 2 ), "Parameters", prsdlg) );
	prsdlg->insert( new TLabel( TRect( 2, 1, 20, 2 ), "LF Parameters", prsdlg) );
	prsdlg->pLFParam  = new TDerivedCheckBoxes(TRect(2,2,35,5 ),new TSItem(paraminfo[0].szParamAbr,
											new TSItem(paraminfo[1].szParamAbr,
											new TSItem(paraminfo[2].szParamAbr,
											new TSItem(paraminfo[3].szParamAbr,
											new TSItem(paraminfo[4].szParamAbr,
											new TSItem(paraminfo[5].szParamAbr,
											new TSItem(paraminfo[6].szParamAbr,
											new TSItem(paraminfo[7].szParamAbr,0)))))))));
	prsdlg->insert(prsdlg->pLFParam);
	prsdlg->insert( new TLabel( TRect( 2, 5, 20, 6 ), "HF Parameters", prsdlg) );
	prsdlg->pHFParam  = new TDerivedCheckBoxes(TRect(2,6,35,7 ),new TSItem(paraminfo[8].szParamAbr,
											new TSItem(paraminfo[9].szParamAbr,
											new TSItem(paraminfo[10].szParamAbr,0))));
											//Modified by:sdt:30092005:2005 for quad software.
											//new TSItem(paraminfo[11].szParamAbr,
											//new TSItem(paraminfo[12].szParamAbr,
											//new TSItem(paraminfo[13].szParamAbr,
											//new TSItem(paraminfo[14].szParamAbr,0))))))))))))))));

	prsdlg->insert(prsdlg->pHFParam);
	prsdlg->insert( new TLabel( TRect( 37, 1, 55, 2 ), "Fixtures", prsdlg) );
	prsdlg->pFixtures  = new TRadioButtons(TRect(37,2,55,5 ),new TSItem("INNER",
															 new TSItem("OUTER",
															 new TSItem("BOTH",0))));
	prsdlg->insert(prsdlg->pFixtures);
	//Uncommented by:sdt:09112006 //Fanout test required for pair software.
	prsdlg->insert( new TLabel( TRect( 37, 5, 55, 6 ), "Fanout", prsdlg) );
	prsdlg->pFanout = new TDerivedCheckBoxes(TRect(37,6,55,8 ),new TSItem("SHORT",
																new TSItem("BREAK",0)));
	prsdlg->insert(prsdlg->pFanout);
	prsdlg->pSingleStep = new TCheckBoxes(TRect(2,9,35,10 ),new TSItem("Single Step",0));
	prsdlg->insert(prsdlg->pSingleStep);

	//Added by:sdt:28042015:2100
	prsdlg->pAlarmsOff = new TCheckBoxes(TRect(37,9,55,10 ),new TSItem("Alarms OFF",0));
	prsdlg->insert(prsdlg->pAlarmsOff);
	//Commented by:sdt:30092005:2330 //As it is not required for Quad Software.
	//prsdlg->pZchMeas = new TCheckBoxes(TRect(37,9,55,10 ),new TSItem("ZCH Measured",0));// for \nATN/XTs",0));
	//prsdlg->insert(prsdlg->pZchMeas);

	prsdlg->pBypassPairs = new TInputLine( TRect( 16, 11, 55, 12), 45);
	prsdlg->insert(prsdlg->pBypassPairs);
	prsdlg->insert( new TLabel( TRect( 2, 11, 15, 12 ), "Bypass Pairs", prsdlg->pBypassPairs) );

	prsdlg->insert(new TButton( TRect( 5, 13 ,25, 15), "~E~dit Parameteres", cmEditRunParameter, bfNormal ));
	prsdlg->insert(new TButton( TRect( 27, 13 ,35 ,15  ), "~O~K", cmOKRunParameter, bfDefault ));
	prsdlg->insert(new TButton( TRect( 37, 13 ,49 ,15  ), "~C~ancel", cmCancel, bfNormal ));

	prsdlg->getParameterBits();
	prsdlg->pLFParam->setData(prsdlg->runLFParam);
	prsdlg->pHFParam->setData(prsdlg->runHFParam);
	prsdlg->pLFParam->select();
	//Added by:sdt:24082005:2355//Start
	prsdlg->runParam->select = runSetup.wFixture;
	prsdlg->pFixtures->setData(prsdlg->runParam);
	prsdlg->runParam->select = runSetup.wShortBreak;//Uncommented by:sdt:09112006:2135//Commented by:sdt:30092005:2330
	prsdlg->pFanout->setData(prsdlg->runParam);//Uncommented by:sdt:09112006:2135//Commented by:sdt:30092005:2330
	prsdlg->runParam->select = runSetup.bSingleStep;
	prsdlg->pSingleStep->setData(prsdlg->runParam);
	//Added by:sdt:28042015:2100
	prsdlg->runParam->select = runSetup.bAlarmsOff;
	prsdlg->pAlarmsOff->setData(prsdlg->runParam);

	//prsdlg->runParam->select = runSetup.bZchMeas;//Commented by:sdt:30092005:2330
	//prsdlg->pZchMeas->setData(prsdlg->runParam); //Commented by:sdt:30092005:2330
	strcpy(prsdlg->pBypassPairs->data,runSetup.szBypassPairs);
	//Added by:sdt:24082005:2355//end
	//&runSetup = prsdlg->runSetup;//Added by:sdt:24082005:2300
	TProgram::deskTop->execView( prsdlg );
	TDialog::destroy (prsdlg);

}


void TRunSetupDlg::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmEditRunParameter)
		{
			if(this->pLFParam->getState(sfFocused))//Added by:sdt:25082005:0055
			{
				iSelRunParam = *(this->pLFParam->iSel);
				this->pLFParam->getData(runLFParam);
				//Added by:sdt:20102005:0050
				if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
				{
					clrscr();
					printf( "\nError Opening File PARAM.CTS File for Reading..." );
					RemoveIniFiles();
					exit( 1 );
				}

				ReadParamDotCts( paraminfo[iSelRunParam].szParamAbr , prmarray);
				if(!(iSelRunParam==SRNO_RU||iSelRunParam==SRNO_LR||iSelRunParam==SRNO_CR))
				{
					this->selectRunFrequency();
				}
				//else //Commented by:sdt:25082005:0055
				//{
				//	this->selectRunFrequency();
				//}
			}
			if(this->pHFParam->getState(sfFocused))//Added by:sdt:25082005:0055
			{
				iSelRunParam = (*(this->pHFParam->iSel))+Param.Hf_Param_Start;
				this->pLFParam->getData(runHFParam);
				//Added by:sdt:20102005:0050
				if ( (ParamCtsHandle = open( "PARAM.CTS", O_RDONLY | O_BINARY )) == -1)
				{
					clrscr();
					printf( "\nError Opening File PARAM.CTS File for Reading..." );
					RemoveIniFiles();
					exit( 1 );
				}

				ReadParamDotCts( paraminfo[iSelRunParam].szParamAbr , prmarray);
				this->selectRunFrequency();
				//else //Commented by:sdt:25082005:0055
				//{
				//	this->selectRunFrequency();
				//}
			}
		}

		if(ev.message.command == cmOKRunParameter)
		{
			this->pLFParam->getData(runLFParam);
			//Condition added by:sdt:01102005:0010
			//If "both" fixtures are not selected then only
			//LF parameters can be selected.
			if(this->pHFParam->getState(sfDisabled))
			{
				runHFParam->select = 0;
			}
			else
				this->pHFParam->getData(runHFParam);
			setParameterBits();
			//Added by:sdt:24082005:2355//Start
			this->pFixtures->getData(runParam);
			runSetup.wFixture = runParam->select;
			this->pFanout->getData(runParam);//Uncommented by:sdt:09112006:2135//Commented by:sdt:30092005:2330
			runSetup.wShortBreak = runParam->select;//Uncommented by:sdt:09112006:2135//Commented by:sdt:30092005:2330
			this->pSingleStep->getData(runParam);
			runSetup.bSingleStep = (Boolean)runParam->select;
			//Added by:sdt:28042015:2100
			this->pAlarmsOff->getData(runParam);
			runSetup.bAlarmsOff = (Boolean)runParam->select;

			//this->pZchMeas->getData(runParam);//Commented by:sdt:30092005:2330
			//runSetup.bZchMeas = (Boolean)runParam->select;//Commented by:sdt:30092005:2330
			strcpy(runSetup.szBypassPairs, this->pBypassPairs->data);
			//Added by:sdt:24082005:2355//end
			//Added by:sdt:15122016:2140:start
			FILE *frnstp;
			if ((frnstp = fopen("rnsetup.bin", "wb")) != NULL)
			{
				fwrite(&runSetup, sizeof(RunSetup),1,frnstp);
			}
			if(frnstp){fclose(frnstp);}
			//Added by:sdt:15122016:2140:end
			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );

		}

	}
	//Condition added by:sdt:09112006:2250
	//This will deactivate Break & Short check boxes if Bns.exe file
	//is not present in current directory.
	if ( ev.what == evBroadcast )
	{
		struct ffblk ffblk;
		if(findfirst("bns.exe",&ffblk,0)!=0)
		{
			this->pFanout->setState( sfDisabled, True );
			runSetup.wShortBreak = 0x0;//NONEBREAKSHORT;
		}
	}
	//Condition commented by:sdt:03112006:2345
	//Even though both fixtures are not selected, user can select LF
	//frequencies of HF parameters as these are calculated from
	//LF parameters readings.
	//Added by:sdt:30092005:2355
	//If only both fixtures are selected then only HF Parameters
	//can be selected otherwise it should be disabled.
	/*if ( ev.what == evBroadcast )
	{

		if ( ev.message.command == cmReleasedFocus ||
				ev.message.command == cmReceivedFocus )
		{
			this->pFixtures->getData(runParam);
			if(runParam->select==2)
			{
				this->pHFParam->setState( sfDisabled, False );
			}
			else
				this->pHFParam->setState( sfDisabled, True );

		}
	}*/
	//TDialog::handleEvent( ev );
	//clearEvent( ev );

}
TRunSetupDlg::~TRunSetupDlg()
{
	delete runLFParam;
	delete runHFParam;
	delete runParam;
	//delete runSetup;
}

void TRunSetupDlg:: getParameterBits()
{
	unsigned long ulParamCheckMask = 0x1l;//, ulParameter;
	runLFParam->select=0;
	//for(int i=0; i< MAXPARAMETERS; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	for(int i=0; i<= Param.Lf_Param_End; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	{
		if((runSetup.ulParamRunSetup[i] & 0x40000000l)!=0)//Added by:sdt:24082005:2300
		{
			runLFParam->select |= ulParamCheckMask;
		}
	}
	ulParamCheckMask = 0x1l;
	runHFParam->select=0;
	//for(int i=0; i< MAXPARAMETERS; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	for(int i=Param.Hf_Param_Start; i<MAXPARAMETERS; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	{
		if((runSetup.ulParamRunSetup[i] & 0x40000000l)!=0)//Added by:sdt:24082005:2300
		{
			runHFParam->select |= ulParamCheckMask;
		}
	}
}

void TRunSetupDlg:: setParameterBits()
{
	unsigned long ulParamCheckMask = 0x1l, ulParameter;

	//for(int i=0; i< MAXPARAMETERS; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	for(int i=0; i<= Param.Lf_Param_End; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	{
		ulParameter = ulParamCheckMask & runLFParam->select;

		if(ulParameter!=0)
		{
			//Condition added by:sdt:06022007
			//This will stop the user from activating parameters
			//which are not activated in Runsetup.ini.
			if(TempRunSetup.ulParamRunSetup[i]&0x40000000l)
			{
				runSetup.ulParamRunSetup[i] |= 0x40000000l;//Added by:sdt:24082005:2300
			}
			else
			{
				char strTemp[80];
				sprintf(strTemp,"%s parameter is currently not implemented",paraminfo[i].szParamAbr);
				messageBox( strTemp, mfError|mfOKButton );
			}
		}
		else
		{
			runSetup.ulParamRunSetup[i] &= 0x3FFFFFFFl;//Added by:sdt:24082005:2300
		}
	}
	ulParamCheckMask = 0x1l;
	for(int i=Param.Hf_Param_Start; i< MAXPARAMETERS; i++,ulParamCheckMask<<=1)//-1 removed by:sdt:30092005:2135 for Quad
	{
		ulParameter = ulParamCheckMask & runHFParam->select;
		if(ulParameter!=0)
		{
			//Condition added by:sdt:06022007
			//This will stop the user from activating parameters
			//which are not activated in Runsetup.ini.
			if(TempRunSetup.ulParamRunSetup[i]&0x40000000l)
			{
				runSetup.ulParamRunSetup[i] |= 0x40000000l;//Added by:sdt:24082005:2300
			}
			else
			{
				char strTemp[80];
				sprintf(strTemp,"%s parameter is currently not implemented",paraminfo[i].szParamAbr);
				messageBox( strTemp, mfError|mfOKButton );
			}

		}
		else
		{
			runSetup.ulParamRunSetup[i] &= 0x3FFFFFFFl;//Added by:sdt:24082005:2300
		}
	}

}

void TRunSetupDlg::selectRunFrequency()
{
	TRunSetupFrequency *pSF = new TRunSetupFrequency( TRect( 21, 7,59,19),"Select Frequency");

	pSF->insert( new TLabel( TRect( 2, 1, 17, 2 ), "LOW FREQ.", pSF) );

	//pSF->pLFFrequency  = new TDerivedCheckBoxes(TRect(2,2,17,6 ),new TSItem("0.08 KHZ",
	//										new TSItem("0.8 KHZ",
	//										new TSItem("1 KHZ",
	//										new TSItem("1.6 KHZ",0)))));

	//Modified by:sdt:01102005:0930
	//For Quad : LF Frequencied increased to 6 & HF reduced to 2.
	pSF->pLFFrequency  = new TDerivedCheckBoxes(TRect(2,2,33,5 ),new TSItem(SpeckList[0],
											new TSItem(SpeckList[1],
											new TSItem(SpeckList[2],
											new TSItem(SpeckList[3],
											new TSItem(SpeckList[4],
											new TSItem(SpeckList[5],0)))))));
	pSF->insert(pSF->pLFFrequency);
	pSF->insert( new TLabel( TRect( 2, 6, 17, 7 ), "HIGH FREQ.", pSF) );
	//pSF->pHFFrequency  = new TDerivedCheckBoxes(TRect(18,2,33,8 ),new TSItem("12 KHZ",
	//											new TSItem("80 KHZ",
	//											new TSItem("150 KHZ",
	//											new TSItem("772 KHZ",
	//											new TSItem("1000 KHZ",
	//											new TSItem("2000 KHZ",0)))))));
	pSF->pHFFrequency  = new TDerivedCheckBoxes(TRect(2,7,33,8 ),new TSItem(SpeckList[6],
												new TSItem(SpeckList[7],0)));

	pSF->insert(pSF->pHFFrequency);
	//Commented by:sdt:16102005:1000
	//pSF->insert(new TButton( TRect( 4, 9 ,13, 11), "~E~dit", cmEditRunFrequency, bfDefault ));
	pSF->insert(new TButton( TRect( 15, 9 ,22 ,11  ), "~O~K", cmOKRunFrequency, bfNormal ));
	pSF->insert(new TButton( TRect( 24, 9 ,35 ,11  ), "~C~ancel", cmCancel, bfNormal ));

	pSF->getFrequencyBits();
	if(!prmarray->nParamType) //For LF Parameters HF Frequencies are disabled.
	{
		pSF->pHFFrequency->setState( sfDisabled, True );
		pSF->pLFFrequency->setData( pSF->runFreq);
		pSF->pLFFrequency->select();
	}
	else
	{
		pSF->pLFFrequency->setData( pSF->runFreq);
		pSF->runFreq->select>>=LFFrequency.nTotalFrequencies; //For HF Frequencies
		pSF->pHFFrequency->setData( pSF->runFreq);
		pSF->pHFFrequency->select();
	}

	TProgram::deskTop->execView( pSF );
	destroy (pSF);

}


void TRunSetupFrequency::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		//Commented by:sdt:16102005
		//Code not required for frequency
		/*if(ev.message.command == cmEditRunFrequency)
		{
			if(this->pHFFrequency->getState(sfFocused))
			{
				iSelRunFreq = *(this->pHFFrequency->iSel);
				iSelRunFreq += LFFrequency.nTotalFrequencies;
				this->pHFFrequency->getData(runFreq);
				//Added by:sdt:09082005:1225
				runFreq->select<<=LFFrequency.nTotalFrequencies;//Added as Limits for HF frequencies stored after LF Frequencies
				//Commented by:sdt:01102005:1410
				//Code not required in quad Software.
				//if(!prmarray->nTestingMethod)//else
				//{
				//	prsdlg->selectRunCombination();
				//}
			}
			else
			{
				iSelRunFreq = *(this->pLFFrequency->iSel);
				this->pLFFrequency->getData(runFreq);
				//Condiotion removed by:sdt:01102005:1300 //for Quad software
				//if(prmarray->nParamType && (iSelRunFreq==0))
				//{
				//	messageBox( "Not valid HF frequency", mfError|mfOKButton );
				//}

				//Commented by:sdt:16102005:0950
				//To avoid forceful deselection of Lowest LF freq. for HF parameters.
				//if(prmarray->nParamType)
				//{
				//	runFreq->select &= 0xFFFE;
				//}
				//else if(!prmarray->nTestingMethod)
				//Commented by:sdt:01102005:1410
				//Code not required in quad Software.
				//if(!prmarray->nTestingMethod)
				//{
				//	prsdlg->selectRunCombination();
				//}
			}

		}*/
		if(ev.message.command == cmOKRunFrequency)
		{

			this->pLFFrequency->getData(runFreq);

			//Added by sdt:16102005:1020
			//To avoid any HF freq bit is set
			runFreq->select&=0x3F;

			ushort usrunFreqTemp = runFreq->select;
			this->pHFFrequency->getData(runFreq);
			runFreq->select<<=LFFrequency.nTotalFrequencies;//For HF Frequencies
			runFreq->select |=  usrunFreqTemp;
			//Commented by:sdt:16102005:0950
			//To avoid forceful deselection of Lowest LF freq. for HF parameters.
			/*if(prmarray->nParamType)
			{
				runFreq->select &= 0xFFFE;
			}*/
			this->setFrequencyBits();

			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );

		}

	}
	//TDialog::handleEvent( ev );
	//clearEvent( ev );
}

TRunSetupFrequency::~TRunSetupFrequency()
{
	delete runFreq;
}

void TRunSetupFrequency::getFrequencyBits()
{
	unsigned long ulFrequency=0;//, ulFreqMask=0xFFFFFFFEl;
	unsigned long ulFreqCheckMask = 0x01l;
	runFreq->select=0;
	for(int i=0;i<MAXFREQUENCIES;i++)
	{
		ulFrequency=runSetup.ulParamRunSetup[iSelRunParam];//Added by:sdt:24082005:2300
		ulFrequency >>= ((i+1)*2);
		runFreq->select |= (ulFrequency & ulFreqCheckMask);
		ulFreqCheckMask = ulFreqCheckMask << 1;
	}
}

void TRunSetupFrequency::setFrequencyBits()
{
	unsigned long ulFrequency=0, ulFreqMask=0xFFFFFFFEl;
	unsigned long ulFreqCheckMask = 0x01l;

	for(int i=0;i<MAXFREQUENCIES;i++)
	{
		ulFrequency=ulFreqCheckMask & runFreq->select;
		unsigned long ulTempMask = ulFreqMask;
		for(int j=0; j < ((i+1)*2) ; j++)
		{
			ulFrequency = (ulFrequency << 1) ;//| 0x1;
			ulTempMask = (ulTempMask << 1) | 0x1;
		}
		runSetup.ulParamRunSetup[iSelRunParam] &= ulTempMask;//Added by:sdt:24082005:2300
		runSetup.ulParamRunSetup[iSelRunParam] |= ulFrequency;//Added by:sdt:24082005:2300

		ulFreqMask = (ulFreqMask << 1) | 0x1;
		ulFreqCheckMask = ulFreqCheckMask << 1;
	}
}

//Commented by:sdt:15112005:1545
/*void TRunSetupDlg::selectRunCombination()
{
	TRunSetupCmbn *pSC = new TRunSetupCmbn( TRect( 23, 8, 57,20),"Select Combination");

	pSC->insert( new TLabel( TRect( 2, 1, 32, 2 ), "PAIR TO PAIR", pSC) );
	pSC->pPairCombinations  = new TDerivedCheckBoxes(TRect( 2, 2, 32, 4 ),	new TSItem("INTRA ADJACENT PAIRS",
														new TSItem("INTRA NONADJACENT PAIRS", 0)));

	pSC->insert(pSC->pPairCombinations);
	pSC->insert( new TLabel( TRect( 2, 5, 32, 6 ), "UNIT TO UNIT", pSC) );
	pSC->pUnitCombinations  = new TDerivedCheckBoxes(TRect( 2, 6, 32, 8 ),	new TSItem("ADJACENT UNITS",
														new TSItem("NON ADJACENT UNITS",0)));
	pSC->insert(pSC->pUnitCombinations);

	pSC->insert(new TButton( TRect( 5, 9, 14 , 11 ), "~O~K", cmOKRunCombination, bfDefault ));
	pSC->insert(new TButton( TRect( 16, 9, 27 , 11 ), "~C~ancel", cmCancel, bfNormal ));

	pSC->getCombinationBits();
	if(iSelRunParam==SRNO_CUPPI)//For Unit to Unit OR Pair to Pair Testing Method.
	{
		pSC->pPairCombinations->setState( sfDisabled, True );
		pSC->pUnitCombinations->setData(pSC->runCmbn);
		pSC->pUnitCombinations->select();//movedTo(1);
	}
	else
	{
		pSC->pUnitCombinations->setState( sfDisabled, True );
		pSC->pPairCombinations->setData(pSC->runCmbn);
		pSC->pPairCombinations->select();//movedTo(1);
	}

	TProgram::deskTop->execView( pSC );
	destroy (pSC);

}*/


//Commented by:sdt:15112005:1545
/*void TRunSetupCmbn::handleEvent(TEvent &ev)
{
	TDialog::handleEvent( ev );
	if ( ev.what == evCommand )
	{
		if(ev.message.command == cmOKRunCombination)
		{
			if(this->pPairCombinations->getState(sfFocused))
			{
				this->pPairCombinations->getData(runCmbn);
				this->setCombinationBits();
			}
			else
			{
				this->pUnitCombinations->getData(runCmbn);
				this->setCombinationBits();
			}

			ev.what = evCommand;
			ev.message.command = cmOK;
			putEvent( ev );

		}
	}
	//TDialog::handleEvent( ev );
	//clearEvent( ev );

}*/
//Commented by:sdt:15112005:1545
//TRunSetupCmbn::~TRunSetupCmbn()
//{
//	delete runCmbn;
//}

//Commented by:sdt:15112005:1545
/*void TRunSetupCmbn::getCombinationBits()
{
	unsigned long ulCombination=0, ulCmbnMask=0x03l;

	ulCombination=runSetup.ulParamRunSetup[iSelRunParam];//Added by:sdt:24082005:2300
	int iShiftCounter = 3 * iSelRunFreq;
	ulCombination>>=iShiftCounter;
	runCmbn->select = ulCombination & ulCmbnMask;

}*/
//Commented by:sdt:15112005:1545
/*void TRunSetupCmbn::setCombinationBits()
{
	unsigned long ulCombination=0, ulCmbnMask=0x3l;//0xFFFFFFFCl;

	ulCombination=0xFFFFFFFCl|runCmbn->select;
	int iShiftCounter = 3 * iSelRunFreq;
	for(int i=0; i<iShiftCounter; i++)
	{
		ulCombination= (ulCombination << 1) | 0x1;
		ulCmbnMask = (ulCmbnMask << 1) ;//| 0x1;
	}

	runSetup.ulParamRunSetup[iSelRunParam] |= ulCmbnMask;//Added by:sdt:24082005:2300
	runSetup.ulParamRunSetup[iSelRunParam] &= ulCombination;//Added by:sdt:24082005:2300

}*/
