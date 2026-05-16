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
#include <ncurses.h>

// Project Imports
#include "players.h"
#include "game.h"
#include "clock.h"

// ************************************************************************************
// ----- Static Variables -------------------------------------------------------------
// ************************************************************************************

static struct termios orig;
static struct timespec ts = {0, 100000000};

// ************************************************************************************
// ----- Game Struct ------------------------------------------------------------------
// ************************************************************************************

struct Game {
    Clock *clock;
    Player *p1;
    Player *p2;
    struct timespec end_of_turn;
    // int standby;
    int running;
    int paused;
    int active_pid;
};

Game *create_game(Clock *cl) {
    // fflush(stdout);
    Game *g = malloc(sizeof(Game));
    g->clock = cl;
    // g->standby = 0;
    g->running = 0;
    g->paused = 0;
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

// ************************************************************************************
// ----- Helper Functions -------------------------------------------------------------
// ************************************************************************************

static int get_duration(Game *g) {
    printf("Get duration\n");
    // fflush(stdout);
    return get_minutes(get_time_controls(g->clock));
}

static void flush_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// ************************************************************************************
// ----- Internal Functions -----------------------------------------------------------
// ************************************************************************************

static void start_game(Game *g) {
    // activate player 1
    g->active_pid = 1;
    set_active(g->p1, true); // do i even need this?
    // start clock
    // g->standby = 0;
    g->running = 1;
    calculate_turn_end(g->clock, get_time_remaining(get_active_player(g)));

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long duration_sec = get_turn_end(g->clock).tv_sec - now.tv_sec;
}

static void pause_game(Game *g) { // TODO
    flush_buffer();
    printw("Game paused, press Enter to resume...");
    refresh();
    getchar();
}

static void end_turn(Game *g) {
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

static void game_over(Game *g) {
    // deactivate both players
    g->active_pid = -1;
    set_active(g->p1, false);
    set_active(g->p2, false);
    // stops clock
    g->running = 0;
    g->paused = 0;

    printw("\rGame Over, Player %d wins!", g->active_pid);
    refresh();
}

static void reset_game(Game *g) { // TODO
    game_over(g);
    reset_players(g);
}

static void update(Game *g) {
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

    // update clock - could make this into its own function in player that returns a string
    struct timespec p1_time_remaining = get_time_remaining(get_player(g, 1));
    long p1_seconds = p1_time_remaining.tv_sec % 60;
    long p1_minutes = p1_time_remaining.tv_sec / 60;
    long p1_tenth = p1_time_remaining.tv_nsec / 100000000; 

    struct timespec p2_time_remaining = get_time_remaining(get_player(g, 2));
    long p2_seconds = p2_time_remaining.tv_sec % 60;
    long p2_minutes = p2_time_remaining.tv_sec / 60;
    long p2_tenth = p2_time_remaining.tv_nsec / 100000000; 

    printw("\rPlayer 1: %02ld:%02ld.%ld | Player 2: %02ld:%02ld.%ld", p1_minutes, p1_seconds, p1_tenth, p2_minutes, p2_seconds, p2_tenth); // have two clocks, one per player
    refresh();
}

// ************************************************************************************
// ----- Acessing players -------------------------------------------------------------
// ************************************************************************************

static void initialize_players(Game *g) {
    // fflush(stdout);
    Player *p1 = create_player(get_duration(g) * 60); // seconds
    Player *p2 = create_player(get_duration(g) * 60);
    g->p1 = p1;
    g->p2 = p2;
}

static void reset_players(Game *g) {
    struct timespec t = { get_duration(g), 0 };
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    // Reset values
    set_active(p1, false);
    set_active(p2, false);
    set_time_remaining(p1, t);
    set_time_remaining(p2, t);
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

Player *get_active_player(Game *g) {
    return get_player(g, g->active_pid);
}

// ************************************************************************************
// ----- Game Loop --------------------------------------------------------------------
// ************************************************************************************

void start_game_loop(Game *g) { 
    printw("Game ready, press Enter to begin...\n");
    refresh();
    int begin = getch();

    start_game(g);
    nodelay(stdscr, TRUE);
    while (g->running && g->paused == 0) {
        int input = getch();
        switch (input) {
            case 'p': pause_game(g); break;
            case 's': game_over(g); break;
            case 'e': end_turn(g); break; // TODO: have a different function for pause vs game over
            case 'r': reset_game(g); break;
            default: break;
        }
        
        update(g);
    
        // check if eliminated - make its own function
        if (get_time_remaining(get_active_player(g)).tv_sec < 0) {
            game_over(g); // have a different function for pause vs game over
        }

        // sleep
        nanosleep(&ts, NULL);
    }
    endwin();
}