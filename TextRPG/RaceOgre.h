/* RaceOgre.h - The RaceOgre prototype
** Written by Nicholas Denaro
*/

#pragma once
#include "PlayerRace.h"
#include <iostream>
using namespace std;

class RaceOgre:public PlayerRace
{
public:
	RaceOgre():PlayerRace()
	{
		Bonuses=new int[6];
		Bonuses[HEALTH]=5;
		Bonuses[MANA]=-5;
		Bonuses[STRENGTH]=10;
		Bonuses[DEXTERITY]=-4;
		Bonuses[INTELLECT]=-3;
		Bonuses[LUCK]=-3;
		Bonuses[CHARISMA]=-5;
	}
	
	void PrintDescription()
	{
		cout<<"Ogres are very clumsy, and dull; yet immensly powerful. Their bonuses are: "<<endl;
		cout<<"Health: "<<Bonuses[HEALTH]<<endl;
		cout<<"Mana: "<<Bonuses[MANA]<<endl;
		cout<<"Strength: "<<Bonuses[STRENGTH]<<endl;
		cout<<"Dexterity: "<<Bonuses[DEXTERITY]<<endl;
		cout<<"Intellect: "<<Bonuses[INTELLECT]<<endl;
		cout<<"Luck: "<<Bonuses[LUCK]<<endl;
		cout<<"Charisma: "<<Bonuses[CHARISMA]<<endl;
	}
};