#include <gtest/gtest.h>
#include "hcFSM/detail/state.h"

namespace hcFSM
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
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg<StateA>::value, false);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg<StateB>::value, true);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg<StateC>::value, false);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg<StateD>::value, false);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg<StateE>::value, false);

        EXPECT_EQ(hcFSM::has_onEntryNoEventArg_v<StateA>, false);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg_v<StateB>, true);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg_v<StateC>, false);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg_v<StateD>, false);
        EXPECT_EQ(hcFSM::has_onEntryNoEventArg_v<StateE>, false);
    }

    TEST(StateTraitsTests, HasOnEntryEvent) {
        auto hasEntryStateA = hcFSM::has_onEntry<StateA, EventA>::value;
        auto hasEntryStateA_v = hcFSM::has_onEntry_v<StateA, EventA>;
        EXPECT_EQ(hasEntryStateA, false);
        EXPECT_EQ(hasEntryStateA_v, false);

        auto hasEntryStateB = hcFSM::has_onEntry<StateB, EventA>::value;
        auto hasEntryStateB_v = hcFSM::has_onEntry_v<StateB, EventA>;
        EXPECT_EQ(hasEntryStateB, false);
        EXPECT_EQ(hasEntryStateB_v, false);

        auto hasEntryStateC = hcFSM::has_onEntry<StateC, EventA>::value;
        auto hasEntryStateC_v = hcFSM::has_onEntry_v<StateC, EventA>;
        EXPECT_EQ(hasEntryStateC, true);
        EXPECT_EQ(hasEntryStateC_v, true);

        auto hasEntryStateD = hcFSM::has_onEntry<StateD, EventA>::value;
        auto hasEntryStateD_v = hcFSM::has_onEntry_v<StateD, EventA>;
        EXPECT_EQ(hasEntryStateD, true);
        EXPECT_EQ(hasEntryStateD_v, true);

        auto hasEntryStateE = hcFSM::has_onEntry<StateE, EventA>::value;
        auto hasEntryStateE_v = hcFSM::has_onEntry_v<StateE, EventA>;
        EXPECT_EQ(hasEntryStateE, true);
        EXPECT_EQ(hasEntryStateE_v, true);
    }

    TEST(StateTraitsTests, HasOnExitNoEventArg) {
        EXPECT_EQ(hcFSM::has_onExitNoEventArg<StateA>::value, false);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg<StateB>::value, true);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg<StateC>::value, false);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg<StateD>::value, false);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg<StateE>::value, false);

        EXPECT_EQ(hcFSM::has_onExitNoEventArg_v<StateA>, false);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg_v<StateB>, true);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg_v<StateC>, false);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg_v<StateD>, false);
        EXPECT_EQ(hcFSM::has_onExitNoEventArg_v<StateE>, false);
    }

    TEST(StateTraitsTests, HasOnExitEvent) {
        auto hasEntryStateA = hcFSM::has_onExit<StateA, EventA>::value;
        auto hasEntryStateA_v = hcFSM::has_onExit_v<StateA, EventA>;
        EXPECT_EQ(hasEntryStateA, false);
        EXPECT_EQ(hasEntryStateA_v, false);

        auto hasEntryStateB = hcFSM::has_onExit<StateB, EventA>::value;
        auto hasEntryStateB_v = hcFSM::has_onExit_v<StateB, EventA>;
        EXPECT_EQ(hasEntryStateB, false);
        EXPECT_EQ(hasEntryStateB_v, false);

        auto hasEntryStateC = hcFSM::has_onExit<StateC, EventA>::value;
        auto hasEntryStateC_v = hcFSM::has_onExit_v<StateC, EventA>;
        EXPECT_EQ(hasEntryStateC, true);
        EXPECT_EQ(hasEntryStateC_v, true);

        auto hasEntryStateD = hcFSM::has_onExit<StateD, EventA>::value;
        auto hasEntryStateD_v = hcFSM::has_onExit_v<StateD, EventA>;
        EXPECT_EQ(hasEntryStateD, true);
        EXPECT_EQ(hasEntryStateD_v, true);

        auto hasEntryStateE = hcFSM::has_onExit<StateE, EventA>::value;
        auto hasEntryStateE_v = hcFSM::has_onExit_v<StateE, EventA>;
        EXPECT_EQ(hasEntryStateE, true);
        EXPECT_EQ(hasEntryStateE_v, true);
    }
}