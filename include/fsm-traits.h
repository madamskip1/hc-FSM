#pragma once
#include <type_traits>
#include <tuple>
#include <variant>
#include "finite-state-machine-fwd.h"

namespace FSM
{
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
}