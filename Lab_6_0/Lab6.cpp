#include <iostream>
#include <string>
#include <vector>
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

    int getHealth() const {
        return health;
    }

    virtual ~Entity() {}
};

// Игрок
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

// Враг
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

// Шаблонный класс GameManager с обработкой исключений
template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        if (entity->getHealth() <= 0) {
            throw std::invalid_argument("Entity has invalid health (<= 0)");
        }
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
};

// Шаблонный класс Queue с исключениями
template <typename T>
class Queue {
private:
    std::vector<T> items;

public:
    void push(const T& item) {
        items.push_back(item);
    }

    void pop() {
        if (items.empty()) {
            throw std::out_of_range("Queue is empty — cannot pop.");
        }
        items.erase(items.begin());
    }

    void display() const {
        std::cout << "Queue contents:\n";
        for (const auto& item : items) {
            std::cout << item << std::endl;
        }
    }
};

// Функция main
int main() {
    // Работа с GameManager + try-catch
    GameManager<Entity*> manager;

    try {
        manager.addEntity(new Player("Hero", 100, 0)); // OK
        manager.addEntity(new Enemy("Zombie", -10, "Undead")); // Ошибка
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error adding entity: " << e.what() << std::endl;
    }

    manager.displayAll();

    // Работа с очередью и исключением
    Queue<std::string> queue;
    queue.push("Potion");
    queue.push("Elixir");

    queue.display();
    queue.pop(); // OK
    queue.pop(); // OK

    try {
        queue.pop(); // Ошибка — очередь пуста
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Queue error: " << e.what() << std::endl;
    }

    return 0;
}
