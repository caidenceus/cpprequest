#pragma once

#include "cpprequest.h"

// ws2_32.dll function variables
typedef int (__stdcall *WSAS)(WORD, LPWSADATA);
typedef SOCKET (__stdcall *WSASo)(int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD);
typedef int (__stdcall *WSAAS)(SOCKET, HWND hWnd, unsigned int, long);
typedef int (__stdcall *WSAFDIS)(SOCKET, fd_set *);
typedef int (__stdcall *WSAI)(SOCKET, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef int (__stdcall *WSAGLE)(void);
typedef int (__stdcall *WSAC)(void);
typedef SOCKET (__stdcall *SOCK)(int, int, int);
typedef int (__stdcall *IOCTLS)(SOCKET, long cmd, u_long *);
typedef int (__stdcall *CON)(SOCKET, const struct sockaddr *, int);
typedef char * FAR (__stdcall *INTOA)(struct in_addr);
typedef unsigned long (__stdcall *IADDR)(const char *);
typedef u_short (__stdcall *HTONS)(u_short);
typedef u_long (__stdcall *HTONL)(u_long);
typedef u_short (__stdcall *NTOHS)(u_short);
typedef u_long (__stdcall *NTOHL)(u_long);
typedef int (__stdcall *SEND)(SOCKET, const char *, int, int);
typedef int (__stdcall *SENDTO)(SOCKET, const char *, int, int, const struct sockaddr *, int);
typedef int (__stdcall *RECV)(SOCKET, char *, int, int);
typedef int (__stdcall *RECVFROM)(SOCKET, char *, int, int, struct sockaddr *, int *);
typedef int (__stdcall *BIND)(SOCKET, const struct sockaddr *, int);
typedef int (__stdcall *SEL)(int, fd_set *, fd_set *, fd_set *, const struct timeval *);
typedef int (__stdcall *LIS)(SOCKET, int);
typedef SOCKET (__stdcall *ACC)(SOCKET, struct sockaddr *, int *);
typedef int (__stdcall *SSO)(SOCKET, int, int, const char *, int);
typedef int (__stdcall *GSN)(SOCKET, struct sockaddr *, int *);
typedef int (__stdcall *GHN)(char *, int);
typedef struct hostent* FAR (__stdcall *GHBN)(const char *);
typedef struct hostent* FAR (__stdcall *GHBA)(const char *, int, int);
typedef int (__stdcall *GPN)(SOCKET, struct sockaddr*, int *);
typedef int (__stdcall *CLSO)(SOCKET);
typedef int(__stdcall* GSOCKOPT)(SOCKET, int, int, char *, int *);

extern WSAS fWSAStartup;
extern WSASo fWSASocket;
extern WSAAS fWSAAsyncSelect;
extern WSAFDIS __fWSAFDIsSet;
extern WSAI fWSAIoctl;
extern WSAGLE fWSAGetLastError;
extern WSAC fWSACleanup;
extern SOCK fsocket;
extern IOCTLS fioctlsocket;
extern CON fconnect;
extern INTOA finet_ntoa;
extern IADDR finet_addr;
extern HTONS fhtons;
extern HTONL fhtonl;
extern NTOHS fntohs;
extern NTOHL fntohl;
extern SEND fsend;
extern SENDTO fsendto;
extern RECV frecv;
extern RECVFROM frecvfrom;
extern BIND fbind;
extern SEL fselect;
extern LIS flisten;
extern ACC faccept;
extern SSO fsetsockopt;
extern GSN fgetsockname;
extern GHN fgethostname;
extern GHBN fgethostbyname;
extern GHBA fgethostbyaddr;
extern GPN fgetpeername;
extern CLSO fclosesocket;
extern GSOCKOPT fgetsockopt;

BOOL LoadDLLs(void);