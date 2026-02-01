#pragma once
#include <ctime>

#define __BORLAND_DOS_TIME_DEFINED__

struct date {
    int da_year;
    int da_mon;
    int da_day;
};
#if 0
struct time {
    int ti_hour;
    int ti_min;
    int ti_sec;
    int ti_hund;
};

#endif
static inline void getdate(struct date *d)
{
    if (!d) return;
    std::time_t t = std::time(nullptr);
    std::tm lt{};
    localtime_r(&t, &lt);
    d->da_year = lt.tm_year + 1900;
    d->da_mon  = lt.tm_mon + 1;
    d->da_day  = lt.tm_mday;
}
