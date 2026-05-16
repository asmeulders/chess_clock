#ifndef CLOCK
#define CLOCK

// System Imports
#include <time.h>

// Project Imports
#include "time_controls.h"

typedef struct Clock Clock;

Clock *create_clock(TimeControls *tc);
void destroy_clock(Clock *c);
struct timespec get_turn_end(Clock *c);
TimeControls *get_time_controls(Clock *c);
void calculate_turn_end(Clock *c, struct timespec time_remaining);

#endif