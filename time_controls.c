// System Imports
#include <stdlib.h>
#include <stdio.h>

// Project Imports
#include "time_controls.h"

// ************************************************************************************
// ----- Time Controls Struct ---------------------------------------------------------
// ************************************************************************************

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

// ************************************************************************************
// ----- Time Controls Functions ------------------------------------------------------
// ************************************************************************************

int get_minutes(TimeControls *tc) {
    return tc->minutes;
}

int get_seconds(TimeControls *tc) {
    return tc->seconds;
}

void set_minutes(TimeControls *tc, int minutes) {
    tc->minutes = minutes;
}

void set_seconds(TimeControls *tc, int seconds) {
    tc->seconds = seconds;
}