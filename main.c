// System Imports
#include <stdio.h>
#include <stdbool.h>

// Project Imports
#include "game.h"
#include "test.h"

void user_begin();
void input_time_controls();

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
            Game *g = create_game(duration);
            start_game_loop(g);
            break;
        
        }
    }
}

void input_time_controls() {
    
}