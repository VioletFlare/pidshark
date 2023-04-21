#ifndef PIDSHARK_PROCESS_H
#define PIDSHARK_PROCESS_H
#include <stdio.h>

#ifndef WINDOWS_H
#define WINDOWS_H
#include <windows.h>
#endif

#include "utils.h"

#include <tlhelp32.h>

 size_t FindRunningPids(cvector_vector_type(unsigned int) *pids, sds processName) {
    bool procRunning = false;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    size_t pidsNum = 0;

    if (!(hProcessSnap == INVALID_HANDLE_VALUE)) {
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hProcessSnap, &pe32)) {
            bool isProcessFound = IncludesStringWCS(pe32.szExeFile, sizeof(pe32.szExeFile), processName);

            if (isProcessFound) {
                cvector_push_back(*pids, pe32.th32ProcessID);
                pidsNum++;
            }

            while (Process32Next(hProcessSnap, &pe32)) {
                bool isProcessFound = IncludesStringWCS(&pe32.szExeFile, sizeof(pe32.szExeFile), processName);

                if (isProcessFound) {
                    cvector_push_back(*pids, pe32.th32ProcessID);
                    pidsNum++;
                }
            }

            CloseHandle(hProcessSnap);
        }
    }

    return pidsNum;
}

#endif