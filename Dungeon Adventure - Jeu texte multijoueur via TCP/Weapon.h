#pragma once

#include "IAttack.h"
#include "WeaponType.h"
#include "Item.h"
#include "Character.h"

class Character;

#include <string>


class Weapon : public IAttack , public Item
{
protected:
	int m_damage;
	WeaponType m_type;

public:
	Weapon(int damage, WeaponType type, ItemType itemType);
	virtual ~Weapon();
	int GetDamage() const;
	std::string GetName() const;
	WeaponType GetType() const;
	std::string Attack(Character& damageable, float multiplicateur) override = 0;
};
