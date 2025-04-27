#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// Базовый класс
class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }
    virtual std::string getType() const = 0;
    virtual std::string serialize() const = 0;
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

    std::string getType() const override {
        return "Player";
    }

    std::string serialize() const override {
        return getType() + " " + name + " " + std::to_string(health) + " " + std::to_string(experience);
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

    std::string getType() const override {
        return "Enemy";
    }

    std::string serialize() const override {
        return getType() + " " + name + " " + std::to_string(health) + " " + type;
    }
};

// Шаблонный класс GameManager
template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    const std::vector<T>& getEntities() const {
        return entities;
    }

    void clear() {
        entities.clear();
    }
};

// Функция сохранения
void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    for (const auto& entity : manager.getEntities()) {
        file << entity->serialize() << std::endl;
    }
}

// Функция загрузки
void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    std::string type, name;
    int health;
    manager.clear(); // Очистка перед загрузкой

    while (file >> type >> name >> health) {
        if (type == "Player") {
            int exp;
            file >> exp;
            manager.addEntity(new Player(name, health, exp));
        }
        else if (type == "Enemy") {
            std::string enemyType;
            file >> enemyType;
            manager.addEntity(new Enemy(name, health, enemyType));
        }
    }
}

// main
int main() {
    try {
        GameManager<Entity*> manager;
        manager.addEntity(new Player("Hero", 100, 15));
        manager.addEntity(new Enemy("Goblin", 50, "Goblin"));
        manager.addEntity(new Player("Mage", 80, 45));
        manager.addEntity(new Enemy("Dragon", 200, "Dragon"));

        std::cout << "Original entities:\n";
        manager.displayAll();

        saveToFile(manager, "game_save.txt");

        GameManager<Entity*> loadedManager;
        loadFromFile(loadedManager, "game_save.txt");

        std::cout << "\nLoaded entities from file:\n";
        loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "\nException: " << e.what() << std::endl;
    }

    return 0;
}
