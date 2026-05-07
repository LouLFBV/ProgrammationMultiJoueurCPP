#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Enemy.h"
#include "Item.h"

class Room {
private:
    int m_id;
    std::string m_description;
    std::vector<int> m_exits;       
    std::vector<Item*> m_items;
    std::vector<Enemy*> m_enemies;
public: 

    Room(int id, std::string description);
    ~Room();

    int GetId() const;
    std::string GetDescription() const;
    std::vector<int> GetExits() const;       
    std::vector<Item*>& GetItems();
    std::vector<Enemy*>& GetEnemies();

    void AddExit(int roomId);
    void AddItem(Item* item);
    void AddEnemy(Enemy* monster);
};