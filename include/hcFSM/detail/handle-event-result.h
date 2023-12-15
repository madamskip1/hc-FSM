#pragma once
namespace hcFSM
{
    enum class HandleEventResult
    {
        PROCESSED,
        PROCESSED_SAME_STATE, // Event processed, but there is no need to change state, for e.g. StateA (EventA) -> StateA
        PROCESSED_INNER_STATE_MACHINE,
        NO_VALID_TRANSITION,
        EXIT_INNER_STATE_MACHINE,
        EXIT_AUTOMATIC_INNER_STATE_MACHINE,
        GUARD_FAILED
    };

    constexpr bool isEventResultOk(HandleEventResult result)
    {
        return result == HandleEventResult::PROCESSED ||
            result == HandleEventResult::PROCESSED_SAME_STATE ||
            result == HandleEventResult::PROCESSED_INNER_STATE_MACHINE;
    }
}
