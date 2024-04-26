#pragma once
#include "Shooter.h"
#include "Bullet.h"

class Repeater : public Shooter {
	Bullet bullet[2];

public:
	Repeater(Texture& tex, int columns, float pos[2]) {
		this->sprite.setTexture(tex);
		this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
		this->position[0] = pos[0], this->position[1] = pos[1];
		this->health = 100;
		shooting = true;
		this->cost = 100;
		this->exists = true;
		this->anim = Animation(71, 71, columns);
		this->sprite.setPosition(xFactor + position[0] * 80, yFactor + position[1] * 96);
		bulletDelayClock.restart();

		bullet[0].setPosition(position[0], position[1]);
		bullet[1].setPosition(position[0] - 0.5, position[1]);
		startClock.restart();
	}

	void shoot() {
		bullet[0].move();
		bullet[1].move();
		if ((bullet[0].getExist() == false || bullet[1].getExist() == false) && bulletDelayClock.getElapsedTime().asSeconds() > 2) {
			bullet[0].setPosition(this->position[0], this->position[1]);
			bullet[0].setExist(true);

			bullet[1].setPosition(position[0] - 0.5, position[1]);
			bullet[1].setExist(true);
			bulletDelayClock.restart();
		}
	}

	void animate() {
		this->anim.animate(this->sprite);
	}

	void draw(RenderWindow& window) {
		if (exists) {
			if (bullet[0].getExist() && startClock.getElapsedTime().asSeconds() > 2) {
				bullet[0].draw(window);
			}
			if (bullet[1].getExist() && startClock.getElapsedTime().asSeconds() > 2) {
				bullet[1].draw(window);
			}
			window.draw(this->sprite);
		}
	}
};