#include "Orc.h"

Orc::Orc(int health, Weapon* weapon) : Enemy(health, weapon, EnemyType::OrcType, WeaponType::BombType, WeaponType::MagicWandType) {}

Orc::~Orc() {}

std::string Orc::GetName() const
{
	return "Orc";
}

std::string Orc::Die() const
{
	return "Orc is dead !\n";
}

std::string Orc::UseWeapon(Weapon* weapon, Character& target)
{
	return Grunting() +	Enemy::UseWeapon(weapon, target);
}

std::string Orc::Grunting() const
{
	return "Orc attack : 'Grrroaww!'\n" ;
}
