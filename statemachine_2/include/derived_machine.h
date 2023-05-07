#ifndef DERIVED_MACHINE_H_
#define DERIVED_MACHINE_H_

#include "machine_base.h"
#include "derived_states.h"

template<typename... States>
class MachineOne : public MachineBase {
 public:
  // Constructor
  MachineOne(std::string name);

 private:
  // Add all the states to the machine
  void AddStates();
};

// Include the implementation file for the template class
// #include "derived_machine.tpp"

#endif  // DERIVED_MACHINE_H_
