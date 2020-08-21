#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/DynamicArray.h"
#include "../Utils/Constants.h"

class Snake
{
    private:
        int length;
        char direction;
        int headX, headY;
        Utils::DynamicArray<sf::RectangleShape*> nodes;
    public:
        Snake();
        Utils::DynamicArray<sf::RectangleShape*>* getNodes();
        int getLength() const;
        char getDirection() const;

        // Snake functionallities
        void move();
        void setDirection(const char& direction);
};

#include "Snake.cpp"
