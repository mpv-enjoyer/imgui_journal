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

    AUTOOPS2(LoggedClass, name, value)
};

#include "model.h"

void look_into_model(const Model& model)
{
    std::cout << model->students()->cref_contracts().cbegin().get()->get_id();
    //model->students()->add("fhfds", 1);
}

int main()
{
    Model model;
    model->
        attendance_wdays()->
        ref_attendance_wday(Wday::make_begin_EN())->
        ref_merged_lessons().
        push_back(new Attendance_Merged_Lesson({new Attendance_Internal_Lesson(Attendance_Internal_Lesson::Type::SPECIALCOURSE, JTime(10, 40), JTime(11, 20))}));
    model->students()->add("Student Name", 741234);

    look_into_model(model);

    auto p = Ptr<int>::make();

    //Attendance_Wdays w;
    //auto& ref = w.ref_attendance_wday(Wday::make_begin_EN());
    //auto& merged = ref->ref_merged_lessons();
    //
    //auto internal_new = new Attendance_Internal_Lesson(Attendance_Internal_Lesson::Type::DRAWING, JTime(10, 40), JTime(11, 20));
    //auto merged_new = new Attendance_Merged_Lesson({internal_new});
    //merged.push_back(merged_new);
    
}