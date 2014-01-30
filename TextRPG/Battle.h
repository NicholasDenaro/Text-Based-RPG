/* Battle.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <iostream>
#include <conio.h>
#include "Monster.h"
#include "Player.h"
#include "Printer.h"
using namespace std;

class Battle
{
public:
	Battle(){}

	Battle(Player* character, Monster& monster, bool turn)
	{
		Character=character;
		Enemy=monster;
		MyTurn=turn;
	}

	bool Run()
	{
		while((!Enemy.IsDead())&&(!Character->IsDead()))
		{
			Draw();
			cout<<endl;
			if(MyTurn)
			{
				map<string,PlayerSkill> skillsmap=Character->GetSkills();
				vector<PlayerSkill> skills;
				map<string,PlayerSkill>::iterator it=skillsmap.begin();
				while(it!=skillsmap.end())
				{
					if(it->second.GetType()==ACTIVE)
						skills.push_back(it->second);
					it++;
				}
				int tag;
				string input;
				do
				{
					//Print the skills
					for(unsigned int i=0;i<skills.size();i+=1)
					{
						cout<<i<<". "<<skills[i].GetName()<<": Damage mult.: "<<skills[i].GetValue()<<"x";
						Printer::pout<<Player::IntToStrStats(skills[i].GetActiveEffectType()+2)<<" Mana cost: "<<skills[i].GetManaCost()<<endl;
					}
					//get the skill
					cout<<"Enter the number of the skill you want to use: ";
					cin>>input;
					tag=Printer::StringToInt(input);
					while(tag<0||tag>=(signed int)skills.size())
					{
						Printer::MoveBackLine();
						Printer::ClearLine();
						cout<<"Enter the number of the skill you want to use: ";
						cin>>input;
						tag=Printer::StringToInt(input);
					}
					//check if there is enough mana
					if(skills[tag].GetManaCost()>Character->GetMana())
					{
						Printer::pout<<"#012You don't have enough mana for this skill."<<endl;
						system("pause");
						system("cls");
						Draw();
						cout<<endl;
					}
				}while(skills[tag].GetManaCost()>Character->GetMana());

				PlayerSkill skill=skills[tag];
				Character->UseMana(skill.GetManaCost());
				EffectType et=skill.GetActiveEffectType();
				double damage;
				if(et==STR_DMG)
					damage=Character->GetStat(STRENGTH);
				else if(et==DEX_DMG)
					damage=Character->GetStat(DEXTERITY);
				else if(et==INT_DMG)
					damage=Character->GetStat(INTELLECT);
				else if(et==LUK_DMG)
					damage=Character->GetStat(LUCK);

				damage*=skill.GetValue();//The multiplier
				damage=(int)(damage+0.5);

				if(damage==0)
					damage=1;//minimum damage;

				Printer::pout<<"#010You used "<<skill.GetName();Printer::pout<<"#010 and did ";
				Enemy.Damage((int)damage);
				cout<<damage;Printer::pout<<"#010 damage."<<endl;
  				system("pause");
			}
			else
			{
				MonsterSkill skill=Enemy.RandomSkill();
				Printer::pout<<"#012The "<<Enemy.GetName();Printer::pout<<"#012 used "<<skill.GetName();
				int damage=Character->Damage((int)(skill.GetDamage()+pow(Enemy.GetLevel()/4,1.3)));
				Printer::pout<<"#012 and did "<<damage;Printer::pout<<"#012 damage."<<endl;
				system("pause");
			}
			MyTurn=!MyTurn;
		}
		Draw();
		cout<<endl;
		if(Enemy.IsDead())
		{
			Printer::pout<<"#014You've defeated the "<<Enemy.GetName()<<"!"<<endl;
			Character->GainExperience(Enemy.GetLevel()*4);
			int rQulz=Enemy.GetLevel()*5;
			rQulz+=(int)(rQulz*(rand()*0.50/RAND_MAX-0.25));//*rand+-25%
			if(rQulz>0)
				Character->AddQulz(rQulz);
			Character->CheckQuest(Enemy);
			return(true);//victorious
		}
		else
		{
			Printer::pout<<"#004Whoops, you've died. The client will close now."<<endl;
			//figure out how to kick out of prgram...
			//GameFlow::Flow.SetRunning(false);
			system("pause");
			exit(0);
			return(false);//You've died.
		}
	}

	char GetUserInput()
	{
		//cout<<"Action: "<<endl;
		char c;
		c=toupper(_getch());
		//cin>>action;
		while(!Actions.IsValidAction(c))
		{
			//cout<<"Action invalid, please try agian."<<endl<<endl;
			//cout<<"Action: "<<endl;
			c=toupper(_getch());
		}
		return(c);
	}

	void Draw()
	{
		system("cls");
		Character->PrintInfo();
		//string name="#010"+Character->GetName();
		//Printer::pout<<name<<" Lvl. "<<Character->GetLevel()<<" Health: "<<Character->GetHealth()<<"/"<<Character->GetStat(HEALTH)<<endl;
		string name="#012"+Enemy.GetName();
		Printer::pout<<name<<" Lvl. "<<Enemy.GetLevel()<<" Health: "<<Enemy.GetHealth()<<"/"<<Enemy.GetMaxHealth()<<endl;

	}

private:
	ActionList Actions;
	Player* Character;
	Monster Enemy;
	bool MyTurn;
};