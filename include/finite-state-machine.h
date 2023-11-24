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

	template <>
	struct variantTypeFromStatesTuple<std::tuple<>>
	{
		using type = std::variant<>;
	};

	template <typename ...States>
	struct variantTypeFromStatesTuple<std::tuple<States...>>
	{
		using type = std::variant<std::monostate, States...>;
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
		EXIT_INNER_STATE_MACHINE,
		EXIT_AUTOMATIC_INNER_STATE_MACHINE
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

		constexpr StateMachine() 
		{
			statesVariant.template emplace<initial_state_type>();
		};

		template <typename NewState>
		constexpr void forceTransition()
		{
			statesVariant.template emplace<NewState>();
		};

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr void forceTransition()
		{
			std::get<State>(statesVariant).template forceTransition<InnerState, InnerStates...>();
		}

		template <typename State>
		constexpr bool isInState() const
		{
			return std::holds_alternative<State>(statesVariant);
		}

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr bool isInState() const
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

		template <typename State>
		constexpr State& getState()
		{
			return std::get<State>(statesVariant);
		}

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr auto getState()
		{
			return std::get<State>(statesVariant).template getState<InnerState, InnerStates...>();
		}

		template <typename EventTriggerType>
		constexpr HandleEventResult handleEvent()
		{
			return handleEvent_impl(EventTriggerType{});
		}

		template <typename EventTriggerType>
		constexpr HandleEventResult handleEvent(const EventTriggerType& event)
		{
			return handleEvent_impl(event);
		}

	private:
		states_variant_type statesVariant;

		template <typename EventTriggerType>
		constexpr HandleEventResult handleEvent_impl(const EventTriggerType& event)
		{
			const auto lambda = [this, &event](auto& currentState) -> HandleEventResult
				{
					using cur_state_type = std::decay_t<decltype(currentState)>;

					if constexpr (isStateMachine_v<cur_state_type>)
					{
						const auto innerTransitResult = innerStateMachineTransition(currentState, event);

						if (innerTransitResult == HandleEventResult::EXIT_AUTOMATIC_INNER_STATE_MACHINE)
						{
							return normalTransition(currentState, AUTOMATIC_TRANSITION{});
						}
						else if (innerTransitResult != HandleEventResult::EXIT_INNER_STATE_MACHINE)
						{
							return innerTransitResult;
						}
					}
					
					return normalTransition(currentState, event);
				};

			return std::visit(lambda, statesVariant);
		}

		template <typename Transition, typename CurrentStateType, typename EventTriggerType>
		constexpr HandleEventResult transit(CurrentStateType& currentState, const EventTriggerType& event)
		{
			if constexpr (!isValidTransition_v<Transition>)
			{
				return HandleEventResult::NO_VALID_TRANSITION;
			}
			else
			{
				using next_state_type = getNextState_t<Transition>;

				if constexpr (std::is_same_v<std::decay_t<CurrentStateType>, next_state_type>)
				{
					tryCallTransitionAction<Transition>(currentState, event, currentState);
					return HandleEventResult::PROCESSED_SAME_STATE;
				}
				else if constexpr (std::is_same_v<next_state_type, ExitState>)
				{
					tryCallOnExit(currentState, event);
					return HandleEventResult::EXIT_INNER_STATE_MACHINE;
				}
				else
				{
					tryCallOnExit(currentState, event);
					next_state_type nextState;
					tryCallTransitionAction<Transition>(currentState, event, nextState);
					tryCallOnEntry(nextState, event);
					
					statesVariant = std::move(nextState);
					return HandleEventResult::PROCESSED;
				}
			}
		}

		template <typename InnerStateMachineType, typename EventTriggerType>
		constexpr HandleEventResult innerStateMachineTransition(InnerStateMachineType& innerStateMachine, const EventTriggerType& event) const
		{
			const auto innerTransitionResult = innerStateMachine.handleEvent(event);
			if (innerTransitionResult == HandleEventResult::PROCESSED ||
				innerTransitionResult == HandleEventResult::PROCESSED_SAME_STATE ||
				innerTransitionResult == HandleEventResult::PROCESSED_INNER_STATE_MACHINE)
			{
				return HandleEventResult::PROCESSED_INNER_STATE_MACHINE;
			}
			
			return innerTransitionResult;
		}

		template <typename CurrentStateType, typename EventTriggerType>
		constexpr HandleEventResult normalTransition(CurrentStateType& currentState, const EventTriggerType& event)
		{
			using transition = getTransition_t<transitions_table, CurrentStateType, EventTriggerType>;			
			const auto transitResult = transit<transition>(currentState, event);

			if constexpr (isValidTransition_v<transition>)
			{
				using next_state_type = getNextState_t<transition>;
				if constexpr (!isStateMachine_v<next_state_type> && hasAutomaticTransition_v<transitions_table, next_state_type>)
				{
					return tryAutomaticTransition(std::get<next_state_type>(statesVariant));
				}
			}

			return transitResult;
		}

		template <typename CurrentStateType>
		constexpr HandleEventResult tryAutomaticTransition(CurrentStateType& currentState)
		{
			if constexpr (hasAutomaticTransition_v<transitions_table, CurrentStateType>)
			{
				using transition = getTransition_t<transitions_table, CurrentStateType, AUTOMATIC_TRANSITION>;
				using next_state_type = getNextState_t<transition>;

				if constexpr (std::is_same_v<next_state_type, ExitState>)
				{
					tryCallOnExit(currentState, AUTOMATIC_TRANSITION{});
					return HandleEventResult::EXIT_AUTOMATIC_INNER_STATE_MACHINE;
				}
				
				const auto transitResult = transit<transition>(currentState, AUTOMATIC_TRANSITION{});
				
				if (auto nextAutomaticTransitionsResult = tryAutomaticTransition(std::get<next_state_type>(statesVariant));
					nextAutomaticTransitionsResult != HandleEventResult::NO_VALID_TRANSITION)
				{
					return nextAutomaticTransitionsResult;
				}
				return transitResult;
			}
			else
			{
				return HandleEventResult::NO_VALID_TRANSITION;
			}
		}

		template <typename StateType, typename EventTriggerType>
		constexpr void tryCallOnExit(StateType& state, const EventTriggerType& event) const
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
		constexpr void tryCallOnEntry(StateType& state, const EventTriggerType& event) const
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

		template <typename Transition, typename StateBeforeType, typename EventTriggerType, typename StateAfterType>
		constexpr void tryCallTransitionAction(StateBeforeType& stateBefore, const EventTriggerType& event, StateAfterType& stateAfter) const
		{
			if constexpr (hasAction_v<Transition>)
			{
				getAction_t<Transition> {} (stateBefore, event, stateAfter);
			}
		}
	};
}