#ifndef GAME
#define GAME

// System Imports

// Project Imports
#include "clock.h"

typedef struct Game Game;
typedef struct Player Player;

Game *create_game(Clock *cl);
void destroy_game(Game *g);
void start_game_loop(Game *g);
Player *get_player(Game *g, int id);

#endif