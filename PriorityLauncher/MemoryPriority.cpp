// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#include "stdafx.h"
#include "MemoryPriority.h"


MemoryPriority::MemoryPriorityManager::MemoryPriorityManager(HANDLE process)
    : process(process)
{
}


MemoryPriority::MemoryPriorityManager::~MemoryPriorityManager()
{
}

bool MemoryPriority::MemoryPriorityManager::SetPriority(MemoryPriority priority)
{
    ULONG lPriority = priority;
    return this->SetProcessInformation(process,
                                       ProcessMemoryPriority,
                                       &lPriority,
                                       sizeof(lPriority));
}
