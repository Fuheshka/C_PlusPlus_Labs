#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower; // ���� ������������� � attack �� attackPower
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {
    }

    // �������
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defense; }

    // �������
    void setHealth(int h) {
        health = h;
        if (health < 0) health = 0;
        if (health > 100) health = 100;
    }

    // ����������� ����� ��� �����
    virtual void attack(Entity& target) {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // ����������� ����� ��� ������ ����������
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    // ����� heal � ������� ������
    virtual void heal(int amount) {
        setHealth(health + amount);
        std::cout << name << " heals for " << amount << " HP!\n";
    }

    // ����������� ����������
    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) { // 20% ���� ������������ �����
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    void heal(int amount) override {
        setHealth(health + amount * 2); // ��������� �������
        std::cout << name << " heals effectively for " << amount << " HP (doubled to " << amount * 2 << ")!\n";
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) { // 30% ���� �������� �����
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 40) { // 40% ���� ��������� �����
                damage += 10;
                std::cout << "Fire Strike! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); // ������������� ���������� ��������� �����

    // �������� ��������
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 150, 25, 20);

    // ������ ���������� ��� ������������
    Entity* entities[] = { &hero, &goblin, &dragon };

    // ����� ���������� ���������
    std::cout << "Initial state:\n";
    for (auto* entity : entities) {
        entity->displayInfo();
    }

    // ���
    std::cout << "\nBattle begins:\n";
    hero.attack(goblin);
    goblin.attack(hero);
    dragon.attack(hero);

    // ��������� ����� ���
    std::cout << "\nState after attacks:\n";
    for (auto* entity : entities) {
        entity->displayInfo();
    }

    // ������������ �������
    std::cout << "\nHealing demonstration:\n";
    hero.heal(20);
    goblin.heal(20);
    dragon.heal(20);

    // ��������� ���������
    std::cout << "\nFinal state:\n";
    for (auto* entity : entities) {
        entity->displayInfo();
    }

    return 0;
}