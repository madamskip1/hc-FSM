#pragma once
namespace FSM
{
	template <typename BeforeState, typename EventTrigger, typename NextState>
	struct Transition
	{
		// Maybe in future there will be point in forcing States to derive from FSM::State
		/*static_assert(std::is_base_of_v<FSM::State, BeforeState>, "BeforeState must derive from FSM::State");
		static_assert(std::is_base_of_v<FSM::Event, EventTrigger>, "EventTrigger must derive from FSM::Event");
		static_assert(std::is_base_of_v<FSM::State, NextState>, "NextState must derive from FSM::State"); */
		
		using before_state_type = BeforeState;
		using event_type = EventTrigger;
		using next_state_type = NextState;
	};
}