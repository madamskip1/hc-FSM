#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"
#include "hcFSM/detail/state-machine.h"

namespace
{
        
    struct EventA {};
    struct EventB {};
    struct EventC {};

    enum class ACTIONS_VALUE
    {
        NoValue,
        OnExitNoEventParameter,
        OnExitEventParameter,
        OnExitAutomaticTransitionEventParameter,
        OnEntryNoEventParameter,
        OnEntryEventParameter,
        OnEntryAutomaticTransitionEventParameter
    };
    
    #define onExitNoEventMethod() void onExit() { value = ACTIONS_VALUE::OnExitNoEventParameter; }

    #define onExitEventMethod() void onExit(const EventB&) { value = ACTIONS_VALUE::OnExitEventParameter;  }
    
    #define onEntryNoEventMethod() void onEntry() { value = ACTIONS_VALUE::OnEntryNoEventParameter;  }

    #define onEntryEventMethod() void onEntry(const EventB&) { value = ACTIONS_VALUE::OnEntryEventParameter;  }

    #define onExitAutomaticTransitionEventMethod() void onExit(const hcFSM::AUTOMATIC_TRANSITION&) { value = ACTIONS_VALUE::OnExitAutomaticTransitionEventParameter;  }

    #define onEntryAutomaticTransitionEventMethod() void onEntry(const hcFSM::AUTOMATIC_TRANSITION&) { value = ACTIONS_VALUE::OnEntryAutomaticTransitionEventParameter;  }
    

    struct StateA 
    {
        onExitNoEventMethod();
        onExitEventMethod();

        onEntryNoEventMethod();
        onEntryEventMethod();

        static ACTIONS_VALUE value;
    };
    ACTIONS_VALUE StateA::value = ACTIONS_VALUE::NoValue;

    struct StateB
    {
        onExitNoEventMethod();
        onExitEventMethod();

        onEntryNoEventMethod();
        onEntryEventMethod();
        
        ACTIONS_VALUE value = ACTIONS_VALUE::NoValue;
    };

    struct StateC
    {
        onExitAutomaticTransitionEventMethod();

        onEntryAutomaticTransitionEventMethod();

        static ACTIONS_VALUE value;
    };
    ACTIONS_VALUE StateC::value = ACTIONS_VALUE::NoValue;
    

    struct StateD 
    {
        onExitAutomaticTransitionEventMethod();

        onEntryAutomaticTransitionEventMethod();

        ACTIONS_VALUE value = ACTIONS_VALUE::NoValue;
    };
    
    struct StateNoActions {};


    //////////////////////////////////////
    //              TESTS               //
    //////////////////////////////////////
    

    TEST(OnExitonEntryActionsTests, shouldCallOnExitWithoutEventParameter)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateNoActions>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnExitNoEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnExitWithEventParameter)
    {
        using transition1 = hcFSM::Transition<StateA, EventB, StateNoActions>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventB>();

        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnExitEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryWithoutEventParameter)
    {
        using transition1 = hcFSM::Transition<StateNoActions, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();
        
        ASSERT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(stateMachine.getState<StateB>().value, ACTIONS_VALUE::OnEntryNoEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryWithEventParameter)
    {
        using transition1 = hcFSM::Transition<StateNoActions, EventB, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventB>();

        ASSERT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(stateMachine.getState<StateB>().value, ACTIONS_VALUE::OnEntryEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryAndOnExitWithoutEventParameter)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnExitNoEventParameter);
        ASSERT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(stateMachine.getState<StateB>().value, ACTIONS_VALUE::OnEntryNoEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryAndOnExitWithEventParameter)
    {
        using transition1 = hcFSM::Transition<StateA, EventB, StateB>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventB>();

        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnExitEventParameter);
        ASSERT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(stateMachine.getState<StateB>().value, ACTIONS_VALUE::OnEntryEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldNotCallOnEntryAndOnExitIfNextStateIsSameAsCurrent)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateA>;
        using transitions_table = hcFSM::TransitionsTable<
            transition1
        >;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        EXPECT_EQ(StateA::value, ACTIONS_VALUE::NoValue);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryAndOnExitDuringAutomaticTransitionWithoutEventParameter)
    {
        using transition1 = hcFSM::Transition<StateB, EventA, StateA>;
        using transition2 = hcFSM::TransitionAutomatic<StateA, StateB>; // or Transition<StateB, AUTOMATIC_TRANSITION, StateC>
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        ASSERT_EQ(stateMachine.isInState<StateB>(), true);
        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnExitNoEventParameter);
        EXPECT_EQ(stateMachine.getState<StateB>().value, ACTIONS_VALUE::OnEntryNoEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryAndOnExitDuringAutomaticTransitionWithEventParameter)
    {
        using transition1 = hcFSM::Transition<StateA, EventA, StateC>;
        using transition2 = hcFSM::TransitionAutomatic<StateC, StateD>; // or Transition<StateB, AUTOMATIC_TRANSITION, StateC>
        using transitions_table = hcFSM::TransitionsTable<
            transition1,
            transition2
        >;
        StateC::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {};
        stateMachine.handleEvent<EventA>();

        ASSERT_EQ(stateMachine.isInState<StateD>(), true);
        EXPECT_EQ(StateC::value, ACTIONS_VALUE::OnExitAutomaticTransitionEventParameter);
        EXPECT_EQ(stateMachine.getState<StateD>().value, ACTIONS_VALUE::OnEntryAutomaticTransitionEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryAndOnExitInInnerStateMachineTransition)
    {
        using innerTransition = hcFSM::Transition<StateA, EventA, StateB>;
        using innerTransitionsTable = hcFSM::TransitionsTable<innerTransition>;
        using innerStateMachine = hcFSM::StateMachine<innerTransitionsTable>;

        using transition = hcFSM::Transition<innerStateMachine, EventB, StateA>; // innerStateMachine is initial state
        using transitions_table = hcFSM::TransitionsTable<transition>;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table> {}; // is in innerStateMachine right now
        stateMachine.handleEvent<EventA>();

        const auto isInInnerStateB = stateMachine.isInState<innerStateMachine, StateB>();
        ASSERT_EQ(isInInnerStateB, true);
        const auto innerStateBValue = stateMachine.getState<innerStateMachine, StateB>().value;
        EXPECT_EQ(innerStateBValue, ACTIONS_VALUE::OnEntryNoEventParameter);
        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnExitNoEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldCallOnEntryOnInitializationOfInitialState)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateA>;
        using transitions_table = hcFSM::TransitionsTable<transition>;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table, StateA, true> {};

        ASSERT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(StateA::value, ACTIONS_VALUE::OnEntryNoEventParameter);
    }

    TEST(OnExitonEntryActionsTests, shouldNotCallOnEntryOnInitializationOfInitialState)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateA>;
        using transitions_table = hcFSM::TransitionsTable<transition>;
        StateA::value = ACTIONS_VALUE::NoValue;

        auto stateMachine = hcFSM::StateMachine<transitions_table, StateA, false> {};

        ASSERT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(StateA::value, ACTIONS_VALUE::NoValue);
    }
}