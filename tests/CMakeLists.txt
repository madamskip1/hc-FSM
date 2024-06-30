SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

macro(add_unit_test testname)
    add_executable(
        unit_${testname}
        ${testname}.cpp
    )
    target_link_libraries(
        unit_${testname}
        PRIVATE
        hcFSM::hcFSM
        GTest::gtest_main)
    add_test(
        NAME unit_${testname}
        COMMAND unit_${testname}
    )
    gtest_discover_tests(
        unit_${testname}
    )
endmacro(add_unit_test)

macro(add_integration_test testname)
    add_executable(
        integration_${testname}
        integration_tests/${testname}.cpp
    )
    target_link_libraries(
        integration_${testname}
        PRIVATE
        hcFSM::hcFSM
        GTest::gtest_main
    )
    add_test(
        NAME integration_${testname}
        COMMAND integration_${testname}
    )
    gtest_discover_tests(
        integration_${testname}
        )
endmacro(add_integration_test)

include(FetchContent)
include(GoogleTest)

# Download and build gtest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Add gtest include directory and link against gtest library
include_directories(${googletest_SOURCE_DIR}/include)
link_directories(${googletest_BINARY_DIR})


add_unit_test(state_traits_tests)
add_unit_test(state_machine_tests)
add_unit_test(state_machine_traits_tests)
add_unit_test(hierarchical_state_machine_tests)
add_unit_test(automatic_transition_tests)
add_unit_test(initial_states_ctor_dtor_tests)
add_unit_test(transition_action_tests)
add_unit_test(transition_guard_tests)
add_unit_test(handle_event_result_tests)
add_unit_test(transition_struct_tests)
add_unit_test(transition_traits_tests)
add_unit_test(transition_table_tests)


add_integration_test(onEntry_onExit_actions_tests)
add_integration_test(guard_action_tests)
add_integration_test(simple_transition_tests)
add_integration_test(inner_sm_transition_tests)