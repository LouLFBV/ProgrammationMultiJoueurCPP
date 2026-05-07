#pragma once


#include "Weapon.h"
#include "WeaponType.h"

#include <string>

class Weapon;

class Character 
{
protected:
	int m_maxHealth;
	int m_currentHealth;

public:
	Character(int health);
	virtual ~Character();
	bool IsAlive() const;
	virtual std::string GetName() const = 0;
	virtual std::string Die() const;
	int GetCurrentHealth() const;
	virtual std::string TakeDamage(int damage, WeaponType weaponType);
	virtual std::string UseWeapon(Weapon* weapon, Character& target);
};
