#pragma once

#include "Weapon.h"

#include <iostream>

class Bomb : public Weapon
{
public:
	Bomb(int damage);
	~Bomb();
	std::string Attack(Character& damageable, float multiplicateur) override;
};
