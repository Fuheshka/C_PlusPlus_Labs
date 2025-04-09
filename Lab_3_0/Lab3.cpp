#include <iostream>
#include <string>

// Класс Character
class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // Перегрузка оператора ==
    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

// Класс Weapon
class Weapon {
private:
    std::string name;
    int damage;
    int weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, int w)
        : name(n), damage(d), weight(w) {
    }

    // Геттеры
    std::string getName() const { return name; }
    int getDamage() const { return damage; }
    int getWeight() const { return weight; }

    // Перегрузка оператора +
    Weapon operator+(const Weapon& other) const {
        std::string newName = name + "-" + other.name;
        int newDamage = damage + other.damage;
        int newWeight = weight + other.weight;
        return Weapon(newName, newDamage, newWeight);
    }

    // Перегрузка оператора >
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage
            << ", Weight: " << weapon.weight;
        return os;
    }
};

// Функция main
int main() {
    // Тестирование Character
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    std::cout << hero1 << std::endl;

    // Тестирование Weapon
    Weapon sword("Sword", 50, 10);
    Weapon bow("Bow", 30, 5);

    Weapon combined = sword + bow;
    std::cout << "\nCombined Weapon:\n" << combined << std::endl;

    if (sword > bow) {
        std::cout << sword.getName() << " is stronger than " << bow.getName() << "!\n";
    }
    else {
        std::cout << bow.getName() << " is stronger than " << sword.getName() << "!\n";
    }

    return 0;
}
