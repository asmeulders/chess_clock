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
    printf("Create clock\n");
    // fflush(stdout);
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

void calculate_turn_end(Clock *c, struct timespec time_remaining) {
    clock_gettime(CLOCK_MONOTONIC, &c->turn_end);
    c->turn_end.tv_sec += time_remaining.tv_sec;
    c->turn_end.tv_nsec += time_remaining.tv_nsec;
}