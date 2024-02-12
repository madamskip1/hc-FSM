#include <hcFSM/hcFSM.h>
#include "states.h"
#include "events.h"
#include "transitions_action.h"


using hostileNPC_transitions_table = hcFSM::TransitionsTable<
	hcFSM::Transition<Spawn, hcFSM::AUTOMATIC_TRANSITION, Wandering, SpawnedStartWandering>,
	hcFSM::Transition<Wandering, Died, Death>,
	hcFSM::Transition<Wandering, PlayerSpotted, Fight>,
	hcFSM::Transition<Wandering, LowHealth, RestoreHealth>,
	hcFSM::Transition<Fight, PlayerKilled, Wandering>,
	hcFSM::Transition<Fight, PlayerFled, Wandering>,
	hcFSM::Transition<Fight, Died, Death>,
	hcFSM::Transition<Fight, LowHealth, Escape, LowHealthTryEscape>,
	hcFSM::Transition<RestoreHealth, HighHealth, Wandering>,
	hcFSM::Transition<RestoreHealth, PlayerWithinRange, Escape>,
	hcFSM::Transition<RestoreHealth, Died, Death>,
	hcFSM::Transition<Escape, HighHealth, Fight>,
	hcFSM::Transition<Escape, Died, Death>,
	hcFSM::Transition<Escape, PlayerOutOfRange, RestoreHealth>
>;

int main()
{
	auto hostileNPC = hcFSM::StateMachine<hostileNPC_transitions_table> {};
	
	if (hostileNPC.isInState<Wandering>())
	{
		std::cout << "I'm kind NPC and looking for a player to have some fun." << std::endl;
	}

	hostileNPC.handleEvent<PlayerSpotted>();

	// Player hit the NPC
	// Player hit the NPC -> NPC's health is low
	hostileNPC.handleEvent<LowHealth>();

	// Player hit once more
	hostileNPC.handleEvent<Died>();
}