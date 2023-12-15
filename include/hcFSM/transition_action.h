#pragma once

namespace FSM
{
    #define CREATE_TRANSITION_ACTION(Name, SourceStateParam, EventParam, TargetStateParam, MethodBody) \
        struct Name { \
            template <typename SourceStateParamType, typename EventParamType, typename TargetStateParamType> \
            void operator()(SourceStateParamType& SourceStateParam, EventParamType& EventParam, TargetStateParamType& TargetStateParam) \
                MethodBody \
        };
}