#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"

struct StateA {};
struct StateB {};

struct EventA  {};
struct EventB {};

namespace hcFSM
{

    TEST(TransitionTable, isValidTransitionsTable)
	{
		using transition1 = Transition<StateA, EventB, StateB>;
		using transitions_table1 = TransitionsTable<
			transition1
		>;
		EXPECT_EQ(isValidTransitionsTable<transitions_table1>::value, true);
		EXPECT_EQ(isValidTransitionsTable_v<transitions_table1>, true);

		using transition2 = StateA;
		using transitions_table2 = TransitionsTable<
			transition2
		>;
		EXPECT_EQ(isValidTransitionsTable<transitions_table2>::value, false);
		EXPECT_EQ(isValidTransitionsTable_v<transitions_table2>, false);

		using transitions_table3 = TransitionsTable<
		>;

		EXPECT_EQ(isValidTransitionsTable<transitions_table3>::value, false);
		EXPECT_EQ(isValidTransitionsTable_v<transitions_table3>, false);

		EXPECT_EQ(isValidTransitionsTable<void>::value, false);
		EXPECT_EQ(isValidTransitionsTable_v<void>, false);
		EXPECT_EQ(isValidTransitionsTable<StateA>::value, false);
		EXPECT_EQ(isValidTransitionsTable_v<StateA>, false);
	}
    
    TEST(TransitionTable, doTransitionMatch)
	{
		using transition = Transition<StateA, EventB, StateB>;

		constexpr auto matchStateAEventB = doTransitionMatch<transition, StateA, EventB>();
		EXPECT_EQ(matchStateAEventB, true);

		constexpr auto matchStateAEventA = doTransitionMatch<transition, StateA, EventA>();
		EXPECT_EQ(matchStateAEventA, false);

		constexpr auto matchStateBEventA = doTransitionMatch<transition, StateB, EventA>();
		EXPECT_EQ(matchStateBEventA, false);

		constexpr auto matchStateBEventB = doTransitionMatch<transition, StateB, EventB>();
		EXPECT_EQ(matchStateBEventB, false);
	}

    TEST(TransitionTable, HasTransition)
	{
		using transition1 = Transition<StateA, EventB, StateB>;
		using transition2 = Transition<StateB, EventA, StateA>;
		using transitions_table = TransitionsTable<
			transition1,
			transition2
		>;

		constexpr auto hasTransitionStateAEventB = hasTransition<transitions_table, StateA, EventB>::value;
		constexpr auto hasTransitionStateAEventB_v = hasTransition_v<transitions_table, StateA, EventB>;
		EXPECT_EQ(hasTransitionStateAEventB, true);
		EXPECT_EQ(hasTransitionStateAEventB_v, true);

		constexpr auto hasTransitionStateBEventA = hasTransition<transitions_table, StateB, EventA>::value;
		constexpr auto hasTransitionStateBEventA_v = hasTransition_v<transitions_table, StateB, EventA>;
		EXPECT_EQ(hasTransitionStateBEventA, true);
		EXPECT_EQ(hasTransitionStateBEventA_v, true);

		constexpr auto hasTransitionStateBEventB = hasTransition<transitions_table, StateB, EventB>::value;
		constexpr auto hasTransitionStateBEventB_v = hasTransition_v<transitions_table, StateB, EventB>;
		EXPECT_EQ(hasTransitionStateBEventB, false);
		EXPECT_EQ(hasTransitionStateBEventB_v, false);
	}

    TEST(TransitionTable, getStatesFromTransitionsTable)
	{
		using transition1 = Transition<StateA, EventB, StateB>;
		using transition2 = Transition<StateB, EventA, StateA>;
		using transitions_table = TransitionsTable<
			transition1,
			transition2
		>;

		using statesFromTransition = typename getStatesFromTransitionsTable<transitions_table>::states_tuple_type;
		using statesFromTransition_t = getStatesFromTransitionsTable_t<transitions_table>;

		constexpr auto is_same_statesFromTransition = std::is_same_v<std::tuple<StateA, StateB>, statesFromTransition>;
		constexpr auto is_same_statesFromTransition_t = std::is_same_v<std::tuple<StateA, StateB>, statesFromTransition_t>;
		EXPECT_EQ(is_same_statesFromTransition, true);
		EXPECT_EQ(is_same_statesFromTransition_t, true);

		constexpr auto is_same_statesFromTransition_Wrong = std::is_same_v<std::tuple<StateB, StateA>, statesFromTransition>;
		constexpr auto is_same_statesFromTransition_t_Wrong = std::is_same_v<std::tuple<StateB, StateA>, statesFromTransition_t>;
		EXPECT_EQ(is_same_statesFromTransition_Wrong, false);
		EXPECT_EQ(is_same_statesFromTransition_t_Wrong, false);
	}

    TEST(TransitionTable, getTransition)
	{
		using transition1 = Transition<StateA, EventB, StateB>;
		using transition2 = Transition<StateB, EventA, StateA>;
		using transitions_table = TransitionsTable<
			transition1,
			transition2
		>;

		using transitionFromStateAEventB = typename getTransition<transitions_table, StateA, EventB>::type;
		using transitionFromStateAEventB_t = getTransition_t<transitions_table, StateA, EventB>;
		constexpr auto is_same_transitionFromStateAEventB = std::is_same_v<transition1, transitionFromStateAEventB>;
		constexpr auto is_same_transitionFromStateAEventB_t = std::is_same_v<transition1, transitionFromStateAEventB_t>;
		EXPECT_EQ(is_same_transitionFromStateAEventB, true);
		EXPECT_EQ(is_same_transitionFromStateAEventB_t, true);

		using transitionFromStateBEventA = typename getTransition<transitions_table, StateB, EventA>::type;
		using transitionFromStateBEventA_t = getTransition_t<transitions_table, StateB, EventA>;
		constexpr auto is_same_transitionFromStateBEventA = std::is_same_v<transition2, transitionFromStateBEventA>;
		constexpr auto is_same_transitionFromStateBEventA_t = std::is_same_v<transition2, transitionFromStateBEventA_t>;
		EXPECT_EQ(is_same_transitionFromStateBEventA, true);
		EXPECT_EQ(is_same_transitionFromStateBEventA_t, true);

		using transtionFromStateBEventB = typename getTransition<transitions_table, StateB, EventB>::type;
		using transtionFromStateBEventB_t = getTransition_t<transitions_table, StateB, EventB>;
		constexpr auto is_same_transtionFromStateBEventB = std::is_same_v<NO_VALID_TRANSITION, transtionFromStateBEventB>;
		constexpr auto is_same_transtionFromStateBEventB_t = std::is_same_v<NO_VALID_TRANSITION, transtionFromStateBEventB_t>;
		EXPECT_EQ(is_same_transtionFromStateBEventB, true);
		EXPECT_EQ(is_same_transtionFromStateBEventB_t, true);
	}
}