#include <windows.h>

#include "lib/cvector/cvector.h"
#include "types.h";

void EnrichPidPortVectorTCP4(cvector_vector_type(pid_port)* v) {
    DWORD size = 0;
    DWORD dwResult = 0;
    MIB_TCPTABLE_OWNER_PID* pTCP4Info;
    MIB_TCPROW_OWNER_PID* ownerTCP4;

    dwResult = GetExtendedTcpTable(NULL, &size, true, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    pTCP4Info = (MIB_TCPTABLE_OWNER_PID*)malloc(size);
    dwResult = GetExtendedTcpTable(pTCP4Info, &size, true, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);

    for (DWORD dwLoop = 0; dwLoop < pTCP4Info->dwNumEntries; dwLoop++)
    {
        pid_port pidPort;
        ownerTCP4 = &pTCP4Info->table[dwLoop];
        pidPort.pid = ownerTCP4->dwOwningPid;
        pidPort.port = ntohs(ownerTCP4->dwLocalPort);
        cvector_push_back(*v, pidPort);
    }

    free(pTCP4Info);
}

void EnrichPidPortVectorTCP6(cvector_vector_type(pid_port)* v) {
    DWORD size = 0;
    DWORD dwResult = 0;
    MIB_TCP6TABLE_OWNER_PID* pTCP6Info;
    MIB_TCP6ROW_OWNER_PID* ownerTCP6;

    dwResult = GetExtendedTcpTable(NULL, &size, true, AF_INET6, TCP_TABLE_OWNER_PID_ALL, 0);
    pTCP6Info = (MIB_TCP6TABLE_OWNER_PID*)malloc(size);
    dwResult = GetExtendedTcpTable(pTCP6Info, &size, true, AF_INET6, TCP_TABLE_OWNER_PID_ALL, 0);

    for (DWORD dwLoop = 0; dwLoop < pTCP6Info->dwNumEntries; dwLoop++)
    {
        pid_port pidPort;
        ownerTCP6 = &pTCP6Info->table[dwLoop];
        pidPort.pid = ownerTCP6->dwOwningPid;
        pidPort.port = ntohs(ownerTCP6->dwLocalPort);
        cvector_push_back(*v, pidPort);
    }

    free(pTCP6Info);
}

void EnrichPidPortVectorUDP4(cvector_vector_type(pid_port)* v) {
    DWORD size = 0;
    DWORD dwResult = 0;
    MIB_UDPTABLE_OWNER_PID* pUDPInfo;
    MIB_UDPROW_OWNER_PID* ownerUDP;

    dwResult = GetExtendedUdpTable(NULL, &size, true, AF_INET, UDP_TABLE_OWNER_PID, 0);
    pUDPInfo = (MIB_UDP6TABLE_OWNER_PID*)malloc(size);
    dwResult = GetExtendedUdpTable(pUDPInfo, &size, true, AF_INET, UDP_TABLE_OWNER_PID, 0);

    for (DWORD dwLoop = 0; dwLoop < pUDPInfo->dwNumEntries; dwLoop++)
    {
        pid_port pidPort;
        ownerUDP = &pUDPInfo->table[dwLoop];
        pidPort.pid = ownerUDP->dwOwningPid;
        pidPort.port = ntohs(ownerUDP->dwLocalPort);
        cvector_push_back(*v, pidPort);
    }

    free(pUDPInfo);
}

void EnrichPidPortVectorUDP6(cvector_vector_type(pid_port)* v) {
    DWORD size = 0;
    DWORD dwResult = 0;
    MIB_UDP6TABLE_OWNER_PID* pUDP6Info;
    MIB_UDP6ROW_OWNER_PID* ownerUDP6;

    dwResult = GetExtendedUdpTable(NULL, &size, true, AF_INET, UDP_TABLE_OWNER_PID, 0);
    pUDP6Info = (MIB_UDP6TABLE_OWNER_PID*)malloc(size);
    dwResult = GetExtendedUdpTable(pUDP6Info, &size, true, AF_INET, UDP_TABLE_OWNER_PID, 0);

    for (DWORD dwLoop = 0; dwLoop < pUDP6Info->dwNumEntries; dwLoop++)
    {
        pid_port pidPort;
        ownerUDP6 = &pUDP6Info->table[dwLoop];
        pidPort.pid = ownerUDP6->dwOwningPid;
        pidPort.port = ntohs(ownerUDP6->dwLocalPort);
        cvector_push_back(*v, pidPort);
    }

    free(pUDP6Info);
}

cvector_vector_type(pid_port) GetListOfLocalPorts() {
    cvector_vector_type(pid_port) v = NULL;

    EnrichPidPortVectorTCP4(&v);
    EnrichPidPortVectorTCP6(&v);
    EnrichPidPortVectorUDP4(&v);
    EnrichPidPortVectorUDP6(&v);

    return v;
}