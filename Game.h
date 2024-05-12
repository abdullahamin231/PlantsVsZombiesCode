#pragma once

#include "PlantFactory.h"
#include "SoundManager.h"
#include "ZombieFactory.h"
#include "fstream"
#include "FontManager.h"
#include "TextureManager.h"
#include "Background.h"
#include "Inventory.h"
#include "Life.h"
#include "Garden.h"
#include "FallingSun.h"
#include "Menu.h"
#include <cmath>

#include "BeginnersGarden.h"
#include "FullGarden.h"
#include "NightGarden.h"
#include "LimitedGarden.h"
#include "GameOver.h"

class Game {
	// window
	RenderWindow window;
	TextureManager TM;
	SoundManager SM;
	FontManager FM;
	Background background;
	Inventory Inv;

	GameOver gameover;

	int sunCount = 100;
	Text sunCountText;


	PlantFactory PF;
	ZombieFactory ZF;

	LawnMower* lawnmowers[5]{ nullptr };
	float lawnMowerPos[2] = { -1, 0 };

	Life lives;

	FallingSun sun;


	Text TimeText;
	string timeString;

	Menu menu;


	bool showMenu = true;
	bool showHighScores = false;
	bool quit = false;
	bool restarted = false;
	bool hasStarted = false;
	bool gameOver = false;

	int highScores[10] = {};
	Text HighScores[10], heading;
	Sprite medals[3];

	// time handling things
	float gameTime;
	Clock* runClock = nullptr;
	float remainingTime = 120;
	//float remainingTime = 12; // for testing
	Level** levels = new Level * [4];
	int levelIndex = 0;

public:
	Game() : window(VideoMode(1400, 600), "game"), background(&TM), Inv(&TM, &SM), PF(&SM, &TM), ZF(&TM, &SM), menu(&TM, &FM) {


		levels[0] = new BeginnersGarden{ background, &TM, &FM, &SM, runClock, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		levels[1] = nullptr;
		levels[2] = nullptr;
		levels[3] = nullptr;

		medals[0].setTexture(this->TM.getTexture("gold"));
		medals[1].setTexture(this->TM.getTexture("silver"));
		medals[2].setTexture(this->TM.getTexture("bronze"));

		medals[0].setScale(0.2, 0.2);
		medals[1].setScale(0.15, 0.15);
		medals[2].setScale(0.12, 0.12);

		srand((unsigned)time(0));
		//this->runClock.restart();
		this->TimeText.setPosition(1230, 550);
		this->TimeText.setFont(FM[0]);
		this->TimeText.setFillColor(Color::Black);
		this->TimeText.setCharacterSize(36);

		this->sunCountText.setFont(FM[0]);
		this->sunCountText.setString(to_string(this->sunCount));
		this->sunCountText.setCharacterSize(24);
		this->sunCountText.setPosition(86, 62);
		this->sunCountText.setFillColor(Color::Black);

		for (int i = 0; i < 5; i++) {
			this->lawnMowerPos[1] = i;
			this->lawnmowers[i] = new LawnMower(&TM, this->lawnMowerPos);

		}

	}

	void restartGame() {
		this->restarted = false, this->showMenu = false, this->showHighScores = false; // maybe redundant work

		this->menu.setMenuIndex(0);
		this->remainingTime = 120;
		this->runClock = new Clock;
		this->TimeText.setFillColor(Color::Black);
		this->sunCount = 100;
		this->PF.reset();
		this->ZF.reset();
		this->lives.reset();
		this->sun.reset();
		this->Inv.reset();

		for (int i = 0; i < 5; i++) {
			this->lawnMowerPos[1] = i;
			this->lawnmowers[i] = new LawnMower(&TM, this->lawnMowerPos);
		}

		if (this->levels[0] != nullptr) delete this->levels[0];
		levels[0] = new BeginnersGarden{ background, &TM, &FM, &SM, runClock, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		if (this->levels[1] != nullptr) delete this->levels[1];
		levels[1] = nullptr;
		if (this->levels[2] != nullptr) delete this->levels[2];
		levels[2] = nullptr;
		if (this->levels[3] != nullptr) delete this->levels[3];
		levels[3] = nullptr;
		this->levelIndex = 0;
	}

	void updateRound() {
		this->runClock->restart();
		this->levelIndex++;
		this->remainingTime = 120;
		//this->remainingTime = 60; //for testing
		this->TimeText.setFillColor(Color::Black);
		this->sun.reset();
		this->sunCount = 100;

		if (this->levelIndex > 3) this->window.close();

		if (levels[levelIndex] == nullptr && levelIndex == 1) {
			levels[levelIndex] = new FullGarden{ background, &PF, &ZF, &Inv, &TM, &FM, &SM, runClock, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		}
		else if (levels[levelIndex] == nullptr && levelIndex == 2) {
			levels[levelIndex] = new NightGarden{ background, &PF, &ZF, &Inv, &TM, &FM, &SM, runClock, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		}
		else if (levels[levelIndex] == nullptr && levelIndex == 3) {
			levels[levelIndex] = new LimitedGarden{ background, &PF, &ZF, &Inv, &TM, &FM, &SM, runClock, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		}
	}

	void drawEverything() {
		levels[levelIndex]->drawEverything(this->window, this->background, &Inv, sunCount, &PF, &ZF, lawnmowers, lives, &sun, sunCountText);
		this->window.draw(this->TimeText);
	}


	void updateEverything() {
		levels[levelIndex]->updateEverything(&PF, &ZF, lawnmowers, lives, sun);
		calculateTime();
		this->TimeText.setString("TIME: " + this->timeString);

		if (this->gameTime <= 0) {
			this->updateRound();
		}

		if (this->lives.livesGone()) {
			this->gameover.restartClock();
			this->gameOver = true;
		}
	}


	void sortScores() {
		bool swapped;
		for (int i = 0; i < 9; i++) {
			swapped = false;
			for (int j = 0; j < 10 - i - 1; j++) {
				if (this->highScores[j] < this->highScores[j + 1]) {
					swap(this->highScores[j], this->highScores[j + 1]);
					swapped = true;
				}
			}

			if (swapped == false)
				break;
		}
	}


	void initHighScores() {
		ifstream scores("highscores.txt");
		if (!scores.is_open()) {
			cerr << "error" << endl;
			return;
		}
		for (int i = 0; i < 10; i++) {
			scores >> highScores[i];
		}
		scores.close();

		sortScores();
		heading.setFont(FM[0]);
		heading.setCharacterSize(120);
		heading.setFillColor(Color::White);
		heading.setString("TOP 10 HIGHSCORES");
		heading.setPosition(30, -10);
		int pos = 110;
		for (int i = 0; i < 10; i++) {
			if (i == 0 || i == 1 || i == 2) {
				medals[i].setPosition(420, pos + i * 4);
				HighScores[i].setCharacterSize(48);
			}
			else {
				HighScores[i].setCharacterSize(40);
			}

			HighScores[i].setFont(FM[0]);
			if (i < 9) HighScores[i].setString(to_string(0) + to_string(i + 1) + ". -------------------------- " + to_string(highScores[i]));
			else HighScores[i].setString(to_string(i + 1) + ". -------------------------- " + to_string(highScores[i]));
			HighScores[i].setFillColor(Color{ 255,240, (Uint8)(230 - (Uint8)(20 * i)) });
			HighScores[i].setPosition(40, pos);
			if (i == 0) {
				pos += 65;
			}
			else if (i == 1) {
				pos += 60;
			}
			else if (i == 2) {
				pos += 55;
			}
			else {
				pos += 40;
			}

		}
	}


	void run() {


		while (this->window.isOpen()) {
			Event event;
			while (this->window.pollEvent(event)) {
				if (event.type == Event::Closed)
					this->window.close();
				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::Escape) {
						if (this->showHighScores) {
							this->showHighScores = false;
						}
						else if (this->gameOver) {
							this->gameOver = false;
							this->showMenu = true;
							this->menu.reset();
						}
						else {
							if (!this->showMenu) {
								this->showMenu = true;
								this->remainingTime = this->remainingTime - this->runClock->getElapsedTime().asSeconds();
								if (this->runClock) delete this->runClock;
								this->runClock = nullptr;
							}
						}
					}
					else if (event.key.code == Keyboard::C) {
						system("cls");
					}
					else if (event.key.code == Keyboard::Return) {
						if (this->showMenu) {
							this->menu.handleEnter(this->showMenu, this->showHighScores, this->quit, this->hasStarted, restarted, &ZF, &sun);
							if (!this->showMenu && !this->restarted) { // resume or start mode
								this->runClock = new Clock();
							}
							else if (this->restarted) {
								restartGame();
							}
							else if (this->showHighScores) {
								initHighScores();
							}
							else if (this->quit) {
								this->window.close();
							}
						}
					}
					else if (event.key.code == Keyboard::Up) {
						if (this->showMenu && !this->showHighScores) {
							this->menu.handleUp();
						}
					}
					else if (event.key.code == Keyboard::Down) {
						if (this->showMenu && !this->showHighScores) {
							this->menu.handleDown();
						}
					}
				}
				else if (event.type == Event::MouseButtonPressed) {
					if (!this->showMenu && !this->showHighScores) {
						if (event.mouseButton.button == Mouse::Left) {
							int mouseX = event.mouseButton.x;
							int mouseY = event.mouseButton.y;

							if (gardenCords.valid(mouseX, mouseY)) {
								cout << "Position on Grid: " << (mouseY - gardenCords.topY) / 96 << ", " << (mouseX - gardenCords.leftX) / 80 << endl;

								// Handle placing of plants
								int gy = (mouseY - gardenCords.topY) / 96;
								int gx = (mouseX - gardenCords.leftX) / 80;
								this->PF.handleSunClick(gx, gy, this->sunCountText, this->sunCount);
								this->PF.handlePlacing(&this->Inv, gx, gy, this->sunCount, this->levelIndex + 1);
								this->PF.handleSunClick(gx, gy, this->sunCountText, this->sunCount);
								this->PF.handleWallnutClick(gx, gy);
								this->PF.handleFallingSun(gx, gy, &this->sun, this->sunCountText, this->sunCount);
							}

							// no need to call in if statement

							this->Inv.validMouseClick(mouseX, mouseY, this->sunCount);
						}
					}
				}
			}


			if (this->showMenu && !this->gameOver) {
				this->window.clear();
				this->menu.display(this->window);
				if (this->showHighScores) {
					this->window.draw(this->menu.getHSSprite());
					this->window.draw(this->heading);
					for (int i = 0; i < 10; i++) {
						if (i < 3) {
							this->window.draw(medals[i]);
						}
						this->window.draw(this->HighScores[i]);
					}
				}
				this->window.display();
			}
			else if (this->gameOver) {
				this->window.clear();
				this->gameover.draw(this->window);
				this->window.display();
			}
			else {
				this->window.clear();
				this->updateEverything();
				this->drawEverything();
				this->window.display();
			}

		}

	}

	void calculateTime() {
		if (this->runClock == nullptr) return;
		this->gameTime = this->remainingTime - this->runClock->getElapsedTime().asSeconds();

		string minutes = "0" + to_string((int)(this->gameTime) / 60);
		string seconds = "";
		if (((int)(this->gameTime) % 60) / 10 == 0) {
			seconds += '0';
		}
		seconds += to_string((int)(this->gameTime) % 60);
		this->timeString = minutes + ":" + seconds;
		if ((int)(this->gameTime) / 60 == 0 && (int)(this->gameTime) % 60 == 10) {
			this->TimeText.setFillColor(Color::Red);
		}
	}

};