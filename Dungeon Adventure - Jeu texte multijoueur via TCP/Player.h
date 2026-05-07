#pragma once
#include <vector>
#include "Character.h"
#include "Item.h"

class Weapon;

class Player : public Character
{
private:
    std::string m_name;
    Weapon* m_currentWeapon;
    int m_currentRoomId;

public:
    Player(int health, std::string name, Weapon* currentWeapon);
    ~Player();

    std::string GetName() const override;
    int GetCurrentRoomId() const;
    Weapon* GetCurrentWeapon() const;
    void SetCurrentWeapon(Weapon* weapon);
    void SetCurrentRoomId(int room);
    std::string Die() const override;
    std::string Heal(int heal);
    std::string TakeItem(Item* item);
    std::string UseWeapon(Weapon* weapon, Character& target) override;
};