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
void initialize_players(Game *g);
void start_game_loop(Game *g);
void start_game(Game *g);
void pause_game(Game *g);
void game_over(Game *g);
void reset_game(Game *g);
Player *get_player(Game *g, int id);
Player *get_active_player(Game *g);
int get_duration(Game *g);
void end_turn(Game *g);
void flush_buffer();
void set_raw_mode();
void restore_terminal();

#endif