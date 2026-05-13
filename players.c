#define _POSIX_C_SOURCE 199309L
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "players.h"

typedef struct {
    struct timespec time_remaining;
    bool is_active;
} Player;

Player* create_player(int game_duration) {
    struct timespec duration = { .tv_sec = game_duration, .tv_nsec = 0 };
    Player *p = {duration, false};
}

void destroy_player(Player *p) {
    if (p == NULL) return;

    free(p);
}

bool is_active(Player *p) {
    return p->is_active;
}

bool is_eliminated(Player *p) {
    return p->time_remaining.tv_sec <= 0 && p->time_remaining.tv_nsec <= 0;
}

void change_turns(Player *p1, Player *p2) {
    if (p1->is_active != p2->is_active) {
        p1->is_active = !p1->is_active;
        p2->is_active = !p2->is_active;
    } else {
        printf("Game has not started yet.");
    }
}