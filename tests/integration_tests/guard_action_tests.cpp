#include <gtest/gtest.h>
#include "transition-guard.h"
#include "transition.h"
#include "transitions-table.h"
#include "finite-state-machine.h"

namespace FSM
{
    CREATE_TRANSITION_GUARD(TransitionGuardPass, sourceState, eventParam, 
    {
        return true;
    });

    CREATE_TRANSITION_GUARD(TransitionGuardFail, sourceState, eventParam, 
    {
        return false;
    });

    struct EventA {};
    struct StateA {};
    struct StateB {};

    TEST(GuardActionTests, shouldAllowTransition)
    {
        using transition1 = TransitionWithGuard<StateA, EventA, StateB, TransitionGuardPass>; // or Transition<StateA, EventA, StateB, void, TransitionGuardPass>
        using transitions_table = TransitionsTable<
            transition1
        >;

        auto stateMachine = StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(GuardActionTests, shouldNotAllowTransition)
    {
        using transition1 = TransitionWithGuard<StateA, EventA, StateB, TransitionGuardFail>; // or Transition<StateA, EventA, StateB, void, TransitionGuardFail>
        using transitions_table = TransitionsTable<
            transition1
        >;

        auto stateMachine = StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(handleEventResult, HandleEventResult::GUARD_FAILED);
    }
}