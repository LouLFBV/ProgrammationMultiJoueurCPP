#pragma once

#include "Weapon.h"

class Slingshot : public Weapon
{
public:
	Slingshot(int damage);
	~Slingshot();
	std::string Attack(Character& damageable, float multiplicateur) override;
};