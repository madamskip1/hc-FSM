#pragma once

namespace hcFSM
{
    #define CREATE_TRANSITION_GUARD(Name, SourceStateParam, EventParam,  MethodBody) \
        struct Name { \
            template <typename SourceStateParamType, typename EventParamType> \
            const bool operator()(SourceStateParamType& SourceStateParam, EventParamType& EventParam) \
                MethodBody \
        };
}