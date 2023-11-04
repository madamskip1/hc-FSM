#pragma once
#include <type_traits>
#include <tuple>
#include <variant>
#include "transitions-table.h"
#include <iostream>

namespace FSM
{
	template <typename States>
	struct variantTypeFromStatesTuple;

	template <typename ...States>
	struct variantTypeFromStatesTuple<std::tuple<States...>>
	{
		using type = std::variant<States...>;
	};

	template <typename StatesTuple>
	using variantTypeFromStatesTuple_t = typename variantTypeFromStatesTuple<StatesTuple>::type;


	template <typename StateType, typename EventType, typename = void>
	struct has_onEntry : std::false_type {};

	template <typename StateType, typename EventType>
	struct has_onEntry<StateType, EventType, std::void_t<decltype(std::declval<StateType>().onEntry(std::declval<EventType>()))>> : std::true_type {};

	template <typename StateType, typename EventType>
	inline constexpr bool has_onEntry_v = has_onEntry<StateType, EventType>::value;

	template <typename StateType, typename = void>
	struct has_onEntryNoEventArg : std::false_type {};

	template <typename StateType>
	struct has_onEntryNoEventArg<StateType, std::void_t<decltype(std::declval<StateType>().onEntry())>> : std::true_type {};

	template <typename StateType>
	inline constexpr bool has_onEntryNoEventArg_v = has_onEntryNoEventArg<StateType>::value;



	template <typename TransitionsTable, typename InitialState = void>
	class StateMachine
	{
	public:
		using trasitions_table = TransitionsTable;
		using states_tuple_type = getStatesFromTransitionsTable_t<TransitionsTable>;
		using states_variant_type = variantTypeFromStatesTuple_t<states_tuple_type>;
		using initial_state_type = std::conditional_t<
			std::is_same_v<InitialState, void>,
			std::tuple_element_t<0, states_tuple_type>,
			InitialState
		>;

		StateMachine() : statesVariant{ initial_state_type{} } {};//, statesVariantID{ typeid(initial_state_type)} {};

		template <typename NewState>
		void forceTransition()
		{
			statesVariant.emplace<NewState>();
		};

		template <typename State>
		constexpr bool isInState()
		{
			return std::holds_alternative<State>(statesVariant);
		}



	private:
		states_variant_type statesVariant;
		//std::type_info statesVariantID;
	};

	
}