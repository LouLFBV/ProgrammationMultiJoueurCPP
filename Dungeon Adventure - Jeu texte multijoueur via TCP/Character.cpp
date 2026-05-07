#include "Character.h"

Character::Character(int health) : m_maxHealth(health), m_currentHealth(health) {}

Character::~Character() {}

bool Character::IsAlive() const
{
	return m_currentHealth > 0;
}

std::string Character::Die() const
{
	return "The character is dead !\n";
}

int Character::GetCurrentHealth() const
{
	return m_currentHealth;
}

std::string Character::TakeDamage(int damage, WeaponType weaponType)
{
	std::string response = "- " + std::to_string(damage) + " HP\n";
	m_currentHealth -= damage;
	if (m_currentHealth <= 0) {
		m_currentHealth = 0;
		return response + Die();
	}
	return response ;
}

std::string Character::UseWeapon(Weapon* weapon, Character& target)
{
	if (weapon != nullptr)
	{
		return weapon->Attack(target, 1) + " attack with " + std::to_string(weapon->GetDamage()) + " attack points !\n";
	}
	else
		return "No weapons equipped to attack!\n";
}
