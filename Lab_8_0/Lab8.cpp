#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address; // New private field for address

public:
    // �������
    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const { // Getter for address
        return address;
    }

    // �������
    void setName(const std::string& newName) {
        if (!newName.empty()) {
            name = newName;
        }
        else {
            std::cerr << "Error: Name cannot be empty!" << std::endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            std::cerr << "Error: Age must be between 0 and 120!" << std::endl;
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos) {
            email = newEmail;
        }
        else {
            std::cerr << "Error: Invalid email format!" << std::endl;
        }
    }

    void setAddress(const std::string& newAddress) { // Setter for address
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            std::cerr << "Error: Address cannot be empty!" << std::endl;
        }
    }

    // ����� ��� ������ ���������� � ��������
    void displayInfo() const {
        std::cout << "Name: " << name
            << ", Age: " << age
            << ", Email: " << email
            << ", Address: " << address << std::endl;
    }
};

int main() {
    Person person;

    // ������������� �������� � ������� ��������
    person.setName("John Doe");
    person.setAge(25);
    person.setEmail("john.doe@example.com");
    person.setAddress("123 Main St, Springfield"); // Set valid address

    // ������� ���������� � ������� ��������
    std::cout << "Name: " << person.getName() << std::endl;
    std::cout << "Age: " << person.getAge() << std::endl;
    std::cout << "Email: " << person.getEmail() << std::endl;
    std::cout << "Address: " << person.getAddress() << std::endl;

    // �������� ���������� ������������ ��������
    person.setName(""); // ������: ��� �� ����� ���� ������
    person.setAge(150); // ������: ������� ������ ���� �� 0 �� 120
    person.setEmail("invalid-email"); // ������: ������������ email
    person.setAddress(""); // ������: ����� �� ����� ���� ������

    // ������� ���������� � ��������
    std::cout << "\nPerson Info:\n";
    person.displayInfo();

    return 0;
}