#pragma once
#include <type_traits>

namespace FSM
{
	struct NO_VALID_TRANSITION {};
	struct AUTOMATIC_TRANSITION {};

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Action = void>
	struct Transition
	{		
		using before_state_type = BeforeStateType;
		using event_type = EventTriggerType;
		using next_state_type = NextStateType;
		using action_type = Action;
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

	template <typename TransitionType>
	struct getAction
	{
		using type = typename TransitionType::action_type;
	};

	template <typename TransitionType>
	using getAction_t = typename getAction<TransitionType>::type;

	template <typename TransitionType>
	struct hasAction : std::is_invocable<getAction_t<TransitionType>> {};

	template <typename TransitionType>
	static constexpr bool hasAction_v = hasAction<TransitionType>::value;
}