/* Quest.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <string>
class Player;
//#include "Player.h"
#include "Monster.h"
using namespace std;

class Quest
{
public:
	Quest();

	Quest(int targetCount, string target, int level, double bonus);

	int GetCount() const;

	void CheckMob(Monster& monster);

	bool IsFinished() const;

	string GetTarget() const;

	int GetQulzReward() const;

	int GetExpReward() const;

	static Quest RandomQuest(Player* player);

	void PrintQuest();

	friend ostream& operator<<(ostream& os, const Quest& quest);

	friend istream& operator>>(istream& is, Quest& quest);

private:
	int Count;
	int TargetCount;
	string Target;
	int TargetLevel;
	int QulzReward;
	int ExpReward;
};