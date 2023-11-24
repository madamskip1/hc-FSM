#include <gtest/gtest.h>
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
	struct dummyCallableStruct
	{
		void operator() () {};
	};

	TEST(TransitionTraitsTests, Transition)
	{
		using transition = Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_before_state_type = std::is_same_v<StateA, typename transition::before_state_type>;
		EXPECT_EQ(is_same_transition_before_state_type, true);

		constexpr auto is_same_transition_event_type = std::is_same_v<EventB, typename transition::event_type>;
		EXPECT_EQ(is_same_transition_event_type, true);

		constexpr auto is_same_transition_next_state_type = std::is_same_v<StateB, typename transition::next_state_type>;
		EXPECT_EQ(is_same_transition_next_state_type, true);
	}

	TEST(TransitionTraitsTests, isValidTransition)
	{
		using transition = Transition<StateA, EventB, StateB>;

		constexpr auto is_valid_transition = isValidTransition<transition>::value;
		constexpr auto is_valid_transition_v = isValidTransition_v<transition>;
		EXPECT_EQ(is_valid_transition, true);
		EXPECT_EQ(is_valid_transition_v, true);
	}

	TEST(TransitionTraitsTests, getBeforeState)
	{
		using transition = Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_before_state_type = std::is_same_v<StateA, getBeforeState_t<transition>>;
		EXPECT_EQ(is_same_transition_before_state_type, true);
	}

	TEST(TransitionTraitsTests, getNextState)
	{
		using transition = Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_next_state_type = std::is_same_v<StateB, getNextState_t<transition>>;
		EXPECT_EQ(is_same_transition_next_state_type, true);
	}

	TEST(TransitionTraitsTests, getEvent)
	{
		using transition = Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_event_type = std::is_same_v<EventB, getEvent_t<transition>>;
		EXPECT_EQ(is_same_transition_event_type, true);
	}

	TEST(TransitionTraitsTests, getAction)
	{
		using transition = Transition<StateA, EventB, StateB, dummyCallableStruct>;
		using transitionNoAction = Transition<StateA, EventB, StateB>;

		constexpr bool thereIsAction = std::is_same_v<dummyCallableStruct, getAction_t<transition>>;
		EXPECT_EQ(thereIsAction, true);

		constexpr auto noActionVoid = std::is_same_v<void, getAction_t<transitionNoAction>>;
		EXPECT_EQ(noActionVoid, true);
	}
	
	TEST(TransitionTraitsTests, hasAction)
	{
		using transition = Transition<StateA, EventB, StateB, dummyCallableStruct>;
		using transitionNoAction = Transition<StateA, EventB, StateB>;

		EXPECT_EQ(hasAction<transition>::value, true);
		EXPECT_EQ(hasAction_v<transition>, true);	
		EXPECT_EQ(hasAction<transitionNoAction>::value, false);
		EXPECT_EQ(hasAction_v<transitionNoAction>, false);
	}

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

	TEST(TransitionTraitsTests, getTransition)
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