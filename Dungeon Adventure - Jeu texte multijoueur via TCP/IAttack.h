#pragma once

#include <string>
class Character;

class IAttack
{
public:
	virtual ~IAttack() = default;
	virtual std::string Attack(Character& damageable, float multiplicateur) = 0;
};
