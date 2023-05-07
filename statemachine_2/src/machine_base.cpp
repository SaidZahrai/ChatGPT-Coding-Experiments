#include "machine_base.h"

// Constructor
MachineBase::MachineBase(std::string name)
    : name_(name), current_state_(nullptr) {}

// Private function for adding a state
int MachineBase::AddState(StateBase* new_state) {
  // Check if a state with the same name already exists
  if (states_.count(new_state->GetName()) != 0) {
    return -1;
  }
  // Add the new state
  states_.emplace(new_state->GetName(), new_state);
  return 0;
}

// Public function for executing the machine
int MachineBase::Execute() {
  // Call the action of the current state
  return current_state_->Action();
}

// Setter for current state
int MachineBase::SetCurrentState(std::string state_name) {
  // Search for a state in 'states' with the name 'state_name'
  auto state_it = states_.find(state_name);
  if (state_it == states_.end()) {
    // If the state doesn't exist, return -1
    return -1;
  }
  // Otherwise, set the current state to the corresponding state
  current_state_ = state_it->second;
  return 0;
}

// Getters
std::string MachineBase::GetName() {
  return name_;
}

StateBase* MachineBase::GetCurrentState() {
  return current_state_;
}
