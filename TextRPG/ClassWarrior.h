/* ClassWarrior.h - ClassWarrior prototype
** Written by Nicholas Denaro
*/

#pragma once
#include "PlayerClass.h"

class ClassWarrior:public PlayerClass
{
public:
	ClassWarrior(char* name):PlayerClass()
	{
		cout<<name<<endl;
		cout<<"loading skills..."<<endl;
		Skills=LoadSkills("WarriorSkills.xml");
		cout<<"skills loaded!"<<endl;
	}

};