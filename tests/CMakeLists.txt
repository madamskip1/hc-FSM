# FILEPATH: /home/maciej/Programowanie/Finite-State-Machine/test/CMakeLists.txt
cmake_minimum_required(VERSION 3.11)
project(YourProjectName)


macro(add_unit_test testname)
    add_executable(${testname} ${testname}.cpp)
    target_link_libraries(${testname} GTest::gtest_main)
    add_test(NAME ${testname} COMMAND ${testname})
    gtest_discover_tests(${testname})
endmacro(add_unit_test)

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

enable_testing()

add_unit_test(state_tests)
add_unit_test(transition_tests)
add_unit_test(state_machine_tests)