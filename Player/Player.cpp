//FRANCISCO ALEJANDRO GONZALEZ HERRERA 23310145 
#include "Player.h"
#include <iostream>
#include "../Utils.h"

using namespace std;
using namespace combat_utils;

// Función para comparar la velocidad de dos enemigos
bool compareSpeed(Enemy *a, Enemy *b) {
    return a->getSpeed() > b->getSpeed();
}

// Constructor de la clase Player
Player::Player(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense,
                                                                                        speed, true) {
    experience = 0;
    level = 1;
}

// Método para que el jugador realice un ataque
void Player::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

// Método para que el jugador reciba daño
void Player::takeDamage(int damage) {
    setHealth(health - damage);
    cout << "Has recibido " << damage << " puntos de daño" << endl;
    if (health <= 0) {
        cout << "Has muerto" << endl;
    }
}

// Método para que el jugador huya del combate
void Player::flee(vector<Enemy *> enemies) {
    std::sort(enemies.begin(), enemies.end(), compareSpeed);
    Enemy *fastestEnemy = enemies[0];
    bool huyo = false;
    if (this->getSpeed() > fastestEnemy->getSpeed()) {
        huyo = true;
    } else {
        srand(time(nullptr));
        int chance = rand() % 100;
        cout << "Probabilidad de escapar: " << chance << "%" << endl;
        huyo = chance > 97;
    }

    this->huyo = huyo;
}

// Método para subir de nivel al jugador
void Player::levelUp() {
    level++;
    setHealth(getHealth() + 10);
    setAttack(getAttack() + 5);
    setDefense(getDefense() + 5);
    setSpeed(getSpeed() + 5);
}

// Método para ganar experiencia
void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        levelUp();
        experience = 0;
    }
}

// Método para seleccionar un objetivo para atacar
Character *Player::getTarget(vector<Enemy *> enemies) {
    cout << "Elige un objetivo" << endl;
    int targetIndex = 0;
    for (int i = 0; i < enemies.size(); i++) {
        cout << i << ". " << enemies[i]->getName() << endl;
    }
    cin >> targetIndex;
    //TODO: Agregar validación de entrada
    return enemies[targetIndex];
}

// Método para que el jugador realice una acción en el combate
Action Player::takeAction(const vector<Enemy *>& enemies) {
    int option = 0;
    cout << "Elige una acción" << endl;
    cout << "1. Atacar" << endl;
    cout << "2. Huir" << endl;
    cin >> option;
    Character *target = nullptr;

    // Esta variable guarda
    // 1. ¿Qué voy a hacer?
    // 2. ¿Con qué velocidad/prioridad?
    Action myAction;
    // 2.
    myAction.speed = this->getSpeed();
    myAction.subscriber = this;

    switch (option) {
        case 1:
            target = getTarget(enemies);
            myAction.target = target;
            // 1.
            myAction.action = [this, target]() {
                doAttack(target);
            };
            break;
        case 2:
            myAction.action = [this, enemies]() {
                flee(enemies);
            };
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
    }

    return myAction;
}