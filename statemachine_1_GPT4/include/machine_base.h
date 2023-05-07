#ifndef MACHINE_BASE_H_
#define MACHINE_BASE_H_

#include <map>
#include <memory>
#include <string>

#include "state_base.h"  // Include the StateBase header file

// MachineBase class declaration
class MachineBase {
 public:
  // Constructor with 'name' argument
  explicit MachineBase(const std::string& name);

  // Getter for 'name' property
  const std::string& get_name() const { return name_; }

  // Getter for 'states' property
  const std::map<std::string, std::shared_ptr<StateBase>>& get_states() const {
    return states_;
  }

  // Getter for 'current_state' property
  StateBase* get_current_state() const { return current_state_; }

  // Public method 'set_current_state' with a string argument
  // Returns an integer error code
  int SetCurrentState(const std::string& state_name);

  // Public method 'execute' with no arguments
  // Returns an integer error code
  int Execute();

 protected:
  // Protected function 'add_state' with a pointer to a 'StateBase' argument
  int AddState(std::shared_ptr<StateBase> state);

 private:
  // Property 'name'
  std::string name_;

  // Property 'states'
  std::map<std::string, std::shared_ptr<StateBase>> states_;

  // Property 'current_state'
  StateBase* current_state_;

  // Disallow copying and assignment
  MachineBase(const MachineBase&) = delete;
  MachineBase& operator=(const MachineBase&) = delete;
};

#endif  // MACHINE_BASE_H_
