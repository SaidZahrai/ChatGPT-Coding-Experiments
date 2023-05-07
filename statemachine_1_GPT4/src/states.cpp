#include "states.h"

#include <iostream>

// StateA class implementation

// Constructor: initializes the StateBase with the name "A" and the given MachineBase pointer
StateA::StateA(MachineBase* machine_ptr) : StateBase("A", machine_ptr) {}

// Action function: prints the state name and sets the current state of the machine to "B"
int StateA::Action() {
  std::cout << "State A" << std::endl;
  machine_ptr_->SetCurrentState("B");
  return 0;
}

// StateB class implementation

// Constructor: initializes the StateBase with the name "B" and the given MachineBase pointer
StateB::StateB(MachineBase* machine_ptr) : StateBase("B", machine_ptr) {}

// Action function: prints the state name and sets the current state of the machine to "C"
int StateB::Action() {
  std::cout << "State B" << std::endl;
  machine_ptr_->SetCurrentState("C");
  return 0;
}

// StateC class implementation

// Constructor: initializes the StateBase with the name "C" and the given MachineBase pointer
StateC::StateC(MachineBase* machine_ptr) : StateBase("C", machine_ptr) {}

// Action function: prints the state name and sets the current state of the machine to "A"
int StateC::Action() {
  std::cout << "State C" << std::endl;
  machine_ptr_->SetCurrentState("A");
  return 0;
}
