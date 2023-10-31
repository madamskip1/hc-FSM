#include <iostream>
#include <typeindex>

#include "state.h"
#include "event.h"
#include "transitions-table.h"
#include "transition.h"


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
    using states_tuple = typename FSM::getStatesFromTransitionsTable<Trans>::states_types_tuple;
    std::cout << std::type_index(typeid(states_tuple)).name() << '\n';
    std::cout << std::type_index(typeid(FSM::getStatesFromTransitionsTable_t<Trans>)).name() << '\n';

    return 0;
}