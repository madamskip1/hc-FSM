#pragma once

namespace hcFSM
{
    template <typename Transitions_Table, typename InitialState = void, bool CallOnEntryOnInitialState = false>
    class StateMachine;
}