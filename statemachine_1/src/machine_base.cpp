#include "machine_base.h"

// Constructor that stores the argument 'name' in the protected variable called 'name_'
MachineBase::MachineBase(std::string name)
    : name_(name), current_state_(nullptr) {}

// Virtual destructor
MachineBase::~MachineBase() {}

// Getter function for 'name_'
std::string MachineBase::get_name() const {
    return name_;
}

// Getter function for 'states_'
std::map<std::string, StateBase*> MachineBase::get_states() const {
    return states_;
}

// Getter function for 'current_state_'
StateBase* MachineBase::get_current_state() const {
    return current_state_;
}

// Public method that sets the current state to the state with the given name
int MachineBase::set_current_state(std::string state_name) {
    auto it = states_.find(state_name);
    if (it != states_.end()) { // found the state
        current_state_ = it->second;
        return 0; // return 0 on success
    }
    else { // state not found
        return -1; // return -1 on failure
    }
}

// Public function for executing the machine
int MachineBase::execute() {
    if (current_state_ != nullptr) { // current state exists
        return current_state_->action(); // call the action function of the current state
    }
    else { // current state does not exist
        return -1; // return -1 on failure
    }
}

// Protected function that adds a state to the 'states_' map
int MachineBase::add_state(StateBase* state) {
    std::string name = state->get_name();
    auto it = states_.find(name);
    if (it == states_.end()) { // state not found
        states_.insert(std::make_pair(name, state));
        return 0; // return 0 on success
    }
    else { // state already exists
        return -1; // return -1 on failure
    }
}
