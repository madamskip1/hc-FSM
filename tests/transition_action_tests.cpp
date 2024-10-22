#include <gtest/gtest.h>
#include "hcFSM/detail/transition-action.h"
#include "hcFSM/detail/transition-guard.h"
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace
{
    constexpr int INITIAL_VALUE = 0;
    constexpr int SOURCE_STATE_VALUE = 1;
    constexpr int TARGET_STATE_VALUE = 2;

    CREATE_TRANSITION_ACTION(transitionFunction, sourceState, event, targetState, 
    {
        sourceState.value = SOURCE_STATE_VALUE;
        targetState.value = TARGET_STATE_VALUE;
    });

    CREATE_TRANSITION_GUARD(guardFunctionFail, sourceState, event, 
    {
        return false;
    });

    struct StateA
    {
        static int value;
    };
    int StateA::value = INITIAL_VALUE;
    
    struct StateB
    {
        int value = INITIAL_VALUE;
    };

    struct StateC {};

    struct EventA {};
    

    TEST(TransitionActionTests, hasAction)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB, transitionFunction>;
        EXPECT_TRUE(hcFSM::hasAction_v<transition>);
    }

    TEST(TransitionActionTests, shouldCallTransitionAction)
    {
        StateA::value = INITIAL_VALUE;

        using transition = hcFSM::Transition<StateA, EventA, StateB, transitionFunction>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        EXPECT_EQ(StateA::value, SOURCE_STATE_VALUE);
        EXPECT_EQ(stateMachine.getState<StateB>().value, TARGET_STATE_VALUE);
    }

    TEST(TransitionActionTests, shouldCallTransitionActionIfNextStateIsSameAsCurrent)
    {
        StateA::value = INITIAL_VALUE;

        using transition = hcFSM::Transition<StateA, EventA, StateA, transitionFunction>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        // first set StateA::value = SOURCE_STATE_VALUE as sourceState, then StateA::value = TARGET_STATE_VALUE as targetState
        EXPECT_EQ(stateMachine.getState<StateA>().value, TARGET_STATE_VALUE);
    }

    TEST(TransitionActionTess, shouldCallTransitionActionDuringAutomaticTransition)
    {
        StateA::value = INITIAL_VALUE;
        
        using transition1 = hcFSM::Transition<StateC, EventA, StateA>;
        using transition2 = hcFSM::Transition<StateA, hcFSM::AUTOMATIC_TRANSITION, StateB, transitionFunction>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1, 
            transition2
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        EXPECT_EQ(StateA::value, SOURCE_STATE_VALUE);
        EXPECT_EQ(stateMachine.getState<StateB>().value, TARGET_STATE_VALUE);
    }

    TEST(TransitionActionTests, shouldNotCallTransitionActionIfGuardFail)
    {
        StateA::value = INITIAL_VALUE;

        using transition = hcFSM::Transition<StateA, EventA, StateB, transitionFunction, guardFunctionFail>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();
    
        EXPECT_EQ(StateA::value, INITIAL_VALUE);
    }
}