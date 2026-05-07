#pragma once

#include "Weapon.h"

class Knife : public Weapon
{
public:
	Knife(int damage);
	~Knife();
	std::string Attack(Character& damageable, float multiplicateur) override;
};