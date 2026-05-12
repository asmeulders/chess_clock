#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#include "players.h"

struct Player {
  bool is_active;
  time_t time_remaining;
};

Player* create_player(int game_duration) {
  time_t duration = ;
  Player *p = {false, duration};
}

bool is_active(Player *p) {
  return p->is_active;
}

bool is_eliminated(Player *p) {
  return p->time_remaining <= 0;
}

void change_turns(Player *p1, Player *p2) {

}