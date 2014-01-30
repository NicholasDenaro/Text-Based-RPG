/* TileContainer.h - TileContainer header
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include "ActionList.h"
using namespace std;

enum ContainerType{TOWN,CASTLE,DUNGEON};

class TileContainer
{
public:
	TileContainer(){}

	TileContainer(string name, int level)
	{
		Name=name;
		Level=level;
		Explored=0;
	}

	virtual void Explore()=0;

	string GetName() const
	{
		return(Name);
	}

	int GetLevel() const
	{
		return(Level);
	}

	void SetExploration(int e)
	{
		Explored=e;
	}

	int GetExplored() const
	{
		return(Explored);
	}

	virtual void DrawInfo()=0;

	virtual vector<string> GetActions() const=0;

	virtual void PrintUnlocks() const=0;

	virtual string GetColor() const=0;

	virtual char GetChar() const=0;

	virtual ContainerType GetType() const=0;

protected:
	string Name;
	int Level;
	int Explored;
};