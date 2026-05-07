#pragma once

#include "Weapon.h"

class Sword : public Weapon
{
public:
	Sword(int damage);
	~Sword();
	std::string Attack(Character& damageable, float multiplicateur) override;
};