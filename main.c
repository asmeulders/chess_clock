// System Imports
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>

// Project Imports
#include "game.h"
#include "clock.h"
#include "time_controls.h"
#include "test.h"

/* TODO:
- input validation for time
- input player names
- better print statements
- make unknown command a separate function
*/

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
    endwin();
    return 0;
}

void user_begin() {
    printw("Enter 's' to begin, 'q' to quit\n");
    refresh();
    bool listening = true;
    int unknown_count = 0;
    while (listening) {
        int c = getch();
        switch (c)
        {
        case 's':
            TimeControls *tc = input_time_controls();
            if (tc == NULL) {
                listening = false;
                break;
            }
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
            unknown_count++;
            printw("\rUnknown command");
            if (unknown_count >=2 ) printw(" (x%d)", unknown_count);
            refresh();
            break;
        }
    }
}