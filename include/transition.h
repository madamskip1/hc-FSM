#pragma once
#include <type_traits>

namespace FSM
{
	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType>
	struct Transition
	{
		// Maybe in future there will be point in forcing States to derive from FSM::State
		/*static_assert(std::is_base_of_v<FSM::State, BeforeState>, "BeforeState must derive from FSM::State");
		static_assert(std::is_base_of_v<FSM::Event, EventTrigger>, "EventTrigger must derive from FSM::Event");
		static_assert(std::is_base_of_v<FSM::State, NextState>, "NextState must derive from FSM::State"); */
		
		using before_state_type = BeforeStateType;
		using event_type = EventTriggerType;
		using next_state_type = NextStateType;
	};

	struct NoValidTransition {};
}