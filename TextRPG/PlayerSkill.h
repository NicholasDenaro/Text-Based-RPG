/* PlayerSkill.h -
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include <sstream>
#include "Printer.h"
#include "Player.h"
using namespace std;

enum SkillType{ACTIVE,PASSIVE};
enum EffectType{STR_DMG,DEX_DMG,INT_DMG,LUK_DMG};
	
class PlayerSkill
{
public:
	PlayerSkill()
	{
		Name="nullskill";
	}
	PlayerSkill(string name, string skilltype, string effect, string value, string manaCost)
	{
		Name=name;
		//Type
		if(skilltype=="ACTIVE")
			Type=ACTIVE;
		else if(skilltype=="PASSIVE")
			Type=PASSIVE;

		//Active EffectType
		if(Type==ACTIVE)
		{
			if(effect=="STR_DMG")
				ActiveEffectType=STR_DMG;
			else if(effect=="DEX_DMG")
				ActiveEffectType=DEX_DMG;
			else if(effect=="INT_DMG")
				ActiveEffectType=INT_DMG;
			else if(effect=="LUK_DMG")
				ActiveEffectType=LUK_DMG;
		}
		if(Type==PASSIVE)
		{
			if(effect=="HEALTH")
				PassiveEffectType=HEALTH;
			else if(effect=="MANA")
				PassiveEffectType=MANA;
			else if(effect=="STRENGTH")
				PassiveEffectType=STRENGTH;
			else if(effect=="DEXTERITY")
				PassiveEffectType=DEXTERITY;
			else if(effect=="INTELLECT")
				PassiveEffectType=INTELLECT;
			else if(effect=="LUCK")
				PassiveEffectType=LUCK;
			else if(effect=="CHARISMA")
				PassiveEffectType=CHARISMA;
			else if(effect=="ARMOR")
				PassiveEffectType=ARMOR;
		}

		//Value
		stringstream ss;
		ss<<value;
		ss>>Value;

		ManaCost=Printer::StringToInt(manaCost);
		//cout<<"New skill: "<<Name<<endl;
	}

	string GetName() const
	{
		return(Name);
	}

	SkillType GetType() const
	{
		return(Type);
	}

	EffectType GetActiveEffectType() const
	{
		 return(ActiveEffectType);
	}

	PlayerStats GetPassiveEffectType() const
	{
		 return(PassiveEffectType);
	}

	double GetValue() const
	{
		return(Value);
	}

	int GetManaCost() const
	{
		return(ManaCost);
	}

	friend ostream& operator<<(ostream& os, const PlayerSkill& skill)
	{
		os<<skill.Name<<endl;
		cout<<"saved shit"<<endl;
		return(os);
	}

	friend istream& operator>>(istream& is, PlayerSkill& skill)
	{
		is>>skill.Name;
		string restofline;
		getline(is,restofline);
		skill.Name+=restofline;
		cout<<"loaded"<<endl;
		return(is);
	}

private:
	string Name;
	SkillType Type;
	EffectType ActiveEffectType;
	PlayerStats PassiveEffectType;
	double Value;
	int ManaCost;
};