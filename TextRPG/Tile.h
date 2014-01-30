/* Tile.h Tile Header
** Written by Nicholas Denaro
*/

#pragma once
#include "TileContainer.h"

#define DEFAULTVALUE 0
#define WATER "#025 "
#define PLAINS "#042 "
#define HILLS "#162 "
#define MOUNTAINS "#196 "

#define WATERLEVEL 0.6
#define PLAINSLEVEL 0.7
#define HILLSLEVEL 0.76
#define MOUNTAINSLEVEL 1

class Tile
{
public:
	Tile(double value=DEFAULTVALUE);

	~Tile();

	void SetLevel(int level);

	int GetLevel() const;

	double GetValue() const;

	void ResetExploration();

	void Explore(int explore);

	int GetExploration() const;

	void SetAvailable(bool avail);

	bool IsAvailable() const;

	void DrawChar(char player) const;

	void SetContainer(TileContainer* container);

	void SetScreenPos(int x, int y);

	TileContainer* GetContainer() const;

	friend ostream& operator<<(ostream& os, const Tile& tile);

	friend istream& operator>>(istream& is, Tile& tile);

private:
	double Value;
	bool Available;
	int Explored;
	int Level;
	TileContainer* Container;
	int ScreenPosX, ScreenPosY;
};

