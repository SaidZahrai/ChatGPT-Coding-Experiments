#include "machine_base.h"

// MachineBase class implementation

// Constructor: initializes 'name_' with the given argument
MachineBase::MachineBase(const std::string& name) : name_(name), current_state_(nullptr) {}

// Sets the current state to the state with the given name
// Returns 0 if successful, and -1 if the state is not found
int MachineBase::SetCurrentState(const std::string& state_name) {
  auto it = states_.find(state_name);
  if (it == states_.end()) {
    return -1;  // State not found
  }
  current_state_ = it->second.get();
  return 0;
}

// Executes the action of the current state and returns its result
int MachineBase::Execute() {
  if (current_state_ == nullptr) {
    return -1;  // No current state
  }
  return current_state_->Action();
}

// Adds a new state to the 'states' map
// If the state already exists, it returns -1 (error code)
// Otherwise, it adds the state and returns 0 (success code)
int MachineBase::AddState(std::shared_ptr<StateBase> state) {
  const std::string& state_name = state->get_name();
  auto result = states_.emplace(state_name, state);
  if (!result.second) {
    // State with the same name already exists
    return -1;
  }
  return 0;
}
