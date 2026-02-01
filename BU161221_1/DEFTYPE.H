//
// Deftype.H - Includes all type definations and constants used throughout
//				the system
//
#if !defined ( __DEFTYPE_H )
#define __DEFTYPE_H
typedef unsigned char	BYTE;
//typedef int			CtrlLine; //Commented by:sdt:31072005:2340
typedef float 			VALUE;
typedef BYTE			Allignment;
//typedef unsigned int	WORD;
//typedef unsigned int	BOOL;
//typedef unsigned int    WORD;

#define TRUE			1
#define FALSE           0
#define ON				1
#define OFF				0

#define LOBYTE(w) ((BYTE) (w))
#define HIBYTE(w) ((BYTE) (((WORD)w >> 8) & 0xFF))


#endif
