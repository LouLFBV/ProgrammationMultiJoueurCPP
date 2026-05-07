#include "Slingshot.h"

Slingshot::Slingshot(int damage) : Weapon(damage, WeaponType::SlingshotType, ItemType::WeaponItemType) {}

Slingshot::~Slingshot(){}

std::string Slingshot::Attack(Character& damageable, float multiplicateur)
{
	return "PIN PIN ! \n" +	damageable.TakeDamage(m_damage * multiplicateur + (rand() % 4), WeaponType::SlingshotType);
}