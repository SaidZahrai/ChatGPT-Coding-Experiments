#include "derived_machine.h"

// Constructor
template<typename... States>
MachineOne<States...>::MachineOne(std::string name)
    : MachineBase(name) {
  // Add all the states to the machine
  AddStates();
}

// Private function to add all the states to the machine
template<typename... States>
void MachineOne<States...>::AddStates() {
  // Create a new object of each state and add it to the machine
  (void)std::initializer_list<int>{(
      AddState(new States(this)) , 0)...
  };
}

// Explicit instantiation of the template class
template class MachineOne<StateA, StateB, StateC>;
