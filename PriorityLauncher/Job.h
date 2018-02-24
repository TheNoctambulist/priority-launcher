// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#pragma once
class Job
{
public:
    Job();
    ~Job();

    bool AddProcess(HANDLE process);
private:
    HANDLE jobHandle;
};

