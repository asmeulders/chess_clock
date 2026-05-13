#ifndef GAME
#define GAME

typedef struct Game Game;

Game* create_game();
void destroy_game(Game *g);
void start_game();
void end_game();

#endif