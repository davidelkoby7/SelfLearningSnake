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

void drawScreen(sf::RenderWindow& window, Snake& snake, sf::RectangleShape& apple, sf::Text& infoText, int& score, size_t& generation, int& genScoreSum, const size_t& snakeIndex)
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
    infoString << "Score: " << score << ", Generation: " << generation << ", SnakeNumber: " << snakeIndex << ", GenerationAvg: " << (static_cast<float>(genScoreSum) / snakeIndex);

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

void resetGame(Snake& currSnake, int score, Utils::DynamicArray<int>& bestSnakesIndecies, size_t& worseBestSnakeIndex, int& worseBestSnakeScore, int& biggestGenScore, size_t& biggestGenScoreIndex, size_t& snakeIndex, Utils::DynamicArray<float>& scores, int& genScoreSum, int& turnsLived, int& turnsWithoutEating, bool& isPlaying)
{
    genScoreSum += score;

    if (score > biggestGenScore)
    {
        std::cout << "New highscore! score: " << score << "\n";
        biggestGenScore = score;
        biggestGenScoreIndex = snakeIndex;
    }
    double fitness = score + static_cast<double>(turnsLived) * Constants::TURNS_FITNESS_VALUE;
    fitness -= 1 * (turnsWithoutEating > Constants::MAX_TURNS_WITHOUT_EATING);

    scores.SetItem(snakeIndex, fitness);
    isPlaying = false;

    // Updating the bestSnakes array for the genetic algoritm
    // First N snakes to fill the amount of snakes needed
    if (bestSnakesIndecies.GetLength() < Constants::SURVIVING_SNAKES_PER_GENERATION)
    {
        bestSnakesIndecies.AddItem(snakeIndex);
        if (worseBestSnakeIndex == 0)
            worseBestSnakeScore = score;
        // If the snake scored less then all - set him to the worst snake
        if (worseBestSnakeScore > score)
        {
            worseBestSnakeIndex = snakeIndex;
            worseBestSnakeScore = score;
        }
    }

    // After the first N snakes - check if the score is higher than the lowest scoring and if so - replace it and look for the next worse scoring in the best scoring snakes array
    else if (score > worseBestSnakeScore)
    {
        bestSnakesIndecies.SetItem(worseBestSnakeIndex, snakeIndex);
        worseBestSnakeScore = int(score);
        size_t bestSnakesLength = bestSnakesIndecies.GetLength();
        for (size_t i = 0; i < bestSnakesLength; i++)
        {
            size_t currScoreIndex = bestSnakesIndecies.GetItem(i);
            int currScore = scores.GetItem(currScoreIndex);
            if (currScore < worseBestSnakeScore)
            {
                worseBestSnakeIndex = currScoreIndex;
                worseBestSnakeScore = currScore;
            }
        }
    }
}

void createNewGeneration(Utils::DynamicArray<Snake>& snakes, Utils::DynamicArray<int>& bestSnakesIndecies)
{
    Utils::DynamicArray<Snake> newSnakesArray(snakes.GetLength());
    int duplicationFactor = (1.0f - Constants::RANDOM_SNAKES_PRECENTAGE_PER_GENERATION) / Constants::SURVIVING_PERCENTAGE_PER_GENERATION;
    std::cout << "duplicationFactor: " << duplicationFactor << "\n";

    // Copying the best snakes to the newSnakesArray and duplicating them (keeping enough random snakes as defined in Constants.h)
    for (size_t i = 0; i < bestSnakesIndecies.GetLength(); i++)
        for (size_t j = 0; j < duplicationFactor; j++)
            newSnakesArray.GetItem(i * duplicationFactor + j).setNN(snakes.GetItem(bestSnakesIndecies.GetItem(i)).getNN());

    // Mutating all the snakes and resetting them
    for (size_t i = 0; i < newSnakesArray.GetLength(); i++)
    {
        newSnakesArray.GetItem(i).mutate();
        newSnakesArray.GetItem(i).resetSnakeNodes();
    }

    snakes = newSnakesArray;
}

int main()
{
    bool geneticLearning = true;
    bool bestSnakeAutoPlay = false;

    // Setting up the game
    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SnakeLearning!");
    window.setFramerateLimit(Constants::FPS);

    // Setting up an event handler
    sf::Event event;

    // Setting up the scoreboard and the generation number
    sf::Font font;
    if (!font.loadFromFile(Constants::FONT_PATH))
        std::cout << "Failed to load font!\n";

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(Constants::INFO_TEXT_SIZE);

    // Creating the snakes
    size_t snakesLen = geneticLearning ? Constants::SNAKES_PER_GENERATION : 1;
    Utils::DynamicArray<Snake> snakes(snakesLen);

    if (bestSnakeAutoPlay == true)
        snakes.SetItem(0, Snake("./Snake.nn"));

    const int numOfGenerations = geneticLearning ? Constants::NUM_OF_GENERATION : 1;

    // Each iteration is a new generation
    for (size_t generation = 1; generation <= numOfGenerations; generation++)
    {
        Utils::DynamicArray<int> bestSnakesIndecies;
        size_t worseBestSnakeIndex = 0;
        int worseBestSnakeScore = 0;

        Utils::DynamicArray<float> scores(snakesLen);
        size_t biggestGenScoreIndex = 0;
        int biggestGenScore = 0;

        int genScoreSum = 0;

        /*
        for (int i = 0; i < snakes.GetLength(); i++)
        {
            std::string filename = "./snakes/snake_";
            filename += std::to_string(i);
            filename += "_generation_";
            filename += std::to_string(generation);
            snakes.GetItem(i).saveNN(filename.c_str());
        }
        */

        // Each foor loop iteration is a different snake in the generation
        for (size_t i = 0; i < snakesLen; i++)
        {
            Snake& snake = snakes.GetItem(i);
            int turnsWithoutEating = 0;

            int score = 0;
            int turnsLived = 0;

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
                    resetGame(snake, score, bestSnakesIndecies, worseBestSnakeIndex, worseBestSnakeScore, biggestGenScore, biggestGenScoreIndex, i, scores, genScoreSum, turnsLived, turnsWithoutEating, isPlaying);

                if (turnsWithoutEating > Constants::MAX_TURNS_WITHOUT_EATING)
                    std::cout << "Max turns reached. Index: " << i << "\n";

                turnsLived++;

                // Clear the old screen
                drawScreen(window, snake, apple, infoText, score, generation, genScoreSum, i + 1);
            } // end of while
        } // end of snake len for loop

        // Regenerating new snakes from the best scoring ones and starting a new generation
        if (generation != numOfGenerations)
            createNewGeneration(snakes,  bestSnakesIndecies);

        if (generation == numOfGenerations)
            snakes.GetItem(biggestGenScoreIndex).saveNN("bestScorer.nn");
    } // end of generation for loope
}

