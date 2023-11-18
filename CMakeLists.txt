cmake_minimum_required(VERSION 3.11.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FSM)

enable_testing()

include_directories(
    include
)
add_subdirectory(
    tests
)
