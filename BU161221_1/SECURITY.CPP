#include "borland_compat.h"
#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <bios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ct.h"
#include "share.h"

int copyFile(char *File1, char *File2);
extern CONFIG nConfig;

char exesignature[13];
char MBRbuf[512];
long getMBR(int drive);
long getDiskSrNo(int Drive);


extern int RuncountExpiryFlag; //Added by:sdt:05012006:2010
//Added:Start:sdt:17092001
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
//end

//*********************************************************
//Following functions have been implemented to
//secure the  S/W from illegal usage.
//*********************************************************
//Added by:sdt:17092001:1505
//start
int error_win(char *msg)
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




//Added:jj:28082001
int onexit()
{
	struct ftime ft;
	//char mess[30];//Commented by:sdt:13112005:1405
	int lckval;
	long pos;
	FILE *fp1;
	fp1=fopen(exesignature,"r+b");
	if(fp1==NULL)
	{
		//closegraph();//Commented by:sdt:11112005:2215
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
    	return FALSE;
	}

	//Commented by:sdt:25112005:1055
	/*if(atof(nConfig.szLength) > 0.0)
    {
		runkey.lckm+=(atoi(nConfig.szNoofpairs)*2* atof(nConfig.szLength)/1000);
        if(runkey.lckm >= LCKM)
        {
			lckval=(int)(runkey.lckm/LCKM);
            runkey.RunCount-=lckval;
            runkey.lckm=(double) (runkey.lckm - (LCKM*lckval));
		    fseek(fp1, -pos,SEEK_END);
		    fwrite(&runkey, sizeof(runkey), 1, fp1);
        }
        else
		{
		    fseek(fp1, -pos,SEEK_END);
		    fwrite(&runkey, sizeof(runkey), 1, fp1);
		}
	}*/
	//Added by:sdt:25112005:1055
	//Now instead of LCKM, file count directly used as runcount.
	runkey.RunCount-=1;
    fseek(fp1, -pos,SEEK_END);
	fwrite(&runkey, sizeof(runkey), 1, fp1);


	fflush(fp1);
	setftime(fileno(fp1), &ft);

    if(fp1){fclose(fp1);}
    return TRUE;
}

void init()
{
    char path[MAXPATH];
	//int i;//Commented by:sdt:13112005:1405

    int disk=getdisk();
	FILE * fp1;
    long pos,srno;
	fp1=fopen(exesignature,"rb");
    if(fp1==NULL)
    {
		//closegraph();//Commented by:sdt:11112005:2215
		clrscr();
	   printf(" \n Error no: 2521 occured in the program ");
	   //printf("\nIf EXE file can not be opened");
	   getch();
	   exit(0);
	}
    getcwd(path,MAXPATH);
    pos=sizeof(runkey);
    fseek(fp1, -pos,SEEK_END);
    fread(&runkey, sizeof(runkey), 1, fp1);
	if(fp1){fclose(fp1);}
	srno=GetSrNo(disk);
	if( runkey.DiskSrNo != srno || strcmp(runkey.Security,SecuString) || stricmp(runkey.path,path) || runkey.InstallCount>0)//Modified:jj:14092001
	{
		//closegraph();//Commented by:sdt:11112005:2215
		clrscr();
		printf(" Sorry ! Cannot run .");
		//printf("\nrunkey.DiskSrNo = %ld     srno = %ld",runkey.DiskSrNo,srno);
		//printf("\nrunkey.Security = %s      SecuString = %s",runkey.Security,SecuString);
		//printf("\nrunkey.path = %s          path = %s",runkey.path,path);
		//printf("\nrunkey.InstallCount = %d",runkey.InstallCount);
		getch();
		exit(0);
	}
	if( runkey.RunCount<=0)
	{
		//closegraph();//Commented by:sdt:11112005:2215
		//Commented by:sdt:05012006:2015
		/*clrscr();
		printf("\n Error no: 2520 occured in the program ");
		//printf("\nrunkey.RunCount = %d",runkey.RunCount);
		getch();
		exit(0);*/
		RuncountExpiryFlag = TRUE;//Added by:sdt:05012006:2010
	}
}

//commented:jj:07092001
/*long GetSrNo(int nDrive)
{
	unsigned char	*pBuf = NULL;
	struct fatinfo	diskinfo;
	long			lSrNo;


	getfat(nDrive+1, &diskinfo);               // 0 is default drive

    pBuf = (unsigned char *) malloc (sizeof(unsigned char)* diskinfo.fi_bysec);

	if (absread(nDrive, 1, 0, pBuf) != 0)  // 0 sector
	{
		closegraph();
    	perror("Disk problem");
		exit(1);
	}

	lSrNo = *(long *)(&pBuf[39]);

    free(pBuf);

	return lSrNo;
}*/

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
  char BOOTbuf[512];
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


void install()
{
	struct ftime ft;
	long pos;
    char ch;
    int disk;
    long srno;
    FILE *fp;
    char path[MAXPATH];
    char path1[MAXPATH];
    char drive[MAXDRIVE];
	int Drive;
    char szCommand[128];

	clrscr();

	printf(" \n Please ensure you are installing from floppy drive \n or else you may corrupt one of your drives\n");
    printf(" \n Do you want to exit install ? (Y/N) ");
    ch=toupper(getche());
    if(ch=='Y')
	exit(1);
	harderr(handler);
    disk = getdisk() ;//+ 'A';
	if ( disk != 0 && disk != 1 )
    {
	printf(" \n Use the installable from floppy ");
		exit(1);
    }

    fp=fopen(exesignature,"r+b");
    if(fp==NULL)
    {
	  printf(" \n Error No: 2523 occured in the program ");
	  exit(1);
    }
    getftime(fileno(fp), &ft);

    srno=GetSrNo(disk);
    pos=sizeof(runkey);
    fseek(fp, -pos,SEEK_END);
    fread(&runkey, sizeof(runkey), 1, fp);
    if(strcmp(runkey.Security,SecuString) || runkey.InstallCount==0 || runkey.FloppySrNo!=srno)
    {
		printf("\n Sorry ! Cannot install");
	if (fp) if(fp){fclose(fp);}
    	exit(1);
    }

    runkey.InstallCount=0;

    printf("\n\n Please enter complete path for installation (Note : no spaces)\n");
    scanf("%s",path);
    printf("\n Please wait :Installation in process...\n");

    if ( access( path, 00 ) != 0 )
    {
    	if ( mkdir( path ,0755) != 0 )
		{
	    	printf("\n Invalid path");
    	    exit(1);
        }
    }

    fnsplit(path,drive,NULL,NULL,NULL);
	Drive=toupper(drive[0])-'A';

    fseek(fp, -pos,SEEK_END);
    runkey.DiskSrNo=GetSrNo(Drive);
    strcpy(runkey.path,path);

    fwrite(&runkey, sizeof(runkey), 1, fp);

    fflush(fp);
	setftime(fileno(fp), &ft);

	if (fp) if(fp){fclose(fp);}

	SetSrNo(disk);

	//Commented Temporary by:sdt:19112005:1225
	//sprintf( szCommand, "copy *.* %s > message",path );
	//system( szCommand );
	//system("del message");
	//Commented by:sdt:19112005:2300
	//strcat(path,"\\");//Added by:sdt:19112005:1225
	//strcat(path,exesignature);//Added by:sdt:19112005:1225
	//copyFile(exesignature,path);//Added by:sdt:19112005:1225

   //Added by:sdt:19112005:2305 //To copy all files to destination
   struct ffblk ffblk;
   int done;
   done = findfirst("*.*",&ffblk,0);
   while (!done)
   {
	  //Commented by:sdt:20112005:1115
	  //strcat(path,"\\");
	  //strcat(path,ffblk.ff_name);
	  //copyFile(ffblk.ff_name,path);
	  //Modified by:sdt:20112005:1115
	  sprintf(path1,"%s\\%s",path,ffblk.ff_name);
	  copyFile(ffblk.ff_name,path1);
	  done = findnext(&ffblk);
   }


    fp=fopen(exesignature,"r+b");
    getftime(fileno(fp), &ft);

	runkey.RunCount=0;
    strcpy(runkey.Security,"ABCDEFG");
    pos=sizeof(runkey);
    fseek(fp, -pos,SEEK_END);
    fwrite(&runkey, sizeof(runkey), 1, fp);

	fflush(fp);
	setftime(fileno(fp), &ft);
    if (fp) if(fp){fclose(fp);}

    printf("\n Installation Complete !");

}

void uninstall()
{
    struct ftime ft;
	int ret;
    long pos;
    char FloppyDrv[2];
	int FDisk;
	char path[MAXPATH];
	struct ffblk ffblk;
    FILE *fp;
    STRUCT_KEY temp;
    int disk;
    long srno;
    char szCommand[128];

	clrscr();
	harderr(handler);


    printf(" \n Insert a clean diskett into Floppy drive \n");
    printf(" \n Please ensure that the Floppy is not write protected \n or else the s/w may be permanently damaged during uninstall \n");
    printf(" \n Now if you are ready ,enter the drive letter for floppy drive \n\t");
    scanf("%s",FloppyDrv);
    FDisk=toupper(FloppyDrv[0])-'A';

	if ( FDisk != 0 && FDisk != 1 )
    {
		printf(" Not a valid Floppy Drive");
		exit(1);
    }

    getcwd(path,MAXPATH);

    fp=fopen(exesignature,"r+b");
    if(fp==NULL)
    {
       printf(" \n Error No: 2524 occured in the program ");
       exit(1);
    }

	printf("\n Please wait :The Software is being uninstalled...\n");

    disk=getdisk();
    srno=GetSrNo(disk);
    pos=sizeof(runkey);
    fseek(fp, -pos,SEEK_END);
	fread(&runkey, sizeof(runkey), 1, fp);
	if (fp) if(fp){fclose(fp);}
    if(strcmp(runkey.Security,SecuString) || runkey.InstallCount!=0 || stricmp(runkey.path,path) || runkey.DiskSrNo!= srno || runkey.RunCount<1)
    {
       	printf("\n Sorry ! Cannot uninstall");
    	if (fp) if(fp){fclose(fp);}
    	exit(1);
    }

    sprintf(path,"%c:\\*.*",FloppyDrv[0],exesignature);
    if(findfirst(path,&ffblk,FA_RDONLY|FA_HIDDEN|FA_ARCH)==0 )
    {
		printf(" \n Please insert a clean diskett and try again");
	exit(1);
	}
	//Commented by:sdt:19112005:2245
	//sprintf( szCommand, "copy *.* %c:\\ > message",FloppyDrv[0]);
	//ret= system( szCommand );
	//Commented by:sdt:19112005:2305
	//sprintf(path,"%c:\\%s",FloppyDrv[0],exesignature);//Added by:sdt:19112005:2225
	//copyFile(exesignature,path);//Added by:sdt:19112005:1225

   //Added by:sdt:19112005:2305 //To copy all files to destination
   int done;
   done = findfirst("*.*",&ffblk,0);
   while (!done)
   {
	  sprintf(path,"%c:\\%s",FloppyDrv[0],ffblk.ff_name);//Added by:sdt:19112005:2225
	  ret=copyFile(ffblk.ff_name,path);//Modified by:sdt:20112005:1145
	  done = findnext(&ffblk);
   }


    if(ret!=0)
    {
       	printf("\n Sorry ! Cannot uninstall");
		printf(" \n The disk is write protected \n Correct the problem and try again");
    	exit(1);
    }

	//system("del message");//Commented by:sdt:19112005:2245

	sprintf(path,"%c:\\%s",FloppyDrv[0],exesignature);
    fp=fopen(path,"r+b");
    if(fp==NULL)
    {
       	printf("\n Sorry ! Cannot uninstall");
		printf(" \n The disk has been write protected \n Correct the problem and try again");
	exit(1);
    }

    getftime(fileno(fp), &ft);

    temp=runkey;

    srno=GetSrNo(FDisk);

    runkey.FloppySrNo=srno;
    SetSrNo(FDisk);         //Changes serial no of floppy during uninstall
    srno=GetSrNo(FDisk);    //The floppy is given new serial no
    						//to avoid 0 as serial no which is default for new floppy

    temp.InstallCount=1;
    temp.FloppySrNo=srno;
    strcpy(temp.path,"");//Added:jj:14092001
    strcpy(temp.Security,SecuString);

    fseek(fp, -pos,SEEK_END);
    fwrite(&temp, sizeof(temp), 1, fp);

    fflush(fp);
	setftime(fileno(fp), &ft);

 	if (fp) if(fp){fclose(fp);}


    fp=fopen(exesignature,"r+b");
    getftime(fileno(fp), &ft);

	runkey.RunCount=0;
    runkey.InstallCount=0;
    strcpy(runkey.Security,"ABCDEF");
    pos=sizeof(runkey);
    fseek(fp, -pos,SEEK_END);
    fwrite(&runkey, sizeof(runkey), 1, fp);

    fflush(fp);
	setftime(fileno(fp), &ft);
    if (fp) if(fp){fclose(fp);}

    printf("\n Software successfully uninstalled !");
    printf("\n Use the Floppy to install again on any system .");

}

void SetSrNo(int nDrive)
{
	unsigned char	*pBuf = NULL;
	struct fatinfo	diskinfo;
	long			lSrNo;

	getfat(nDrive+1, &diskinfo);               // 0 is default drive

	//pBuf = new unsigned char[diskinfo.fi_bysec];
	pBuf = (unsigned char *) malloc (sizeof(unsigned char)* diskinfo.fi_bysec);

	if (absread(nDrive, 1, 0, pBuf) != 0)  // 0 sector
	{
	perror("Disk problem");
		exit(1);
	}
    lSrNo = *(long *)(&pBuf[39]);
    if(runkey.FloppySrNo==lSrNo)
    {

    	randomize();
	    pBuf[39]=(char)b_random (9);
    	pBuf[40]=(char)b_random (9);
	    pBuf[41]=(char)b_random (9);
    	pBuf[42]=(char)b_random (9);

	    if (abswrite(nDrive, 1, 0, pBuf) != 0)  // 0 sector
		{
			perror("Disk problem");
			exit(1);
		}
    }

	//delete pBuf;
    free(pBuf);

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
void getinfo()
{
	long pos;
    FILE *fp1;
    fp1=fopen(exesignature,"rb");
    if(fp1==NULL)
    {
		//closegraph();//Commented by:sdt:11112005:2215
        printf(" \n Error in opening exe \n ");
        exit(0);
    }
    pos=sizeof(runkey);
    fseek(fp1, -pos,SEEK_END);
    fread(&runkey, sizeof(runkey), 1, fp1);
    if( strcmp(runkey.Security,SecuString))
    	exit(0);
	clrscr();

	/*printf(" \t Original Run Count/LCKM	: %d \n ",runkey.OriginalRunCount);
    printf(" \t Covered CKM           	: %lf\n ",(runkey.OriginalRunCount-runkey.RunCount)*LCKM+runkey.lckm);
    printf(" \t Remaining CKM			: %lf\n ",runkey.RunCount * LCKM-runkey.lckm);*/

    printf("\n\t %lf/%d L  ", (runkey.OriginalRunCount-runkey.RunCount)+runkey.lckm/LCKM,runkey.OriginalRunCount);
    getch();
    if(fp1){fclose(fp1);}
}
//end

int copyFile(char *File1, char *File2)
{
   void *buf;
   int handle, bytes, handle1;
   struct ftime ftimep;//Added by:sdt:15012006:2300
   buf = malloc(1);
/*
   Looks for a file in the current directory
   named TEST.$$$ and attempts to read 10
   bytes from it. To use this example you
   should create the file TEST.$$$
*/
   if ((handle =
	  open(File1, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
   {
	 printf("Error Opening File\n");
	 return -1;
   }

   if ((handle1 = open(File2, O_CREAT | O_WRONLY | O_BINARY,
				   S_IWRITE | S_IREAD)) == -1)
   {
	 printf("Error Opening File\n");
	 return -1;
   }

   //_dos_setftime(handle1, ffblk.ff_fdate, ffblk.ff_ftime); //Added by:sdt:15012006:2245

   //if ((bytes = _write(handle, buf, 200)) == -1) {
   //	 printf("Write Failed.\n");
   //	 exit(1);
   //}
   while((bytes = _read(handle, buf, 1)) != 0)
   {
	_write(handle1, buf, 1);
   }

   getftime(handle, &ftimep); //Added by:sdt:15012006:2300
   setftime(handle1, &ftimep); //Added by:sdt:15012006:2300

   close(handle);//Added by:sdt:20112005:1205
   close(handle1);//Added by:sdt:20112005:1205
   return 0;
}
