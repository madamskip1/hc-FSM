#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace
{
    struct StateA {};
    struct StateB {};
    struct StateC {};

    struct EventA {};
    struct EventB {};

    TEST(SimpleTransitionsTests, shouldTransitState)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(SimpleTransitionsTests, shouldNotTransitState)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventB>();

        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::NO_VALID_TRANSITION);
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
    }

    TEST(SimpleTransitionsTests, shouldTransitInPlace_SameState)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateA>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED_SAME_STATE);
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
    }

    TEST(SimpleTransitionsTests, shouldAutomaticallyTransitState)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transition2 = hcFSM::TransitionAutomatic<StateB, StateC>; // or Transition<StateB, AUTOMATIC_TRANSITION, StateC>
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        auto hadleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(hadleEventResult, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateC>(), true);
    }

    TEST(SimpleTransitionsTests, shouldAutomicallyTransitiFromInitialState)
    {
        using transition = hcFSM::TransitionAutomatic<StateA, StateB>; // or Transition<StateA, AUTOMATIC_TRANSITION, StateB>
        using transitions_table = hcFSM::TransitionsTable<
            transition
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }
}