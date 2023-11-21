#include <gtest/gtest.h>
#include "transition.h"
#include "transitions-table.h"
#include "finite-state-machine.h"

namespace FSM
{
    struct StateA {};
    struct StateB {};
    struct StateC {};
    struct StateD {};
    struct EventA {};
    struct EventB {};


    TEST(AutomaticTransitionTests, NextState_AutomaticTransition)
    {
        using transition1 = Transition<StateA, EventB, StateB>;
        using transition2 = Transition<StateB, AUTOMATIC_TRANSITION, StateC>;
        using transitions_table = TransitionsTable<
            transition1,
            transition2
        >;
        auto hasAutomaticTransitionFromStateA = hasAutomaticTransition<transitions_table, StateA>::value;
        auto hasAutomaticTransitionFromStateA_v = hasAutomaticTransition_v<transitions_table, StateA>;
        EXPECT_EQ(hasAutomaticTransitionFromStateA, false);
        EXPECT_EQ(hasAutomaticTransitionFromStateA_v, false);

        auto hasAutomaticTransitionFromStateB = hasAutomaticTransition<transitions_table, StateB>::value;
        auto hasAutomaticTransitionFromStateB_v = hasAutomaticTransition_v<transitions_table, StateB>;
        EXPECT_EQ(hasAutomaticTransitionFromStateB, true);
        EXPECT_EQ(hasAutomaticTransitionFromStateB_v, true);
    }

    TEST(AutomaticTransitionTests, SimpleAutomaticTransition)
    {
        using transition1 = Transition<StateA, EventA, StateB>;
        using transition2 = Transition<StateB, AUTOMATIC_TRANSITION, StateC>;
        using transitions_table = TransitionsTable<
            transition1,
            transition2
        >;   

        auto stateMachine = StateMachine<transitions_table, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        
        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateB>(), false);
        EXPECT_EQ(stateMachine.isInState<StateC>(), true);
    }

    TEST(AutomaticTransitionTests, FewAutomaticTransitionsInARow)
    {
        using transition1 = Transition<StateA, EventA, StateB>;
        using transition2 = Transition<StateB, AUTOMATIC_TRANSITION, StateC>;
        using transition3 = Transition<StateC, AUTOMATIC_TRANSITION, StateD>;
        using transitions_table = TransitionsTable<
            transition1,
            transition2,
            transition3
        >;

        auto stateMachine = StateMachine<transitions_table, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateD>(), true);
    }
}
