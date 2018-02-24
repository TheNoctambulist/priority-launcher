// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#pragma once
#include "SetProcessInfoWrapper.h"

namespace IoPriority
{
    enum IoPriority
    {
        VeryLow = 0,
        Low,
        Normal
    };

    class IoPriorityManager : protected SetProcessInfoWrapper
    {
    public:
        IoPriorityManager(HANDLE process);
        ~IoPriorityManager();

        bool SetPriority(IoPriority priority);
    private:
        HANDLE process;

        static const int ProcessIoPriority = 0x21;
    };
}
