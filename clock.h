#ifndef CLOCK
#define CLOCK

#include <time.h>

typedef struct Clock Clock;

Clock *make_clock();
void destroy_clock(Clock *c);
void start_clock(Clock *c);
void stop_clock(Clock *c);
void clear_clock(Clock *c);


#endif