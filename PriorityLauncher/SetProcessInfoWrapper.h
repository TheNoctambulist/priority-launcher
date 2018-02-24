// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#pragma once
class SetProcessInfoWrapper
{
public:
    SetProcessInfoWrapper();
    ~SetProcessInfoWrapper();

    bool SetProcessInformation(HANDLE process,
                               int processInformationClass,
                               LPVOID processInformation,
                               ULONG processInformationLength);

private:
    typedef BOOL(WINAPI *NtSetProcessInformation)(HANDLE process,
                                                  int processInformationClass,
                                                  LPVOID processInformation,
                                                  ULONG processInformationLength);

    HMODULE ntdllModule;
    NtSetProcessInformation setProcessInformation;
};

