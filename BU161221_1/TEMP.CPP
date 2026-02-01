#include <stdio.h>
#include <io.h>
#include <alloc.h>
#include <fcntl.h>
#include <process.h>
#include <sys\stat.h>
#include <math.h>
#include <conio.h>
#include <mem.h>

void SortArray(float *Array,int Arraysize);
void InsertNumber(float Val,float *Array,int Arraysize);
int Search(int ArrayIndex,float *Array,float Val,int Arraysize);

int main(void)
{
	int OnePerCount=0;
	float OnePerCuppRaw[10];
	int i=1;
	float fRawReading=0.0;
	clrscr();
	printf("Insert OnePerCount = ");
	scanf("%d",&OnePerCount);
	//OnePerCuppRaw = (float *)farmalloc(sizeof(float)*OnePerCount);
	memset(OnePerCuppRaw,0,OnePerCount);
	do
	{
		printf("\nEnter Raw Reading = ");
		scanf("%f",&fRawReading);
		if(i<OnePerCount+1)
		{
			OnePerCuppRaw[i-1]=fRawReading;
		}
		else
		if((i==OnePerCount+1)&&(OnePerCount!=1))//Condition Modified by:sdt:09022007:2220
		{
			SortArray(OnePerCuppRaw,OnePerCount);
			InsertNumber(fRawReading,OnePerCuppRaw,OnePerCount);
		}
		else
		{
				InsertNumber(fRawReading,OnePerCuppRaw,OnePerCount);
		}
		i++;
	}while(fRawReading!=-999.0);
	return 0;
}



void SortArray(float *Array,int Arraysize)
{
    int temp;
    for(int i=0;i<Arraysize;i++)
    {
	for(int j=i+1;j<Arraysize;j++)
	   {
		if(Array[i]>Array[j])
		  {
			temp=Array[i];
			 Array[i]=Array[j];
			 Array[j]=temp;
		  }
	   }
    }
}

void InsertNumber(float Val,float *Array,int Arraysize)
{
    int nInsertIndex;
	if(Val>=Array[Arraysize-1])
	return;
    else
    if(Val<=Array[0])
	nInsertIndex =0;
    else
    nInsertIndex = Search(ceil(float(Arraysize)/2),Array,Val,ceil(float(Arraysize)/2));

    for(int i=Arraysize-1;i>nInsertIndex;i--)
    {
	Array[i] = Array[i-1];
    }
    Array[nInsertIndex]=Val;
}

int Search(int ArrayIndex,float *Array,float Val,int Arraysize)
{
   int Index;
   if(ArrayIndex>0)//&&ArrayIndex<Arraysize)
   {
	if(Val>Array[ArrayIndex-1]&&Val<=Array[ArrayIndex])
		return ArrayIndex;
   }

   if(Val>Array[ArrayIndex])
   {

		Index = Search(ArrayIndex+((floor(float(Arraysize)/2))==0?1:(floor(float(Arraysize)/2))),Array,Val,((floor(float(Arraysize)/2))==0?1:(floor(float(Arraysize)/2))));
   }
   else
   {
		Index = Search(ArrayIndex-ceil(float(Arraysize)/2),Array,Val,ceil(float(Arraysize)/2));
   }
   return Index;
}
