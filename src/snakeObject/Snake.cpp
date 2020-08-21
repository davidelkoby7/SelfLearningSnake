#include "Snake.h"

Snake::Snake()
{
    this->length = 1;
    this->direction = Constants::SNAKE_LEFT;

    this->r = GeneralFunctions::RandomInt(0, 255);
    this->g = GeneralFunctions::RandomInt(0, 255);
    this->b = GeneralFunctions::RandomInt(0, 255);

    sf::RectangleShape* headRec = new sf::RectangleShape(sf::Vector2f(Constants::TILE_SIZE, Constants::TILE_SIZE));
    headRec->setPosition(Constants::SCREEN_WIDTH / 2, Constants::SCREEN_HEIGHT / 2);
    headRec->setFillColor(sf::Color(this->r, this->g, this->b));
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

sf::RectangleShape* Snake::getHead()
{
    return this->nodes.GetItem(this->length - 1);
}

void Snake::updateVelocity()
{
    this->snakeVX = 0;
    this->snakeVY = 0;

    if (this->direction == Constants::SNAKE_UP)
        this->snakeVY = -1 * Constants::SNAKE_SPEED;
    else if (this->direction == Constants::SNAKE_DOWN)
        this->snakeVY = Constants::SNAKE_SPEED;
    else if (this->direction == Constants::SNAKE_LEFT)
        this->snakeVX = -1 * Constants::SNAKE_SPEED;
    else if (this->direction == Constants::SNAKE_RIGHT)
        this->snakeVX = Constants::SNAKE_SPEED;
}

void Snake::move()
{
    this->updateVelocity();

    for (size_t i = 0; i < this->length - 1; i++)
    {
         sf::RectangleShape* currNode = this->nodes.GetItem(i);
         sf::RectangleShape* nextNode = this->nodes.GetItem(i + 1);
         currNode->setPosition(nextNode->getPosition());
    }

    sf::RectangleShape* head = this->getHead();
    head->setPosition(head->getPosition().x + snakeVX, head->getPosition().y + snakeVY);
}

void Snake::setDirection(const char& direction)
{
    this->direction = direction;
}

void Snake::grow()
{
    this->updateVelocity();
    sf::RectangleShape* head = this->getHead();

    sf::RectangleShape* newNode = new sf::RectangleShape(sf::Vector2f(Constants::TILE_SIZE, Constants::TILE_SIZE));
    newNode->setFillColor(sf::Color(this->r, this->g, this->b));

    newNode->setPosition(head->getPosition().x + snakeVX, head->getPosition().y + snakeVY);
    this->nodes.AddItem(newNode);
    this->length++;
}

bool Snake::isEatingItself()
{
    sf::RectangleShape* head = this->getHead();
    for (size_t i = 0; i < this->length - 1; i++)
    {
        sf::RectangleShape* currNode = this->nodes.GetItem(i);
        if (currNode->getGlobalBounds().intersects(head->getGlobalBounds()) == true)
            return true;
    }
    return false;
}

bool Snake::isOutOfBounds()
{
    sf::RectangleShape* head = this->getHead();
    float headX = head->getPosition().x;
    float headY = head->getPosition().y;
    if (headX < 0 || headY < 0)
        return true;
    if (headX > Constants::SCREEN_WIDTH - Constants::TILE_SIZE || headY > Constants::SCREEN_HEIGHT - Constants::TILE_SIZE)
        return true;
    return false;
}

