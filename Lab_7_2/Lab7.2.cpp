#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

// Базовый класс
class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        if (h <= 0) throw std::invalid_argument("Health must be positive.");
        if (n.empty()) throw std::invalid_argument("Name cannot be empty.");
        if (a < 0 || d < 0) throw std::invalid_argument("Attack and defense must be non-negative.");
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    virtual std::string getType() const = 0;
    virtual std::string serialize() const = 0;
    virtual ~Entity() = default;

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

    void takeDamage(int damage) {
        if (damage > 0) {
            health -= damage;
            if (health < 0) health = 0;
        }
    }
};

// Класс Player
class Player : public Entity {
private:
    int experience;

public:
    Player(const std::string& n, int h, int a, int d, int exp)
        : Entity(n, h, a, d), experience(exp) {
        if (exp < 0) throw std::invalid_argument("Experience cannot be negative.");
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Experience: " << experience << std::endl;
    }

    std::string getType() const override {
        return "Player";
    }

    std::string serialize() const override {
        return getType() + " " + name + " " + std::to_string(health) + " " +
            std::to_string(attack) + " " + std::to_string(defense) + " " +
            std::to_string(experience);
    }
};

// Класс Enemy
class Enemy : public Entity {
private:
    std::string type;

public:
    Enemy(const std::string& n, int h, int a, int d, const std::string& t)
        : Entity(n, h, a, d), type(t) {
        if (t.empty()) throw std::invalid_argument("Enemy type cannot be empty.");
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << type << std::endl;
    }

    std::string getType() const override {
        return "Enemy";
    }

    std::string serialize() const override {
        return getType() + " " + name + " " + std::to_string(health) + " " +
            std::to_string(attack) + " " + std::to_string(defense) + " " + type;
    }
};

// Шаблонный класс GameManager
template <typename T>
class GameManager {
private:
    std::vector<T> entities;
    mutable std::mutex mutex; // Mutable to allow locking in const methods

public:
    void addEntity(T&& entity) {
        std::lock_guard<std::mutex> lock(mutex);
        entities.push_back(std::move(entity));
    }

    void displayAll() const {
        std::lock_guard<std::mutex> lock(mutex);
        if (entities.empty()) {
            std::cout << "No entities in the game.\n";
            return;
        }
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    bool getFirstEnemy(std::unique_ptr<Entity>& outEnemy) {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if ((*it)->getType() == "Enemy") {
                outEnemy = std::move(*it);
                entities.erase(it);
                return true;
            }
        }
        return false;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        entities.clear();
    }
};

// Глобальные переменные
bool gameRunning = true;
std::mutex coutMutex;

// Функция генерации монстров
void generateMonsters(GameManager<std::unique_ptr<Entity>>& manager) {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::string name = "Goblin_" + std::to_string(rand() % 1000);
        manager.addEntity(std::make_unique<Enemy>(name, 50, 15, 5, "Goblin"));
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "New monster generated: " << name << "\n";
        }
    }
}

// Функция имитации боя
void combat(std::unique_ptr<Entity> player, std::unique_ptr<Entity> enemy) {
    if (!player || !enemy || player->getType() != "Player" || enemy->getType() != "Enemy") {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Invalid combat participants.\n";
        return;
    }

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\nCombat started between " << player->getName() << " and " << enemy->getName() << "!\n";
    }

    while (player->getHealth() > 0 && enemy->getHealth() > 0) {
        // Игрок атакует врага
        int damageToEnemy = player->getAttack() - enemy->getDefense();
        enemy->takeDamage(damageToEnemy);
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << player->getName() << " attacks " << enemy->getName() << " for "
                << (damageToEnemy > 0 ? damageToEnemy : 0) << " damage. "
                << enemy->getName() << " HP: " << enemy->getHealth() << "\n";
        }

        // Враг атакует игрока (если ещё жив)
        if (enemy->getHealth() > 0) {
            int damageToPlayer = enemy->getAttack() - player->getDefense();
            player->takeDamage(damageToPlayer);
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << enemy->getName() << " attacks " << player->getName() << " for "
                    << (damageToPlayer > 0 ? damageToPlayer : 0) << " damage. "
                    << player->getName() << " HP: " << player->getHealth() << "\n";
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Результат боя
    std::lock_guard<std::mutex> lock(coutMutex);
    if (player->getHealth() <= 0) {
        std::cout << player->getName() << " has been defeated!\n";
        gameRunning = false;
    }
    else {
        std::cout << enemy->getName() << " has been defeated!\n";
    }
}

// main
int main() {
    srand(static_cast<unsigned>(time(0)));

    try {
        GameManager<std::unique_ptr<Entity>> manager;

        // Создаем игрока
        auto hero = std::make_unique<Player>("Hero", 100, 20, 10, 15);
        manager.addEntity(std::move(hero));

        // Запускаем поток генерации монстров
        std::thread monsterGenerator(generateMonsters, std::ref(manager));

        // Основной игровой цикл
        while (gameRunning) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_ptr<Entity> enemy;
            if (manager.getFirstEnemy(enemy)) {
                // Создаем нового игрока для боя
                auto player = std::make_unique<Player>("Hero", 100, 20, 10, 15);
                {
                    std::lock_guard<std::mutex> lock(coutMutex);
                    std::cout << "\nStarting combat with " << enemy->getName() << "!\n";
                }

                // Запускаем поток боя
                std::thread combatThread(combat, std::move(player), std::move(enemy));
                combatThread.join();

                // Выводим состояние
                {
                    std::lock_guard<std::mutex> lock(coutMutex);
                    std::cout << "\nCurrent game state:\n";
                    manager.displayAll();
                }
            }
        }

        // Очистка
        monsterGenerator.detach();
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Game over.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\nException: " << e.what() << std::endl;
    }

    return 0;
}