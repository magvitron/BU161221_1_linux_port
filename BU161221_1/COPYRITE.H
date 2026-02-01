#if !defined( __COPYRITE_H )
#define __COPYRITE_H

#include <stdio.h>

typedef struct copyright
{
	char message[50];
	int endoffile;
} COPYRIGHT;
#define copyrightmessage {"MultiTest (c) 1992 Sivananda Electronics, Nashik"\
							 , 26}

#define putcopyright(fp) if (fp != NULL) \
							 { COPYRIGHT cp = copyrightmessage; \
							 fwrite (&cp, sizeof (COPYRIGHT), 1, fp); }
#define eatcopyright(fp) fseek (fp, sizeof (COPYRIGHT), SEEK_CUR)

#endif