#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/DynamicArray.h"
#include "../Utils/Constants.h"
#include "../Utils/GeneralFunctions.cpp"
#include "../NeuralNetwork/NeuralNetwork.h"

class Snake
{
    private:
        int length;
        char direction;
        int r, g, b;
        float snakeVX, snakeVY;
        Utils::DynamicArray<sf::RectangleShape*> nodes;
        NeuralNetwork* neuralNetwork;
    public:
        Snake();
        Snake(const char* nnPath);
        void reset(const char* nnPath);

        // Getters
        Utils::DynamicArray<sf::RectangleShape*>* getNodes();
        int getLength() const;
        char getDirection() const;
        NeuralNetwork* getNN();
        sf::RectangleShape* getHead();

        // Snake functionallities
        void saveNN();
        void move();
        void setDirection(const char& direction);
        void grow();
        void updateVelocity();
        bool isEatingItself();
        bool isOutOfBounds();

        // Learning functionallities
        double getObstacleDistance(const char& direction);
        void autoMove(const int& appleX, const int& appleY);
        void evolve();
};

#include "Snake.cpp"
