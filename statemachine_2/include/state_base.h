#ifndef STATE_BASE_H_
#define STATE_BASE_H_

#include <string>
#include <map>

// Forward declaration to class MachineBase
class MachineBase;

// StateBase class
class StateBase {
 public:
  // Getters for non-const name, machine ptr and transitions
  std::string GetName();
  MachineBase* GetMachinePtr();
  std::map<std::string, int (*)()> GetTransitions();

  // Constructor
  StateBase(std::string name, MachineBase* machine_ptr);

  // Pure virtual function for action
  virtual int Action() = 0;

 protected:
  // Member variables
  std::string name_;
  MachineBase* machine_ptr_;
  std::map<std::string, int (*)()> transitions_;

 private:
  // Private function for adding a transition
  int AddTransition(std::string name, int (*fcn)());
};

#endif  // STATE_BASE_H_
