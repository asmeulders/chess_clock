#define _POSIX_C_SOURCE 199309L
// System Imports
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Project Imports
#include "players.h"

struct Player {
    struct timespec time_remaining;
    bool is_active;
};

Player *create_player(int game_duration) {
    struct timespec t = {game_duration, 0};

    Player *p = malloc(sizeof(Player));
    p->time_remaining = t;
    p->is_active = false;
    return p;
}

void destroy_player(Player *p) {
    if (p == NULL) return;

    free(p);
}

bool is_active(Player *p) {
    return p->is_active;
}

void set_active(Player *p, bool is_active) {
    p->is_active = is_active;
}

bool is_eliminated(Player *p) {
    return p->time_remaining.tv_sec <= 0 && p->time_remaining.tv_nsec <= 0;
}