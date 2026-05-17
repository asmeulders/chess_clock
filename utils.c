#include <ncurses.h>

void unknown_command(int *unknown_count) {
    (*unknown_count)++;
    printw("\rUnknown command");
    if (*unknown_count >= 2) printw(" (x%d)", *unknown_count);
    refresh();
}