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

            // Handling direction changes by the user
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                    snake.setDirection(Constants::SNAKE_UP);
                else if (event.key.code == sf::Keyboard::Down)
                    snake.setDirection(Constants::SNAKE_DOWN);
                else if (event.key.code == sf::Keyboard::Left)
                    snake.setDirection(Constants::SNAKE_LEFT);
                else if (event.key.code == sf::Keyboard::Right)
                    snake.setDirection(Constants::SNAKE_RIGHT);
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

