#include <gtest/gtest.h>
#include "hcFSM/detail/state-machine.h"
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"

struct StateA {};
struct StateB {};
struct EventA {};
struct EventB {};

namespace
{
    TEST(StateMachineTests, InitialState_NotSetExplicity)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        auto stateMachine1 = hcFSM::StateMachine<transitions_table>{};

        EXPECT_EQ(stateMachine1.isInState<StateA>(), true);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), false);
    }

    TEST(StateMachineTests, InitialState_SetExplicity)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine2 = hcFSM::StateMachine<transitions_table, StateB>{};

        EXPECT_EQ(stateMachine2.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine2.isInState<StateB>(), true);
    }

    TEST(StateMachineTests, ForceTransition_nextState)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        auto stateMachine = hcFSM::StateMachine<transitions_table>{};

        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(stateMachine.isInState<StateB>(), false);

        stateMachine.forceTransition<StateB>();
        
        EXPECT_EQ(stateMachine.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(StateMachineTests, ForceTransition_sameState)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table>{};

        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(stateMachine.isInState<StateB>(), false);

        stateMachine.forceTransition<StateA>();

        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(stateMachine.isInState<StateB>(), false);
    }

    TEST(StateMachineTests, HandleEvent)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transition2 = hcFSM::Transition<StateB, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;

        auto stateMachine1 = hcFSM::StateMachine<transitions_table>{};

        auto handleEventResultTemplate1 = stateMachine1.handleEvent<EventA>();
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResultTemplate1, hcFSM::HandleEventResult::PROCESSED);

        auto handleEventResultTemplate2 = stateMachine1.handleEvent<EventA>();
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResultTemplate2, hcFSM::HandleEventResult::PROCESSED_SAME_STATE);

        auto handleEventResultTemplate3 = stateMachine1.handleEvent<EventB>();
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResultTemplate3, hcFSM::HandleEventResult::NO_VALID_TRANSITION);

        auto stateMachine2 = hcFSM::StateMachine<transitions_table>{};

        auto handleEventResult1 = stateMachine2.handleEvent(EventA{});
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult1, hcFSM::HandleEventResult::PROCESSED);

        auto handleEventResult2 = stateMachine2.handleEvent(EventA{});
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult2, hcFSM::HandleEventResult::PROCESSED_SAME_STATE);

        auto handleEventResult3 = stateMachine2.handleEvent(EventB{});
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult3, hcFSM::HandleEventResult::NO_VALID_TRANSITION);
    }
}