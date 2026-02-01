# BU161221 Linux port (Turbo Vision)

This project is a Borland/Turbo Vision text-UI app. The simplest Linux port is to build it against **magiblot/tvision** (a modern Turbo Vision port).

## Build

```bash
cd BU161221_1_linux_port
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/bu161221
```

## Notes
- The CMake uses FetchContent to clone tvision. You need network access during configure.
- If you get linker errors about ncurses/gpm, install dev packages (names vary by distro):
  - Debian/Ubuntu: `libncursesw5-dev`, `libgpm-dev`
  - Fedora: `ncurses-devel`, `gpm-devel`

## First expected porting fixes
- Any remaining direct BIOS/DOS interrupt usage (`int86`, `inportb`, etc.) must be removed or replaced with POSIX equivalents.
- `farmalloc/farfree` should map to `malloc/free` (tvision compat headers often handle this).
