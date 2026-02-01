#pragma once

#ifdef __linux__

// Always present in the TVision port you're using.
#include <tvision/compat/borland/io.h>

// Optional: some TVision trees include these, some don't.
#if defined(__has_include)
  #if __has_include(<tvision/compat/borland/dos.h>)
    #include <tvision/compat/borland/dos.h>
  #endif

  #if __has_include(<tvision/compat/borland/bios.h>)
    #include <tvision/compat/borland/bios.h>
  #elif __has_include(<tvision/compat/borland/bios.hpp>)
    #include <tvision/compat/borland/bios.hpp>
  #endif
#endif

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>

// ------------------------------------------------------------
// Console (Turbo C style)
// ------------------------------------------------------------
#ifndef cputs
#define cputs(msg) fputs((msg), stdout)
#endif

static inline int getche_compat()
{
    int c = getchar();
    if (c != EOF) putchar(c);
    return c;
}
#ifndef getche
#define getche() getche_compat()
#endif

// ------------------------------------------------------------
// Random (Turbo C style)
// ------------------------------------------------------------
static inline void randomize()
{
    srandom((unsigned)time(nullptr));
}

// Turbo C's random(n) returns 0..n-1
//static inline long b_random(long n)
//{
 //   return (n <= 0) ? 0 : (::random() % n);
//}

// ------------------------------------------------------------
// Low level I/O names (some code uses _read/_write)
// ------------------------------------------------------------
#ifndef _read
#define _read  read
#endif
#ifndef _write
#define _write write
#endif

// ------------------------------------------------------------
// Some DOS code references _DI
// ------------------------------------------------------------
#ifndef _DI
#define _DI 0
#endif

// ------------------------------------------------------------
// File time (Borland) - only define if not provided by TVision.
// io.h in your tree already defines struct ftime, so DO NOT
// redefine it here.
// ------------------------------------------------------------
#ifndef HAVE_BORLAND_GETFTIME
static inline int getftime(int /*fd*/, struct ftime *ft)
{
    // Stub: return "success" with zeroed timestamp.
    if (ft) std::memset(ft, 0, sizeof(*ft));
    return 0;
}
#endif

#ifndef HAVE_BORLAND_SETFTIME
static inline int setftime(int /*fd*/, const struct ftime * /*ft*/)
{
    // Stub: accept but do nothing.
    return 0;
}
#endif

// ------------------------------------------------------------
// DOS disk / BIOS stuff.
// If your TVision tree doesn't provide bios.h/dos.h with these,
// we provide stubs so the code compiles on Linux.
// ------------------------------------------------------------
#ifndef _DISK_READ
#define _DISK_READ 0
#endif

// If diskinfo_t is not defined by included headers, stub it.
#ifndef HAVE_BORLAND_DISKINFO_T
// Many old DOS sources expect a *struct* diskinfo_t with fields.
// Provide a compatible struct so code like dinfo.drive works.
struct diskinfo_t {
    int drive;
    int head;
    int track;
    int sector;
    int nsectors;
    void *buffer;
};
#endif

#ifndef HAVE_BORLAND_BIOS_DISK
static inline int _bios_disk(int /*cmd*/, diskinfo_t* /*info*/)
{
    return 0; // stub
}
#endif

// fatinfo + getfat + absread/abswrite
#ifndef HAVE_BORLAND_FATINFO
struct fatinfo {
    unsigned fi_sclus;
    unsigned fi_nclus;
    unsigned fi_bysec;
    unsigned fi_fatid;
};
#endif

#ifndef HAVE_BORLAND_GETFAT
static inline int getfat(int /*drive*/, fatinfo* /*fi*/)
{
    return 0; // stub
}
#endif

#ifndef HAVE_BORLAND_ABSREAD
static inline int absread(int /*drive*/, int /*nsects*/, int /*sect*/, void* /*buf*/)
{
    return 0; // stub
}
#endif

#ifndef HAVE_BORLAND_ABSWRITE
static inline int abswrite(int /*drive*/, int /*nsects*/, int /*sect*/, void* /*buf*/)
{
    return 0; // stub
}
#endif

// ------------------------------------------------------------
// DOS hard error handling stubs
// ------------------------------------------------------------
#ifndef harderr
static inline void harderr(...) {}
#endif
#ifndef hardretn
static inline void hardretn(int) {}
#endif
#ifndef hardresume
static inline void hardresume(int) {}
#endif

// ------------------------------------------------------------
// Borland code sometimes uses mkdir(path) with 1 arg.
// On Linux mkdir requires 2 args.
// Provide a compatibility wrapper when code calls mkdir(path).
// ------------------------------------------------------------
static inline int mkdir_compat(const char *path)
{
    return ::mkdir(path, 0777);
}

// If your code literally calls mkdir(path) (1 arg), you can
// enable this macro to redirect it:
// #define mkdir(path) mkdir_compat(path)

// ------------------------------------------------------------
// Port I/O (Borland) - either real Linux port I/O or stubs
// ------------------------------------------------------------
#ifdef BU_ENABLE_REAL_PORTIO
  #include <sys/io.h>
  static inline void outportb(unsigned short port, unsigned char val)
  {
      outb(val, port);
  }
  static inline unsigned char inportb(unsigned short port)
  {
      return inb(port);
  }
#else
  // Stubbed: compile-only (hardware I/O disabled)
  static inline void outportb(unsigned short /*port*/, unsigned char /*val*/) {}
  static inline unsigned char inportb(unsigned short /*port*/) { return 0; }
#endif

// ------------------------------------------------------------
// Far-memory helpers (DOS). On Linux, normal memset.
// ------------------------------------------------------------
#ifndef _fmemset
  #define _fmemset(ptr, val, sz) std::memset((ptr), (val), (sz))
#endif





#endif // __linux__

// ----- DOS interrupt / far pointer compatibility (Linux stubs) -----
#ifndef __MSDOS__
struct WORDREGS { unsigned short ax, bx, cx, dx, si, di, cflag, flags; };
struct BYTEREGS { unsigned char al, ah, bl, bh, cl, ch, dl, dh; };
union  REGS     { WORDREGS x; BYTEREGS h; };
struct SREGS    { unsigned short es, cs, ss, ds; };

#ifndef FP_OFF
#define FP_OFF(p) (0)
#endif
#ifndef FP_SEG
#define FP_SEG(p) (0)
#endif

static inline int int86(int, REGS*, REGS*) { return 0; }
static inline int int86x(int, REGS*, REGS*, SREGS*) { return 0; }
#endif
