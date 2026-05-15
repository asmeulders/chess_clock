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
    struct timespec end_of_turn;
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
    g->active_pid = -1;
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

Player *get_active_player(Game *g) {
    return get_player(g, g->active_pid);
}

void start_game_loop(Game *g) {   
    flush_buffer();
    printf("Game ready, press Enter to begin...");
    getchar();
    start_game(g);
    struct timespec ts = {0, 100000000};
    
    atexit(restore_terminal);
    set_raw_mode();
    while (g->in_progress) {
        char c;
        // printf("Enter a character: "); // flush the buffer here because it is getting called twice
        if (read(STDIN_FILENO, &c, 1) == 1) {
            // input was available
            switch (c) {
                case 'p': pause_game(g); break;
                case 's': game_over(g); break;
                case 'e': end_turn(g); break; // have a different function for pause vs game over
                case 'r': reset_game(g); break;
                default: break;
            }
        } else {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            // do calculations
            long remaining_sec = get_turn_end(g->clock).tv_sec - now.tv_sec;
            long remaining_nsec = get_turn_end(g->clock).tv_nsec - now.tv_nsec;
            if (remaining_nsec < 0) {
                remaining_sec -= 1;
                remaining_nsec += 1000000000; // this can continue to add time after the game is over so be careful
            }
            struct timespec time_remaining = { remaining_sec, remaining_nsec };
            set_time_remaining(get_active_player(g), time_remaining);

            // check if eliminated
            if (time_remaining.tv_sec < 0) {
                game_over(g); // have a different function for pause vs game over
            }

            // update clock 
            long seconds = remaining_sec % 60;
            long minutes = remaining_sec / 60;
            long tenth = remaining_nsec / 100000000L; 
            printf("\r%02ld:%02ld.%ld active - %d", minutes, seconds, tenth, g->active_pid);
            fflush(stdout);
            nanosleep(&ts, NULL);  // 0.1 seconds
        }
    }
}

void start_game(Game *g) {
    printf("Start Game\n");
    // activate player 1
    g->active_pid = 1;
    set_active(g->p1, true); // do i even need this?
    // start clock
    // g->in_standby = false;
    g->in_progress = true;
    calculate_turn_end(g->clock, get_time_remaining(get_active_player(g)));

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long duration_sec = get_turn_end(g->clock).tv_sec - now.tv_sec;
    printf("Turn end (s): %ld\n", duration_sec);
}

void pause_game(Game *g) {
    flush_buffer();
    printf("Game paused, press Enter to resume...");
    getchar();
}

void game_over(Game *g) {
    // deactivate both players
    g->active_pid = -1;
    set_active(g->p1, false);
    set_active(g->p2, false);
    // stops clock
    g->in_progress = false;
}

void reset_game(Game *g) {
    printf("Reset Game\n");
    game_over(g);
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
    g->active_pid = (g->active_pid) % 2 + 1;
    // Change active players - this might not be necessary
    if (is_active(p1) != is_active(p2)) {
        set_active(p1, !is_active(p1));
        set_active(p2, !is_active(p2));
    } else {
        printf("Game has not started yet.\n");
    }

    calculate_turn_end(g->clock, get_time_remaining(get_active_player(g)));
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