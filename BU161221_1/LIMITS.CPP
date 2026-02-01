#include<stdio.h>
#include<conio.h>
#include<string.h>


#include "ct.h"
#include "share.h"
#include "common.h"
#include "key.h"
//#include "menu.h"
#include "copyrite.h"
#include "funproto.h"

//Global Variable //Start
extern char cRamDrive[2];

//Global Variable //End


//This function Added by:sdt:20082001
//This will write the Limits in .SPK Files.
//If File is new it will write banks in SIV file, else it will read the
//Values from limits.bin file and write in SIV file.
//limits.BIN file will be present in Ram Drive.
void WriteLimitInSivFile(int SpeckFileNo,int bNewFile)
{
	FILE *sFp,*lFp;
	SPECKLIMIT specksLimit;
	char szSpeckFile[20];
	memset(&specksLimit,0,sizeof(SPECKLIMIT));
	sprintf(szSpeckFile,"%d.spk",SpeckFileNo);
	sFp=fopen(szSpeckFile,"rb+");
	//fseek(sFp,sizeof(COPYRIGHT),SEEK_SET);
	//fseek(sFp,sizeof(SPECIFICATION),SEEK_CUR);
	fseek(sFp,sizeof( Header ), SEEK_SET );
	fseek(sFp,sizeof( STRUCT_SPECKS ),SEEK_CUR );

	if(bNewFile)
	{
		specksLimit.lowerlimit = BLANKVALUE;// 0; //Modified by:sdt:04122015:0615
		specksLimit.upperlimit = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.averagelow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.averagehi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.stdlow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.stdhi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.rmslow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.rmshi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615

		//MOD0507002:01:jj
		specksLimit.psumlow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.psumhi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615

		//MOD06072002:01:jj
		specksLimit.wpnlow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.wpnhi = BLANKVALUE;//0;  //Modified by:sdt:04122015:0615
		specksLimit.anyother = BLANKVALUE;//0; //Modified by:sdt:04122015:0615

		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//-1 removed by:sdt:30092005:2145
		{
			for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
			{
				//for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)//Modified by:sdt:02092001
				for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
				{
					fwrite (&specksLimit, sizeof (SPECKLIMIT), 1,sFp);
				}
			}
		}
	}
    else
    {

		sprintf(szSpeckFile,"%s:\\intmlim.bin",cRamDrive);
     	lFp=fopen(szSpeckFile,"rb");
        //condition Added by:sdt:23082001
        //Function CheckValidHFFreqInSpecks() is called before
		//speckinitialise() and it is overwritting the current specks.
        //In that case "intmlim.bin" is not present in RamDrive, so
		//it was not not write limits part in .SPK file. Thus we are getting
		// a Blank value for Limits. Therefor "limits.bin" used which
		//is present in Ramdrive at this state.
        if(lFp==NULL)
        {
			sprintf(szSpeckFile,"%s:\\limits.bin",cRamDrive);
	     	lFp=fopen(szSpeckFile,"rb");
        }
		if(lFp!=NULL)
        {
			for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//-1 removed by:sdt:30092005:2145
    	    {
        		for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
            	{
	            	//for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)//Modified by:sdt:02092001
                    for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
    	            {
        	        	fread(&specksLimit, sizeof (SPECKLIMIT), 1, lFp);
                    	fwrite (&specksLimit, sizeof (SPECKLIMIT), 1, sFp);
        			}
				}
			}
        }
        if(lFp){fclose(lFp);}
        //remove(szSpeckFile);//Commented by:sdt:22082001
    }
    if(sFp){fclose(sFp);}
    ReadLimitFromSivFile(SpeckFileNo);//Added by:sdt:20082001
}

//This function Added by:sdt:20082001
//This Function will read the limits from .SPK File and will create the
//Limits.bin file in the RamDrive.This file contains Limits for all
//parameters.
void ReadLimitFromSivFile(int SpeckFileNo)
{
	FILE *sFp,*lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
	sprintf(szSpeckFile,"%d.spk",SpeckFileNo);
	sFp=fopen(szSpeckFile,"rb");
	//fseek(sFp,sizeof(COPYRIGHT),SEEK_SET);
	//fseek(sFp,sizeof(SPECIFICATION),SEEK_CUR);
	//Added by:sdt:03082005:1858
	fseek(sFp,sizeof( Header ), SEEK_SET );
	fseek(sFp,sizeof( STRUCT_SPECKS ),SEEK_CUR );


	sprintf(szSpeckFile,"%s:\\limits.bin",cRamDrive);
	remove(szSpeckFile);
    lFp=fopen(szSpeckFile,"wb");
    for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//-1 removed by:sdt:30092005:2145
    {
       	for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
        {
          	//for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)//Modified by:sdt:02092001
            for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
            {
               	fread(&specksLimit, sizeof (SPECKLIMIT), 1, sFp);
            	fwrite (&specksLimit, sizeof (SPECKLIMIT), 1, lFp);
       		}
		}
	}
    if(sFp){fclose(sFp);}
    if(lFp){fclose(lFp);}
}

//Function is Added by:sdt:20082001
//This function will return the limits for given parameter , at given
//Frequency and given Limit Index.
SPECKLIMIT FindLimits(int nParam,int nFreq,int nLimitIndex,int bCreateSpeckFlag)
{
    FILE *lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
    if(bCreateSpeckFlag)
		sprintf(szSpeckFile,"%s:\\intmlim.bin",cRamDrive);
    else
		sprintf(szSpeckFile,"%s:\\limits.bin",cRamDrive);
    lFp=fopen(szSpeckFile,"rb");
    //fseek(lFp,sizeof(SPECKLIMIT)*nParam*nFreq*4,SEEK_SET);//Commented by:sdt:22082001
    //fseek(lFp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*4)+(nFreq*4)),SEEK_SET);
    //Modified by:sdt:02092001
    fseek(lFp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*MAXLIMITTYPES)+(nFreq*MAXLIMITTYPES)),SEEK_SET);
	fseek(lFp,sizeof(SPECKLIMIT)*(nLimitIndex),SEEK_CUR);
	fread(&specksLimit, sizeof (SPECKLIMIT), 1, lFp);
	if(lFp){fclose(lFp);}
    return specksLimit;
}

void ModifyIntermediateLimits(int nParam,int nFreq,int nLimitIndex,SPECKLIMIT specksLimit)
{
    char szSpeckFile[20];
    FILE *fp;
	sprintf(szSpeckFile,"%s:\\intmlim.bin",cRamDrive);
    fp=fopen(szSpeckFile,"rb+");

	//fseek(fp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*4)+(nFreq*4)),SEEK_SET);
    //Modified by:sdt:02092001
    fseek(fp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*MAXLIMITTYPES)+(nFreq*MAXLIMITTYPES)),SEEK_SET);
    fseek(fp,sizeof(SPECKLIMIT)*(nLimitIndex),SEEK_CUR);
    fwrite(&specksLimit, sizeof(SPECKLIMIT), 1, fp);
    if (fp) if(fp){fclose(fp);}
}

//Commented by:sdt:03082005:1900
CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit)
{
	CONFIGLIMIT configLimit;

	configLimit.lowerlimit = specksLimit.lowerlimit;
	configLimit.upperlimit = specksLimit.upperlimit;
	configLimit.averagelow = specksLimit.averagelow;
	configLimit.averagehi = specksLimit.averagehi;
	configLimit.stdlow = specksLimit.stdlow;
	configLimit.stdhi = specksLimit.stdhi;
	configLimit.rmslow = specksLimit.rmslow;
	configLimit.rmshi = specksLimit.rmshi;

	//Grand power sum MOD0507002:01:jj
	configLimit.psumlow = specksLimit.psumlow;
	configLimit.psumhi = specksLimit.psumhi;

	//1%Worst Pair Next MOD06072002:01:jj
	configLimit.wpnlow = specksLimit.wpnlow;
	configLimit.wpnhi = specksLimit.wpnhi;
	configLimit.anyother = specksLimit.anyother;
	return configLimit;
}

void CopyLimitsToIntmLim(void) //Added by:sdt:21082001
{
	FILE *sFp,*lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
	memset(&specksLimit,0,sizeof(SPECKLIMIT));

	sprintf(szSpeckFile,"%s:\\limits.bin",cRamDrive);
	sFp=fopen(szSpeckFile,"rb");

	if(sFp!=NULL)
	{
		sprintf(szSpeckFile,"%s:\\intmlim.bin",cRamDrive);
		remove(szSpeckFile);
		lFp=fopen(szSpeckFile,"wb");
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//-1 removed by:sdt:30092005:2145
		{
			for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
			{
				//for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)//Modified by:sdt:02092001
				for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
				{
					fread(&specksLimit, sizeof (SPECKLIMIT), 1, sFp);
					fwrite (&specksLimit, sizeof (SPECKLIMIT), 1, lFp);
				}
			}
		}
		if(sFp){fclose(sFp);}
		if(lFp){fclose(lFp);}
	}
	else
	{
		specksLimit.lowerlimit = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.upperlimit = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.averagelow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.averagehi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.stdlow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.stdhi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.rmslow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.rmshi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615

		//MOD0507002:01:jj
		specksLimit.psumlow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.psumhi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		//MOD06072002:01:jj
		specksLimit.wpnlow = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.wpnhi = BLANKVALUE;//0; //Modified by:sdt:04122015:0615
		specksLimit.anyother = BLANKVALUE;//0; //Modified by:sdt:04122015:0615

		sFp=fopen(szSpeckFile,"wb");
		sprintf(szSpeckFile,"%s:\\intmlim.bin",cRamDrive);
		remove(szSpeckFile);
		lFp=fopen(szSpeckFile,"wb");
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//-1 removed by:sdt:30092005:2145
		{
			for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
			{
				//for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)//Modified by:sdt:02092001
				for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
				{
					fwrite (&specksLimit, sizeof (SPECKLIMIT), 1,sFp);
					fwrite (&specksLimit, sizeof (SPECKLIMIT), 1,lFp);
				}
			}
		}
		if(sFp){fclose(sFp);}
		if(lFp){fclose(lFp);}
	}
}


/*void SaveLimitsInSivFile(int SpeckFileNo)
{
	FILE *sFp,*lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
	sprintf(szSpeckFile,"%s:\\intmlim.bin",cRamDrive);
    lFp=fopen(szSpeckFile,"rb");

	sprintf(szSpeckFile,"%d.siv",SpeckFileNo);
	sFp=fopen(szSpeckFile,"rb+");
    fseek(sFp,sizeof(COPYRIGHT),SEEK_SET);
    fseek(sFp,sizeof(SPECIFICATION),SEEK_CUR);

    if(lFp!=NULL)
    {
		for(int nParam=0;nParam<MAXPARAMETERS;nParam++)//-1 removed by:sdt:30092005:2145
    	{
        	for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
            {
	           	for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)
    	        {
        	       	fread(&specksLimit, sizeof (SPECKLIMIT), 1, lFp);
                   	fwrite (&specksLimit, sizeof (SPECKLIMIT), 1, sFp);
        		}
			}
		}
	}
	if(lFp){fclose(lFp);}
	if(sFp){fclose(sFp);}
	ReadLimitFromSivFile(SpeckFileNo);//Added by:sdt:20082001
}*/

//Added by:sdt:02092001
void InitSummaryFile(void)
{
	char path[MAXPATH];
    SUMMARYINFO SummaryInfo;
    //SUM Summary;
    FILE *fpSummary;
    memset(&SummaryInfo,0,sizeof(SummaryInfo));
	sprintf(path,"%s:\\summary.bin",cRamDrive);
    remove(path);
    fpSummary=fopen(path,"wb");
    for(int nParam=0;nParam<MAXPARAMETERS;nParam++)
	{
		for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
        {
        	for(int nSumType=0;nSumType<MAXSUMMARYTYPES;nSumType++)
            {
            	fwrite(&SummaryInfo,sizeof(SUMMARYINFO),1,fpSummary);
            }
		}
	}
}

//Added by:sdt:02092001
void WriteSummary(int nParam,int nFreqNum,int nSumType,SUMMARYINFO SummaryInfo)
{
	char path[MAXPATH];
    FILE *fpSummary;
	sprintf(path,"%s:\\summary.bin",cRamDrive);
    fpSummary=fopen(path,"rb+");
    fseek(fpSummary,sizeof(SUMMARYINFO)*((nParam*MAXFREQUENCIES*MAXSUMMARYTYPES)+(nFreqNum*MAXSUMMARYTYPES)),SEEK_SET);
	fseek(fpSummary,sizeof(SUMMARYINFO)*(nSumType),SEEK_CUR);
    fwrite(&SummaryInfo, sizeof(SUMMARYINFO), 1, fpSummary);
    if(fpSummary){fclose(fpSummary);}
}

//Added by:sdt:03092001
SUMMARYINFO ReadSummary(int nParam,int nFreqNum,int nSumType)
{
	char path[MAXPATH];
    FILE *fpSummary;
    SUMMARYINFO SummaryInfo;
    memset(&SummaryInfo,0,sizeof(SUMMARYINFO));
	sprintf(path,"%s:\\summary.bin",cRamDrive);
    fpSummary=fopen(path,"rb");
    fseek(fpSummary,sizeof(SUMMARYINFO)*((nParam*MAXFREQUENCIES*MAXSUMMARYTYPES)+(nFreqNum*MAXSUMMARYTYPES)),SEEK_SET);
    fseek(fpSummary,sizeof(SUMMARYINFO)*(nSumType),SEEK_CUR);
    fread(&SummaryInfo, sizeof(SUMMARYINFO), 1, fpSummary);
    if(fpSummary){fclose(fpSummary);}
	return SummaryInfo;
}