// System imports
#include <stdio.h>

// Project imports
#include "game.h"
#include "players.h"


void basic_test() {
    Game *g = create_game(60);
    printf("Game duration: %d\n", get_duration(g));
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    printf("Handle incorrect id: %s\n", get_player(g, 3) == NULL ? "success" : "failure");
    destroy_game(g);
    g = NULL; p1 = NULL; p2 = NULL;
}

void basic_game_test() {
    Game *g = create_game(60);
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
}

void change_turns_test() {
    Game *g = create_game(60);
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
}

void change_turns_not_started_test() {
    Game *g = create_game(60);
    end_turn(g);
    destroy_game(g);
    g = NULL;
}