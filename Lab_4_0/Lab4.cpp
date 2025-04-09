#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Базовый класс Entity
class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }
    virtual ~Entity() {}
};

// Класс Player
class Player : public Entity {
private:
    int experience;

public:
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Experience: " << experience << std::endl;
    }
};

// Класс Enemy
class Enemy : public Entity {
private:
    std::string type;

public:
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << type << std::endl;
    }
};

// Класс Inventory
class Inventory {
private:
    std::unique_ptr<std::string[]> items;
    int capacity;
    int count;

public:
    // Конструктор с начальной вместимостью
    Inventory(int cap = 10)
        : capacity(cap), count(0) {
        items = std::make_unique<std::string[]>(capacity);
    }

    // Метод добавления предмета
    void addItem(const std::string& item) {
        if (count < capacity) {
            items[count++] = item;
        }
        else {
            std::cout << "Inventory full! Cannot add: " << item << std::endl;
        }
    }

    // Метод отображения инвентаря
    void displayInventory() const {
        std::cout << "\nInventory contents:\n";
        for (int i = 0; i < count; ++i) {
            std::cout << "- " << items[i] << std::endl;
        }
    }
};

// Функция main
int main() {
    // Массив умных указателей на базовый класс
    std::unique_ptr<Entity> entities[] = {
        std::make_unique<Player>("Hero", 100, 10),
        std::make_unique<Enemy>("Goblin", 50, "Goblin")
    };

    // Вывод информации о сущностях
    std::cout << "Entities:\n";
    for (const auto& entity : entities) {
        entity->displayInfo();
        std::cout << std::endl;
    }

    // Работа с инвентарём
    Inventory inv;
    inv.addItem("Sword");
    inv.addItem("Health Potion");
    inv.addItem("Bow");
    inv.displayInventory();

    return 0;
}
