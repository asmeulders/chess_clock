#include <ncurses.h>

void unknown_command(int *unknown_count) {
    (*unknown_count)++;
    int y, x;
    getyx(stdscr, y, x);
    wmove(stdscr, y, 0);
    wclrtoeol(stdscr);
    printw("Unknown command");
    if (*unknown_count >= 2) printw(" (x%d)", *unknown_count);
    refresh();
}