// System Imports
#include <stdio.h>
#include <stdbool.h>

// Project Imports
#include "game.h"


int main()
{
    Game *g = create_game(60);
    printf("Game duration: %d\n", get_duration(g));
    Player *p1 = get_player(g, 1);
    Player *p2 = get_player(g, 2);
    printf("Player 1:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p1) ? "false" : "true", is_eliminated(p1) ? "false" : "true");
    printf("Player 2:\n - Not Active: %s\n - Not Eliminated: %s\n", is_active(p2) ? "false" : "true", is_eliminated(p2) ? "false" : "true");
    printf("Handle incorrect id: %s\n", get_player(g, 3) == NULL ? "success" : "failure");
}

