#include "Goblin.h"

Goblin::Goblin(int health, Weapon* weapon) : Enemy(health, weapon, EnemyType::GoblinType, WeaponType::KnifeType, WeaponType::BombType) {}

Goblin::~Goblin() {}

std::string Goblin::GetName() const
{
	return "Goblin";
}

std::string Goblin::Die() const
{
	return "Goblin is dead !\n";
}

std::string Goblin::UseWeapon(Weapon* weapon, Character& target)
{
	return Grunting() +	Enemy::UseWeapon(weapon, target);
}

std::string Goblin::Grunting() const
{
	return "Goblin attack : 'Grrr!'\n" ;
}