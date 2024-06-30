#include <gtest/gtest.h>
#include "hcFSM/detail/transition.h"
#include "hcFSM/detail/transitions-table.h"

struct StateA {};
struct StateB  {};

struct EventA {};
struct EventB {};

namespace
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
		using transition = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr auto is_valid_transition = hcFSM::isValidTransition<transition>::value;
		constexpr auto is_valid_transition_v = hcFSM::isValidTransition_v<transition>;
		EXPECT_EQ(is_valid_transition, true);
		EXPECT_EQ(is_valid_transition_v, true);
	}

    TEST(TransitionTraitsTests, getBeforeState)
	{
		using transition = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_before_state_type = std::is_same_v<StateA, hcFSM::getBeforeState_t<transition>>;
		EXPECT_EQ(is_same_transition_before_state_type, true);
	}

    TEST(TransitionTraitsTests, getNextState)
	{
		using transition = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_next_state_type = std::is_same_v<StateB, hcFSM::getNextState_t<transition>>;
		EXPECT_EQ(is_same_transition_next_state_type, true);
	}

    TEST(TransitionTraitsTests, getEvent)
	{
		using transition = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr auto is_same_transition_event_type = std::is_same_v<EventB, hcFSM::getEvent_t<transition>>;
		EXPECT_EQ(is_same_transition_event_type, true);
	}

    TEST(TransitionTraitsTests, getAction)
	{
		using transition = hcFSM::Transition<StateA, EventA, StateB, dummyCallableStruct3Args>;
		using transitionNoAction = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr bool thereIsAction = std::is_same_v<dummyCallableStruct3Args, hcFSM::getAction_t<transition>>;
		EXPECT_EQ(thereIsAction, true);

		constexpr auto noActionVoid = std::is_same_v<void, hcFSM::getAction_t<transitionNoAction>>;
		EXPECT_EQ(noActionVoid, true);
	}

    TEST(TransitionTraitsTests, hasAction)
	{
		using transition = hcFSM::Transition<StateA, EventA, StateB, dummyCallableStruct3Args>;
		using transitionNoAction = hcFSM::Transition<StateA, EventB, StateB>;

		EXPECT_EQ(hcFSM::hasAction<transition>::value, true);
		EXPECT_EQ(hcFSM::hasAction_v<transition>, true);	
		EXPECT_EQ(hcFSM::hasAction<transitionNoAction>::value, false);
		EXPECT_EQ(hcFSM::hasAction_v<transitionNoAction>, false);
	}

    TEST(TransitionTraitsTests, hasGuard)
	{
		using transition = hcFSM::Transition<StateA, EventA, StateB, dummyCallableStruct3Args, dummyCallableStruct2Args>;
		using transitionNoGuard = hcFSM::Transition<StateA, EventB, StateB>;

		EXPECT_EQ(hcFSM::hasGuard<transition>::value, true);
		EXPECT_EQ(hcFSM::hasGuard_v<transition>, true);
		EXPECT_EQ(hcFSM::hasGuard<transitionNoGuard>::value, false);
		EXPECT_EQ(hcFSM::hasGuard_v<transitionNoGuard>, false);
	}

    TEST(TransitionTraitsTests, getGuard)
	{
		using transition = hcFSM::Transition<StateA, EventA, StateB, dummyCallableStruct3Args, dummyCallableStruct2Args>;
		using transitionNoGuard = hcFSM::Transition<StateA, EventB, StateB>;

		constexpr auto is_same_guard_type = std::is_same_v<dummyCallableStruct2Args, hcFSM::getGuard_t<transition>>;
		EXPECT_EQ(is_same_guard_type, true);

		constexpr auto noGuardVoid = std::is_same_v<void, hcFSM::getGuard_t<transitionNoGuard>>;
		EXPECT_EQ(noGuardVoid, true);
	}

    TEST(TransitionTraitsTests, isAutomaticTransition_Valid)
    {
        using transition = hcFSM::Transition<StateA, hcFSM::AUTOMATIC_TRANSITION, StateB>;

        EXPECT_EQ(hcFSM::isAutomaticTransition<transition>::value, true);
        EXPECT_EQ(hcFSM::isAutomaticTransition_v<transition>, true);
    }

    TEST(TransitionTraitsTests, isAutomaticTransition_Invalid)
    {
        using transition = hcFSM::Transition<StateA, EventA, StateB>;

        EXPECT_EQ(hcFSM::isAutomaticTransition<transition>::value, false);
        EXPECT_EQ(hcFSM::isAutomaticTransition_v<transition>, false);
    }
}