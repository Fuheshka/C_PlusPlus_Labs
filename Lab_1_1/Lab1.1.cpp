#include <iostream>
#include <string>

class Character {
private:
    std::string name; // Приватное поле: имя персонажа
    int health;       // Приватное поле: уровень здоровья
    int attack;       // Приватное поле: уровень атаки
    int defense;      // Приватное поле: уровень защиты

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) { // Исправлено: передача по ссылке
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Задание 1: Метод для лечения персонажа
    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100; // Максимальное здоровье ограничено 100
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }

    // Задание 2: Метод для получения урона
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0; // Здоровье не опускается ниже 0
        std::cout << name << " takes " << amount << " damage!" << std::endl;
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим начальную информацию о персонажах
    std::cout << "Initial state:" << std::endl;
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Тестируем метод takeDamage
    hero.takeDamage(30);
    hero.displayInfo();

    // Тестируем метод heal
    hero.heal(50);
    hero.displayInfo();

    // Проверяем ограничение максимального здоровья
    hero.heal(60); // Попробуем вылечить больше, чем максимум
    hero.displayInfo();

    // Проверяем ограничение минимального здоровья
    monster.takeDamage(70); // Урон больше текущего здоровья
    monster.displayInfo();

    return 0;
}