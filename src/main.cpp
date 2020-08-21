#include <iostream>
#include <SFML/Graphics.hpp>

#include "Utils/Constants.h"
#include "snakeObject/Snake.h"

int main()
{
    // Setting up the window
    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SnakeLearning!");
    window.setFramerateLimit(Constants::FPS);

    // Setting up an event handler
    sf::Event event;

    // Creating the snake
    Snake snake;

    while (window.isOpen())
    {
        // Handling events
        while (window.pollEvent(event))
        {
            // Handling window closing
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Logic handling
        // Moving the snake
        snake.move();

        // Clear the old screen
        window.clear(sf::Color(0 ,0, 0));

        // Drawing stuff
        size_t snakeLength = snake.getLength();
        Utils::DynamicArray<sf::RectangleShape*>* snakeNodes = snake.getNodes();

        for (size_t i = 0; i < snakeLength; i++)
        {
            sf::RectangleShape* currNode = snakeNodes->GetItem(i);
            window.draw(*currNode);
        }

        // Display the rendered stuff
        window.display();
    }
}

