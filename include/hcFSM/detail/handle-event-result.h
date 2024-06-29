#pragma once
namespace hcFSM
{
    enum class HandleEventResult
    {
        PROCESSED,
        PROCESSED_SAME_STATE, // Event processed, but there is no need to change state, for e.g. StateA (EventA) -> StateA
        NO_VALID_TRANSITION,
        GUARD_FAILED,

        // INTERNAL USED
        EXIT_INNER_STATE_MACHINE,
        EXIT_AUTOMATIC_INNER_STATE_MACHINE,
    };

    constexpr bool isEventResultOk(HandleEventResult result)
    {
        return result == HandleEventResult::PROCESSED ||
            result == HandleEventResult::PROCESSED_SAME_STATE ||
            result == HandleEventResult::EXIT_AUTOMATIC_INNER_STATE_MACHINE ||
            result == HandleEventResult::EXIT_INNER_STATE_MACHINE;
    }
}
