#pragma once
#include "Event.h"
#include "State.h"
namespace FSM
{
	

	template <typename BeforeState, typename EventTrigger, typename NextState>
	struct Transition
	{
		static_assert(std::is_base_of_v<FSM::State, BeforeState>, "BeforeState must derive from FSM::State");
		static_assert(std::is_base_of_v<FSM::Event, EventTrigger>, "EventTrigger must derive from FSM::Event");
		static_assert(std::is_base_of_v<FSM::State, NextState>, "NextState must derive from FSM::State");

		using before_state_type = BeforeState;
		using event_type = EventTrigger;
		using next_state_type = NextState;
	};

	namespace Traits
	{
		// Helper struct to check if a type is a Transition
		template <typename T>
		struct is_transition : std::false_type {};

		template <typename BeforeState, typename EventTrigger, typename NextState>
		struct is_transition<FSM::Transition<BeforeState, EventTrigger, NextState>> : std::true_type {};

	}
}

