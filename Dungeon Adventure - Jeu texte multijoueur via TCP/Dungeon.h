#ifndef DUNGEON_H
#define DUNGEON_H

#include "Room.h"
#include "Weapon.h"
#include "Sword.h"
#include "MagicWand.h"
#include "Bomb.h"
#include "Knife.h"
#include "Slingshot.h"

#include "Potion.h"

#include "Goblin.h"
#include "Mage.h"
#include "Orc.h"

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <random>


// Classe qui va instancier les pièces, les ennemis et les items
class Dungeon {
private:
    std::map<int, Room*> m_rooms;

public:
    Dungeon() {
        Init();
    }
    ~Dungeon();

    void Init();
    Weapon* GenerateWeapon(); // Choisi les armes de façon aléatoire, pour que cela change à chaque lancement du serveur
    std::map<int, Room*>& GetRooms();
};

#endif