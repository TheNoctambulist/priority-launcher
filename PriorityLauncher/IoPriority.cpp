// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#include "stdafx.h"
#include "IoPriority.h"


IoPriority::IoPriorityManager::IoPriorityManager(HANDLE process) 
    : process(process)
{
}


IoPriority::IoPriorityManager::~IoPriorityManager()
{
}

bool IoPriority::IoPriorityManager::SetPriority(IoPriority priority)
{
    ULONG lPriority = priority;
    return this->SetProcessInformation(process,
                                       ProcessIoPriority,
                                       &lPriority,
                                       sizeof(lPriority));
}
