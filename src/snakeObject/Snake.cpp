#include "Snake.h"

Snake::Snake()
{
    this->headX = Constants::SCREEN_WIDTH / 2;
    this->headY = Constants::SCREEN_HEIGHT / 2;

    this->length = 1;
    this->direction = Constants::SNAKE_LEFT;

    sf::RectangleShape* headRec = new sf::RectangleShape(sf::Vector2f(Constants::TILE_SIZE, Constants::TILE_SIZE));
    headRec->setPosition(Constants::SCREEN_WIDTH / 2, Constants::SCREEN_HEIGHT / 2);
    headRec->setFillColor(sf::Color(0, 255, 0));
    this->nodes.AddItem(headRec);
}

Utils::DynamicArray<sf::RectangleShape*>* Snake::getNodes()
{
    return &(this->nodes);
}

int Snake::getLength() const
{
    return this->length;
}

char Snake::getDirection() const
{
    return this->direction;
}

void Snake::move()
{
    float snakeVX = 0;
    float snakeVY = 0;

    if (this->direction == Constants::SNAKE_UP)
        snakeVY = -1 * Constants::SNAKE_SPEED;
    else if (this->direction == Constants::SNAKE_DOWN)
        snakeVY = Constants::SNAKE_SPEED;
    else if (this->direction == Constants::SNAKE_LEFT)
        snakeVX = -1 * Constants::SNAKE_SPEED;
    else if (this->direction == Constants::SNAKE_RIGHT)
        snakeVX = Constants::SNAKE_SPEED;

    for (size_t i = this->getLength(); i > 1; i--)
    {
         sf::RectangleShape* currNode = this->nodes.GetItem(i);
         sf::RectangleShape* nextNode = this->nodes.GetItem(i - 1);
         currNode->setPosition(nextNode->getPosition());
    }

    sf::RectangleShape* head = this->nodes.GetItem(0);
    head->setPosition(head->getPosition().x + snakeVX, head->getPosition().y + snakeVY);
}

void Snake::setDirection(const char& direction)
{
    this->direction = direction;
}

