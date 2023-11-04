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
}