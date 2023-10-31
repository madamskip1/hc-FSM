#pragma once
#include <type_traits>
#include <tuple>

namespace FSM
{
	template <typename... T>
	struct TransitionsTable {
		using transitions = std::tuple<T...>;
	};




	template <typename Transitions_Table>
	struct getStatesFromTransitionsTable {

		template <typename T, typename ...Types>
		struct isInTypes
		{
			static constexpr bool value = std::disjunction_v<std::is_same<T, Types>...>;
		};


		template<typename T, typename U>
		struct addTypeToTupleIfNotOccured;

		template <typename T, typename ...Types>
		struct addTypeToTupleIfNotOccured<T, std::tuple<Types...>>
		{
			using type = typename std::conditional_t<
				isInTypes<T, Types...>::value,
				std::tuple<Types...>,
				std::tuple<Types..., T>
			>;
		};


		template <typename ...Transitions>
		struct getStatesFromTransitionsTable_impl;

		template <typename StatesTuple, typename Transition>
		struct getStatesFromTransitionsTable_impl<StatesTuple, std::tuple<Transition>>
		{
			using before_state = typename Transition::before_state_type;
			using next_state = typename Transition::next_state_type;

			using type = typename addTypeToTupleIfNotOccured<next_state, typename addTypeToTupleIfNotOccured<before_state, StatesTuple>::type>::type;
		};

		template <typename States, typename Head, typename ...Tail>
		struct getStatesFromTransitionsTable_impl<States, std::tuple<Head, Tail...>>
		{
			using before_state = typename Head::before_state_type;
			using next_state = typename Head::next_state_type;

			using with_before_state = typename addTypeToTupleIfNotOccured<before_state, States>::type;
			using with_before_after_state = typename addTypeToTupleIfNotOccured<next_state, with_before_state>::type;

			using type = typename getStatesFromTransitionsTable_impl<with_before_after_state, std::tuple<Tail...>>::type;
		};

		using states_tuple_type = typename getStatesFromTransitionsTable_impl<typename std::tuple<>, typename Transitions_Table::transitions>::type;
	};

	template <typename Transitions_Table>
	using getStatesFromTransitionsTable_t = typename getStatesFromTransitionsTable<Transitions_Table>::states_tuple_type;
}