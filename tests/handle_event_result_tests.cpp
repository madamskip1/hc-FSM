#include <gtest/gtest.h>
#include "hcFSM/detail/state-machine.h"
#include "hcFSM/detail/state.h"
#include "hcFSM/detail/handle-event-result.h"
#include "hcFSM/detail/transition-guard.h"


namespace hcFSM
{
    struct StateA {};
    struct StateB {};
    struct StateC {};

    struct EventA {};
    struct EventB {};

    CREATE_TRANSITION_GUARD(TransitionGuardFail, sourceState, eventParam, 
    {
        return false;
    });

    TEST(HandleEventResultTests, shouldReturnProcessed_simpleTransition)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB>;
        using transitionsTable = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitionsTable> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
    }

    TEST(HandleEventResultTests, shouldReturnNoValidTransition_simpleTransition)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB>;
        using transitionsTable = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitionsTable> {};
        auto handleEventResult = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::NO_VALID_TRANSITION);
    }

    TEST(HandleEventResultTests, shouldReturnProcessedSameState_simpleTransition)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateA>;
        using transitionsTable = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitionsTable> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED_SAME_STATE);
    }
    
    TEST(HandleEventResultTest, shouldReturnGuardFailed_simpleTransition)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB, void, TransitionGuardFail>;
        using transitionsTable = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitionsTable> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::GUARD_FAILED);
    }

    TEST(HandleEventResultTests, shouldReturnProcessed_goDownIntoInnerStateMachine)
    {
        using innerStateMachineTransitionsTable = hcFSM::TransitionsTable<
            hcFSM::Transition<StateB, EventB, StateC>
        >;
        using InnerStateMachine = hcFSM::StateMachine<innerStateMachineTransitionsTable>;
        using transitionsTable_TopLevel = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine>
        >;

        auto stateMachine = hcFSM::StateMachine<transitionsTable_TopLevel> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
    }

    TEST(HandleEventResultTests, shouldReturnProcessed_innerStateMachine)
    {
        using innerStateMachineTransitionsTable = hcFSM::TransitionsTable<
            hcFSM::Transition<StateB, EventB, StateC>
        >;
        using InnerStateMachine = hcFSM::StateMachine<innerStateMachineTransitionsTable>;
        using transitionsTable_TopLevel = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine>
        >;

        auto stateMachine = hcFSM::StateMachine<transitionsTable_TopLevel> {};
        stateMachine.handleEvent<EventA>();
        auto isInInnerStateMachineStateB = stateMachine.isInState<InnerStateMachine, StateB>();
        EXPECT_EQ(isInInnerStateMachineStateB, true);

        auto handleEventResult = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
    }

    TEST(HandleEventResultTests, shouldReturnProcessedSameState_innerStateMachine)
    {
        using innerStateMachineTransitionsTable = hcFSM::TransitionsTable<
            hcFSM::Transition<StateB, EventB, StateB>
        >;
        using InnerStateMachine = hcFSM::StateMachine<innerStateMachineTransitionsTable>;
        using transitionsTable_TopLevel = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine>
        >;

        auto stateMachine = hcFSM::StateMachine<transitionsTable_TopLevel> {};
        stateMachine.handleEvent<EventA>();
        auto isInInnerStateMachineStateB = stateMachine.isInState<InnerStateMachine, StateB>();
        EXPECT_EQ(isInInnerStateMachineStateB, true);

        auto handleEventResult = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED_SAME_STATE);
    }

    TEST(HandleEventResultTests, shouldReturnProcessed_exitInnerStateMachine)
    {
        using innerStateMachineTransitionsTable = hcFSM::TransitionsTable<
            hcFSM::Transition<StateB, EventB, hcFSM::ExitState>
        >;
        using InnerStateMachine = hcFSM::StateMachine<innerStateMachineTransitionsTable>;
        using transitionsTable_TopLevel = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine>,
            hcFSM::Transition<InnerStateMachine, EventB, StateC>
        >;

        auto stateMachine = hcFSM::StateMachine<transitionsTable_TopLevel> {};
        stateMachine.handleEvent<EventA>();

        auto handleEventResult = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
    }

    TEST(HandleEventResultTests, shouldReturnGuardFailed_innerStateMachine)
    {
        using innerStateMachineTransitionsTable = hcFSM::TransitionsTable<
            hcFSM::Transition<StateB, EventB, StateC, void, TransitionGuardFail>
        >;
        using InnerStateMachine = hcFSM::StateMachine<innerStateMachineTransitionsTable>;
        using transitionsTable_TopLevel = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine>
        >;

        auto stateMachine = hcFSM::StateMachine<transitionsTable_TopLevel> {};
        stateMachine.handleEvent<EventA>();

        auto handleEventResult = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::GUARD_FAILED);
    }
}