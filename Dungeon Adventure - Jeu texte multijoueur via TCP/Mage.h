#pragma once

#include "Enemy.h"

class Mage : public Enemy
{
public:
	Mage(int health, Weapon* weapon);
	~Mage();
	std::string GetName() const override;
	std::string Die() const override;
	std::string UseWeapon(Weapon* weapon, Character& target) override;
	std::string Grunting() const override;
};