/* PlayerRace.cpp - 
** Written by Nicholas Denaro
*/

#include "PlayerRace.h"
#include "Player.h"
using namespace std;

PlayerRace::PlayerRace(){}

PlayerRace::PlayerRace(string name, char c, vector<int> bonuses, string description)
{
	Name=name;
	Char=c;
	Bonuses=bonuses;
	Description=description;
}

map<string,PlayerRace> PlayerRace::RaceMap;

void PlayerRace::LoadRaces(string dir, string fname)
{
	string path=dir+fname;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;// character type defaults to char
	doc.parse<0>(xmlFile.data());// 0 means default parse flags
	rapidxml::xml_node<>* root=doc.first_node("Races");
	rapidxml::xml_node<>* raceNode;
	rapidxml::xml_node<>* bonusNode;
	rapidxml::xml_node<>* descriptionNode;
	while(root->first_node()!=0)
	{
		raceNode=root->first_node();
		//cout<<"Adding Race: "<<raceNode->name()<<endl;
		string name=raceNode->name();
		char c=raceNode->first_attribute("Char")->value()[0];
		bonusNode=raceNode->first_node("Bonuses");
		vector<int> bonuses;
		while(bonusNode->first_attribute()!=0)
		{
			stringstream stream;
			int n;
			stream<<bonusNode->first_attribute()->value();
			if(!(stream>>n))
				throw "PlayerRace::LoadRaces(): Can't convert string to int";
			bonuses.push_back(n);
			bonusNode->remove_first_attribute();
		}
		descriptionNode=raceNode->first_node("Description");
		string description=descriptionNode->first_attribute("Attr1")->value();

		RaceMap[name]=PlayerRace(name,c,bonuses,description);
		//remove node
		root->remove_first_node();
	}
}

vector<int> PlayerRace::GetBonuses() const
{
	return(Bonuses);
}

void PlayerRace::PrintDescription() const
{
	Printer::pout<<"Race: "<<Name<<endl;
	Printer::pout<<"  "<<Description<<" It's bonuses are:"<<endl;
	Printer::pout<<"    #012Health#015: "<<((Bonuses[HEALTH]>0)?"+":"")<<Bonuses[HEALTH]<<endl;
	Printer::pout<<"    #009Mana#015: "<<((Bonuses[MANA]>0)?"+":"")<<Bonuses[MANA]<<endl;
	Printer::pout<<"    #004Strength#015: "<<((Bonuses[STRENGTH]>0)?"+":"")<<Bonuses[STRENGTH]<<endl;
	Printer::pout<<"    #002Dexterity#015: "<<((Bonuses[DEXTERITY]>0)?"+":"")<<Bonuses[DEXTERITY]<<endl;
	Printer::pout<<"    #011Intellect#015: "<<((Bonuses[INTELLECT]>0)?"+":"")<<Bonuses[INTELLECT]<<endl;
	Printer::pout<<"    #008Luck#015: "<<((Bonuses[LUCK]>0)?"+":"")<<Bonuses[LUCK]<<endl;
	Printer::pout<<"    #013Charisma#015: "<<((Bonuses[CHARISMA]>0)?"+":"")<<Bonuses[CHARISMA]<<endl;
}

string PlayerRace::GetName() const
{
	return(Name);
}

char PlayerRace::GetChar() const
{
	return(Char);
}

ostream& operator<<(ostream& os, const PlayerRace& pr)
{
	os<<pr.Name;
	return(os);
}

istream& operator>>(istream& is, PlayerRace& pr)
{
	is>>pr.Name;
	return(is);
}