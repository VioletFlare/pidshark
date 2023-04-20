#include <stdio.h>
#include <stdbool.h>

#include "lib/sds/sds.h"
#include "lib/cvector/cvector.h"
#include "ports.h"
#include "types.h"
#include "process.h"

size_t FilterPidPortsByPids(cvector_vector_type(pid_port) *filteredPidPorts, cvector_vector_type(pid_port) pidPorts, size_t pidPortNum, cvector_vector_type(unsigned int) pids, size_t pidsNum) {
    size_t filteredPidPortsNum = 0;

    for (int i = 0; i < pidsNum; i++) {
        for (int j = 0; j < pidPortNum; j++) {
            if (pids[i] == pidPorts[j].pid) {
                cvector_push_back(*filteredPidPorts, pidPorts[j]);
                filteredPidPortsNum++;
            }
        }
    }
    
    return filteredPidPortsNum;
}

void PrintFilters(cvector_vector_type(pid_port) pidPorts, size_t filteredPidPortsNum) {
    for (int i = 0; i < filteredPidPortsNum; i++) {
        bool isTCP = IncludesStringSC(pidPorts[i].type, "TCP");

        if (isTCP) {
            printf("tcp.port == % u", pidPorts[i].port);
        } else {
            printf("udp.port == % u", pidPorts[i].port);
        }

        if (i < (filteredPidPortsNum - 1)) {
            printf(" || ");
        }
    }
}

void main(int argc, char *argv[]) {
    sds processName = sdsnew(argv[1]);
    cvector_vector_type(unsigned int) pids = NULL;
    
    size_t pidsNum = FindRunningPids(&pids, processName);

    sdsfree(processName);

    cvector_vector_type(pid_port) localPidPorts = NULL;

    size_t pidPortNum = GetListOfLocalPorts(&localPidPorts);

    cvector_vector_type(pid_port) filteredPidPorts = NULL;
    size_t filteredPidPortsNum = FilterPidPortsByPids(&filteredPidPorts, localPidPorts, pidPortNum, pids, pidsNum);
    
    cvector_free(pids);
    cvector_free(localPidPorts);

    PrintFilters(filteredPidPorts, filteredPidPortsNum);

    cvector_free(filteredPidPorts);
}