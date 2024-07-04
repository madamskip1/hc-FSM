#pragma once
#include <type_traits>
#include <tuple>
#include <variant>
#include "state-machine-fwd.h"
#include "transitions-table.h"
#include "state.h"
#include "fsm-traits.h"
#include "handle-event-result.h"

namespace hcFSM
{
	template <typename Transitions_Table, typename InitialState, bool CallOnEntryOnInitialState>
	class StateMachine
	{
		static_assert(isValidTransitionsTable_v<Transitions_Table>, "Transitions_Table must be a type of TransitionsTable, be valid and has at least one transition");

	public:
		using transitions_table = Transitions_Table;
		using states_tuple_type = getStatesFromTransitionsTable_t<Transitions_Table>;
		using states_variant_type = variantTypeFromStatesTuple_t<states_tuple_type>;
		using initial_state_type = std::conditional_t<
			std::is_same_v<InitialState, void>,
			std::tuple_element_t<0, states_tuple_type>,
			InitialState
		>;
		static_assert(isTypeInTuple_v<initial_state_type, states_tuple_type>, "InitialState must be a one of valid states");

		constexpr StateMachine() 
		{
			statesVariant.template emplace<initial_state_type>();

			if constexpr (CallOnEntryOnInitialState && has_onEntryNoEventArg_v<initial_state_type>)
			{
				std::get<initial_state_type>(statesVariant).onEntry();
			}

			tryAutomaticTransition(std::get<initial_state_type>(statesVariant));
		};

		template <typename NewState>
		constexpr void forceTransition()
		{
			static_assert(isTypeInTuple_v<NewState, states_tuple_type>, "NewState must be a one of valid states");
			statesVariant.template emplace<NewState>();
		};

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr void forceTransition()
		{
			static_assert(isTypeInTuple_v<State, states_tuple_type>, "State must be a one of valid states");
			std::get<State>(statesVariant).template forceTransition<InnerState, InnerStates...>();
		}

		template <typename State>
		constexpr bool isInState() const
		{
			static_assert(isTypeInTuple_v<State, states_tuple_type>, "State must be a one of valid states");
			return std::holds_alternative<State>(statesVariant);
		}

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr bool isInState() const
		{
			static_assert(isTypeInTuple_v<State, states_tuple_type>, "State must be a one of valid states");
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
			static_assert(isTypeInTuple_v<State, states_tuple_type>, "State must be a one of valid states");
			return std::get<State>(statesVariant);
		}

		template <typename State, typename InnerState, typename ...InnerStates>
		constexpr auto getState()
		{
			static_assert(isTypeInTuple_v<State, states_tuple_type>, "State must be a one of valid states");
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
					using CurrentStateType = std::decay_t<decltype(currentState)>;
					
					
					if constexpr (isStateMachine_v<CurrentStateType>)
					{
						const auto innerTransitResult = innerStateMachineTransition(currentState, event);

						if (innerTransitResult == HandleEventResult::EXIT_AUTOMATIC_INNER_STATE_MACHINE ||
							innerTransitResult == HandleEventResult::EXIT_INNER_STATE_MACHINE)
						{
							if (innerTransitResult == HandleEventResult::EXIT_INNER_STATE_MACHINE)
							{
								if (const auto exitInnerStateMachineResult = normalTransition(currentState, event);
									isEventResultOk(exitInnerStateMachineResult))
								{
									return exitInnerStateMachineResult;
								}
							}

							return tryAutomaticTransition(currentState);
						}

						return innerTransitResult;
					}
					else
					{
						return normalTransition(currentState, event);
					}
				};

			return std::visit(lambda, statesVariant);
		}

		template <typename CurrentStateType, typename EventTriggerType>
		constexpr HandleEventResult normalTransition(CurrentStateType& currentState, const EventTriggerType& event)
		{
			using transition = getTransition_t<transitions_table, CurrentStateType, EventTriggerType>;

			if constexpr (isValidTransition_v<transition>)
			{
				auto transitResult = transit<transition>(currentState, event);
				
				if (!isEventResultOk(transitResult))
				{
					return transitResult;
				}

				using NextStateType = getNextState_t<transition>;
				if constexpr (!isStateMachine_v<NextStateType> && !std::is_same_v<NextStateType, ExitState>)
				{
					const auto automaticTransitionResult = tryAutomaticTransition(std::get<NextStateType>(statesVariant));
					
					if (isEventResultOk(automaticTransitionResult))
					{
						transitResult = automaticTransitionResult;
					}
				}
				
				return transitResult;
			}
			else
			{
				return HandleEventResult::NO_VALID_TRANSITION;
			}
		}

		template <typename InnerStateMachineType, typename EventTriggerType>
		static constexpr HandleEventResult innerStateMachineTransition(InnerStateMachineType& innerStateMachine, const EventTriggerType& event)
		{
			const auto innerTransitionResult = innerStateMachine.handleEvent(event);
			return innerTransitionResult;
		}

		template <typename CurrentStateType>
		constexpr HandleEventResult tryAutomaticTransition(CurrentStateType& currentState)
		{
			if constexpr (hasAutomaticTransition_v<transitions_table, CurrentStateType>)
			{
				using transition = getTransition_t<transitions_table, CurrentStateType, AUTOMATIC_TRANSITION>;
				using NextStateType = getNextState_t<transition>;
				
				auto transitResult = transit<transition>(currentState, AUTOMATIC_TRANSITION{});

				if (transitResult == HandleEventResult::EXIT_INNER_STATE_MACHINE)
				{
					return HandleEventResult::EXIT_AUTOMATIC_INNER_STATE_MACHINE;
				}

				if (isEventResultOk(transitResult))
				{
					if (const auto nextAutomaticTransitionsResult = tryAutomaticTransition(std::get<NextStateType>(statesVariant));
							isEventResultOk(nextAutomaticTransitionsResult))
					{
						transitResult = nextAutomaticTransitionsResult;
					}
				}

				return transitResult;
			}
			else
			{
				return HandleEventResult::NO_VALID_TRANSITION;
			}
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
				if constexpr (hasGuard_v<Transition>)
				{
					if (!getGuard_t<Transition>{}(currentState, event))
						return HandleEventResult::GUARD_FAILED;
				}
				
				using NextStateType = getNextState_t<Transition>;

				if constexpr (std::is_same_v<std::decay_t<CurrentStateType>, NextStateType>)
				{
					tryCallTransitionAction<Transition>(currentState, event, currentState);
					return HandleEventResult::PROCESSED_SAME_STATE;
				}
				else if constexpr (std::is_same_v<NextStateType, ExitState>)
				{
					tryCallOnExit(currentState, event);
					return HandleEventResult::EXIT_INNER_STATE_MACHINE;
				}
				else
				{
					tryCallOnExit(currentState, event);
					NextStateType nextState;
					tryCallTransitionAction<Transition>(currentState, event, nextState);
					tryCallOnEntry(nextState, event);
					
					statesVariant = std::move(nextState);
					return HandleEventResult::PROCESSED;
				}
			}

			return HandleEventResult::NO_VALID_TRANSITION;
		}


		template <typename StateType, typename EventTriggerType>
		static constexpr void tryCallOnExit(StateType& state, const EventTriggerType& event)
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
		static constexpr void tryCallOnEntry(StateType& state, const EventTriggerType& event)
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
		static constexpr void tryCallTransitionAction(StateBeforeType& stateBefore, const EventTriggerType& event, StateAfterType& stateAfter)
		{
			if constexpr (hasAction_v<Transition>)
			{
				getAction_t<Transition> {} (stateBefore, event, stateAfter);
			}
		}
	};
}