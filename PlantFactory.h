#ifndef PLANT_FAC_H
#define PLANT_FAC_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "SoundManager.h"
#include "Inventory.h"
#include "FallingSun.h"
#include "Scoreboard.h"

class PlantFactory {
	Plant** plants;
	int plantsArrayIndex;
	const int maxPlants = 45;
	SoundManager* SMptr;
	TextureManager* TMptr;

public:
	PlantFactory(SoundManager* SM, TextureManager* TMptr);
	Plant** getPlants();
	int getPlantsArrayIndex();
	int& getPlantsArrayIndexByRef();

	// Handle placing of plant from inventory
	void handlePlacing(Inventory* Inv, int gx, int gy, int& sunCount, int round);

	void reset();

	// handles clicking of player on sun
	void handleSunClick(int gx, int gy, Text& sunCountText, int& sunCount);

	void handleWallnutClick(int gx, int gy);

	void handleFallingSun(int gx, int gy, FallingSun* sun, Text& sunCountText, int& sunCount);

	void updateEverything(Zombie** zombies, int zombiesArrayIndex, Scoreboard* scoreboard);

	void draw(RenderWindow& window);

};
#endif