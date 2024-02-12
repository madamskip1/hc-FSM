#include <iostream>
#include <hcFSM/hcFSM.h>

CREATE_TRANSITION_GUARD(CreateGuard_test_guard, currentState, event, 
        { 
            return true;
        }
    );
struct stateA{};
struct stateB{};

int main()
{
    bool statea = true;
    bool stateb = false;
    std::cout << "Hello, World!" << CreateGuard_test_guard{}(stateA{}, stateB{}) << std::endl;
}