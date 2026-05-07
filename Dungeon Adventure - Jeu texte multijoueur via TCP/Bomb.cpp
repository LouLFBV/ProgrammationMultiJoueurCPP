#include "Bomb.h"

Bomb::Bomb(int damage) : Weapon(damage, WeaponType::BombType, ItemType::WeaponItemType) {}

Bomb::~Bomb(){}

std::string Bomb::Attack(Character& damageable, float multiplicateur)
{
	return "BOOM ! \n" + damageable.TakeDamage(m_damage * multiplicateur + (rand() % 4), WeaponType::BombType);
}
