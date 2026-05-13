// System Imports
#include <stdio.h>

// Project Imports
#include "players.h"

typedef struct {
    Player *p1;
    Player *p2;
} Game;

Game* create_game(int duration) {
    Player *p1 = create_player(duration);
    Player *p2 = create_player(duration);

    Game *g = malloc(sizeof(Game));
    g->p1 = p1;
    g->p2 = p2;
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