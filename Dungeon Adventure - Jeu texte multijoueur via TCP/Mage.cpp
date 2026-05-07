#include "Mage.h"

Mage::Mage(int health, Weapon* weapon) : Enemy(health, weapon, EnemyType::MageType, WeaponType::MagicWandType, WeaponType::SwordType) {}

Mage::~Mage() {}

std::string Mage::GetName() const
{
	return "Mage";
}

std::string Mage::Die() const
{
	return "Mage is dead !\n";
}

std::string Mage::UseWeapon(Weapon* weapon, Character& target)
{
	return Grunting() +	Enemy::UseWeapon(weapon, target);
}

std::string Mage::Grunting() const
{
	return  "Mage attack : 'Incantation...'\n" ;
}