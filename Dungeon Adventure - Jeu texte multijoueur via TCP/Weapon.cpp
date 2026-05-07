#include "Weapon.h"


Weapon::Weapon(int damage, WeaponType type, ItemType itemType) : m_damage(damage), m_type(type), Item(itemType) {}

Weapon::~Weapon(){}

int Weapon::GetDamage() const
{
	return m_damage;
}

WeaponType Weapon::GetType() const
{
	return m_type;
}

std::string Weapon::GetName() const
{
	std::string damageStr = std::to_string(m_damage);
	switch (m_type)
	{
	case WeaponType::SwordType:
		return "a sword (Attack points :" + damageStr + ")";
		break;
	case WeaponType::KnifeType:
		return "a knife (Attack points :" + damageStr + ")";
		break;
	case WeaponType::MagicWandType:
		return "a magic wand (Attack points :" + damageStr + ")";
		break;
	case WeaponType::SlingshotType:
		return "a slingshot (Attack points :" + damageStr + ")";
		break;
	case WeaponType::BombType:
		return "bombs (Attack points :" + damageStr + ")";
		break;
	default:
		break;
	}
	return "a unknow weapon";
}