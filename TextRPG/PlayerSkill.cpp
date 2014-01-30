/* PlayerSkill.cpp -
** Written by Nicholas Denaro
*/

/*
#include "PlayerSkill.h"
using namespace std;
	
PlayerSkill::PlayerSkill()
{
	Name="nullskill";
}
PlayerSkill::PlayerSkill(string name, string skilltype, string effect, string value, string manaCost)
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

string PlayerSkill::GetName() const
{
	return(Name);
}

SkillType PlayerSkill::GetType() const
{
	return(Type);
}

EffectType PlayerSkill::GetActiveEffectType() const
{
		return(ActiveEffectType);
}

PlayerStats PlayerSkill::GetPassiveEffectType() const
{
		return(PassiveEffectType);
}

double PlayerSkill::GetValue() const
{
	return(Value);
}

int PlayerSkill::GetManaCost() const
{
	return(ManaCost);
}

ostream& operator<<(ostream& os, const PlayerSkill& skill)
{
	os<<skill.Name<<endl;
	cout<<"saved shit"<<endl;
	return(os);
}

istream& operator>>(istream& is, PlayerSkill& skill)
{
	is>>skill.Name;
	//getline(is,skill.Name);
	cout<<"loaded"<<endl;
	return(is);
}
*/