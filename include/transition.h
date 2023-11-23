#pragma once
#include <type_traits>

namespace FSM
{
	struct NO_VALID_TRANSITION {};
	struct AUTOMATIC_TRANSITION {};

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
	
	template <typename Transition>
	struct isValidTransition : std::false_type {};

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType>
	struct isValidTransition<Transition<BeforeStateType, EventTriggerType, NextStateType>> : std::true_type {};

	template <typename Transition>
	static constexpr bool isValidTransition_v = isValidTransition<Transition>::value;

	template <typename TransitionType>
	struct getBeforeState
	{
		using type = typename TransitionType::before_state_type;
	};

	template <typename TransitionType>
	using getBeforeState_t = typename getBeforeState<TransitionType>::type;

	template <typename TransitionType>
	struct getNextState
	{
		using type = typename TransitionType::next_state_type;
	};

	template <typename TransitionType>
	using getNextState_t = typename getNextState<TransitionType>::type;

	template <typename TransitionType>
	struct getEvent
	{
		using type = typename TransitionType::event_type;
	};

	template <typename TransitionType>
	using getEvent_t = typename getEvent<TransitionType>::type;
}