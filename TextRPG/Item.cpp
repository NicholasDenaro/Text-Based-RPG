/* Item.cpp - 
** Written by Nicholas Denaro
*/

#include "Item.h"
//#include "Player.h"
#include <string>
using namespace std;

Item::Item()
{
	Name="nullitem";
}

Item::Item(string name, ItemType type, int level, vector<ItemEffect> effects,int worth)
{
	Name=name;
	Level=level;
	Type=type;
	Effects=effects;
	Worth=worth;
}

PlayerStats Item::StEPlayerStats(string s)
{
	if(s.compare("Health")==0)
		return(HEALTH);
	else if(s.compare("Mana")==0)
		return(MANA);
	else if(s.compare("Strength")==0)
		return(STRENGTH);
	else if(s.compare("Dexterity")==0)
		return(DEXTERITY);
	else if(s.compare("Intellect")==0)
		return(INTELLECT);
	else if(s.compare("Luck")==0)
		return(LUCK);
	else if(s.compare("Charisma")==0)
		return(CHARISMA);
	else if(s.compare("Armor")==0)
		return(ARMOR);
	return(HEALTH);
}

ItemType Item::StEItemType(string s)
{
	if(s.compare("Helmet")==0)
		return(HELMET);
	else if(s.compare("BodyArmor")==0)
		return(BODYARMOR);
	else if(s.compare("LegArmor")==0)
		return(LEGARMOR);
	else if(s.compare("Weapon")==0)
		return(WEAPON);
	else if(s.compare("Usable")==0)
		return(USABLE);
	return(USABLE);
}

map<string,Item> Item::ItemMap;
map<int,vector<string>> Item::ItemLevelMap;

void Item::LoadItems(string dir, string fname)
{
	string path=dir+fname;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;// character type defaults to char
	doc.parse<0>(xmlFile.data());// 0 means default parse flags
	rapidxml::xml_node<>* root=doc.first_node("ItemList");
	rapidxml::xml_node<>* itemNode;
	rapidxml::xml_node<>* effectNode;
	while(root->first_node()!=0)
	{
		itemNode=root->first_node();
		string name=itemNode->first_attribute("Name")->value();
		string type=itemNode->first_attribute("Type")->value();
		int level=Printer::StringToInt(itemNode->first_attribute("Level")->value());
		int worth=Printer::StringToInt(itemNode->first_attribute("Worth")->value());
		effectNode=itemNode->first_node("Effects");
		vector<ItemEffect> effects;
		while(effectNode->first_attribute()!=0)
		{

			string effectType=effectNode->first_attribute()->name();
			int effectValue=Printer::StringToInt(effectNode->first_attribute()->value());
			effects.push_back(ItemEffect(StEPlayerStats(effectType),effectValue));
			effectNode->remove_first_attribute();
		}
		Item item=Item(name,StEItemType(type),level,effects,worth);
		ItemMap[name]=item;
		ItemLevelMap[item.GetLevel()].push_back(item.GetName());
		root->remove_first_node();
	}
}

int Item::GetLevel() const
{
	return(Level);
}

int Item::GetWorth() const
{
	return(Worth);
}

void Item::AddEffect(ItemEffect effect)
{
	Effects.push_back(effect);
}

string Item::GetName() const
{
	return(Name);
}

ItemType Item::GetType() const
{
	return(Type);
}

vector<ItemEffect> Item::GetEffects() const
{
	return(Effects);
}

ostream& operator<<(ostream& os,Item& item)
{
	os<<item.Name<<'\n';
	os<<item.Level<<'\n';
	os<<item.Worth<<'\n';
	os<<(signed int)item.Effects.size()<<'\n';
	for(unsigned int i=0;i<item.Effects.size();i+=1)
	{
		os<<item.Effects[i]<<'\n';
	}
	os<<item.Type;
	return(os);
}

istream& operator>>(istream& is,Item& item)
{
	int size;
	string tempname;
	is>>item.Name;
	getline(is,tempname);
	item.Name+=tempname;
	is>>item.Level>>item.Worth>>size;
	for(int i=0;i<size;i+=1)
	{
		ItemEffect effect;
		is>>effect;
		item.AddEffect(effect);
	}
	int type;
	is>>type;
	item.Type=(ItemType)type;
	return(is);
}