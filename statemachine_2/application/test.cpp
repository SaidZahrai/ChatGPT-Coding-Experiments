#include "derived_machine.h"

int main() {
  // Instantiate the derived class 'MachineOne' with the three classes StateA, StateB, and StateC
  MachineOne<StateA, StateB, StateC> sm("State Machine");

  // Set the current state to 'A'
  sm.SetCurrentState("A");

  // Execute the state machine 7 times
  for (int i = 0; i < 7; i++) {
    sm.Execute();
  }

  return 0;
}
