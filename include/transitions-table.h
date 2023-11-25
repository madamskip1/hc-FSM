#pragma once
#include <type_traits>
#include <tuple>
#include "transition.h"

namespace FSM
{
		template <typename... Transitions>
		struct TransitionsTable
		{
			static_assert(sizeof...(Transitions) >= 1, "TransitionsTable must have at least one transition");
			static_assert((isValidTransition_v<Transitions> && ...), "TransitionsTable must have only valid transitions. All must be Transition<...> types");
			
			using transitions = std::tuple<Transitions...>;
		};

	template <typename Transitions_Table>
	struct getTransitions
	{
		using type = typename Transitions_Table::transitions;
	};

	template <typename Transitions_Table>
	using getTransitions_t = typename getTransitions<Transitions_Table>::type;


	template <typename Transition, typename BeforeStateType, typename EventTriggerType>
	static constexpr bool doTransitionMatch()
	{
		return std::conjunction_v<
			std::is_same<getBeforeState_t<Transition>, BeforeStateType>,
			std::is_same<getEvent_t<Transition>, EventTriggerType>
		>;
	}

	//  hasTransition

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	struct hasTransition
	{
		template <typename ...Transitions>
		struct hasTransition_impl;

		template <typename ...Transitions>
		struct hasTransition_impl<std::tuple<Transitions...>>
		{
			static constexpr bool value = (doTransitionMatch<Transitions, BeforeStateType, EventTriggerType>() || ...);
		};

		static constexpr bool value = hasTransition_impl<getTransitions_t<Transitions_Table>>::value;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	static constexpr bool hasTransition_v = hasTransition<Transitions_Table, BeforeStateType, EventTriggerType>::value;

	// ~hasTransition

	// hasAutomaticTransition

	template <typename Transitions_Table, typename BeforeStateType>
	struct hasAutomaticTransition : hasTransition<Transitions_Table, BeforeStateType, AUTOMATIC_TRANSITION> {};
	
	template <typename Transitions_Table, typename BeforeStateType>
	static constexpr bool hasAutomaticTransition_v = hasAutomaticTransition<Transitions_Table, BeforeStateType>::value;

	// ~hasAutomaticTransition

	// getTransition

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	struct getTransition
	{
		template <typename ...Transitions>
		struct getTransition_impl;

		template <typename Transition>
		struct getTransition_impl<std::tuple<Transition>>
		{
			using type = std::conditional_t<
				doTransitionMatch<Transition, BeforeStateType, EventTriggerType>(),
				Transition,
				NO_VALID_TRANSITION
			>;
		};

		template <typename Transition, typename ...RestTransitions>
		struct getTransition_impl<std::tuple<Transition, RestTransitions...>>
		{
			using type = std::conditional_t<
				doTransitionMatch<Transition, BeforeStateType, EventTriggerType>(),
				Transition,
				typename getTransition_impl<std::tuple<RestTransitions...>>::type
			>;
		};

		using type = typename getTransition_impl<getTransitions_t<Transitions_Table>>::type;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	using getTransition_t = typename getTransition<Transitions_Table, BeforeStateType, EventTriggerType>::type;

	// ~getTransition

	// getStatesFromTransitionsTable

	template <typename Transitions_Table>
	struct getStatesFromTransitionsTable
	{
		template <typename Type, typename ...Types>
		struct isInTypes
		{
			static constexpr bool value = std::disjunction_v<std::is_same<Type, Types>...>;
		};

		template<typename Type, typename Types>
		struct addTypeToTupleIfNotOccured;

		template <typename Type, typename ...Types>
		struct addTypeToTupleIfNotOccured<Type, std::tuple<Types...>>
		{
			using type = typename std::conditional_t<
				isInTypes<Type, Types...>::value,
				std::tuple<Types...>,
				std::tuple<Types..., Type>
			>;
		};

		template <typename ...Transitions>
		struct getStatesFromTransitionsTable_impl;

		template <typename StatesTuple, typename Transition>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition>>
		{
			using type = typename addTypeToTupleIfNotOccured<getNextState_t<Transition>, typename addTypeToTupleIfNotOccured<getBeforeState_t<Transition>, StatesTuple>::type>::type;
		};

		template <typename StatesTuple, typename Transition, typename ...RestTransitions>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition, RestTransitions...>>
		{
			using states_tuple_with_this_transition = typename addTypeToTupleIfNotOccured<getNextState_t<Transition>, typename addTypeToTupleIfNotOccured<getBeforeState_t<Transition>, StatesTuple>::type>::type;

			using type = typename getStatesFromTransitionsTable_impl<states_tuple_with_this_transition, std::tuple<RestTransitions...>>::type;
		};

		using states_tuple_type = typename getStatesFromTransitionsTable_impl<typename std::tuple<>, getTransitions_t<Transitions_Table>>::type;
	};

	template <typename Transitions_Table>
	using getStatesFromTransitionsTable_t = typename getStatesFromTransitionsTable<Transitions_Table>::states_tuple_type;

	// ~getStatesFromTransitionsTable
}