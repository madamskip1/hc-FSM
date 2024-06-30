#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"


namespace
{
	struct StateA {};
	struct StateB {};

	struct EventA  {};
	struct EventB {};
	
	struct dummyCallableStruct2Args
	{
		void operator() (StateA&, EventA&) {};
	};

	TEST(TransitionStructMembersTests, SimpleTransition)
	{
		using transition = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_before_state_type = std::is_same_v<StateA, typename transition::before_state_type>;
		EXPECT_EQ(is_same_transition_before_state_type, true);

		constexpr auto is_same_transition_event_type = std::is_same_v<EventB, typename transition::event_type>;
		EXPECT_EQ(is_same_transition_event_type, true);

		constexpr auto is_same_transition_next_state_type = std::is_same_v<StateB, typename transition::next_state_type>;
		EXPECT_EQ(is_same_transition_next_state_type, true);
	}

	TEST(TransitionStructMembersTests, TransitionWithoutActionWithGuard)
	{
		using transition = hcFSM::TransitionWithGuard<StateA, EventA, StateB, dummyCallableStruct2Args>;

		constexpr auto is_same_transition_before_state_type = std::is_same_v<StateA, typename transition::before_state_type>;
		EXPECT_EQ(is_same_transition_before_state_type, true);

		constexpr auto is_same_transition_event_type = std::is_same_v<EventA, typename transition::event_type>;
		EXPECT_EQ(is_same_transition_event_type, true);

		constexpr auto is_same_transition_next_state_type = std::is_same_v<StateB, typename transition::next_state_type>;
		EXPECT_EQ(is_same_transition_next_state_type, true);

		constexpr auto noActionVoid = std::is_same_v<void, typename transition::action_type>;
		EXPECT_EQ(noActionVoid, true);

		constexpr auto is_same_guard_type = std::is_same_v<dummyCallableStruct2Args, typename transition::guard_type>;
		EXPECT_EQ(is_same_guard_type, true);
	}
}