#ifndef PLAYERS
#define PLAYERS

#include <stdbool.h>
#include <time.h>

typedef struct Player Player;

Player* create_player(int game_duration); // in seconds
void destroy_player(Player *p);
bool is_active(Player *p);
void set_active(Player *p, bool is_active);
bool is_eliminated(Player *p);



#endif