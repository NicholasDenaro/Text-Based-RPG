/* WorldMap.h WorldMap header file
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include <vector>
#include "Tile.h"
#include "Player.h"

using namespace std;

#define DEFAULTWIDTH 16
#define DEFAULTHEIGHT 16
#define DEFAULT_SEED 0
#define DUNGEON_COUNT 3

class WorldMap
{
public:
	static WorldMap* Map;

	WorldMap(int width=DEFAULTWIDTH, int height=DEFAULTHEIGHT, int seed=DEFAULT_SEED);

	~WorldMap();

	void BuildWorld();

	int GetWidth() const;

	int GetHeight() const;

	void Explore();

	int GetExplored() const;

	Tile* GetTileAt(int i, int a); //const;

	Tile* GetCurrentTile(int deltaX=0, int deltaY=0); //const;

	void Save();

	static bool Load(string fname);

	void UpdateTile(int x, int y);

	void UpdateTileInfo();

	void Draw();

	Player* GetCharacter();

	void SetCharacter(Player* character);

	void MoveCharacter(int deltaX, int deltaY);

	friend ostream& operator<<(ostream& os, const WorldMap& map);

	friend istream& operator>>(istream& is, WorldMap& map);

private:
	int Width, Height;
	int SpawnX, SpawnY;
	int ActionsPosX, ActionsPosY;
	Player* Character;
	vector<vector<Tile*>> Tiles;

	void BuildMap();

	void SmoothMap();

	void CreateTowns();

	void CreateCastles();

	void CreateDungeons(int dungeons);

	void SetTileLevels();

	int GetLandArea();
};