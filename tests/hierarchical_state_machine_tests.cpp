#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace
{
    struct StateA {};
    struct StateB {};
    struct StateC {};
    
    struct EventA {};
    struct EventB {};

    TEST(HierarchicalStateMachineTests, shouldEnterInnerStateMachine)
    {
        using innerTransition = hcFSM::Transition<StateB, EventA, StateA>;
        using innerSM = hcFSM::StateMachine<hcFSM::TransitionsTable<innerTransition>>;
        using transition = hcFSM::Transition<StateA, EventA, innerSM>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};

        auto handleEventResult = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateB = stateMachine.isInState<innerSM, StateB>();
        EXPECT_EQ(isInInnerStateB, true);
    }

    TEST(HierarchicalStateMachineTests, shouldTransitInInnerStateMachine)
    {
        using innerTransition = hcFSM::Transition<StateA, EventB, StateB>;
        using innerSM = hcFSM::StateMachine<hcFSM::TransitionsTable<innerTransition>>;

        using transition = hcFSM::Transition<innerSM, EventA, StateC>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {}; // initial state is innerSM::StateA

        auto handleEventResult = stateMachine.handleEvent<EventB>();

        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateB = stateMachine.isInState<innerSM, StateB>();
        EXPECT_EQ(isInInnerStateB, true);
    }

    TEST(HierarchicalStateMachineTests, shouldNotTransitInInnerStateMachine)
    {
        using innerTransition = hcFSM::Transition<StateA, EventB, StateB>;
        using innerSM = hcFSM::StateMachine<hcFSM::TransitionsTable<innerTransition>>;

        using transition = hcFSM::Transition<innerSM, EventB, StateC>;
        using transitions_table = hcFSM::TransitionsTable<transition>;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {}; // initial state is innerSM::StateA

        auto handleEventResult = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleEventResult, hcFSM::HandleEventResult::NO_VALID_TRANSITION);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateA = stateMachine.isInState<innerSM, StateA>();
        EXPECT_EQ(isInInnerStateA, true);
    }

    TEST(HierarchicalStateMachineTests, shouldExitInnerStateMachine)
    {
        using innerTransition = hcFSM::Transition<StateA, EventA, hcFSM::ExitState>;
        using innerSM = hcFSM::StateMachine<hcFSM::TransitionsTable<innerTransition>>;

        using transition = hcFSM::Transition<innerSM, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<transition>;
        
        auto stateMachine = hcFSM::StateMachine<transitions_table> {}; // initial state is innerSM::StateA

        auto handleResultEvent = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleResultEvent, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(HierarchicalStateMachineTests, shouldChainExitInnerStateMachine)
    {
        using innerTransition2 = hcFSM::Transition<StateA, EventA, hcFSM::ExitState>;
        using innerSM2 = hcFSM::StateMachine<hcFSM::TransitionsTable<innerTransition2>>;

        using innerTransition1 = hcFSM::Transition<innerSM2, EventA, hcFSM::ExitState>;
        using innerSM1 = hcFSM::StateMachine<hcFSM::TransitionsTable<innerTransition1>>;

        using transition = hcFSM::Transition<innerSM1, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<transition>;
        
        auto stateMachine = hcFSM::StateMachine<transitions_table> {}; // initial state is innerSM1::innerSM2::StateA
        
        auto handleResultEvent = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(handleResultEvent, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(HierarchicalStateMachineTests, shouldAutomaticalyTransitStateInInnerStateMachine)
    {
        using innerTransition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using innerTransition2 = hcFSM::TransitionAutomatic<StateB, StateC>; // or hcFSM::Transition<StateB, AUTOMATIC_TRANSITION, StateC>
        using innerSM = hcFSM::StateMachine<hcFSM::TransitionsTable<
            innerTransition1,
            innerTransition2
        >>;

        using transition = hcFSM::Transition<innerSM, EventA, StateA>;
        using transitions_table = hcFSM::TransitionsTable<transition>;
        
        auto stateMachine = hcFSM::StateMachine<transitions_table> {}; // initial state is innerSM::StateA
        
        auto handleResultEvent = stateMachine.handleEvent<EventA>();
        
        EXPECT_EQ(handleResultEvent, hcFSM::HandleEventResult::PROCESSED);
        EXPECT_EQ(stateMachine.isInState<innerSM>(), true);
        auto isInInnerStateC = stateMachine.isInState<innerSM, StateC>();
        EXPECT_EQ(isInInnerStateC, true);   
    }

    TEST(HierarchicalStateMachineTests, MultiLayerSM)
    {
        using InnerStateTransitions3 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, hcFSM::ExitState>
        >;
        using InnerStateMachine3 = hcFSM::StateMachine<InnerStateTransitions3>;

        using InnerStateTransitions2 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine3>,
            hcFSM::Transition<InnerStateMachine3, EventA, StateB>,
            hcFSM::Transition<StateB, EventB, hcFSM::ExitState>
        >;
        using InnerStateMachine2 = hcFSM::StateMachine<InnerStateTransitions2>;

        using InnerStateTransitions1 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine2>,
            hcFSM::Transition<InnerStateMachine2, EventB, hcFSM::ExitState>
        >;
        using InnerStateMachine1 = hcFSM::StateMachine<InnerStateTransitions1>;

        using Transitions = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine1>,
            hcFSM::Transition<InnerStateMachine1, EventB, StateB>
        >;

        auto stateMachine = hcFSM::StateMachine<Transitions>{};

        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        auto handleEventResult1 = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult1 = stateMachine.isInState<InnerStateMachine1, StateA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, hcFSM::HandleEventResult::PROCESSED);

        auto handleEventResult2 = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult2 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateA>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, hcFSM::HandleEventResult::PROCESSED);

        auto handleEventResult3 = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult3 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, InnerStateMachine3, StateA>();
        EXPECT_EQ(isInInnerStateResult3, true);
        EXPECT_EQ(handleEventResult3, hcFSM::HandleEventResult::PROCESSED);
        
        auto handleEventResult4 = stateMachine.handleEvent<EventA>();

        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult4 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateB>();
        EXPECT_EQ(isInInnerStateResult4, true);
        EXPECT_EQ(handleEventResult4, hcFSM::HandleEventResult::PROCESSED);

        auto handleEventResult5 = stateMachine.handleEvent<EventB>();
        
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult5, hcFSM::HandleEventResult::PROCESSED);
    }
}