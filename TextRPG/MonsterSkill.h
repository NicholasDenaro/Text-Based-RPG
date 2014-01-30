/* MonsterSkill.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include <sstream>
using namespace std;

class MonsterSkill
{
public:
	MonsterSkill(){}
	MonsterSkill(string name, string damage)
	{
		Name=name;
		stringstream ss;
		ss<<damage;
		ss>>Damage;
	}

	string GetName()
	{
		return(Name);
	}

	int GetDamage()
	{
		return(Damage);
	}

	friend ostream& operator<<(ostream& os, MonsterSkill& skill)
	{
		os<<skill.Name<<'\n';
		os<<skill.Damage;
		return(os);
	}

	friend istream& operator>>(istream& is, MonsterSkill& skill)
	{
		string tempname;
		is>>skill.Name;
		getline(is,tempname);
		skill.Name+=tempname;
		is>>skill.Damage;
		return(is);
	}

private:
	string Name;
	int Damage;
};