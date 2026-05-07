#pragma once

#include "Enemy.h"

class Orc : public Enemy
{
public:
	Orc(int health, Weapon* weapon);
	~Orc();
	std::string GetName() const override;
	std::string Die() const override;
	std::string UseWeapon(Weapon* weapon, Character& target) override;
	std::string Grunting() const override;
};