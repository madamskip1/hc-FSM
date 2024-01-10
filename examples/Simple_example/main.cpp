#include <iostream>
#include <hcFSM/hcFSM.h>

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

    std::cout << "Am I in state ON: " << (light.isInState<On>() ? "Yes" : "No") << std::endl;
	return 0;
}
