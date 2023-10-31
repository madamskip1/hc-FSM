#include <iostream>
#include <typeindex>

#include "state.h"
#include "event.h"
#include "transitions-table.h"
#include "transition.h"
#include "finite-state-machine.h"


struct EventA : public FSM::Event {};
struct EventB : public FSM::Event {};

struct StateA : public FSM::State {};
struct StateB : public FSM::State {};
class StateC : public FSM::State {};


int main()
{
    using transition1 = FSM::Transition<StateA, EventB, StateC>;
    using transition2 = FSM::Transition<StateC, EventA, StateA>;
    using transition3 = FSM::Transition<StateB, EventB, StateB>;
    using transition4 = FSM::Transition<StateA, EventA, StateC>;
    using Trans = FSM::TransitionsTable <
        transition1,
        transition2,
        transition3,
        transition4
    >;
    using states_tuple_type = typename FSM::getStatesFromTransitionsTable<Trans>::states_tuple_type;
    std::cout << std::type_index(typeid(states_tuple_type)).name() << '\n';
    std::cout << std::type_index(typeid(FSM::getStatesFromTransitionsTable_t<Trans>)).name() << '\n';
    std::cout << std::type_index(typeid(FSM::variantTypeFromStatesTuple_t<states_tuple_type>)).name() << '\n';
    auto fsm = FSM::StateMachine<Trans, StateC>{};
    return 0;
}