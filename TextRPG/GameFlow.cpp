/* GameFlow.cpp - 
** Written by Nicholas Denaro
*/

#include <conio.h>
#include "GameFlow.h"
#include "Town.h"
#include "Castle.h"
#include <time.h>
using namespace std;

ActionList GameFlow::MainMenuActions("[N] New Game,[L] Load Game,[T] Tutorial");

#define NEWGAMECHAR 'N'
#define LOADGAMECHAR 'L'
#define TUTORIALCHAR 'T'

#define INVENTORYCHAR 'B'
#define EXPLORECHAR 'E'
#define TRAINCHAR 'T'
#define ENTERTILECHAR 'R'
#define TRAVELNORTHCHAR 'W'
#define TRAVELWESTCHAR 'A'
#define TRAVELSOUTHCHAR 'S'
#define TRAVELEASTCHAR 'D'

#define EQUIPCHAR 'E'
#define USEITEMCHAR 'U'
#define STATSCHAR 'A'
#define SKILLSCHAR 'S'

GameFlow GameFlow::Flow;

GameFlow::GameFlow()
{
	Process=MAINMENU;
	CurrentActions=MainMenuActions;
	Running=false;
	Flow=*this;
	Map=NULL;
}

GameFlow::~GameFlow()
{
	//breaks?
	/*if(Map!=NULL)
		delete Map;*/
}

void GameFlow::Run()
{
	Running=true;
	while(Running)
	{
		try
		{
			char action;

			GetAvailableActions();

			UpdateActions();

			action=GetUserInput();
		
			ProcessInput(action);
		}
		catch(char* error)
		{
			cout<<error<<endl;
			Running=false;
		}
	}
}

void GameFlow::SetRunning(bool run)
{
	Running=run;
}

char GameFlow::GetUserInput()
{
	char c;
	c=toupper(_getch());
	while(!CurrentActions.IsValidAction(c))
	{
		c=toupper(_getch());
	}
	return(c);
}

char GameFlow::GetUserInput(ActionList actions)
{
	char c;
	c=toupper(_getch());
	while(!actions.IsValidAction(c))
	{
		c=toupper(_getch());
	}
	return(c);
}

void GameFlow::ProcessInput(char action)
{
	int x;
	int y;
	Battle b;
	int level;
	int r;
	vector<string> monsters;
	Monster monster;
	Player* player;
	int tag=0;
	string input;
	bool alive;
	string fname;
	string tempname;
	switch(Process)
	{
		case MAINMENU:
		{
			switch(action)
			{	
				case NEWGAMECHAR://New Game
				{
					system("cls");
					cout<<"Enter map size(12-24): ";
					int size;
					cin>>size;
					while(size<12||size>24)
					{
						system("cls");
						cout<<"Invalid Size\nEnter map size(12-24): ";
						cin>>size;
					}
					int seed=time(NULL);
					Map=new WorldMap(size,size,seed);
					WorldMap::Map=Map;
					Map->BuildWorld();
					Map->SetCharacter(CreateCharacter());
					Process=WORLDMAP;
					GetAvailableActions();
					system("cls");
					Map->Draw();
				}
				break;
				case LOADGAMECHAR:
					cout<<"Enter the filename you wish to load: ";
					cin>>fname;
					getline(cin,tempname);
					fname+=tempname;
					if(Map->Load(fname))
					{
						Map=WorldMap::Map;
						Process=WORLDMAP;
						GetAvailableActions();
						system("cls");
						Map->Draw();
					}
					else
					{
						system("cls");
						cout<<"Welcome to TextRPG"<<endl;
					}
				break;
				case TUTORIALCHAR:
					cout<<"Tips: "<<endl;
					cout<<"-Change the size of the font to 8x8, and the window size to 80x60"<<endl;
					cout<<"-Don't die, it'll close the game and put you back to your last save."<<endl;
					cout<<"-Qulz is the currency."<<endl;
					cout<<"-To heal, buy some potions or rest at an inn."<<endl;
					cout<<"-You gain a skill point at lvl 5,10,15,etc."<<endl;
					cout<<"-You can't escape from battle, so be careful."<<endl;
					cout<<"-To win, defeat a lvl 30 dungeon (Their level increases by 2 every time they're reset)."<<endl;
					system("pause");
					system("cls");
					cout<<"Welcome to TextRPG"<<endl;
				break;
			}//end switch Action
		}//end case
		break;
		case WORLDMAP:
		{
			switch(action)
			{
				case INVENTORYCHAR:
					system("cls");
					Process=PAUSEMENU;
					Map->GetCharacter()->PrintInventory();
				break;
				case EXPLORECHAR://Explore
					Map->Explore();
				break;
				case TRAINCHAR://Train
					Printer::pout<<"#014Battle!"<<endl;
					system("pause");
					system("cls");
					//random level
					level=Map->GetCurrentTile()->GetLevel()+rand()%3-1;
					if(level<=0)
						level=1;
					b=Battle(Map->GetCharacter(),Monster(Monster::GetRandomMonster(level),level),true);
					b.Run();
					system("pause");
					system("cls");
					Map->Draw();
				break;
				case ENTERTILECHAR://Enter town
					Process=TILECONTAINER;
					//system("pause");
					system("cls");
					Map->GetCharacter()->PrintInfo();
					Map->GetCurrentTile()->GetContainer()->DrawInfo();
				break;
				case TRAVELNORTHCHAR://Travel North
					x=Map->GetCharacter()->GetX();
					y=Map->GetCharacter()->GetY();
					Map->MoveCharacter(0,-1);
					Map->UpdateTile(x,y);
					Map->UpdateTile(Map->GetCharacter()->GetX(),Map->GetCharacter()->GetY());
					Map->UpdateTileInfo();
				break;
				case TRAVELSOUTHCHAR://Travel South
					x=Map->GetCharacter()->GetX();
					y=Map->GetCharacter()->GetY();
					Map->MoveCharacter(0,1);
					Map->UpdateTile(x,y);
					Map->UpdateTile(Map->GetCharacter()->GetX(),Map->GetCharacter()->GetY());
					Map->UpdateTileInfo();
				break;
				case TRAVELEASTCHAR://Travel East
					x=Map->GetCharacter()->GetX();
					y=Map->GetCharacter()->GetY();
					Map->MoveCharacter(1,0);
					Map->UpdateTile(x,y);
					Map->UpdateTile(Map->GetCharacter()->GetX(),Map->GetCharacter()->GetY());
					Map->UpdateTileInfo();
				break;
				case TRAVELWESTCHAR://Travel West
					x=Map->GetCharacter()->GetX();
					y=Map->GetCharacter()->GetY();
					Map->MoveCharacter(-1,0);
					Map->UpdateTile(x,y);
					Map->UpdateTile(Map->GetCharacter()->GetX(),Map->GetCharacter()->GetY());
					Map->UpdateTileInfo();
				break;
			}//end switch Action
		}//end case
		break;
		case TILECONTAINER:
		{
			if(CurrentActions[action]=="Leave")
			{
				Process=WORLDMAP;
				system("cls");
				Map->Draw();
			}
			else if(CurrentActions[action].substr(0,CurrentActions[action].find('('))=="Rest at Inn")
			{
				int paren=CurrentActions[action].find('(');
				int end=CurrentActions[action].find(' ',paren+1);
				int cost=Printer::StringToInt(CurrentActions[action].substr(paren+1,end))-Map->GetCharacter()->GetStat(CHARISMA)*5;
				if(Map->GetCharacter()->GetQulz()>=cost)
				{
					Map->GetCharacter()->FullRestore();
					Printer::pout<<"#014Health and Mana fully restored!"<<endl;
					Map->GetCharacter()->AddQulz(-cost);
					//SAVE THE GAME!
					Map->Save();
					Printer::pout<<"#011Game saved!"<<endl;
				}
				else
				{
					Printer::pout<<"#012Not enough Qulz to rest at Inn!"<<endl;
				}
				Process=TILECONTAINER;
				system("pause");
				system("cls");
				Map->GetCharacter()->PrintInfo();
				Map->GetCurrentTile()->GetContainer()->DrawInfo();
			}
			else if(CurrentActions[action]=="Shop at Market")
			{
				//Do market things, like draw items you can buy.
				system("cls");
				Shop();
				Process=TILECONTAINER;
				system("cls");
				Map->GetCharacter()->PrintInfo();
				Map->GetCurrentTile()->GetContainer()->DrawInfo();
			}
			else if(CurrentActions[action]=="Get Quest from Guild HQ")
			{
				Map->GetCharacter()->AssignQuest(Quest::RandomQuest(Map->GetCharacter()));
				Process=TILECONTAINER;
				system("pause");
				system("cls");
				Map->GetCharacter()->PrintInfo();
				Map->GetCurrentTile()->GetContainer()->DrawInfo();
			}
			else if(CurrentActions[action]=="Reset Dungeon")
			{
				Map->GetCurrentTile()->ResetExploration();
				if(Map->GetCurrentTile()->GetLevel()<30)
					Map->GetCurrentTile()->SetLevel(Map->GetCurrentTile()->GetLevel()+2);
				Process=WORLDMAP;
				system("cls");
				Map->Draw();
			}
		}
		break;
		case PAUSEMENU:
		{
			switch(action)
			{
				case EQUIPCHAR://Equipment
					while(tag!=-1)
					{
						cout<<"Enter the item you wish to equip(-1 to exit): ";
						cin>>input;
						tag=Printer::StringToInt(input);
						while((tag<-1)||(tag>=(signed int)Map->GetCharacter()->GetInventory().size())||cin.fail())
						{
							Printer::MoveBackLine();
							Printer::ClearLine();
							cout<<"Enter the item you wish to equip(-1 to exit): ";
							cin>>input;
							tag=Printer::StringToInt(input);
						}
						if(tag!=-1)
						{
							if(Map->GetCharacter()->EquipItem(Map->GetCharacter()->GetInventory()[tag])==true)
							{
								Map->GetCharacter()->RemoveItem(tag);
							}
						}
						system("pause");
						system("cls");
						Map->GetCharacter()->PrintInventory();
					}
					system("cls");
					Process=PAUSEMENU;
					Map->GetCharacter()->PrintInventory();
				break;
				case USEITEMCHAR://Use Item
					while(tag!=-1)
					{
						cout<<"Enter the item you wish to equip(-1 to exit): ";
						cin>>input;
						tag=Printer::StringToInt(input);
						while((tag<-1)||(tag>=(signed int)Map->GetCharacter()->GetInventory().size())||cin.fail())
						{
							Printer::MoveBackLine();
							Printer::ClearLine();
							cout<<"Enter the item you wish to equip(-1 to exit): ";
							cin>>input;
							tag=Printer::StringToInt(input);
						}
						if(tag!=-1)
						{
							if(Map->GetCharacter()->UseItem(Map->GetCharacter()->GetInventory()[tag])==true)
							{
								Map->GetCharacter()->RemoveItem(tag);
							}
						}
						system("pause");
						system("cls");
						Map->GetCharacter()->PrintInventory();
					}
					system("cls");
					Process=PAUSEMENU;
					Map->GetCharacter()->PrintInventory();
				break;
				case STATSCHAR://stats
					tag=0;
					while(tag!=-1)
					{
						cout<<"Enter the number of the stat you wish to upgrade (-1 to exit): ";
						cin>>input;
						tag=Printer::StringToInt(input);
						while((tag<-1)||(tag>=(signed int)Map->GetCharacter()->GetStats().size()-1))
						{
							Printer::MoveBackLine();
							Printer::ClearLine();
							cout<<"Enter the number of the stat you wish to upgrade (-1 to exit): ";
							cin>>input;
							tag=Printer::StringToInt(input);
						}
						if(tag!=-1)
						{
							if(Map->GetCharacter()->GetStatPoints()>0)
							{
								if(Map->GetCharacter()->UpgradeStat(tag)==true)
								{
									Printer::pout<<"#014You've upgraded: ";
									Printer::pout<<Player::IntToStrStats(tag)<<endl;
									Map->GetCharacter()->AddStatPoints(-1);
								}
							}
							else
							{
								Printer::pout<<"#012You don't have enough stat points."<<endl;
							}
						}
						system("pause");
						system("cls");
						Map->GetCharacter()->PrintInventory();
					}//end while
					system("cls");
					Process=PAUSEMENU;
					Map->GetCharacter()->PrintInventory();
				break;
				case SKILLSCHAR://skills
					system("cls");
					tag=0;
					while(tag!=-1)
					{
						//get the available skills
						map<string,PlayerSkill> skills=Map->GetCharacter()->GetSkills();
						map<string,PlayerSkill>::iterator it=skills.begin();
						map<string,PlayerSkill> availableSkillsMap;
						vector<PlayerSkill> availableSkills;
						PlayerSkillNode* root=Map->GetCharacter()->GetClass().GetSkills();
						while(it!=skills.end())
						{
							if(it->second.GetName()!="nullskill")
							{
								vector<PlayerSkillNode*> temp=root->FindSkillChildren(it->first);
								for(unsigned int i=0;i<temp.size();i+=1)
								{
									availableSkillsMap[temp[i]->GetSkill()->GetName()]=*temp[i]->GetSkill();
								}
							}
							it++;
						}
						it=availableSkillsMap.begin();
						while(it!=availableSkillsMap.end())
						{
							if(Map->GetCharacter()->GetSkill(it->first).GetName()=="nullskill")
								availableSkills.push_back(it->second);
							it++;
						}

						cout<<"Available SkillPoints: "<<Map->GetCharacter()->GetSkillPoints()<<endl;
						cout<<"Available skills: "<<endl;
						for(int i=0;i<(signed int)availableSkills.size();i+=1)
						{
							if(availableSkills[i].GetType()==ACTIVE)
							{
								cout<<i<<". "<<availableSkills[i].GetName()<<": Damage mult.: "<<availableSkills[i].GetValue()<<"x";
								Printer::pout<<Player::IntToStrStats(availableSkills[i].GetActiveEffectType()+2)<<" Mana cost: "<<availableSkills[i].GetManaCost()<<endl;
							}
							else
							{
								cout<<i<<". "<<availableSkills[i].GetName()<<": Upgrade: "<<availableSkills[i].GetValue()<<" ";
								Printer::pout<<Player::IntToStrStats(availableSkills[i].GetPassiveEffectType())<<endl;
							}
						}

						cout<<"Enter the number of the skill you wish to gain (-1 to exit): ";
						cin>>input;
						tag=Printer::StringToInt(input);
						while((tag<-1)||(tag>=(signed int)availableSkills.size()))
						{
							Printer::MoveBackLine();
							Printer::ClearLine();
							cout<<"Enter the number of the skill you wish to gain (-1 to exit): ";
							cin>>input;
							tag=Printer::StringToInt(input);
						}
						if(tag!=-1)
						{
							if(Map->GetCharacter()->GetSkillPoints()>0)
								Map->GetCharacter()->AddSkill(availableSkills[tag]);
							else
								Printer::pout<<"#012You don't have enough skill points."<<endl;
						}
						system("pause");
						system("cls");
					}//end while
					Process=PAUSEMENU;
					Map->GetCharacter()->PrintInventory();
				break;
				case 'X':
					Process=WORLDMAP;
					system("cls");
					Map->Draw();
				break;
			}
		}
	}//end switch Process
}

void GameFlow::UpdateActions(/*bool redraw*/)
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&SBInfo);
	//if(redraw)
	{
		
		for(int i=0;i<10;i+=1)
			cout<<string(50,' ')<<endl;
	}
	COORD cur ={SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	CurrentActions.PrintActions();
}

void GameFlow::GetAvailableActions()// const
{
	//CurrentActions.Clear();
	switch(Process)
	{
		case WORLDMAP:
		{
			CurrentActions.Clear();
			stringstream ss;
			ss<<"["<<INVENTORYCHAR<<"] Inventory";
			CurrentActions.AddAction(ss.str());
			if(Map->GetExplored()<100)
			{
				ss.str("");
				ss<<"["<<EXPLORECHAR<<"] Explore";
				CurrentActions.AddAction(ss.str());
			}
			if(Map->GetCurrentTile()->GetContainer()==NULL)
			{
				ss.str("");
				ss<<"["<<TRAINCHAR<<"] Train";
				CurrentActions.AddAction(ss.str());
			}
			else
			{
				ss.str("");
				ss<<"["<<ENTERTILECHAR<<"] Enter "<<Map->GetCurrentTile()->GetContainer()->GetName();
				CurrentActions.AddAction(ss.str());
			}
			
			try{
				if(Map->GetCurrentTile(0,-1)->IsAvailable())
				{
					ss.str("");
					ss<<"["<<TRAVELNORTHCHAR<<"] Travel North";
					CurrentActions.AddAction(ss.str());
				}
			}
			catch(char* error){error;}
			try{
				if(Map->GetCurrentTile(0,1)->IsAvailable())
				{
					ss.str("");
					ss<<"["<<TRAVELSOUTHCHAR<<"] Travel South";
					CurrentActions.AddAction(ss.str());
				}
			}
			catch(char* error){error;}
			try{
				if(Map->GetCurrentTile(1,0)->IsAvailable())
				{
					ss.str("");
					ss<<"["<<TRAVELEASTCHAR<<"] Travel East";
					CurrentActions.AddAction(ss.str());
				}
			}
			catch(char* error){error;}
			try{
				if(Map->GetCurrentTile(-1,0)->IsAvailable())
				{
					ss.str("");
					ss<<"["<<TRAVELWESTCHAR<<"] Travel West";
					CurrentActions.AddAction(ss.str());
				}
			}
			catch(char* error){error;}
		}//end case
		break;
		case TILECONTAINER:
		{
			CurrentActions.Clear();
			CurrentActions=ActionList(Map->GetCurrentTile()->GetContainer()->GetActions());
		}//end case
		break;
		case PAUSEMENU:
		{
			CurrentActions.Clear();
			stringstream ss;
			
			CurrentActions.AddAction("[X] Leave");
			ss<<"["<<EQUIPCHAR<<"] Change Equipment";
			CurrentActions.AddAction(ss.str());
			ss.str("");
			ss<<"["<<USEITEMCHAR<<"] Use Item";
			CurrentActions.AddAction(ss.str());
			ss.str("");
			ss<<"["<<STATSCHAR<<"] Upgrade Stats";
			CurrentActions.AddAction(ss.str());
			ss.str("");
			ss<<"["<<SKILLSCHAR<<"] Upgrade Skills";
			CurrentActions.AddAction(ss.str());
		}
		break;
	}//end switch Process
}

void GameFlow::Draw()
{
	
}

void GameFlow::Shop()
{
	ActionList actions;
	actions.AddAction("[B] Buy");
	actions.AddAction("[S] Sell");
	actions.AddAction("[X] Leave");
	Map->GetCharacter()->PrintInfo();
	actions.PrintActions();
	char action=GetUserInput(actions);
	while(action!='X')
	{
		system("cls");
		Map->GetCharacter()->PrintInfo();
		//cout<<"Market:"<<endl;
		if(action=='B')
		{
			//set the level of the items available
			int highlevel=Map->GetCurrentTile()->GetLevel()+4;
			int lowlevel=Map->GetCurrentTile()->GetLevel()-4;
			if(lowlevel<1)
				lowlevel=1;

			double markup=3;
			if(Map->GetCurrentTile()->GetContainer()->GetType()==TOWN)
			{
				Town* t=(Town*)Map->GetCurrentTile()->GetContainer();
				if(t->GetSpecialty()=="Market")
				{
					markup=1.5;
					if(Map->GetCharacter()->GetLevel()>highlevel)
						highlevel+=(Map->GetCharacter()->GetLevel()-highlevel)/2;
				}
			}
			else if(Map->GetCurrentTile()->GetContainer()->GetType()==CASTLE)
			{
				markup=2;
				if(Map->GetCharacter()->GetLevel()>highlevel)
					highlevel=Map->GetCharacter()->GetLevel();
			}
			
			//get the items from the level, adding them to possible items
			map<string,Item> tempitemMap;
			for(int i=lowlevel;i<=highlevel;i+=1)
			{
				vector<string> items=Item::ItemLevelMap[i];
				for(unsigned int j=0;j<items.size();j+=1)
					tempitemMap[items[j]]=Item::ItemMap[items[j]];
			}
			//add them to a vector
			vector<Item> sellingItems;
			map<string,Item>::iterator it=tempitemMap.begin();
			while(it!=tempitemMap.end())
			{
				sellingItems.push_back(it->second);
				it++;
			}
			int tag=0;
			while(tag!=-1)
			{
				cout<<"Buying:"<<endl;
				//print the items
				for(unsigned int i=0;i<sellingItems.size();i+=1)
				{
					int price=(int)(sellingItems[i].GetWorth()*markup);
					cout<<i<<". "<<sellingItems[i].GetName()<<" - "<<price<<"Qulz"<<endl;
				}
				//select item
				string input;
				cout<<"Enter the item number you wish to buy(-1 to exit): ";
				cin>>input;
				tag=Printer::StringToInt(input);
				while((tag<(-1))||(tag>=(signed int)sellingItems.size()))
				{
					Printer::MoveBackLine();
					Printer::ClearLine();
					cout<<"Enter the item number you wish to buy(-1 to exit): ";
					cin>>input;
					tag=Printer::StringToInt(input);
				}
				//check to see if you can buy
				if(tag!=-1)
				{
					if(Map->GetCharacter()->GetQulz()>=sellingItems[tag].GetWorth()*markup)
					{
						if(Map->GetCharacter()->AddItem(sellingItems[tag]))
						{
							Map->GetCharacter()->AddQulz((int)(-sellingItems[tag].GetWorth()*markup));
							Printer::pout<<"#014You've purchased the "<<sellingItems[tag].GetName()<<endl;
						}
					}
					else
					{
						Printer::pout<<"#014You don't have enough Qulz to purchase that item"<<endl;
					}
					system("pause");
					system("cls");
					Map->GetCharacter()->PrintInfo();
				}
			}//end while
			system("cls");
			Map->GetCharacter()->PrintInfo();
		}//end buying
		if(action=='S')
		{
			cout<<"Selling:"<<endl;
			double markdown=0.5;
			if(Map->GetCurrentTile()->GetContainer()->GetType()==TOWN)
			{
				Town* t=(Town*)Map->GetCurrentTile()->GetContainer();
				if(t->GetSpecialty()=="Market")
					markdown=1;
			}
			else if(Map->GetCurrentTile()->GetContainer()->GetType()==CASTLE)
				markdown=0.75;
			int tag=0;
			while(tag!=-1)
			{
				vector<Item> inv=Map->GetCharacter()->GetInventory();
				for(unsigned int i=0;i<inv.size();i+=1)
				{
					cout<<i<<". "<<inv[i].GetName()<<" - "<<inv[i].GetWorth()*markdown<<"Qulz"<<endl;
				}
				cout<<"Enter the item number you wish to buy(-1 to exit): ";
				string input;
				cin>>input;
				tag=Printer::StringToInt(input);
				while((tag<(-1))||(tag>=(signed int)inv.size()))
				{
					Printer::MoveBackLine();
					Printer::ClearLine();
					cout<<"Enter the item number you wish to buy(-1 to exit): ";
					cin>>input;
					tag=Printer::StringToInt(input);
				}
				if(tag!=-1)
				{
					Item i=inv[tag];
					int price=(int)(i.GetWorth()*markdown);
					Printer::pout<<"#014Sold "<<i.GetName()<<endl;
					Map->GetCharacter()->AddQulz(price);
					Map->GetCharacter()->RemoveItem(tag);
				}
				system("pause");
				system("cls");
				Map->GetCharacter()->PrintInfo();
			}//end while
		}//end selling
		actions.Clear();
		actions.AddAction("[B] Buy");
		actions.AddAction("[S] Sell");
		actions.AddAction("[X] Leave");
		actions.PrintActions();
		action=GetUserInput(actions);
	}
}

Player* GameFlow::CreateCharacter()
{
	system("cls");

	cout<<"Select a Race: "<<endl;
	map<string,PlayerRace>::iterator it1=PlayerRace::RaceMap.begin();

	vector<string> tempActions;
	while(it1!=PlayerRace::RaceMap.end())
	{
		string action="[";
		action+=it1->second.GetChar();
		action+="] ";
		action+=it1->second.GetName();

		tempActions.push_back(action);
		cout<<"["<<it1->second.GetChar()<<"] ";
		it1++->second.PrintDescription();
		cout<<endl;
	}

	CurrentActions=ActionList(tempActions);
	CurrentActions.PrintActions();
	char action=GetUserInput();
	PlayerRace pr=PlayerRace::RaceMap[CurrentActions[action]];

	system("cls");

	cout<<"Select a class: "<<endl;
	map<string,PlayerClass>::iterator it2=PlayerClass::ClassMap.begin();
	tempActions.clear();
	while(it2!=PlayerClass::ClassMap.end())
	{
		string action="[";
		action+=it2->second.GetChar();
		action+="] ";
		action+=it2->second.GetName();
		
		tempActions.push_back(action);
		cout<<"["<<it2->second.GetChar()<<"] "<<it2->second.GetName();
		it2++;
		cout<<endl;
	}

	CurrentActions=ActionList(tempActions);
	//CurrentActions.PrintActions();
	action=GetUserInput();
	PlayerClass pc=PlayerClass::ClassMap[CurrentActions[action]];

	system("cls");

	cout<<"Enter a name for your character: ";
	string name;
	cin>>name;
	string tempname;
	getline(cin,tempname);
	name+=tempname;
	//getline(cin,name);

	//Player player(name,pr,pc);
	//Player player(name,PlayerRace::RaceMap[CurrentActions[action]],PlayerClass::ClassMap[CurrentActions[action]]);

	Player* player=new Player(name,pr,pc);
	Player::Character=player;
	return(player);
}