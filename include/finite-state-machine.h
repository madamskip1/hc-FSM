#pragma once
#include <type_traits>
#include <tuple>
#include "transitions-table.h"

namespace FSM
{
	template <typename TransitionsTable, typename InitialState = void>
	class StateMachine
	{
	public:
		using trasitions_table = TransitionsTable;
		using states_types_tuple = FSM::getStatesFromTransitionsTable_t<TransitionsTable>;
		using initial_state = std::conditional_t<
			std::is_same_v<InitialState, void>,
			std::tuple_element_t<0, states_types_tuple>,
			InitialState
		>;

	};
}