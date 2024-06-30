#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace {
    struct StateA {};
    struct StateB {};
    struct StateC {};
    struct StateD {};
    struct EventA {};
    struct EventB {};


    TEST(AutomaticTransitionTests, NextState_AutomaticTransition)
    {
        using transition1 = hcFSM::Transition<StateA, EventB, StateB>;
        using transition2 = hcFSM::Transition<StateB, hcFSM::AUTOMATIC_TRANSITION, StateC>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;
        auto hasAutomaticTransitionFromStateA = hcFSM::hasAutomaticTransition<transitions_table, StateA>::value;
        auto hasAutomaticTransitionFromStateA_v = hcFSM::hasAutomaticTransition_v<transitions_table, StateA>;
        EXPECT_EQ(hasAutomaticTransitionFromStateA, false);
        EXPECT_EQ(hasAutomaticTransitionFromStateA_v, false);

        auto hasAutomaticTransitionFromStateB = hcFSM::hasAutomaticTransition<transitions_table, StateB>::value;
        auto hasAutomaticTransitionFromStateB_v = hcFSM::hasAutomaticTransition_v<transitions_table, StateB>;
        EXPECT_EQ(hasAutomaticTransitionFromStateB, true);
        EXPECT_EQ(hasAutomaticTransitionFromStateB_v, true);
    }

    TEST(AutomaticTransitionTests, SimpleAutomaticTransition)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transition2 = hcFSM::Transition<StateB, hcFSM::AUTOMATIC_TRANSITION, StateC>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;   

        auto stateMachine = hcFSM::StateMachine<transitions_table, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        
        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateB>(), false);
        EXPECT_EQ(stateMachine.isInState<StateC>(), true);
    }

    TEST(AutomaticTransitionTests, FewAutomaticTransitionsInARow)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transition2 = hcFSM::Transition<StateB, hcFSM::AUTOMATIC_TRANSITION, StateC>;
        using transition3 = hcFSM::Transition<StateC, hcFSM::AUTOMATIC_TRANSITION, StateD>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2,
            transition3
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateD>(), true);
    }

    TEST(AutomaticTransitionTests, InDeeperLayerAutomaticTransition)
    {
        using InnerStateTransitions3 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, hcFSM::ExitState>
        >;
        using InnerStateMachine3 = hcFSM::StateMachine<InnerStateTransitions3>;

        using InnerStateTransitions2 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, StateB>,
            hcFSM::Transition<StateB, hcFSM::AUTOMATIC_TRANSITION, StateC>,
            hcFSM::Transition<StateC, EventA, StateD>,
            hcFSM::Transition<StateD, hcFSM::AUTOMATIC_TRANSITION, hcFSM::ExitState>
            >;
        using InnerStateMachine2 = hcFSM::StateMachine<InnerStateTransitions2>;

         using InnerStateTransitions1 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine2>,
            hcFSM::Transition<InnerStateMachine2, hcFSM::AUTOMATIC_TRANSITION, InnerStateMachine3>,
            hcFSM::Transition<InnerStateMachine3, EventA, hcFSM::ExitState>
            >;
        using InnerStateMachine1 = hcFSM::StateMachine<InnerStateTransitions1>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine1>,
            hcFSM::Transition<InnerStateMachine1, EventA, StateB>
        >;

        auto stateMachine = hcFSM::StateMachine<Transitions, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        
        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine1StateA = stateMachine.isInState<InnerStateMachine1, StateA>();
        EXPECT_EQ(isInInnerStateMachine1StateA, true);
        
        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateA = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateA>();
       EXPECT_EQ(isInInnerStateMachine2StateA, true);
        
        auto autoTransitResult1 = stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateC = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateC>();
        EXPECT_EQ(isInInnerStateMachine2StateC, true);
        EXPECT_EQ(autoTransitResult1, hcFSM::HandleEventResult::PROCESSED);

        auto autoTransitResult2 = stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine3StateA = stateMachine.isInState<InnerStateMachine1, InnerStateMachine3, StateA>();
        EXPECT_EQ(isInInnerStateMachine3StateA, true);
        EXPECT_EQ(autoTransitResult2, hcFSM::HandleEventResult::PROCESSED);

        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(AutomaticTransitionTests, FromInnerToNextInnerWithAutomaticTransition)
    {
        using InnerStateTransitions2 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, StateB>
        >;
        using InnerStateMachine2 = hcFSM::StateMachine<InnerStateTransitions2>;

         using InnerStateTransitions1 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, StateB>,
            hcFSM::Transition<StateB, hcFSM::AUTOMATIC_TRANSITION, hcFSM::ExitState>
            >;
        using InnerStateMachine1 = hcFSM::StateMachine<InnerStateTransitions1>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine1>,
            hcFSM::Transition<InnerStateMachine1, hcFSM::AUTOMATIC_TRANSITION, InnerStateMachine2>
        >;

        auto stateMachine = hcFSM::StateMachine<Transitions, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine1StateA = stateMachine.isInState<InnerStateMachine1, StateA>();
        EXPECT_EQ(isInInnerStateMachine1StateA, true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateA = stateMachine.isInState<InnerStateMachine2, StateA>();
        EXPECT_EQ(isInInnerStateMachine2StateA, true);
    }

    TEST(AutomaticTransitonTests, FromInitialState)
    {
        using TransitionsTable = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, hcFSM::AUTOMATIC_TRANSITION, StateB>
        >;

        auto stateMachine = hcFSM::StateMachine<TransitionsTable>{};
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }


    TEST(AutomaticTransitionTests, FewLayerUpAutomaticTransition)
    {
        using InnerStateTransitions3 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, hcFSM::ExitState>
        >;
        using InnerStateMachine3 = hcFSM::StateMachine<InnerStateTransitions3>;

        using InnerStateTransitions2 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine3>,
            hcFSM::Transition<InnerStateMachine3, hcFSM::AUTOMATIC_TRANSITION, hcFSM::ExitState>
            >;
        using InnerStateMachine2 = hcFSM::StateMachine<InnerStateTransitions2>;

         using InnerStateTransitions1 = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, InnerStateMachine2>,
            hcFSM::Transition<InnerStateMachine2, hcFSM::AUTOMATIC_TRANSITION, hcFSM::ExitState>
            >;
        using InnerStateMachine1 = hcFSM::StateMachine<InnerStateTransitions1>;

        using Transitions = hcFSM::TransitionsTable <
            hcFSM::Transition<StateA, EventA, InnerStateMachine1>,
            hcFSM::Transition<InnerStateMachine1, hcFSM::AUTOMATIC_TRANSITION, StateB>
        >;

        auto stateMachine = hcFSM::StateMachine<Transitions, StateA>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine1StateA = stateMachine.isInState<InnerStateMachine1, StateA>();
        EXPECT_EQ(isInInnerStateMachine1StateA, true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine2StateA = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, StateA>();
        EXPECT_EQ(isInInnerStateMachine2StateA, true);

        stateMachine.handleEvent(EventA{});
        auto isInInnerStateMachine3StateA = stateMachine.isInState<InnerStateMachine1, InnerStateMachine2, InnerStateMachine3, StateA>();
        EXPECT_EQ(isInInnerStateMachine3StateA, true);

        stateMachine.handleEvent(EventA{});
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }
}
