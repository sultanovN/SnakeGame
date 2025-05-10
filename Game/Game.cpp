#include <stdlib.h>
#include <memory.h>
#include "Engine/GameClass.h"

Game game;

// initialize game data in this function
void initialize()
{
}
 
// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    game.gameLoop(dt);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    game.gameDraw();
}

// free game data in this function
void finalize()
{
}

