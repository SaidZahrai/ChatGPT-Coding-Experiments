#ifndef DERIVED_STATES_H_
#define DERIVED_STATES_H_

#include "state_base.h"
#include "machine_base.h"

// StateA class
class StateA : public StateBase {
 public:
  // Constructor
  StateA(MachineBase* machine_ptr);

  // Override the Action function
  int Action() override;

 private:
  // Private member variables
  int count_ = 0;
};

// StateB class
class StateB : public StateBase {
 public:
  // Constructor
  StateB(MachineBase* machine_ptr);

  // Override the Action function
  int Action() override;

 private:
  // Private member variables
  int value_ = 0;
};

// StateC class
class StateC : public StateBase {
 public:
  // Constructor
  StateC(MachineBase* machine_ptr);

  // Override the Action function
  int Action() override;

 private:
  // Private member variables
  std::string message_ = "Hello, world!";
};

#endif  // DERIVED_STATES_H_
