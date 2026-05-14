#ifndef TIME_CONTROLS
#define TIME_CONTROLS

typedef struct TimeControls TimeControls;

TimeControls *make_time_controls();
void destroy_time_controls(TimeControls *tc);

#endif