#ifndef __UNET_H
#define __UNET_H

#if defined(_WINDOWS)
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#if defined(_LINUX) || defined(_MAC)
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define closesocket close
#endif

#if defined(_WINDOWS)
#	ifdef errno
#		undef errno
#		define errno WSAGetLastError()
#	endif
#	define EINPROGRESS WSAEINPROGRESS
#	define EWOULDBLOCK WSAEWOULDBLOCK
#endif

#define PACKET_MAX_SIZE 508
#define PACKET_HDR_SIZE 10


#endif
