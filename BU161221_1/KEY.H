#if !defined( __KEY_H )
#define __KEY_H

#define FUNCTIONKEY 0x100
#define SPECIALKEY  0X200

#define F1  (0x3b + FUNCTIONKEY)
#define F2  (0x3c + FUNCTIONKEY)
#define F3  (0x3d + FUNCTIONKEY)
#define F4  (0x3e + FUNCTIONKEY)
#define F5  (0x3f + FUNCTIONKEY)
#define F6  (0x40 + FUNCTIONKEY)
#define F7  (0x41 + FUNCTIONKEY)
#define F8  (0x42 + FUNCTIONKEY)
#define F9  (0x43 + FUNCTIONKEY)
#define F10 (0x44 + FUNCTIONKEY)

#define SHIFTF1  (0x54 + FUNCTIONKEY)
#define SHIFTF2  (0x55 + FUNCTIONKEY)
#define SHIFTF3  (0x56 + FUNCTIONKEY)
#define SHIFTF4  (0x57 + FUNCTIONKEY)
#define SHIFTF5  (0x58 + FUNCTIONKEY)
#define SHIFTF6  (0x59 + FUNCTIONKEY)
#define SHIFTF7  (0x5a + FUNCTIONKEY)
#define SHIFTF8  (0x5b + FUNCTIONKEY)
#define SHIFTF9  (0x5c + FUNCTIONKEY)
#define SHIFTF10 (0x5d + FUNCTIONKEY)

#define CONTROLF1  (0x5e + FUNCTIONKEY)
#define CONTROLF2  (0x5f + FUNCTIONKEY)
#define CONTROLF3  (0x60 + FUNCTIONKEY)
#define CONTROLF4  (0x61 + FUNCTIONKEY)
#define CONTROLF5  (0x62 + FUNCTIONKEY)
#define CONTROLF6  (0x63 + FUNCTIONKEY)
#define CONTROLF7  (0x64 + FUNCTIONKEY)
#define CONTROLF8  (0x65 + FUNCTIONKEY)
#define CONTROLF9  (0x66 + FUNCTIONKEY)
#define CONTROLF10 (0x67 + FUNCTIONKEY)

#define ALTF1  (0x68 + FUNCTIONKEY)
#define ALTF2  (0x69 + FUNCTIONKEY)
#define ALTF3  (0x6a + FUNCTIONKEY)
#define ALTF4  (0x6b + FUNCTIONKEY)
#define ALTF5  (0x6c + FUNCTIONKEY)
#define ALTF6  (0x6d + FUNCTIONKEY)
#define ALTF7  (0x6e + FUNCTIONKEY)
#define ALTF8  (0x6f + FUNCTIONKEY)
#define ALTF9  (0x70 + FUNCTIONKEY)
#define ALTF10 (0x71 + FUNCTIONKEY)

#define HOME       (0x47 + FUNCTIONKEY)
#define UPARROW    (0x48 + FUNCTIONKEY)
#define PAGEUP     (0x49 + FUNCTIONKEY)
#define LEFTARROW  (0x4b + FUNCTIONKEY)
#define RIGHTARROW (0x4d + FUNCTIONKEY)
#define END        (0x4f + FUNCTIONKEY)
#define DOWNARROW  (0x50 + FUNCTIONKEY)
#define PAGEDOWN   (0x51 + FUNCTIONKEY)
#define INSERT     (0x52 + FUNCTIONKEY)
#define DELETE     (0x53 + FUNCTIONKEY)

#define CONTROLRIGHTARROW (0x73 + FUNCTIONKEY)
#define CONTROLLEFTARROW  (0x74 + FUNCTIONKEY)
#define CONTROLEND        (0x75 + FUNCTIONKEY)
#define CONTROLPAGEDOWN   (0x76 + FUNCTIONKEY)
#define CONTROLHOME       (0x77 + FUNCTIONKEY)

#define CEE ('C'+ SPECIALKEY)
#define cee ('c'+ SPECIALKEY)
#define DEE ('D'+ SPECIALKEY)
#define dee ('d'+ SPECIALKEY)
#define CEE ('C'+ SPECIALKEY)
#define cee ('c'+ SPECIALKEY)
#define EE  ('E'+ SPECIALKEY)
#define ee  ('e'+ SPECIALKEY)
#define EFF ('F'+ SPECIALKEY)
#define eff ('f'+ SPECIALKEY)
#define ARR ('R'+ SPECIALKEY)
#define ELL ('L'+ SPECIALKEY)
#define ell ('l'+ SPECIALKEY)
#define ENN ('N'+ SPECIALKEY)
#define enn ('n'+ SPECIALKEY)
#define arr ('r'+ SPECIALKEY)
#define ASS ('S'+ SPECIALKEY)
#define ass ('s'+ SPECIALKEY)
#define TEE ('T'+ SPECIALKEY)
#define tee ('t'+ SPECIALKEY)

#define ESCAPE 0x1b
#define BACKSPACE 0x8
#define ENTER '\r'
#define TAB			0x9
#define SPACEBAR	0x20
#define LFTARR	 	0x14b
#define RGTARR      0x14d
#define UPARR		0x148
#define DNARR		0x150
#define PGUP		0x49
#define PGDN        0x51


int getkey (void);

#endif