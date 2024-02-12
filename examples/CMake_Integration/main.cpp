#include <iostream>
#include <hcFSM/hcFSM.h>

struct StateA {};
struct StateB {};

struct EventA {};



int main()
{
    using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
    using transitions_table = hcFSM::TransitionsTable<
        transition1
    >;
    auto stateMachine = hcFSM::StateMachine<transitions_table>{};
    stateMachine.handleEvent<EventA>();

    std::cout << "Am I in StateB?: " << (stateMachine.isInState<StateB>() ? "Yes" : "No") << std::endl;
    return 0;
}