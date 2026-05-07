#include "Room.h"

Room::Room(int id, std::string description) : m_id(id), m_description(description) {}

Room::~Room() {
	for (Enemy* e : m_enemies) delete e;
	for (Item* i : m_items) delete i;
}

int Room::GetId() const
{
	return m_id;
}

std::string Room::GetDescription() const
{
	return m_description;
}

std::vector<int> Room::GetExits() const
{
	return m_exits;
}

std::vector<Item*>& Room::GetItems()
{
	return m_items;
}

std::vector<Enemy*>& Room::GetEnemies()
{
	return m_enemies;
}



void Room::AddExit(int roomId) 
{
	m_exits.push_back(roomId); 
}

void Room::AddItem(Item* item)
{
	m_items.push_back(item);
}

void Room::AddEnemy(Enemy* enemy)
{
	m_enemies.push_back(std::move(enemy));
}