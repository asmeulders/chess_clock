#define _POSIX_C_SOURCE 199309L
// System Imports
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Project Imports
#include "clock.h"
#include "time_controls.h"

struct Clock {
    struct timespec game_start;
    struct timespec turn_start;
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

void start_clock(Clock *c) {
    set_game_start(c, NULL);
    set_turn_start(c, NULL);
}

void stop_clock(Clock *c) {
    // stops clock for one player
}

void clear_clock(Clock *c) {
    printf("Clear Clock");
    // reset everything to 0
    struct timespec zero = { 0, 0 };
    c->game_start = zero;
    c->turn_start = zero;
}

void set_game_start(Clock *c, struct timespec *time) {
    if (time != NULL) {
        c->game_start = *time;
    } else {
        clock_gettime(CLOCK_MONOTONIC, &c->game_start);
    }
}

void set_turn_start(Clock *c, struct timespec *time) {
    if (time != NULL) {
        c->turn_start = *time;
    } else {
        clock_gettime(CLOCK_MONOTONIC, &c->turn_start);
    }
}

struct timespec get_game_start(Clock *c) {
    return c->game_start;
}

struct timespec get_turn_start(Clock *c) {
    return c->turn_start;
}

TimeControls *get_time_controls(Clock *c) {
    return c->tc;
}