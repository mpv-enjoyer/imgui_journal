#include "attendance_wdays.h"

#include <iostream>
#include <utility> // for std::move
#include <string>

class LoggedClass {
private:
    std::string name;
    int value;
public:
    // Default constructor
    LoggedClass() : name("Default"), value(0) {
        std::cout << "Default constructor called\n";
    }

    // Parameterized constructor
    LoggedClass(const std::string& n, int v) : name(n), value(v) {
        std::cout << "Parameterized constructor called (" << name << ", " << value << ")\n";
    }

    // Copy constructor
    LoggedClass(const LoggedClass& other) : name(other.name), value(other.value) {
        std::cout << "Copy constructor called (from " << other.name << ")\n";
    }

    // Move constructor
    LoggedClass(LoggedClass&& other) noexcept 
        : name(std::move(other.name)), value(std::exchange(other.value, 0)) {
        std::cout << "Move constructor called (from " << name << ")\n";
    }

    // Copy assignment operator
    LoggedClass& operator=(const LoggedClass& other) {
        if (this != &other) {
            name = other.name;
            value = other.value;
            std::cout << "Copy assignment called (from " << other.name << ")\n";
        }
        return *this;
    }

    // Move assignment operator
    LoggedClass& operator=(LoggedClass&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            value = std::exchange(other.value, 0);
            std::cout << "Move assignment called (from " << name << ")\n";
        }
        return *this;
    }

    // Destructor
    ~LoggedClass() {
        std::cout << "Destructor called for " << name << "\n";
    }

    // Additional member functions
    void print() const {
        std::cout << "LoggedClass: " << name << ", " << value << "\n";
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    bool operator< (const decltype(*this)& other) { return std::tie(name, value) <  std::tie(other.name, other.value); }
    bool operator==(const LoggedClass& other) { return std::tie(name, value) == std::tie(other.name, other.value); }
};

int main()
{
    LoggedClass l1("1231", 2);
    LoggedClass l2("1231", 3);
    std::cout << (l1 < l2) << "\n";
}