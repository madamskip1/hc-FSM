# hc-FSM

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Tests](https://github.com/madamskip1/hc-FSM/actions/workflows/build_and_test.yaml/badge.svg?branch=main)](https://github.com/madamskip1/hc-FSM/actions/workflows/build_and_test.yaml)

##### Compiler build support
[![Build g++](https://github.com/madamskip1/hc-FSM/actions/workflows/build_g++.yaml/badge.svg?branch=main)](https://github.com/madamskip1/hc-FSM/actions/workflows/build_g++.yaml)
[![Build clang++](https://github.com/madamskip1/hc-FSM/actions/workflows/build_clang++.yaml/badge.svg?branch=main)](https://github.com/madamskip1/hc-FSM/actions/workflows/build_clang++.yaml)
[![Build MSVC](https://github.com/madamskip1/hc-FSM/actions/workflows/build_msvc.yaml/badge.svg?branch=main)](https://github.com/madamskip1/hc-FSM/actions/workflows/build_msvc.yaml)

## Features

|Feature| Status | Added in |
|-|:-:|:-:|
|  Basic transition | ✔  | |
|  Automatic transition | ✔  | |
|  Hierarchical SM (substates) | ✔  | |
|  onEntry/onExit action | ✔  | |
|  Transition action | ✔  | |
|  Transition Guard | ✔  | |
|  Set initial state | ✔  | |

## Documentation

Documentation can be found under [doc](https://github.com/madamskip1/hc-FSM/tree/main/examples/).


## Example

### Simple usage example

Provided example illustrates implementation for simple light control system (or what ever what can be on and off). The system involves a light with two states: `Off` and `On`. The transition between them (toggle) is triggered by a `ButtonClicked` event.

<p align="center">
  <img src="examples/simple_example/simple_example_diagram.png" height="150px">  
</p>

```c++
struct On {};
struct Off {};
struct ButtonClicked {};

using transitions_table = hcFSM::TransitionsTable<
	hcFSM::Transition<Off, ButtonClicked, On>,
	hcFSM::Transition<On, ButtonClicked, Off>
>;

int main()
{
	auto light = hcFSM::StateMachine<transitions_table> {};
	// initial state is Off

	light.handleEvent<ButtonClicked>(); // transit to On
	light.handleEvent(ButtonClicked{}); // transit to Off
	auto buttonClickedEvent = ButtonClicked{};
	light.handleEvent(buttonClickedEvent); // transit to On

	return 0;
}
```

### More complex examples

More complex usage examples can be found in [examples dir](https://github.com/madamskip1/hc-FSM/tree/main/examples/):
- NPC AI -
- Elevator

## Integration/Installation

[![Realase version](https://badgen.net/github/release/madamskip1/hc-FSM/)](https://github.com/madamskip1/hc-FSM/releases)

The hc-FSM library provides at least two convenient methods for integration.
First, single header can be directly included in a project.
Alternatively, for a more organized approach or when dealing with larger projects, the library can be effortlessly integrated using CMake's FetchContent. This allows for a easy incorporation into a build process with automatic downloading hc-FSM library.

### Single header
1. Download single header and put into a project
2. Include it in files that use this library
```c++
 #include "{path}/hc-fsm.h"
```

### CMake's FetchContent
This method automatically download hc-FSM library during CMake's configuration and add it to given project. To use [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html), CMake >= 3.11 is required.

```cmake
include(FetchContent)

# Download hc-FSM
FetchContent_Declare(
    hc_fsm
    GIT_REPOSITORY https://https://github.com/madamskip1/hc-FSM.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(hc_fsm)

# link hc-FSM to target/project
target_link_libraries(target_project_name PRIVATE hc_fsm)
```
