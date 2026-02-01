#include <stdio.h>
#include <conio.h>
#include "key.h"


#define isfunctionkey(c) (((c)>=0x3b && (c)<=0x44) || ((c)>=0x54 && (c)<=0x5d) || ((c)>=0x5e && (c)<=0x67) || ((c)>=0x68 && (c)<=0x71) || ((c)>=0x47 && (c)<=0x53) || ((c)>=0x73 && (c)<=0x77)    )
											

int getkey (void)
{
	int c;
	if (kbhit())
		if ((c=getch ()) == 0)
		{
			c=getch ();
			if (isfunctionkey (c))
				return c+FUNCTIONKEY;
			else
				return c+SPECIALKEY;
		}
		else
			return c;
	else
		return 0;
}
	
