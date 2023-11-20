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

    TEST(HierarchicalStateMachineTests, NoValidTransitionInInnerStateMachine)
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


}