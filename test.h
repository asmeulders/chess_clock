#ifndef TEST
#define TEST

// System imports
#include <stdio.h>

// Project imports

TimeControls *make_mtc();
Clock *make_mcl();
void basic_test();
void basic_game_test();
void change_turns_test();
void change_turns_not_started_test();
void start_clock_test();
void clear_clock_test();
void set_game_start_test();
void set_turn_start_test();

#endif