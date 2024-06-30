SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
include(GoogleTest)

# Download and build gtest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)


add_executable(hcFSM_tests)

target_sources(hcFSM_tests
    PRIVATE
        state_traits_tests.cpp
        state_machine_tests.cpp
        state_machine_traits_tests.cpp
        hierarchical_state_machine_tests.cpp
        automatic_transition_tests.cpp
        initial_states_ctor_dtor_tests.cpp
        transition_action_tests.cpp
        transition_guard_tests.cpp
        handle_event_result_tests.cpp
        transition_struct_tests.cpp
        transition_traits_tests.cpp
        transition_table_tests.cpp
        onEntry_onExit_actions_tests.cpp
        simple_transition_tests.cpp
)

target_link_libraries(hcFSM_tests
    PRIVATE
        hcFSM::hcFSM
        GTest::gtest_main
)

add_test(
    NAME hcFSM_tests
    COMMAND hcFSM_tests
)

gtest_discover_tests(
    hcFSM_tests
)