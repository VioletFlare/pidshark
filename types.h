#ifndef PIDSHARK_TYPES_H
#define PIDSHARK_TYPES_H

#ifndef WINDOWS_H
#define WINDOWS_H
#include <windows.h>
#endif

typedef struct pid_port {
	DWORD pid;
	u_short port;
} pid_port;

#endif