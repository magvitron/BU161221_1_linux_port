#pragma once

#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <cstdio>

static inline void delay(unsigned ms) {
    usleep((useconds_t)ms * 1000);
}

static inline void sound(unsigned /*freq*/) {
    // crude beep
    std::printf("\a");
    std::fflush(stdout);
}

static inline void nosound(void) {
    // no-op
}

static inline int kbhit(void) {
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    timeval tv{0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ret > 0;
}

static inline int getch(void) {
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    unsigned char c = 0;
    if (read(STDIN_FILENO, &c, 1) < 0) c = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return (int)c;
}

static inline void clrscr(void) {
    std::printf("\033[2J\033[H");
    std::fflush(stdout);
}

static inline void gotoxy(int x, int y) {
    std::printf("\033[%d;%dH", y, x);
    std::fflush(stdout);
}
