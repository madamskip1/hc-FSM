#include <gtest/gtest.h>
#include "finite-state-machine.h"
#include "state.h"
#include <iostream>

struct StateA {};
struct StateB {};
struct StateInnerA {};
struct StateInnerB {};
struct EventA {};
struct EventB {};

namespace FSM
{
    TEST(HierarchicalStateMachineTests, IsStateMachine)
    {
        EXPECT_EQ(isStateMachine<void>::value, false);
        EXPECT_EQ(isStateMachine_v<void>, false);
        EXPECT_EQ(isStateMachine<StateA>::value, false);
        EXPECT_EQ(isStateMachine_v<StateA>, false);

        using InnerStateTransitions = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, StateInnerB>,
            FSM::Transition<StateInnerB, EventB, FSM::ExitState>
            >;
        using InnerStateMachine = FSM::StateMachine<InnerStateTransitions>;

        EXPECT_EQ(isStateMachine<InnerStateMachine>::value, true);
        EXPECT_EQ(isStateMachine_v<InnerStateMachine>, true);
    }

    TEST(HierarchicalStateMachine, isInState_Inner)
    {
        using InnerStateTransitions = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, StateInnerB>,
            FSM::Transition<StateInnerB, EventB, FSM::ExitState>
            >;
        using InnerStateMachine = FSM::StateMachine<InnerStateTransitions>;

        using Transitions = FSM::TransitionsTable <
            FSM::Transition<StateA, EventA, InnerStateMachine>,
            FSM::Transition<InnerStateMachine, EventB, StateB>
        >;
        auto mainStateMachine = FSM::StateMachine<Transitions>{};
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), true);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);
        auto isInInnerStateResult1 = mainStateMachine.isInState<StateA, StateInnerB>();
        EXPECT_EQ(isInInnerStateResult1, false);
        auto isInInnerStateResult2 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult2, false);

        mainStateMachine.forceTransition<InnerStateMachine>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult3 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult3, true);
    }

    TEST(HierarchicalStateMachineTests, ForceTransition_Inner)
    {
        using InnerStateTransitions = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, StateInnerB>,
            FSM::Transition<StateInnerB, EventB, FSM::ExitState>
            >;
        using InnerStateMachine = FSM::StateMachine<InnerStateTransitions>;

        using Transitions = FSM::TransitionsTable <
            FSM::Transition<StateA, EventA, InnerStateMachine>,
            FSM::Transition<InnerStateMachine, EventB, StateB>
        >;

        auto mainStateMachine = FSM::StateMachine<Transitions>{};
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), true);

        mainStateMachine.forceTransition<InnerStateMachine>();
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);  
        auto isInInnerStateResult1 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);

        mainStateMachine.forceTransition<InnerStateMachine, StateInnerB>();
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult2 = mainStateMachine.isInState<InnerStateMachine, StateInnerB>();
        EXPECT_EQ(isInInnerStateResult2, true);
    }

    TEST(HierarchicalStateMachineTests, NoValidTransition_Inner)
    {
        using InnerStateTransitions = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventB, FSM::ExitState>
            >;
        using InnerStateMachine = FSM::StateMachine<InnerStateTransitions>;

        using Transitions = FSM::TransitionsTable <
            FSM::Transition<StateA, EventA, InnerStateMachine>,
            FSM::Transition<InnerStateMachine, EventB, StateB>
        >;

        auto mainStateMachine = FSM::StateMachine<Transitions>{};
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), true);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);

        auto handleEventResult1 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult1 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, FSM::HandleEventResult::PROCESSED);
        

        auto handleEventResult2 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult2 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, FSM::HandleEventResult::NO_VALID_TRANSITION);
    }

    TEST(HierarchicalStateMachineTests, SimpleHierarchicalStateMachine)
    {
        using InnerStateTransitions = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, StateInnerB>,
            FSM::Transition<StateInnerB, EventB, FSM::ExitState>
            >;
        using InnerStateMachine = FSM::StateMachine<InnerStateTransitions>;

        using Transitions = FSM::TransitionsTable <
            FSM::Transition<StateA, EventA, InnerStateMachine>,
            FSM::Transition<InnerStateMachine, EventB, StateB>
        >;
        auto mainStateMachine = FSM::StateMachine<Transitions>{};
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), true);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);

        auto handleEventResult1 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);auto isInInnerStateResult1 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, FSM::HandleEventResult::PROCESSED);
        
        auto handleEventResult2 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult2 = mainStateMachine.isInState<InnerStateMachine, StateInnerB>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, FSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto handleEventResult3 = mainStateMachine.handleEvent<EventB>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), true);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);
        EXPECT_EQ(handleEventResult3, FSM::HandleEventResult::PROCESSED);
    }

    TEST(HierarchicalStateMachineTests, MultiLayerSM)
    {
        using InnerStateTransitions3 = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, FSM::ExitState>
        >;
        using InnerStateMachine3 = FSM::StateMachine<InnerStateTransitions3>;

        using InnerStateTransitions2 = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, InnerStateMachine3>,
            FSM::Transition<InnerStateMachine3, EventA, StateInnerB>,
            FSM::Transition<StateInnerB, EventB, FSM::ExitState>
        >;
        using InnerStateMachine2 = FSM::StateMachine<InnerStateTransitions2>;

        using InnerStateTransitions1 = FSM::TransitionsTable<
            FSM::Transition<StateInnerA, EventA, InnerStateMachine2>,
            FSM::Transition<InnerStateMachine2, EventB, FSM::ExitState>
        >;
        using InnerStateMachine1 = FSM::StateMachine<InnerStateTransitions1>;

        using Transitions = FSM::TransitionsTable<
            FSM::Transition<StateA, EventA, InnerStateMachine1>,
            FSM::Transition<InnerStateMachine1, EventB, StateB>
        >;

        auto stateMachine = FSM::StateMachine<Transitions>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        auto handleEventResult1 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult1 = stateMachine.isInState<InnerStateMachine1, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, FSM::HandleEventResult::PROCESSED);

        auto handleEventResult2 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult2 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, FSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto handleEventResult3 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult3 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, InnerStateMachine3, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult3, true);
        EXPECT_EQ(handleEventResult3, FSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);
        
        auto handleEventResult4 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult4 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateInnerB>();
        EXPECT_EQ(isInInnerStateResult4, true);
        EXPECT_EQ(handleEventResult4, FSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto handleEventResult5 = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult5, FSM::HandleEventResult::PROCESSED);
    }
}