#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "Utils/Constants.h"
#include "Utils/GeneralFunctions.cpp"
#include "Utils/DynamicArray.h"
#include "snakeObject/Snake.h"

void handleEvents(sf::Event& event, Snake& snake, sf::RenderWindow& window)
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

void drawScreen(sf::RenderWindow& window, Snake& snake, sf::RectangleShape& apple, sf::Text& infoText, int& score, int& generation)
{
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

void resetGame(int& score, int& biggestGenScore, size_t& biggestGenScoreIndex, size_t& snakeIndex, Utils::DynamicArray<int> scores, bool& isPlaying)
{
    if (score > biggestGenScore)
    {
        std::cout << "New highscore! score: " << score << "\n";
        biggestGenScore = score;
        biggestGenScoreIndex = snakeIndex;
    }
    scores.SetItem(snakeIndex, score);
    isPlaying = false;
}

int main()
{
    bool geneticLearning = false;
    bool bestSnakeAutoPlay = true;

    // Setting up the game
    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SnakeLearning!");
    window.setFramerateLimit(Constants::FPS);

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

    // Creating the snakes
    size_t snakesLen = geneticLearning ? Constants::SNAKES_PER_GENERATION : 1;
    Utils::DynamicArray<Snake> snakes(snakesLen);

    if (bestSnakeAutoPlay == true)
        snakes.SetItem(0, Snake("./Snake.nn"));

    Utils::DynamicArray<int> scores(snakesLen);

    size_t biggestGenScoreIndex = 0;
    int biggestGenScore = 0;

    for (size_t i = 0; i < snakesLen; i++)
    {
        Snake& snake = snakes.GetItem(i);
        int turnsWithoutEating = 0;

        // Creating the apple and putting him randomly aligned with the grid
        sf::RectangleShape apple(sf::Vector2f(Constants::TILE_SIZE, Constants::TILE_SIZE));
        repositionApple(apple, snake);
        apple.setFillColor(sf::Color(255, 0, 50));

        bool isPlaying = true;

        while (window.isOpen() && isPlaying)
        {
            // Handling events
            while (window.pollEvent(event))
                handleEvents(event, snake, window);

            // Moving the snake
            if (geneticLearning == true || bestSnakeAutoPlay)
                snake.autoMove(apple.getPosition().x, apple.getPosition().y);
            else
                snake.move();

            // Growing the snake's size if eats the apple
            if (apple.getGlobalBounds().intersects(snake.getHead()->getGlobalBounds()) == true)
            {
                snake.grow();
                repositionApple(apple, snake);
                turnsWithoutEating = 0;
            }
            else
                turnsWithoutEating++;

            // Checking for collision of the snake with it's body
            if (snake.isEatingItself() == true || snake.isOutOfBounds() == true || turnsWithoutEating > Constants::MAX_TURNS_WITHOUT_EATING)
                resetGame(score, biggestGenScore, biggestGenScoreIndex, i, scores, isPlaying);

            // Clear the old screen
            drawScreen(window, snake, apple, infoText, score, generation);
        } // end of while
    } // end of for
    std::cout << "Biggest score: " << biggestGenScore << " in index: " << biggestGenScoreIndex << "\n";
    snakes.GetItem(biggestGenScoreIndex).saveNN();
}

