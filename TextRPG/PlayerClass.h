/* PlayerClass.h - PlayerClass prototype
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include "PlayerSkill.h"
#include "PlayerSkillNode.h"
#include <iostream>
#include <fstream>
#include <map>
#include "rapidxml-1.13\rapidxml.hpp"
#include "rapidxml-1.13\rapidxml_utils.hpp"
using namespace std;


class PlayerClass
{
public:
	PlayerClass();

	PlayerClass(string dir, string name, char c);

	~PlayerClass();

	PlayerSkillNode* GetSkills() const;

	string GetName() const;
	
	char GetChar() const;

	static map<string,PlayerClass> ClassMap;

	static void LoadClasses(string dir, string fname);

	PlayerSkillNode* LoadSkills(string dir, string fname);

	PlayerSkillNode*& LoadSkillChildren(PlayerSkillNode* root, rapidxml::xml_node<>* node);

	friend ostream& operator<<(ostream& os, const PlayerClass& pc);

	friend istream& operator>>(istream& is, PlayerClass& pc);

private:
	PlayerSkillNode* Skills;
	string Name;
	char Char;
};