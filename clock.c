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
    clock_gettime(CLOCK_MONOTONIC, &c->turn_end); // get current time
    c->turn_end.tv_sec += time_remaining.tv_sec;
    c->turn_end.tv_nsec += time_remaining.tv_nsec;
    if (c->turn_end.tv_nsec > 1000000000) {
        c->turn_end.tv_nsec -= 1000000000;
        c->turn_end.tv_sec += 1;
    }
}