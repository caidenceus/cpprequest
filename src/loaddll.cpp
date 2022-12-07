#include "loaddll.h"

WSAS fWSAStartup;
WSASo fWSASocket;
WSAAS fWSAAsyncSelect;
WSAFDIS __fWSAFDIsSet;
WSAI fWSAIoctl;
WSAGLE fWSAGetLastError;
WSAC fWSACleanup;
SOCK fsocket;
IOCTLS fioctlsocket;
CON fconnect;
INTOA finet_ntoa;
IADDR finet_addr;
HTONS fhtons;
HTONL fhtonl;
NTOHS fntohs;
NTOHL fntohl;
SEND fsend;
SENDTO fsendto;
RECV frecv;
RECVFROM frecvfrom;
BIND fbind;
SEL fselect;
LIS flisten;
ACC faccept;
SSO fsetsockopt;
GSN fgetsockname;
GHN fgethostname;
GHBN fgethostbyname;
GHBA fgethostbyaddr;
GPN fgetpeername;
CLSO fclosesocket;
GSOCKOPT fgetsockopt;

BOOL LoadDLLs(void)
{
    BOOL dlls_loaded { FALSE };

    HMODULE ws2_32_dll = LoadLibrary(TEXT("ws2_32.dll"));
    if (ws2_32_dll) {
        fWSAStartup      = (WSAS)GetProcAddress(ws2_32_dll, "WSAStartup");
        fWSASocket       = (WSASo)GetProcAddress(ws2_32_dll, "WSASocketA");
        fWSAAsyncSelect  = (WSAAS)GetProcAddress(ws2_32_dll, "WSAAsyncSelect");
        __fWSAFDIsSet    = (WSAFDIS)GetProcAddress(ws2_32_dll, "__WSAFDIsSet");
        fWSAIoctl        = (WSAI)GetProcAddress(ws2_32_dll, "WSAIoctl");
        fWSAGetLastError = (WSAGLE)GetProcAddress(ws2_32_dll, "WSAGetLastError");
        fWSACleanup      = (WSAC)GetProcAddress(ws2_32_dll, "WSACleanup");
        fsocket          = (SOCK)GetProcAddress(ws2_32_dll, "socket");
        fioctlsocket     = (IOCTLS)GetProcAddress(ws2_32_dll, "ioctlsocket");
        fconnect         = (CON)GetProcAddress(ws2_32_dll, "connect");
        finet_ntoa       = (INTOA)GetProcAddress(ws2_32_dll, "inet_ntoa");
        finet_addr       = (IADDR)GetProcAddress(ws2_32_dll, "inet_addr");
        fhtons           = (HTONS)GetProcAddress(ws2_32_dll, "htons");
        fhtonl           = (HTONL)GetProcAddress(ws2_32_dll, "htonl");
        fntohs           = (NTOHS)GetProcAddress(ws2_32_dll, "ntohs");
        fntohl           = (NTOHL)GetProcAddress(ws2_32_dll, "ntohl");
        fsend            = (SEND)GetProcAddress(ws2_32_dll, "send");
        fsendto          = (SENDTO)GetProcAddress(ws2_32_dll, "sendto");
        frecv            = (RECV)GetProcAddress(ws2_32_dll, "recv");
        frecvfrom        = (RECVFROM)GetProcAddress(ws2_32_dll, "recvfrom");
        fbind            = (BIND)GetProcAddress(ws2_32_dll, "bind");
        fselect          = (SEL)GetProcAddress(ws2_32_dll, "select");
        flisten          = (LIS)GetProcAddress(ws2_32_dll, "listen");
        faccept          = (ACC)GetProcAddress(ws2_32_dll, "accept");
        fsetsockopt      = (SSO)GetProcAddress(ws2_32_dll, "setsockopt");
        fgetsockname     = (GSN)GetProcAddress(ws2_32_dll, "getsockname");
        fgethostname     = (GHN)GetProcAddress(ws2_32_dll, "gethostname");
        fgethostbyname   = (GHBN)GetProcAddress(ws2_32_dll, "gethostbyname");
        fgethostbyaddr   = (GHBA)GetProcAddress(ws2_32_dll, "gethostbyaddr");
        fgetpeername     = (GPN)GetProcAddress(ws2_32_dll, "getpeername");
        fclosesocket     = (CLSO)GetProcAddress(ws2_32_dll, "closesocket");
        fgetsockopt      = (GSOCKOPT)GetProcAddress(ws2_32_dll, "getsockopt");

        if (fWSAStartup && fWSASocket && fWSAAsyncSelect && fWSAIoctl && fWSAGetLastError 
            && fWSACleanup && fsocket && fioctlsocket && fconnect && finet_ntoa && finet_addr
            && fhtons && fhtonl && fntohs && fsend && fsendto && frecv && frecvfrom && fbind
            && fselect && flisten && faccept && fsetsockopt && fgetsockname && fgethostname
            && fgethostbyname && fgethostbyaddr && fclosesocket && fgetsockopt) {
            dlls_loaded = TRUE;
        }
    }

    return dlls_loaded;
}
