/* WorldMap.cpp - 
** Written by Nicholas Denaro
*/
#include "WorldMap.h"
#include <iostream>
#include <iomanip>
#include "Town.h"
#include "Dungeon.h"
#include "Castle.h"
#include <fstream>
using namespace std;

WorldMap* WorldMap::Map;

WorldMap::WorldMap(int width, int height, int seed)
{
	Width=width;
	Height=height;
	Tiles=vector<vector<Tile*>>(Width);
	
	//Set the seed
	srand(seed);
}

WorldMap::~WorldMap()
{
	//breaks?
	/*if(Character!=NULL)
		delete Character;
	for(int a=0;a<Height;a+=1)
	{
		for(int i=0;i<Width;i+=1)
		{
			if(Tiles[i][a]!=NULL)
				delete Tiles[i][a];
		}
	}*/
}

void WorldMap::BuildWorld()
{
	BuildMap();
	CreateTowns();
	SetTileLevels();
	CreateCastles();
	CreateDungeons(DUNGEON_COUNT);
}

Player* WorldMap::GetCharacter()
{
	return(Character);
}

void WorldMap::SetCharacter(Player* character)
{
	Character=character;
	Character->SetPos(SpawnX,SpawnY);
	Tiles[SpawnX][SpawnY]->SetAvailable(true);
}

void WorldMap::MoveCharacter(int deltaX, int deltaY)
{
	if((Character->GetX()+deltaX>=0)&&(Character->GetX()+deltaX<Width)&&(Character->GetY()+deltaY>=0)&&(Character->GetY()+deltaY<Height))
	{
		Character->Move(deltaX,deltaY);
		if(GetCurrentTile()->GetContainer()==NULL)
		{
			int r=(int)(rand()*100.0/RAND_MAX);
			if(r>Character->GetStat(LUCK))
			{
				int level;
				Battle b;
				Printer::pout<<"#014You've been attacked while traveling!"<<endl;
				system("pause");
				system("cls");
				//random level
				level=GetCurrentTile()->GetLevel()+rand()%3-1;
				if(level<=0)
					level=1;
				b=Battle(GetCharacter(),Monster(Monster::GetRandomMonster(level),level),true);
				b.Run();
				system("pause");
				system("cls");
				Draw();
			}
		}
	}
}

int WorldMap::GetWidth() const
{
	return(Width);
}

int WorldMap::GetHeight() const
{
	return(Height);
}

void WorldMap::Explore()
{
	if(Tiles[Character->GetX()][Character->GetY()]->GetContainer()==NULL)
	{
		int ran=(int)(rand()*100.0/RAND_MAX);
		if(ran>Character->GetStat(LUCK))
		{
			int level=GetCurrentTile()->GetLevel()+rand()%3-1;
			if(level<=0)
				level=1;
			Battle b(Character,Monster(Monster::GetRandomMonster(level),level),false);
			bool victory=b.Run();
			system("pause");
			system("cls");
			Draw();
			if(!victory)
				return;
		}
	}
	int roll=(int)(15+rand()*1.0/RAND_MAX*15);
	int prevExplore=Tiles[Character->GetX()][Character->GetY()]->GetExploration();
	Tiles[Character->GetX()][Character->GetY()]->Explore(roll);
	//system("cls");
	if(Tiles[Character->GetX()][Character->GetY()]->GetExploration()>=75)
	{
		try{
			GetTileAt(Character->GetX()-1,Character->GetY())->SetAvailable(true);
			GetTileAt(Character->GetX()-1,Character->GetY())->DrawChar(' ');
		}catch(char* error){error;}//throws if out of bounds
		try{
			GetTileAt(Character->GetX()+1,Character->GetY())->SetAvailable(true);
			GetTileAt(Character->GetX()+1,Character->GetY())->DrawChar(' ');
		}catch(char* error){error;}
		try{
			GetTileAt(Character->GetX(),Character->GetY()+1)->SetAvailable(true);
			GetTileAt(Character->GetX(),Character->GetY()+1)->DrawChar(' ');
		}catch(char* error){error;}
		try{
			GetTileAt(Character->GetX(),Character->GetY()-1)->SetAvailable(true);
			GetTileAt(Character->GetX(),Character->GetY()-1)->DrawChar(' ');
		}catch(char* error){error;}
	}
	//Draw();

	UpdateTileInfo();
	Printer::ClearLine();
	Printer::pout<<"#003Explored: "<<Tiles[Character->GetX()][Character->GetY()]->GetExploration()-prevExplore<<"%"<<endl;
	if(Tiles[Character->GetX()][Character->GetY()]->GetContainer()!=NULL)
		Tiles[Character->GetX()][Character->GetY()]->GetContainer()->PrintUnlocks();
	//Printer::pout<<"#005Current Exploration: "<<Tiles[Character->GetX()][Character->GetY()].GetExploration()<<"%"<<endl;
	if((prevExplore<75)&&(Tiles[Character->GetX()][Character->GetY()]->GetExploration()>=75))
		Printer::pout<<"#014Additional areas unlocked!"<<endl;
}

int WorldMap::GetExplored() const
{
	return(Tiles[Character->GetX()][Character->GetY()]->GetExploration());
}

Tile* WorldMap::GetTileAt(int i, int a) //const
{
	if(i>=0&&i<Width&&a>=0&&a<Height)
		return(Tiles[i][a]);
	else
		throw "WorldMap::GetTileAt(): position out of bounds.";
}

Tile* WorldMap::GetCurrentTile(int deltaX, int deltaY) //const
{
	return(GetTileAt(Character->GetX()+deltaX,Character->GetY()+deltaY)/*Tiles[Character->GetX()+deltaX][Character->GetY()+deltaY]*/);
}

void WorldMap::Save()
{
	string fname=Character->GetName()+".txt";
	ofstream fout(fname);
	fout<<*this;
	fout.close();
}

bool WorldMap::Load(string fname)
{
	ifstream fin(fname);
	if(fin)
	{
		WorldMap* map=new WorldMap();
		fin>>*map;
		fin.close();
		WorldMap::Map=map;
		return(true);
	}
	else
	{
		cout<<"File doesn't exist."<<endl;
		system("pause");
		return(false);
	}
}

void WorldMap::UpdateTile(int x, int y)
{
	char c=' ';
	if((Character->GetX()==x)&&(Character->GetY()==y))
		c=Character->GetChar();
	Tiles[x][y]->DrawChar(c);
	COORD cur ={ActionsPosX, ActionsPosY};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

void WorldMap::UpdateTileInfo()
{
	COORD cur ={0, Height+2+1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	//cout<<string(50,' ');
	if(Tiles[Character->GetX()][Character->GetY()]->GetContainer()!=NULL)
	{
		Printer::ClearLine();
		Tiles[Character->GetX()][Character->GetY()]->GetContainer()->DrawInfo();
	}
	Printer::ClearLine();
	cout<<"Tile Lvl. "<<Tiles[Character->GetX()][Character->GetY()]->GetLevel()<<endl;
	Printer::ClearLine();
	Printer::pout<<"#005Current Exploration: "<<(Tiles[Character->GetX()][Character->GetY()]->GetExploration())<<"%"<<endl;
	
}

void WorldMap::Draw()
{
	Character->PrintInfo();
	/*COORD cur = {1, 1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);*/
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&SBInfo);
	for(int i=0;i<Width+2;i+=1)
		cout<<"_";
	cout<<endl;
	for(int a=0;a<Height;a+=1)
	{
		cout<<"|";
		for(int i=0;i<Width;i+=1)
		{
			char c=' ';
			if((Character->GetX()==i)&&(Character->GetY()==a))
				c=Character->GetChar();
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&SBInfo);
			Tiles[i][a]->SetScreenPos(SBInfo.dwCursorPosition.X,SBInfo.dwCursorPosition.Y);
			Tiles[i][a]->DrawChar(c);
		}
		cout<<"|"<<endl;
	}
	for(int i=0;i<Width+2;i+=1)
		cout<<"_";
	cout<<endl;
	UpdateTileInfo();
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	//Printer::pout<<"#005Current Exploration: "<<(Tiles[Character->GetX()][Character->GetY()]->GetExploration()*100)<<"%"<<endl;
	
	
	ActionsPosX=SBInfo.dwCursorPosition.X;
	ActionsPosY=SBInfo.dwCursorPosition.Y;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&SBInfo);
}


void WorldMap::BuildMap()
{
	//sets the tiles to random values
	for(int i=0;i<Width;i+=1)
	{
		Tiles[i]=vector<Tile*>(Height);
		for(int a=0;a<Height;a+=1)
		{
			double r=rand()*1.0/RAND_MAX;
			Tiles[i][a]=new Tile(sqrt(r));
		}
	}


	//Take the average of the tile to smooth the map
	SmoothMap();
	SmoothMap();
	SmoothMap();
	SmoothMap();
}

void WorldMap::SmoothMap()
{
	//Take the average of the tile
	vector<vector<Tile*>> temp=vector<vector<Tile*>>(Width);//store the temporary average value of the Tiles
	for(int i=0;i<Width;i+=1)
	{
		temp[i]=vector<Tile*>(Height);
	}

	for(int a=0;a<Height;a+=1)
	{
		for(int i=0;i<Width;i+=1)
		{
			double sum=0;
			int count=0;
			for(int deltaA=-1;deltaA<2;deltaA+=1)
			{
				for(int deltaI=-1;deltaI<2;deltaI+=1)
				{
					try
					{
						sum+=GetTileAt(i+deltaI,a+deltaA)->GetValue();
						count+=1;
					}
					catch(char* error){error;}//This will happen, it should just be ignored.
				}
			}
			double avg=sum/count;
			temp[i][a]=new Tile(avg);
		}
	}

	//assign Tiles to temp
	Tiles=temp;
}

void WorldMap::CreateTowns()
{
	int area=GetLandArea();
	//cout<<"land area: "<<area<<endl;
	int towns=(int)pow(area,1.0/4);
	//cout<<"towns: "<<towns<<endl;
	double waterlevel=WATERLEVEL;

	int rx=Width/2;
	int ry=Height/2;
	Tiles[rx][ry]->SetContainer(new Town("Town 1",1,"Beginner Town"));
	SpawnX=rx;
	SpawnY=ry;

	for(int i=0;i<towns;i+=1)
	{
		rx=rand()%Width;
		ry=rand()%Height;
		while((Tiles[rx][ry]->GetValue()<=WATERLEVEL)||(Tiles[rx][ry]->GetContainer()!=NULL))
		{
			rx=rand()%Width;
			ry=rand()%Height;
		}
		string townName="Town ";
		townName+=('2'+i);
		string specialty=Town::GetRandomSpecialty();
		if(i==0)
		{
			
		}
		else
		{
			Tiles[rx][ry]->SetContainer(new Town(townName,1,specialty));
		}
		
	}
}

void WorldMap::CreateCastles()
{
	int area=GetLandArea();
	//cout<<"land area: "<<area<<endl;
	int castles=(int)pow(area,1.0/8);
	//cout<<"towns: "<<towns<<endl;
	if(castles<1)
		castles=1;
	double waterlevel=WATERLEVEL;
	for(int i=0;i<castles;i+=1)
	{
		int rx=rand()%Width;
		int ry=rand()%Height;
		while((Tiles[rx][ry]->GetValue()<=WATERLEVEL)||(Tiles[rx][ry]->GetContainer()!=NULL))
		{
			rx=rand()%Width;
			ry=rand()%Height;
		}
		string castleName="Castle ";
		castleName+=('1'+i);
		Tiles[rx][ry]->SetContainer(new Castle(castleName,1));
		
	}
}

void WorldMap::CreateDungeons(int dungeons)
{
	double waterlevel=WATERLEVEL;
	for(int i=0;i<dungeons;i+=1)
	{
		int rx=rand()%Width;
		int ry=rand()%Height;
		while((Tiles[rx][ry]->GetValue()<=WATERLEVEL)||(Tiles[rx][ry]->GetContainer()!=NULL))
		{
			rx=rand()%Width;
			ry=rand()%Height;
		}
		string dungeonName="Dungeon ";
		dungeonName+=('1'+i);
		Tiles[rx][ry]->SetContainer(new Dungeon(dungeonName,Tiles[rx][ry]->GetLevel()));
	}
}

void WorldMap::SetTileLevels()
{
	for(int a=0;a<Height;a+=1)
	{
		for(int i=0;i<Width;i+=1)
		{
			int level=(int)(abs(SpawnX-i)+abs(SpawnY-a))+1;
			Tiles[i][a]->SetLevel(level);
		}
	}
}

int WorldMap::GetLandArea()
{
	int count=0;
	for(int a=0;a<Width;a+=1)
	{
		for(int i=0;i<Height;i+=1)
		{
			if(Tiles[a][i]->GetValue()>WATERLEVEL)
				count+=1;
		}
	}
	return(count);
}

ostream& operator<<(ostream& os, const WorldMap& map)
{
	os<<map.Width<<'\n';
	os<<map.Height<<'\n';
	os<<*map.Character<<'\n';
	for(int a=0;a<map.Height;a+=1)
	{
		for(int i=0;i<map.Width;i+=1)
		{
			os<<*map.Tiles[i][a]<<'\n';
		}
	}
	return(os);
}

istream& operator>>(istream& is, WorldMap& map)
{
	is>>map.Width>>map.Height;
	Player* p=new Player();
	is>>*p;
	map.Character=p;
	for(int a=0;a<map.Height;a+=1)
	{
		for(int i=0;i<map.Width;i+=1)
		{
			Tile* t=new Tile();
			is>>*t;
			map.Tiles[i].push_back(t);
		}
	}
	return(is);
}