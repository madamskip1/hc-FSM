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

		static constexpr bool value = hasTransition_impl<typename Transitions_Table::transitions>::value;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	static constexpr bool hasTransition_v = hasTransition<Transitions_Table, BeforeStateType, EventTriggerType>::value;

	// ~hasTransition

	// hasAutomaticTransition

	template <typename Transitions_Table, typename BeforeStateType>
	struct hasAutomaticTransition
	{
		static constexpr bool value = hasTransition_v<Transitions_Table, BeforeStateType, AUTOMATIC_TRANSITION>;
	};
	
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

		using type = typename getTransition_impl<typename Transitions_Table::transitions>::type;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	using getTransition_t = typename getTransition<Transitions_Table, BeforeStateType, EventTriggerType>::type;

	// ~getTransition

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
			using before_state = getBeforeState_t<Transition>;
			using next_state = getNextState_t<Transition>;

			using type = typename addTypeToTupleIfNotOccured<next_state, typename addTypeToTupleIfNotOccured<before_state, StatesTuple>::type>::type;
		};

		template <typename StatesTuple, typename Transition, typename ...RestTransitions>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition, RestTransitions...>>
		{
			using before_state = getBeforeState_t<Transition>;
			using next_state = getNextState_t<Transition>;

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