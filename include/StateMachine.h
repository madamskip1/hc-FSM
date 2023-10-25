#pragma once
#include <unordered_map>
#include "State.h"
#include "Event.h"
#include <iostream>
#include <typeindex>


namespace FSM {
    template <typename TransitionsTable, typename StatesTable>
    class StateMachine {
    public:

        using states_tuple_type = typename StatesTable::states_tuple_type;
        using transitions_table = TransitionsTable;
        
        StateMachine(states_tuple_type tuples) : data(tuples) {
            currentState = getState<0>();
        }

        states_tuple_type data;
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
            return hasState_impl<T*>(data);
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
        constexpr static bool hasState_impl(const std::tuple<U...>& tuple) {
            return (std::is_same_v<T, U> || ...);
        }
    };

}
