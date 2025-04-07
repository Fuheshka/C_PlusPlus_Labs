#include <iostream>
#include <string>

class Entity {
protected:
    std::string name; // ���������� ����: ���
    int health;       // ���������� ����: ��������

public:
    // ����������� �������� ������
    Entity(const std::string& n, int h) : name(n), health(h) {}

    // ����������� ����� ��� ������ ����������
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    // ����������� ����������
    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience; // ��������� ����: ����

public:
    // ����������� ������������ ������
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    // ��������������� ������ displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // ����� ������ �������� ������
        std::cout << "Experience: " << experience << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string type; // ��������� ����: ��� �����

public:
    // ����������� ������������ ������
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    // ��������������� ������ displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // ����� ������ �������� ������
        std::cout << "Type: " << type << std::endl;
    }
};

// ������� 1: �������� ������ Boss, ������������ �� Enemy
class Boss : public Enemy {
private:
    std::string specialAbility; // ��������� ����: ������ �����������

public:
    // ����������� ������ Boss
    Boss(const std::string& n, int h, const std::string& t, const std::string& ability)
        : Enemy(n, h, t), specialAbility(ability) {
    }

    // ������� 2: ��������������� ������ displayInfo
    void displayInfo() const override {
        Enemy::displayInfo(); // ����� ������ �������� ������ Enemy
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }
};

int main() {
    // ������� ������� ������, ����� � �����
    Player hero("Hero", 100, 0);
    Enemy monster("Goblin", 50, "Goblin");
    Boss boss("Dragon King", 200, "Dragon", "Fire Breath");

    // ������� ���������� � ����������
    std::cout << "Player Info:" << std::endl;
    hero.displayInfo();

    std::cout << "\nEnemy Info:" << std::endl;
    monster.displayInfo();

    // ������� 3: ������������ ������ ������ Boss
    std::cout << "\nBoss Info:" << std::endl;
    boss.displayInfo();

    return 0;
}