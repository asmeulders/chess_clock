// System Imports
#include <stdlib.h>

// Project Imports
#include "time_controls.h"

struct TimeControls {
    int minutes;
    int seconds;
};

TimeControls *make_time_controls(int minutes, int seconds) {
    TimeControls *tc = malloc(sizeof(TimeControls));
    tc->minutes = minutes;
    tc->seconds = seconds;
    return tc;
}

void destroy_time_controls(TimeControls *tc) {
    free(tc);
}

int get_minutes(TimeControls *tc) {
    return tc->minutes;
}

int get_seconds(TimeControls *tc) {
    return tc->seconds;
}