#ifndef GAME
#define GAME

#include "players.h"

typedef struct Game Game;

Game* create_game(int duration);
void destroy_game(Game *g);
void start_game(Game *g);
void stop_game(Game *g);
Player* get_player(Game *g, int id);
int get_duration(Game *g);
void end_turn(Game *g);

#endif