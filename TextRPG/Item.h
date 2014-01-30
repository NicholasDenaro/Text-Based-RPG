/* Item.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <map>
#include "ItemEffect.h"
class ItemEffect;
#include "rapidxml-1.13\rapidxml.hpp"
#include "rapidxml-1.13\rapidxml_utils.hpp"
using namespace std;

enum ItemType{HELMET=0,BODYARMOR,LEGARMOR,WEAPON,USABLE};

class Item
{
public:
	Item();

	Item(string name, ItemType type, int level, vector<ItemEffect> effects,int worth);

	static PlayerStats StEPlayerStats(string s);

	static ItemType StEItemType(string s);

	static map<string,Item> ItemMap;

	static map<int,vector<string>> ItemLevelMap;

	static void LoadItems(string dir, string fname);

	int GetLevel() const;

	int GetWorth() const;

	void AddEffect(ItemEffect effect);

	string GetName() const;

	ItemType GetType() const;

	vector<ItemEffect> GetEffects() const;

	friend ostream& operator<<(ostream& os,Item& item);

	friend istream& operator>>(istream& is,Item& item);

private:
	string Name;
	int Level;
	int Worth;
	ItemType Type;
	vector<ItemEffect> Effects;
};