#pragma once
#include "NonShooter.h"
#include "Zombie.h"
#include <fstream>
class Wallnut : public NonShooter {
private:
	bool active;
	Clock moveClock;
	float speed = 0.03125;
	bool dead = false;
	bool half = false;
	Texture halfTexture;
	Texture deadTexture;
	bool deflecting = false;
	float deflectFactor;

public:
	Wallnut(Texture& tex, int columns, float pos[2], TextureManager* tm) {
		srand(time(0));
		this->sprite.setTexture(tex);
		this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
		this->position[0] = pos[0], this->position[1] = pos[1];
		this->health = 500;
		this->cost = 50;
		this->exists = true;
		this->active = false;
		this->moveClock.restart();
		this->TMptr = tm;
		this->anim = Animation(71, 71, columns);
		this->sprite.setPosition(xFactor + position[0] * 80, yFactor + position[1] * 96);
		this->halfTexture.loadFromFile("assets/Spritesheets/wallnut-half.png");
		this->deadTexture.loadFromFile("assets/Spritesheets/wallnut-dead.png");
	}

	void saveEverything(ofstream& file) {
		file.write(reinterpret_cast<char*>(&position[0]), sizeof(float));
		file.write(reinterpret_cast<char*>(&position[1]), sizeof(float));
		file.write(reinterpret_cast<char*>(&health), sizeof(int));
		file.write(reinterpret_cast<char*>(&exists), sizeof(bool));

		file.write(reinterpret_cast<char*>(&active), sizeof(bool));
		file.write(reinterpret_cast<char*>(&dead), sizeof(bool));
		file.write(reinterpret_cast<char*>(&half), sizeof(bool));
		file.write(reinterpret_cast<char*>(&deflecting), sizeof(bool));
		file.write(reinterpret_cast<char*>(&deflectFactor), sizeof(float));

		this->anim.saveEverything(file);
	}

	void readEverything(ifstream& file) {
		file.read(reinterpret_cast<char*>(&position[0]), sizeof(float));
		file.read(reinterpret_cast<char*>(&position[1]), sizeof(float));
		this->sprite.setPosition(xFactor + position[0] * 80, yFactor + position[1] * 96);


		file.read(reinterpret_cast<char*>(&health), sizeof(int));
		file.read(reinterpret_cast<char*>(&exists), sizeof(bool));

		file.read(reinterpret_cast<char*>(&active), sizeof(bool));

		file.read(reinterpret_cast<char*>(&dead), sizeof(bool));
		file.read(reinterpret_cast<char*>(&half), sizeof(bool));
		file.read(reinterpret_cast<char*>(&deflecting), sizeof(bool));
		file.read(reinterpret_cast<char*>(&deflectFactor), sizeof(float));

		this->anim.readEverything(file);

		if (this->active) {
			this->sprite = Sprite(this->TMptr->getTexture("spritesheet-wallnut-rolling"));
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 71, 0, 71, 71));
			this->anim.setDelay(60.f);
			this->moveClock.restart();
		}
		if (this->dead) {
			this->sprite = Sprite(this->TMptr->getTexture("wallnut-dead"));
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 71, 0, 71, 71));
		}
		if (this->half) {
			this->sprite = Sprite(this->TMptr->getTexture("wallnut-half"));
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 71, 0, 71, 71));
		}
	}

	void activate(Texture& tex) {
		if (this->active || this->moveClock.getElapsedTime().asMilliseconds() < 50) return;
		this->active = true;
		this->sprite = Sprite(tex);
		this->anim = Animation(71, 71, 38);
		this->anim.setDelay(60.0f);
		this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
		this->moveClock.restart();
	}

	virtual bool getDead() { return this->dead; }

	virtual void animate() {
		this->anim.animate(this->sprite);
		if (this->dead) {
			if (this->anim.getFrame() == 2) {
				this->dead = false;
			}
		}
	}


	void move(Zombie** zombies, int zombiesArrayIndex, Scoreboard* scoreboard) {
		if (!this->active) return;
		if (this->moveClock.getElapsedTime().asMilliseconds() <= 20) {
			return;
		}
		if (this->deflecting) {
			this->position[1] += this->deflectFactor;
			if (abs(this->position[1] - (int)this->position[1]) == 0) this->deflecting = false;
		}

		if (this->exists) {
			for (int i = 0; i < zombiesArrayIndex; i++) {
				if (!zombies[i]->isFlying() && zombies[i]->getExist() && (this->position[1] == zombies[i]->getPosition()[1]) && (this->position[0] == zombies[i]->getPosition()[0] || this->position[0] == zombies[i]->getPosition()[0] - 0.03125 || this->position[0] == zombies[i]->getPosition()[0] - 0.0625 || this->position[0] == zombies[i]->getPosition()[0] - 0.09375 || this->position[0] == zombies[i]->getPosition()[0] - 0.125 || this->position[0] == zombies[i]->getPosition()[0] - 0.15625 || this->position[0] == zombies[i]->getPosition()[0] - 0.1875 || this->position[0] == zombies[i]->getPosition()[0] - 0.21875 || this->position[0] == zombies[i]->getPosition()[0] - 0.25 || this->position[0] == zombies[i]->getPosition()[0] - 0.28125 || this->position[0] == zombies[i]->getPosition()[0] - 0.3125 || this->position[0] == zombies[i]->getPosition()[0] - 0.34375 || this->position[0] == zombies[i]->getPosition()[0] - 0.375 || this->position[0] == zombies[i]->getPosition()[0] - 0.40625 || this->position[0] == zombies[i]->getPosition()[0] - 0.4375 || this->position[0] == zombies[i]->getPosition()[0] - 0.46875 || this->position[0] == zombies[i]->getPosition()[0] - 0.5 || this->position[0] == zombies[i]->getPosition()[0] - 0.53125 || this->position[0] == zombies[i]->getPosition()[0] - 0.5625)) {
					if (!zombies[i]->getHeadFall()) zombies[i]->setHeadFall(true);
					zombies[i]->setExist(false);
					zombies[i]->makeDead();
					zombies[i]->getPainSound().play();
					scoreboard->addScore(20);
					if (rand() % 2) { // if 1 then go down
						if (this->position[1] + 1 <= 4) { // can go down
							this->deflectFactor = 0.0625;
						}
						else { // cannot go down, go up
							this->deflectFactor = -0.0625;
						}
					}
					else { // if 0 then go up
						if (this->position[1] - 1 >= 0) { // can go up
							this->deflectFactor = -0.0625;
						}
						else { // cannot go up, go down
							this->deflectFactor = 0.0625;
						}
					}
					this->deflecting = true;
					this->moveClock.restart();

					return;
				}
			}
			if (this->position[0] <= 8.5) {
				this->position[0] += this->speed;
			}
			else {
				cout << "Walnut died from overspeeding\n";
				this->exists = false;
				this->active = false;
			}
		}
		this->moveClock.restart();
	}

	void beEaten() {
		this->health -= 20;

		if (this->health == 20 && !this->half) {
			this->sprite = Sprite(this->halfTexture);
			this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
			this->anim = Animation(71, 71, 4);
			this->anim.setFrame(0);
			this->half = true;
		}

		if (this->health <= 0 && !this->dead) {
			this->exists = false;
			this->dead = true;
			this->sprite = Sprite(this->deadTexture);
			this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
			this->anim = Animation(71, 71, 3);
			this->anim.setFrame(0);
		}
	}

	virtual void draw(RenderWindow& window) {
		if (this->exists) {
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
		else if (this->dead) {
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
	}
};