/* spawnlp example */
#include <process.h>
#include <stdio.h>
#include <errno.h>

void main(int argc, char *argv[])
{
	FILE *fp1, *fp2;
	char ch;
	fp1 = fopen("temp.exe","rb");
	fp2 = fopen("temp1.exe","wb");
	while(fp1!=NULL)
	//do
	{
		fread (&ch, sizeof (ch), 1, fp1);
		fwrite(&ch, sizeof (ch), 1, fp2);
		//ch = fgetc(fp1);
		//if(ch!=EOF)
		//{
		//	fputc(ch, fp2);
		//}
	}//while(ch!=EOF);

	if(fp1){fclose(fp1);}
	if(fp2){fclose(fp2);}
}

/*#include <process.h>
#include <stdio.h>
#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

int main( int argc, char *argv[])
{
   double 		far *nNormUnnormMatrix;
   nNormUnnormMatrix = (double *)farmalloc(sizeof(double)*atoi(argv[1]));
   farfree(nNormUnnormMatrix);
   printf("Bytes Allocated = %ld ",(sizeof(double)*atoi(argv[1])));
   return 0;
}*/

