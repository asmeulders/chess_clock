#ifndef CLOCK
#define CLOCK

// System Imports
#include <time.h>

// Project Imports
#include "time_controls.h"

typedef struct Clock Clock;

Clock *create_clock(TimeControls *tc);
void destroy_clock(Clock *c);
void start_clock(Clock *c);
void stop_clock(Clock *c);
void clear_clock(Clock *c);
void set_game_start(Clock *c, struct timespec *time);
void set_turn_start(Clock *c, struct timespec *time);
struct timespec get_game_start(Clock *c);
struct timespec get_turn_start(Clock *c);
TimeControls *get_time_controls(Clock *c);

#endif