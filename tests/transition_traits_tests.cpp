#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"

struct StateA {};
struct StateB  {};

struct EventA {};
struct EventB {};

namespace hcFSM
{
    struct dummyCallableStruct3Args
	{
		void operator() (StateA&, EventA&, StateB&) {};
	};

	struct dummyCallableStruct2Args
	{
		void operator() (StateA&, EventA&) {};
	};


    TEST(TransitionTraitsTests, isValidTransition_Valid)
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
		using transition = Transition<StateA, EventA, StateB, dummyCallableStruct3Args>;
		using transitionNoAction = Transition<StateA, EventB, StateB>;

		constexpr bool thereIsAction = std::is_same_v<dummyCallableStruct3Args, getAction_t<transition>>;
		EXPECT_EQ(thereIsAction, true);

		constexpr auto noActionVoid = std::is_same_v<void, getAction_t<transitionNoAction>>;
		EXPECT_EQ(noActionVoid, true);
	}

    TEST(TransitionTraitsTests, hasAction)
	{
		using transition = Transition<StateA, EventA, StateB, dummyCallableStruct3Args>;
		using transitionNoAction = Transition<StateA, EventB, StateB>;

		EXPECT_EQ(hasAction<transition>::value, true);
		EXPECT_EQ(hasAction_v<transition>, true);	
		EXPECT_EQ(hasAction<transitionNoAction>::value, false);
		EXPECT_EQ(hasAction_v<transitionNoAction>, false);
	}

    TEST(TransitionTraitsTests, hasGuard)
	{
		using transition = Transition<StateA, EventA, StateB, dummyCallableStruct3Args, dummyCallableStruct2Args>;
		using transitionNoGuard = Transition<StateA, EventB, StateB>;

		EXPECT_EQ(hasGuard<transition>::value, true);
		EXPECT_EQ(hasGuard_v<transition>, true);
		EXPECT_EQ(hasGuard<transitionNoGuard>::value, false);
		EXPECT_EQ(hasGuard_v<transitionNoGuard>, false);
	}

    TEST(TransitionTraitsTests, getGuard)
	{
		using transition = Transition<StateA, EventA, StateB, dummyCallableStruct3Args, dummyCallableStruct2Args>;
		using transitionNoGuard = Transition<StateA, EventB, StateB>;

		constexpr auto is_same_guard_type = std::is_same_v<dummyCallableStruct2Args, getGuard_t<transition>>;
		EXPECT_EQ(is_same_guard_type, true);

		constexpr auto noGuardVoid = std::is_same_v<void, getGuard_t<transitionNoGuard>>;
		EXPECT_EQ(noGuardVoid, true);
	}

    TEST(TransitionTraitsTests, isAutomaticTransition_Valid)
    {
        using transition = Transition<StateA, AUTOMATIC_TRANSITION, StateB>;

        EXPECT_EQ(isAutomaticTransition<transition>::value, true);
        EXPECT_EQ(isAutomaticTransition_v<transition>, true);
    }

    TEST(TransitionTraitsTests, isAutomaticTransition_Invalid)
    {
        using transition = Transition<StateA, EventA, StateB>;

        EXPECT_EQ(isAutomaticTransition<transition>::value, false);
        EXPECT_EQ(isAutomaticTransition_v<transition>, false);
    }
}