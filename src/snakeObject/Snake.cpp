#include "Snake.h"

Snake::Snake()
{
    reset("None");
}

Snake::Snake(const char* nnPath)
{
    reset(nnPath);
}

void Snake::reset(const char* nnPath)
{
    // Removing the older nodes if any
    size_t nodesLength = nodes.GetLength();
    for (size_t i = 0; i < nodesLength; i++)
        nodes.RemoveItem(0);

    this->length = 1;
    this->direction = Constants::SNAKE_LEFT;

    // Setting the snake's color
    this->r = GeneralFunctions::RandomInt(0, 255);
    this->g = GeneralFunctions::RandomInt(0, 255);
    this->b = GeneralFunctions::RandomInt(0, 255);

    // Creating the first head node
    sf::RectangleShape* headRec = new sf::RectangleShape(sf::Vector2f(Constants::TILE_SIZE, Constants::TILE_SIZE));
    headRec->setPosition(Constants::SCREEN_WIDTH / 2, Constants::SCREEN_HEIGHT / 2);
    headRec->setFillColor(sf::Color(this->r, this->g, this->b));
    this->nodes.AddItem(headRec);

    if (nnPath == "None")
    {
        // Reseting a neural network for the snake. The network will be made of:
        // - 6 inputs: dx, dy to apple and allowed distance in each direction without dying
        // - 5 hidden neurons (1 layer)
        // - 4 outputs - one for each direction
        Utils::DynamicArray<int> numOfNeuronsPerLayer;
        numOfNeuronsPerLayer.AddItem(6);
        numOfNeuronsPerLayer.AddItem(16);
        numOfNeuronsPerLayer.AddItem(16);
        numOfNeuronsPerLayer.AddItem(4);
        this->neuralNetwork = new NeuralNetwork(numOfNeuronsPerLayer, ActivationFunctions::LeakyReLU, ActivationFunctions::LeakyReLUDerivative);
    }
    else
    {
        this->neuralNetwork = new NeuralNetwork(nnPath, ActivationFunctions::LeakyReLU, ActivationFunctions::LeakyReLUDerivative);
    }
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

NeuralNetwork* Snake::getNN()
{
    return this->neuralNetwork;
}

sf::RectangleShape* Snake::getHead()
{
    return this->nodes.GetItem(this->length - 1);
}

void Snake::saveNN()
{
    this->neuralNetwork->SaveToFile("Snake.nn");
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

double Snake::getObstacleDistance(const char& direction)
{
    int mx, my;
    float minDistance;

    sf::RectangleShape* head = this->getHead();
    float headX = head->getPosition().x;
    float headY = head->getPosition().y;

    // Initializing the min distance to the distance to the walls (worst case scenario)
    if (direction == Constants::SNAKE_UP)
    {
        mx = 0;
        my = -1;
        minDistance = headY;
    }
    else if (direction == Constants::SNAKE_DOWN)
    {
        mx = 0;
        my = 1;
        minDistance = Constants::SCREEN_HEIGHT - headY - Constants::TILE_SIZE;
    }
    else if (direction == Constants::SNAKE_LEFT)
    {
        mx = -1;
        my = 0;
        minDistance = headX;
    }
    else if (direction == Constants::SNAKE_RIGHT)
    {
        mx = 1;
        my = 0;
        minDistance = Constants::SCREEN_WIDTH - headX - Constants::TILE_SIZE;
    }

    // Checking if there is a snake node between the head and the node
    for (size_t i = 0; i < this->length - 1; i++)
    {
        sf::RectangleShape* currNode = this->nodes.GetItem(i);
        float currNodeX = currNode->getPosition().x;
        float currNodeY = currNode->getPosition().y;

        if ((currNodeY == headY && mx != 0) || (currNodeX == headX && my != 0))
        {
            double currDistance = mx * (currNodeX - headX) + my * (currNodeY- headY) - Constants::TILE_SIZE;
            if (currDistance < minDistance && currDistance >= 0)
                minDistance = currDistance;
        }
    }

    return minDistance;
}

void Snake::autoMove(const int& appleX, const int& appleY)
{
    sf::RectangleShape* head = this->getHead();
    float headX = head->getPosition().x;
    float headY = head->getPosition().y;

    // Building the input layer
    Utils::DynamicArray<double> inputs(6);

    // First 4 neurons - min distance to obstacles in the 4 directions (and normalizing the value to be between 0 and 1)
    inputs.SetItem(0, this->getObstacleDistance(Constants::SNAKE_UP) / Constants::SCREEN_HEIGHT);
    inputs.SetItem(1, this->getObstacleDistance(Constants::SNAKE_DOWN) / Constants::SCREEN_HEIGHT);
    inputs.SetItem(2, this->getObstacleDistance(Constants::SNAKE_LEFT) / Constants::SCREEN_WIDTH);
    inputs.SetItem(3, this->getObstacleDistance(Constants::SNAKE_RIGHT) / Constants::SCREEN_WIDTH);

    // Last 2 neurons - dx and dy for the apple's position (and normalizing the value to be between 0 and 1)
    inputs.SetItem(4, (appleX - headX) / Constants::SCREEN_WIDTH);
    inputs.SetItem(5, (appleY - headY) / Constants::SCREEN_HEIGHT);

    // Calculating the next move
    this->neuralNetwork->SetInputLayer(inputs);
    this->neuralNetwork->PropagateForward();
    int mostActiveIndex = this->neuralNetwork->GetMostActiveNeuronIndex();

    /*
    std::cout << "~~~ Inputs ~~~\n";
    inputs.Print();
    std::cout << "~~~ Inputs ~~~\n\n\n";

    std::cout << "Most active index: " << mostActiveIndex << "\n";
    std::cout << "~~~ Outputs ~~~\n";
    Utils::DynamicArray<Neuron*>* outputs = this->neuralNetwork->GetOutputLayer();
    for (size_t i = 0; i < outputs->GetLength(); i++)
    {
        std::cout << outputs->GetItem(i)->GetValue() << ", ";
    }
    std::cout << "\n~~~ Outputs ~~~\n\n\n";
    */

    // Setting the direction to it and moving
    this->direction = mostActiveIndex;
    this->move();
}

void evolve()
{
}

