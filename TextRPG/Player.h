/* Player.h
** Written by Nicholas Denaro
*/

#pragma once

#include <vector>
#include <map>
enum PlayerStats{HEALTH=0,MANA,STRENGTH,DEXTERITY,INTELLECT,LUCK,CHARISMA,ARMOR};
class PlayerClass;
#include "PlayerClass.h"
#include "ActionList.h"
class Quest;
#include "Quest.h"
#include "PlayerRace.h"
#include "Item.h"
class Item;
using namespace std;

#define MAXSTATLEVEL 100
#define STATPOINTGAIN 2
#define SKILLPOINTINTERVAL 5
#define STARTINGQULZ 60

class Player
{
public:

	static Player* Character;

	Player();

	Player(string name, PlayerRace& race, PlayerClass& cl);

	static string IntToStrStats(int i);

	string GetName() const;

	PlayerClass& GetClass();

	int GetX() const;

	int GetY() const;

	int GetStat(PlayerStats stat);

	map<string,PlayerSkill> GetSkills() const;

	void AddSkill(PlayerSkill skill);

	bool UpgradeStat(int stat);

	int GetStatPoints() const;

	int GetSkillPoints() const;

	void AddStatPoints(int i);

	void AddSkillPoints(int i);

	int GetBaseStat(PlayerStats stat);

	int GetBonusStat(PlayerStats stat);

	map<PlayerStats,int> GetStats();

	int GetHealth() const;

	int GetMana() const;

	void UseMana(int mana);

	void FullRestore();

	int GetLevel() const;

	int GetQulz() const;

	void SetPos(int x, int y);

	void Move(int deltaX, int deltaY);

	int Damage(int damage);

	bool IsDead() const;

	ActionList SkillsToActions();

	PlayerSkill GetSkill(string name);

	void GainExperience(int exp);

	void AddQulz(int qulz);

	void AssignQuest(Quest& quest);

	void CheckQuest(Monster& monster);

	char GetChar() const;

	bool AddItem(Item i);

	void RemoveItem(int i);

	bool EquipItem(Item i);

	bool UseItem(Item i);

	void CalculateEffectStats();

	void CalculateBonusStats();

	const vector<Item>& GetInventory() const;

	void PrintInventory();

	void PrintInfo();

	friend ostream& operator<<(ostream& os,Player& player);

	friend istream& operator>>(istream& is,Player& player);

private:
	string Name;
	int Level;
	int X, Y;
	int Health;
	int Mana;
	int Qulz;//money<3
	int StatPoints;
	int SkillPoints;
	PlayerRace Race;
	PlayerClass Class;
	map<PlayerStats,int> Stats;
	map<PlayerStats,int> EquipStats;
	map<PlayerStats,int> BonusStats;
	vector<Item> Equipment;
	vector<Item> Inventory;
	map<string,PlayerSkill> Skills;
	int Experience;
	int ExperienceNeeded;
	int InventoryMaxSize;
	Quest CurrentQuest;
};