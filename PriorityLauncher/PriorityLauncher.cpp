// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#include "stdafx.h"
#include "ArgParser.h"
#include "CpuPriority.h"
#include "IoPriority.h"
#include "MemoryPriority.h"
#include "Job.h"

//
// Type definitions
//
typedef ArgParser::EnumArgParser<CpuPriority::CpuPriority> CpuArgParser;
typedef ArgParser::EnumArgParser<IoPriority::IoPriority> IoArgParser;
typedef ArgParser::EnumArgParser<MemoryPriority::MemoryPriority> MemoryArgParser;

//
// Forward declarations
//
void PrintHelp();
void TrimCommandLine(std::wstring&, std::wstring);
void InitialiseArgs(CpuArgParser&);
void InitialiseArgs(IoArgParser&);
void InitialiseArgs(MemoryArgParser&);
bool StartProcess(const std::wstring& cmdLine,
                  int priorityClass,
                  int processCreationFlags,
	              STARTUPINFO* startupInfo,
                  PROCESS_INFORMATION* processInfo);
//
// Argument parsers
//
ArgParser::SwitchArgParser helpArgParser(L"-?");
ArgParser::SwitchArgParser waitArgParser(L"-w");
ArgParser::SwitchArgParser hiddenArgParser(L"-h");
ArgParser::SwitchArgParser minArgParser(L"-min");
CpuArgParser cpuArgParser(L"-c");
IoArgParser ioArgParser(L"-i");
MemoryArgParser memArgParser(L"-m");

//
// Main function
//
int WINAPI WinMain(HINSTANCE hInst,
                   HINSTANCE hPrev,
                   LPSTR lpCmdLine,
                   int show)
{
    // CreateProcess configuration
    bool waitForExit = false;
    int priorityClass = 0;
    int processCreationFlags = 0;

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	std::wstring cmdLine(GetCommandLineW());

	// Argument parsing
    InitialiseArgs(cpuArgParser);
    InitialiseArgs(ioArgParser);
    InitialiseArgs(memArgParser);

    LPWSTR *args;
    int numArgs;
    args = CommandLineToArgvW(GetCommandLineW(), &numArgs);

    // First trim off the current application name
    TrimCommandLine(cmdLine, args[0]);

    for (int i = 1; i < numArgs; i++)
    {
        std::wstring arg(args[i]);

        if (cpuArgParser.IsSwitch(arg))
        {
            i++;
            if (i == numArgs)
            {
                PrintHelp();
                return 1;
            }

            std::wstring param(args[i]);
            
            CpuPriority::CpuPriority cpuPriority;
            if (cpuArgParser.TryParse(param, cpuPriority))
            {
                // CPU Priority can be passed into CreateProcess
                priorityClass = cpuPriority;
            }
            else
            {
                PrintHelp();
                return 1;
            }
        }
        else if (ioArgParser.IsSwitch(arg))
        {
            i++;
            if (i == numArgs)
            {
                PrintHelp();
                return 1;
            }

            std::wstring param(args[i]);

            IoPriority::IoPriority ioPriority;
            if (ioArgParser.TryParse(param, ioPriority))
            {
                // IO Priority must be inherited
                IoPriority::IoPriorityManager ioPriorityManager(GetCurrentProcess());
                ioPriorityManager.SetPriority(ioPriority);
            }
            else
            {
                PrintHelp();
                return 1;
            }
        }
        else if (memArgParser.IsSwitch(arg))
        {
            i++;
            if (i == numArgs)
            {
                PrintHelp();
                return 1;
            }

            std::wstring param(args[i]);

            MemoryPriority::MemoryPriority memPriority;
            if (memArgParser.TryParse(param, memPriority))
            {
                // Memory priority must be inherited
                MemoryPriority::MemoryPriorityManager memPriorityManager(GetCurrentProcess());
                memPriorityManager.SetPriority(memPriority);
            }
            else
            {
                PrintHelp();
                return 1;
            }
        }
        else if (waitArgParser.IsSwitch(arg))
        {
            waitForExit = true;
        }
        else if (hiddenArgParser.IsSwitch(arg))
        {
            processCreationFlags |= CREATE_NO_WINDOW;
        }
		else if (minArgParser.IsSwitch(arg))
		{
			si.dwFlags |= STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_MINIMIZE;
		}
        else if (helpArgParser.IsSwitch(arg))
        {
            PrintHelp();
            return 0;
        }
        else
        {
            // Not one of our arguments, therefore it must be the process
            // to launch.
            break;
        }


        // Trim the command we just processed from the launch command line
        TrimCommandLine(cmdLine, args[i]);
    }

    LocalFree(args);

    // Trim off any remaining whitespace
    auto firstChar = cmdLine.find_first_not_of(L" ");
    if (firstChar == std::wstring::npos)
    {
        PrintHelp();
        return 1;
    }
    cmdLine = cmdLine.substr(firstChar);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    if (StartProcess(cmdLine,
                     priorityClass,
                     processCreationFlags,
		             &si,
                     &pi))
    {
        DWORD exitCode = 0;
        if (waitForExit)
        {
            // Configure to terminate the child if we are killed
            Job job;
            job.AddProcess(pi.hProcess);

            WaitForSingleObject(pi.hProcess, INFINITE);

            GetExitCodeProcess(pi.hProcess, &exitCode);
        }
        else
        {
            exitCode = pi.dwProcessId;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode;
    }
    else
    {
        std::wcerr << L"Unable to launch process: " << cmdLine << std::endl;
        return 1;
    }
}

void PrintHelp()
{
    std::wcout << L"Launches a process with specific CPU, IO, and Memory priorities." << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Usage: PriorityLauncher.exe [-?] [-w] [-h] [-min] [-c CPUPRIORITY] [-i IOPRIORITY] [-m MEMPRIORITY] app [args ...]" << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Options:" << std::endl;
    std::wcout << L"   -?              Show this help and exit." << std::endl;
    std::wcout << L"   -w              Wait for the launched process to terminate before exiting." << std::endl;
    std::wcout << L"                   If specified the return code is the exit code of the launched process." << std::endl;
    std::wcout << L"                   The launched process will be terminated if the parent PriorityLauncher is terminated." << std::endl;
    std::wcout << L"                   If not specified the return code is the PID of the launched process." << std::endl;
    std::wcout << L"   -h              Launch the process as a hidden process." << std::endl;
	std::wcout << L"   -min            Launch the process minimised." << std::endl;
	std::wcout << L"   -c CPUPRIORITY  Set the CPU priority for the launched process (Idle, Below, Normal, Above, High, Realtime)." << std::endl;
    std::wcout << L"   -i IOPRIORITY   Set the IO priority for the launched process (VeryLow, Low, Normal)." << std::endl;
    std::wcout << L"   -m MEMPRIORITY  Set the memory priority for the launched process (VeryLow, Low, Medium, Below, Normal)." << std::endl;
    std::wcout << L"   app             The application to launch." << std::endl;
    std::wcout << L"   args            Optional arguments to pass to the launched process." << std::endl;
}

void TrimCommandLine(std::wstring& cmdLine, std::wstring arg)
{
    // Trim the arg and any trailing quotation mark
    // Note +1 without validation is ok since the arg will be followed by
    // either a quotation mark or whitespace both of which can be trimmed.
    auto argLocation = cmdLine.find(arg);
    if (argLocation != std::wstring::npos && 
        argLocation + arg.length() + 1 < cmdLine.length())
    {
        cmdLine = cmdLine.substr(cmdLine.find(arg) + arg.length() + 1);
    }
    else
    {
        // Off the end so just return empty
        cmdLine = L"";
    }
}

void InitialiseArgs(CpuArgParser& argParser)
{
    argParser.AddValue(CpuPriority::Idle, L"Idle");
    argParser.AddValue(CpuPriority::BelowNormal, L"Below");
    argParser.AddValue(CpuPriority::Normal, L"Normal");
    argParser.AddValue(CpuPriority::AboveNormal, L"Above");
    argParser.AddValue(CpuPriority::High, L"High");
    argParser.AddValue(CpuPriority::Realtime, L"Realtime");
}

void InitialiseArgs(IoArgParser& argParser)
{
    argParser.AddValue(IoPriority::VeryLow, L"VeryLow");
    argParser.AddValue(IoPriority::Low, L"Low");
    argParser.AddValue(IoPriority::Normal, L"Normal");
}

void InitialiseArgs(MemoryArgParser& argParser)
{
    argParser.AddValue(MemoryPriority::VeryLow, L"VeryLow");
    argParser.AddValue(MemoryPriority::Low, L"Low");
    argParser.AddValue(MemoryPriority::Medium, L"Medium");
    argParser.AddValue(MemoryPriority::BelowNormal, L"Below");
    argParser.AddValue(MemoryPriority::Normal, L"Normal");
    argParser.AddValue(MemoryPriority::High, L"High");
    argParser.AddValue(MemoryPriority::Highest, L"Highest");
}

bool StartProcess(const std::wstring& cmdLine,
                  int priorityClass,
                  int processCreationFlags,
	              STARTUPINFO* startupInfo,
                  PROCESS_INFORMATION* processInfo)
{
    // CreateProcess may modify the command line!
    std::vector<wchar_t> cmdLineData(cmdLine.begin(), cmdLine.end());
    cmdLineData.push_back(L'\0');

    return CreateProcess(NULL,
                         cmdLineData.data(),
                         NULL,
                         NULL,
                         FALSE,
                         priorityClass | processCreationFlags,
                         NULL,
                         NULL,
                         startupInfo,
                         processInfo) != FALSE;
}