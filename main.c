#include <stdio.h>
#include <stdbool.h>

#include <WinSock2.h>

#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include <tlhelp32.h>

#include "lib/sds/sds.h"
#include "lib/cvector/cvector.h"
#include "types.h"



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



cvector_vector_type(unsigned int) FindRunningPids(sds processName) {
    bool procRunning = false;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    cvector_vector_type(unsigned int) pids = NULL;

    if (!(hProcessSnap == INVALID_HANDLE_VALUE)) {
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hProcessSnap, &pe32)) {
            bool isProcessFound = IncludesStringWCS(pe32.szExeFile, sizeof(pe32.szExeFile), processName);

            if (isProcessFound) {
                cvector_push_back(pids, pe32.th32ProcessID);
            }

            while (Process32Next(hProcessSnap, &pe32)) {
                bool isProcessFound = IncludesStringWCS(&pe32.szExeFile, sizeof(pe32.szExeFile), processName);

                if (isProcessFound) {
                    cvector_push_back(pids, pe32.th32ProcessID);
                }
            }

            CloseHandle(hProcessSnap);
        }
    }

    return pids;
}

void main() {
    sds processName = sdsnew("brave.exe");
    cvector_vector_type(unsigned int) pids = FindRunningPids(processName);

    GetListOfLocalPorts();

    cvector_free(pids);
    sdsfree(processName);
}