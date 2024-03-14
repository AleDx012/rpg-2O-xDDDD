#include <iostream>
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Combat/Combat.h"
#include <queue>

using namespace std;

int main() {
    Player *player = new Player("Alejandro", 100, 10, 25, 50);
    Enemy *enemy = new Enemy("Pepe", 20, 5, 2, 7);
    Enemy *enemy2 = new Enemy("Ferchis", 1000,100, 200,150 );

    vector<Character*> participants;
    participants.push_back(player);
    participants.push_back(enemy);
    participants.push_back(enemy2);

    Combat* combat = new Combat(participants);
    combat->doCombat();

    delete player;
    delete enemy;
    delete enemy2;
    delete combat;

    return 0;
}
