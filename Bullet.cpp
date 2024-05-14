#include "Bullet.h"

Bullet::Bullet(int x, int y) {
	this->position[0] = x, this->position[1] = y;
	if (!texture.loadFromFile("assets/Bullets/peabullet.png")) cout << "Bullet not found\n";
	this->sprite.setTexture(this->texture);
	this->damage = 20;
	this->exists = false;
	this->bulletClock.restart();
	this->speed = 0.0625;
}

void Bullet::setHitSound(Sound sound) {
	this->hitSound = sound;
	this->hitSound.setPlayingOffset(Time(seconds(0.5)));
}

void Bullet::saveEverything(ofstream& file) {
	file.write(reinterpret_cast<char*>(&position[0]), sizeof(float));
	file.write(reinterpret_cast<char*>(&position[1]), sizeof(float));
	file.write(reinterpret_cast<char*>(&exists), sizeof(bool));
}

void Bullet::readEverything(ifstream& file) {
	file.read(reinterpret_cast<char*>(&position[0]), sizeof(float));
	file.read(reinterpret_cast<char*>(&position[1]), sizeof(float));
	this->sprite.setPosition(305 + this->position[0] * 80, 95 + this->position[1] * 96);

	file.read(reinterpret_cast<char*>(&exists), sizeof(bool));
	this->exists = exists;
}

void Bullet::changeSprite(Texture& tex) {
	this->sprite = Sprite(tex);
}

bool Bullet::getExist() { return exists; }

void Bullet::setExist(bool val) { exists = val; }

void Bullet::setPosition(float x, float y) {
	this->position[0] = x;
	this->position[1] = y;
}

void Bullet::move(Zombie** zombies, int zombiesArrayIndex, bool isSnow, Scoreboard* scoreboard) {
	if (this->bulletClock.getElapsedTime().asMilliseconds() <= 5) {
		return;
	}
	this->bulletClock.restart();

	if (this->exists) {
		for (int i = 0; i < zombiesArrayIndex; i++) {
			if (zombies[i]->getExist() == true && !zombies[i]->isFlying()) {
				float* zombiePos = zombies[i]->getPosition();
				if ((this->position[1] == zombiePos[1]) && (this->position[0] == zombiePos[0] || this->position[0] == zombiePos[0] - 0.03125 || this->position[0] == zombiePos[0] - 0.0625 || this->position[0] == zombiePos[0] - 0.09375 || this->position[0] == zombiePos[0] - 0.125 || this->position[0] == zombiePos[0] - 0.15625 || this->position[0] == zombiePos[0] - 0.1875 || this->position[0] == zombiePos[0] - 0.21875 || this->position[0] == zombiePos[0] - 0.25 || this->position[0] == zombiePos[0] - 0.28125 || this->position[0] == zombiePos[0] - 0.3125 || this->position[0] == zombiePos[0] - 0.34375 || this->position[0] == zombiePos[0] - 0.375 || this->position[0] == zombiePos[0] - 0.40625 || this->position[0] == zombiePos[0] - 0.4375 || this->position[0] == zombiePos[0] - 0.46875 || this->position[0] == zombiePos[0] - 0.5 || this->position[0] == zombiePos[0] - 0.53125 || this->position[0] == zombiePos[0] - 0.5625)) {
					this->exists = false;
					//this->hitSound.play();
					zombies[i]->setFlicker(true);
					zombies[i]->getSprite().setColor(Color(255, 255, 255, 255 * 0.5));

					zombies[i]->reduceHealth(this->damage);
					zombies[i]->checkHealth();

					if (zombies[i]->getType() == "dancing") {
						scoreboard->addScore(50);
					}
					else if (zombies[i]->getType() == "football") {
						scoreboard->addScore(20);
					}
					else if (zombies[i]->getType() == "normal") {
						scoreboard->addScore(10);
					}
					else {
						scoreboard->addScore(0);
					}

					this->bulletClock.restart();
					cout << "Zombie " << i << " health: " << zombies[i]->getHealth() << endl;
					if (isSnow) {
						zombies[i]->setMoveDelay(500);
					}
					//this->hitSound.stop();
					return;
				}
			}
		}
		if (this->position[0] <= 8.5) {
			this->position[0] += this->speed;
		}
		else {
			this->exists = false;
		}
	}
}

void Bullet::draw(RenderWindow& window) {
	if (this->exists) {
		this->sprite.setPosition(305 + this->position[0] * 80, 95 + this->position[1] * 96);
		window.draw(this->sprite);
	}
}
