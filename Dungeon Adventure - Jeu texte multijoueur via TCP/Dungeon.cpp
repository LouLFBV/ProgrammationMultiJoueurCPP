#include "Dungeon.h"


Dungeon::~Dungeon()
{
    for (auto& pair : m_rooms) {
        delete pair.second;
    }
    m_rooms.clear();
}

void Dungeon::Init()
{

    // SALLE 0
    m_rooms[0] = new Room(0, "A cold stone entrance. Moss covers the damp walls.");
    m_rooms[0]->AddExit(1);
    m_rooms[0]->AddItem(GenerateWeapon());

    // SALLE 1
    m_rooms[1] = new Room(1, "Dark room. Rusty chains hang from the ceiling.");
    m_rooms[1]->AddExit(0);
    m_rooms[1]->AddExit(2);
    m_rooms[1]->AddExit(3);
    m_rooms[1]->AddItem(new Potion("Little Potion of Life", 3));
    m_rooms[1]->AddEnemy(new Goblin(10, GenerateWeapon()));

    // SALLE 2
    m_rooms[2] = new Room(2, "An abandoned lab. Broken vials leak a glowing vapor.");
    m_rooms[2]->AddExit(1);
    m_rooms[2]->AddExit(3);
    m_rooms[2]->AddExit(4);
    m_rooms[2]->AddItem(new Potion("Potion of Life", 5));
    m_rooms[2]->AddItem(GenerateWeapon());
    m_rooms[2]->AddEnemy(new Mage(15, GenerateWeapon()));

    // SALLE 3
    m_rooms[3] = new Room(3, "The King's hall. Huge shadows dance on the blood-stained walls.");
    m_rooms[3]->AddExit(1);
    m_rooms[3]->AddExit(2);
    m_rooms[3]->AddExit(4);
    m_rooms[3]->AddExit(5);
    m_rooms[3]->AddItem(new Potion("Potion of Life", 5));
    m_rooms[3]->AddEnemy(new Goblin(10, GenerateWeapon()));
    m_rooms[3]->AddEnemy(new Goblin(10, GenerateWeapon()));
    m_rooms[3]->AddEnemy(new Goblin(10, GenerateWeapon()));

    // SALLE 4
    m_rooms[4] = new Room(4, "A grim chamber. The air is thick with the scent of old blood.");
    m_rooms[4]->AddExit(2);
    m_rooms[4]->AddExit(3);
    m_rooms[4]->AddExit(6);
    m_rooms[4]->AddItem(GenerateWeapon());
    m_rooms[4]->AddItem(GenerateWeapon());
    m_rooms[4]->AddEnemy(new Goblin(15, GenerateWeapon()));

    // SALLE 5
    m_rooms[5] = new Room(5, "A cursed altar. Floating books burn with blue fire.");
    m_rooms[5]->AddExit(3);
    m_rooms[5]->AddExit(6);
    m_rooms[5]->AddItem(new Potion("Big Potion of Life", 15));
    m_rooms[5]->AddEnemy(new Goblin(11, GenerateWeapon()));
    m_rooms[5]->AddEnemy(new Goblin(11, GenerateWeapon()));
    m_rooms[5]->AddEnemy(new Mage(20, GenerateWeapon()));

    // SALLE 6
    m_rooms[6] = new Room(6, "The final balcony. A freezing wind carries the smell of death.");
    m_rooms[6]->AddExit(4);
    m_rooms[6]->AddExit(5);
    m_rooms[6]->AddEnemy(new Orc(20, GenerateWeapon()));
    m_rooms[6]->AddEnemy(new Orc(25, GenerateWeapon()));
}

Weapon* Dungeon::GenerateWeapon()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4); 

    int choice = dis(gen);
    switch (choice) {
    case 0: 
        return new Sword(7);
    case 1: 
        return new Knife(5);
    case 2: 
        return new Bomb(10);
    case 3: 
        return new MagicWand(6);
    case 4: 
        return new Slingshot(4);
    default: 
        return new Sword(7);
    }
}

std::map<int, Room*>& Dungeon::GetRooms()
{
    return m_rooms;
}
