#if !defined(__SEQ_H)
#define __SEQ_H

#define CABLESEQMNULENGHTH 	8
#define ALLPAIRS			0
#define PAIRSINUNIT			1
#define CBLSTRUCT           2
//Created by:sdt:11042001
typedef struct Pair
{
	int Pairno1;
    int Pairno2;
    struct Pair *link;
}SEQPAIR;

typedef struct Seq
{
	char szSequence_Name[30];
	int far *NumberOfPairs;
	struct pairs
    {
		int nIndexPairsCategary;
		int far *NumberOfUnits;
		struct units
		{
			SEQPAIR *Adj_Pairs;
			SEQPAIR *NonAdj_Pairs;
			SEQPAIR *Adj_Units;
			SEQPAIR *NonAdj_Units;
			SEQPAIR *All_Combinations;
			SEQPAIR *Adj_Pairs_InCable; //MOD2802200601:2210
		}Units;
	}TotalPairs;
}SEQUENCE;

typedef struct SeqCount
{
	int Adj_Pairs;
	int NonAdj_Pairs;
	int Adj_Units;
	int NonAdj_Units;
	int All_Combinations;
	int Adj_Pairs_InCable; //MOD2802200601:2210
}SEQCOUNT;

#endif