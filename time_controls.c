// System Imports
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>

// Project Imports
#include "time_controls.h"
#include "utils.h"

// ************************************************************************************
// ----- Static Declarations ----------------------------------------------------------
// ************************************************************************************

static int get_int(const char *promt);
static void select_bullet(int *minutes, int *seconds);
static void select_blitz(int *minutes, int *seconds);
static void select_rapid(int *minutes, int *seconds);
static void input_custom(int *minutes, int *seconds);

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

TimeControls *input_time_controls() {
    clear();
    refresh();
    printw("Please choose a game type!\nBullet (b) -> 1|0, 2|1\nBlitz (z) -> 3|0, 3|2, 5|0, 5|5\nRapid (r) -> 10|0, 10|5, 15|10, 20|0, 30|0, 60|0\nCustom (c) -> Enter your own values X|X\nInput: ");
    refresh();
    char response[8];
    int listening = 1;
    int unknown_count = 0;
    int minutes;
    int seconds;

    echo();
    curs_set(1);
    while (listening) {
        wgetnstr(stdscr, response, sizeof(response));
        for (int i = 0; response[i]; i++) 
            response[i] = tolower(response[i]);

        if (strcmp(response, "bullet") == 0 || strcmp(response, "b") == 0) {
            select_bullet(&minutes, &seconds);
            listening = false;
        } else if (strcmp(response, "blitz") == 0 || strcmp(response, "z") == 0) {
            select_blitz(&minutes, &seconds);
            listening = false;
        } else if (strcmp(response, "rapid") == 0 || strcmp(response, "r") == 0) {
            select_rapid(&minutes, &seconds);
            listening = false;
        } else if (strcmp(response, "custom") == 0 || strcmp(response, "c") == 0) {
            input_custom(&minutes, &seconds);
            listening = false;
        } else if (strcmp(response, "quit") == 0 || strcmp(response, "q") == 0) {
            printw("Quitting\n");
            refresh();
            listening = false;
            return NULL;
        } else {
            unknown_command(&unknown_count);
        }
    }
    TimeControls *tc = make_time_controls(minutes, seconds);
    return tc;
}

static void select_bullet(int *minutes, int *seconds) {
    int listening = 1;
    int unknown_count = 0;
    int input;
    while (listening) {
        input = get_int("Select Time Controls: Bullet\n1. 1|0\n2. 2|1\nInput: ");
        switch (input) {
            case 1: *minutes = 1; *seconds = 0; listening = 0; break;
            case 2: *minutes = 2; *seconds = 1; listening = 0; break; 
            default: unknown_command(&unknown_count); break;
        }
    }
    
}

static void select_blitz(int *minutes, int *seconds) {
    int listening = 1;
    int unknown_count = 0;
    int input;
    while (listening) {
        input = get_int("Select Time Controls: Blitz\n1. 3|0\n2. 3|2\n3. 5|0\n4. 5|5\nInput: ");
        switch (input) {
            case 1: *minutes = 3; *seconds = 0; listening = 0; break;
            case 2: *minutes = 3; *seconds = 2; listening = 0; break; 
            case 3: *minutes = 5; *seconds = 0; listening = 0; break;
            case 4: *minutes = 5; *seconds = 5; listening = 0; break; 
            default: unknown_command(&unknown_count); break;
        }
    }
}

static void select_rapid(int *minutes, int *seconds) {
    int listening = 1;
    int unknown_count = 0;
    int input;
    while (listening) {
        input = get_int("Select Time Controls: Rapid\n1. 10|0\n2. 10|5\n3. 15|10\n4. 20|0\n5. 30|0\n6. 60|0\nInput: ");
        switch (input) {
            case 1: *minutes = 10; *seconds = 0; listening = 0; break;
            case 2: *minutes = 10; *seconds = 5; listening = 0; break; 
            case 3: *minutes = 15; *seconds = 10; listening = 0; break;
            case 4: *minutes = 20; *seconds = 0; listening = 0; break; 
            case 5: *minutes = 30; *seconds = 0; listening = 0; break;
            case 6: *minutes = 60; *seconds = 0; listening = 0; break;
            default: unknown_command(&unknown_count); break;
        }
    }
}

static void input_custom(int *minutes, int *seconds) {
    int y, x;
    getyx(stdscr, y, x);

    // minutes
    while (1) {
        wmove(stdscr, y, x);
        wclrtoeol(stdscr);
        *minutes = get_int("Enter minutes per player: ");
        if (*minutes > 0) break;
        wmove(stdscr, y+1, 0);
        printw("Must be positive.");
        wmove(stdscr, y, 0);
    }

    // seconds
    wmove(stdscr, y+1, 0);
    wclrtoeol(stdscr);
    while (1) {
        wmove(stdscr, y+1, 0);
        wclrtoeol(stdscr);
        *seconds = get_int("Enter seconds added per turn: ");
        if (*seconds >= 0) break;
        wmove(stdscr, y+2, 0);
        printw("Must be positive.");
        wmove(stdscr, y+1, 0);

    }

    wmove(stdscr, y+2, 0);
    wclrtoeol(stdscr);
    refresh();
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