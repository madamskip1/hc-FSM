#pragma once

#include <iostream>
#include <hcFSM/hcFSM.h>

CREATE_TRANSITION_ACTION(SpawnedStartWandering, SourceState, Event, TargetState, {
    std::cout << "I've spawned and going to start wandering." << std::endl;
})

CREATE_TRANSITION_ACTION(LowHealthTryEscape, SourceState, Event, TargetState, {
    std::cout << "My health felt too low. I have to escape. RUNNNNNN!" << std::endl;
})