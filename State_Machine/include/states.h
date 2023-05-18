#ifndef STATES_H_
#define STATES_H_

#include "machine_base.h"  // Include the MachineBase header file
#include "state_base.h"    // Include the StateBase header file

// StateA class declaration, derived from StateBase
class StateA : public StateBase {
 public:
  // Constructor with a pointer to a MachineBase object as an argument
  explicit StateA(MachineBase* machine_ptr);

  // Implementation of the 'action' function from the base class
  int Action() override;
};

// StateB class declaration, derived from StateBase
class StateB : public StateBase {
 public:
  // Constructor with a pointer to a MachineBase object as an argument
  explicit StateB(MachineBase* machine_ptr);

  // Implementation of the 'action' function from the base class
  int Action() override;
};

// StateC class declaration, derived from StateBase
class StateC : public StateBase {
 public:
  // Constructor with a pointer to a MachineBase object as an argument
  explicit StateC(MachineBase* machine_ptr);

  // Implementation of the 'action' function from the base class
  int Action() override;
};

#endif  // STATES_H_
