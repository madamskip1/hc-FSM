#include "pch.h"
#include "transition.h"
#include "transitions-table.h"

struct State {};
struct StateA : State {};
struct StateB : State {};

struct Event {};
struct EventA : Event {};
struct EventB : Event {};


namespace FSM
{
	TEST(TransitionTraitsTests, DoTransitionMatch)
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

	TEST(TransitionTraitsTests, HasTransition)
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

	TEST(TransitionTraitsTests, GetStatesFromTransitionsTable)
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

	TEST(TransitionTraitsTests, NextState)
	{
		using transition1 = Transition<StateA, EventB, StateB>;
		using transition2 = Transition<StateB, EventA, StateA>;
		using transitions_table = TransitionsTable<
			transition1,
			transition2
		>;

		using nextFromStateAEventB = typename getNextStateFromTransitionsTable<transitions_table, StateA, EventB>::type;
		using nextFromStateAEventB_t = getNextStateFromTransitionsTable_t<transitions_table, StateA, EventB>;
		constexpr auto is_same_FromStateAEventB = std::is_same_v<StateB, nextFromStateAEventB>;
		constexpr auto is_same_FromStateAEventB_t = std::is_same_v<StateB, nextFromStateAEventB_t>;
		EXPECT_EQ(is_same_FromStateAEventB, true);
		EXPECT_EQ(is_same_FromStateAEventB_t, true);

		using nextFromStateBEventA = typename getNextStateFromTransitionsTable<transitions_table, StateB, EventA>::type;
		using nextFromStateBEventA_t = getNextStateFromTransitionsTable_t<transitions_table, StateB, EventA>;
		constexpr auto is_same_FromStateBEventA = std::is_same_v<StateA, nextFromStateBEventA>;
		constexpr auto is_same_FromStateBEventA_t = std::is_same_v<StateA, nextFromStateBEventA_t>;
		EXPECT_EQ(is_same_FromStateBEventA, true);
		EXPECT_EQ(is_same_FromStateBEventA_t, true);

		using nextFromStateBEventB = typename getNextStateFromTransitionsTable<transitions_table, StateB, EventB>::type;
		using nextFromStateBEventB_t = getNextStateFromTransitionsTable_t<transitions_table, StateB, EventB>;
		constexpr auto is_same_FromStateBEventB = std::is_same_v<NoValidTransition, nextFromStateBEventB>;
		constexpr auto is_same_FromStateBEventB_t = std::is_same_v<NoValidTransition, nextFromStateBEventB_t>;
		EXPECT_EQ(is_same_FromStateBEventB, true);
		EXPECT_EQ(is_same_FromStateBEventB_t, true);
	}
}