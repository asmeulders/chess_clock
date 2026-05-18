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
#include <string.h>

// Project Imports
#include "../include/player.h"
#include "../include/game.h"
#include "../include/clock.h"
#include "../include/time_controls.h"
#include "../include/utils.h"

// ************************************************************************************
// ----- Static Variables -------------------------------------------------------------
// ************************************************************************************

static struct timespec ts = {0, 100000000};

// ************************************************************************************
// ----- Static Declarations ----------------------------------------------------------
// ************************************************************************************

static int get_duration(Game *g);
static void stand_by();
static void start_game(Game *g);
static void pause_game(Game *g);
static void end_turn(Game *g);
static void stop_game(Game *g);
static void exit_game(Game *g);
static void game_over(Game *g);
static int new_game(Game *g);
static void restart_game(Game *g);
static void update(Game *g);
static void initialize_players(Game *g);
static Player *get_active_player(Game *g);
static void reset_players(Game *g);
static void format_time(char *time_str, int len, Game *g, int id);


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

/* Creates a game and returns a pointer. Initializes:
- running = 0
- paused = 0
- active_pid = 0
- players
*/
Game *create_game(Clock *cl) {
    Game *g = malloc(sizeof(Game));
    g->clock = cl;
    g->running = 0;
    g->paused = 0;
    g->active_pid = 0;
    g->p1 = NULL;
    g->p2 = NULL;
    initialize_players(g);
    return g;
}

/* Frees game from memory. */
void destroy_game(Game *g) {
    if (g == NULL) return;
    destroy_clock(g->clock);
    destroy_player(g->p1);
    destroy_player(g->p2);
    free(g);
}

// ************************************************************************************
// ----- Game Loop --------------------------------------------------------------------
// ************************************************************************************

/* Starts the game loop. Begins by waiting for user to start game.
Then enters non-blocking user input and will update the timer. 
Commands:
- p -> Pauses game
- s -> Stops game
- <space> -> Ends currnt player's turn
- r -> reset game from beginning 
*/
void start_game_loop(Game *g) { 
    stand_by();
    start_game(g);
    int unknown_count = 0;
    while (g->running && g->paused == 0) {
        int input = getch();
        switch (input) {
            case 'p': pause_game(g); break;
            case 'e': stop_game(g); break;
            case ' ': end_turn(g); break; 
            case 'r': restart_game(g); break;
            default: unknown_command(&unknown_count); break;
        }
        
        if (g->active_pid == 0) {
            return;
        }

        update(g);
        if (is_eliminated(get_active_player(g))) {
            game_over(g);
        }

        // sleep
        nanosleep(&ts, NULL);
    }
}

// ************************************************************************************
// ----- Helper Functions -------------------------------------------------------------
// ************************************************************************************

/* Returns minutes per player. */
static int get_duration(Game *g) {
    return get_minutes(get_time_controls(g->clock));
}

static void stand_by() {
    clear();
    refresh();
    printw("Game ready, press Enter to begin...\n");
    refresh();
    int begin;
    while ((begin = getch()) != '\n');
}

static void format_time(char *time_str, int len, Game *g, int id) {
    struct timespec time_remaining = get_time_remaining(get_player(g, id));
    long h = time_remaining.tv_sec / 3600;
    long m = (time_remaining.tv_sec % 3600) / 60;
    long s = time_remaining.tv_sec % 60;
    long t = time_remaining.tv_nsec / 100000000; 

    snprintf(time_str, len, "%02ld:%02ld:%02ld.%ld", h, m, s, t);
}

// ************************************************************************************
// ----- Internal Functions -----------------------------------------------------------
// ************************************************************************************

/* Starts game and changes values for the loop to begin. Prints timer. */
static void start_game(Game *g) {
    // activate player 1
    g->active_pid = 1;
    set_active(g->p1, true); // do i even need this?
    g->running = 1;
    calculate_turn_end(g->clock, get_time_remaining(get_active_player(g)));

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    clear();
    refresh();
    printw("Chess: %d | %d\n", get_minutes(get_time_controls(g->clock)), get_seconds(get_time_controls(g->clock)));
    nodelay(stdscr, TRUE);
}

/* Pauses the game. */
static void pause_game(Game *g) {
    struct timespec time_remaining = get_time_remaining(get_active_player(g));

    int y, x;
    getyx(stdscr, y, x);
    const char *paused = " (PAUSED) Press enter to resume";
    printw("%s", paused);
    refresh();

    nodelay(stdscr, FALSE);
    int resume;
    while ((resume = getch()) != '\n');
    nodelay(stdscr, TRUE);

    calculate_turn_end(g->clock, time_remaining);

    // clear text
    mvwhline(stdscr, y, x, ' ', strlen(paused));  // write n spaces starting at (y, x)
    wmove(stdscr, y, x); // go back to original position
    refresh();
}

/* Ends the turn of the active player. Will add time to clock if needed. */
static void end_turn(Game *g) {
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
        struct timespec time_remaining = get_time_remaining(p);
        time_remaining.tv_sec += get_seconds(get_time_controls(g->clock));
        set_time_remaining(p, time_remaining);
    }

    // Change active players
    g->active_pid = (g->active_pid) % 2 + 1;
    // Change active players - this might not be necessary
    if (is_active(p1) != is_active(p2)) {
        set_active(p1, !is_active(p1));
        set_active(p2, !is_active(p2));
    } else {
        // printf("Game has not started yet.\n");
    }

    calculate_turn_end(g->clock, get_time_remaining(get_active_player(g)));
}

/* Stops current game and allows user to perform other commands. */
static void stop_game(Game *g) {
    printw(" (Game stopped) Play again?\n");
    refresh();
    nodelay(stdscr, FALSE);
    int listening = 1;
    int new_game_fail = 0;
    int unknown_count = 0;

    while (listening) {
        if (new_game_fail) {
            clear();
            printw("Play again?\n");
            refresh();
        }
        int input = getch();
        switch (input) {
            case 'r': listening = 0; restart_game(g); break; // restart game
            case 'n': new_game_fail = new_game(g); listening = new_game_fail; break; // new game
            case 'm': customize_player_names(g); break; // customize player names
            case 'q': listening = 0; exit_game(g); break; // exit
            default: unknown_command(&unknown_count); break; // unknown command
        }
    }
}

/* Ends game where one player wins. */
static void game_over(Game *g) {
    printw("\rGame Over, Player %d wins!", (g->active_pid % 2) + 1);
    refresh();
    stop_game(g);
}

static void exit_game(Game *g) {
    // deactivate both players and stop game
    g->active_pid = 0;
    set_active(g->p1, false);
    set_active(g->p2, false);
    g->running = 0;
    g->paused = 0;
}

/* Reset the game from standby with current time controls. */
static void restart_game(Game *g) {
    clear();
    reset_players(g);
    printw("Restarting game...\n");
    stand_by();
    start_game(g);
}

/* Make a new game by updating time controls and clock in game struct. */
static int new_game(Game *g) {
    printw("Making new game...\n");
    TimeControls *tc = input_time_controls();
    if (tc == NULL) 
        return EXIT_FAILURE;
    destroy_clock(g->clock);
    Clock *cl = create_clock(tc);
    g->clock = cl;
    reset_players(g);
    
    stand_by();
    start_game(g);
    return EXIT_SUCCESS;
}

/* Update the timer display and sets the time remaining for a player. */
static void update(Game *g) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec time_remaining = subtract_timespec(get_turn_end(g->clock), now);
    set_time_remaining(get_active_player(g), time_remaining);

    // update clock
    char p1_time_str[12];
    char p2_time_str[12];
    format_time(p1_time_str, sizeof(p1_time_str), g, 1);
    format_time(p2_time_str, sizeof(p2_time_str), g, 2);
    printw("\r%s: %s | %s: %s", get_name(get_player(g, 1)), p1_time_str, get_name(get_player(g, 2)), p2_time_str);
    refresh();
}
// ************************************************************************************
// ----- Acessing players -------------------------------------------------------------
// ************************************************************************************

/* Initialize players with the game duration. */
static void initialize_players(Game *g) {
    Player *p1 = create_player(get_duration(g) * 60, 1, NULL); // seconds
    Player *p2 = create_player(get_duration(g) * 60, 2, NULL);
    g->p1 = p1;
    g->p2 = p2;
    // TODO: get player names here
    customize_player_names(g);
}

/* Reset the players to be not active and have current time controls minutes remaining. */
static void reset_players(Game *g) {
    struct timespec t = { get_duration(g) * 60, 0 };
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    // Reset values
    set_active(p1, false);
    set_active(p2, false);
    set_time_remaining(p1, t);
    set_time_remaining(p2, t);
}

/* Returns a player with a given id. */
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
        // printf("Invalid id: %d\n", id);
        break;
    }

    return p;
}

/* Returns the current active player in a game. */
static Player *get_active_player(Game *g) {
    return get_player(g, g->active_pid);
}