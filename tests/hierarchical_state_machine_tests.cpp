#include <gtest/gtest.h>
#include "hcFSM/detail/finite-state-machine.h"
#include "hcFSM/detail/state.h"

struct StateA {};
struct StateB {};
struct StateInnerA {};
struct StateInnerB {};
struct EventA {};
struct EventB {};

namespace hcFSM
{
    TEST(HierarchicalStateMachineTests, IsStateMachine)
    {
        EXPECT_EQ(isStateMachine<void>::value, false);
        EXPECT_EQ(isStateMachine_v<void>, false);
        EXPECT_EQ(isStateMachine<StateA>::value, false);
        EXPECT_EQ(isStateMachine_v<StateA>, false);

        using InnerStateTransitions = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, StateInnerB>,
            hcFSM::Transition<StateInnerB, EventB, hcFSM::ExitState>
            >;
        using InnerStateMachine = hcFSM::StateMachine<InnerStateTransitions>;

        EXPECT_EQ(isStateMachine<InnerStateMachine>::value, true);
        EXPECT_EQ(isStateMachine_v<InnerStateMachine>, true);
    }

    TEST(HierarchicalStateMachine, isInState_Inner)
    {
        using InnerStateTransitions = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, StateInnerB>,
            hcFSM::Transition<StateInnerB, EventB, hcFSM::ExitState>
            >;
        using InnerStateMachine = hcFSM::StateMachine<InnerStateTransitions>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine>,
            hcFSM::Transition<InnerStateMachine, EventB, StateB>
        >;
        auto mainStateMachine = hcFSM::StateMachine<Transitions>{};
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
        using InnerStateTransitions = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, StateInnerB>,
            hcFSM::Transition<StateInnerB, EventB, hcFSM::ExitState>
            >;
        using InnerStateMachine = hcFSM::StateMachine<InnerStateTransitions>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine>,
            hcFSM::Transition<InnerStateMachine, EventB, StateB>
        >;

        auto mainStateMachine = hcFSM::StateMachine<Transitions>{};
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
        using InnerStateTransitions = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventB, hcFSM::ExitState>
            >;
        using InnerStateMachine = hcFSM::StateMachine<InnerStateTransitions>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine>,
            hcFSM::Transition<InnerStateMachine, EventB, StateB>
        >;

        auto mainStateMachine = hcFSM::StateMachine<Transitions>{};
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), true);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);

        auto handleEventResult1 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult1 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, hcFSM::HandleEventResult::PROCESSED);
        

        auto handleEventResult2 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult2 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, hcFSM::HandleEventResult::NO_VALID_TRANSITION);
    }

    TEST(HierarchicalStateMachineTests, SimpleHierarchicalStateMachine)
    {
        using InnerStateTransitions = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, StateInnerB>,
            hcFSM::Transition<StateInnerB, EventB, hcFSM::ExitState>
            >;
        using InnerStateMachine = hcFSM::StateMachine<InnerStateTransitions>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine>,
            hcFSM::Transition<InnerStateMachine, EventB, StateB>
        >;
        auto mainStateMachine = hcFSM::StateMachine<Transitions>{};
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), true);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);

        auto handleEventResult1 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);auto isInInnerStateResult1 = mainStateMachine.isInState<InnerStateMachine, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, hcFSM::HandleEventResult::PROCESSED);
        
        auto handleEventResult2 = mainStateMachine.handleEvent<EventA>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), false);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), true);
        auto isInInnerStateResult2 = mainStateMachine.isInState<InnerStateMachine, StateInnerB>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, hcFSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto handleEventResult3 = mainStateMachine.handleEvent<EventB>();
        EXPECT_EQ(mainStateMachine.isInState<StateA>(), false);
        EXPECT_EQ(mainStateMachine.isInState<StateB>(), true);
        EXPECT_EQ(mainStateMachine.isInState<InnerStateMachine>(), false);
        EXPECT_EQ(handleEventResult3, hcFSM::HandleEventResult::PROCESSED);
    }

    TEST(HierarchicalStateMachineTests, MultiLayerSM)
    {
        using InnerStateTransitions3 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, hcFSM::ExitState>
        >;
        using InnerStateMachine3 = hcFSM::StateMachine<InnerStateTransitions3>;

        using InnerStateTransitions2 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, InnerStateMachine3>,
            hcFSM::Transition<InnerStateMachine3, EventA, StateInnerB>,
            hcFSM::Transition<StateInnerB, EventB, hcFSM::ExitState>
        >;
        using InnerStateMachine2 = hcFSM::StateMachine<InnerStateTransitions2>;

        using InnerStateTransitions1 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateInnerA, EventA, InnerStateMachine2>,
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
        auto isInInnerStateResult1 = stateMachine.isInState<InnerStateMachine1, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult1, true);
        EXPECT_EQ(handleEventResult1, hcFSM::HandleEventResult::PROCESSED);

        auto handleEventResult2 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult2 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult2, true);
        EXPECT_EQ(handleEventResult2, hcFSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto handleEventResult3 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult3 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, InnerStateMachine3, StateInnerA>();
        EXPECT_EQ(isInInnerStateResult3, true);
        EXPECT_EQ(handleEventResult3, hcFSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);
        
        auto handleEventResult4 = stateMachine.handleEvent<EventA>();
        EXPECT_EQ(stateMachine.isInState<InnerStateMachine1>(), true);
        auto isInInnerStateResult4 = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateInnerB>();
        EXPECT_EQ(isInInnerStateResult4, true);
        EXPECT_EQ(handleEventResult4, hcFSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto handleEventResult5 = stateMachine.handleEvent<EventB>();
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult5, hcFSM::HandleEventResult::PROCESSED);
    }
}