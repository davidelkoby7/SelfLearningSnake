#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML");
    window.setFramerateLimit(3);

    sf::Event event;
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

        // Clear the old screen
        window.clear(sf::Color(0 ,0, 0));

        // Drawing stuff

        // Display the rendered stuff
        window.display();
    }
}

