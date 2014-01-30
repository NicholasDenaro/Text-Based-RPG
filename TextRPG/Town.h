/* Town.h Town Header
** Written by Nicholas Denaro
*/

#pragma once
#include "TileContainer.h"
#include "WorldMap.h"

#define DEFAULTVALUE 0
#define TOWNCHARACTER 'T'
#define TOWNCOLOR "#136"


class Town:public TileContainer
{
public:
	Town(){}
	Town(string name, int level, string specialty):TileContainer(name,level)
	{
		Specialty=specialty;
	}

	static string GetRandomSpecialty()
	{
		int r=rand()%3;
		switch(r)
		{
			case 0:
				return("Inn");
			break;
			case 1:
				return("Market");
			break;
			case 2:
				return("Quests");
			break;
			default:
				return("Inn");
		}
	}

	void Explore()
	{
		Explored+=10;
	}

	void PrintUnlocks() const
	{
		if(Explored==30)
		{
			Printer::pout<<"#014You've discovered the Inn!"<<endl;
		}
		if(Explored==60)
		{
			Printer::pout<<"#014You've discovered the Market!"<<endl;
		}
		if(Explored==100)
		{
			Printer::pout<<"#014You've discovered the Guild HQ!"<<endl;
		}
	}

	void DrawInfo()
	{
		cout<<Name<<": Specialty: "<<Specialty<<endl;
	}

	vector<string> GetActions() const
	{
		vector<string> actions;
		actions.push_back("[X] Leave");
		if(Explored>=30)
		{
			int cost=100+(WorldMap::Map->GetCharacter()->GetLevel()-1)*10-(WorldMap::Map->GetCharacter()->GetStat(CHARISMA)*5);
			if(Specialty=="Inn")
				cost=(int)(cost*3.0/4);
			stringstream act;
			act<<"[I] Rest at Inn("<<cost<<" Qulz)";
			actions.push_back(act.str());
		}
		if(Explored>=60)
		{
			actions.push_back("[M] Shop at Market");
		}
		if(Explored==100)
		{
			actions.push_back("[Q] Get Quest from Guild HQ");
		}
		return(actions);
	}

	char GetChar() const
	{
		return((char)TOWNCHARACTER);
	}

	string GetColor() const
	{
		return(TOWNCOLOR);
	}

	ContainerType GetType() const
	{
		return(TOWN);
	}

	string GetSpecialty() const
	{
		return(Specialty);
	}

	friend ostream& operator<<(ostream& os, const Town& town)
	{
		os<<town.Name<<'\n';
		os<<town.Level<<'\n';
		os<<town.Explored<<'\n';
		os<<town.Specialty;
		return(os);
	}

	friend istream& operator>>(istream& is, Town& town)
	{
		string tempname;
		is>>town.Name;
		getline(is,tempname);
		town.Name+=tempname;
		is>>town.Level>>town.Explored>>town.Specialty;
		getline(is,tempname);
		town.Specialty+=tempname;
		return(is);
	}

private:
	string Specialty;
};

