#include <gtest/gtest.h>
#include "transition.h"
#include "transitions-table.h"
#include "finite-state-machine.h"

namespace FSM
{
    struct StateA {};
    struct StateB {};

    struct EventA {};
    struct EventB {};

    TEST(SimpleTransitionsTests, shouldTransitState)
    {
        using transition = Transition<StateA, EventA, StateB>;
        using transitions_table = TransitionsTable<transition>;

        auto stateMachine = StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(handleEventResult, HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(SimpleTransitionsTests, shouldNotTransitState)
    {
        using transition = Transition<StateA, EventA, StateB>;
        using transitions_table = TransitionsTable<transition>;

        auto stateMachine = StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(handleEventResult, HandleEventResult::NO_VALID_TRANSITION);
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
    }

    TEST(SimpleTransitionsTests, shouldTransitInPlace_SameState)
    {
        using transition = Transition<StateA, EventA, StateA>;
        using transitions_table = TransitionsTable<transition>;

        auto stateMachine = StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(handleEventResult, HandleEventResult::PROCESSED_SAME_STATE);
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
    }
}