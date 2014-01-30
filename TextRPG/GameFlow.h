/* GameFlow.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Printer.h"
#include "WorldMap.h"
#include "ActionList.h"
#include "Battle.h"
using namespace std;


class GameFlow
{
public:

	enum Task{MAINMENU,WORLDMAP,TILECONTAINER,PAUSEMENU};

	static GameFlow Flow;

	GameFlow();

	~GameFlow();

	void Run();

	void SetRunning(bool run);

	char GetUserInput();

	char GetUserInput(ActionList actions);

	void ProcessInput(char action);

	void UpdateActions();

	void GetAvailableActions();// const;

	void Draw();

	void Shop();

	Player* CreateCharacter();
private:
	bool Running;
	WorldMap* Map;
	Task Process;
	ActionList CurrentActions;
	static ActionList MainMenuActions;
};