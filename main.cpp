#include <SFML/Graphics.hpp>
#include "abd.h"
#include <iostream>
using namespace std;
using namespace sf;

int main()
{
    cout << prints() << endl;
    RenderWindow window(VideoMode(200, 200), "SFML works!");
    CircleShape shape(100.f);
    shape.setFillColor(Color::Red);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}