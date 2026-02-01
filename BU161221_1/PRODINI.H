#include <stdio.h>

void dGetSeed(char *pszPassword, unsigned char *pucSeed1, unsigned char *pucSeed2);
void DecodeFile(FILE *fpSrc, FILE *fpDest, unsigned char cbSeed1, unsigned char cbSeed2);
void dec(int argc, char *argv1,char *argv2,char *argv3);

void eGetSeed(char *pszPassword, char *pucSeed1, char *pucSeed2);
void EncodeFile(FILE *fpSrc, FILE *fpDest, char cbSeed1, char cbSeed2);
void enc(int argc, char *argv1, char *argv2, char *argv3 );