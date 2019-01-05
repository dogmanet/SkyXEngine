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


#pragma pack(push, 1)
struct msg_header_s
{
	uint16_t usSeq;
	uint16_t usAck;
	byte i8SPort;
	byte isReliable : 1;
};
#pragma pack(pop)

#define PACKET_MAX_SIZE 508
#define PACKET_HDR_SIZE sizeof(msg_header_s)


#endif
