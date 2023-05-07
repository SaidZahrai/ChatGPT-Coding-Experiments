#include "state_base.h"
#include "machine_base.h" // include necessary header files

// Constructor that sets the protected properties 'name_' and 'machine_ptr_'
StateBase::StateBase(std::string name, MachineBase* machine_ptr)
    : name_(name), machine_ptr_(machine_ptr) {}

// Private function that adds a new transition to 'transitions_'
int StateBase::add_transition(std::string name, std::function<int()> func) {
    transitions_.insert(std::make_pair(name, func));
    return 0;
}

// Getter function for 'name_'
std::string StateBase::get_name() const {
    return name_;
}

// Getter function for 'machine_ptr_'
MachineBase* StateBase::get_machine() const {
    return machine_ptr_;
}

// Getter function for 'transitions_'
std::map<std::string, std::function<int()>> StateBase::get_transitions() const {
    return transitions_;
}

// Pure virtual function that will be implemented by derived classes
int StateBase::action() {
    return 0;
}
