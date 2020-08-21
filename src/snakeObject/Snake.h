#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/DynamicArray.h"
#include "../Utils/Constants.h"
#include "../Utils/GeneralFunctions.cpp"

class Snake
{
    private:
        int length;
        char direction;
        int r, g, b;
        float snakeVX, snakeVY;
        Utils::DynamicArray<sf::RectangleShape*> nodes;
    public:
        Snake();

        // Getters
        Utils::DynamicArray<sf::RectangleShape*>* getNodes();
        int getLength() const;
        char getDirection() const;
        sf::RectangleShape* getHead();

        // Snake functionallities
        void move();
        void setDirection(const char& direction);
        void grow();
        void updateVelocity();
        bool isEatingItself();
        bool isOutOfBounds();

        // Learning functionallities
        double getObstacleDistance(const char& direction);
};

#include "Snake.cpp"
