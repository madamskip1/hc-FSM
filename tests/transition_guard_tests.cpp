#include <gtest/gtest.h>
#include "hcFSM/detail/transition-guard.h"
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace
{
    CREATE_TRANSITION_GUARD(TransitionGuardPass, sourceState, eventParam, 
    {
        return true;
    });

    CREATE_TRANSITION_GUARD(TransitionGuardFail, sourceState, eventParam, 
    {
        return false;
    });

    CREATE_TRANSITION_GUARD(TransitionGuardOnStateValue, sourceState, eventParam, 
    {
        return sourceState.value > 0 ? true : false;
    });

    struct EventA {};
    struct StateA {};
    struct StateB {};
    struct StateC
    {
        int value;
    };


    TEST(GuardActionTests, hasGuard)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB, void, TransitionGuardPass>;
        ASSERT_EQ(hcFSM::hasGuard_v<transition>, true);
    }

    TEST(GuardActionTests, shouldAllowTransition)
    {
        using transition1 = hcFSM::TransitionWithGuard<StateA, EventA, StateB, TransitionGuardPass>; // or Transition<StateA, EventA, StateB, void, TransitionGuardPass>
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};

        stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(GuardActionTests, shouldNotAllowTransition)
    {
        using transition1 = hcFSM::TransitionWithGuard<StateA, EventA, StateB, TransitionGuardFail>; // or Transition<StateA, EventA, StateB, void, TransitionGuardFail>
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};

        auto handleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::GUARD_FAILED);
    }

    TEST(GuardActionTests, shouldAllowTransitionDependingOnStateValue)
    {
        using transition1 = hcFSM::TransitionWithGuard<StateC, EventA, StateB, TransitionGuardOnStateValue>; // or Transition<StateC, EventA, StateB, void, TransitionGuardOnStateValue>
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.getState<StateC>().value = 1;

        auto handleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(GuardActionTests, shouldNotAllowTransitionDependingOnStateValue)
    {
        using transition1 = hcFSM::TransitionWithGuard<StateC, EventA, StateB, TransitionGuardOnStateValue>; // or Transition<StateC, EventA, StateB, void, TransitionGuardOnStateValue>
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.getState<StateC>().value = 0;

        auto handleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<StateC>(), true);
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::GUARD_FAILED);
    }
    
    TEST(GuardActionTests, shouldAllowAutomaticTransition)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transition2 = hcFSM::TransitionAutomaticWithGuard<StateB, StateC, TransitionGuardPass>;
        // or transition2 = TransitionWithGuard<StateA, AUTOMATIC_TRANSITION, StateB, TransitionGuardPass>
        // or transition2 = Transition<StateA, AUTOMATIC_TRANSITION, StateB, void, TransitionGuardPass>
        
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};

        stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(stateMachine.isInState<StateC>(), true);
    }

    TEST(GuardActionTests, shouldNotAllowAutomaticTransition)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transition2 = hcFSM::TransitionAutomaticWithGuard<StateB, StateA, TransitionGuardFail>;
        // or transition2 = TransitionWithGuard<StateA, AUTOMATIC_TRANSITION, StateB, TransitionGuardFail>
        // or transition2 = Transition<StateA, AUTOMATIC_TRANSITION, StateB, void, TransitionGuardFail>
        
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }
}