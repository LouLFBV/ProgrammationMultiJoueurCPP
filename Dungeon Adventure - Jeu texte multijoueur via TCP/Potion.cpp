#include "Potion.h"

Potion::Potion(std::string name, int heal) : m_name(name), m_heal(heal), Item(ItemType::PotionItemType) {}

Potion::~Potion(){}

std::string Potion::HealPlayer(Player& player) const
{
	return player.Heal(m_heal);
}

std::string Potion::GetName() const
{
	return m_name;
}
