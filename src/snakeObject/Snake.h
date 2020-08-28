#pragma once

#include <SFML/Graphics.hpp>

#include "../Utils/DynamicArray.h"
#include "../Utils/Constants.h"
#include "../Utils/GeneralFunctions.cpp"
#include "../NeuralNetwork/NeuralNetwork.h"
#include "../NeuralNetwork/Neuron.h"

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
        void resetSnakeNodes();
        void reset(const char* nnPath);

        // Getters
        Utils::DynamicArray<sf::RectangleShape*>* getNodes();
        int getLength() const;
        char getDirection() const;
        NeuralNetwork* getNN();
        sf::RectangleShape* getHead();

        // Snake functionallities
        void saveNN(const char* path = "Snake.nn");
        void setNN(NeuralNetwork* newNN);
        void move();
        void setDirection(const char& direction);
        void grow();
        void updateVelocity();
        bool isEatingItself();
        bool isOutOfBounds();

        // Learning functionallities
        double getObstacleDistance(const char& direction);
        void autoMove(const int& appleX, const int& appleY);
        void mutate();
};

#include "Snake.cpp"
