#define _POSIX_C_SOURCE 199309L
// System Imports
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Project Imports
#include "player.h"

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