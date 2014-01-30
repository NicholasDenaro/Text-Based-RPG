/* Monster.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include "MonsterSkill.h"
#include "rapidxml-1.13\rapidxml.hpp"
#include "rapidxml-1.13\rapidxml_utils.hpp"
using namespace std;

class Monster
{
public:
	Monster();

	Monster(string name, int baseHealth);

	Monster(Monster& monster,int level);

	string GetName() const;

	int GetHealth() const;

	int GetMaxHealth() const;

	int GetLevel() const;

	static map<string,Monster> MonsterMap;
	static map<int,vector<string>> MonsterLevelMap;

	static map<string,Monster> BossMap;
	static map<int,vector<string>> BossLevelMap;

	static void LoadMonsters(string dir, string fname);

	static void LoadBosses(string dir, string fname);

	static map<string,Monster> GetLevelRange(int low, int high);

	static map<string,Monster> GetBossLevelRange(int low, int high);

	static Monster& GetRandomMonster(int level);

	void AddSkill(MonsterSkill skill);

	bool IsDead() const;

	void Damage(int damage);

	MonsterSkill RandomSkill() const;

	friend ostream& operator<<(ostream& os, Monster& monster);

	friend istream& operator>>(istream& is, Monster& monster);

private:
	string Name;
	int BaseHealth;
	int Level;
	int MaxHealth, Health;
	vector<MonsterSkill> Skills;
};