#ifndef STATE_BASE_H_
#define STATE_BASE_H_

#include <string>
#include <map>
#include <functional>

class MachineBase; // forward declaration

class StateBase {
protected:
    std::string name_; // protected property 'name'
    MachineBase* machine_ptr_; // protected property 'machine_ptr'

    // protected map list called 'transitions'
    std::map<std::string, std::function<int()>> transitions_;

public:
    StateBase(std::string name, MachineBase* machine_ptr)
        : name_(name), machine_ptr_(machine_ptr) {}

    std::string get_name() const { return name_; } // getter function for 'name_'
    MachineBase* get_machine() const { return machine_ptr_; } // getter function for 'machine_ptr_'

    virtual int action() = 0; // pure virtual function called 'action'

    std::map<std::string, std::function<int()>> get_transitions() const { return transitions_; } // getter function for 'transitions_'

protected:
    int add_transition(std::string name, std::function<int()> func) {
        transitions_.insert(std::make_pair(name, func)); // add a new transition to 'transitions_'
        return 0; // return 0 on success
    }
};

#endif // STATE_BASE_H_
