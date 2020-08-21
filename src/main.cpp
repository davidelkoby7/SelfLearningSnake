#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "Utils/Constants.h"
#include "Utils/GeneralFunctions.cpp"
#include "Utils/DynamicArray.h"
#include "snakeObject/Snake.h"

void repositionApple(sf::RectangleShape& apple, Snake& snake)
{
    bool isAppleOnSnake = true;
    while (isAppleOnSnake == true)
    {
        apple.setPosition(GeneralFunctions::RandomInt(1, Constants::GRID_WIDTH - 2) * Constants::TILE_SIZE, GeneralFunctions::RandomInt(1, Constants::GRID_HEIGHT -2) * Constants::TILE_SIZE);
        isAppleOnSnake = false;

        Utils::DynamicArray<sf::RectangleShape*>* snakeNodes = snake.getNodes();
        for (size_t i = 0; i < snakeNodes->GetLength(); i++)
        {
            sf::RectangleShape* currNode = snakeNodes->GetItem(i);
            if (currNode->getGlobalBounds().intersects(apple.getGlobalBounds()))
            {
                isAppleOnSnake = true;
                break;
            }
        }
    }
}

int main()
{
    // Setting up the game
    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SnakeLearning!");
    window.setFramerateLimit(Constants::FPS);

    bool isPlaying = true;

    // Setting up an event handler
    sf::Event event;

    // Setting up the scoreboard and the generation number
    sf::Font font;
    if (!font.loadFromFile(Constants::FONT_PATH))
        std::cout << "Failed to load font!\n";

    // Setting up the info display for score and generation
    int score = 0;
    int generation = 1;
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(Constants::INFO_TEXT_SIZE);

    // Creating the snake
    Snake snake;

    // Creating the apple and putting him randomly aligned with the grid
    sf::RectangleShape apple(sf::Vector2f(Constants::TILE_SIZE, Constants::TILE_SIZE));
    repositionApple(apple, snake);
    apple.setFillColor(sf::Color(255, 0, 50));

    while (window.isOpen() && isPlaying)
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

        // Moving the snake
        snake.move();

        // Growing the snake's size if eats the apple
        if (apple.getGlobalBounds().intersects(snake.getHead()->getGlobalBounds()) == true)
        {
            snake.grow();
            repositionApple(apple, snake);
        }

        // Checking for collision of the snake with it's body
        if (snake.isEatingItself() == true || snake.isOutOfBounds() == true)
        {
            isPlaying = false;
            std::cout << "You lost! your score is " << score << "!\n";
            break;
        }

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

        window.draw(apple);

        std::stringstream infoString;
        score = snake.getLength() - 1;
        infoString << "Score: " << score << ", Generation: " << generation;

        infoText.setString(infoString.str());
        window.draw(infoText);

        // Display the rendered stuff
        window.display();
    }
}

