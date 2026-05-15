#define _POSIX_C_SOURCE 199309L
// System Imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

// Project Imports
#include "players.h"
#include "game.h"
#include "clock.h"

struct Game {
    Clock *clock;
    Player *p1;
    Player *p2;
    bool in_standby;
    bool in_progress;
    int active_pid;
};

Game *create_game(Clock *cl) {
    printf("Create game\n");
    // fflush(stdout);
    Game *g = malloc(sizeof(Game));
    g->clock = cl;
    g->in_standby = false;
    g->in_progress = false;
    g->active_pid = 1;
    initialize_players(g);
    return g;
}

void destroy_game(Game *g) {
    if (g == NULL) return;
    destroy_clock(g->clock);
    destroy_player(g->p1);
    destroy_player(g->p2);
    free(g);
}

void initialize_players(Game *g) {
    printf("Initialize players\n");
    // fflush(stdout);
    Player *p1 = create_player(get_duration(g) * 60); // seconds
    Player *p2 = create_player(get_duration(g) * 60);
    g->p1 = p1;
    g->p2 = p2;
}

void reset_players(Game *g) {
    struct timespec t = { get_duration(g), 0 };
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    // Reset values
    set_active(p1, false);
    set_active(p2, false);
    set_time_remaining(p1, t);
    set_time_remaining(p2, t);
}

void start_game_loop(Game *g) {   
    flush_buffer();
    printf("Game ready, press Enter to begin...");
    getchar();
    start_game(g);
    struct timespec ts = {0, 100000000};
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    atexit(restore_terminal);
    set_raw_mode();
    while (g->in_progress) {
        char c;
        // printf("Enter a character: "); // flush the buffer here because it is getting called twice
        if (read(STDIN_FILENO, &c, 1) == 1) {
            // input was available
            switch (c) {
                case 'p': stop_game(g); break;
                case 'e': end_turn(g); break;
                case 'r': reset_game(g); break;
                default: break;
            }
        } else {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            long elapsed_sec = now.tv_sec - start.tv_sec;
            long elapsed_nsec = now.tv_nsec - start.tv_nsec;
            if (elapsed_nsec < 0) {
                elapsed_sec -= 1;
                elapsed_nsec += 1000000000;
            }
            long seconds = elapsed_sec % 60;
            long minutes = elapsed_sec / 60;
            long tenth = elapsed_nsec / 100000000L; 
            printf("\r%02ld:%02ld.%ld", minutes, seconds, tenth);
            fflush(stdout);
            nanosleep(&ts, NULL);  // 0.1 seconds
        }
    }
}

void start_game(Game *g) {
    printf("Start Game\n");
    // activate player 1
    set_active(g->p1, true);
    // start clock
    // g->in_standby = false;
    g->in_progress = true;
}

void stop_game(Game *g) {
    printf("Stop Game\n");
    // deactivate both players
    set_active(g->p1, false);
    set_active(g->p2, false);
    // stops clock
    g->in_progress = false;
}

void reset_game(Game *g) {
    printf("Reset Game\n");
    stop_game(g);
    reset_players(g);
}

Player *get_player(Game *g, int id) {
    Player *p = NULL;

    switch (id)
    {
    case 1:
        p = g->p1;
        break;
    
    case 2:
        p = g->p2;
        break;
    default:
        printf("Invalid id: %d\n", id);
        break;
    }

    return p;
}

int get_duration(Game *g) {
    printf("Get duration\n");
    // fflush(stdout);
    return get_minutes(get_time_controls(g->clock));
}

void end_turn(Game *g) {
    printf("End Turn\n");
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    Player *p;

    // Add seconds
    if (get_seconds(get_time_controls(g->clock)) > 0) {
        if (is_active(p1)) {
            p = p1;
        } else {
            p = p2;
        }
        struct timespec time = get_time_remaining(p);
        time.tv_sec = time.tv_sec + get_seconds(get_time_controls(g->clock));
        set_time_remaining(p, time);
    }

    // Change active players
    if (is_active(p1) != is_active(p2)) {
        set_active(p1, !is_active(p1));
        set_active(p2, !is_active(p2));
    } else {
        printf("Game has not started yet.\n");
    }
}

void flush_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void set_raw_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);  // disable line buffering and echo
    t.c_cc[VMIN] = 0;   // don't block waiting for input
    t.c_cc[VTIME] = 0;  // no timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void restore_terminal() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}