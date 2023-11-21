#pragma once
#include <type_traits>
#include <tuple>
#include "transition.h"

namespace FSM
{
	template <typename... T>
	struct TransitionsTable
	{
		using transitions = std::tuple<T...>;
	};


	template <typename TransitionTuple, typename BeforeStateType, typename EventTriggerType>
	static constexpr bool doTransitionMatch()
	{
		return std::conjunction_v<
			std::is_same<typename TransitionTuple::before_state_type, BeforeStateType>,
			std::is_same<typename TransitionTuple::event_type, EventTriggerType>
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

		static constexpr bool value = hasTransition_impl<typename Transitions_Table::transitions>::value;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	static constexpr bool hasTransition_v = hasTransition<Transitions_Table, BeforeStateType, EventTriggerType>::value;

	// ~hasTransition

	// getNextStateFromTransitionsTable

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	struct getNextStateFromTransitionsTable
	{

		template <typename ...Transitions>
		struct getNextStateFromTransitionsTable_impl;

		template <typename LastTransition>
		struct getNextStateFromTransitionsTable_impl<std::tuple<LastTransition>>
		{
			using type = std::conditional_t<
				doTransitionMatch<LastTransition, BeforeStateType, EventTriggerType>(),
				typename LastTransition::next_state_type,
				NO_VALID_TRANSITION
			>;
		};

		template <typename Transition, typename ...RestTransitions>
		struct getNextStateFromTransitionsTable_impl<std::tuple<Transition, RestTransitions...>>
		{
			using type = std::conditional_t<
				doTransitionMatch<Transition, BeforeStateType, EventTriggerType>(),
				typename Transition::next_state_type,
				typename getNextStateFromTransitionsTable_impl<std::tuple<RestTransitions...>>::type
			>;
		};

		using type = typename getNextStateFromTransitionsTable_impl<typename Transitions_Table::transitions>::type;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	using getNextStateFromTransitionsTable_t = typename getNextStateFromTransitionsTable<Transitions_Table, BeforeStateType, EventTriggerType>::type;

	// ~getNextStateFromTransitionsTable
	
	// getStatesFromTransitionsTable

	template <typename Transitions_Table>
	struct getStatesFromTransitionsTable
	{
		template <typename T, typename ...Types>
		struct isInTypes
		{
			static constexpr bool value = std::disjunction_v<std::is_same<T, Types>...>;
		};


		template<typename T, typename U>
		struct addTypeToTupleIfNotOccured;

		template <typename T, typename ...Types>
		struct addTypeToTupleIfNotOccured<T, std::tuple<Types...>>
		{
			using type = typename std::conditional_t<
				isInTypes<T, Types...>::value,
				std::tuple<Types...>,
				std::tuple<Types..., T>
			>;
		};


		template <typename ...Transitions>
		struct getStatesFromTransitionsTable_impl;

		template <typename StatesTuple, typename Transition>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition>>
		{
			using before_state = typename Transition::before_state_type;
			using next_state = typename Transition::next_state_type;

			using type = typename addTypeToTupleIfNotOccured<next_state, typename addTypeToTupleIfNotOccured<before_state, StatesTuple>::type>::type;
		};

		template <typename StatesTuple, typename Transition, typename ...RestTransitions>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition, RestTransitions...>>
		{
			using before_state = typename Transition::before_state_type;
			using next_state = typename Transition::next_state_type;

			using with_before_state = typename addTypeToTupleIfNotOccured<before_state, StatesTuple>::type;
			using with_before_after_state = typename addTypeToTupleIfNotOccured<next_state, with_before_state>::type;

			using type = typename getStatesFromTransitionsTable_impl<with_before_after_state, std::tuple<RestTransitions...>>::type;
		};

		using states_tuple_type = typename getStatesFromTransitionsTable_impl<typename std::tuple<>, typename Transitions_Table::transitions>::type;
	};

	template <typename Transitions_Table>
	using getStatesFromTransitionsTable_t = typename getStatesFromTransitionsTable<Transitions_Table>::states_tuple_type;

	// ~getStatesFromTransitionsTable
}