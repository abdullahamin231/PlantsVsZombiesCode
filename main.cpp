#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

#include "FontManager.h"
#include "TextureManager.h"
#include "Background.h"
#include "Inventory.h"

struct {
    int leftX = 255;
    int rightX = 995;
    int topY = 80;
    int bottomY = 574;

    bool valid(int x, int y) const {
        if (x >= leftX && x <= rightX && y >= topY && y <= bottomY) {
			return true;
		}
		return false;
    }
} gardenCords;


int main()
{
    RenderWindow window(VideoMode(1400, 600), "game");
    Background background;

    TextureManager TM;
    TM.addTexture("assets/images/i_cherry.png", "cherrybomb");
    TM.addTexture("assets/images/i_chomper.png", "chomper");
    TM.addTexture("assets/images/i_freeze.png", "freezer");
    TM.addTexture("assets/images/i_mine.png", "mine");
    TM.addTexture("assets/images/i_sunflower.png", "sunflower");
    TM.addTexture("assets/images/i_walnut.png", "walnut");
    TM.addTexture("assets/images/sun.png", "sun");
    TM.addTexture("assets/images/i_pea.png", "peashooter");


    Inventory Inv;
    Inv.addCard(TM.getTexture("cherrybomb"));
    Inv.addCard(TM.getTexture("walnut"));
    Inv.addCard(TM.getTexture("sunflower"));
    Inv.addCard(TM.getTexture("mine"));
    Inv.addCard(TM.getTexture("peashooter"));



    RectangleShape garden[5][9];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++) {
			garden[i][j].setSize(Vector2f(80, 96));
            garden[i][j].setFillColor(((i + j) % 2) == 0 ? Color(255, 255, 255, 50) : Color(255, 255, 255, 100));
			garden[i][j].setPosition(255 + j * 80, 80 + i * 96);
		}
    }
   

    Clock timer;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::E) {
                    window.close();
                }
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
					cout << "Mouse X: " << mouseY << " Mouse Y: " << mouseY << endl;
                    if (gardenCords.valid(mouseX, mouseY)) {
                        cout << (mouseX + 255) / 80 << " " << (mouseY + 80) / 96 << endl;
                    }
				}
			}
        }
        /*Vector2i localPosition = sf::Mouse::getPosition(window);
        cout << "Mouse X: " << localPosition.x << " Mouse Y: " << localPosition.y << endl;*/

        window.clear(Color::Red);

        window.draw(background.getSprite());
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 9; j++) {
				window.draw(garden[i][j]);
			}
		}
         //Inv.drawInventory(window);
        window.display();
    }

    return 0;
}

// Top Left of Garden : 255, 80
// Bottom Right of Garden : 995, 574