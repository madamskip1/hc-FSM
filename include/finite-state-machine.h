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


	template <typename StateType>
	struct StateTypeHolder
	{
		using type = StateType;
	};


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

		template <typename EventType>
		void handleEvent()
		{
			handleEvent_impl(EventType{});
		}

		template <typename EventType>
		void handleEvent(const EventType& event)
		{
			handleEvent_impl(event);
		}


	private:
		states_variant_type statesVariant;
		//std::type_info statesVariantID;

		template <typename EventType>
		constexpr void handleEvent_impl(const EventType& event)
		{
		/*	auto lambda = [&event](const auto& curState) -> auto
				{
					using curStateType = decltype(curState);
					using 
				};

			auto curStateHolder = std::visit(lambda, statesVariant);*/
			// auto currenState = ...
		//	tryCallOnExit(State, event);

			// auto nextState = ...
		//	tryCallOnEnter(State, event);
		}

		template <typename StateType, typename EventType>
		constexpr void tryCallOnExit(StateType& state, const EventType& event)
		{
			if constexpr (has_onExit_v<StateType, EventType>)
			{
				state.onExit(event);
			}
			else if constexpr (has_onEntryNoEventArg_v<StateType>)
			{
				state.onExit();
			}
		}

		template <typename StateType, typename EventType>
		constexpr void tryCallOnEntry(StateType& state, const EventType& event)
		{
			if constexpr (has_onExit_v<StateType, EventType>)
			{
				state.onEntry(event);
			}
			else if constexpr (has_onExitNoEventArg_v<StateType>)
			{
				state.onEntry();
			}
		}
	};

	
}
