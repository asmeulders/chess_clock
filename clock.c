#define _POSIX_C_SOURCE 199309L
// System Imports
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Project Imports
#include "clock.h"
#include "time_controls.h"

// ************************************************************************************
// ----- Clock Struct -----------------------------------------------------------------
// ************************************************************************************

struct Clock {
    struct timespec turn_end;
    TimeControls *tc;
};

Clock *create_clock(TimeControls *tc) {
    Clock *c = malloc(sizeof(Clock));
    c->tc = tc;
    return c;
}

void destroy_clock(Clock *c) {
    destroy_time_controls(c->tc);
    free(c);
}

// ************************************************************************************
// ----- Clock Functions --------------------------------------------------------------
// ************************************************************************************

struct timespec get_turn_end(Clock *c) {
    return c->turn_end;
}

TimeControls *get_time_controls(Clock *c) {
    return c->tc;
}

/* Updates the clock turn end based on time remaining. */
void calculate_turn_end(Clock *c, struct timespec time_remaining) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    c->turn_end = add_timespec(now, time_remaining);
}


struct timespec add_timespec(struct timespec ts1, struct timespec ts2) {
    struct timespec result;
    long sec = ts1.tv_sec + ts2.tv_sec;
    long nsec = ts1.tv_nsec + ts2.tv_nsec;
    if (nsec > 1000000000) {
        sec++;
        nsec -= 1000000000;
    }
    result.tv_sec = sec; result.tv_nsec = nsec;
    return result;
}

struct timespec subtract_timespec(struct timespec ts1, struct timespec ts2) {
    struct timespec result;
    long sec = ts1.tv_sec - ts2.tv_sec;
    long nsec = ts1.tv_nsec - ts2.tv_nsec;
    if (nsec < 0) {
        sec--;
        nsec += 1000000000;
    }
    result.tv_sec = sec; result.tv_nsec = nsec;
    return result;
}