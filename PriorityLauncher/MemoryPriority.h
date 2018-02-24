// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#pragma once
#include "SetProcessInfoWrapper.h"

namespace MemoryPriority
{
    enum MemoryPriority
    {
        VeryLow = 1,
        Low,
        Medium,
        BelowNormal,
        Normal,
        High,
        Highest
    };

    class MemoryPriorityManager : protected SetProcessInfoWrapper
    {
    public:
        MemoryPriorityManager(HANDLE process);
        ~MemoryPriorityManager();

        bool SetPriority(MemoryPriority priority);
    private:
        HANDLE process;

        static const int ProcessMemoryPriority = 0x27;
    };

}
