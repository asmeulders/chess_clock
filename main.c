// System Imports
#include <stdio.h>
#include <stdbool.h>

// Project Imports
#include "game.h"
#include "test.h"
#include "time_controls.h"

void user_begin();
TimeControls *input_time_controls();

int main()
{
    user_begin();
    return 0;
}

void user_begin() {
    printf("Enter 's' to begin, 'q' to quit: ");

    char input[10]; // figure out what i want to do to collect duration info

    bool listening = true;
    while (listening) {
        char c;
        scanf("%s", input);
        
        if (input[0] == 'q') {
            listening = false;
        } else {
            printf("Unknown command\n");
        }
        switch (c)
        {
        case 's':
            TimeControls *tc = input_time_controls();
            Game *g = create_game(tc);
            start_game_loop(g);
            break;
        
        }
    }
}

TimeControls *input_time_controls() {
    // validate user input (no negatives, etc)
    printf("Input minutes per player: ");
    int minutes;
    scanf("%d", &minutes);
    printf("Input seconds added per turn: ");
    int seconds;
    scanf("%d", &seconds);

    TimeControls *tc = make_time_controls(minutes, seconds);
    return tc;
}