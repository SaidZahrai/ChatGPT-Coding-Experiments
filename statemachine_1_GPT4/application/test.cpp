#include <iostream>

#include "machine_one.h"
#include "states.h"

int main() {
  // Instantiate the StateMachine with StateA, StateB, and StateC
  MachineOne<StateA, StateB, StateC> sm;

  // Set the current state to "A"
  if (sm.SetCurrentState("A") != 0) {
    std::cerr << "Error: Failed to set the initial state to A" << std::endl;
    return 1;
  }

  // Call the 'execute' function 7 times
  for (int i = 0; i < 7; ++i) {
    if (sm.Execute() != 0) {
      std::cerr << "Error: Failed to execute action in iteration " << i + 1 << std::endl;
      return 1;
    }
  }

  return 0;
}
