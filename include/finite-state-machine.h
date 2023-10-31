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

		StateMachine() : statesVariant{ initial_state_type{} } {};

	private:
		states_variant_type statesVariant;
	};

	
}