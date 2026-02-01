#include<stdio.h>
#include<conio.h>
#include<string.h>


#include "ct.h"
#include "share.h"
#include "key.h"
#include "menu.h"
#include "copyrite.h"

void ReadLimitFromSivFile(int SpeckFileNo);

extern char RamDrv[2];


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
        for(int nParam=0;nParam<MAXPARAMETERS-1;nParam++)
        {
        	for(int nFreq=0;nFreq<MAXFREQUENCIES;nFreq++)
			{
				//for(int nLimitIndex=0;nLimitIndex<4;nLimitIndex++)//Modified by:sdt:02092001
				for(int nLimitIndex=0;nLimitIndex<MAXLIMITTYPES;nLimitIndex++)
				{
					//Commented by:sdt:03082005:1855
					/*strcpy(specksLimit.lowerlimit,"");
					strcpy(specksLimit.upperlimit,"");
			        strcpy(specksLimit.averagelow,"");
			        strcpy(specksLimit.averagehi,"");
			    	strcpy(specksLimit.stdlow,"");
			        strcpy(specksLimit.stdhi,"");
			    	strcpy(specksLimit.rmslow,"");
			        strcpy(specksLimit.rmshi,"");

                    //MOD0507002:01:jj
                    strcpy(specksLimit.psumlow,"");
                    strcpy(specksLimit.psumhi,"");

                    //MOD06072002:01:jj
                    strcpy(specksLimit.wpnlow,"");
					strcpy(specksLimit.wpnhi,"");*/
					speckLimit.bFreqStatus = False;
					speckLimit.bCmbnStatus = False;
					specksLimit.lowerlimit = 0;
					specksLimit.upperlimit = 0;
					specksLimit.averagelow = 0;
					specksLimit.averagehi = 0;
					specksLimit.stdlow = 0;
					specksLimit.stdhi = 0;
					specksLimit.rmslow = 0;
					specksLimit.rmshi = 0;

					//MOD0507002:01:jj
					specksLimit.psumlow = 0;
					specksLimit.psumhi = 0;

					//MOD06072002:01:jj
					specksLimit.wpnlow = 0;
					specksLimit.wpnhi = 0;

					fwrite (&specksLimit, sizeof (SPECKLIMIT), 1,sFp);
				}
			}
		}
	}
    else
    {

        sprintf(szSpeckFile,"%s:\\intmlim.bin",RamDrv);
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
        	sprintf(szSpeckFile,"%s:\\limits.bin",RamDrv);
	     	lFp=fopen(szSpeckFile,"rb");
        }
        if(lFp!=NULL)
        {
	        for(int nParam=0;nParam<MAXPARAMETERS-1;nParam++)
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


    sprintf(szSpeckFile,"%s:\\limits.bin",RamDrv);
    remove(szSpeckFile);
    lFp=fopen(szSpeckFile,"wb");
    for(int nParam=0;nParam<MAXPARAMETERS-1;nParam++)
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
        sprintf(szSpeckFile,"%s:\\intmlim.bin",RamDrv);
    else
        sprintf(szSpeckFile,"%s:\\limits.bin",RamDrv);
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
    sprintf(szSpeckFile,"%s:\\intmlim.bin",RamDrv);
    fp=fopen(szSpeckFile,"rb+");

	//fseek(fp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*4)+(nFreq*4)),SEEK_SET);
    //Modified by:sdt:02092001
    fseek(fp,sizeof(SPECKLIMIT)*((nParam*MAXFREQUENCIES*MAXLIMITTYPES)+(nFreq*MAXLIMITTYPES)),SEEK_SET);
    fseek(fp,sizeof(SPECKLIMIT)*(nLimitIndex),SEEK_CUR);
    fwrite(&specksLimit, sizeof(SPECKLIMIT), 1, fp);
}

//Commented by:sdt:03082005:1900
/*CONFIGLIMIT CovertSpeckLimitToConfigLimit(SPECKLIMIT specksLimit)
{
	CONFIGLIMIT configLimit;
	configLimit.lowerlimit=strcmp(specksLimit.lowerlimit,"")?atof(specksLimit.lowerlimit):BLANKVALUE;
	configLimit.upperlimit=strcmp(specksLimit.upperlimit,"")?atof(specksLimit.upperlimit):BLANKVALUE;
	configLimit.averagelow=strcmp(specksLimit.averagelow,"")?atof(specksLimit.averagelow):BLANKVALUE;
	//configLimit.averagehi=strcmp(specksLimit.averagelow,"")?atof(specksLimit.averagehi):BLANKVALUE;
    //MOD17012002:03:sdt::1545
    //Here first "specksLimit.averagehi" variable was bymistakly written as
    //"specksLimit.averagelow"
    configLimit.averagehi=strcmp(specksLimit.averagehi,"")?atof(specksLimit.averagehi):BLANKVALUE;
	configLimit.stdlow=strcmp(specksLimit.stdlow,"")?atof(specksLimit.stdlow):BLANKVALUE;
	configLimit.stdhi=strcmp(specksLimit.stdhi,"")?atof(specksLimit.stdhi):BLANKVALUE;
	configLimit.rmslow=strcmp(specksLimit.rmslow,"")?atof(specksLimit.rmslow):BLANKVALUE;
	configLimit.rmshi=strcmp(specksLimit.rmshi,"")?atof(specksLimit.rmshi):BLANKVALUE;

    //MOD0507002:01:jj
	configLimit.psumlow=strcmp(specksLimit.psumlow,"")?atof(specksLimit.psumlow):BLANKVALUE;
	configLimit.psumhi=strcmp(specksLimit.psumhi,"")?atof(specksLimit.psumhi):BLANKVALUE;

	//MOD06072002:01:jj
	configLimit.wpnlow=strcmp(specksLimit.wpnlow,"")?atof(specksLimit.wpnlow):BLANKVALUE;
	configLimit.wpnhi=strcmp(specksLimit.wpnhi,"")?atof(specksLimit.wpnhi):BLANKVALUE;

	return configLimit;
}*/

void CopyLimitsToIntmLim(void) //Added by:sdt:21082001
{
	FILE *sFp,*lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));

	sprintf(szSpeckFile,"%s:\\limits.bin",RamDrv);
	sFp=fopen(szSpeckFile,"rb");

    sprintf(szSpeckFile,"%s:\\intmlim.bin",RamDrv);
    remove(szSpeckFile);
    lFp=fopen(szSpeckFile,"wb");
    for(int nParam=0;nParam<MAXPARAMETERS-1;nParam++)
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


/*void SaveLimitsInSivFile(int SpeckFileNo)
{
	FILE *sFp,*lFp;
    SPECKLIMIT specksLimit;
	char szSpeckFile[20];
    memset(&specksLimit,0,sizeof(SPECKLIMIT));
    sprintf(szSpeckFile,"%s:\\intmlim.bin",RamDrv);
    lFp=fopen(szSpeckFile,"rb");

	sprintf(szSpeckFile,"%d.siv",SpeckFileNo);
	sFp=fopen(szSpeckFile,"rb+");
    fseek(sFp,sizeof(COPYRIGHT),SEEK_SET);
    fseek(sFp,sizeof(SPECIFICATION),SEEK_CUR);

    if(lFp!=NULL)
    {
		for(int nParam=0;nParam<MAXPARAMETERS-1;nParam++)
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

//Commented Temporary by:sdt:03082005:1700
//Added by:sdt:02092001
/*void InitSummaryFile(void)
{
	char path[MAXPATH];
    SUMMARYINFO SummaryInfo;
    //SUM Summary;
    FILE *fpSummary;
    memset(&SummaryInfo,0,sizeof(SummaryInfo));
    sprintf(path,"%s:\\summary.bin",RamDrv);
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
    sprintf(path,"%s:\\summary.bin",RamDrv);
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
    sprintf(path,"%s:\\summary.bin",RamDrv);
    fpSummary=fopen(path,"rb");
    fseek(fpSummary,sizeof(SUMMARYINFO)*((nParam*MAXFREQUENCIES*MAXSUMMARYTYPES)+(nFreqNum*MAXSUMMARYTYPES)),SEEK_SET);
    fseek(fpSummary,sizeof(SUMMARYINFO)*(nSumType),SEEK_CUR);
    fread(&SummaryInfo, sizeof(SUMMARYINFO), 1, fpSummary);
    if(fpSummary){fclose(fpSummary);}
	return SummaryInfo;
} */