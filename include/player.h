#ifndef PLAYERS
#define PLAYERS

// System Imports
#include <stdbool.h>
#include <time.h>

// Project Imports
#include "game.h"

Player* create_player(int game_duration, int id, char *name); // in seconds
void destroy_player(Player *p);
bool is_active(Player *p);
void set_active(Player *p, bool is_active);
bool is_eliminated(Player *p);
struct timespec get_time_remaining(Player *p);
void set_time_remaining(Player *p, struct timespec t);
char *get_name(Player *p);
void customize_player_names(Game *g);


#endif