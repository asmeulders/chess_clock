// System Imports
#include <stdio.h>
#include <stdlib.h>

// Project Imports
#include "players.h"
#include "game.h"

struct Game {
    Player *p1;
    Player *p2;
    int duration;
};

Game *create_game(int duration) {
    Player *p1 = create_player(duration);
    Player *p2 = create_player(duration);

    Game *g = malloc(sizeof(Game));
    g->p1 = p1;
    g->p2 = p2;
    g->duration = duration;
}

void destroy_game(Game *g) {
    if (g == NULL) return;
    destroy_player(g->p1);
    destroy_player(g->p2);
    free(g);
}

void start_game() {
  // create players
  // find out game length
  // start clock
}

void stop_game() {
  // stops clock
  // both players are not active
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
    return g->duration;
}