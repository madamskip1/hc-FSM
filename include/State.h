#pragma once
#include <type_traits>

namespace FSM
{
	struct State {};

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
}