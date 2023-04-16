#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdbool.h>
#include "lib/sds/sds.h"

sds ConvertWCToS(WCHAR* str, size_t oldStrSize) {
    size_t i = 0;
    char* newStr = (char*)malloc(oldStrSize);

    wcstombs_s(i, newStr, oldStrSize, str, oldStrSize - 1);

    sds sdsStr = sdsnew(newStr);

    free(newStr);

    return sdsStr;
}

bool IncludesStringSS(sds that, sds str) {
    return sdscmp(that, str) == 0;
}

bool IncludesStringWCS(WCHAR* that, size_t thatSize, sds str) {
    sds thatConverted = ConvertWCToS(that, thatSize);
    bool isIncluding = IncludesStringSS(thatConverted, str);
    sdsfree(thatConverted);

    return isIncluding;
}

bool FindRunningProcess(sds processName) {
    /*
    Function takes in a string value for the process it is looking for like ST3Monitor.exe
    then loops through all of the processes that are currently running on windows.
    If the process is found it is running, therefore the function returns true.
    */

    bool procRunning = false;

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        procRunning = false;
    }
    else {
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hProcessSnap, &pe32)) { // Gets first running process
            bool isProcessFound = IncludesStringWCS(pe32.szExeFile, sizeof(pe32.szExeFile), processName);

            if (isProcessFound) {
                procRunning = true;
            } else {
                // loop through all running processes looking for process
                while (Process32Next(hProcessSnap, &pe32)) {
                    bool isProcessFound = IncludesStringWCS(&pe32.szExeFile, sizeof(pe32.szExeFile), processName);

                    if (isProcessFound) {
                        // if found process is running, set to true and break from loop
                        procRunning = true;
                        break;
                    }
                }
            }
            // clean the snapshot object
            CloseHandle(hProcessSnap);
        }
    }

    return procRunning;
}

void main() {
    sds processName = sdsnew("brave.exe");

    if (FindRunningProcess(processName)) {
        printf("IsRunning");
    }
    else {
        printf("IsNotRunning");
    }

    sdsfree(processName);
}