#ifndef GAME
#define GAME

// System Imports

// Project Imports
#include "clock.h"
#include "players.h"
#include "time_controls.h"

typedef struct Game Game;

Game *create_game(Clock *cl);
void destroy_game(Game *g);
void start_game_loop(Game *g);
Player *get_player(Game *g, int id);
Player *get_active_player(Game *g);

#endif