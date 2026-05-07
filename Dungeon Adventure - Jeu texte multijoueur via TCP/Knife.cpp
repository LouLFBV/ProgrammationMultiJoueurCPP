#include "Knife.h"

Knife::Knife(int damage) : Weapon(damage, WeaponType::KnifeType, ItemType::WeaponItemType) {}

Knife::~Knife(){}

std::string Knife::Attack(Character& damageable, float multiplicateur)
{
	return "TIC TIC ! \n" +	damageable.TakeDamage(m_damage * multiplicateur + (rand() % 4), WeaponType::KnifeType);
}