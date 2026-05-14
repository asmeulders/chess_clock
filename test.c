#define _POSIX_C_SOURCE 199309L
// System Imports
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Project imports
#include "game.h"
#include "players.h"
#include "clock.h"

TimeControls *make_mtc() {
    TimeControls *mtc = malloc(sizeof(TimeControls*));
    return mtc;
}

void destroy_mtc(TimeControls *mtc) {
    free(mtc);
}

void basic_test() {
    TimeControls *mtc = make_mtc();
    Game *g = create_game(mtc);
    printf("Game duration: %d\n", get_duration(g));
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    printf("Handle incorrect id: %s\n", get_player(g, 3) == NULL ? "success" : "failure");
    destroy_game(g);
    g = NULL; p1 = NULL; p2 = NULL;
    destroy_mtc(mtc);
}

void basic_game_test() {
    TimeControls *mtc = make_mtc();
    Game *g = create_game(mtc);
    start_game(g);
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    stop_game(g);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    destroy_game(g);
    g = NULL; p1 = NULL; p2 = NULL;
    destroy_mtc(mtc);
}

void change_turns_test() {
    TimeControls *mtc = make_mtc();
    Game *g = create_game(mtc);
    start_game(g);
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    end_turn(g);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    destroy_game(g);
    g = NULL; p1 = NULL; p2 = NULL;
    destroy_mtc(mtc);
}

void change_turns_not_started_test() {
    TimeControls *mtc = make_mtc();
    Game *g = create_game(mtc);
    end_turn(g);
    destroy_game(g);
    g = NULL;
    destroy_mtc(mtc);
}

void start_clock_test() {
    TimeControls *mtc = make_mtc();
    Clock *c = create_clock(mtc);
    start_clock(c);

    printf("Clock start time: %ld\n", get_game_start(c).tv_sec);
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    printf("Time Now: %ld\n", time.tv_sec);
    destroy_clock(c);
    // free(mtc);
}

void clear_clock_test() {
    TimeControls *mtc = make_mtc();
    Clock *c = create_clock(mtc);
    clear_clock(c);
    printf("Clock start time: %ld\n", get_game_start(c).tv_sec);
    destroy_clock(c);
    // free(mtc);
}

void set_game_start_test() {
    TimeControls *mtc = make_mtc();
    Clock *c = create_clock(mtc);
    struct timespec *time = malloc(sizeof(struct timespec));
    time->tv_sec = 10; time->tv_nsec = 10;

    set_game_start(c, time);
    printf("Clock start time: %ld\n", get_game_start(c).tv_sec);

    free(time);
    time = NULL;
    destroy_clock(c);
}

void set_turn_start_test() {
    TimeControls *mtc = make_mtc();
    Clock *c = create_clock(mtc);
    struct timespec *time = malloc(sizeof(struct timespec));
    time->tv_sec = 10; time->tv_nsec = 10;

    set_turn_start(c, time);
    printf("Clock start time: %ld\n", get_turn_start(c).tv_sec);

    free(time);
    time = NULL;
    destroy_clock(c);
}