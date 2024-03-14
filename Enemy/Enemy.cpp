// Francisco Alejandro Gonzalez Herrera 23310145
#include "Enemy.h"
#include <iostream>


using namespace std;

//TODO: Check the circular dependency
int getRolledAttack(int attack) {
    int lowerLimit = attack * .80;
    return (rand() % (attack - lowerLimit)) + lowerLimit;
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
        cout<<getName()<<" ha muerto"<<endl;
    }
    else {
        cout<<getName()<<" ha recibido " << damage << " de daño" << endl;
    }
}

Character* Enemy::getTarget(vector<Player *> teamMembers) {
    
    int targetIndex = 0;
    int lowestHealth = INT_MAX;
    for(int i=0; i < teamMembers.size(); i++) {
        if(teamMembers[i]->getHealth() < lowestHealth) {
            lowestHealth = teamMembers[i]->getHealth();
            targetIndex = i;
        }
    }

    return teamMembers[targetIndex];
}

Action Enemy::takeAction(vector<Player *> player) {
    Action myAction;
    myAction.speed = getSpeed();
    myAction.subscriber = this;
    Character* target = getTarget(player);
    
    // se calcula el porcentaje de vida actual.
    double currentHealthPercent = static_cast<double>(getHealth()) / static_cast<double>(getMaxHealth());

    // Comprobar si el enemigo tiene menos del 15% de vida
    if (currentHealthPercent < 0.15) {
        // Generar un número aleatorio entre 0 y 99 (inclusive)
        int escapeChance = rand() % 100;
        
        // Si el número generado está en el rango del 5% el enemigo va escapar
        if (escapeChance < 5) {
            cout << getName() << " ha escapado." << endl;
            myAction.action = []() {
                // Acción de escape: no hace nada
            };
            return myAction;
        }
    }

    // Si no se escapa va realizarse el ataque normalmente
    myAction.target = target;
    myAction.action = [this, target]() {
        doAttack(target);
    };

    return myAction;
}