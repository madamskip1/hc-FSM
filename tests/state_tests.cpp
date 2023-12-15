#include <gtest/gtest.h>
#include "hcFSM/state.h"

namespace FSM
{
    struct Event {};
    struct EventA : Event {};

    struct StateA {};
    struct StateB
    {
        void onEntry() {};

        void onExit() {};
    };
    struct StateC
    {
        void onEntry(const Event& event) {};

        void onExit(const EventA& event) {};

    };
    struct StateD
    {
        void onEntry(const EventA& event) {};
        void onEntry(const Event& event) {};

        void onExit(const EventA& event) {};
        void onExit(const Event& event) {};
    };
    struct StateE
    {
        void onEntry(const EventA& eventA) {};

        void onExit(const EventA& eventA) {};
    };

    TEST(StateTraitsTests, HasOnEntryNoEventArg) {
        EXPECT_EQ(FSM::has_onEntryNoEventArg<StateA>::value, false);
        EXPECT_EQ(FSM::has_onEntryNoEventArg<StateB>::value, true);
        EXPECT_EQ(FSM::has_onEntryNoEventArg<StateC>::value, false);
        EXPECT_EQ(FSM::has_onEntryNoEventArg<StateD>::value, false);
        EXPECT_EQ(FSM::has_onEntryNoEventArg<StateE>::value, false);

        EXPECT_EQ(FSM::has_onEntryNoEventArg_v<StateA>, false);
        EXPECT_EQ(FSM::has_onEntryNoEventArg_v<StateB>, true);
        EXPECT_EQ(FSM::has_onEntryNoEventArg_v<StateC>, false);
        EXPECT_EQ(FSM::has_onEntryNoEventArg_v<StateD>, false);
        EXPECT_EQ(FSM::has_onEntryNoEventArg_v<StateE>, false);
    }

    TEST(StateTraitsTests, HasOnEntryEvent) {
        auto hasEntryStateA = FSM::has_onEntry<StateA, EventA>::value;
        auto hasEntryStateA_v = FSM::has_onEntry_v<StateA, EventA>;
        EXPECT_EQ(hasEntryStateA, false);
        EXPECT_EQ(hasEntryStateA_v, false);

        auto hasEntryStateB = FSM::has_onEntry<StateB, EventA>::value;
        auto hasEntryStateB_v = FSM::has_onEntry_v<StateB, EventA>;
        EXPECT_EQ(hasEntryStateB, false);
        EXPECT_EQ(hasEntryStateB_v, false);

        auto hasEntryStateC = FSM::has_onEntry<StateC, EventA>::value;
        auto hasEntryStateC_v = FSM::has_onEntry_v<StateC, EventA>;
        EXPECT_EQ(hasEntryStateC, true);
        EXPECT_EQ(hasEntryStateC_v, true);

        auto hasEntryStateD = FSM::has_onEntry<StateD, EventA>::value;
        auto hasEntryStateD_v = FSM::has_onEntry_v<StateD, EventA>;
        EXPECT_EQ(hasEntryStateD, true);
        EXPECT_EQ(hasEntryStateD_v, true);

        auto hasEntryStateE = FSM::has_onEntry<StateE, EventA>::value;
        auto hasEntryStateE_v = FSM::has_onEntry_v<StateE, EventA>;
        EXPECT_EQ(hasEntryStateE, true);
        EXPECT_EQ(hasEntryStateE_v, true);
    }

    TEST(StateTraitsTests, HasOnExitNoEventArg) {
        EXPECT_EQ(FSM::has_onExitNoEventArg<StateA>::value, false);
        EXPECT_EQ(FSM::has_onExitNoEventArg<StateB>::value, true);
        EXPECT_EQ(FSM::has_onExitNoEventArg<StateC>::value, false);
        EXPECT_EQ(FSM::has_onExitNoEventArg<StateD>::value, false);
        EXPECT_EQ(FSM::has_onExitNoEventArg<StateE>::value, false);

        EXPECT_EQ(FSM::has_onExitNoEventArg_v<StateA>, false);
        EXPECT_EQ(FSM::has_onExitNoEventArg_v<StateB>, true);
        EXPECT_EQ(FSM::has_onExitNoEventArg_v<StateC>, false);
        EXPECT_EQ(FSM::has_onExitNoEventArg_v<StateD>, false);
        EXPECT_EQ(FSM::has_onExitNoEventArg_v<StateE>, false);
    }

    TEST(StateTraitsTests, HasOnExitEvent) {
        auto hasEntryStateA = FSM::has_onExit<StateA, EventA>::value;
        auto hasEntryStateA_v = FSM::has_onExit_v<StateA, EventA>;
        EXPECT_EQ(hasEntryStateA, false);
        EXPECT_EQ(hasEntryStateA_v, false);

        auto hasEntryStateB = FSM::has_onExit<StateB, EventA>::value;
        auto hasEntryStateB_v = FSM::has_onExit_v<StateB, EventA>;
        EXPECT_EQ(hasEntryStateB, false);
        EXPECT_EQ(hasEntryStateB_v, false);

        auto hasEntryStateC = FSM::has_onExit<StateC, EventA>::value;
        auto hasEntryStateC_v = FSM::has_onExit_v<StateC, EventA>;
        EXPECT_EQ(hasEntryStateC, true);
        EXPECT_EQ(hasEntryStateC_v, true);

        auto hasEntryStateD = FSM::has_onExit<StateD, EventA>::value;
        auto hasEntryStateD_v = FSM::has_onExit_v<StateD, EventA>;
        EXPECT_EQ(hasEntryStateD, true);
        EXPECT_EQ(hasEntryStateD_v, true);

        auto hasEntryStateE = FSM::has_onExit<StateE, EventA>::value;
        auto hasEntryStateE_v = FSM::has_onExit_v<StateE, EventA>;
        EXPECT_EQ(hasEntryStateE, true);
        EXPECT_EQ(hasEntryStateE_v, true);
    }
}