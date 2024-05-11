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

class Game {
	// window
	RenderWindow window;
	TextureManager TM;
	SoundManager SM;
	FontManager FM;
	Background background;
	Inventory Inv;

	int sunCount = 10000;
	Text sunCountText;


	PlantFactory PF;
	ZombieFactory ZF;

	LawnMower* lawnmowers[5]{ nullptr };
	float lawnMowerPos[2] = { -1, 0 };

	Life lives;

	FallingSun sun;

	Clock RunClock;
	Text TimeText;
	string timeString;

	Menu menu;

	float roundTimeLimit = 5;

	bool play = false, showHighScores = false, resume = false, quit = false;

	bool hasStarted = false;


	int highScores[10] = {};
	Text HighScores[10], heading;
	Sprite medals[3];

	Level** levels = new Level * [4];
	int levelIndex = 0;

public:
	Game() : window(VideoMode(1400, 600), "game"), background(&TM), Inv(&TM, &SM), PF(&SM, &TM), ZF(&TM, &SM), menu(&TM, &FM) {


		levels[0] = new BeginnersGarden{ background, &TM, &FM, &SM, &RunClock, &TimeText, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		levels[1] = nullptr;
		levels[2] = nullptr;
		levels[3] = nullptr;
		// levels[1] = new ZombieOutskirts(&FM, &SM);
		// levels[1] = new SunflowerField(&FM, &SM);
		// levels[3] = new NightTimeField

		medals[0].setTexture(this->TM.getTexture("gold"));
		medals[1].setTexture(this->TM.getTexture("silver"));
		medals[2].setTexture(this->TM.getTexture("bronze"));

		medals[0].setScale(0.2, 0.2);
		medals[1].setScale(0.15, 0.15);
		medals[2].setScale(0.12, 0.12);

		srand((unsigned)time(0));
		/*this->RunClock.restart();
		this->TimeText.setPosition(1300, 550);
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
		}*/
	}


	void updateRound() {
		levelIndex += 1;
		if (this->levelIndex > 3) levelIndex = 3;

		if (levels[levelIndex] == nullptr && levelIndex == 1) {
			levels[levelIndex] = new FullGarden{ background, &PF, &ZF, &Inv, &TM, &FM, &SM, &RunClock, &TimeText, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		}
		else if (levels[levelIndex] == nullptr && levelIndex == 2) {
			levels[levelIndex] = new NightGarden{ background, &PF, &ZF, &Inv, &TM, &FM, &SM, &RunClock, &TimeText, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		}
		else if (levels[levelIndex] == nullptr && levelIndex == 3) {
			levels[levelIndex] = new LimitedGarden{ background, &PF, &ZF, &Inv, &TM, &FM, &SM, &RunClock, &TimeText, &sunCountText,  sunCount, lawnmowers, lawnMowerPos };
		}
	}

	void drawEverything() {
		levels[levelIndex]->drawEverything(this->window, this->background, &Inv, sunCount, &PF, &ZF, lawnmowers, lives, &sun, TimeText, sunCountText);
	}

	void updateEverything() {
		levels[levelIndex]->updateEverything(timeString, RunClock, roundTimeLimit, TimeText, &PF, &ZF, lawnmowers, lives, sun);
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
						this->menu.setInMenu(true);
						this->menu.handleEnter(this->hasStarted, this->play, this->showHighScores, this->resume, this->quit, 0);
					}
					else if (event.key.code == Keyboard::C) {
						system("cls");
					}

					if (this->menu.inMenu()) {
						if (event.key.code == Keyboard::Up) {
							this->menu.handleUp();
						}
						else if (event.key.code == Keyboard::Down) {
							this->menu.handleDown();
						}
						else if (event.key.code == Keyboard::Enter) {
							this->menu.handleEnter(this->hasStarted, this->play, this->showHighScores, this->resume, this->quit);
							if (this->quit) {
								window.close();
							}
							if (this->showHighScores) {
								this->initHighScores();
							}
						}
					}
				}
				if (event.type == Event::MouseButtonPressed) {
					if (event.mouseButton.button == Mouse::Left) {
						int mouseX = event.mouseButton.x;
						int mouseY = event.mouseButton.y;

						if (gardenCords.valid(mouseX, mouseY)) {
							cout << "Position on Grid: " << (mouseY - gardenCords.topY) / 96 << ", " << (mouseX - gardenCords.leftX) / 80 << endl;

							// Handle placing of plants
							int gy = (mouseY - gardenCords.topY) / 96;
							int gx = (mouseX - gardenCords.leftX) / 80;

							if (this->play || (this->play == 0 && this->resume == 1)) {
								this->PF.handlePlacing(&this->Inv, gx, gy, this->sunCount, levelIndex + 1);
								this->PF.handleSunClick(gx, gy, this->sunCountText, this->sunCount);
								this->PF.handleWallnutClick(gx, gy);
								this->PF.handleFallingSun(gx, gy, &this->sun, this->sunCountText, this->sunCount);
							}
						}

						// no need to call in if statement
						if (this->play || (this->play == 0 && this->resume == 1))
							this->Inv.validMouseClick(mouseX, mouseY, this->sunCount);
					}
				}
			}

			this->window.clear();

			if (this->play || (this->play == 0 && this->resume == 1))
				this->updateEverything();

			if (this->RunClock.getElapsedTime().asSeconds() > 5) {
				this->updateRound();
			}

			// Draw everything here...

			if (this->menu.inMenu()) {
				this->menu.display(window);
			}

			if (this->play || (this->play == 0 && this->resume == 1)) {
				this->drawEverything();
			}

			if (this->showHighScores) {
				window.draw(menu.getHSSprite());
				window.draw(this->heading);
				for (int i = 0; i < 10; i++) {
					if (i < 3) {
						window.draw(medals[i]);
					}
					window.draw(this->HighScores[i]);
				}

			}

			this->window.display();
		}
	}
};
