#include <gtest/gtest.h>
#include "hcFSM/detail/state-machine.h"
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"

struct StateA {};
struct StateB {};
struct EventA {};

namespace hcFSM
{
    TEST(StateMachineTraitsTests, VariantFromTuple_simpleTuple)
    {
        using SimpleTuple = std::tuple<int, float, double>;
        using SimpleVariant = typename variantTypeFromStatesTuple<SimpleTuple>::type;
        using SimpleVariant_t = variantTypeFromStatesTuple_t<SimpleTuple>;

        auto is_same_simple = std::is_same_v<SimpleVariant, std::variant<std::monostate, int, float, double>>;
        auto is_same_simple_t = std::is_same_v<SimpleVariant_t, std::variant<std::monostate, int, float, double>>;
        EXPECT_EQ(is_same_simple, true);
        EXPECT_EQ(is_same_simple_t, true);
    }

    TEST(StateMachineTraitsTests, VariantFromTuple_emptyTuple)
    {
        using EmptyTuple = std::tuple<>;
        using EmptyVariant = typename variantTypeFromStatesTuple<EmptyTuple>::type;
        using EmptyVariant_t = variantTypeFromStatesTuple_t<EmptyTuple>;

        auto is_same_empty = std::is_same_v<EmptyVariant, std::variant<>>;
        auto is_same_empty_t = std::is_same_v<EmptyVariant_t, std::variant<>>;
        EXPECT_EQ(is_same_empty, true);
        EXPECT_EQ(is_same_empty_t, true);
    }

    TEST(StateMachineTraitsTests, VariantFromTuple_statesTuple)
    {
        using StatesTuple = std::tuple<StateA, StateB>;
        using StatesVariant = typename variantTypeFromStatesTuple<StatesTuple>::type;
        using StatesVariant_t = variantTypeFromStatesTuple_t<StatesTuple>;

        auto is_same_states = std::is_same_v<StatesVariant, std::variant<std::monostate, StateA, StateB>>;
        auto is_same_states_t = std::is_same_v<StatesVariant_t, std::variant<std::monostate, StateA, StateB>>;
        EXPECT_EQ(is_same_states, true);
        EXPECT_EQ(is_same_states_t, true);
    }

    TEST(StateMachineTraitsTests, isTypeinTuple_simpleTuple)
    {
        using SimpleTuple = std::tuple<int, float, double>;

        auto isInSimple = isTypeInTuple_v<int, SimpleTuple>;
        EXPECT_EQ(isInSimple, true);
        
        auto isNotInSimple = isTypeInTuple_v<char, SimpleTuple>;
        EXPECT_EQ(isNotInSimple, false);
    }

    TEST(StateMachineTraitsTests, isTypeinTuple_emptyTuple)
    {
        using EmptyTuple = std::tuple<>;

        auto isInEmpty = isTypeInTuple_v<int, EmptyTuple>;
        EXPECT_EQ(isInEmpty, false);
    }

    TEST(StateMachineTraitsTests, isStateMachine_Valid)
    {
        using transitions_table = hcFSM::TransitionsTable<
            hcFSM::Transition<StateA, EventA, StateB>
        >;
        using StateMachine = hcFSM::StateMachine<transitions_table>;

        EXPECT_EQ(isStateMachine<StateMachine>::value, true);
        EXPECT_EQ(isStateMachine_v<StateMachine>, true);
    }
    
    TEST(StateMachineTraitsTests, isStateMachine_Invalid)
    {
        EXPECT_EQ(isStateMachine<void>::value, false);
        EXPECT_EQ(isStateMachine_v<void>, false);

        EXPECT_EQ(isStateMachine<StateA>::value, false);
        EXPECT_EQ(isStateMachine_v<StateA>, false);
    }
}