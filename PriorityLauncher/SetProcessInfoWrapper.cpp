// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#include "stdafx.h"
#include "SetProcessInfoWrapper.h"


SetProcessInfoWrapper::SetProcessInfoWrapper()
{
    ntdllModule = LoadLibrary(L"ntdll.dll");
    if (ntdllModule != NULL)
    {
        setProcessInformation = (NtSetProcessInformation)GetProcAddress(ntdllModule, "NtSetInformationProcess");
    }
}


SetProcessInfoWrapper::~SetProcessInfoWrapper()
{
    setProcessInformation = NULL;
    if (ntdllModule != NULL)
    {
        FreeLibrary(ntdllModule);
        ntdllModule = NULL;
    }
}

bool SetProcessInfoWrapper::SetProcessInformation(HANDLE process, int ProcessInformationClass, LPVOID ProcessInformation, ULONG ProcessInformationLength)
{
    if (setProcessInformation != NULL)
    {
        return (setProcessInformation)(process,
                                       ProcessInformationClass,
                                       ProcessInformation,
                                       ProcessInformationLength) != FALSE;
    }
    else
    {
        return false;
    }
}
