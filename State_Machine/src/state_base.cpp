#include "state_base.h"

// StateBase class implementation

// Constructor: initializes 'name_' and 'machine_ptr_' with the given arguments
StateBase::StateBase(const std::string& name, MachineBase* machine_ptr)
    : name_(name), machine_ptr_(machine_ptr) {}

// Adds a new transition to the 'transitions_' map
// If the transition already exists, it returns -1 (error code)
// Otherwise, it adds the transition and returns 0 (success code)
int StateBase::AddTransition(const std::string& transition_name,
                             const std::function<int()>& transition_function) {
  // Check if the transition already exists
  if (transitions_.find(transition_name) != transitions_.end()) {
    return -1;
  }

  // Add the new transition to the map
  transitions_[transition_name] = transition_function;
  return 0;
}
