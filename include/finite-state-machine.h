#pragma once
#include <type_traits>
#include <tuple>
#include <variant>
#include "transitions-table.h"
#include "state.h"
#include <typeindex>
#include <typeinfo>

namespace FSM
{
	template <typename Transitions_Table, typename InitialState = void>
	class StateMachine;

	template <typename StatesTuple>
	struct variantTypeFromStatesTuple;

	template <typename ...States>
	struct variantTypeFromStatesTuple<std::tuple<States...>>
	{
		using type = std::variant<States...>;
	};

	template <typename StatesTuple>
	using variantTypeFromStatesTuple_t = typename variantTypeFromStatesTuple<StatesTuple>::type;

	template <typename T>
	struct isStateMachine : std::false_type {};

	template <typename Machine>
	struct isStateMachine<StateMachine<Machine>> : std::true_type {};

	template <typename T>
	inline constexpr bool isStateMachine_v = isStateMachine<T>::value;


	enum class HandleEventResult
	{
		PROCESSED,
		PROCESSED_SAME_STATE, // Event processed, but there is no need to change state, for e.g. StateA (EventA) -> StateA
		PROCESSED_INNER_STATE_MACHINE,
		NO_VALID_TRANSITION,
		EXIT_INNER_STATE_MACHINE
	};

	template <typename Transitions_Table, typename InitialState>
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
			statesVariant.template emplace<NewState>();
		};

		template <typename State>
		constexpr bool isInState()
		{
			return std::holds_alternative<State>(statesVariant);
		}

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr bool isInState()
		{
			if constexpr (isStateMachine_v<State>)
			{
				if  (std::holds_alternative<State>(statesVariant))
				{
					return std::get<State>(statesVariant).template isInState<InnerState, InnerStates...>();
				}
			}
			return false;
		}

		template <typename EventTriggerType>
		HandleEventResult handleEvent()
		{
			return handleEvent_impl(EventTriggerType{});
		}

		template <typename EventTriggerType>
		HandleEventResult handleEvent(const EventTriggerType& event)
		{
			return handleEvent_impl(event);
		}


	private:
		states_variant_type statesVariant;

		template <typename EventTriggerType>
		constexpr HandleEventResult handleEvent_impl(const EventTriggerType& event)
		{
			auto lambda = [this, &event](auto& curState) -> HandleEventResult
				{
					
					using cur_state_type = std::decay_t<decltype(curState)>;

					if constexpr (isStateMachine<cur_state_type>::value)
					{
						std::cout << "Wchodzimy niżej\n";
						auto innerTransitionResult = curState.handleEvent(event);
						if (innerTransitionResult == HandleEventResult::NO_VALID_TRANSITION)
						{
							std::cout << "No valid transition\n";
							return HandleEventResult::NO_VALID_TRANSITION;
						}
						else if (innerTransitionResult == HandleEventResult::PROCESSED ||
							innerTransitionResult == HandleEventResult::PROCESSED_SAME_STATE ||
							innerTransitionResult == HandleEventResult::PROCESSED_INNER_STATE_MACHINE)
						{
							std::cout << "Processed inner state machine\n";
							return HandleEventResult::PROCESSED_INNER_STATE_MACHINE;
						}
					}
					std::cout << "Wchodzimy w transit\n";
					using next_state_type = getNextStateFromTransitionsTable_t<transitions_table, cur_state_type, EventTriggerType>;
					return transit<next_state_type>(curState, event);
				};

			return std::visit(lambda, statesVariant);
		}

		template <typename NextStateType, typename CurrentStateType, typename EventTriggerType>
		constexpr HandleEventResult transit(CurrentStateType& currentState, const EventTriggerType& event)
		{
			std::cout << "Current " << std::type_index(typeid(CurrentStateType)).name() << "\n";
			std::cout << "nextType: " << std::type_index(typeid(NextStateType)).name() << "\n";
			if constexpr (std::is_same_v<std::decay_t<CurrentStateType>, NextStateType>)
			{
				// TODO: Transition function
				return HandleEventResult::PROCESSED_SAME_STATE;
			}
			else if constexpr (std::is_same_v<NextStateType, ExitState>)
			{
				tryCallOnExit(currentState, event);
				return HandleEventResult::EXIT_INNER_STATE_MACHINE;
			}
			else if constexpr (!std::is_same_v<NextStateType, FSM::NoValidTransition>)
			{
				tryCallOnExit(currentState, event);
				statesVariant.template emplace<NextStateType>();
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