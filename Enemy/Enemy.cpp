// Francisco Alejandro Gonzalez Herrera 23310145
#include "Enemy.h"
#include <iostream>
#include <algorithm>

using namespace std;

Enemy::Enemy(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, false) {
}

void Enemy::doAttack(Character* target) {
    int rolledAttack = rand() % getAttack();
    int trueDamage = max(0, rolledAttack - target->getDefense());
    target->takeDamage(trueDamage);
}

void Enemy::takeDamage(int damage) {
    setHealth(getHealth() - damage);
    if (getHealth() <= 0) {
        cout << getName() << " has died" << endl;
    }
    else {
        cout << getName() << " has taken " << damage << " damage" << endl;
    }
}

Character* Enemy::getTarget(vector<Player*> teamMembers) {
    auto target = *min_element(teamMembers.begin(), teamMembers.end(),
        [](const Player* a, const Player* b) {
            return a->getHealth() < b->getHealth();
        });
    return target;
}

Action Enemy::takeAction(vector<Player*> player) {
    Action myAction;
    myAction.speed = getSpeed();
    myAction.subscriber = this;
    Character* target = getTarget(player);
    myAction.target = target;

    if (getHealth() <= 0.15 * getMaxHealth()) {
        flee = true;
        myAction.action = [this]() {
            cout << getName() << " is fleeing!" << endl;
            };
    }
    else {
        myAction.action = [this, target]() {
            doAttack(target);
            };
    }

    return myAction;
}