#include "machine_base.h"
#include "derived_states.h"
#include <iostream>

// StateA constructor
StateA::StateA(MachineBase* machine_ptr)
    : StateBase("A", machine_ptr) {}

// StateA action function
int StateA::Action() {
  // Print the name of the state to the console
  std::cout << "StateA\n";
  // Set the current state of the machine to 'B'
  GetMachinePtr()->SetCurrentState("B");
  return 0;
}

// StateB constructor
StateB::StateB(MachineBase* machine_ptr)
    : StateBase("B", machine_ptr) {}

// StateB action function
int StateB::Action() {
  // Print the name of the state to the console
  std::cout << "StateB\n";
  // Set the current state of the machine to 'C'
  GetMachinePtr()->SetCurrentState("C");
  return 0;
}

// StateC constructor
StateC::StateC(MachineBase* machine_ptr)
    : StateBase("C", machine_ptr) {}

// StateC action function
int StateC::Action() {
  // Print the name of the state to the console
  std::cout << "StateC\n";
  // Set the current state of the machine to 'A'
  GetMachinePtr()->SetCurrentState("A");
  return 0;
}
