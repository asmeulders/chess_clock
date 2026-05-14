// System Imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// Project Imports
#include "players.h"
#include "game.h"
#include "clock.h"

struct Game {
    Clock *c;
    Player *p1;
    Player *p2;
    bool in_progress;
    int active_pid;
};

Game *create_game(TimeControls *tc) {
    Clock *c = create_clock();
    Player *p1 = create_player(get_minutes(tc) * 60); // seconds
    Player *p2 = create_player(get_minutes(tc) * 60);

    Game *g = malloc(sizeof(Game));
    g->p1 = p1;
    g->p2 = p2;
    g->in_progress = false;
    g->active_pid = 1;
}

void destroy_game(Game *g) {
    if (g == NULL) return;
    destroy_clock(g->c);
    destroy_player(g->p1);
    destroy_player(g->p2);
    free(g);
}

void start_game_loop(Game *g) {    
    while (g->in_progress) {
        char c;
        printf("Enter a character: ");
        getchar();
        switch (c)
        {
        case 's': // start
            /* code */
            break;

        case 'p': // pause
            /* code */
            break;

        case 'e': // end turn
            /* code */
            break;

        case 'c': // clear
            /* code */
            break;

        default:
            // count down
            printf("Default case");
            // sleep
            usleep(1000000); // sleep for one second for now to test
            // print time
            break;
        }
    }
        
    

    printf("Input: %c\n", c);
    printf("End of program\n");
}

void start_game(Game *g) {
    // activate player 1
    set_active(g->p1, true);
    // start clock
    
}

void stop_game(Game *g) {
    // deactivate both players
    set_active(g->p1, false);
    set_active(g->p2, false);
    // stops clock
}

Player *get_player(Game *g, int id) {
    Player *p = NULL;

    switch (id)
    {
    case 1:
        p = g->p1;
        break;
    
    case 2:
        p = g->p2;
        break;
    default:
        printf("Invalid id: %d\n", id);
        break;
    }

    return p;
}

int get_duration(Game *g) {
    return get_minutes(get_time_controls(g->c));
}

void end_turn(Game *g) {
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    if (is_active(p1) != is_active(p2)) {
        set_active(p1, !is_active(p1));
        set_active(p2, !is_active(p2));
    } else {
        printf("Game has not started yet.\n");
    }
}