#include <iostream>
#include <string>
#include <memory>

// ����� Weapon
class Weapon {
private:
    std::string name;
    int damage;
    int weight;

public:
    // �����������
    Weapon(const std::string& n, int d, int w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // ����������
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // �������
    std::string getName() const {
        return name;
    }

    int getDamage() const {
        return damage;
    }

    // ����� ��� ������ ����������
    void displayInfo() const {
        std::cout << "Name: " << name << ", Damage: " << damage << ", Weight: " << weight << std::endl;
    }
};

// ����� Character
class Character {
private:
    std::string name;
    int health;
    int baseAttack;
    int defense;
    std::unique_ptr<Weapon> equippedWeapon;

public:
    // �����������
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), baseAttack(a), defense(d), equippedWeapon(nullptr) {
        std::cout << "Character " << name << " created!\n";
    }

    // ����������
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    // ����� ��� ���������� ������
    void equipWeapon(std::unique_ptr<Weapon> weapon) {
        if (equippedWeapon) {
            std::cout << name << " already has a weapon equipped. Unequipping first.\n";
            unequipWeapon();
        }
        equippedWeapon = std::move(weapon);
        baseAttack += equippedWeapon->getDamage();
        std::cout << name << " equipped " << equippedWeapon->getName() << ".\n";
    }

    // ����� ��� ������ ������
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

    // ����� ��� ������ ����������
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << ", Attack: " << baseAttack
            << ", Defense: " << defense;
        if (equippedWeapon) {
            std::cout << ", Equipped Weapon: " << equippedWeapon->getName();
        }
        std::cout << std::endl;
    }
};

// ����� Monster
class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // �����������
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // ����������
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    // ����� ��� ������ ����������
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

// ������� main ��� ������������
int main() {
    // �������� ���������
    Character hero("Hero", 100, 20, 10);

    // �������� �������
    Monster goblin("Goblin", 50, 15, 5);

    // �������� ������
    auto sword = std::make_unique<Weapon>("Sword", 50, 10);
    auto bow = std::make_unique<Weapon>("Bow", 30, 5);

    // ����� ��������� ����������
    std::cout << "\nInitial info:\n";
    hero.displayInfo();
    goblin.displayInfo();

    // ���������� ����
    hero.equipWeapon(std::move(sword));
    std::cout << "\nAfter equipping sword:\n";
    hero.displayInfo();

    // ������ ����
    hero.unequipWeapon();
    std::cout << "\nAfter unequipping sword:\n";
    hero.displayInfo();

    // ���������� ����
    hero.equipWeapon(std::move(bow));
    std::cout << "\nAfter equipping bow:\n";
    hero.displayInfo();

    return 0;
}