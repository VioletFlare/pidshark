#include <stdio.h>
#include <stdbool.h>

#include "lib/sds/sds.h"
#include "lib/cvector/cvector.h"
#include "ports.h"
#include "types.h"
#include "process.h"

cvector_vector_type(pid_port) FilterPidPortsByPids(cvector_vector_type(pid_port) pidPorts, cvector_vector_type(unsigned int) pids) {
    cvector_vector_type(pid_port) filteredPidPorts = NULL;
    
    int pidsLength = cvector_size(pids) / sizeof(unsigned int);
    int pidPortsLength = cvector_size(pidPorts) / sizeof(pid_port);

    for (int i = 0; i < pidsLength; i++) {
        for (int j = 0; j < pidPortsLength; j++) {
            if (pids[i] == pidPorts[j].pid) {
                cvector_push_back(filteredPidPorts, pidPorts[j]);
            }
        }
    }
    
    return filteredPidPorts;
}

void PrintFilters(cvector_vector_type(pid_port) pidPorts) {
    int pidPortsLength = cvector_size(pidPorts) / sizeof(pid_port);

    for (int i = 0; i < pidPortsLength; i++) {
        bool isTCP = IncludesStringSC(pidPorts[i].type, "TCP");

        if (isTCP) {
            printf("tcp.port == % u", pidPorts[i].port);
        } else {
            printf("udp.port == % u", pidPorts[i].port);
        }

        if (i < (pidPortsLength - 1)) {
            printf(" || ");
        }
    }
}

void main() {
    sds processName = sdsnew("brave.exe");
    cvector_vector_type(unsigned int) pids = FindRunningPids(processName);

    sdsfree(processName);

    cvector_vector_type(pid_port) localPidPorts = GetListOfLocalPorts();
    cvector_vector_type(pid_port) filteredPidPorts = FilterPidPortsByPids(localPidPorts, pids);
    
    cvector_free(pids);
    cvector_free(localPidPorts);

    PrintFilters(filteredPidPorts);

    cvector_free(filteredPidPorts);
}