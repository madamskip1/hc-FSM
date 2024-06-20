#include <gtest/gtest.h>
#include "hcFSM/detail/state-machine.h"
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"

namespace hcFSM
{
    struct StateA {};
    struct StateB {};
    struct EventA {};
    struct EventB {};

    TEST(StateMachineTests, VariantFromTuple)
    {
        using SimpleTuple = std::tuple<int, float, double>;
        using SimpleVariant = typename variantTypeFromStatesTuple<SimpleTuple>::type;
        using SimpleVariant_t = variantTypeFromStatesTuple_t<SimpleTuple>;
        auto is_same_simple = std::is_same_v<SimpleVariant, std::variant<std::monostate, int, float, double>>;
        auto is_same_simple_t = std::is_same_v<SimpleVariant_t, std::variant<std::monostate, int, float, double>>;
        EXPECT_EQ(is_same_simple, true);
        EXPECT_EQ(is_same_simple_t, true);

        using EmptyTuple = std::tuple<>;
        using EmptyVariant = typename variantTypeFromStatesTuple<EmptyTuple>::type;
        using EmptyVariant_t = variantTypeFromStatesTuple_t<EmptyTuple>;
        auto is_same_empty = std::is_same_v<EmptyVariant, std::variant<>>;
        auto is_same_empty_t = std::is_same_v<EmptyVariant_t, std::variant<>>;
        EXPECT_EQ(is_same_empty, true);
        EXPECT_EQ(is_same_empty_t, true);

        using StatesTuple = std::tuple<StateA, StateB>;
        using StatesVariant = typename variantTypeFromStatesTuple<StatesTuple>::type;
        using StatesVariant_t = variantTypeFromStatesTuple_t<StatesTuple>;
        auto is_same_states = std::is_same_v<StatesVariant, std::variant<std::monostate, StateA, StateB>>;
        auto is_same_states_t = std::is_same_v<StatesVariant_t, std::variant<std::monostate, StateA, StateB>>;
        EXPECT_EQ(is_same_states, true);
        EXPECT_EQ(is_same_states_t, true);
    }

    TEST(StateMachineTests, isTypeinTuple)
    {
        using SimpleTuple = std::tuple<int, float, double>;
        auto isInSimple = isTypeInTuple_v<int, SimpleTuple>;
        EXPECT_EQ(isInSimple, true);
        
        auto isNotInSimple = isTypeInTuple_v<char, SimpleTuple>;
        EXPECT_EQ(isNotInSimple, false);

        using EmptyTuple = std::tuple<>;
        auto isInEmpty = isTypeInTuple_v<int, EmptyTuple>;
        EXPECT_EQ(isInEmpty, false);
    }

    TEST(StateMachineTests, InitialState)
    {
        using transition1 = Transition<StateA, EventA, StateB>;
        using transitions_table = TransitionsTable<
            transition1
        >;
        auto stateMachine1 = StateMachine<transitions_table>{};
        EXPECT_EQ(stateMachine1.isInState<StateA>(), true);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), false);

        auto stateMachine2 = StateMachine<transitions_table, StateB>{};
        EXPECT_EQ(stateMachine2.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine2.isInState<StateB>(), true);
    }

    TEST(StateMachineTests, ForceTransition)
    {
        using transition1 = Transition<StateA, EventA, StateB>;
        using transitions_table = TransitionsTable<
            transition1
        >;
        auto stateMachine = StateMachine<transitions_table>{};
        EXPECT_EQ(stateMachine.isInState<StateA>(), true);
        EXPECT_EQ(stateMachine.isInState<StateB>(), false);

        stateMachine.forceTransition<StateB>();
        EXPECT_EQ(stateMachine.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);

        stateMachine.forceTransition<StateB>();
        EXPECT_EQ(stateMachine.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine.isInState<StateB>(), true);
    }

    TEST(StateMachineTests, HandleEvent)
    {
        using transition1 = Transition<StateA, EventA, StateB>;
        using transition2 = Transition<StateB, EventA, StateB>;
        using transitions_table = TransitionsTable<
            transition1,
            transition2
        >;

        // handleEvent<Event>();

        auto stateMachine1 = StateMachine<transitions_table>{};

        auto handleEventResultTemplate1 = stateMachine1.handleEvent<EventA>();
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResultTemplate1, HandleEventResult::PROCESSED);

        auto handleEventResultTemplate2 = stateMachine1.handleEvent<EventA>();
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResultTemplate2, HandleEventResult::PROCESSED_SAME_STATE);

        auto handleEventResultTemplate3 = stateMachine1.handleEvent<EventB>();
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResultTemplate3, HandleEventResult::NO_VALID_TRANSITION);

        //handleEvent(Event);

        auto stateMachine2 = StateMachine<transitions_table>{};

        auto handleEventResult1 = stateMachine2.handleEvent(EventA{});
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult1, HandleEventResult::PROCESSED);

        auto handleEventResult2 = stateMachine2.handleEvent(EventA{});
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult2, HandleEventResult::PROCESSED_SAME_STATE);

        auto handleEventResult3 = stateMachine2.handleEvent(EventB{});
        EXPECT_EQ(stateMachine1.isInState<StateA>(), false);
        EXPECT_EQ(stateMachine1.isInState<StateB>(), true);
        EXPECT_EQ(handleEventResult3, HandleEventResult::NO_VALID_TRANSITION);
    }
}