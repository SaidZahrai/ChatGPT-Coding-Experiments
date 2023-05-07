#include "state_base.h"
#include "machine_base.h"

// Constructor
StateBase::StateBase(std::string name, MachineBase* machine_ptr)
    : name_(name), machine_ptr_(machine_ptr) {}

// Private function for adding a transition
int StateBase::AddTransition(std::string name, int (*fcn)()) {
  // Check if a transition with the same name already exists
  if (transitions_.count(name) != 0) {
    return -1;
  }
  // Add the new transition
  transitions_.emplace(name, fcn);
  return 0;
}

// Getters
std::string StateBase::GetName() {
  return name_;
}

MachineBase* StateBase::GetMachinePtr() {
  return machine_ptr_;
}

std::map<std::string, int (*)()> StateBase::GetTransitions() {
  return transitions_;
}
