#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include <io.h>//Added:jj:18042001
#include <fcntl.h>
#include <alloc.h> //Added by:sdt:22042001
#include <dir.h> //Added by:sdt:03052001

#include "ct.h"
#include "share.h"
#include "seq.h" //added by:sdt:22042001
#include "funproto.h"
//Global variable
extern char 	szSeqFileName[15];
extern SEQUENCE CableSequence;

void GetCableCategaryList(char *SeqTitle)
{
	char *StrCategary;
    int nNoOfCategaries;
    char *szpToken;
    char szdpTagTitles[2][30];//Added :sdt:17042001

    strcpy(CableSequence.szSequence_Name,SeqTitle);
    sprintf(szdpTagTitles[0],"CABLESEQ_%s",CableSequence.szSequence_Name);//Added :sdt:17042001
    //strcpy(szdpTagTitles[1],NULL);
    //MOD18012002:02:sdt::1155
    szdpTagTitles[1][0]='\0';
    StrCategary=NewGetPrivateProfileString(szdpTagTitles,"CATEGARY"," ",StrCategary,szSeqFileName,0);//"cable.seq",0);
	nNoOfCategaries=CharOccurInString(StrCategary,',');

 	CableSequence.NumberOfPairs = (int *)farmalloc(sizeof(int)*(nNoOfCategaries+2));
     szpToken=strtok(StrCategary,",");
 int i =0;   
for( i=0;i<nNoOfCategaries+1&&(szpToken!=NULL);i++)
    {
        CableSequence.NumberOfPairs[i]=atoi(szpToken);
        szpToken=strtok(NULL,",");
	}
	CableSequence.NumberOfPairs[i]=-1;
}


void GetCableUnitsList(int Categary)
{
    char szdpTagTitles[3][30],*szpToken;
    char *szpUnits;//modified
    int nTotalUnitTypes;
    int count=0,i=0,j=0; 
    for(i=0;CableSequence.NumberOfPairs[i]!=-1;i++)
    {
       if(CableSequence.NumberOfPairs[i]==Categary)
        break;
	}
    CableSequence.TotalPairs.nIndexPairsCategary =i;

    sprintf(szdpTagTitles[count],"CABLESEQ_%s",CableSequence.szSequence_Name);//Added :sdt:17042001
    sprintf(szdpTagTitles[++count],"%d_PAIRS",Categary);
    //strcpy(szdpTagTitles[++count],NULL);
    //MOD18012002:02:sdt::1155
    szdpTagTitles[++count][0]='\0';

	szpUnits=NewGetPrivateProfileString(szdpTagTitles,"PAIRSIN_UNIT_CATEGARY"," ",szpUnits,szSeqFileName,0);//"cable.seq",0);
    nTotalUnitTypes=CharOccurInString(szpUnits,',');
    CableSequence.TotalPairs.NumberOfUnits = (int *)farmalloc(sizeof(int)*(nTotalUnitTypes+2));
    szpToken=strtok(szpUnits,",");
    for(i=0;i<nTotalUnitTypes+1&&(szpToken!=NULL);i++)
    {
        CableSequence.TotalPairs.NumberOfUnits[i]=atoi(szpToken);
        szpToken=strtok(NULL,",");
	}
	CableSequence.TotalPairs.NumberOfUnits[i]=-1;
}

void GetCableStructure(int Unit)
{
	char szdpTagTitles[4][30], *SeqFile;
    int count=0;
    FILE *pFile;

    sprintf(szdpTagTitles[count],"CABLESEQ_%s",CableSequence.szSequence_Name);//Added :sdt:17042001
    sprintf(szdpTagTitles[++count],"%d_PAIRS",CableSequence.NumberOfPairs[CableSequence.TotalPairs.nIndexPairsCategary]);
    sprintf(szdpTagTitles[++count],"%d_PAIRSINUNIT",Unit);
    //strcpy(szdpTagTitles[++count],NULL);
    //MOD18012002:02:sdt:1205
    szdpTagTitles[++count][0]='\0';

    SeqFile=szSeqFileName;//"cable.seq";
	pFile=ReturnFilePtr(szdpTagTitles,"UNIT_TO_UNIT_ADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.Adj_Units=ReadPairsFromFile(pFile);

    pFile=ReturnFilePtr(szdpTagTitles,"UNIT_TO_UNIT_NONADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.NonAdj_Units=ReadPairsFromFile(pFile);

    pFile=ReturnFilePtr(szdpTagTitles,"PAIR_TO_PAIR_ADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.Adj_Pairs=ReadPairsFromFile(pFile);

    pFile=ReturnFilePtr(szdpTagTitles,"PAIR_TO_PAIR_NONADJACENCIES",SeqFile);
    CableSequence.TotalPairs.Units.NonAdj_Pairs=ReadPairsFromFile(pFile);

    //Added by:sdt:01102005:1500
    //For XTalk Parameters All combinations are used in Quad
    pFile=ReturnFilePtr(szdpTagTitles,"ALL_COMBINATIONS",SeqFile);
	CableSequence.TotalPairs.Units.All_Combinations=ReadPairsFromFile(pFile);

	//MOD2802200601:2230
	//For XTalk Parameters Adjacent Pairs in the cable are used in Quad
	pFile=ReturnFilePtr(szdpTagTitles,"PAIR_ADAJACENCIES_INCABLE",SeqFile);
	CableSequence.TotalPairs.Units.Adj_Pairs_InCable=ReadPairsFromFile(pFile);
}


void FreeSequenceStructure(int bFlag)
{
    SEQPAIR *pTemp,*pCurr;
    if((bFlag==ALLPAIRS)||(bFlag==PAIRSINUNIT)||(bFlag==CBLSTRUCT))
    {
    	pCurr=CableSequence.TotalPairs.Units.Adj_Units;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.Adj_Units=NULL;
        pCurr=CableSequence.TotalPairs.Units.NonAdj_Units;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.NonAdj_Units=NULL;
	   pCurr=CableSequence.TotalPairs.Units.Adj_Pairs;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
     	CableSequence.TotalPairs.Units.Adj_Pairs=NULL;
        pCurr=CableSequence.TotalPairs.Units.NonAdj_Pairs;
        while(pCurr!=NULL)
        {
        	pTemp = pCurr;
            pCurr = pCurr->link;
            free(pTemp);
        }
	   CableSequence.TotalPairs.Units.NonAdj_Pairs=NULL;

	   //Added by:sdt:01102005:1500
	   //For XTalk Parameters All combinations are used in Quad
	   pCurr=CableSequence.TotalPairs.Units.All_Combinations;
	   while(pCurr!=NULL)
	   {
		  pTemp = pCurr;
		  pCurr = pCurr->link;
		  free(pTemp);
	   }
	   CableSequence.TotalPairs.Units.All_Combinations=NULL;

	   //MOD2802200601:2230
	   //For XTalk Parameters Adjacent Pairs in cable are used in Quad
	   pCurr=CableSequence.TotalPairs.Units.Adj_Pairs_InCable;
	   while(pCurr!=NULL)
	   {
		  pTemp = pCurr;
		  pCurr = pCurr->link;
		  free(pTemp);
	   }
	   CableSequence.TotalPairs.Units.Adj_Pairs_InCable=NULL;
    }


    if((bFlag==ALLPAIRS)||(bFlag==PAIRSINUNIT))
    farfree(CableSequence.TotalPairs.NumberOfUnits);

    if(bFlag==ALLPAIRS)
    farfree(CableSequence.NumberOfPairs);
}

SEQPAIR *ReadPairsFromFile(FILE *fpPtr)
{
    int i,j =0;
    char cChar,szString[10],SubString[10];
    SEQPAIR *pNewPair,*pCurrPair,*Pairs;
    cChar=fgetc(fpPtr);
    fseek(fpPtr,-(long(sizeof(cChar))),SEEK_CUR);
    Pairs = pCurrPair=NULL;
    while(cChar!='#'&&cChar!=EOF)
    {
        i=-1;
	int k =0;
        do
        {
            szString[++i]=fgetc(fpPtr);
            if(szString[i]==' '||szString[i]=='\n'||szString[i]=='\t')
            	i--;

        }while(szString[i]!='#'&&szString[i]!=EOF&&szString[i]!=',');
        cChar=szString[i];
		pNewPair=(SEQPAIR *)malloc(sizeof(SEQPAIR));
        pNewPair->link=NULL;

        for(int j=0;szString[j]!='-'&&szString[j]!='#';j++)
        SubString[j]=szString[j];
        SubString[j]='\0';
        pNewPair->Pairno1=atoi(SubString);
        j++;
        for(int k=0;szString[j]!=','&&szString[j]!='#';j++,k++)
        SubString[k]=szString[j];
        SubString[k]='\0';
        pNewPair->Pairno2=atoi(SubString);

        if(Pairs==NULL)
        {
        	Pairs=pNewPair;
            pCurrPair = pNewPair;
        }
        else
        {
        	pCurrPair->link=pNewPair;
            pCurrPair=pCurrPair->link;
        }

    }
    if(fpPtr){fclose(fpPtr);}
    return Pairs;
}

SEQCOUNT CountPairsandUnits(void)
{
    SEQCOUNT SeqCount;
    SEQPAIR *pCurr;
    pCurr=CableSequence.TotalPairs.Units.Adj_Units;
    SeqCount.Adj_Units=0;
    while(pCurr!=NULL)
    {
		SeqCount.Adj_Units++;
        pCurr = pCurr->link;
    }
    pCurr=CableSequence.TotalPairs.Units.NonAdj_Units;
    SeqCount.NonAdj_Units=0;
    while(pCurr!=NULL)
    {
       	SeqCount.NonAdj_Units++;
        pCurr = pCurr->link;
    }
    SeqCount.Adj_Pairs=0;
	pCurr=CableSequence.TotalPairs.Units.Adj_Pairs;
    while(pCurr!=NULL)
    {
       	SeqCount.Adj_Pairs++;
        pCurr = pCurr->link;
    }
    SeqCount.NonAdj_Pairs=0;
    pCurr=CableSequence.TotalPairs.Units.NonAdj_Pairs;
    while(pCurr!=NULL)
    {
        SeqCount.NonAdj_Pairs++;
 	    pCurr = pCurr->link;
    }
    //Added by:sdt:01102005:1500
    //For XTalk Parameters All combinations are used in Quad
    SeqCount.All_Combinations=0;
    pCurr=CableSequence.TotalPairs.Units.All_Combinations;
    while(pCurr!=NULL)
    {
	   SeqCount.All_Combinations++;
	    pCurr = pCurr->link;
	}
	//MOD2802200601:2230
	//For XTalk Parameters Adjacent Pairs in the Cable are used in Quad
	SeqCount.Adj_Pairs_InCable=0;
	pCurr=CableSequence.TotalPairs.Units.Adj_Pairs_InCable;
	while(pCurr!=NULL)
	{
	   SeqCount.Adj_Pairs_InCable++;
	    pCurr = pCurr->link;
    }

	return SeqCount;
}

PAIRSWITCH ReadPairSwitchingOptions()
{
	PAIRSWITCH Pairswitch;
	int nSwitch_Elfext=0, nSwitch_Onext=0, nSwitch_Fext=0, nSwitch_Inext=0;

	Pairswitch.CUPP = GetPrivateProfileInt("PAIR_SWITCHING","CUPP",0,"default.ini");
	Pairswitch.CUPPI = GetPrivateProfileInt("PAIR_SWITCHING","CUPPI",3,"default.ini");
	//Commented by:sdt:24042006:2040
	/*Pairswitch.ELFEXT = GetPrivateProfileInt("PAIR_SWITCHING","ELFEXT",0,"default.ini");
	Pairswitch.ONEXT = GetPrivateProfileInt("PAIR_SWITCHING","ONEXT",0,"default.ini");
	Pairswitch.FEXT = GetPrivateProfileInt("PAIR_SWITCHING","FEXT",0,"default.ini");
	Pairswitch.INEXT = GetPrivateProfileInt("PAIR_SWITCHING","INEXT",0,"default.ini");*/
	//Added by:sdt:24042006:2250:start
	nSwitch_Elfext = GetPrivateProfileInt("PAIR_SWITCHING","ELFEXT",0,"default.ini");
	switch(nSwitch_Elfext)
	{
		case 2 : Pairswitch.ELFEXT = ADJPAIRS_INCABLE;
				 break;
		case 1 : Pairswitch.ELFEXT = ADJPAIRS;
				 break;
		case 0 : Pairswitch.ELFEXT = ALL_COMBINATIONS;
				 break;
		default : Pairswitch.ELFEXT = ALL_COMBINATIONS;
	}
	nSwitch_Onext = GetPrivateProfileInt("PAIR_SWITCHING","ONEXT",0,"default.ini");
	switch(nSwitch_Onext)
	{
		case 2 : Pairswitch.ONEXT = ADJPAIRS_INCABLE;
				 break;
		case 1 : Pairswitch.ONEXT = ADJPAIRS;
				 break;
		case 0 : Pairswitch.ONEXT = ALL_COMBINATIONS;
				 break;
		default : Pairswitch.ONEXT = ALL_COMBINATIONS;
	}
	nSwitch_Fext = GetPrivateProfileInt("PAIR_SWITCHING","FEXT",0,"default.ini");
	switch(nSwitch_Fext)
	{
		case 2 : Pairswitch.FEXT = ADJPAIRS_INCABLE;
				 break;
		case 1 : Pairswitch.FEXT = ADJPAIRS;
				 break;
		case 0 : Pairswitch.FEXT = ALL_COMBINATIONS;
				 break;
		default : Pairswitch.FEXT = ALL_COMBINATIONS;
	}
	nSwitch_Inext = GetPrivateProfileInt("PAIR_SWITCHING","INEXT",0,"default.ini");
	switch(nSwitch_Inext)
	{
		case 2 : Pairswitch.INEXT = ADJPAIRS_INCABLE;
				 break;
		case 1 : Pairswitch.INEXT = ADJPAIRS;
				 break;
		case 0 : Pairswitch.INEXT = ALL_COMBINATIONS;
				 break;
		default : Pairswitch.INEXT = ALL_COMBINATIONS;
	}
	//Added by:sdt:24042006:2250:end
	return Pairswitch;
}

//Function added by:sdt:02072001
//Purpose :- This function will return the No of pairs combinations
//			 for last Unit. Also for unit to unit combinations
//			 it calculates the no of combinatios of Last unit.
SEQCOUNT CountPairsandUnitCmbLastUnit(int UnitSize,int nLastUnit)
{
	SEQCOUNT SeqCount;
    SEQPAIR *pCurr;
    pCurr=CableSequence.TotalPairs.Units.Adj_Units;
    SeqCount.Adj_Units=0;
    while(pCurr!=NULL)
    {
    	if((pCurr->Pairno1 == nLastUnit)||(pCurr->Pairno2 == nLastUnit))
        {
       		SeqCount.Adj_Units++;
        }
		pCurr = pCurr->link;
    }
    pCurr=CableSequence.TotalPairs.Units.NonAdj_Units;
    SeqCount.NonAdj_Units=0;
    while(pCurr!=NULL)
    {
        if((pCurr->Pairno1 == nLastUnit)||(pCurr->Pairno2 == nLastUnit))
        {
       		SeqCount.NonAdj_Units++;
        }
        pCurr = pCurr->link;
    }

    SeqCount.Adj_Pairs=0;
    pCurr=CableSequence.TotalPairs.Units.Adj_Pairs;
    while(pCurr!=NULL)
    {
        if((pCurr->Pairno1 <= UnitSize)&&(pCurr->Pairno2 <= UnitSize))
        {
       		SeqCount.Adj_Pairs++;
        }
        pCurr = pCurr->link;
    }
    SeqCount.NonAdj_Pairs=0;
    pCurr=CableSequence.TotalPairs.Units.NonAdj_Pairs;
    while(pCurr!=NULL)
    {
        if((pCurr->Pairno1 <= UnitSize)&&(pCurr->Pairno2 <= UnitSize))
        {
       		SeqCount.NonAdj_Pairs++;
        }
        pCurr = pCurr->link;
    }

 	return SeqCount;
}
