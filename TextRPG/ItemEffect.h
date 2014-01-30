/* ItemEffect.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include "Player.h"
using namespace std;

class ItemEffect
{
public:
	ItemEffect();
	
	ItemEffect(PlayerStats type, int value);

	PlayerStats GetType() const;

	int GetValue() const;

	friend ostream& operator<<(ostream& os, const ItemEffect& effect);

	friend istream& operator>>(istream& is, ItemEffect& effect);

private:
	PlayerStats Type;
	int Value;
};
