#pragma once
#include "Transition.h"

namespace FSM
{
	// TransitionsTable to hold variadic transitions
		template <typename... T>
		struct TransitionsTable {
			using transitions = std::tuple<T...>;
		};
	

	template <typename Transitions_Table, typename BeforeState, typename EventTrigger>
	struct hasTransition
	{
		template <typename TransitionType>
		static constexpr bool MatchesTransition() {
			return std::is_same<typename TransitionType::before_state_type, BeforeState>::value
				&& std::is_same<typename TransitionType::event_type, EventTrigger>::value;
		}

		template<typename ...Transitions>
		struct hasTransition_impl;

		template <>
		struct hasTransition_impl<> : std::false_type {};

		template<typename Transition, typename... RestTransitions>
		struct hasTransition_impl<Transition, RestTransitions...>
		{
			static constexpr bool value =
				MatchesTransition<Transition>() || hasTransition_impl<RestTransitions...>::value;
		};

		template <typename... TransitionTypes>
		struct hasTransition_impl<std::tuple<TransitionTypes...>> {
			static constexpr bool value =
				(MatchesTransition<TransitionTypes>() || ...);
		};

		static constexpr bool value = hasTransition_impl<typename Transitions_Table::transitions>::value;
	};

	template <typename Transitions, typename BeforeState, typename EventTrigger>
	struct getNextStateFromTransition_impl;

	template <typename BeforeState, typename EventTrigger, typename Head, typename... Tail>
	struct getNextStateFromTransition_impl<std::tuple<Head, Tail...>, BeforeState, EventTrigger> {
		using type = std::conditional_t<
			std::is_same_v<BeforeState, typename Head::before_state_type>&&
			std::is_same_v<EventTrigger, typename Head::event_type>,
			typename Head::next_state_type,
			typename getNextStateFromTransition_impl<std::tuple<Tail...>, BeforeState, EventTrigger>::type
		>;
	};


	template <typename BeforeState, typename EventTrigger>
	struct getNextStateFromTransition_impl<std::tuple<>, BeforeState, EventTrigger> {
		using type = void; // Not found
	};

	template <typename Transitions_Table, typename BeforeState, typename EventTrigger>
	struct getNextStateFromTransition {
		using type = typename getNextStateFromTransition_impl<typename Transitions_Table::transitions, BeforeState, EventTrigger>::type;
	};
	
}