#include <iostream>
#include <string>
#include <memory>

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
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Геттеры
    std::string getName() const {
        return name;
    }

    int getDamage() const {
        return damage;
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name << ", Damage: " << damage << ", Weight: " << weight << std::endl;
    }
};

// Класс Character
class Character {
private:
    std::string name;
    int health;
    int baseAttack;
    int defense;
    std::unique_ptr<Weapon> equippedWeapon;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), baseAttack(a), defense(d), equippedWeapon(nullptr) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    // Метод для экипировки оружия
    void equipWeapon(std::unique_ptr<Weapon> weapon) {
        if (equippedWeapon) {
            std::cout << name << " already has a weapon equipped. Unequipping first.\n";
            unequipWeapon();
        }
        equippedWeapon = std::move(weapon);
        baseAttack += equippedWeapon->getDamage();
        std::cout << name << " equipped " << equippedWeapon->getName() << ".\n";
    }

    // Метод для снятия оружия
    void unequipWeapon() {
        if (equippedWeapon) {
            baseAttack -= equippedWeapon->getDamage();
            std::cout << name << " unequipped " << equippedWeapon->getName() << ".\n";
            equippedWeapon.reset();
        }
        else {
            std::cout << name << " has no weapon to unequip.\n";
        }
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << ", Attack: " << baseAttack
            << ", Defense: " << defense;
        if (equippedWeapon) {
            std::cout << ", Equipped Weapon: " << equippedWeapon->getName();
        }
        std::cout << std::endl;
    }
};

// Класс Monster
class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

// Функция main для демонстрации
int main() {
    // Создание персонажа
    Character hero("Hero", 100, 20, 10);

    // Создание монстра
    Monster goblin("Goblin", 50, 15, 5);

    // Создание оружий
    auto sword = std::make_unique<Weapon>("Sword", 50, 10);
    auto bow = std::make_unique<Weapon>("Bow", 30, 5);

    // Вывод начальной информации
    std::cout << "\nInitial info:\n";
    hero.displayInfo();
    goblin.displayInfo();

    // Экипировка меча
    hero.equipWeapon(std::move(sword));
    std::cout << "\nAfter equipping sword:\n";
    hero.displayInfo();

    // Снятие меча
    hero.unequipWeapon();
    std::cout << "\nAfter unequipping sword:\n";
    hero.displayInfo();

    // Экипировка лука
    hero.equipWeapon(std::move(bow));
    std::cout << "\nAfter equipping bow:\n";
    hero.displayInfo();

    return 0;
}