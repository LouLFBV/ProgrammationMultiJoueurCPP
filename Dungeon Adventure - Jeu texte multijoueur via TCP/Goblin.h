#pragma once

#include "Enemy.h"

class Goblin : public Enemy
{
public:
	Goblin(int health, Weapon* weapon);
	~Goblin();
	std::string GetName() const override;
	std::string Die() const override;
	std::string UseWeapon(Weapon* weapon, Character& target) override;
	std::string Grunting() const override;
};