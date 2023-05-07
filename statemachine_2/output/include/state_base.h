// state_base.h
// C++17
// Google coding style

#ifndef STATE_BASE_H_
#define STATE_BASE_H_

#include <string>
#include <map>
#include <functional>

// Forward declaration
class MachineBase;

class StateBase {
 public:
  // Getters for non-const 'name', 'machine ptr' and 'transitions'
  std::string& name();
  MachineBase* machine_ptr();
  std::map<std::string, std::function<int()>>& transitions();

  // Constructor
  StateBase(std::string name, MachineBase* machine_ptr);

  // Pure virtual function
  virtual int action() = 0;

 protected:
  // Properties
  std::string name_;
  MachineBase* machine_ptr_;
  std::map<std::string, std::function<int()>> transitions_;

 private:
  // Function to add transition
  int add_transition(std::string name, std::function<int()> fcn);
};

#endif  // STATE_BASE_H_