#include <gtest/gtest.h>
#include "hcFSM/transition_action.h"
#include "hcFSM/transition.h"
#include "hcFSM/transitions-table.h"
#include "hcFSM/finite-state-machine.h"

namespace FSM
{
    struct StateA 
    {
        int value = 0;

        void setValue(int v)
        {
            value = v;
        }
    };
    struct StateB 
    {
        int value = 0;
    };
    struct EventA 
    {
        int value = 3;
    };

    CREATE_TRANSITION_ACTION(CreateAction_test_action, currentState, event, nextState, 
        { 
            currentState = 1;
            event = 2; 
            nextState = 3; 
        }
    );

    TEST(TransitionActionTests, CreateAction)
    {
        int currentState = 0;
        int event = 0;
        int nextState = 0;
        CreateAction_test_action {} (currentState, event, nextState);
        EXPECT_EQ(currentState, 1);
        EXPECT_EQ(event, 2);
        EXPECT_EQ(nextState, 3);
    }

    CREATE_TRANSITION_ACTION(ActionWithStates_test_action, currentState, event, nextState,
        {
            currentState.setValue(1);
            nextState.value = event.value;
        }
    );

    TEST(TransitionActionTests, ActionWithStates)
    {
        StateA stateA;
        EventA eventA { 10 };
        StateB stateB;

        ActionWithStates_test_action {} (stateA, eventA, stateB);
        EXPECT_EQ(stateA.value, 1);
        EXPECT_EQ(stateB.value, 10);
    }

    TEST(TransitionActionTest, hasAction_withGeneratedAction)
    {
        using transition = Transition<StateA, EventA, StateB, CreateAction_test_action>;
        ASSERT_EQ(isValidTransition_v<transition>, true);
        ASSERT_EQ(hasAction_v<transition>, true);
    }

    TEST(TransitionActionTest, TransitionWithAction)
    {
        using transition = Transition<StateA, EventA, StateB, ActionWithStates_test_action>;
        ASSERT_EQ(isValidTransition_v<transition>, true);
        ASSERT_EQ(hasAction_v<transition>, true);

        auto stateMachine = StateMachine<TransitionsTable<transition>>{};
        stateMachine.handleEvent(EventA{});
        ASSERT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(stateMachine.getState<StateB>().value, 3);
    }
}