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
	struct hasTransition : std::false_type
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

}