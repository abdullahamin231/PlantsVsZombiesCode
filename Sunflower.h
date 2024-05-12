#pragma once
#include "NonShooter.h"

class Sunflower : public NonShooter {
	//Animation sunAnim;
	Sprite sunSprite;
	bool showSun = false;
	Clock showSunClock;

public:
	Sunflower(Texture& tex, Texture& sunT, int columns, float pos[2]) {
		this->sprite.setTexture(tex);
		this->sprite.setTextureRect(IntRect(0, 0, 71, 71));
		this->position[0] = pos[0], this->position[1] = pos[1];
		this->health = 100;
		this->cost = 50;
		this->exists = true;
		this->anim = Animation(71, 71, columns);
		this->sprite.setPosition(xFactor + position[0] * 80, yFactor + position[1] * 96);
		this->anim.setDelay(80.f);

		// Everything sun related
		//this->sunAnim = Animation(71, 71, 22);
		this->sunSprite.setTexture(sunT);
		this->sunSprite.setScale(0.7, 0.7);
		this->sunSprite.setPosition(xFactor + 30 + position[0] * 80, yFactor + position[1] * 96 - 10);
		this->showSunClock.restart();

	}

	void saveEverything(ofstream& file) {
		file.write(reinterpret_cast<char*>(&position[0]), sizeof(float));
		file.write(reinterpret_cast<char*>(&position[1]), sizeof(float));
		file.write(reinterpret_cast<char*>(&health), sizeof(int));
		file.write(reinterpret_cast<char*>(&exists), sizeof(bool));
		this->anim.saveEverything(file);

		file.write(reinterpret_cast<char*>(&showSun), sizeof(bool));
	}

	void readEverything(ifstream& file) {
		file.read(reinterpret_cast<char*>(&position[0]), sizeof(float));
		file.read(reinterpret_cast<char*>(&position[1]), sizeof(float));
		this->sprite.setPosition(xFactor + position[0] * 80, yFactor + position[1] * 96);
		this->sunSprite.setPosition(xFactor + 30 + position[0] * 80, yFactor + position[1] * 96 - 10);

		file.read(reinterpret_cast<char*>(&health), sizeof(int));
		this->health = health;

		file.read(reinterpret_cast<char*>(&exists), sizeof(bool));
		this->exists = exists;

		this->anim.readEverything(file);

		file.read(reinterpret_cast<char*>(&showSun), sizeof(bool));
	}

	void generateSun() {
		if (this->showSunClock.getElapsedTime().asSeconds() < 10.0) return;
		if (this->exists) {
			this->showSun = true;
			this->showSunClock.restart();
		}
	}

	void clickSun(int& sunCount, SoundManager* SMptr) {
		if (this->showSun) {
			this->showSun = false;
			sunCount += 50;
			SMptr->playSound("sunclick");
			cout << "Suncount is now: " << sunCount << endl;
			this->showSunClock.restart();
		}
	}

	void animate() {
		if (this->exists) {
			this->anim.animate(this->sprite);
		}
		//if (this->showSun) this->sunAnim.animate(this->sunSprite);
	}

	void draw(RenderWindow& window) {
		if (this->exists) {
			window.draw(this->sprite);
		}
		if (this->exists && this->showSun) window.draw(this->sunSprite);
	}
};