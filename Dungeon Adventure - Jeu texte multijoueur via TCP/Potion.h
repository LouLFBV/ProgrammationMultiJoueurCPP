#pragma once

#include <string>

#include "Item.h"
#include "Player.h"

class Potion : public Item
{
private:
	std::string m_name;
	int m_heal;

public:
	Potion(std::string name, int heal);
	~Potion();
	std::string HealPlayer(Player& player) const;
	std::string GetName() const override;
};