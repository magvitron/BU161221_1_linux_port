//========================================================================//
//					Modifications
//Date :- 16062001
//1. Changes done for New parameters INEXT & FEXT.
//   Changes Are :-
//					a. In Param, ParamAb, Meth, Pos, etc. arrays values
//					   for above parameters are added.
//					b. Values for INEXT are added in SYSFIX.1,.2&.3 Files.
//========================================================================//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include <dos.h>
#include <io.h>
#include <bios.h>
#include <dir.h>

#include "prodini.c"//Added:sdt:18092001

//Added:Start:sdt:18092001
char MBRbuf[512];
char BOOTbuf[512];
long getMBR(int drive);
long getDiskSrNo(int Drive);

#define IGNORE  0
#define RETRY   1
#define ABORT   2
#define SecuString	"EPOINTECH"
typedef struct key
{
	char	Security[10];
	char 	path[MAXPATH];
	char	exename[13];
	int 	OriginalRunCount;
	int 	InstallCount;
	int 	RunCount;
	long    FloppySrNo;
	long    DiskSrNo;
	double	lckm;	//Added:jj:21082001
}STRUCT_KEY;
long GetSrNo(int nDrive);
void GetExeName(char *path);
void init();
int onexit();
void getinfo();
STRUCT_KEY runkey;
 static char *err_msg[] = {
     "write protect",
     "unknown unit",
     "drive not ready",
     "unknown command",
     "data error (CRC)",
     "bad request",
	 "seek error",
	 "unknown media type",
	 "sector not found",
	 "printer out of paper",
	 "write fault",
	 "read fault",
	 "general failure",
	 "reserved",
	 "reserved",
	 "invalid disk change"
 };

error_win(char *msg)
{
	int retval;

	cputs(msg);

	retval = ABORT;
	return retval;
}
 #pragma warn -par

 int handler(int errval,int ax,int bp,int si)
 {
	static char msg[80];
	unsigned di;
	int drive;
	int errorno;

    di= _DI;
 /*
 if this is not a disk error then it was
 another device having trouble
 */

    if (ax < 0)
    {
       /* report the error */
       error_win("Device error");
       /* and return to the program directly requesting abort */
       hardretn(ABORT);
    }
 /* otherwise it was a disk error */
    drive = ax & 0x00FF;
    errorno = di & 0x00FF;
 /* report which error it was */

	sprintf(msg, "Error: %s on drive %c\r\nPlease rectify the problem and try again",
	err_msg[errorno], 'A' + drive);
 /*
 return to the program via dos interrupt 0x23 with abort, retry,
 or ignore as input by the user.
 */
    hardresume(error_win(msg));
    return ABORT;
 }
 #pragma warn +par

char exesignature[13];
//End:sdt:18092001


#define 	FALSE			0
#define		NA				0
#define		MAXFIXTURES		3
//#define		MAXFREQ			4
#define		MAXFREQ			8//10   //changed 4 to 10 :sdt:06052001
#define		MAXRANGES		4
//#define		MAXPARAMS		12
#define		MAXPARAMS		11//16//14 //Modified by:sdt:11112005:2345 for Quad Software.
#define		MAXMETHODS		5
#define		MAXPOSITIONS	10
//#define		PARAARRAY		14 //Changed:13 to 14 :P.N.B.(05101999)
//#define		PARAARRAY		15 //Changed:14 to 15 :P.N.B.(08101999)
#define		PARAARRAY		11//16 //Modified by:sdt:11112005:2345 for Quad Software.
struct SysOffsetLocation
{
	unsigned char FixNo:2;
	unsigned char Freq :4;//3;//MOD18012002:03:sdt::1255
	unsigned char Range:3;
	unsigned char ParamAbr:4;
	unsigned char Method:3;
	unsigned char nPos:4;
};

struct  SysOffVal
{
	float fSysOffsetVal;
	union
	{
		SysOffsetLocation m_SysOffsetLocation;
		unsigned long lLocation;
	};
    int Dummy;//Added:sdt:18092001
};

int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szDefault,
										char *szStr,char *FileName,int nDefault);

FILE *fix,*sys;

SysOffVal 	OffsetVal,Location;

void main(int argc,char *argv[])//Modified:jj:22012001:1230
{
	 //Modified:sdt:17092001
	 //char szstr[10];
     char szstr[MAXPATH];

	 char buf1[20],buf3[150],buf2[20];
	 char  * val;
	 int  i,j,k,m;
	 char Param[PARAARRAY][PARAARRAY+5] =
								{
										{"CR_NA_TIP"},
										{"CR_NA_RING"},
										{"CM_RAMP1_NA"},
										{"CM_RAMP2_NA"},
										{"CM_NORMAL_NA"},
										{"CUPP_NA_NA"},
										{"CUPG_NA_NA"},
										{"CUPPI_NA_NA"}, //Added by:sdt:06052001
										//{"GM_NA_NA"},
										{"ATN_NA_NA"},
										{"ONEXT_NA_NA"},
										{"FEXT_NA_NA"}//Added by:sdt:17062001
							};

	 char ParamAb[PARAARRAY] = {    	1,//{"CR"}, //Modified by:sdt:06052001
										1,//{"CR"}, //entry for GM is modiied
										2,//{"CM"}, //entry for fext is removed
										2,//{"CM"},
										2,//{"CM"},
										3,//{"CUPP"},
										4,//{"CUPG"},
										5,//{"CUPPI"}
										7,//6 //{"ATN"}, //Values modified by:sdt:26112005:2150
										8,//7 //{"ONEXT"},
										9//8  //{"FEXT"}//Added by:sdt:17062001
							   };

	 char Meth[PARAARRAY] =  {	    	0,//{"_NA"},
										0,//{"_NA"},
										1,//{"_RAMP1"},
										2,//{"_RAMP2"},
										3,//{"_NORMAL"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
							 };

	 char Pos[PARAARRAY] =  {   		1,//{"_TIP"},
										2,//{"_RING"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
										0,//{"_NA"},
							};

	 if( !strcmp(argv[1],"ver") )//Added:jj:22012001
	 {
		char version[10];
		clrscr();
		printf("\n�FX20118");//Alt159
		//GetPrivateProfileString ("MAIN","VER","FX110122",version,"ver.sig",0);
		//Modified by:start:sdt:18092001
		dec (3,"ever.sig", "ver.sig","");
		if(fopen("ver.sig","rb"))
		{
			GetPrivateProfileString ("MAIN","VER","FX110122",version,"ver.sig",0);
			remove("ver.sig");
		}
		else
		strcpy(version,"QXX51126");
		//end:sdt:18092001

     	printf("\nCompatible to  %s",version);

        getch();
        exit(0);
     }

    //Added:Start:sdt:18092001
	GetExeName(argv[0]);
    if(stricmp(argv[1],"/showinfo")==0)
    {
      	getinfo();
        exit(1);
    }
	if(argc < 3)
	   return;
	init();
    if ( access( argv[1], 00 ) != 0 )
	{
	    	printf("\n Invalid source path");
    	    exit(1);
    }
    //End:sdt:18092001

	//Modified:sdt:18092001
	//sys=fopen("sys.off","wb+");
    sys=fopen(argv[2],"wb+");

	clrscr();
    printf("\n Please wait...\n"); //added:sdt:18092001
	memset (&OffsetVal,sizeof (OffsetVal),0);

	for(i=0;i<=MAXFIXTURES-1;i++)
	{
        //Modified:sdt:18092001
		//sprintf(szstr,"sysfix.%d",i+1);
		sprintf(szstr,"%s\\sysfix.%d",argv[1],i+1);
//		fix=fopen(szstr,"rt");
		OffsetVal.m_SysOffsetLocation.FixNo = (unsigned char)(i+1);
		for(j=0;j<=MAXFREQ-1;j++)
		{
			sprintf(buf1,"FREQ_%d",j+1);
			OffsetVal.m_SysOffsetLocation.Freq = (unsigned char)(j+1);
			for(k=0;k<=MAXPARAMS;k++)
			{
				sprintf(buf2,Param[k]);
				//printf("%s  ",buf2);
				OffsetVal.m_SysOffsetLocation.ParamAbr = (unsigned char) (ParamAb[k]);
				OffsetVal.m_SysOffsetLocation.Method = (unsigned char) (Meth[k]);
				OffsetVal.m_SysOffsetLocation.nPos = (unsigned char) (Pos[k]);
				GetPrivateProfileString(buf1,buf2,"",buf3,szstr,0);
//				printf("%s  \n",buf3);
				val = strtok(buf3,",");
				for(m=0;m<=MAXRANGES-1;m++)
				{
					if(strcmp(val,NA)==0)
					{
						val = 0;
						OffsetVal.fSysOffsetVal = atof(val);
						OffsetVal.m_SysOffsetLocation.Range = (unsigned char)(m+1);
					}
					else
					{
						OffsetVal.fSysOffsetVal = atof(val);
						OffsetVal.m_SysOffsetLocation.Range = (unsigned char)(m+1);
					}
					fwrite(&OffsetVal,sizeof(OffsetVal),1,sys);
					memset (&OffsetVal,sizeof (OffsetVal),0);
					val = strtok(NULL,",");
				}
			}
		}

	}
	if(sys){fclose(sys);}
	onexit();//Added:sdt:18092001
}

int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szDefault,char *szStr,char *FileName,int nDefault)
{
	FILE  *pFile;
	char	cBuf[150], *szTemp;
	int	nLen, i,TokenFound=0;

	if((pFile=fopen(FileName,"rt"))==NULL)
		return FALSE;
	while(fgets(cBuf,150,pFile)!=NULL)
	{
		szTemp=strstr(cBuf,szAppName);
		if(szTemp!=NULL)
			TokenFound=1;
		szTemp=strstr(cBuf,szkeyname);
		if(szTemp!=NULL&&TokenFound)
		{
			szTemp+=strlen(szkeyname);
			nLen=strlen(szTemp);
			i=0;
			while((nLen)&&(*szTemp!='='))
			{
				nLen--;
				szTemp++;
			}
			if(nLen)
			{
				szTemp++;
				nLen=strlen( szTemp);
				i=0;
				if(nDefault)
				{
					 while((nLen)&&(*szTemp==' '))
					 {
						 nLen--;
						 szTemp++;
					 }
				}
				if(nLen)
				{
					if(pFile){fclose(pFile);}
					for(i=0,nLen=strlen(szTemp);nLen&&(*szTemp!='\n')&&(*szTemp!=0)&&(*szTemp!=EOF);szTemp++,i++)
						szStr[i] = *szTemp;
					szStr[i] = 0;
					return i;
				}
			}
		}
	}
	if(pFile){fclose(pFile);}
	return sprintf(szStr,szDefault);

}	// End of GetPrivateProfileString


//Added:Start::sdt:18092001
long GetSrNo(int nDrive)
{
    if(nDrive==0 || nDrive==1)
		return(getMBR(nDrive));//FOR FLOPPY
    else
    	return(getMBR(0x80));//FOR HARDDISK
}

//READING MASTER BOOT RECORD
long getMBR(int drive)
{
  long lSrNo;
  int result;
  struct diskinfo_t dinfo;
  dinfo.drive =  drive;
  dinfo.head  =  0;    /* disk head number */
  dinfo.track =  0;    /* track number */
  dinfo.sector  =  1;  /* sector number */
  dinfo.nsectors =  1; /* sector count */
  dinfo.buffer = MBRbuf; /* data buffer */
  result = _bios_disk(_DISK_READ, &dinfo);
  result = _bios_disk(_DISK_READ, &dinfo);
  result = _bios_disk(_DISK_READ, &dinfo);
  if ((result & 0xff00) != 0)
    printf("Disk problem.\n");
  if (drive==0 || drive==1)
  {
    lSrNo = *(long *)(&MBRbuf[39]);
    return(lSrNo);
  }
  else
  {
   	return(getDiskSrNo(drive));
  }
}

//READING BOOT RECORD OF FIRST PARTITION
long getDiskSrNo(int Drive)
{
  long lSrNo;
  struct diskinfo_t dinfo;
  int result,x;
  dinfo.drive =  Drive;
  dinfo.head  =  (unsigned)(MBRbuf[447]);    /* disk head number */
  dinfo.track =  0;    /* track number */

  x=(int)(MBRbuf[447] & 0x03f);
  dinfo.sector= x; //(int)(dbuf[447] & );  /* sector number */
  dinfo.nsectors =  1; /* sector count */
  dinfo.buffer = BOOTbuf; /* data buffer */
  result = _bios_disk(_DISK_READ, &dinfo);
  result = _bios_disk(_DISK_READ, &dinfo);
  result = _bios_disk(_DISK_READ, &dinfo);
  if ((result & 0xff00) != 0)
    printf("Disk problem.\n");
  else
  {
    if((MBRbuf[450] & 0x03f)==0x06|| (MBRbuf[450] & 0x03f)==0x04 || (MBRbuf[450] & 0x03f)==0x01)
    {
	    lSrNo = *(long *)(&BOOTbuf[39]);
    	return(lSrNo);
    }
	else
    {
	    lSrNo = *(long *)(&BOOTbuf[67]);
    	return(lSrNo);
    }
  }
}

void GetExeName(char *path)
{
	char drive[MAXDRIVE];
	char dir[MAXDIR];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;

	flags=fnsplit(path,drive,dir,file,ext);
    strcpy(exesignature,file);
    strcat(exesignature,".exe");
}
void init()
{
	struct ftime ft;
    int		disk;
    FILE 	*fp;
    long pos,srno;
	harderr(handler);
    disk = getdisk() ;//+ 'A';
	if ( disk != 0 && disk != 1 )
    {
    	printf(" \n Use the software from floppy ");
		exit(1);
	}
    fp=fopen(exesignature,"r+b");
    if(fp==NULL)
    {
       printf(" \n Error No: 2521 occured in the program ");
       exit(1);
    }
    srno=GetSrNo(disk);
    pos=sizeof(runkey);
    fseek(fp, -pos,SEEK_END);
    fread(&runkey, sizeof(runkey), 1, fp);
    if(strcmp(runkey.Security,SecuString) || runkey.FloppySrNo!=srno)
    {
       	printf("\n Sorry ! Not an authorized usage .");
    	if (fp) if(fp){fclose(fp);}
    	exit(1);
    }
    if (fp) if(fp){fclose(fp);}
    if( runkey.RunCount<=0)
	{
        clrscr();
    	printf("\n Error no: 2520 occured in the program ");
        exit(0);

    }

    fp=fopen(exesignature,"r+b");
    getftime(fileno(fp), &ft);
    if(fp==NULL)
    {
       printf(" \n Error No: 2521 occured in the program ");
       exit(1);
    }
    pos=sizeof(runkey);
    fseek(fp, -pos,SEEK_END);
    fwrite(&runkey, sizeof(runkey), 1, fp);

    fflush(fp);
	setftime(fileno(fp), &ft);

	if (fp) if(fp){fclose(fp);}

}
int onexit()
{
   	struct ftime ft;
	long pos;
    FILE *fp1;
    fp1=fopen(exesignature,"r+b");
    if(fp1==NULL)
    {
        printf(" \n Error no: 2522 occured in the program ");
        exit(0);
    }
    getftime(fileno(fp1), &ft);
    pos=sizeof(runkey);
	fseek(fp1, -pos,SEEK_END);
	fread(&runkey, sizeof(runkey), 1, fp1);
    if( strcmp(runkey.Security,SecuString))
    {
        if(fp1){fclose(fp1);}
    	return 0;
    }

    runkey.RunCount--;
    fseek(fp1, -pos,SEEK_END);
    fwrite(&runkey, sizeof(runkey), 1, fp1);

    fflush(fp1);
	setftime(fileno(fp1), &ft);

    if(fp1){fclose(fp1);}
    return 1;
}
void getinfo()
{
	long pos;
    FILE *fp1;
    fp1=fopen(exesignature,"r");
    if(fp1==NULL)
    {
        printf(" \n Error in opening exe \n ");
        exit(0);
    }
    pos=sizeof(runkey);
    fseek(fp1, -pos,SEEK_END);
    fread(&runkey, sizeof(runkey), 1, fp1);
    if( strcmp(runkey.Security,SecuString))
    	exit(0);
    clrscr();

    printf("\n\t %d/%d ", (runkey.OriginalRunCount-runkey.RunCount),runkey.OriginalRunCount);
    getch();
    if(fp1){fclose(fp1);}
}
//End:sdt:18092001
