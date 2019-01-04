#include "Server.h"

CServer::CServer(const char *szIp, unsigned short usPort)
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

	//int optval = 1;
	//setsockopt(m_iListener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	/*if(fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK) == -1)
	{
		printf("Cannot set O_NONBLOCK, errno=%d\n", errno);
	}*/
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
	addr.sin_addr.s_addr = inet_addr(szIp);

	if(bind(m_iListener, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror(COLOR_LRED "bind"); fprintf(stderr, COLOR_RESET);
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to bind socket\n");
		return;
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "Ready for incoming connections\n");

	m_pNetChannel = new CNetChannel(m_iListener);
}

CServer::~CServer()
{
	//@TODO: kick all clients
	mem_delete(m_pNetChannel);
	closesocket(m_iListener);
}

void CServer::update()
{
	m_pNetChannel->update();
	CNETbuff buf;
	CNetUser *pNetUser = NULL;
	CNetPeer netPeer;
	bool isRaw = false;
	while(m_pNetChannel->readPacket(&buf, &pNetUser, &netPeer, &isRaw))
	{
		if(isRaw)
		{
			switch(buf.readUInt32())
			{
			case 0xFFFFFFFF:
				//@TODO: pass to steamworks
				break;
			case 0x00000000:
				switch(buf.readInt8())
				{
				case 'S':
					//\x00\x00\x00\x00SkyXEngineGetChallenge\x00
					{
						byte str[22] = {0};
						buf.readBytes(str, 22);
						if(!memcmp(str, "kyXEngineGetChallenge", 22))
						{
							uint32_t uResponse[2];
							uResponse[0] = 0;
							uResponse[1] = getChallenge(netPeer.m_uIP);
							m_pNetChannel->sendRaw(&netPeer, (byte*)uResponse, sizeof(uResponse));
						}
					}
					break;

				case 'C':
					//\x00\x00\x00\x00C\xXX\xXX\xXX\xXXSkyXEngineQueryInfo\x00
					//\x00\x00\x00\x00C\xXX\xXX\xXX\xXXSkyXEngineAuthorize\x00\xNN\xNN...
					// - XXXXXXXX - received challenge
					// - NNNN - auth ticket length
					// - ... - auth ticket blob
					if(isChallengeValid(netPeer.m_uIP, buf.readUInt32()))
					{
						byte str[20] = {0};
						buf.readBytes(str, 20);
						if(!memcmp(str, "SkyXEngineQueryInfo", 20))
						{
							sendQueryInfo(&netPeer);
						}
						else if(!memcmp(str, "SkyXEngineAuthorize", 20))
						{
							uint16_t usTicketLen = buf.readUInt16();
							if(usTicketLen > 0)
							{
								byte *pTicketBlob = (byte*)alloca(usTicketLen);
								buf.readBytes(pTicketBlob, usTicketLen);
								if(validateTicket(&netPeer, pTicketBlob, usTicketLen))
								{
									// accept client
									CNetUser *pNetUser;
									ID iUserId = m_pNetChannel->acceptClient(&netPeer, &pNetUser);
									if(ID_VALID(iUserId))
									{
										//@TODO: setup required data for pNetUser
										printf("Client connected\n");
									}
									else
									{
										//@TODO: send kick message
									}
								}
							}
						}
					}
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

void CServer::sendQueryInfo(CNetPeer *pPeer)
{
	m_pNetChannel->sendRaw(pPeer, (byte*)("\x00\x00\x00\x00NotImplemented\x00"), 19);
}

uint32_t CServer::getChallenge(uint32_t uIP)
{
	time_t tNow = time(0);
	uint32_t uChallenge = rand();

	int i = 0;
	for(int l = m_vChallenges.size(); i < l; ++i)
	{
		if(m_vChallenges[i].m_isUsed || m_vChallenges[i].m_time + NET_CHALLENGE_LIFETIME < tNow)
		{
			break;
		}
	}
	m_vChallenges[i] = {uIP, uChallenge, tNow, false};

	return(uChallenge);
}

bool CServer::isChallengeValid(uint32_t uIP, uint32_t uChallenge)
{
	time_t tNow = time(0);
	for(int i = 0, l = m_vChallenges.size(); i < l; ++i)
	{
		if(!m_vChallenges[i].m_isUsed && 
			m_vChallenges[i].m_uIP == uIP && 
			m_vChallenges[i].m_uChallenge == uChallenge && 
			m_vChallenges[i].m_time + NET_CHALLENGE_LIFETIME >= tNow)
		{
			m_vChallenges[i].m_isUsed = true;
			return(true);
		}
	}
	return(false);
}

bool CServer::validateTicket(CNetPeer *pPeer, byte *pData, int iLen)
{
	return(true);
}
