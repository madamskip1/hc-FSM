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

option(
    HCFSM_ENABLE_EXAMPLES
    "Enable examples for hcFSM"
    OFF
)

option(
    HCFSM_CPPCHECK
    "Enable cppcheck for hcFSM"
    OFF
)

if(HCFSM_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(HCFSM_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

if(HCFSM_ENABLE_CPPCHECK)
    find_program(CPPCHECK_EXECUTABLE cppcheck REQUIRED)

    set(CXX_CPPCHECK_OPTIONS
        "--enable=all"
        "--inconclusive"
        "--force"
        "--inline-suppr"
        "--std=c++17"
        "--language=c++"
        "--template=gcc"
        "--verbose"
        "--suppress=missingIncludeSystem"
    )

    file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/include/hcFSM/detail/*.h)

    add_custom_target(run_cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE} ${CXX_CPPCHECK_OPTIONS} ${HEADER_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running Cppcheck static analysis on hcFSM header files"
        VERBATIM
    )
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
