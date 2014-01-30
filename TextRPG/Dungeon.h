/* Town.h Town Header
** Written by Nicholas Denaro
*/

#pragma once
#include "TileContainer.h"
#include <vector>
#include "Monster.h"
#include "Printer.h"
#include "Battle.h"

#define DEFAULTVALUE 0
#define DUNGEONCHARACTER 'D'
#define DUNGEONCOLOR "#068"
#define MAXDUNGEONLEVEL 30

class Dungeon:public TileContainer
{
public:
	Dungeon(){}
	Dungeon(string name, int level):TileContainer(name,level)
	{
		int minLevel=Level-2;
		if(minLevel<1)
			minLevel=1;
		Monsters=Monster::GetLevelRange(minLevel,Level+2);
		map<string,Monster> bosses=Monster::GetBossLevelRange(Level+2,Level+2);
		int r=rand()%bosses.size();
		map<string,Monster>::iterator it=bosses.begin();
		for(int i=0;i<r;i+=1)
			it++;
		Boss=it->second;
		Cleared=false;
	}

	void SetLevel(int lvl)
	{
		Level=lvl;
		if(Level>MAXDUNGEONLEVEL)
			Level=MAXDUNGEONLEVEL;
	}

	void Explore()
	{
		if(Cleared==false)
		{
			Explored+=10;
			if(Explored<100)
			{
				Printer::pout<<"#014Battle!"<<endl;
				system("pause");
				system("cls");
				//random level
				int level=Level+rand()%3-1;
				if(level<=0)
					level=1;
				//get monsters at the level
				vector<string> monsters=Monster::MonsterLevelMap[level];
				int r=rand()%monsters.size();
				//random monster from the level
				Monster monster=Monster::MonsterMap[monsters[r]];
				Battle b=Battle(Player::Character,Monster(monster,level),true);
				b.Run();
				system("pause");
				system("cls");
				WorldMap::Map->Draw();
			}
			else
			{
				Printer::pout<<"#013Boss Battle!"<<endl;
				system("pause");
				system("cls");
				Battle b=Battle(Player::Character,Monster(Boss,Level+2),true);
				b.Run();
				if(Level==30)
				{
					Printer::pout<<"#014You've defeated the final boss. You've won the game.\nThe Game will exit."<<endl;
					system("pause");
					exit(0);
				}
				system("pause");
				system("cls");
				WorldMap::Map->Draw();
			}
		}
	}

	void DrawInfo()
	{
		cout<<"Dungeon"<<endl;
	}

	vector<string> GetActions() const
	{
		vector<string> actions;
		actions.push_back("[X] Leave");
		if(Explored==100)
		{
			actions.push_back("[R] Reset Dungeon");
		}
		return(actions);
	}

	void PrintUnlocks() const
	{
		if(Explored==100)
		{
			Printer::pout<<"#014You've discovered the Dungeon Resetter!"<<endl;
		}
	}

	char GetChar() const
	{
		return((char)DUNGEONCHARACTER);
	}

	string GetColor() const
	{
		return(DUNGEONCOLOR);
	}

	ContainerType GetType() const
	{
		return(DUNGEON);
	}

	bool IsCleared() const
	{
		return(Cleared);
	}

	friend ostream& operator<<(ostream& os, Dungeon& dungeon)
	{
		os<<dungeon.Name<<'\n';
		os<<dungeon.Level<<'\n';
		os<<dungeon.Explored<<'\n';
		os<<dungeon.Monsters.size();
		map<string,Monster>::iterator it=dungeon.Monsters.begin();
		while(it!=dungeon.Monsters.end())
		{
			os<<it->second<<'\n';
			it++;
		}
		os<<dungeon.Boss;
		return(os);
	}

	friend istream& operator>>(istream& is, Dungeon& dungeon)
	{
		int size;
		string tempname;
		is>>dungeon.Name;
		getline(is,tempname);
		dungeon.Name+=tempname;
		is>>dungeon.Level>>dungeon.Explored>>size;
		for(int i=0;i<size;i+=1)
		{
			Monster m;
			is>>m;
			dungeon.Monsters[m.GetName()]=m;
		}
		is>>dungeon.Boss;
		return(is);
	}

private:
	map<string,Monster> Monsters;
	Monster Boss;
	bool Cleared;
};

