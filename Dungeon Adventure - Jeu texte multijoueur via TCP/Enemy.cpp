#include "Enemy.h"

Enemy::Enemy(int health, Weapon* weapon, EnemyType enemyType, WeaponType favoriteWeapon, WeaponType weakness) : Character(health)
{
	m_weapon = weapon;
	m_type = enemyType;
	m_favoriteWeapon = favoriteWeapon;
	m_weakness = weakness;
}

Enemy::~Enemy() 
{
	if (m_weapon != nullptr)
	{
		delete m_weapon;
		m_weapon = nullptr;
	}
}

std::string Enemy::Die() const
{
	return "The enemy is dead!\n";
}

Weapon* Enemy::GetWeapon() const
{
	return m_weapon;
}

std::string Enemy::GetName() const
{
	return "Enemy";
}

std::string Enemy::TakeDamage(int damage, WeaponType weaponType)
{
	std::string response;
	if (weaponType == m_weakness)
	{
		response +=  "Your weapon deals more damage to " + GetName() + " ! The damage is doubled !\n" ;
		damage *= 2;
	}
	m_currentHealth -= damage;
	if (m_currentHealth <= 0) {
		m_currentHealth = 0;
		return response + Die();
	}
	response += "- " + std::to_string(damage) + " HP \n";
	return response;
}

std::string Enemy::UseWeapon(Weapon* weapon, Character& target)
{
	if (weapon != nullptr)
	{
		std::string weaponName = weapon->GetName();
		if (m_favoriteWeapon == weapon->GetType())
		{
			return "The enemy is using his favorite weapon, watch out ! : " + weaponName + " !\n" + weapon->Attack(target, 2);
		}
		else
		{
			return "The enemy doesn't really know how to use " + weaponName + "... \n"+	weapon->Attack(target, 0.5);
		}
	}
}


