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

    void onExit() {
        std::cout << "StateB::onExit() \n";
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

    void onExit(const EventA& event)
    {
        std::cout << "StateC::onExit(const FSM::Event& event) \n";
    }
    void onExit(const FSM::Event& event)
    {
        std::cout << "StateC::onExit(const FSM::Event& event) \n";
    }
    
};
struct StateD : public FSM::State {
    void onEntry(const EventA& eventA)
    {
        std::cout << "StateC::onEntry(const EventA& event) \n";
    }

    void onExit(const EventA& eventA)
    {
        std::cout << "StateC::onExit(const EventA& event) \n";
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
    
    
    std::cout << "StateA: \n";
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg<StateA>::value << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit<StateA, EventA>::value << std::endl;
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg_v<StateA> << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit_v<StateA, EventA> << std::endl;

    std::cout << "StateB: \n";
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg<StateB>::value << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit<StateB, EventA>::value << std::endl;
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg_v<StateB> << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit_v<StateB, EventA> << std::endl;

    std::cout << "StateC: \n";
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg<StateC>::value << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit<StateC, EventA>::value << std::endl;
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg_v<StateC> << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit_v<StateC, EventA> << std::endl;

    std::cout << "StateD: \n";
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg<StateD>::value << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit<StateD, EventA>::value << std::endl;
    std::cout << "onExitNoArgs(): " << FSM::has_onExitNoEventArg_v<StateD> << std::endl;
    std::cout << "onExit(FSM::EventA&): " << FSM::has_onExit_v<StateD, EventA> << std::endl;


    std::cout << "hasTransition<StateA, EventB>: " << FSM::hasTransition<Trans, StateA, EventB>::value << std::endl;
    std::cout << "hasTransition<StateB, EventA>: " << FSM::hasTransition_v<Trans, StateB, EventA> << std::endl;
    
    std::cout << "hasTransition<StateA, EventB>: " << FSM::hasTransition<Trans, StateA, EventB>::value << std::endl;
    std::cout << "hasTransition<StateB, EventA>: " << FSM::hasTransition_v<Trans, StateB, EventA> << std::endl;

    std::cout << "Next (StateA, EventB): " << std::type_index(typeid(typename FSM::getNextStateFromTransitionsTable<Trans, StateA, EventB>::type)).name() << std::endl;
    std::cout << "Next (StateB, EventA): " << std::type_index(typeid(typename FSM::getNextStateFromTransitionsTable<Trans, StateB, EventA>::type)).name() << std::endl;
    
    std::cout << "Next (StateA, EventB): " << std::type_index(typeid(FSM::getNextStateFromTransitionsTable_t<Trans, StateA, EventB>)).name() << std::endl;
    std::cout << "Next (StateB, EventA): " << std::type_index(typeid(FSM::getNextStateFromTransitionsTable_t<Trans, StateB, EventA>)).name() << std::endl;
    
    return 0;
}