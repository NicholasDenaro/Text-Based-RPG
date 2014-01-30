/* ActionList.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <map>
#include <vector>
#include <string>
using namespace std;

class ActionList
{
public:
	ActionList();

	ActionList(string actions);

	ActionList(vector<string> actions);

	bool operator==(ActionList& other);

	bool operator!=(ActionList& other);

	void Clear();

	void AddAction(string action);

	string operator[](char index);// const;

	bool IsValidAction(char action);// const;

	void PrintActions();// const;

	int Size() const;

private:
	map<char,string> Actions;
};