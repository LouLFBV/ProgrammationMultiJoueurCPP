#include "MagicWand.h"

MagicWand::MagicWand(int damage) : Weapon(damage, WeaponType::MagicWandType, ItemType::WeaponItemType) {}

MagicWand::~MagicWand(){}

std::string MagicWand::Attack(Character& damageable, float multiplicateur)
{
	return "**Magic effect** !  \n" + damageable.TakeDamage(m_damage * multiplicateur + (rand() % 4), WeaponType::MagicWandType);
}