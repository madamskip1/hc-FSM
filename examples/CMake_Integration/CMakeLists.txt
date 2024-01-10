cmake_minimum_required(VERSION 3.11.0)

project(
    CMakeIntegrationExample
    DESCRIPTION "hcFSM usage example showing how to integrate it in your project"
    LANGUAGES CXX
)

add_executable(
    CMakeIntegrationExample
    main.cpp
)

target_compile_features(
    CMakeIntegrationExample
    PRIVATE
    cxx_std_17
)

include(FetchContent)

# download hcFSM
FetchContent_Declare(
    hcFSM
    GIT_REPOSITORY https://github.com/madamskip1/hc-FSM.git
    GIT_TAG d670fc8452b877bf13bf8cd59b853e3ccaa7c3a5
)
FetchContent_MakeAvailable(hcFSM)

# link hc-FSM to target/project
target_link_libraries(
    CMakeIntegrationExample
    PRIVATE 
    hcFSM
)
