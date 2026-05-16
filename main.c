// System Imports
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

// Project Imports
#include "game.h"
#include "clock.h"
#include "time_controls.h"
#include "test.h"

void user_begin();
TimeControls *input_time_controls();

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
    printw("Enter 's' to begin, 'q' to quit: ");
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

TimeControls *input_time_controls() {
    // validate user input (no negatives, etc)
    printf("Input minutes per player: ");
    int minutes;
    scanf(" %d", &minutes);
    printf("Input seconds added per turn: ");
    int seconds;
    scanf(" %d", &seconds);
    printf("Entering make time controls\n");
    // fflush(stdout);
    TimeControls *tc = make_time_controls(minutes, seconds);
    return tc;
}