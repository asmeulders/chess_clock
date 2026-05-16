// System Imports
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

// Project Imports
#include "game.h"
#include "clock.h"
#include "time_controls.h"
#include "test.h"

void user_begin();
TimeControls *input_time_controls();
int get_int(const char *promt);

int main()
{
    // ncurses setup
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    user_begin();
    return 0;
}

void user_begin() {
    printw("Enter 's' to begin, 'q' to quit: \n");
    refresh();
    bool listening = true;
    while (listening) {
        int c = getch();
        switch (c)
        {
        case 's':
            TimeControls *tc = input_time_controls();
            Clock *cl = create_clock(tc);
            Game *g = create_game(cl);
            listening = false;
            start_game_loop(g);
            break;
        
        case 'q':
            printw("Quitting\n");
            refresh();
            listening = false;
            break;

        default:
            printf("Unknown command\n");
            refresh();
            break;
        }
    }
}

TimeControls *input_time_controls() { // TODO: validate user input (no negatives, etc)
    int minutes = get_int("Enter minutes per player: ");
    int seconds = get_int("Enter seconds added per turn: ");
    TimeControls *tc = make_time_controls(minutes, seconds);
    return tc;
}

int get_int(const char *prompt) {
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