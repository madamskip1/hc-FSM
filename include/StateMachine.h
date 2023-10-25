#pragma once
#include <unordered_map>
#include "State.h"
#include "Event.h"


namespace FSM {
    template <typename... Args>
    class StateMachine {
    public:
        static_assert(std::conjunction_v<std::is_base_of<FSM::State, Args>...>,
            "All states must be derifed from FSM::State");

        using states_tuple_types = std::tuple<Args*...>;

        StateMachine(Args*... args) : data(std::make_tuple(args...)) {
            currentState = getState<0>();
        }

        std::tuple<Args*...> data;
        State* currentState;

        template <std::size_t N> 
        auto getState() const
        {
            //typename std::tuple_element<N, states_tuple_types>::type
            return std::get<N>(data); // (X : FSM::State)*
        }

        template <typename T>
        T* getState() const
        {
            return std::get<T*>(data);
        }

        template <typename T>
        bool hasState() {
            return hasState_impl<T, Args...>();
        }

        template <typename T>
        void forceTransition()
        {
            currentState = getState<T>();
        }

        // test function
        void print() const
        {
            currentState->print();
        }

    private:
        template <typename T, typename ...U>
        constexpr static bool hasState_impl() {
            return (std::is_same_v<T, U> || ...);
        }
    };
}
