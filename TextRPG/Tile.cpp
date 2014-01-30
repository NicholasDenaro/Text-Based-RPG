/* Tile.h Tile Header
** Written by Nicholas Denaro
*/
#pragma once
#include "Tile.h"
#include "Printer.h"
#include <string>
#include "Town.h"
#include "Castle.h"
#include "Dungeon.h"
using namespace std;


Tile::Tile(double value)
{
	Value=value;
	Available=false;
	Container=NULL;
	Level=1;
	Explored=0;
}

Tile::~Tile()
{
	if(Container!=NULL)
		delete Container;
}

void Tile::SetLevel(int level)
{
	Level=level;
}

int Tile::GetLevel() const
{
	return(Level);
}

double Tile::GetValue() const
{
	return(Value);
}

void Tile::ResetExploration()
{
	Explored=0;
	if(Container!=NULL)
		Container->SetExploration(0);
}

void Tile::Explore(int explore)
{
	if(Container==NULL)
	{
		Explored+=explore;
		if(Explored>=100)
			Explored=100;
	}
	else
	{
		Container->Explore();
		Explored=Container->GetExplored();
	}
}

int Tile::GetExploration() const
{
	return(Explored);
}

void Tile::SetAvailable(bool avail)
{
	Available=avail;
}

bool Tile::IsAvailable() const
{
	return(Available);
}

TileContainer* Tile::GetContainer() const
{
	return(Container);
}

void Tile::SetContainer(TileContainer* container)
{
	Container=container;
}

void Tile::SetScreenPos(int x, int y)
{
	ScreenPosX=x;
	ScreenPosY=y;
}

void Tile::DrawChar(char player) const
{
	COORD cur ={ScreenPosX, ScreenPosY};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	if(!Available)
	{
		cout<<" ";
		return;
	}
	if(Container!=NULL)
	{
		if(player==' ')
		{
			string color=Container->GetColor();
			char c=Container->GetChar();
			string out=color+c;
			Printer::pout<<out;
		}
		else
		{
			string out=Printer::CreateColor("#015",Printer::GetBGColor(Container->GetColor()));
			out+=player;
			Printer::pout<<out;
		}
	}
	else
	{
		string out="#000 ";
		if(Value<=WATERLEVEL)
		{
			out=WATER;
		}
		else if(Value<=PLAINSLEVEL)
		{
			out=PLAINS;
		}
		else if(Value<=HILLSLEVEL)
		{
			 out=HILLS;
		}
		else if(Value<=MOUNTAINSLEVEL)
		{
			 out=MOUNTAINS;
		}
		if(player!=' ')
		{
			out=Printer::CreateColor("#015",Printer::GetBGColor(out))+player;
		}
		Printer::pout<<out;
	}
	//SetConsoleTextAttribute(hConsole,15);
}

ostream& operator<<(ostream& os, const Tile& tile)
{
	os<<tile.Value<<'\n';
	os<<tile.Available<<'\n';
	os<<tile.Explored<<'\n';
	os<<tile.Level<<'\n';
	os<<(tile.GetContainer()!=NULL);
	if(tile.GetContainer()!=NULL)
	{
		os<<'\n'<<tile.Container->GetType()<<'\n';
		if(tile.Container->GetType()==TOWN)
		{
			Town* t=(Town*)tile.Container;
			os<<*t;
		}
		if(tile.Container->GetType()==CASTLE)
		{
			Castle* c=(Castle*)tile.Container;
			os<<*c;
		}
		if(tile.Container->GetType()==DUNGEON)
		{
			Dungeon* d=(Dungeon*)tile.Container;
			os<<*d;
		}
	}
	return(os);
}

istream& operator>>(istream& is, Tile& tile)
{
	is>>tile.Value>>tile.Available>>tile.Explored>>tile.Level;
	bool container;
	is>>container;
	if(container)
	{
		int type;
		is>>type;
		ContainerType ctype=(ContainerType)type;
		if(ctype==TOWN)
		{
			Town* t=new Town();
			is>>*t;
			tile.Container=t;
		}
		else if(ctype==CASTLE)
		{
			Castle* c=new Castle();
			is>>*c;
			tile.Container=c;
		}
		else if(ctype==DUNGEON)
		{
			Dungeon* d=new Dungeon();
			is>>*d;
			tile.Container=d;
		}
	}
	return(is);
}