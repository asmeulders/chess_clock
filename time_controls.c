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

static int get_int(const char *promt, int *out);
static int select_bullet(int *minutes, int *seconds);
static int select_blitz(int *minutes, int *seconds);
static int select_rapid(int *minutes, int *seconds);
static int input_custom(int *minutes, int *seconds);

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
    int input;
    int listening = 1;
    int needs_redraw = 1;
    int unknown_count = 0;
    int minutes, seconds;
    while (listening) {
        if (needs_redraw) {
            wmove(stdscr, 0, 0);
            wclrtobot(stdscr);
            printw("Please choose a game type!\nBullet (b) -> 1|0, 2|1\nBlitz  (z) -> 3|0, 3|2, 5|0, 5|5\nRapid  (r) -> 10|0, 10|5, 15|10, 20|0, 30|0, 60|0\nCustom (c) -> Enter your own values X|X\n");
            refresh();
            needs_redraw = 0;
        }
        input = getch();
        switch (input) {
            case 'b': listening = select_bullet(&minutes, &seconds); needs_redraw = 1; break;
            case 'z': listening = select_blitz(&minutes, &seconds); needs_redraw = 1; break;
            case 'r': listening = select_rapid(&minutes, &seconds); needs_redraw = 1; break;
            case 'c': listening = input_custom(&minutes, &seconds); needs_redraw = 1; break;
            case 'q': return NULL;
            default:
                unknown_command(&unknown_count);
                refresh();
                break;
        }
    }
    TimeControls *tc = make_time_controls(minutes, seconds);
    return tc;
}

static int select_bullet(int *minutes, int *seconds) {
    int listening = 1;
    int unknown_count = 0;
    int input;
    int y, x;
    clear();
    printw("Select Time Controls: Bullet\n1. 1|0\n2. 2|1");
    refresh();
    getyx(stdscr, y, x);
    while (listening) {
        input = getch();
        switch (input) {
            case '1': *minutes = 1; *seconds = 0; listening = 0; break;
            case '2': *minutes = 2; *seconds = 1; listening = 0; break; 
            case 'b': wclrtobot(stdscr); return EXIT_FAILURE;
            default: 
                wmove(stdscr, y+1, 0); 
                unknown_command(&unknown_count); 
                wmove(stdscr, y, x); 
                wclrtoeol(stdscr); 
                break;
        }
    }
    return EXIT_SUCCESS;
}

static int select_blitz(int *minutes, int *seconds) {
    int listening = 1;
    int unknown_count = 0;
    int input;
    int y, x;
    clear();
    printw("Select Time Controls: Blitz\n1. 3|0\n2. 3|2\n3. 5|0\n4. 5|5");
    refresh();
    getyx(stdscr, y, x);
    while (listening) {
        input = getch();
        switch (input) {
            case '1': *minutes = 3; *seconds = 0; listening = 0; break;
            case '2': *minutes = 3; *seconds = 2; listening = 0; break; 
            case '3': *minutes = 5; *seconds = 0; listening = 0; break;
            case '4': *minutes = 5; *seconds = 5; listening = 0; break; 
            case 'b': return EXIT_FAILURE;
            default: 
                wmove(stdscr, y+1, 0); 
                unknown_command(&unknown_count); 
                wmove(stdscr, y, x); 
                wclrtoeol(stdscr); break;
        }
    }
    return EXIT_SUCCESS;
}

static int select_rapid(int *minutes, int *seconds) {
    int listening = 1;
    int unknown_count = 0;
    int input;
    int y, x;
    clear();
    printw("Select Time Controls: Rapid\n1. 10|0\n2. 10|5\n3. 15|10\n4. 20|0\n5. 30|0\n6. 60|0");
    refresh();
    getyx(stdscr, y, x);
    while (listening) {
        input = getch();
        switch (input) {
            case '1': *minutes = 10; *seconds = 0; listening = 0; break;
            case '2': *minutes = 10; *seconds = 5; listening = 0; break; 
            case '3': *minutes = 15; *seconds = 10; listening = 0; break;
            case '4': *minutes = 20; *seconds = 0; listening = 0; break; 
            case '5': *minutes = 30; *seconds = 0; listening = 0; break;
            case '6': *minutes = 60; *seconds = 0; listening = 0; break;
            case 'b': return EXIT_FAILURE;
            default: 
                wmove(stdscr, y+1, 0); 
                unknown_command(&unknown_count); 
                wmove(stdscr, y, x); 
                wclrtoeol(stdscr); 
                break;
        }
    }
    return EXIT_SUCCESS;
}

static int input_custom(int *minutes, int *seconds) {
    clear();
    printw("Enter custom time controls...\n");
    refresh();

    // minutes
    while (1) {
        wmove(stdscr, 1, 0);
        wclrtoeol(stdscr);

        if (get_int("Enter minutes per player: ", minutes)) 
            return EXIT_FAILURE;

        if (*minutes > 0) 
            break;

        wmove(stdscr, 2, 0);
        printw("Must be positive.");
        wmove(stdscr, 1, 0);
        refresh();
    }

    // seconds
    wmove(stdscr, 2, 0);
    wclrtoeol(stdscr);
    while (1) {
        wmove(stdscr, 2, 0);
        wclrtoeol(stdscr);

        if (get_int("Enter seconds added per turn: ", seconds)) 
            return EXIT_FAILURE;

        if (*seconds >= 0) 
            break;

        wmove(stdscr, 3, 0);
        printw("Must be positive.");
        wmove(stdscr, 2, 0);
        refresh();
    }
    return EXIT_SUCCESS;
}

static int get_int(const char *prompt, int *out) {
    char buf[32];
    printw("%s", prompt);
    refresh();

    echo();
    curs_set(1);
    wgetnstr(stdscr, buf, sizeof(buf) - 1);
    noecho();
    curs_set(0);

    if (strcmp(buf, "q") == 0 || strcmp(buf, "Q") == 0 || strcmp(buf, "quit") == 0 || strcmp(buf, "Quit") == 0) {
        return 1;
    }

    if (strlen(buf) > 0)
        *out = atoi(buf);

    return 0;
}