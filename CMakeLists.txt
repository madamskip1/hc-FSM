cmake_minimum_required(VERSION 3.11.0)

project(
    hcFSM
    VERSION 0.5.6
    DESCRIPTION "hierarchical compile-time Finite State Machine"
    HOMEPAGE_URL "https://github.com/madamskip1/hc-FSM"
    LANGUAGES CXX
)

add_library(
    hcFSM
    INTERFACE
)

add_library(
    hcFSM::hcFSM
    ALIAS
    hcFSM
)

target_include_directories(
    hcFSM
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:hcFSM/include>
)

target_compile_features(
    hcFSM
    INTERFACE
    cxx_std_17
)


option(
    HCFSM_ENABLE_TESTS
    "Enable tests for hcFSM"
    OFF
)

if(HCFSM_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()


option(
    HCFSM_ENABLE_EXAMPLES
    "Enable examples for hcFSM"
    OFF
)

if(HCFSM_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()


install(
    TARGETS hcFSM
    EXPORT hcFSMTargets
    PUBLIC_HEADER DESTINATION hcFSM/include/hcFSM
)

install(
    EXPORT hcFSMTargets
    FILE hcFSMTargets.cmake
    NAMESPACE hcFSM::
    DESTINATION hcFSM/cmake/hcFSM
)
