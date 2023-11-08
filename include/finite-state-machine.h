#pragma once
#include <type_traits>
#include <tuple>
#include <variant>
#include "transitions-table.h"

namespace FSM
{
	template <typename StatesTuple>
	struct variantTypeFromStatesTuple;

	template <typename ...States>
	struct variantTypeFromStatesTuple<std::tuple<States...>>
	{
		using type = std::variant<States...>;
	};

	template <typename StatesTuple>
	using variantTypeFromStatesTuple_t = typename variantTypeFromStatesTuple<StatesTuple>::type;

	enum class HandleEventResult
	{
		PROCESSED,
		PROCESSED_SAME_STATE, // Event processed, but there is no need to change state, for e.g. StateA (EventA) -> StateA
		NO_VALID_TRANSITION
	};

	template <typename Transitions_Table, typename InitialState = void>
	class StateMachine
	{
	public:
		using transitions_table = Transitions_Table;
		using states_tuple_type = getStatesFromTransitionsTable_t<Transitions_Table>;
		using states_variant_type = variantTypeFromStatesTuple_t<states_tuple_type>;
		using initial_state_type = std::conditional_t<
			std::is_same_v<InitialState, void>,
			std::tuple_element_t<0, states_tuple_type>,
			InitialState
		>;

		StateMachine() : statesVariant{ initial_state_type{} } {};

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

		template <typename EventTriggerType>
		void handleEvent()
		{
			handleEvent_impl(EventTriggerType{});
		}

		template <typename EventTriggerType>
		void handleEvent(const EventTriggerType& event)
		{
			handleEvent_impl(event);
		}


	private:
		states_variant_type statesVariant;

		template <typename EventTriggerType>
		constexpr HandleEventResult handleEvent_impl(const EventTriggerType& event)
		{
			auto lambda = [this, &event](const auto& curState) -> HandleEventResult
				{
					using cur_state_type = std::decay_t<decltype(curState)>;
					using next_state_type = getNextStateFromTransitionsTable_t<transitions_table, cur_state_type, EventTriggerType>;

					return transit<next_state_type>(curState, event);
				};

			return std::visit(lambda, statesVariant);
		}

		template <typename NextStateType, typename CurrentStateType, typename EventTriggerType>
		constexpr HandleEventResult transit(CurrentStateType& currentState, const EventTriggerType& event)
		{
			if constexpr (std::is_same_v<CurrentStateType, NextStateType>)
			{
				// TODO: Transition function
				return HandleEventResult::PROCESSED_SAME_STATE;
			}
			else if constexpr (!std::is_same_v<NextStateType, FSM::NoValidTransition>)
			{
				tryCallOnExit(currentState, event);
				statesVariant.emplace<NextStateType>();
				NextStateType& nextState = std::get<NextStateType>(statesVariant);
				// TODO: Transition function
				tryCallOnEntry(nextState, event);
				
				return HandleEventResult::PROCESSED;
			}
			else
			{
				return HandleEventResult::NO_VALID_TRANSITION;
			}
		}

		template <typename StateType, typename EventTriggerType>
		constexpr void tryCallOnExit(StateType& state, const EventTriggerType& event)
		{
			if constexpr (has_onExit_v<StateType, EventTriggerType>)
			{
				state.onExit(event);
			}
			else if constexpr (has_onExitNoEventArg_v<StateType>)
			{
				state.onExit();
			}
		}

		template <typename StateType, typename EventTriggerType>
		constexpr void tryCallOnEntry(StateType& state, const EventTriggerType& event)
		{
			if constexpr (has_onEntry_v<StateType, EventTriggerType>)
			{
				state.onEntry(event);
			}
			else if constexpr (has_onEntryNoEventArg_v<StateType>)
			{
				state.onEntry();
			}
		}
	};
}
