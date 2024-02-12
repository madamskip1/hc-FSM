#pragma once

#include <iostream>
#include <hcFSM/hcFSM.h>

#include "events.h"

struct Spawn 
{
    void onEntry() { std::cout << "I'm spawning." << std::endl; }
};

struct Fight {
    void onEntry(PlayerSpotted) { std::cout << "Player spotted. I'm gonna kill him." << std::endl; }
};

struct Death {
    void onEntry() { std::cout << "You have won… Maiev. But the huntress… is nothing without the hunt. You… are nothing… without me." << std::endl; }
};

struct Wandering {};
struct RestoreHealth {};
struct Escape {};

