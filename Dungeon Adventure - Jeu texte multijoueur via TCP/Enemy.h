#pragma once

#include "Character.h"
#include "Weapon.h"
#include "IAttack.h"
#include "EnemyType.h"

#include <iostream>

// Pour rendre l'expérience plus amusante, les ennemis ont une arme avec laquelle ils infligent plus de dégâts mais ils ont aussi un point faible qui fait subir plus de dégât à l'ennemie en fonction de l'arme utilisée par le joueur.
class Enemy : public Character
{
protected:
	Weapon* m_weapon;
	EnemyType m_type;
	WeaponType m_favoriteWeapon;
	WeaponType m_weakness;

public:
	Enemy(int health, Weapon* weapon, EnemyType enemyType, WeaponType favoriteWeapon, WeaponType weakness);
	virtual ~Enemy();
	virtual std::string Die() const override;
	Weapon* GetWeapon() const;
	virtual std::string GetName() const;
	virtual std::string UseWeapon(Weapon* weapon, Character& target) override;
	virtual std::string TakeDamage(int damage, WeaponType weaponType) override;
	virtual std::string Grunting() const = 0;
};
