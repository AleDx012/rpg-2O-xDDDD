// Francisco Alejandro Gonzalez Herrera 23310145

#include "Enemy.h"
#include <iostream>

using namespace std;

int getRolledAttack(int attack) {
    int lowerLimit = attack * 0.80; 
    return (rand() % (attack - lowerLimit)) + lowerLimit;
}

// Función para comparar la velocidad de dos jugadores
bool compareSpeed(Player *a, Player *b) {
    return a->getSpeed() > b->getSpeed();
}

// Constructor de la clase Enemy.
Enemy::Enemy(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, false) {
}

// Método para que el enemigo realice un ataque
void Enemy::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

// Método para que el enemigo reciba daño
void Enemy::takeDamage(int damage) {
    setHealth(getHealth() - damage);
    if(getHealth() <= 0) {
        cout << getName() << " ha muerto" << endl;
    }
    else {
        cout << getName() << " ha recibido " << damage << " puntos de daño" << endl;
    }
}

// enemigo huya del combate
void Enemy::flee(vector<Player *> partyMembers) {
    std::sort(partyMembers.begin(), partyMembers.end(), compareSpeed);
    Player *fastestPlayer = partyMembers[0];
    bool huyo = false;
    if (this->getSpeed() > fastestPlayer->getSpeed()) {
        huyo = true;
    } else {
        srand(time(nullptr));
        int chance = rand() % 100;
        cout << "Probabilidad de escapar: " << chance << "%" << endl;
        huyo = chance > 95;
    }

    this->huyo = huyo;
}

// Método para seleccionar un objetivo para atacar
Character* Enemy::getTarget(vector<Player *> partyMembers) {
    // Jugador del equipo con menos vida
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

// Método para que el enemigo realice una acción en el combate.
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