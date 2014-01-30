/* Monster.cpp - 
** Written by Nicholas Denaro
*/

#pragma once
#include <math.h>
#include <iostream>
#include "Monster.h"
#include "rapidxml-1.13\rapidxml.hpp"
#include "rapidxml-1.13\rapidxml_utils.hpp"
#include "Printer.h"
using namespace std;

Monster::Monster(){}

Monster::Monster(string name, int baseHealth)
{
	Name=name;
	BaseHealth=baseHealth;
	Skills=vector<MonsterSkill>(0);
}

Monster::Monster(Monster& monster,int level)
{
	Name=monster.Name;
	BaseHealth=monster.BaseHealth;
	Level=level;
	MaxHealth=BaseHealth*Level/2;
	Health=MaxHealth;
	Skills=monster.Skills;
}

string Monster::GetName() const
{
	return(Name);
}

int Monster::GetHealth() const
{
	return(Health);
}

int Monster::GetMaxHealth() const
{
	return(MaxHealth);
}

int Monster::GetLevel() const
{
	return(Level);
}

map<string,Monster> Monster::MonsterMap;
map<int,vector<string>> Monster::MonsterLevelMap;

map<string,Monster> Monster::BossMap;
map<int,vector<string>> Monster::BossLevelMap;

void Monster::LoadMonsters(string dir, string fname)
{
	string path=dir+fname;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;// character type defaults to char
	doc.parse<0>(xmlFile.data());// 0 means default parse flags
	rapidxml::xml_node<>* node=doc.first_node("Monsters");
	rapidxml::xml_node<>* monsterNode;
	rapidxml::xml_node<>* skillNode;
	while(node->first_node()!=0)
	{
		monsterNode=node->first_node();
		string name=monsterNode->first_attribute("Name")->value();
		int baseHealth=Printer::StringToInt(monsterNode->first_attribute("BaseHealth")->value());
		int lowLevel=Printer::StringToInt(monsterNode->first_attribute("LowLevel")->value());
		int highLevel=Printer::StringToInt(monsterNode->first_attribute("HighLevel")->value());
		Monster monster=Monster(name,baseHealth);
		for(int i=lowLevel;i<=highLevel;i+=1)
		{
			MonsterLevelMap[i].push_back(monster.GetName());
		}
		while(monsterNode->first_node()!=0)
		{
			skillNode=monsterNode->first_node();
			monster.AddSkill(MonsterSkill(skillNode->first_attribute("Name")->value(),skillNode->first_attribute("Damage")->value()));
			monsterNode->remove_first_node();
		}
		MonsterMap[name]=monster;
		node->remove_first_node();
	}
}

void Monster::LoadBosses(string dir, string fname)
{
	string path=dir+fname;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;// character type defaults to char
	doc.parse<0>(xmlFile.data());// 0 means default parse flags
	rapidxml::xml_node<>* node=doc.first_node("Bosses");
	rapidxml::xml_node<>* monsterNode;
	rapidxml::xml_node<>* skillNode;
	while(node->first_node()!=0)
	{
		monsterNode=node->first_node();
		string name=monsterNode->first_attribute("Name")->value();
		int baseHealth=Printer::StringToInt(monsterNode->first_attribute("BaseHealth")->value());
		int lowLevel=Printer::StringToInt(monsterNode->first_attribute("LowLevel")->value());
		int highLevel=Printer::StringToInt(monsterNode->first_attribute("HighLevel")->value());
		Monster monster=Monster(name,baseHealth);
		for(int i=lowLevel;i<=highLevel;i+=1)
		{
			BossLevelMap[i].push_back(monster.GetName());
		}
		while(monsterNode->first_node()!=0)
		{
			skillNode=monsterNode->first_node();
			monster.AddSkill(MonsterSkill(skillNode->first_attribute("Name")->value(),skillNode->first_attribute("Damage")->value()));
			monsterNode->remove_first_node();
		}
		BossMap[name]=monster;
		node->remove_first_node();
	}
}

map<string,Monster> Monster::GetLevelRange(int low, int high)
{
	map<string,Monster> monsters;
	for(int i=low;i<=high;i+=1)
	{
		vector<string> levelGroup=MonsterLevelMap[i];
		string name;
		for(unsigned int j=0;j<levelGroup.size();j+=1)
		{
			name=levelGroup[j];
			monsters[name]=MonsterMap[name];
		}
	}
	return(monsters);
}

map<string,Monster> Monster::GetBossLevelRange(int low, int high)
{
	map<string,Monster> monsters;
	for(int i=low;i<=high;i+=1)
	{
		vector<string> levelGroup=BossLevelMap[i];
		string name;
		for(unsigned int j=0;j<levelGroup.size();j+=1)
		{
			name=levelGroup[j];
			monsters[name]=BossMap[name];
		}
	}
	return(monsters);
}

Monster& Monster::GetRandomMonster(int level)
{
	vector<string> monsters=MonsterLevelMap[level];
	int r=rand()%monsters.size();
	//random monster from the level
	return(Monster::MonsterMap[monsters[r]]);
}

void Monster::AddSkill(MonsterSkill skill)
{
	Skills.push_back(skill);
}

bool Monster::IsDead() const
{
	return(Health<=0);
}

void Monster::Damage(int damage)
{
	Health-=damage;
	if(Health>MaxHealth)
		Health=MaxHealth;
	if(Health<0)
		Health=0;
}

MonsterSkill Monster::RandomSkill() const
{
	int r=rand()%Skills.size();
	return(Skills[r]);
}

ostream& operator<<(ostream& os, Monster& monster)
{
	os<<monster.Name<<'\n';
	os<<monster.BaseHealth<<'\n';
	os<<monster.Level<<'\n';
	os<<monster.Skills.size()<<'\n';
	for(unsigned int i=0;i<monster.Skills.size();i+=1)
	{
		os<<monster.Skills[i]<<'\n';
	}
	os<<monster.MaxHealth<<'\n';
	os<<monster.Health;
	return(os);
}

istream& operator>>(istream& is, Monster& monster)
{
	int size;
	string tempname;
	is>>monster.Name;
	getline(is,tempname);
	monster.Name+=tempname;
	is>>monster.BaseHealth>>monster.Level>>size;
	for(int i=0;i<size;i+=1)
	{
		MonsterSkill ms;
		is>>ms;
		monster.Skills.push_back(ms);
	}
	is>>monster.MaxHealth>>monster.Health;
	return(is);
}