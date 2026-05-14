#ifndef TIME_CONTROLS
#define TIME_CONTROLS

typedef struct TimeControls TimeControls;

TimeControls *make_time_controls(int minutes, int seconds);
void destroy_time_controls(TimeControls *tc);
int get_minutes(TimeControls *tc);
int get_seconds(TimeControls *tc);

#endif