// Francisco Alejandro Gonzalez Herrera 23310145
#include "Enemy.h"
#include <iostream>

using namespace std;

// Función para obtener el ataque aleatorio dentro del rango
int getRolledAttack(int attack) {
    int lowerLimit = attack * 0.80; 
    return (rand() % (attack - lowerLimit)) + lowerLimit;
}

bool compareSpeed(Player *a, Player *b) {
    return a->getSpeed() > b->getSpeed();
}

Enemy::Enemy(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, false) {
}

void Enemy::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

void Enemy::takeDamage(int damage) {
    setHealth(getHealth() - damage);
    if(getHealth() <= 0) {
        cout << getName() << " has died" << endl;
    }
    else {
        cout << getName() << " has taken " << damage << " damage" << endl;
    }
}

void Enemy::flee(vector<Player *> partyMembers) {
    std::sort(partyMembers.begin(), partyMembers.end(), compareSpeed);
    Player *fastestPlayer = partyMembers[0];
    bool fled = false;
    if (this->getSpeed() > fastestPlayer->getSpeed()) {
        fled = true;
    } else {
        srand(time(nullptr));
        int chance = rand() % 100;
        cout << "Chance to escape: " << chance << "%" << endl;
        fled = chance > 95;
    }

    this->fleed = fled; // Cambiado el nombre de la variable de "fleed" a "fled"
}

Character* Enemy::getTarget(vector<Player *> partyMembers) {
    // Miembro del equipo con menor vida
    int targetIndex = 0;
    int lowestHealth = INT_MAX;
    for(int i=0; i < partyMembers.size(); i++) {
        if(partyMembers[i]->getHealth() < lowestHealth) {
            lowestHealth = partyMembers[i]->getHealth();
            targetIndex = i;
        }
    }

    return partyMembers[targetIndex];
}


Action Enemy::takeAction(const vector<Player *>& players) {
    Action myAction;
    myAction.speed = getSpeed();
    myAction.subscriber = this;
    Character* target = nullptr;
    if (getHealth() <= (getHealth() * 0.15)){ // Cambiado el número por el valor decimal
        myAction.action = [this, players]() {
            flee(players);
        };
    } else {
        target = getTarget(players);
        myAction.target = target;
        myAction.action = [this, target]() {
            doAttack(target);
        };
    }

    return myAction;
}