#include <iostream>
#include "Event.h"
#include "State.h"
#include "StateMachine.h"

class EventA : public FSM::Event
{
public:
    void print()
    {
        std::cout << "EventA \n";
    }
};
class EventB : public FSM::Event
{
public:
    void print()
    {
        std::cout << "EventB \n";
    }
};

class StateA : public FSM::State {
public:
    virtual void handleEvent(FSM::Event& event) override {
        std::cout << "handleEvent StateA\n";
        event.print();
    }
    void handleEvent(EventA& event)
    {
        std::cout << "handleEvent StateA EventA\n";
        event.print();
    }
    virtual void print() const override {
        std::cout << "StateA" << std::endl;
    }
};

class StateB : public FSM::State {
public:
    virtual void handleEvent(FSM::Event& event) override {
        std::cout << "handleEvent StateB\n";
        event.print();
    }
    virtual void print() const {
        std::cout << "StateB" << std::endl;
    }
};


int main()
{
    StateA* stateA = new StateA{};
    StateB* stateB = new StateB{};

    auto FSM = FSM::StateMachine(stateA, stateB);
    auto stateAx = FSM.getState<1>();
    stateAx->print();

    auto stateAy = FSM.getState<StateA>();
    stateAy->print();

    std::cout << FSM.hasState<StateA>();
    std::cout << FSM.hasState<StateB>();
    std::cout << FSM.hasState<EventA>();
    std::cout << std::endl;

    FSM.print();
    FSM.forceTransition<StateB>();
    FSM.print();
    return 0;
}