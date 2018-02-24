# Priority Launcher

A utility to launch a process with a specified CPU, Memory, and IO priority. Can also be used to launch a hidden or minimised process.

## Usage


```
Usage: PriorityLauncher.exe [-?] [-w] [-h] [-c CPUPRIORITY] [-i IOPRIORITY] [-m MEMPRIORITY] app [args ...]

Options:
   -?              Show this help and exit.
   -w              Wait for the launched process to terminate before exiting.
                   If specified the return code is the exit code of the launched process.
                   The launched process will be terminated if the parent PriorityLauncher is terminated.
                   If not specified the return code is the PID of the launched process.
   -h              Launch the process as a hidden process.
   -min            Launch the process minimised.
   -c CPUPRIORITY  Set the CPU priority for the launched process (Idle, Below, Normal, Above, High, Realtime).
   -i IOPRIORITY   Set the IO priority for the launched process (VeryLow, Low, Normal).
   -m MEMPRIORITY  Set the memory priority for the launched process (VeryLow, Low, Medium, Below, Normal).
   app             The application to launch.
   args            Optional arguments to pass to the launched process.
```

Priority Launcher is a command line program however, to avoid unwanted command line windows when launched from Task Scheduler (or other similar tools), it doesn't attach to any command line output. To view the command line help simply pipe into the `more` program:
```
PriorityLauncher.exe -? | more
```
