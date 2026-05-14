// System Imports
#include <stdio.h>
#include <stdbool.h>

// Project Imports
#include "game.h"
#include "clock.h"
#include "time_controls.h"
#include "test.h"

void user_begin();
TimeControls *input_time_controls();

int main()
{
    user_begin();
    return 0;
}

void user_begin() {
    printf("Enter 's' to begin, 'q' to quit: ");
    bool listening = true;
    while (listening) {
        char c;
        scanf(" %c", &c);
        switch (c)
        {
        case 's':
            printf("Starting\n");
            TimeControls *tc = input_time_controls();
            Clock *cl = create_clock(tc);
            Game *g = create_game(cl);
            // fflush(stdout);
            start_game_loop(g);
            break;
        
        case 'q':
            printf("Quitting\n");
            listening = false;
            break;

        default:
            printf("Unknown command\n");
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