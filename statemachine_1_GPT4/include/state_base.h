#ifndef STATE_BASE_H_
#define STATE_BASE_H_

#include <functional>
#include <map>
#include <memory>
#include <string>

// Forward declaration of MachineBase class
class MachineBase;

// StateBase class declaration
class StateBase {
 public:
  // Constructor
  StateBase(const std::string& name, MachineBase* machine_ptr);

  // Pure virtual function 'action' with no arguments, returns an integer error code
  virtual int Action() = 0;

  // Getter for 'name' property
  const std::string& get_name() const { return name_; }

  // Getter for 'machine_ptr' property
  MachineBase* get_machine() const { return machine_ptr_; }

  // Getter for 'transitions' property
  const std::map<std::string, std::function<int()>>& get_transitions() const {
    return transitions_;
  }

 protected:
  // Property 'name'
  std::string name_;

  // Property 'machine_ptr'
  MachineBase* machine_ptr_;

  // Property 'transitions'
  std::map<std::string, std::function<int()>> transitions_;

  // Private function 'add_transition'
  int AddTransition(const std::string& transition_name,
                    const std::function<int()>& transition_function);

 private:
  // Disallow copying and assignment
  StateBase(const StateBase&) = delete;
  StateBase& operator=(const StateBase&) = delete;
};

#endif  // STATE_BASE_H_
