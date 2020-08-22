#pragma once

namespace Constants
{
    // Math constants
    static const double e = 2.718281828459;

    // General game constants
    static const int FPS = 5;
    static const char* FONT_PATH = "fonts/Montserrat-SemiBold.otf";
    static const int INFO_TEXT_SIZE = 20;
    static const int SNAKES_PER_GENERATION = 10000;
    static const int MAX_TURNS_WITHOUT_EATING = 1000;

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

    /* ~~~ Neural Network file reading constants ~~~ */
    static const int LEARNING_RATE_LINE = 0;
    static const int NUM_OF_LAYERS_LINE = 1;
    static const int NUM_OF_NEURONS_PER_LAYER_LINE = 2;
    static const int NEURONS_BIASES_STARTING_LINE = 3;
    /* ~~~ Neural Network file reading constants ~~~ */
}
