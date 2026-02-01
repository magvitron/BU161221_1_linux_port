//========================================================================//
//					Modifications
//Date :- 16062001
//1. Changes done for New parameters INEXT & FEXT.
//   Changes Are :-
//					a. In ParameterAbr, etc. arrays values
//						  for above parameters are added.
//					b. Values for FEXT are added in OFFSET.FIX Files.
//========================================================================//

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>  //Added by:18092001
#include <bios.h> //Added by:18092001
#include <io.h> //Added by:18092001

#include "prodini.c"//Added:sdt:18092001

#define PARAMETERSTRLEN 9
#define MAXPARAMETERS 6  //Modified by:sdt:17062001
//#define MAXPARAMETERS 	11 //Changed : Added: P.N.B.(0810199)
#define INTRABLOCK 		0
#define INTERBLOCK 		1
//#define	MAXFREQ			4//Commented by:sdt:15052001
#define	MAXFREQ			8//10	//Modified by:sdt:11112005:2330
#define V1 0
#define V2 1
#define NA 0


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
void getinfo();//Added:jj:03092001
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

int GetPrivateProfileString (char * ,char * ,char * ,char * ,char * ,int );

void GetAndWriteOffset (char *);

struct Location
{
   unsigned char ParameterAbr : 5;
   unsigned char Freq : 4;//3; //MOD18012002:03:sdt::1255
   unsigned char Block : 3;
   unsigned char Voltage : 2;
   unsigned char Range : 3;
};

struct Offset
{
   union
   {
	  Location m_Location;
	  unsigned int nLocation;
   };
   float fOffset;
   int Dummy;//Added:sdt:18092001
};

Offset m_Offset;

char ParameterAbr [MAXPARAMETERS][PARAMETERSTRLEN] = { {"ATN"},
													  {"NEXT"},
													  {"FEXT"},//added here from last by:sdt:26112005:2350
													  {"ELFEXT"},
						  // Changed(below):Added:P.N.B.(05101999)
													  {"INEXT"},
						  // Changed(below):Added:P.N.B.(05101999)
													  {"ZCH"}
													};

char Voltage [][3] = {   {"V1"},{"V2"}  };
char Block [][10] = {   {"INTRA"},{"INTER"}	};

FILE *fptr_fixoff;



void main (int argc,char *argv[])
{
	int 	block = 0,voltage = 0,param = 0,Freq=0;
	char 	str[150],key[150];
    char    szApp[150];

    if( !strcmp(argv[1],"ver") )//Added:jj:22012001:1215
     {
     	char version[10];
        clrscr();
		printf("\n�FX20118");//Alt213
        //Modified by:start:sdt:18092001

		dec (3,"ever.sig", "ver.sig","");
        if(fopen("ver.sig","rb"))
        {
       		GetPrivateProfileString ("MAIN","VER","FX110122",version,"ver.sig",0);
            remove("ver.sig");
        }
        else
		strcpy(version,"QXX51111");
        //end:sdt:18092001

     	printf("\nCompatible to  %s",version);
        getch();
        exit(0);
     }

	clrscr ();

	//Added:Start:sdt:18092001
	GetExeName(argv[0]);
    if(stricmp(argv[1],"/showinfo")==0)
	{
      	getinfo();
        exit(1);
    }
	if(argc < 3)
	   return;
    if ( access( argv[1], 00 ) != 0 )
    {
	    	printf("\n Invalid source ");
            exit(1);
    }
	init();
    //End:sdt:18092001

	printf("\n Please wait...\n");//added:sdt:18092001

    //Modified by:sdt:18092001
	//fptr_fixoff = fopen ("fix.off","wb");
    fptr_fixoff = fopen (argv[2],"wb");
    if(fptr_fixoff==NULL)//Added:sd:18092001
		exit(1);
	memset (&m_Offset,sizeof (m_Offset),0);

	for( Freq = 0;Freq < MAXFREQ; Freq++ )
	{
		m_Offset.m_Location.Freq = Freq+1;
		for (voltage = 0;voltage <= V2;voltage ++)
		{
			sprintf( szApp, "%s_FREQ%d",Voltage[voltage],Freq+1);
			m_Offset.m_Location.Voltage = voltage;
			for (block = 0;block <= INTERBLOCK;block ++)
			{
				m_Offset.m_Location.Block = block;
				for (param = 0;param < MAXPARAMETERS ;param ++)
				{
					 m_Offset.m_Location.ParameterAbr = param;
					 sprintf (key,"%s_%s_%d",ParameterAbr [param],Block[block],voltage + 1);

                     //Modified by:sdt:18092001
					 //GetPrivateProfileString (szApp,key,"0.0",str,"offset.fix",0);
					 GetPrivateProfileString (szApp,key,"0.0",str,argv[1],0);
					 GetAndWriteOffset (str);
				}
			}
		}
   	}
	if(fptr_fixoff){fclose(fptr_fixoff);}
    onexit();//Added:sdt:18092001

}


void GetAndWriteOffset (char *str)
{
	char *p;
	int Range = 0;

	p = strtok (str,",");
	if (p)
	{
		do
		{
			m_Offset.m_Location.Range = Range++;
			if (!(strcmp (p,"NA")))
			   m_Offset.fOffset = NA;
			else
			   m_Offset.fOffset = atof (p);

				fwrite (&m_Offset,sizeof (m_Offset),1,fptr_fixoff);
			p = strtok (NULL,",");
		}while (p);
	}
}

int GetPrivateProfileString (char *szAppName,char *szkeyname,char *szDefault,
										char *szStr,char *FileName,int nDefault)
{
	FILE  *pFile;
	char	cBuf[150], *szTemp;
	int	nLen, i,TokenFound=0;

	if((pFile=fopen(FileName,"rt"))==NULL)
		return 0;
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
	return sprintf(szStr,szDefault);;
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
	if ( disk != 00 && disk != 1 )
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

	 if(fp){fclose(fp);}

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