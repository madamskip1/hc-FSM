#include <gtest/gtest.h>
#include "hcFSM/transition.h"
#include "hcFSM/transitions-table.h"
#include "hcFSM/finite-state-machine.h"

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

    TEST(AutomaticTransitionTests, InDeeperLayerAutomaticTransition)
    {
        using InnerStateTransitions3 = FSM::TransitionsTable<
            FSM::Transition<StateA, EventA, ExitState>
        >;
        using InnerStateMachine3 = FSM::StateMachine<InnerStateTransitions3>;

        using InnerStateTransitions2 = FSM::TransitionsTable<
            FSM::Transition<StateA, EventA, StateB>,
            FSM::Transition<StateB, AUTOMATIC_TRANSITION, StateC>,
            FSM::Transition<StateC, EventA, StateD>,
            FSM::Transition<StateD, AUTOMATIC_TRANSITION, ExitState>
            >;
        using InnerStateMachine2 = FSM::StateMachine<InnerStateTransitions2>;

         using InnerStateTransitions1 = FSM::TransitionsTable<
            FSM::Transition<StateA, EventA, InnerStateMachine2>,
            FSM::Transition<InnerStateMachine2, AUTOMATIC_TRANSITION, InnerStateMachine3>,
            FSM::Transition<InnerStateMachine3, EventA, ExitState>
            >;
        using InnerStateMachine1 = FSM::StateMachine<InnerStateTransitions1>;

        using Transitions = FSM::TransitionsTable <
            FSM::Transition<StateA, EventA, InnerStateMachine1>,
            FSM::Transition<InnerStateMachine1, EventA, StateB>
        >;

        auto stateMachine = FSM::StateMachine<Transitions, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        
        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine1StateA = stateMachine.isInState<InnerStateMachine1, StateA>();
        EXPECT_EQ(isInInnerStateMachine1StateA, true);
        std::cout << "przed eventem______" << std::endl;
        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateA = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateA>();
       EXPECT_EQ(isInInnerStateMachine2StateA, true);
        
        auto autoTransitResult1 = stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateC = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateC>();
        EXPECT_EQ(isInInnerStateMachine2StateC, true);
        EXPECT_EQ(autoTransitResult1, FSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        auto autoTransitResult2 = stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine3StateA = stateMachine.isInState<InnerStateMachine1, InnerStateMachine3, StateA>();
        EXPECT_EQ(isInInnerStateMachine3StateA, true);
        EXPECT_EQ(autoTransitResult2, FSM::HandleEventResult::PROCESSED_INNER_STATE_MACHINE);

        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(AutomaticTransitionTests, FromInnerToNextInnerWithAutomaticTransition)
    {
        using InnerStateTransitions2 = FSM::TransitionsTable<
            FSM::Transition<StateA, EventA, StateB>
        >;
        using InnerStateMachine2 = FSM::StateMachine<InnerStateTransitions2>;

         using InnerStateTransitions1 = FSM::TransitionsTable<
            FSM::Transition<StateA, EventA, StateB>,
            FSM::Transition<StateB, AUTOMATIC_TRANSITION, ExitState>
            >;
        using InnerStateMachine1 = FSM::StateMachine<InnerStateTransitions1>;

        using Transitions = FSM::TransitionsTable <
            FSM::Transition<StateA, EventA, InnerStateMachine1>,
            FSM::Transition<InnerStateMachine1, AUTOMATIC_TRANSITION, InnerStateMachine2>
        >;

        auto stateMachine = FSM::StateMachine<Transitions, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine1StateA = stateMachine.isInState<InnerStateMachine1, StateA>();
        EXPECT_EQ(isInInnerStateMachine1StateA, true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateA = stateMachine.isInState<InnerStateMachine2, StateA>();
        EXPECT_EQ(isInInnerStateMachine2StateA, true);
    }
}
