#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Animation.h"
#include "Zombie.h"
using namespace sf;

class Bullet {
	Texture texture;
	Sprite sprite;
	float position[2];
	int damage;
	Clock bulletClock;
	bool exists;
	float speed;

public:
	Bullet(int x = 0, int y = 0) {
		this->position[0] = x, this->position[1] = y;
		if (!texture.loadFromFile("assets/Bullets/peabullet.png")) cout << "Bullet not found\n";
		this->sprite.setTexture(this->texture);
		this->damage = 1;
		this->exists = true;
		this->bulletClock.restart();
		this->speed = 0.0625;
	}

	void changeSprite(Texture& tex) {
		this->sprite = Sprite(tex);
	}

	bool getExist() { return exists; }
	void setExist(bool val) { exists = val; }

	void setPosition(float x, float y) {
		this->position[0] = x;
		this->position[1] = y;
	}

	void move(Zombie** zomb, int zombiesArrayIndex) {
		if (bulletClock.getElapsedTime().asMilliseconds() <= 5) {
			return;
		}
		if (exists) {
			for (int i = 0; i < zombiesArrayIndex; i++) {
				if (zomb[i]->getExist() && (this->position[1] == zomb[i]->getPosition()[1]) && (this->position[0] == zomb[i]->getPosition()[0] || this->position[0] == zomb[i]->getPosition()[0] - 0.03125 || this->position[0] == zomb[i]->getPosition()[0] - 0.0625 || this->position[0] == zomb[i]->getPosition()[0] - 0.09375 || this->position[0] == zomb[i]->getPosition()[0] - 0.125 || this->position[0] == zomb[i]->getPosition()[0] - 0.15625 || this->position[0] == zomb[i]->getPosition()[0] - 0.1875 || this->position[0] == zomb[i]->getPosition()[0] - 0.21875 || this->position[0] == zomb[i]->getPosition()[0] - 0.25 || this->position[0] == zomb[i]->getPosition()[0] - 0.28125 || this->position[0] == zomb[i]->getPosition()[0] - 0.3125 || this->position[0] == zomb[i]->getPosition()[0] - 0.34375 || this->position[0] == zomb[i]->getPosition()[0] - 0.375 || this->position[0] == zomb[i]->getPosition()[0] - 0.40625 || this->position[0] == zomb[i]->getPosition()[0] - 0.4375 || this->position[0] == zomb[i]->getPosition()[0] - 0.46875 || this->position[0] == zomb[i]->getPosition()[0] - 0.5 || this->position[0] == zomb[i]->getPosition()[0] - 0.53125 || this->position[0] == zomb[i]->getPosition()[0] - 0.5625)) {
					this->exists = false;
					zomb[i]->reduceHealth();
					if (zomb[i]->getHealth() == 0) zomb[i]->setExist(false);
					this->bulletClock.restart();
					cout << "Zombie " << i << " health: " << zomb[i]->getHealth() << endl;
					return;
				}
			}
			if (this->position[0] <= 8.5) {
				this->position[0] += this->speed;
			}
			else {
				this->exists = false;
			}
		}
		this->bulletClock.restart();
	}

	void draw(RenderWindow& window) {
		if (this->exists) {
			this->sprite.setPosition(305 + position[0] * 80, 95 + position[1] * 96);
			window.draw(sprite);
		}
	}
};