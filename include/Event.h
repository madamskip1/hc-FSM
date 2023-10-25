#pragma once
#include <iostream>

namespace FSM
{
	class Event
	{
	public:
		virtual void print()
		{
			std::cout << "Event \n";
		}
	protected:
		Event() = default;
	};
}

