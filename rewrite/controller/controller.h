#pragma once
#include <optional>
#include <string>

class Controller
{
    class ModelCreatorThatYouMustNeverUse { };
public:
    class Model
    {
        // I didn't do a singleton Model to prevent
        // accidental calling of non-const methods from View.
    public:
        Model(ModelCreatorThatYouMustNeverUse) { };
    };
private:
    Model m_model = ModelCreatorThatYouMustNeverUse();
    static Controller* m_instance_ptr;
    Controller() { }
    Model& model() { return m_model; }
public:
    static Controller& controller()
    {
        if (!m_instance_ptr) m_instance_ptr = new Controller();
        return *m_instance_ptr;
    }
    Controller(const Controller& obj) = delete;
    const Model& model() const { return m_model; }

    // Interface for all actions:
    class ICommand
    {
        virtual void call() = 0;
        virtual std::optional<std::string> get_error();
    };
};