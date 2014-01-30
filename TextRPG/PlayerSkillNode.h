/* PlayerSkillNode.h - PlayerSkillNode prototype
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
#include <vector>
#include "PlayerSkill.h"
using namespace std;

#define NOTFOUND -1

class PlayerSkillNode
{
public:
	PlayerSkillNode()
	{
		Skill=NULL;
	}

	PlayerSkillNode(PlayerSkill* skill)
	{
		Skill=skill;
		Children=vector<PlayerSkillNode*>();
	}

	~PlayerSkillNode()
	{
		delete Skill;
		Skill=NULL;
		for(unsigned int i=0;i<Children.size();i+=1)
		{
			if(Children[i]!=NULL)
				delete Children[i];
		}
	}

	void AddChild(PlayerSkillNode* node)
	{
		Children.push_back(node);
	}

	PlayerSkill* GetSkill() const
	{
		return(Skill);
	}

	string GetSkillName()
	{
		return(Skill->GetName());
	}

	vector<PlayerSkillNode*>& GetChildren()
	{
		return(Children);
	}

	PlayerSkill* FindSkill(string name)
	{
		if(Skill==NULL)
			return(NULL);
		else
		{
			if(Skill->GetName()==name)
				return(Skill);
			else
			{
				for(unsigned int i=0;i<Children.size();i+=1)
				{
					PlayerSkill* ps=Children[i]->FindSkill(name);
					if(ps!=NULL)
						return(ps);
				}
			}
		}
		return(NULL);
	}

	vector<PlayerSkillNode*> FindSkillChildren(string name)
	{
		if(Skill==NULL)
			return(vector<PlayerSkillNode*>());
		else
		{
			if(Skill->GetName()==name)
				return(Children);
			else
			{
				for(unsigned int i=0;i<Children.size();i+=1)
				{
					vector<PlayerSkillNode*> ps=Children[i]->FindSkillChildren(name);
					if(!ps.empty())
						return(ps);
				}
			}
		}
		return(vector<PlayerSkillNode*>());
	}

private:
	PlayerSkill* Skill;
	vector<PlayerSkillNode*> Children;//Vector of pointers to children nodes;
};