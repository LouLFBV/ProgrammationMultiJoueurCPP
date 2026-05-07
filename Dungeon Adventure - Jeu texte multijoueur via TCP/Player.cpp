#include "Player.h"
#include "Potion.h"

Player::Player(int health, std::string name, Weapon* currentWeapon) : Character(health), m_name(name) {
	m_currentWeapon = currentWeapon;
	m_currentRoomId = 0;
}

Player::~Player()
{
	if (m_currentWeapon != nullptr)
	{
		delete m_currentWeapon;
		m_currentWeapon = nullptr;
	}
}

std::string Player::GetName() const
{
	return m_name;
}

int Player::GetCurrentRoomId() const
{
	return m_currentRoomId;
}

void Player::SetCurrentRoomId(int room)
{
	m_currentRoomId = room;
}

std::string Player::Die() const
{
	return "You are dead !\n";
}

std::string Player::Heal(int heal)
{
	if (m_currentHealth == m_maxHealth)
		return "HP already at its maximum\n";
	m_currentHealth += heal;
	if (m_currentHealth > m_maxHealth)
	{
		m_currentHealth = m_maxHealth;
		return "HP at maximum\n";
	}
	return "You have gained " + std::to_string(heal) + " HP\n";
}


std::string Player::TakeItem(Item* item) {
	if (item->GetItemType() == ItemType::WeaponItemType) {
		if (m_currentWeapon) delete m_currentWeapon;

		m_currentWeapon = static_cast<Weapon*>(item);
		return "You equipped: " + m_currentWeapon->GetName() + "\n";
	}
	else if (item->GetItemType() == ItemType::PotionItemType) {
		Potion* potion = static_cast<Potion*>(item);
		std::string healMessage = potion->HealPlayer(*this);
		delete potion;                                  
		return healMessage;                              
	}
	else 
		return "Unknown item type.\n";
}


Weapon* Player::GetCurrentWeapon() const
{
	return m_currentWeapon;
}

void Player::SetCurrentWeapon(Weapon* weapon)
{
	if (weapon != nullptr) {
		if (m_currentWeapon) 
			delete m_currentWeapon;
		m_currentWeapon = weapon;
	}
}

std::string Player::UseWeapon(Weapon* weapon, Character& target)
{
	if (weapon != nullptr)
	{
		return m_name + " attack !\n" + weapon->Attack(target, 1);
	}
	else
		return "No weapons equipped to attack ! \n";
}



