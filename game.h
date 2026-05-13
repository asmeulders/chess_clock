#ifndef GAME
#define GAME

#include "players.h"

typedef struct Game Game;

Game* create_game(int duration);
void destroy_game(Game *g);
void start_game();
void end_game();
Player* get_player(Game *g, int id);
int get_duration(Game *g);

#endif