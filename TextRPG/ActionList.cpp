/* ActionList.cpp - 
** Written by Nicholas Denaro
*/

#pragma once
#include <iostream>
#include "ActionList.h"
using namespace std;

ActionList::ActionList(){}

ActionList::ActionList(string actions)
{
	int commaIndex=actions.find(',');
	while(commaIndex!=string::npos)
	{
		string action=actions.substr(0,commaIndex);
		Actions[action[1]]=action;
		actions=actions.substr(commaIndex+1);
		commaIndex=actions.find(',');
	}
	Actions[actions[1]]=actions;
}

ActionList::ActionList(vector<string> actions)
{
	for(int i=0;i<actions.size();i+=1)
	{
		Actions[actions[i][1]]=actions[i];
	}
}

bool ActionList::operator==(ActionList& other)
{
	if(Actions.size()!=other.Actions.size())
		return(false);
	else
	{
		map<char,string>::iterator it1=Actions.begin();
		map<char,string>::iterator it2=other.Actions.begin();
		while(it1!=Actions.end())
		{
			if(it1->second!=it2->second)
				return(false);
			it1++;
			it2++;
		}
	}
	return(true);
}

bool ActionList::operator!=(ActionList& other)
{
	return(!(*this==other));
}

void ActionList::Clear()
{
	Actions.clear();
}

void ActionList::AddAction(string action)
{
	Actions[action[1]]=action;
}

string ActionList::operator[](char index)// const
{
	return(Actions[index].substr(Actions[index].find(']')+2));
}

bool ActionList::IsValidAction(char action)// const
{
	return(Actions.find(action)!=Actions.end());
}

void ActionList::PrintActions()// const
{
	map<char,string>::iterator it=Actions.begin();
	while(it!=Actions.end())
		cout<<it++->second<<endl;
}

int ActionList::Size() const
{
	return(Actions.size());
}