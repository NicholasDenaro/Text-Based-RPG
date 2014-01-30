/* Castle.h - Castle class
** Written by Nicholas Denaro
*/

#pragma once
#include "TileContainer.h"

#define DEFAULTVALUE 0
#define CASTLECHARACTER 'C'
#define CASTLECOLOR "#119"


class Castle:public TileContainer
{
public:
	Castle(){}
	Castle(string name, int level):TileContainer(name,level)
	{
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
		cout<<Name<<endl;
	}

	vector<string> GetActions() const
	{
		vector<string> actions;
		actions.push_back("[X] Leave");
		if(Explored>=30)
		{
			actions.push_back("[I] Rest at Inn");
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
		return((char)CASTLECHARACTER);
	}

	string GetColor() const
	{
		return(CASTLECOLOR);
	}

	ContainerType GetType() const
	{
		return(CASTLE);
	}

	friend ostream& operator<<(ostream& os, const Castle& castle)
	{
		os<<castle.Name<<'\n';
		os<<castle.Level<<'\n';
		os<<castle.Explored;
		return(os);
	}

	friend istream& operator>>(istream& is, Castle& castle)
	{
		string tempname;
		is>>castle.Name;
		getline(is,tempname);
		castle.Name+=tempname;
		is>>castle.Level>>castle.Explored;
		return is;
	}

private:
};

