#define _POSIX_C_SOURCE 199309L
// System Imports
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Imports
#include "player.h"
#include "game.h"

// ************************************************************************************
// ----- Static Declarations ----------------------------------------------------------
// ************************************************************************************

static void input_player_name(Player *p, int id);
static void set_name(Player *p, const char *name);

// ************************************************************************************
// ----- Player Struct ----------------------------------------------------------------
// ************************************************************************************

struct Player {
    struct timespec time_remaining;
    char name[21];
    bool is_active;
};

Player *create_player(int game_duration, int id, char *name) {
    struct timespec t = {game_duration, 0};

    Player *p = malloc(sizeof(Player));
    p->time_remaining = t;
    p->is_active = false;
    if (name == NULL) {
        snprintf(p->name, sizeof(p->name), "Player %d", id);
    } else {
        snprintf(p->name, sizeof(p->name), "%.10s", name);
    }
    
    return p;
}

void destroy_player(Player *p) {
    if (p == NULL) return;
    free(p);
}

// ************************************************************************************
// ----- Player Functions -------------------------------------------------------------
// ************************************************************************************

bool is_active(Player *p) {
    return p->is_active;
}

void set_active(Player *p, bool is_active) {
    p->is_active = is_active;
}

bool is_eliminated(Player *p) {
    return p->time_remaining.tv_sec < 0;
}

struct timespec get_time_remaining(Player *p) {
    return p->time_remaining;
}

void set_time_remaining(Player *p, struct timespec t) {
    p->time_remaining = t;
}

char *get_name(Player *p) {
    return p->name;
}

static void set_name(Player *p, const char *name) {
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
}

static void input_player_name(Player *p, int id) {
    char name[21];
    printw("Input Player %d's name (max 20 characters): ", id);
    refresh();

    echo();
    curs_set(1);
    wgetnstr(stdscr, name, sizeof(name));
    noecho();
    curs_set(0);
    if (strcmp(name, "") == 0)
        return;
    set_name(p, name);
}

void customize_player_names(Game *g) {
    clear();
    refresh();
    printw("Customize player names? (y/n): ");
    int ch;
    echo();
    curs_set(1);
    ch = getch();
    noecho();
    curs_set(0);

    // Skip and have defaults
    if (ch == 'n')
        return;

    clear();
    refresh();
    for (int i = 1; i <= 2; i++)
        input_player_name(get_player(g, i), i);

}