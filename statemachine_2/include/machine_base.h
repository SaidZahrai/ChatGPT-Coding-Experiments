#ifndef MACHINE_BASE_H_
#define MACHINE_BASE_H_

#include <string>
#include <map>
#include "state_base.h"

// MachineBase class
class MachineBase {
 public:
  // Constructor
  MachineBase(std::string name);

  // Getters
  std::string GetName();
  StateBase* GetCurrentState();
  int SetCurrentState(std::string state_name);

  // Public function for executing the machine
  int Execute();

 protected:
  // Protected member variables
  std::string name_;
  StateBase* current_state_;
  int AddState(StateBase* new_state);

 private:
  // Private member variables
  std::map<std::string, StateBase*> states_;
};

#endif  // MACHINE_BASE_H_
