#define _POSIX_C_SOURCE 199309L
// System Imports
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Project Imports
#include "clock.h"

struct Clock {
    struct timespec game_start;
    struct timespec turn_start;
};

Clock *make_clock() {
    Clock *c = malloc(sizeof(Clock));
    return c;
}

void destroy_clock(Clock *c) {
    free(c);
}

void start_clock(Clock *c) {

}

void stop_clock(Clock *c) {
    // stops clock for one player
}

void clear_clock(Clock *c) {
    // reset everything to 0
}