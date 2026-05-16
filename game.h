#ifndef GAME
#define GAME

// System Imports

// Project Imports
#include "clock.h"

typedef struct Game Game;

Game *create_game(Clock *cl);
void destroy_game(Game *g);
void start_game_loop(Game *g);

#endif