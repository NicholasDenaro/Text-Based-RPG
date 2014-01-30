/* RaceElf.h - The RaceHuman class
** Written by Nicholas Denaro
*/

#pragma once
#include <iostream>
#include "PlayerRace.h"
using namespace std;

class RaceElf:public PlayerRace
{
public:
	RaceElf():PlayerRace()
	{
		Bonuses=new int[6];
		Bonuses[HEALTH]=-7;
		Bonuses[MANA]=6;
		Bonuses[STRENGTH]=-6;
		Bonuses[DEXTERITY]=3;
		Bonuses[INTELLECT]=4;
		Bonuses[LUCK]=0;
		Bonuses[CHARISMA]=0;
	}
	
	void PrintDescription()
	{
		cout<<"Elves are fragile but possess great magics. Their bonuses are: "<<endl;
		cout<<"Health: "<<Bonuses[HEALTH]<<endl;
		cout<<"Mana: "<<Bonuses[MANA]<<endl;
		cout<<"Strength: "<<Bonuses[STRENGTH]<<endl;
		cout<<"Dexterity: "<<Bonuses[DEXTERITY]<<endl;
		cout<<"Intellect: "<<Bonuses[INTELLECT]<<endl;
		cout<<"Luck: "<<Bonuses[LUCK]<<endl;
		cout<<"Charisma: "<<Bonuses[CHARISMA]<<endl;
	}
};