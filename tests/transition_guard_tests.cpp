#include <gtest/gtest.h>
#include "hcFSM/detail/transition-guard.h"
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace hcFSM
{
    struct StateA 
    {
        int value = 0;
    };
    struct StateB {};
    struct EventA {};
    struct EventB {};

    CREATE_TRANSITION_GUARD(CreateGuard_test_guard, currentState, event, 
        { 
            return true;
        }
    );

    TEST(TransitionGuardTests, CreateGuard)
    {
        StateA stateA;
        EventA eventA;
        EXPECT_EQ(CreateGuard_test_guard {} (stateA, eventA), true);
    }

    CREATE_TRANSITION_GUARD(GuardWithState_test_guard, currentState, event,
        {
            return currentState.value == 1 ? true : false;
        }
    );

    TEST(TransitionGuardTests, GuardWithState)
    {
        StateA stateA;
        EventA eventA;

        EXPECT_EQ(GuardWithState_test_guard {} (stateA, eventA), false);

        stateA.value = 1;
        EXPECT_EQ(GuardWithState_test_guard {} (stateA, eventA), true);
    }

    TEST(TransitionGuardTests, hasGuard_withGeneratedGuard)
    {
        using transition = Transition<StateA, EventA, StateB, void, CreateGuard_test_guard>;
        ASSERT_EQ(hasGuard_v<transition>, true);
    }

    CREATE_TRANSITION_GUARD(FalseGuard_test_guard, currentState, event,
        {
            return false;
        }
    );

    TEST(TransitionGuardTests, TransitionWithGuard)
    {
        using transition1 = Transition<StateA, EventA, StateB, void, FalseGuard_test_guard>;
        using transition2 = Transition<StateA, EventB, StateB, void, CreateGuard_test_guard>;
        using transitions_table = TransitionsTable<transition1, transition2>;
        ASSERT_EQ(hasGuard_v<transition1>, true);
        ASSERT_EQ(hasGuard_v<transition2>, true);

        auto stateMachine = StateMachine<transitions_table>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        auto handleEventResult1 = stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(handleEventResult1, HandleEventResult::GUARD_FAILED);

        auto handleEventResult2 = stateMachine.handleEvent(EventB{});
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult2, HandleEventResult::PROCESSED);
    }
}