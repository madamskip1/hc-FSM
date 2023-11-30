#include <gtest/gtest.h>
#include "transition.h"
#include "transitions-table.h"
#include "finite-state-machine.h"

namespace FSM
{
    struct StateA {};
    struct StateB {};
    struct StateC {};
    
    struct EventA {};
    struct EventB {};

    TEST(InnerSMTransitionsTests, shouldEnterInnerStateMachine)
    {
        using innerTransition = Transition<StateB, EventA, StateA>;
        using innerSM = StateMachine<TransitionsTable<innerTransition>>;
        using transition = Transition<StateA, EventA, innerSM>;
        using transitions_table = TransitionsTable<transition>;

        auto stateMachine = StateMachine<transitions_table> {};
        auto handleEventResult = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(handleEventResult, HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateB = stateMachine.isInState<innerSM, StateB>();
        EXPECT_EQ(isInInnerStateB, true);
    }

    TEST(InnerSMTransitionsTests, shouldTransitInInnerStateMachine)
    {
        using innerTransition = Transition<StateA, EventB, StateB>;
        using innerSM = StateMachine<TransitionsTable<innerTransition>>;

        using transition = Transition<innerSM, EventA, StateC>;
        using transitions_table = TransitionsTable<transition>;

        auto stateMachine = StateMachine<transitions_table> {}; // initial state is innerSM::StateA
        auto handleEventResult = stateMachine.handleEvent<EventB>();

        EXPECT_EQ(handleEventResult, HandleEventResult::PROCESSED_INNER_STATE_MACHINE);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateB = stateMachine.isInState<innerSM, StateB>();
        EXPECT_EQ(isInInnerStateB, true);
    }

    TEST(InnerSMTransitionsTests, shouldNotTransitInInnerStateMachine)
    {
        using innerTransition = Transition<StateA, EventB, StateB>;
        using innerSM = StateMachine<TransitionsTable<innerTransition>>;

        using transition = Transition<innerSM, EventB, StateC>;
        using transitions_table = TransitionsTable<transition>;

        auto stateMachine = StateMachine<transitions_table> {}; // initial state is innerSM::StateA
        auto handleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleEventResult, HandleEventResult::NO_VALID_TRANSITION);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateA = stateMachine.isInState<innerSM, StateA>();
        EXPECT_EQ(isInInnerStateA, true);
    }

    TEST(InnerSMTransitionsTests, shouldExitInnerStateMachine)
    {
        using innerTransition = Transition<StateA, EventA, ExitState>;
        using innerSM = StateMachine<TransitionsTable<innerTransition>>;

        using transition = Transition<innerSM, EventA, StateB>;
        using transitions_table = TransitionsTable<transition>;
        
        auto stateMachine = StateMachine<transitions_table> {}; // initial state is innerSM::StateA
        auto handleResultEvent = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleResultEvent, HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(InnerSMTransitionsTests, shouldChainExitInnerStateMachine)
    {
        using innerTransition2 = Transition<StateA, EventA, ExitState>;
        using innerSM2 = StateMachine<TransitionsTable<innerTransition2>>;

        using innerTransition1 = Transition<innerSM2, EventA, ExitState>;
        using innerSM1 = StateMachine<TransitionsTable<innerTransition1>>;

        using transition = Transition<innerSM1, EventA, StateB>;
        using transitions_table = TransitionsTable<transition>;
        
        auto stateMachine = StateMachine<transitions_table> {}; // initial state is innerSM1::innerSM2::StateA
        auto handleResultEvent = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleResultEvent, HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(InnerSMTransitionsTests, shouldAutomaticalyTransitStateInInnerStateMachine)
    {
        using innerTransition1 = Transition<StateA, EventA, StateB>;
        using innerTransition2 = TransitionAutomatic<StateB, StateC>; // or Transition<StateB, AUTOMATIC_TRANSITION, StateC>
        using innerSM = StateMachine<TransitionsTable<
            innerTransition1,
            innerTransition2
        >>;

        using transition = Transition<innerSM, EventA, StateA>;
        using transitions_table = TransitionsTable<transition>;
        
        auto stateMachine = StateMachine<transitions_table> {}; // initial state is innerSM::StateA
        auto handleResultEvent = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(handleResultEvent, HandleEventResult::PROCESSED_INNER_STATE_MACHINE);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateC = stateMachine.isInState<innerSM, StateC>();
        EXPECT_EQ(isInInnerStateC, true);   
    }
}