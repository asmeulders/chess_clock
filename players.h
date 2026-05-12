#ifndef PLAYERS
#define PLAYERS

#include <stdbool.h>
#include <time.h>

typedef struct Player Player;

Player* create_player(int game_duration); // in seconds

bool is_active(Player *p);
bool is_eliminated(Player *p);
void change_turns(Player *p1, Player *p2);


#endif