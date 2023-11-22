#include <gtest/gtest.h>
#include "transition_action.h"

namespace FSM
{
    CREATE_TRANSITION_ACTION(CreateAction_test_action, source_state, event, target_state, 
        { 
            source_state = 1;
            event = 2; 
            target_state = 3; 
        }
    );

    TEST(TransitionActionTests, CreateAction)
    {
        int source_state = 0;
        int event = 0;
        int target_state = 0;
        CreateAction_test_action {} (source_state, event, target_state);
        EXPECT_EQ(source_state, 1);
        EXPECT_EQ(event, 2);
        EXPECT_EQ(target_state, 3);
    }
}