/* ItemEffect.cpp -
** Written by Nicholas Denaro
*/

#include "ItemEffect.h"
using namespace std;

ItemEffect::ItemEffect(){};
	
ItemEffect::ItemEffect(PlayerStats type, int value)
{
	Type=type;
	Value=value;
}

PlayerStats ItemEffect::GetType() const
{
	return(Type);
}

int ItemEffect::GetValue() const
{
	return(Value);
}

ostream& operator<<(ostream& os, const ItemEffect& effect)
{
	os<<effect.Type<<'\n';
	os<<effect.Value;
	return(os);
}

istream& operator>>(istream& is, ItemEffect& effect)
{
	int type;
	is>>type;
	effect.Type=(PlayerStats)type;
	is>>effect.Value;
	return(is);
}