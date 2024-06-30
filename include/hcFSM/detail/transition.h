#pragma once
#include <type_traits>

namespace hcFSM
{
	struct NO_VALID_TRANSITION {};
	struct AUTOMATIC_TRANSITION {};

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Action = void, typename Guard = void>
	struct Transition
	{
		static_assert(!std::is_same_v<BeforeStateType, void>, "BeforeStateType cannot be void");
		static_assert(!std::is_same_v<EventTriggerType, void>, "EventTriggerType cannot be void");
		static_assert(!std::is_same_v<NextStateType, void>, "NextStateType cannot be void");
		static_assert(std::is_same_v<Action, void> || std::is_invocable_v<Action, BeforeStateType&, EventTriggerType&, NextStateType&>,
			"Action must be void or callable struct with BeforeStateType&, EventTriggerType&, NextStateType&");
		static_assert(std::is_same_v<Guard, void> || std::is_invocable_v<Guard, BeforeStateType&, EventTriggerType&>,
			"Guard must be void or callable struct with BeforeStateType&, EventTriggerType&");
		
		using before_state_type = BeforeStateType;
		using event_type = EventTriggerType;
		using next_state_type = NextStateType;
		using action_type = Action;
		using guard_type = Guard;
	};

	// Specialized Transition types

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Guard = void>
	using TransitionWithGuard = Transition<BeforeStateType, EventTriggerType, NextStateType, void, Guard>;

	template <typename BeforeStateType,typename NextStatetype, typename Action = void, typename Guard = void>
	using TransitionAutomatic = Transition<BeforeStateType, AUTOMATIC_TRANSITION, NextStatetype, Action, Guard>;

	template <typename BeforeStateType, typename NextStatetype, typename Guard = void>
	using TransitionAutomaticWithGuard = Transition<BeforeStateType, AUTOMATIC_TRANSITION, NextStatetype, void, Guard>;

	// ~Specialized Transition types

	// getBeforeState, getNextState, getEvent, getAction, getGuard

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
	struct getGuard
	{
		using type = typename TransitionType::guard_type;
	};

	template <typename TransitionType>
	using getGuard_t = typename getGuard<TransitionType>::type;

	// ~getBeforeState, getNextState, getEvent, getAction, getGuard

	// hasAction, hasGuard

	template <typename TransitionType>
	struct hasAction : std::is_invocable<getAction_t<TransitionType>, getBeforeState_t<TransitionType>&, getEvent_t<TransitionType>&, getNextState_t<TransitionType>&> {};

	template <typename TransitionType>
	static constexpr bool hasAction_v = hasAction<TransitionType>::value;

	template <typename TransitionType>
	struct hasGuard : std::is_invocable<getGuard_t<TransitionType>, getBeforeState_t<TransitionType>&, getEvent_t<TransitionType>&> {};

	template <typename TransitionType>
	static constexpr bool hasGuard_v = hasGuard<TransitionType>::value;

	// ~hasAction, hasGuard


	// isValidTransition
	
	template <typename Transition>
	struct isValidTransition : std::false_type {};

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Action, typename Guard>
	struct isValidTransition<Transition<BeforeStateType, EventTriggerType, NextStateType, Action, Guard>> : std::true_type {};

	template <typename Transition>
	static constexpr bool isValidTransition_v = isValidTransition<Transition>::value;

	// ~isValidTransition

	// isAutomaticTransition

	template <typename Transition>
	struct isAutomaticTransition : std::is_same<getEvent_t<Transition>, AUTOMATIC_TRANSITION> {};

	template <typename Transition>
	static constexpr bool isAutomaticTransition_v = isAutomaticTransition<Transition>::value;

	// ~isAutomaticTransition
}