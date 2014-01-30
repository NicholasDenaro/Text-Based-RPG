/* PlayerRace.h - The PlayerRace prototype
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "Printer.h"
#include "rapidxml-1.13\rapidxml.hpp"
#include "rapidxml-1.13\rapidxml_utils.hpp"
using namespace std;


class PlayerRace
{
public:
	PlayerRace();

	PlayerRace(string name, char c, vector<int> bonuses, string description);

	static map<string,PlayerRace> RaceMap;

	static void LoadRaces(string dir, string fname);
	
	vector<int> GetBonuses() const;

	void PrintDescription() const;

	string GetName() const;

	char GetChar() const;

	friend ostream& operator<<(ostream& os, const PlayerRace& pr);

	friend istream& operator>>(istream& is, PlayerRace& pr);

private:
	string Name;
	char Char;
	vector<int> Bonuses;
	string Description;
};