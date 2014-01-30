/* Player.cpp - 
** Written by Nicholas Denaro
*/

#include "Player.h"
using namespace std;

static map<string,PlayerStats> StEPlayerStats;

Player* Player::Character;

Player::Player()
{
	Name="nullplayer";
	Stats[HEALTH]=0;
	Stats[MANA]=0;
	Stats[STRENGTH]=0;
	Stats[DEXTERITY]=0;
	Stats[INTELLECT]=0;
	Stats[LUCK]=0;
	Stats[CHARISMA]=0;
	Stats[ARMOR]=0;
}

Player::Player(string name, PlayerRace& race, PlayerClass& cl)
{
	Name=name;
	Race=race;
	Class=cl;
	Level=1;
	Qulz=STARTINGQULZ;
	Stats[HEALTH]=20;
	Stats[MANA]=10;
	Stats[STRENGTH]=5;
	Stats[DEXTERITY]=5;
	Stats[INTELLECT]=5;
	Stats[LUCK]=5;
	Stats[CHARISMA]=5;
	Stats[ARMOR]=0;
	InventoryMaxSize=15;
	Equipment=vector<Item>(4);
	Skills=map<string,PlayerSkill>();
	PlayerSkill* ps=Class.GetSkills()->GetSkill();
	Skills[ps->GetName()]=*ps;
	Experience=0;
	ExperienceNeeded=15;
	StatPoints=0;
	SkillPoints=0;
	//set stats
	CalculateEffectStats();
	CalculateBonusStats();
	Health=GetStat(HEALTH);
	Mana=GetStat(MANA);
}

string Player::IntToStrStats(int i)
{
	switch(i)
	{
		case HEALTH:
			return("#012Health");
		break;
		case MANA:
			return("#009Mana");
		break;
		case STRENGTH:
			return("#004Strength");
		break;
		case DEXTERITY:
			return("#002Dexterity");
		break;
		case INTELLECT:
			return("#011Intellect");
		break;
		case LUCK:
			return("#007Luck");
		break;
		case CHARISMA:
			return("#013Charisma");
		break;
		case ARMOR:
			return("#008Armor");
		break;
	}
}

string Player::GetName() const
{
	return(Name);
}

PlayerClass& Player::GetClass()
{
	return(Class);
}

int Player::GetX() const
{
	return(X);
}

int Player::GetY() const
{
	return(Y);
}

int Player::GetStat(PlayerStats stat)
{
	return(Stats[stat]+EquipStats[stat]+BonusStats[stat]);
}

bool Player::UpgradeStat(int stat)
{
	if(Stats[(PlayerStats)stat]<MAXSTATLEVEL)
	{
		if(stat!=HEALTH&&stat!=MANA)
			Stats[(PlayerStats)stat]+=1;
		else
			Stats[(PlayerStats)stat]+=5;
		return(true);
	}
	else
	{
		return(false);
	}
}

int Player::GetStatPoints() const
{
	return(StatPoints);
}

int Player::GetSkillPoints() const
{
	return(SkillPoints);
}

void Player::AddStatPoints(int i)
{
	StatPoints+=i;
	if(i<0)
	{
		Printer::pout<<"#011You've lost "<<-i;Printer::pout<<"#011 Stat points."<<endl;
	}
	else
	{
		Printer::pout<<"#011You've gained "<<i;Printer::pout<<"#011 Stat points."<<endl;
	}
}

void Player::AddSkillPoints(int i)
{
	SkillPoints+=i;
	if(StatPoints<0)
	{
		Printer::pout<<"#011You've lost "<<-i;Printer::pout<<"#011 skill points."<<endl;
	}
	else
	{
		Printer::pout<<"#011You've gained "<<i;Printer::pout<<"#011 skill points."<<endl;
	}
}

int Player::GetBaseStat(PlayerStats stat)
{
	return(Stats[stat]);
}

int Player::GetBonusStat(PlayerStats stat)
{
	return(EquipStats[stat]+BonusStats[stat]);
}

map<PlayerStats,int> Player::GetStats()
{
	return(Stats);
}

int Player::GetHealth() const
{
	return(Health);
}

int Player::GetMana() const
{
	return(Mana);
}

void Player::UseMana(int mana)
{
	Mana-=mana;
}

void Player::FullRestore()
{
	Health=GetStat(HEALTH);
	Mana=GetStat(MANA);
}

int Player::GetLevel() const
{
	return(Level);
}

int Player::GetQulz() const
{
	return(Qulz);
}

void Player::SetPos(int x, int y)
{
	X=x;
	Y=y;
}

void Player::Move(int deltaX, int deltaY)
{
	X+=deltaX;
	Y+=deltaY;
}

int Player::Damage(int damage)
{
	if(damage>0)
	{
		int armor=GetStat(ARMOR);
		damage-=(int)(log(armor+1.0)/log(5.0));
		if(damage<1)
			damage=1;
	}
	Health-=damage;
	if(Health>Stats[HEALTH])
		Health=Stats[HEALTH];
	if(Health<0)
		Health=0;
	return(damage);
}

bool Player::IsDead() const
{
	return(Health<=0);
}

ActionList Player::SkillsToActions()
{
	ActionList actions;
	map<string,PlayerSkill>::iterator it=Skills.begin();
	int i=0;
	while(it!=Skills.end())
	{
		if(it->second.GetType()==ACTIVE)
		{
			stringstream action;
			action<<"["<<(char)('A'+i)<<"] "<<it->second.GetName();
			actions.AddAction(action.str());
			i+=1;
		}
		it++;
	}
	return(actions);
}

PlayerSkill Player::GetSkill(string name)
{
	return(Skills[name]);
}

map<string,PlayerSkill> Player::GetSkills() const
{
	return(Skills);
}

void Player::AddSkill(PlayerSkill skill)
{
	Skills[skill.GetName()]=skill;
	Printer::pout<<"#010You've learned the skill: "<<skill.GetName();
	Printer::pout<<"#010."<<endl;
	if(skill.GetType()==PASSIVE)
		CalculateBonusStats();
}

void Player::GainExperience(int exp)
{
	Experience+=exp;
	Printer::pout<<"#011You've gained "<<exp;
	Printer::pout<<"#011 experience!"<<endl;
	while(Experience>=ExperienceNeeded)
	{
		Level+=1;
		Printer::pout<<"#014You've leveled up! You are now level "<<Level<<endl;
		ExperienceNeeded+=(int)(ExperienceNeeded*1.5);
		AddStatPoints(STATPOINTGAIN);
		if(Level%SKILLPOINTINTERVAL==0)
		{
			AddSkillPoints(1);	
		}
	}
	
	Printer::pout<<"#013You need "<<(ExperienceNeeded-Experience);
	Printer::pout<<"#013 more experience to level up."<<endl;
}

void Player::AddQulz(int qulz)
{
	Qulz+=qulz;
	if(qulz>0)
		Printer::pout<<"#007You've #010gained "<<qulz;
	else
		Printer::pout<<"#007You've #012lost "<<-qulz;
	Printer::pout<<"#007 Qulz."<<endl;
}

void Player::AssignQuest(Quest& quest)
{
	if(CurrentQuest.GetCount()==-1)
	{
		CurrentQuest=quest;
		CurrentQuest.PrintQuest();
	}
	else
	{
		if(!CurrentQuest.IsFinished())
		{
			Printer::pout<<"#014You already have an active quest!"<<endl;
			CurrentQuest.PrintQuest();
		}
		else
		{
			int qulz=CurrentQuest.GetQulzReward();
			int exp=CurrentQuest.GetExpReward();
			GainExperience(exp);
			Printer::pout<<"#007You've gained "<<qulz;
			Printer::pout<<"#007 Qulz."<<endl;
			CurrentQuest=Quest();
		}
	}
}

void Player::CheckQuest(Monster& monster)
{
	CurrentQuest.CheckMob(monster);
}

char Player::GetChar() const
{
	return(Race.GetChar());
}

bool Player::AddItem(Item i)
{
	if(Inventory.size()<(unsigned int)InventoryMaxSize)
	{
		Inventory.push_back(i);
		return(true);
	}
	return(false);
}

void Player::RemoveItem(int i)
{
	Inventory.erase(Inventory.begin()+i);
}

bool Player::EquipItem(Item i)
{
	if(i.GetType()==HELMET||i.GetType()==BODYARMOR||i.GetType()==LEGARMOR||i.GetType()==WEAPON)
	{
		Item temp=Equipment[i.GetType()];
		if(temp.GetName()!="nullitem")
			Inventory.push_back(temp);
		Equipment[i.GetType()]=i;
		Printer::pout<<"#014You equiped the "<<i.GetName()<<endl;
		CalculateEffectStats();
		return(true);
	}
	else
	{
		Printer::pout<<"#012You can't equip this item.";
		return(false);
	}
}

bool Player::UseItem(Item item)
{
	if(item.GetType()==USABLE)
	{
		vector<ItemEffect> effects=item.GetEffects();
		for(unsigned int i=0;i<effects.size();i+=1)
		{
			int val=effects[i].GetValue();
			if(effects[i].GetType()==HEALTH)
			{
				Health+=val;
				if(Health>GetStat(HEALTH))
					Health=GetStat(HEALTH);
				Printer::pout<<"#012You restored "<<val;Printer::pout<<" health.";
			}
			if(effects[i].GetType()==MANA)
			{
				Mana+=val;
				if(Mana>GetStat(MANA))
					Mana=GetStat(MANA);
				Printer::pout<<"#006You restored "<<val;Printer::pout<<" mana.";
			}
		}
		Printer::pout<<"#014You used the "<<item.GetName()<<endl;
		return(true);
	}
	else
	{
		Printer::pout<<"#012You can't equip this item.";
		return(false);
	}
}

void Player::CalculateEffectStats()
{
	EquipStats.clear();
	for(unsigned int i=0;i<Equipment.size();i+=1)
	{
		if(Equipment[i].GetName()!="nullitem")
		{
			vector<ItemEffect> ie=Equipment[i].GetEffects();
			for(unsigned int j=0;j<ie.size();j+=1)
				EquipStats[ie[j].GetType()]+=ie[j].GetValue();
		}
	}
}

void Player::CalculateBonusStats()
{
	BonusStats.clear();
	BonusStats[HEALTH]=Race.GetBonuses()[HEALTH];
	BonusStats[MANA]=Race.GetBonuses()[MANA];
	BonusStats[STRENGTH]=Race.GetBonuses()[STRENGTH];
	BonusStats[DEXTERITY]=Race.GetBonuses()[DEXTERITY];
	BonusStats[INTELLECT]=Race.GetBonuses()[INTELLECT];
	BonusStats[LUCK]=Race.GetBonuses()[LUCK];
	BonusStats[CHARISMA]=Race.GetBonuses()[CHARISMA];
	BonusStats[ARMOR]=0;
	map<string,PlayerSkill>::iterator it=Skills.begin();
	while(it!=Skills.end())
	{
		if(it->second.GetName()!="nullskill")
			BonusStats[it->second.GetPassiveEffectType()]+=(int)(it->second.GetValue());
		it++;
	}
}

const vector<Item>& Player::GetInventory() const
{
	return(Inventory);
}

void Player::PrintInventory()
{
	PrintInfo();

	cout<<endl;

	Printer::pout<<"#007StatPoints: "<<StatPoints<<endl;
	Printer::pout<<"#007SkillPoints: "<<SkillPoints<<endl;

	cout<<endl;

	stringstream val;
	Printer::pout<<"#002Stats:"<<endl;
	Printer::pout<<"  #0150. #012Health: "<<GetBaseStat(HEALTH)<<"("<<(GetBonusStat(HEALTH)>=0?"+":"")<<GetBonusStat(HEALTH)<<")(";
	val<<"#012"<<GetStat(HEALTH);
	Printer::pout<<val.str()<<")"<<endl;
	//Mana
	Printer::pout<<"  #0151. #009Mana#015: "<<GetBaseStat(MANA)<<"("<<(GetBonusStat(MANA)>=0?"+":"")<<GetBonusStat(MANA)<<")(";//<<GetStat(MANA)<<")"<<endl;
	val.str("");//clears the string stream
	val<<"#009"<<GetStat(MANA);
	Printer::pout<<val.str()<<")"<<endl;
	//Strength
	Printer::pout<<"  #0152. #004Strength#015: "<<GetBaseStat(STRENGTH)<<"("<<(GetBonusStat(STRENGTH)>=0?"+":"")<<GetBonusStat(STRENGTH)<<")(";//<<GetStat(STRENGTH)<<")"<<endl;
	val.str("");
	val<<"#004"<<GetStat(STRENGTH);
	Printer::pout<<val.str()<<")"<<endl;
	//Dexterity
	Printer::pout<<"  #0153. #002Dexterity#015: "<<GetBaseStat(DEXTERITY)<<"("<<(GetBonusStat(DEXTERITY)>=0?"+":"")<<GetBonusStat(DEXTERITY)<<")(";//<<GetStat(DEXTERITY)<<")"<<endl;
	val.str("");
	val<<"#002"<<GetStat(DEXTERITY);
	Printer::pout<<val.str()<<")"<<endl;
	//Intellect
	Printer::pout<<"  #0154. #011Intellect#015: "<<GetBaseStat(INTELLECT)<<"("<<(GetBonusStat(INTELLECT)>=0?"+":"")<<GetBonusStat(INTELLECT)<<")(";//<<GetStat(INTELLECT)<<")"<<endl;
	val.str("");
	val<<"#011"<<GetStat(INTELLECT);
	Printer::pout<<val.str()<<")"<<endl;
	//Luck
	Printer::pout<<"  #0155. #007Luck#015: "<<GetBaseStat(LUCK)<<"("<<(GetBonusStat(LUCK)>=0?"+":"")<<GetBonusStat(LUCK)<<")(";//<<GetStat(LUCK)<<")";<<endl;
	val.str("");
	val<<"#007"<<GetStat(LUCK);
	Printer::pout<<val.str()<<")"<<endl;
	//Charisma
	Printer::pout<<"  #0156. #013Charisma#015: "<<GetBaseStat(CHARISMA)<<"("<<(GetBonusStat(CHARISMA)>=0?"+":"")<<GetBonusStat(CHARISMA)<<")(";//<<GetStat(CHARISMA)<<")"<<endl;
	val.str("");
	val<<"#013"<<GetStat(CHARISMA);
	Printer::pout<<val.str()<<")"<<endl;

	//Amor
	Printer::pout<<"  #008Armor#015: "<<GetBaseStat(ARMOR)<<"("<<(GetBonusStat(ARMOR)>=0?"+":"")<<GetBonusStat(ARMOR)<<")(";//<<GetStat(ARMOR)<<")"<<endl;
	val.str("");
	val<<"#008"<<GetStat(ARMOR);
	Printer::pout<<val.str()<<")"<<endl;

	cout<<endl;

	Printer::pout<<"#003Skills: "<<endl;
	map<string,PlayerSkill>::iterator it=Skills.begin();
	while(it!=Skills.end())
	{
		if(it->second.GetName()!="nullskill")
		{
			if(it->second.GetType()==ACTIVE)
			{
				cout<<it->second.GetName()<<": Damage mult.: "<<it->second.GetValue()<<"x";
				Printer::pout<<Player::IntToStrStats(it->second.GetActiveEffectType()+2)<<" Mana cost: "<<it->second.GetManaCost()<<endl;
			}
			else
			{
				cout<<it->second.GetName()<<": Upgrade: "<<it->second.GetValue()<<" ";
				Printer::pout<<Player::IntToStrStats(it->second.GetPassiveEffectType())<<endl;
			}
		}
		it++;
	}

	cout<<endl;

	Printer::pout<<"#008Equipment: "<<endl;
	cout<<"Helmet: "<<Equipment[HELMET].GetName()<<endl;
	cout<<"Body Armor: "<<Equipment[BODYARMOR].GetName()<<endl;
	cout<<"Leg Armor: "<<Equipment[LEGARMOR].GetName()<<endl;
	cout<<"Weapon: "<<Equipment[WEAPON].GetName()<<endl;

	cout<<endl;

	Printer::pout<<"#007Inventory: "<<endl;
	for(unsigned int i=0;i<Inventory.size();i+=1)
	{
		cout<<i<<". "<<Inventory[i].GetName()<<endl;
	}
	cout<<endl;
}


void Player::PrintInfo()
{
	cout<<Name<<": ";
	Printer::pout<<"#014Level: "<<Level<<" ";
	Printer::pout<<"#012Health: "<<Health<<"/"<<GetStat(HEALTH)<<" ";
	Printer::pout<<"#009Mana: "<<Mana<<"/"<<GetStat(MANA)<<" ";
	Printer::pout<<"#011Exp: "<<Experience<<"/"<<ExperienceNeeded<<" ";
	Printer::pout<<"#007Qulz: "<<Qulz<<" ";
	Printer::pout<<"#006Inv: "<<Inventory.size()<<"/"<<InventoryMaxSize<<" ";
	cout<<endl;

}

ostream& operator<<(ostream& os, Player& player)
{
	os<<player.Name<<'\n';
	os<<player.Level<<'\n';
	os<<player.X<<'\n';
	os<<player.Y<<'\n';
	os<<player.Health<<'\n';
	os<<player.Mana<<'\n';
	os<<player.Qulz<<'\n';
	os<<player.StatPoints<<'\n';
	os<<player.SkillPoints<<'\n';
	os<<player.Race<<'\n';
	os<<player.Class<<'\n';
	//stats
	os<<ARMOR<<'\n';
	for(int i=0;i<ARMOR;i+=1)
	{
		os<<player.Stats[(PlayerStats)i]<<'\n';
	}
	//equipment
	os<<player.Equipment.size()<<'\n';
	for(unsigned int i=0;i<player.Equipment.size();i+=1)
	{
		os<<player.Equipment[i]<<'\n';
	}
	//inventory
	os<<player.Inventory.size()<<'\n';
	for(unsigned int i=0;i<player.Inventory.size();i+=1)
	{
		os<<player.Inventory[i]<<'\n';
	}
	//skills
	map<string,PlayerSkill>::iterator it2=player.Skills.begin();
	int count=0;
	while(it2!=player.Skills.end())
	{
		if(it2->second.GetName()!="nullskill")
		{
			count+=1;
		}
		it2++;
	}
	os<<count<<'\n';
	it2=player.Skills.begin();
	while(it2!=player.Skills.end())
	{
		if(it2->second.GetName()!="nullskill")
		{
			os<<it2->second<<'\n';
		}
		it2++;
	}
	//back to more variables
	os<<player.Experience<<'\n';
	os<<player.ExperienceNeeded<<'\n';
	os<<player.InventoryMaxSize<<'\n';
	os<<player.CurrentQuest;
	return(os);
}

istream& operator>>(istream& is, Player& player)
{
	string tempname;
	is>>player.Name;
	getline(is,tempname);
	player.Name+=tempname;
	is>>player.Level>>player.X>>player.Y>>player.Health;
	is>>player.Mana>>player.Qulz>>player.StatPoints>>player.SkillPoints;
	PlayerRace race;
	is>>race;
	player.Race=PlayerRace::RaceMap[race.GetName()];
	PlayerClass cl;
	is>>cl;
	player.Class=PlayerClass::ClassMap[cl.GetName()];

	int sizestats;
	int sizeequips;
	int sizeinvtentory;
	int sizeskills;
	//stats
	is>>sizestats;
	for(int i=0;i<sizestats;i+=1)
	{
		int val;
		is>>val;
		player.Stats[(PlayerStats)i]=val;
	}
	
	//Equipment
	is>>sizeequips;
	for(int i=0;i<sizeequips;i+=1)
	{
		Item item;
		is>>item;
		player.Equipment.push_back(item);
	}

	//inventory
	is>>sizeinvtentory;
	for(int i=0;i<sizeinvtentory;i+=1)
	{
		Item item;
		is>>item;
		player.Inventory.push_back(item);
	}

	//skills
	is>>sizeskills;
	for(int i=0;i<sizeskills;i+=1)
	{
		PlayerSkill skill;
		is>>skill;
		skill=*player.Class.GetSkills()->FindSkill(skill.GetName());
		player.Skills[skill.GetName()]=skill;
		//player.AddSkill(skill);
	}

	//back to more variables
	is>>player.Experience>>player.ExperienceNeeded>>player.InventoryMaxSize>>player.CurrentQuest;

	player.CalculateBonusStats();
	player.CalculateEffectStats();

	return(is);
}