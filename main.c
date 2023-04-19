#include <stdio.h>
#include <stdbool.h>

#include "lib/sds/sds.h"
#include "lib/cvector/cvector.h"
#include "ports.h"
#include "types.h"
#include "process.h"

void main() {
    sds processName = sdsnew("brave.exe");
    cvector_vector_type(unsigned int) pids = FindRunningPids(processName);

    GetListOfLocalPorts();

    cvector_free(pids);
    sdsfree(processName);
}