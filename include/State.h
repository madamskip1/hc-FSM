#pragma once
#include "Event.h"
#include <iostream>

namespace FSM
{
	class State
	{
	public:
		virtual ~State() = default;

		virtual void onEntry() {};
		virtual void onExit() {};
		
		//template <typename T>
		//void handleEvent(T param) { std::cout << "base::handle \n"; }

		//virtual void handleEvent(Event&&) { std::cout << "base::handle::&\n"; };
		virtual void handleEvent(Event&) { std::cout << "base::handle::&&\n"; };
		virtual void print() const { std::cout << "State base \n"; };
		State() = default;
	};
}


