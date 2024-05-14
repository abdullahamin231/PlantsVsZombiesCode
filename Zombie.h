#ifndef ZOMBIES_H
#define ZOMBIES_H
#include "Animation.h"
#include "TextureManager.h"
#include "Plant.h"

class Zombie {
protected:
	Sprite sprite;
	Animation anim;
	float position[2];
	int health;
	float speed;
	float xFactor, yFactor;
	bool exists;
	Clock moveClock;
	float moveDelay = 250;
	TextureManager* TMptr;
	bool blocked = false;
	int eatIndex = -1;

	Sprite headSprite;
	Animation headAnim;
	Clock headClock;
	bool headFall = false;
	bool headOnceFell = false;

	float limit;


	string state;
	bool flicker = false;
	Clock flickerClock;

	bool ashes = false;
	Clock ashesClock;

	bool dead = false;
	Clock deadClock;

	SoundManager* SMptr;

	Clock growlClock;
	float growlDelay = 3.0f;
	SoundBuffer growlBuffer;
	Sound growlSound;

	SoundBuffer painBuffer;
	Sound painSound;

	SoundBuffer spawnBuffer;
	Sound spawnSound;

	string type;
public:
	Zombie() {
		this->xFactor = 185;
		this->yFactor = 32;
		this->state = "walk";
		this->loadBuffer();

		this->painBuffer.loadFromFile("assets/sounds/zombie/bullethitzombie.mp3");
		this->painSound.setBuffer(this->painBuffer);

		this->growlSound.setBuffer(this->growlBuffer);
	}

	virtual void loadBuffer() {
		if (rand() % 2) {
			this->growlBuffer.loadFromFile("assets/sounds/zombie/lowgroan.ogg");
		}
		else {
			this->growlBuffer.loadFromFile("assets/sounds/zombie/lowgroan2.ogg");
		}
	}

	virtual Sound& getSpawnSound() {
		return this->spawnSound;
	}


	Sound& getPainSound() {
		this->painSound.setVolume(50);
		return this->painSound;
	}
	/*float getSpeed() { return this->speed; }
	void setSpeed(float val) { this->speed = val; }*/

	virtual void saveEverything(ofstream& file) = 0;
	virtual void readEverything(ifstream& file) = 0;

	string getType() { return this->type; }

	void setAnimDelay(float val) { this->anim.setDelay(val); }
	void setMoveDelay(float val) { this->moveDelay = val; }
	float* getPosition() {
		return this->position;
	}
	bool getExist() { return this->exists; }

	bool getDead() { return this->dead; }
	virtual float getLimit() { return this->limit; }

	void changeTexture(Texture& tex, int frame = 0, int columns = 21) {
		this->sprite = Sprite(tex);
		this->anim = Animation(166, 144, columns);
		this->anim.setFrame(frame);
	}

	virtual void animate() {
		this->anim.animate(this->sprite);
		if (this->headFall) {
			if (this->headAnim.getFrame() < this->headAnim.getColumns()) {
				this->headAnim.animate(this->headSprite);
			}
			if (this->headAnim.getFrame() == this->headAnim.getColumns() - 1) {
				this->headFall = false;
				this->headOnceFell = true;
			}
		}
	}
	virtual bool isFlying() { return false; }
	void setFlicker(bool value) {
		this->flicker = value, this->flickerClock.restart();
	}

	void setHeadFall(bool val) {
		this->headFall = val;
		this->headOnceFell = val;
		this->headClock.restart();
		this->headSprite.setPosition(this->xFactor + 50 + this->position[0] * 80, this->yFactor - 10 + this->position[1] * 96);
	}

	bool getHeadFall() {
		return this->headOnceFell;
	}

	virtual void spawnZombie(Zombie** zombies, int& zombiesArrayIndex, const int maxZombies, string* types) {}

	// changes sprite texture to ashes
	// restarts ashes clock
	virtual void setAshes(bool value) {
		this->changeTexture(this->TMptr->getTexture("spritesheet-zombieAshes"), 0, 20);
		this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
		this->ashes = value, this->ashesClock.restart();
	}

	//
	virtual void checkHealth() {}
	Sprite& getSprite() { return this->sprite; }
	//
	virtual void draw(RenderWindow& window) {

		if (this->exists) {
			// Draw
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);

			window.draw(this->sprite);
		}

		handleAshes(window);

		handleDeath(window);
		if (this->headFall) {
			window.draw(this->headSprite);
		}
	}

	virtual void makeDead() {
		if (this->exists == false && this->ashes != true) {
			this->changeTexture(this->TMptr->getTexture("spritesheet-zombieDeath"), 0, 10);
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			this->dead = true, this->deadClock.restart();
			this->SMptr->playSound("zombie-fall");
		}
	}

	virtual void handleDeath(RenderWindow& window) {
		if (!this->exists && this->dead) {
			if (this->deadClock.getElapsedTime().asSeconds() > 1.05 || this->anim.getFrame() == this->anim.getColumns() - 1) {
				this->dead = false;
			}
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
	}

	void handleFlicker() {
		if (!this->exists) return;
		if (this->flicker && this->flickerClock.getElapsedTime().asMilliseconds() > 50) {
			this->flicker = false;
			this->sprite.setColor(Color(255, 255, 255, 255));
		}
	}

	virtual void handleAshes(RenderWindow& window) {
		// show ashes only when this->exists = false i.e zombie is dead
		if (!this->exists && this->ashes) {
			if (this->ashesClock.getElapsedTime().asSeconds() > 2.05 || this->anim.getFrame() == this->anim.getColumns() - 1) {
				// there are 20 frames, each frame takes 100ms delay, added 0.05 as padding
				// set ashes to false after 2 seconds so nothing is drawn afterwards
				this->ashes = false;
			}
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
	}

	void setExist(bool val) {
		this->exists = val;
	}

	int getHealth() { return this->health; }

	void reduceHealth(int damage) {
		this->health -= damage;
	}

	virtual void growl(int i) {
		if (!this->exists) return;
		if (this->growlClock.getElapsedTime().asSeconds() > this->growlDelay) {
			cout << "Zombie[" << i << "] is growling\n";
			this->growlSound.setVolume(60);
			this->growlSound.play();
			this->growlDelay = 4 + rand() % 7;
			this->growlClock.restart();
		}
	}


	// should be specific for each zombie
	virtual void move(Plant** plants, int plantsArrayIndex) = 0;
	virtual void eat(Plant* plant) {}
	virtual ~Zombie() {}
};

#endif