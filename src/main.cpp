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
struct StateB : public FSM::State {
    void onEntry() {
        std::cout << "StateB::onEntry() \n";
    }
};
struct StateC : public FSM::State {
    void onEntry(const EventA& event)
    {
        std::cout << "StateC::onEntry(const FSM::Event& event) \n";
    }
    void onEntry(const FSM::Event& event)
    {
        std::cout << "StateC::onEntry(const FSM::Event& event) \n";
    }
    
};
struct StateD : public FSM::State {
    void onEntry(const EventA& eventA)
    {
        std::cout << "StateC::onEntry(const EventA& event) \n";
    }
};

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

    std::cout << "is StateB: " << fsm.isInState<StateB>() << '\n';
    std::cout << "is StateC: " << fsm.isInState<StateC>() << '\n';
    fsm.forceTransition<StateB>();
    std::cout << "is StateB: " << fsm.isInState<StateB>() << '\n';
    std::cout << "is StateC: " << fsm.isInState<StateC>() << '\n';

    std::cout << "StateA: \n";
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg<StateA>::value << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry<StateA, EventA>::value << std::endl;
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg_v<StateA> << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry_v<StateA, EventA> << std::endl;

    std::cout << "StateB: \n";
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg<StateB>::value << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry<StateB, EventA>::value << std::endl;
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg_v<StateB> << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry_v<StateB, EventA> << std::endl;

    std::cout << "StateC: \n";
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg<StateC>::value << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry<StateC, EventA>::value << std::endl;
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg_v<StateC> << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry_v<StateC, EventA> << std::endl;

    std::cout << "StateD: \n";
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg<StateD>::value << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry<StateD, EventA>::value << std::endl;
    std::cout << "onEntryNoArgs(): " << FSM::has_onEntryNoEventArg_v<StateD> << std::endl;
    std::cout << "onEntry(FSM::EventA&): " << FSM::has_onEntry_v<StateD, EventA> << std::endl;
    
    
    StateC{}.onEntry(FSM::Event{});
    StateC{}.onEntry(EventA{});
    return 0;
}