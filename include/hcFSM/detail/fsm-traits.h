#pragma once
#include <type_traits>
#include <tuple>
#include <variant>
#include "finite-state-machine-fwd.h"

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

