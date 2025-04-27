#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <sstream>

// Base class User
class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& n, int i, int al)
        : name(n), id(i), accessLevel(al) {
        validate();
    }

    virtual ~User() = default;

    // Getters
    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    // Setters
    void setName(const std::string& n) {
        if (n.empty()) {
            throw std::invalid_argument("Name cannot be empty.");
        }
        name = n;
    }

    void setId(int i) {
        if (i <= 0) {
            throw std::invalid_argument("ID must be positive.");
        }
        id = i;
    }

    void setAccessLevel(int al) {
        if (al < 0) {
            throw std::invalid_argument("Access level cannot be negative.");
        }
        accessLevel = al;
    }

    // Virtual method for polymorphism
    virtual void displayInfo() const {
        std::cout << "User: " << name << ", ID: " << id
            << ", Access Level: " << accessLevel << std::endl;
    }

    virtual std::string serialize() const {
        return "User," + name + "," + std::to_string(id) + "," + std::to_string(accessLevel);
    }

protected:
    void validate() const {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty.");
        }
        if (id <= 0) {
            throw std::invalid_argument("ID must be positive.");
        }
        if (accessLevel < 0) {
            throw std::invalid_argument("Access level cannot be negative.");
        }
    }
};

// Derived class Student
class Student : public User {
private:
    std::string group;

public:
    Student(const std::string& n, int i, int al, const std::string& g)
        : User(n, i, al), group(g) {
        if (group.empty()) {
            throw std::invalid_argument("Group cannot be empty.");
        }
    }

    void displayInfo() const override {
        std::cout << "Student: " << name << ", ID: " << id
            << ", Access Level: " << accessLevel << ", Group: " << group << std::endl;
    }

    std::string serialize() const override {
        return "Student," + name + "," + std::to_string(id) + "," +
            std::to_string(accessLevel) + "," + group;
    }
};

// Derived class Teacher
class Teacher : public User {
private:
    std::string department;

public:
    Teacher(const std::string& n, int i, int al, const std::string& d)
        : User(n, i, al), department(d) {
        if (department.empty()) {
            throw std::invalid_argument("Department cannot be empty.");
        }
    }

    void displayInfo() const override {
        std::cout << "Teacher: " << name << ", ID: " << id
            << ", Access Level: " << accessLevel << ", Department: " << department << std::endl;
    }

    std::string serialize() const override {
        return "Teacher," + name + "," + std::to_string(id) + "," +
            std::to_string(accessLevel) + "," + department;
    }
};

// Derived class Administrator
class Administrator : public User {
private:
    std::string role;

public:
    Administrator(const std::string& n, int i, int al, const std::string& r)
        : User(n, i, al), role(r) {
        if (role.empty()) {
            throw std::invalid_argument("Role cannot be empty.");
        }
    }

    void displayInfo() const override {
        std::cout << "Administrator: " << name << ", ID: " << id
            << ", Access Level: " << accessLevel << ", Role: " << role << std::endl;
    }

    std::string serialize() const override {
        return "Administrator," + name + "," + std::to_string(id) + "," +
            std::to_string(accessLevel) + "," + role;
    }
};

// Resource class
class Resource {
private:
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& n, int ral)
        : name(n), requiredAccessLevel(ral) {
        if (name.empty()) {
            throw std::invalid_argument("Resource name cannot be empty.");
        }
        if (requiredAccessLevel < 0) {
            throw std::invalid_argument("Required access level cannot be negative.");
        }
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void displayInfo() const {
        std::cout << "Resource: " << name << ", Required Access Level: " << requiredAccessLevel << std::endl;
    }

    std::string serialize() const {
        return name + "," + std::to_string(requiredAccessLevel);
    }

    std::string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }
};

// Template class AccessControlSystem
template <typename UserPtr, typename ResourcePtr>
class AccessControlSystem {
private:
    std::vector<UserPtr> users;
    std::vector<ResourcePtr> resources;

public:
    void addUser(UserPtr user) {
        if (!user) {
            throw std::invalid_argument("Cannot add null user.");
        }
        users.push_back(std::move(user));
    }

    void addResource(ResourcePtr resource) {
        if (!resource) {
            throw std::invalid_argument("Cannot add null resource.");
        }
        resources.push_back(std::move(resource));
    }

    void displayAllUsers() const {
        if (users.empty()) {
            std::cout << "No users in the system.\n";
            return;
        }
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void displayAllResources() const {
        if (resources.empty()) {
            std::cout << "No resources in the system.\n";
            return;
        }
        for (const auto& resource : resources) {
            resource->displayInfo();
        }
    }

    bool checkAccess(const User& user, const std::string& resourceName) const {
        for (const auto& resource : resources) {
            if (resource->getName() == resourceName) {
                bool access = resource->checkAccess(user);
                std::cout << user.getName() << (access ? " has access to " : " does not have access to ")
                    << resourceName << ".\n";
                return access;
            }
        }
        throw std::invalid_argument("Resource " + resourceName + " not found.");
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
        file << users.size() << "\n";
        for (const auto& user : users) {
            file << user->serialize() << "\n";
        }
        file << resources.size() << "\n";
        for (const auto& resource : resources) {
            file << resource->serialize() << "\n";
        }
        std::cout << "Data saved to " << filename << ".\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }

        users.clear();
        resources.clear();

        size_t userCount;
        file >> userCount;
        file.ignore(); // Consume newline
        for (size_t i = 0; i < userCount; ++i) {
            std::string line;
            std::getline(file, line);
            std::istringstream iss(line);
            std::string type, name, extra;
            int id, accessLevel;
            char comma;
            std::getline(iss, type, ',');
            std::getline(iss, name, ',');
            iss >> id >> comma >> accessLevel;
            std::getline(iss, extra); // Consume comma and get extra field
            extra = extra.substr(1);   // Remove leading comma
            try {
                if (type == "Student") {
                    users.push_back(std::make_unique<Student>(name, id, accessLevel, extra));
                }
                else if (type == "Teacher") {
                    users.push_back(std::make_unique<Teacher>(name, id, accessLevel, extra));
                }
                else if (type == "Administrator") {
                    users.push_back(std::make_unique<Administrator>(name, id, accessLevel, extra));
                }
                else {
                    std::cerr << "Warning: Unknown user type: " << type << "\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Warning: Failed to load user: " << e.what() << "\n";
            }
        }

        size_t resourceCount;
        file >> resourceCount;
        file.ignore();
        for (size_t i = 0; i < resourceCount; ++i) {
            std::string line;
            std::getline(file, line);
            std::istringstream iss(line);
            std::string name;
            int requiredAccessLevel;
            char comma;
            std::getline(iss, name, ',');
            iss >> requiredAccessLevel;
            try {
                resources.push_back(std::make_unique<Resource>(name, requiredAccessLevel));
            }
            catch (const std::exception& e) {
                std::cerr << "Warning: Failed to load resource: " << e.what() << "\n";
            }
        }
        std::cout << "Data loaded from " << filename << ".\n";
    }

    User* findUserByName(const std::string& name) const {
        for (const auto& user : users) {
            if (user->getName() == name) {
                return user.get();
            }
        }
        throw std::invalid_argument("User " + name + " not found.");
    }

    User* findUserById(int id) const {
        for (const auto& user : users) {
            if (user->getId() == id) {
                return user.get();
            }
        }
        throw std::invalid_argument("User with ID " + std::to_string(id) + " not found.");
    }

    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
        std::cout << "Users sorted by access level.\n";
    }
};

// main
int main() {
    try {
        AccessControlSystem<std::unique_ptr<User>, std::unique_ptr<Resource>> system;

        // Add Daniil Kuviko as Administrator (highest access)
        system.addUser(std::make_unique<Administrator>("Kuviko Daniil Igorevich", 3001, 5, "System Admin"));

        // Add groupmates as Students (access level 1)
        system.addUser(std::make_unique<Student>("Barykina Marta Dmitrievna", 1001, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Benesh Timur Dmitrievich", 1002, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Bocharova Ekaterina Sergeevna", 1003, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Vershanskiy Vladislav Nikolaevich", 1004, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Voronin Konstantin Yurievich", 1005, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Dzoblaev Alan Sergeevich", 1006, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Dundukov Nikita Alexandrovich", 1007, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Zubarev Bogdan Pavlovich", 1008, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Karanda Yaroslav Evgenievich", 1009, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Karuna Vadim Maksimovich", 1010, 1, "GD204"));
        //system.addUser(std::make_unique<Student>("Kuviko Daniil Igorevich", 1011, 5, "GD204"));
        system.addUser(std::make_unique<Student>("Madiy Dmitriy Alexandrovich", 1012, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Semkin Dmitriy Pavlovich", 1013, 1, "GD204"));
        system.addUser(std::make_unique<Student>("Teran Nikita Yurievich", 1014, 1, "GD204"));

        // Add fictional teachers (access level 3)
        system.addUser(std::make_unique<Teacher>("Olga Viktorovna Shostik", 2001, 3, "Game Development"));
        system.addUser(std::make_unique<Teacher>("Maxim Vasilyevich Tashilin", 2002, 3, "Mathematics"));

        // Add resources
        system.addResource(std::make_unique<Resource>("Classroom A", 1));
        system.addResource(std::make_unique<Resource>("Lab B", 3));
        system.addResource(std::make_unique<Resource>("Library", 2));

        // Display all users and resources
        std::cout << "\nAll Users:\n";
        system.displayAllUsers();
        std::cout << "\nAll Resources:\n";
        system.displayAllResources();

        // Check access
        std::cout << "\nChecking Access:\n";
        system.checkAccess(*system.findUserByName("Kuviko Daniil Igorevich"), "Lab B");         // Should pass (access level 5)
        system.checkAccess(*system.findUserByName("Barykina Marta Dmitrievna"), "Classroom A"); // Should pass (access level 1)
        system.checkAccess(*system.findUserByName("Barykina Marta Dmitrievna"), "Lab B");       // Should fail (access level 1)
        system.checkAccess(*system.findUserByName("Olga Viktorovna Shostik"), "Lab B");         // Should pass (access level 3)
        system.checkAccess(*system.findUserByName("Maxim Vasilyevich Tashilin"), "Library");  // Should pass (access level 3)

        // Save to file
        system.saveToFile("access_control.txt");

        // Load from file
        AccessControlSystem<std::unique_ptr<User>, std::unique_ptr<Resource>> newSystem;
        newSystem.loadFromFile("access_control.txt");
        std::cout << "\nLoaded Users:\n";
        newSystem.displayAllUsers();

        // Search users
        std::cout << "\nSearching Users:\n";
        User* user = newSystem.findUserByName("Kuviko Daniil Igorevich");
        user->displayInfo();
        user = newSystem.findUserById(1001);
        user->displayInfo();

        // Sort users
        std::cout << "\nSorting Users by Access Level:\n";
        newSystem.sortUsersByAccessLevel();
        newSystem.displayAllUsers();

        // Test exception handling
        std::cout << "\nTesting Invalid Inputs:\n";
        try {
            system.addUser(std::make_unique<Student>("", 1015, 1, "GD204")); // Empty name
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        try {
            system.checkAccess(*system.findUserByName("Kuviko Daniil Igorevich"), "Nonexistent"); // Invalid resource
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}