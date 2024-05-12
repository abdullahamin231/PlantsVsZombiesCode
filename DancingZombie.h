#pragma once
#include "Zombie.h"
#include "TextureManager.h"

// 22 for dancing 1
// 27 for dancing 2
// 21 for dancing walk

class DancingZombie : public Zombie {
	Clock spawnClock;
	int spawnIndex[4];
	Clock danceClock;
	float danceDelay = 1 + rand() % 3; // 1 to 3 seconds random

public:
	DancingZombie(Texture& tex, int columns, float pos[2], TextureManager* tm, SoundManager* sm) {
		this->type = "dancing";
		this->SMptr = sm;
		this->TMptr = tm;
		this->sprite.setTexture(tex);
		this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
		this->speed = 0.09375;
		this->health = 80;
		this->exists = true;
		this->limit = 20;
		this->position[0] = pos[0], this->position[1] = pos[1];
		this->anim = Animation(166, 144, columns);
		this->moveClock.restart(), this->spawnClock.restart(), this->danceClock.restart();
		this->moveDelay = 180;
		this->anim.setDelay(70);
		this->xFactor += 45;
		for (int i = 0; i < 4; spawnIndex[i++] = -1);
		// head
		this->headSprite.setTexture(this->TMptr->getTexture("dancing-head"));
		this->headSprite.setTextureRect(IntRect(0, 0, 150, 186));
		this->headAnim = Animation(150, 186, 9);
	}

	virtual void animate() {
		this->anim.animate(this->sprite);
		if (this->headFall) {
			if (this->headAnim.getFrame() < this->headAnim.getColumns()) {
				this->headAnim.animate(this->headSprite);
			}
			if (this->headClock.getElapsedTime().asSeconds() >= 0.905) {
				this->headFall = false;
				this->headOnceFell = true;
			}
		}
	}

	void dance() {
		if (this->danceClock.getElapsedTime().asSeconds() < this->danceDelay) return;
		if (this->health <= this->limit) return; // don't dance if head is off
		if (this->blocked) return; // don't dance if blocked;

		this->danceClock.restart();
		this->danceDelay = 1 + rand() % 3; // now dance after some another interval

		int dancePos = rand() % 3;
		switch (dancePos) {
		case 0:
			changeTexture(this->TMptr->getTexture("dancing-walk-1"), 0, 21);
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			break;
		case 1:
			changeTexture(this->TMptr->getTexture("dancing-1"), 0, 22);
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			break;
		case 2:
			changeTexture(this->TMptr->getTexture("dancing-2"), 0, 27);
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			break;
		}
	}

	void checkHealth() {
		if (this->exists) {
			if (this->health == this->limit) {
				setHeadFall(true);
				if (this->state == "walk") {
					changeTexture(this->TMptr->getTexture("dancing-walk-2"), 0, 16);
				}
				else {
					changeTexture(this->TMptr->getTexture("dancing-eat-2"), 0, 10);
				}
				this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			}
			else if (this->health == 0) {
				this->exists = false;
				makeDead();
			}
		}
	}

	void makeDead() {
		if (this->exists == false && this->ashes != true) {
			this->changeTexture(this->TMptr->getTexture("dancing-die"), 0, 12);
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			this->dead = true, this->deadClock.restart();
			this->SMptr->playSound("zombie-fall");
		}
	}

	void handleDeath(RenderWindow& window) {
		if (!this->exists && this->dead) {
			if (this->deadClock.getElapsedTime().asSeconds() > 1.205) {
				this->dead = false;
			}
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
	}

	void move(Plant** plants, int plantsArrayIndex) {
		if (this->exists == false) return;

		if (this->moveClock.getElapsedTime().asMilliseconds() < this->moveDelay) return;
		handleFlicker();
		dance();

		if (this->blocked) {
			if (this->eatIndex != -1) {
				this->state = "eat";
				eat(plants[this->eatIndex]);
			}
			return;
		}


		if (rand() % 100 == 1) {
			//this->position[0] += (10 * this->speed);
			if (rand() % 2) {
				this->position[1] += 1;
				if (this->position[1] > 4) this->position[1] = 4;
			}
			else {
				this->position[1] -= 1;
				if (this->position[1] < 0) this->position[1] = 0;
			}
		}


		this->moveClock.restart();
		this->position[0] -= this->speed;
		this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);



		for (int i = 0; i < plantsArrayIndex; i++) {
			if (plants[i]->getExist()) {
				float* plantPos = plants[i]->getPosition();

				if (plantPos[1] == this->position[1]) {
					float dt = plantPos[0] - this->position[0];
					if (dt <= 0 && dt >= -0.6875) {
						this->blocked = true;

						if (this->health > this->limit) {
							this->changeTexture(this->TMptr->getTexture("dancing-eat-1"), 0, 11);
						}
						else {
							this->changeTexture(this->TMptr->getTexture("dancing-eat-2"), 0, 10);
						}

						this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
						this->eatIndex = i;
						return;
					}
				}
			}
		}
	}

	void eat(Plant* plant) {
		if (plant->getExist()) {
			if (plant->getEatClock().getElapsedTime().asMilliseconds() > 500) {
				plant->beEaten();
				plant->getEatClock().restart();

			}
		}
		else {
			this->blocked = false;

			this->state = "walk";

			if (this->health > this->limit) {
				this->changeTexture((*(this->TMptr))["dancing-walk-1"], 0, 21);
			}
			else {
				this->changeTexture((*(this->TMptr))["dancing-walk-2"], 0, 16);
			}

			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			this->eatIndex = -1;
		}
	}


	void spawnZombie(Zombie** zombies, int& zombiesArrayIndex, const int maxZombies) {
		if (this->exists == false) return;
		if (this->spawnClock.getElapsedTime().asSeconds() < 15) return;

		for (int i = 0; i < 4; i++) {
			if (spawnIndex[i] != -1) {
				if (zombies[spawnIndex[i]]->getExist()) {
					return;
				}
			}
		}
		// on right side of zombie
		if (zombiesArrayIndex + 1 < maxZombies) {
			float pos[2] = { this->position[0] + 1, this->position[1] };
			spawnIndex[0] = zombiesArrayIndex;
			zombies[zombiesArrayIndex++] = new NormalZombie(this->TMptr->getTexture("spritesheet-nZombWalk"), 22, pos, this->TMptr, this->SMptr);
		}

		// on lefet side of zombie
		if (zombiesArrayIndex + 1 < maxZombies && this->position[0] - 1 >= 0) {
			float pos[2] = { this->position[0] - 1, this->position[1] };
			spawnIndex[1] = zombiesArrayIndex;
			zombies[zombiesArrayIndex++] = new NormalZombie(this->TMptr->getTexture("spritesheet-nZombWalk"), 22, pos, this->TMptr, this->SMptr);
		}

		// on above
		if (zombiesArrayIndex + 1 < maxZombies && this->position[1] - 1 >= 0) {
			float pos[2] = { this->position[0], this->position[1] - 1 };
			spawnIndex[2] = zombiesArrayIndex;
			zombies[zombiesArrayIndex++] = new NormalZombie(this->TMptr->getTexture("spritesheet-nZombWalk"), 22, pos, this->TMptr, this->SMptr);
		}

		// on below
		if (zombiesArrayIndex + 1 < maxZombies && this->position[1] + 1 <= 4) {
			float pos[2] = { this->position[0], this->position[1] + 1 };
			spawnIndex[3] = zombiesArrayIndex;
			zombies[zombiesArrayIndex++] = new NormalZombie(this->TMptr->getTexture("spritesheet-nZombWalk"), 22, pos, this->TMptr, this->SMptr);
		}

		this->spawnClock.restart();

	}

	virtual void handleAshes(RenderWindow& window) {
		// show ashes only when this->exists = false i.e zombie is dead
		if (!this->exists && this->ashes) {
			if (this->ashesClock.getElapsedTime().asSeconds() > 2.05) {
				// there are 20 frames, each frame takes 100ms delay, added 0.05 as padding
				// set ashes to false after 2 seconds so nothing is drawn afterwards
				this->ashes = false;
			}
			this->sprite.setPosition(this->xFactor - 40 + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
	}

};