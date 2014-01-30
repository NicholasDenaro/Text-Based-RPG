/* PlayerClass.cpp - 
** Written by Nicholas Denaro
*/
#include "Player.h"
#include "PlayerClass.h"
using namespace std;

PlayerClass::PlayerClass()
{
	Skills=NULL;
}

PlayerClass::PlayerClass(string dir, string name, char c)
{
	Name=name;
	//Skills=NULL;
	Skills=LoadSkills(dir,Name+"Skills.xml");
	Char=c;
}

PlayerClass::~PlayerClass()
{
	//breaks the skills?
	/*if(Skills!=NULL)
	{
		delete Skills;
		Skills=NULL;
	}*/
}

PlayerSkillNode* PlayerClass::GetSkills() const
{
	return(Skills);
}

string PlayerClass::GetName() const
{
	return(Name);
}
	
char PlayerClass::GetChar() const
{
	return(Char);
}

map<string,PlayerClass> PlayerClass::ClassMap;

void PlayerClass::LoadClasses(string dir, string fname)
{
	string path=dir+fname;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;// character type defaults to char
	doc.parse<0>(xmlFile.data());// 0 means default parse flags
	rapidxml::xml_node<>* node=doc.first_node("Classes");
	while(node->first_node()!=0)
	{
		string className=node->first_node()->name();
		ClassMap[className]=PlayerClass(dir,className,node->first_node()->first_attribute()->value()[0]);
		node->remove_first_node();
	}
}

PlayerSkillNode* PlayerClass::LoadSkills(string dir, string fname)
{
	string path=dir+fname;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;// character type defaults to char
	doc.parse<0>(xmlFile.data());// 0 means default parse flags
	rapidxml::xml_node<>* node=doc.first_node("Skill");
	PlayerSkill* skill=new PlayerSkill(node->first_attribute("Name")->value(),node->first_attribute("Type")->value(),node->first_attribute("Effect")->value(),node->first_attribute("Value")->value(),node->first_attribute("ManaCost")->value());
	PlayerSkillNode* root=new PlayerSkillNode(skill);
	rapidxml::xml_node<>* children=node->first_node("Children");
	while(children->first_node()!=0)
	{
		rapidxml::xml_node<>* newskill=children->first_node("Skill");
		root->AddChild(LoadSkillChildren(root,newskill));
		children->remove_first_node();
	}
	return(root);
}

PlayerSkillNode*& PlayerClass::LoadSkillChildren(PlayerSkillNode* root, rapidxml::xml_node<>* node)
{
	if(node->first_node("Children")==0)
	{
		PlayerSkill* skill=new PlayerSkill(node->first_attribute("Name")->value(),node->first_attribute("Type")->value(),node->first_attribute("Effect")->value(),node->first_attribute("Value")->value(),node->first_attribute("ManaCost")->value());
		PlayerSkillNode* skillnode=new PlayerSkillNode(skill);
		return(skillnode);
	}
	else
	{
		PlayerSkill* skill=new PlayerSkill(node->first_attribute("Name")->value(),node->first_attribute("Type")->value(),node->first_attribute("Effect")->value(),node->first_attribute("Value")->value(),node->first_attribute("ManaCost")->value());
		PlayerSkillNode* newroot=new PlayerSkillNode(skill);
		rapidxml::xml_node<>* children=node->first_node("Children");
		while(children->first_node()!=0)
		{
			rapidxml::xml_node<>* newskill=children->first_node("Skill");
			newroot->AddChild(LoadSkillChildren(newroot,newskill));
			children->remove_first_node();
		}
		return(newroot);
	}
}

/*void PlayerClass::PrintSkillTree(PlayerSkillNode*& node, int t)
{
	for(int i=0;i<t;i+=1)
		cout<<"--";
	cout<<node->GetSkillName()<<endl;
	vector<PlayerSkillNode*> children=node->GetChildren();
	for(unsigned int i=0;i<children.size();i+=1)
		PrintSkillTree(children[i],t+1);
}*/

ostream& operator<<(ostream& os, const PlayerClass& pc)
{
	os<<pc.Name;
	return(os);
}

istream& operator>>(istream& is, PlayerClass& pc)
{
	is>>pc.Name;
	return(is);
}