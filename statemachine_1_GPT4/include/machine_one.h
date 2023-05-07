#ifndef MACHINE_ONE_H_
#define MACHINE_ONE_H_

#include <memory>
#include <type_traits>

#include "machine_base.h"  // Include the MachineBase header file

// Template class MachineOne declaration, derived from MachineBase
template <typename... States>
class MachineOne : public MachineBase {
 public:
  // Default constructor with no arguments
  MachineOne();

 private:
  // Helper function to add states to the machine
  template <typename State, typename... Rest>
  void AddStates();

  // Base case for the helper function
//  void AddStates() {}
};

// Include the implementation details for the template class
//#include "machine_one_impl.h"

// MachineOne template class implementation

// Default constructor: initializes the base class with the name "MachineOne"
// and adds all state classes from the template pack list
template <typename... States>
MachineOne<States...>::MachineOne() : MachineBase("MachineOne") {
  AddStates<States...>();
}

// Helper function to add states to the machine
template <typename... States>
template <typename State, typename... Rest>
void MachineOne<States...>::AddStates() {
  auto state_ptr = std::make_shared<State>(this);
  AddState(state_ptr);
  if constexpr (sizeof...(Rest) > 0)
      AddStates<Rest...>();
}

#endif  // MACHINE_ONE_H_
