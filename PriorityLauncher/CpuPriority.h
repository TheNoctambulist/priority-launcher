// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#pragma once
#include "stdafx.h"

namespace CpuPriority
{
    enum CpuPriority
    {
        Idle = IDLE_PRIORITY_CLASS,
        BelowNormal = BELOW_NORMAL_PRIORITY_CLASS,
        Normal = NORMAL_PRIORITY_CLASS,
        AboveNormal = ABOVE_NORMAL_PRIORITY_CLASS,
        High = HIGH_PRIORITY_CLASS,
        Realtime = REALTIME_PRIORITY_CLASS
    };

}
