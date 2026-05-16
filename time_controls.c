// System Imports
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

// Project Imports
#include "time_controls.h"

// ************************************************************************************
// ----- Static Declarations ----------------------------------------------------------
// ************************************************************************************

static int get_int(const char *promt);

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

TimeControls *input_time_controls() { // TODO: validate user input (no negatives, etc)
    int minutes = get_int("Enter minutes per player: ");
    int seconds = get_int("Enter seconds added per turn: ");
    TimeControls *tc = make_time_controls(minutes, seconds);
    return tc;
}

static int get_int(const char *prompt) {
    char buf[32];
    int n = 0;

    printw("%s", prompt);
    refresh();

    echo();
    curs_set(1);
    wgetnstr(stdscr, buf, sizeof(buf) - 1);
    noecho();
    curs_set(0);

    if (strlen(buf) > 0)
        n = atoi(buf);
    return n;
}