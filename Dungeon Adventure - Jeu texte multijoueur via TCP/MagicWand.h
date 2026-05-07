#pragma once

#include "Weapon.h"

class MagicWand : public Weapon
{
public:
	MagicWand(int damage);
	~MagicWand();
	std::string Attack(Character& damageable, float multiplicateur) override;
};