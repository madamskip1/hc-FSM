/**
 * @author: Adamski Maciej (madamskip1)
 * @project: hcFSM
 * @project_url: https://github.com/madamskip1/hc-FSM
 * @date: 2024-07-04
 * @license: MIT License. Keep metadata_header intact. 
 * @version: 0.5.6
 */

#pragma once

#include <type_traits>
#include <tuple>
#include <variant>

namespace hcFSM
{
    template <typename Transitions_Table, typename InitialState = void, bool CallOnEntryOnInitialState = false>
    class StateMachine;
}
namespace hcFSM
{
    enum class HandleEventResult
    {
        PROCESSED,
        PROCESSED_SAME_STATE, // Event processed, but there is no need to change state, for e.g. StateA (EventA) -> StateA
        NO_VALID_TRANSITION,
        GUARD_FAILED,

        // INTERNAL USED
        EXIT_INNER_STATE_MACHINE,
        EXIT_AUTOMATIC_INNER_STATE_MACHINE,
    };

    constexpr bool isEventResultOk(HandleEventResult result)
    {
        return result == HandleEventResult::PROCESSED ||
            result == HandleEventResult::PROCESSED_SAME_STATE ||
            result == HandleEventResult::EXIT_AUTOMATIC_INNER_STATE_MACHINE ||
            result == HandleEventResult::EXIT_INNER_STATE_MACHINE;
    }
}

namespace hcFSM
{
	struct ExitState {};

	/* has_onEntry */

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

	/* ~has_onEntry */

	/* has_onExit */

	template <typename StateType, typename EventType, typename = void>
	struct has_onExit : std::false_type {};

	template <typename StateType, typename EventType>
	struct has_onExit<StateType, EventType, std::void_t<decltype(std::declval<StateType>().onExit(std::declval<EventType>()))>> : std::true_type {};

	template <typename StateType, typename EventType>
	inline constexpr bool has_onExit_v = has_onExit<StateType, EventType>::value;

	template <typename StateType, typename = void>
	struct has_onExitNoEventArg : std::false_type {};

	template <typename StateType>
	struct has_onExitNoEventArg<StateType, std::void_t<decltype(std::declval<StateType>().onExit())>> : std::true_type {};

	template <typename StateType>
	inline constexpr bool has_onExitNoEventArg_v = has_onExitNoEventArg<StateType>::value;

	/* ~has_onExit */
}

namespace hcFSM
{
	struct NO_VALID_TRANSITION {};
	struct AUTOMATIC_TRANSITION {};

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Action = void, typename Guard = void>
	struct Transition
	{
		static_assert(!std::is_same_v<BeforeStateType, void>, "BeforeStateType cannot be void");
		static_assert(!std::is_same_v<EventTriggerType, void>, "EventTriggerType cannot be void");
		static_assert(!std::is_same_v<NextStateType, void>, "NextStateType cannot be void");
		static_assert(std::is_same_v<Action, void> || std::is_invocable_v<Action, BeforeStateType&, EventTriggerType&, NextStateType&>,
			"Action must be void or callable struct with BeforeStateType&, EventTriggerType&, NextStateType&");
		static_assert(std::is_same_v<Guard, void> || std::is_invocable_v<Guard, BeforeStateType&, EventTriggerType&>,
			"Guard must be void or callable struct with BeforeStateType&, EventTriggerType&");
		
		using before_state_type = BeforeStateType;
		using event_type = EventTriggerType;
		using next_state_type = NextStateType;
		using action_type = Action;
		using guard_type = Guard;
	};

	// Specialized Transition types

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Guard = void>
	using TransitionWithGuard = Transition<BeforeStateType, EventTriggerType, NextStateType, void, Guard>;

	template <typename BeforeStateType,typename NextStatetype, typename Action = void, typename Guard = void>
	using TransitionAutomatic = Transition<BeforeStateType, AUTOMATIC_TRANSITION, NextStatetype, Action, Guard>;

	template <typename BeforeStateType, typename NextStatetype, typename Guard = void>
	using TransitionAutomaticWithGuard = Transition<BeforeStateType, AUTOMATIC_TRANSITION, NextStatetype, void, Guard>;

	// ~Specialized Transition types

	// getBeforeState, getNextState, getEvent, getAction, getGuard

	template <typename TransitionType>
	struct getBeforeState
	{
		using type = typename TransitionType::before_state_type;
	};

	template <typename TransitionType>
	using getBeforeState_t = typename getBeforeState<TransitionType>::type;

	template <typename TransitionType>
	struct getNextState
	{
		using type = typename TransitionType::next_state_type;
	};

	template <typename TransitionType>
	using getNextState_t = typename getNextState<TransitionType>::type;

	template <typename TransitionType>
	struct getEvent
	{
		using type = typename TransitionType::event_type;
	};

	template <typename TransitionType>
	using getEvent_t = typename getEvent<TransitionType>::type;

	template <typename TransitionType>
	struct getAction
	{
		using type = typename TransitionType::action_type;
	};

	template <typename TransitionType>
	using getAction_t = typename getAction<TransitionType>::type;

	template <typename TransitionType>
	struct getGuard
	{
		using type = typename TransitionType::guard_type;
	};

	template <typename TransitionType>
	using getGuard_t = typename getGuard<TransitionType>::type;

	// ~getBeforeState, getNextState, getEvent, getAction, getGuard

	// hasAction, hasGuard

	template <typename TransitionType>
	struct hasAction : std::is_invocable<getAction_t<TransitionType>, getBeforeState_t<TransitionType>&, getEvent_t<TransitionType>&, getNextState_t<TransitionType>&> {};

	template <typename TransitionType>
	static constexpr bool hasAction_v = hasAction<TransitionType>::value;

	template <typename TransitionType>
	struct hasGuard : std::is_invocable<getGuard_t<TransitionType>, getBeforeState_t<TransitionType>&, getEvent_t<TransitionType>&> {};

	template <typename TransitionType>
	static constexpr bool hasGuard_v = hasGuard<TransitionType>::value;

	// ~hasAction, hasGuard


	// isValidTransition
	
	template <typename Transition>
	struct isValidTransition : std::false_type {};

	template <typename BeforeStateType, typename EventTriggerType, typename NextStateType, typename Action, typename Guard>
	struct isValidTransition<Transition<BeforeStateType, EventTriggerType, NextStateType, Action, Guard>> : std::true_type {};

	template <typename Transition>
	static constexpr bool isValidTransition_v = isValidTransition<Transition>::value;

	// ~isValidTransition

	// isAutomaticTransition

	template <typename Transition>
	struct isAutomaticTransition : std::is_same<getEvent_t<Transition>, AUTOMATIC_TRANSITION> {};

	template <typename Transition>
	static constexpr bool isAutomaticTransition_v = isAutomaticTransition<Transition>::value;

	// ~isAutomaticTransition
}
namespace hcFSM
{
    #define CREATE_TRANSITION_ACTION(Name, SourceStateParam, EventParam, TargetStateParam, MethodBody) \
        struct Name { \
            template <typename SourceStateParamType, typename EventParamType, typename TargetStateParamType> \
            void operator()(SourceStateParamType& SourceStateParam, EventParamType& EventParam, TargetStateParamType& TargetStateParam) \
                MethodBody \
        };
}
namespace hcFSM
{
    #define CREATE_TRANSITION_GUARD(Name, SourceStateParam, EventParam,  MethodBody) \
        struct Name { \
            template <typename SourceStateParamType, typename EventParamType> \
            const bool operator()(SourceStateParamType& SourceStateParam, EventParamType& EventParam) \
                MethodBody \
        };
}
namespace hcFSM
{
	template <typename... Transitions>
	struct TransitionsTable
	{
		static_assert(sizeof...(Transitions) >= 1, "TransitionsTable must have at least one transition");
		static_assert((isValidTransition_v<Transitions> && ...), "TransitionsTable must have only valid transitions. All must be Transition<...> types");
		
		using transitions = std::tuple<Transitions...>;
	};

	// isValidTransitionsTable

	template <typename T>
	struct isValidTransitionsTable : std::false_type {};

	template <typename... Transitions>
	struct isValidTransitionsTable<TransitionsTable<Transitions...>> : std::conjunction<std::bool_constant<sizeof...(Transitions) >= 1>, isValidTransition<Transitions>...> {};

	template <typename T>
	static constexpr bool isValidTransitionsTable_v = isValidTransitionsTable<T>::value;

	// ~isValidTransitionsTable

	// getTransitions

	template <typename Transitions_Table>
	struct getTransitions
	{
		using type = typename Transitions_Table::transitions;
	};

	template <typename Transitions_Table>
	using getTransitions_t = typename getTransitions<Transitions_Table>::type;

	// ~getTransitions


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

		static constexpr bool value = hasTransition_impl<getTransitions_t<Transitions_Table>>::value;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	static constexpr bool hasTransition_v = hasTransition<Transitions_Table, BeforeStateType, EventTriggerType>::value;

	// ~hasTransition

	// hasAutomaticTransition

	template <typename Transitions_Table, typename BeforeStateType>
	struct hasAutomaticTransition : hasTransition<Transitions_Table, BeforeStateType, AUTOMATIC_TRANSITION> {};
	
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

		using type = typename getTransition_impl<getTransitions_t<Transitions_Table>>::type;
	};

	template <typename Transitions_Table, typename BeforeStateType, typename EventTriggerType>
	using getTransition_t = typename getTransition<Transitions_Table, BeforeStateType, EventTriggerType>::type;

	// ~getTransition

	// getStatesFromTransitionsTable

	template <typename Transitions_Table>
	struct getStatesFromTransitionsTable
	{
		template <typename Type, typename ...Types>
		struct isInTypes : std::disjunction<std::is_same<Type, Types>...> {};

		template<typename Type, typename Types>
		struct addTypeToTupleIfNotOccured;

		template <typename Type, typename ...Types>
		struct addTypeToTupleIfNotOccured<Type, std::tuple<Types...>>
		{
			using type = typename std::conditional_t<
				isInTypes<Type, Types...>::value,
				std::tuple<Types...>,
				std::tuple<Types..., Type>
			>;
		};

		template <typename ...Transitions>
		struct getStatesFromTransitionsTable_impl;

		template <typename StatesTuple, typename Transition>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition>>
		{
			using type = typename addTypeToTupleIfNotOccured<getNextState_t<Transition>, typename addTypeToTupleIfNotOccured<getBeforeState_t<Transition>, StatesTuple>::type>::type;
		};

		template <typename StatesTuple, typename Transition, typename ...RestTransitions>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition, RestTransitions...>>
		{
			using states_tuple_with_this_transition = typename addTypeToTupleIfNotOccured<getNextState_t<Transition>, typename addTypeToTupleIfNotOccured<getBeforeState_t<Transition>, StatesTuple>::type>::type;

			using type = typename getStatesFromTransitionsTable_impl<states_tuple_with_this_transition, std::tuple<RestTransitions...>>::type;
		};

		using states_tuple_type = typename getStatesFromTransitionsTable_impl<typename std::tuple<>, getTransitions_t<Transitions_Table>>::type;
	};

	template <typename Transitions_Table>
	using getStatesFromTransitionsTable_t = typename getStatesFromTransitionsTable<Transitions_Table>::states_tuple_type;

	// ~getStatesFromTransitionsTable
}
namespace hcFSM
{
    // variantTypeFromStatesTuple

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

    // ~variantTypeFromStatesTuple

    // isStateMachine

    template <typename T>
    struct isStateMachine : std::false_type {};

    template <typename Machine>
    struct isStateMachine<StateMachine<Machine>> : std::true_type {};

    template <typename T>
    inline constexpr bool isStateMachine_v = isStateMachine<T>::value;

    // ~isStateMachine

    // isTypeInTuple

    template <typename T, typename... Types>
    struct isTypeInTuple;

    template <typename Type, typename ...Types>
    struct isTypeInTuple<Type, std::tuple<Types...>> : std::disjunction<std::is_same<Type, Types>...> {};

    template <typename T, typename Tuple>
    inline constexpr bool isTypeInTuple_v = isTypeInTuple<T, Tuple>::value;

    // ~isTypeInTuple
}

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