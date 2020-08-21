#pragma once

namespace Constants
{
    // General game constants
    static const int FPS = 12;

    // Game sizes constants
    static const int TILE_SIZE = 30;
    static const int GRID_WIDTH = 30; // Units of tiles
    static const int GRID_HEIGHT = 18; // Units of tiles
    static const int SCREEN_WIDTH = TILE_SIZE * GRID_WIDTH;
    static const int SCREEN_HEIGHT = TILE_SIZE * GRID_HEIGHT;

    // Snake constants
    static const float SNAKE_SPEED = TILE_SIZE;
    static const char SNAKE_UP = 0;
    static const char SNAKE_DOWN = 1;
    static const char SNAKE_LEFT = 2;
    static const char SNAKE_RIGHT = 3;
}
