// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#include "stdafx.h"
#include "Job.h"


Job::Job()
{
    jobHandle = CreateJobObject(NULL, NULL);

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limitInfo;
    ZeroMemory(&limitInfo, sizeof(limitInfo));

    // Set to kill processes when the job is closed.
    limitInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

    SetInformationJobObject(jobHandle,
                            JobObjectExtendedLimitInformation,
                            &limitInfo,
                            sizeof(limitInfo));
}

Job::~Job()
{
    if (jobHandle)
    {
        CloseHandle(jobHandle);
        jobHandle = NULL;
    }
}

bool Job::AddProcess(HANDLE process)
{
    return AssignProcessToJobObject(jobHandle, process) != FALSE;
}
