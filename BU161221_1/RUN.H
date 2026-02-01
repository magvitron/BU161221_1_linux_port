#if !defined(__RUN_H)
#define __RUN_H

#define BOTHFIXTURE 2
#define OUTERFIXTURE 1
#define INNERFIXTURE 0
#define NONEBREAKSHORT 0x0
#define ONLYSHORT	0x1
#define ONLYBREAK	0x2
#define BOTHBREAKSHORT 0x3
#define PARAMSELECTIONMASK 0x40000000l
#define FREQANDCMBNMASK 0x3FFFFFFFl
#define ALLFREQMASK 0x24924924l
#define LFFREQMASK 0x00024924l //0x924l      //Corrected on 03112006:sdt:2220
#define HFFREQMASK 0x24900000l //0x24924000l //Corrected on 03112006:sdt:2220
#define MAXFILEINQUEUE                 	15

#endif