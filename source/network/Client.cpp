#include "Client.h"

CClient::CClient()
{

}

CClient::~CClient()
{
	disconnect();
}

void CClient::update()
{
	switch(m_connState)
	{
	case CS_DISCONNECTED:
	case CS_CHALLENGE_REQUESTED:
		break;

	case CS_BEGIN:
		m_pNetChannel->sendRaw(&m_server, (byte*)"\x00\x00\x00\x00SkyXEngineGetChallenge\x00", 27);
		m_connState = CS_CHALLENGE_REQUESTED;
		break;

	case CS_CHALLENGE_RECEIVED:
		//\x00\x00\x00\x00C\xXX\xXX\xXX\xXXSkyXEngineAuthorize\x00\xNN\xNN...
		// - XXXXXXXX - received challenge
		// - NNNN - auth ticket length
		// - ... - auth ticket blob

		{
			CNETbuff buf;
			buf.writeUInt32(0);
			buf.writeInt8('C');
			buf.writeUInt32(m_uChallenge);
			buf.writeString("SkyXEngineAuthorize");
			uint16_t usTicketLen;
			byte *pTicketBlob;
			getAuthTicket(&pTicketBlob, &usTicketLen);
			buf.writeUInt16(usTicketLen);
			buf.writeBytes(pTicketBlob, usTicketLen);
			freeAuthTicket(pTicketBlob, usTicketLen);
			m_pNetChannel->sendRaw(&m_server, (byte*)buf.getPointer(), buf.getSize());
			m_connState = CS_CHALLENGE_SENT;
		}

		break;

	//case CS_CHALLENGE_SENT:

	}



	m_pNetChannel->update();
	CNETbuff buf;
	CNetPeer netPeer;
	bool isRaw = false;
	while(m_pNetChannel->readPacket(&buf, NULL, &netPeer, &isRaw))
	{
		if(isRaw)
		{
			switch(buf.readUInt32())
			{
			case 0xFFFFFFFF:
				// ignore that
				break;
			case 0x00000000:
				switch(m_connState)
				{
				case CS_CHALLENGE_REQUESTED:
					m_uChallenge = buf.readUInt32();
					m_connState = CS_CHALLENGE_RECEIVED;
					break;

				case CS_CHALLENGE_SENT:
					m_u8SPort = buf.readUInt8();
					m_connState = CS_CHALLENGE_READY;
					printf("Connection ready\n");
					break;
				}
				break;
			}
		}
		else
		{
			//@TODO: process messages from packet
		}
	}
}

void CClient::connect(const char *szIp, unsigned short usPort)
{
#if defined(_WINDOWS)
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed!\n");
	}
#endif

	if((m_iListener = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(COLOR_LRED "socket"); fprintf(stderr, COLOR_RESET);
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to create socket\n");
		return;
	}

#if defined(_WINDOWS)
	u_long mode = 1;
	ioctlsocket(m_iListener, FIONBIO, &mode);
#else
	int flags = fcntl(m_iListener, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(m_iListener, F_SETFL, flags);
#endif

	sockaddr_in addr;
	addr.sin_port = htons(usPort);
	addr.sin_family = AF_INET;
	m_server.m_uIP = addr.sin_addr.s_addr = inet_addr(szIp);
	m_server.m_uPort = usPort;

	if(::connect(m_iListener, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror(COLOR_LRED "connect"); fprintf(stderr, COLOR_RESET);
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to connect socket\n");
		return;
	}

	

	m_pNetChannel = new CNetChannel(m_iListener);
	m_isConnected = true;
	m_connState = CS_BEGIN;
}

void CClient::disconnect()
{
	mem_delete(m_pNetChannel);
	closesocket(m_iListener);
	m_isConnected = false;
}


bool CClient::isConnected()
{
	return(m_isConnected);
}

void CClient::getAuthTicket(byte **ppOut, uint16_t *pusLength)
{
	*pusLength = 32;
	byte *pData = new byte[*pusLength];
	memset(pData, 0, *pusLength);

	strcpy((char*)pData, "AuthTicket");

	*ppOut = pData;
}

void CClient::freeAuthTicket(byte *pData, uint16_t usLength)
{
	mem_delete(pData);
}
