# ProgrammationMultiJoueurCPP

un jeu de type “dungeon crawler” au format texte qui réutilise mon précédent projet.
Les joueurs se connectent à un serveur sur le port 54000, explorent des salles, combattant des monstres, ramassent des items et discutent entre eux.
Le donjon est entièrement géré par le serveur. Le client est une interface console simple : il lit les commandes du joueur via std::cin, les envoie au serveur, et écrit ce qu’envoie le serveur via std::cout.

### Protocole

Le protocole est basé sur des lignes. Chaque message, client-serveur ou serveur-client, se termine par un retour chariot (\n), avec END en plus pour le serveur car il peut 
envoyer des mesagges avec plusieurs \n.

Lorsque le client se connecte, le serveur lui envoie un message de bienvenue et demande le nom du joueur. Le client répond avec le nom choisi, puis le serveur confirme et envoie une description de la salle de départ.

### Commandes

Le client et le serveur doivent supporter les commandes suivantes :

Commande :	Effet
LOOK	: Affiche la description de la salle actuelle, les sorties, ainsi que les monstres et items présents.
MOVE <room_id> :	Déplace le joueur vers la salle demandée. Le serveur valide que cette salle existe.
ATTACK <monster_name> :	Attaque un monstre dans la salle actuelle. Le serveur résout le combat et répond par les dommages subis et donnés.
PICKUP <item_name>	: Ramasse un item dans la salle actuelle. L’item disparaît de la salle.
STATUS :	Affiche les PV du joueur, ses stats d’attaque et dans quelle salle il se trouve.
SAY <text> :	Broadcaster un message à tous les joueurs connectés au serveur.
QUIT :	Quitter le serveur. Le serveur doit notifier les autres joueurs.
HELP : Permet d'afficher toutes les commandes

### Réponses du serveur

Le serveur n’envoie pas ses réponses en temps-réel, puisque cela interfèrerait avec le jeu. Il conserve les messages en attente, puis envoie toute la liste, lorsque le client a envoyé une commande et attend une réponse. En conséquence, le serveur doit envoyer au moins une réponse pour chaque commande, mais peut envoyer plusieurs réponses d’un coup.

Les erreurs commencent par le texte “ERROR:”.

### Le donjon

Le donjon doit contient 6 salles, reliées en graphe. Chaque salle a une description, une liste de sorties (numéros de salle), ainsi que possiblement un ou des items, et possiblement un ou des monstres qui ont des armes déterminées aléatoirement.
Le donjon complet doit contient 3 types de monstres distincts (Goblin, Mage et Orc), et 2 types d’items (une arme et une potion de PV) avec plusieurs armes : couteau, épée, lance-pierre, bombes et baguette magique.

### Combat

Lorsque le joueur attaque :
    • Le joueur donne PA + random(0…3) points de dégât
    • Dans un second temps, si le monstre est toujours en vie, il donne PA_MONSTRE + random(0…3)
    • Le serveur notifie tous les joueurs du résultat du coup
    • Si un joueur atteint 0 PV, il est marqué comme mort et ne peut plus interagir avec le monde (il peut toujours utiliser SAY et QUIT).

### Déconnexion

Lorsque le client quitte le serveur, le serveur retire le joueur de la liste qu’il maintient et notifie les autres joueurs. Lorsque le serveur quitte, les clients affichent une erreur et quittent.

### Indication

Puisque TCP est un protocole de stream, le serveur accumule les octets reçus dans une std::string propre à chaque joueur et leur envoie au prochain échange, puis il sépare les messages ligne par ligne.
