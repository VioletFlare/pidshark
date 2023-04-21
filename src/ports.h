#ifndef PIDSHARK_PORTS_H
#define PIDSHARK_PORTS_H

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include "../lib/cvector/cvector.h"
#include "types.h";

size_t EnrichPidPortVectorTCP4(cvector_vector_type(pid_port)* v) {
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
        pidPort.type = sdsnew("TCP");
        cvector_push_back(*v, pidPort);
    }

    size_t vectorSize = pTCP4Info->dwNumEntries;

    free(pTCP4Info);

    return vectorSize;
}

size_t EnrichPidPortVectorTCP6(cvector_vector_type(pid_port)* v) {
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
        pidPort.type = sdsnew("TCP");
        cvector_push_back(*v, pidPort);
    }

    size_t vectorSize = pTCP6Info->dwNumEntries;

    free(pTCP6Info);

    return vectorSize;
}

size_t EnrichPidPortVectorUDP4(cvector_vector_type(pid_port)* v) {
    DWORD size = 0;
    DWORD dwResult = 0;
    MIB_UDPTABLE_OWNER_PID* pUDPInfo;
    MIB_UDPROW_OWNER_PID* ownerUDP;

    dwResult = GetExtendedUdpTable(NULL, &size, true, AF_INET, UDP_TABLE_OWNER_PID, 0);
    pUDPInfo = (MIB_UDPTABLE_OWNER_PID*)malloc(size);
    dwResult = GetExtendedUdpTable(pUDPInfo, &size, true, AF_INET, UDP_TABLE_OWNER_PID, 0);

    for (DWORD dwLoop = 0; dwLoop < pUDPInfo->dwNumEntries; dwLoop++)
    {
        pid_port pidPort;
        ownerUDP = &pUDPInfo->table[dwLoop];
        pidPort.pid = ownerUDP->dwOwningPid;
        pidPort.port = ntohs(ownerUDP->dwLocalPort);
        pidPort.type = sdsnew("UDP");
        cvector_push_back(*v, pidPort);
    }

    size_t vectorSize = pUDPInfo->dwNumEntries;

    free(pUDPInfo);

    return vectorSize;
}

size_t EnrichPidPortVectorUDP6(cvector_vector_type(pid_port)* v) {
    DWORD size = 0;
    DWORD dwResult = 0;
    MIB_UDP6TABLE_OWNER_PID* pUDP6Info;
    MIB_UDP6ROW_OWNER_PID* ownerUDP6;

    dwResult = GetExtendedUdpTable(NULL, &size, true, AF_INET6, UDP_TABLE_OWNER_PID, 0);
    pUDP6Info = (MIB_UDP6TABLE_OWNER_PID*)malloc(size);
    dwResult = GetExtendedUdpTable(pUDP6Info, &size, true, AF_INET6, UDP_TABLE_OWNER_PID, 0);

    for (DWORD dwLoop = 0; dwLoop < pUDP6Info->dwNumEntries; dwLoop++)
    {
        pid_port pidPort;
        ownerUDP6 = &pUDP6Info->table[dwLoop];
        pidPort.pid = ownerUDP6->dwOwningPid;
        pidPort.port = ntohs(ownerUDP6->dwLocalPort);
        pidPort.type = sdsnew("UDP");
        cvector_push_back(*v, pidPort);
    }

    size_t vectorSize = pUDP6Info->dwNumEntries;

    free(pUDP6Info);

    return vectorSize;
}

size_t GetListOfLocalPorts(cvector_vector_type(pid_port)* v) {
    size_t size = 0;

    size += EnrichPidPortVectorTCP4(v);
    size += EnrichPidPortVectorTCP6(v);
    size += EnrichPidPortVectorUDP4(v);
    size += EnrichPidPortVectorUDP6(v);

    return size;
}

#endif