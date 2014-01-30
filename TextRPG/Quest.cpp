/* Quest.cpp - 
** Written by Nicholas Denaro
*/

#include "Quest.h"
#include "Player.h"
#include "WorldMap.h"
#include "Town.h"
using namespace std;

Quest::Quest()
{
	Count=-1;//Default value for NULL quest
	Target="nulltarget";
}

Quest::Quest(int targetCount, string target, int level, double bonus)
{
	Count=0;
	TargetCount=targetCount;
	Target=target;
	TargetLevel=level;
	QulzReward=(int)(TargetLevel*25*bonus);
	ExpReward=(int)(TargetLevel*30*bonus);
}

int Quest::GetCount() const
{
	return(Count);
}

void Quest::CheckMob(Monster& monster)
{
	if((!IsFinished())&&(monster.GetName()==Target)&&(monster.GetLevel()>=TargetLevel))
	{
		Count+=1;
		if(IsFinished())
		{
			Printer::pout<<"#014You've completed your quest, go to the Guild HQ to turn it in!"<<endl;
		}
		else
		{
			PrintQuest();
		}
	}
}

bool Quest::IsFinished() const
{
	return(Count>=TargetCount);
}

string Quest::GetTarget() const
{
	return(Target);
}

int Quest::GetQulzReward() const
{
	return(QulzReward);
}

int Quest::GetExpReward() const
{
	return(ExpReward);
}


Quest Quest::RandomQuest(Player* player)
{
	int count=(int)(5+log(player->GetLevel()*1.0)/log(3.0));
	string target;
	int level=player->GetLevel();
	map<string,Monster> monsters=Monster::GetLevelRange(level,level);
	int r=rand()%monsters.size();
	map<string,Monster>::iterator it=monsters.begin();
	double bonus=1;
	if(WorldMap::Map->GetCurrentTile()->GetContainer()->GetType()==TOWN)
	{
		Town* t=(Town*)WorldMap::Map->GetCurrentTile()->GetContainer();
		if(t->GetSpecialty()=="Quests")
			bonus+=rand()*1.0/RAND_MAX;
	}
	for(int i=0;i<r;i+=1)
		it++;
	return(Quest(count,it->first,level,bonus));
}

void Quest::PrintQuest()
{
	Printer::pout<<"#011Quest Info: #003Kill "<<Count<<"/"<<TargetCount;
	Printer::pout<<"#003 Lvl. "<<TargetLevel<<"+ "<<Target;
	Printer::pout<<"#003s."<<endl;
}

ostream& operator<<(ostream& os, const Quest& quest)
{
	os<<quest.Count<<'\n';
	os<<quest.TargetCount<<'\n';
	os<<quest.Target<<'\n';
	os<<quest.QulzReward<<'\n';
	os<<quest.ExpReward;
	return(os);
}

istream& operator>>(istream& is, Quest& quest)
{
	is>>quest.Count>>quest.TargetCount>>quest.Target>>quest.QulzReward>>quest.ExpReward;
	return(is);
}