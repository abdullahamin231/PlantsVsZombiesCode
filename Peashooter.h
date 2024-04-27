#pragma once
#include "Shooter.h"
#include "Bullet.h"
#include "Zombie.h"

class Peashooter : public Shooter {
	Bullet bullet;

public:
	Peashooter(Texture& tex, int columns, float pos[2]) : bullet(pos[0], pos[1]) {
		this->sprite.setTexture(tex);
		this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
		this->position[0] = pos[0], this->position[1] = pos[1];
		this->health = 100;
		this->shooting = true;
		this->cost = 100;
		this->exists = true;
		this->anim = Animation(71, 71, columns);
		this->sprite.setPosition(xFactor + position[0] * 80, yFactor + position[1] * 96);
		this->bulletDelayClock.restart();
		this->startClock.restart();
	}

	void shoot(Zombie** zomb) {
		this->bullet.move(zomb);
		if (this->bullet.getExist() == false && this->bulletDelayClock.getElapsedTime().asSeconds() > 2) {
			this->bullet.setPosition(position[0], position[1]);
			this->bullet.setExist(true);
			this->bulletDelayClock.restart();
		}
	}

	void animate() {
		this->anim.animate(this->sprite);
	}

	void draw(RenderWindow& window) {
		if (this->exists) {
			if (this->bullet.getExist() == true && this->startClock.getElapsedTime().asSeconds() > 2) {
				this->bullet.draw(window);
			}
			window.draw(this->sprite);
		}
	}
};