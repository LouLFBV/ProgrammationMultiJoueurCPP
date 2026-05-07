#include "Sword.h"

Sword::Sword(int damage) : Weapon(damage, WeaponType::SwordType, ItemType::WeaponItemType) {}

Sword::~Sword(){}

std::string Sword::Attack(Character& damageable, float multiplicateur)
{
	return "SLASH ! \n" + damageable.TakeDamage(m_damage * multiplicateur + (rand() % 4), WeaponType::SwordType);
}