#ifndef PLAYERS
#define PLAYERS

// System Imports
#include <stdbool.h>
#include <time.h>

// Project Imports

typedef struct Player Player;

Player* create_player(int game_duration); // in seconds
void destroy_player(Player *p);
bool is_active(Player *p);
void set_active(Player *p, bool is_active);
bool is_eliminated(Player *p);
struct timespec get_time_remaining(Player *p);
void set_time_remaining(Player *p, struct timespec t);


#endif