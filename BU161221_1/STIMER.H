#define timerExpired( timerVariable ) ( timerVariable != 0 )

/***************************************************************************/
unsigned int startTimer( unsigned long timeValue, unsigned char *pdoneFlag );
void cancelTimer( void );
/***************************************************************************/
