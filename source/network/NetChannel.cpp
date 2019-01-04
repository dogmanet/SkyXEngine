#include "NetChannel.h"
#include "unet.h"


CNetChannel::CNetChannel(int iSocket):
	m_iSocket(iSocket)
{
}

void CNetChannel::update()
{
	sendMessages();
	recvMessages();
	processMessages();
}

//void CNetChannel::sendMessage(CNetUser *pNetUser, INETbuff *pBuf, bool isReliable)
//{
//
//}

void CNetChannel::sendRaw(CNetPeer *pNetPeer, byte *pData, int iSize)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(pNetPeer->m_uPort);
	addr.sin_addr.s_addr = pNetPeer->m_uIP;
	sendto(m_iSocket, (const char*)pData, iSize, 0, (sockaddr*)&addr, sizeof(addr));

}

bool CNetChannel::readPacket(INETbuff *pBuf, CNetUser **ppFrom, CNetPeer *pNetPeer, bool *pIsRaw)
{
	sockaddr_in addr;
	static byte pInBuff[PACKET_MAX_SIZE];
	socklen_t iLen = sizeof(addr);
	int rb;
	//pBuf->getPointer(); pBuf->getAllocatedSize();
	while((rb = recvfrom(m_iSocket, (char*)pInBuff, sizeof(byte) * PACKET_MAX_SIZE, 0, (sockaddr*)&addr, &iLen)) >= 0)
	{
		printf(COLOR_LRED "Recieved %d bytes from %hhu.%hhu.%hhu.%hhu" COLOR_RESET "\n", rb, addr.sin_addr.S_un.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4);
		if(rb < 4)
		{
			continue;
		}

		uint32_t *pData = (uint32_t*)pInBuff;

		pNetPeer->m_uIP = addr.sin_addr.s_addr;
		pNetPeer->m_uPort = htons(addr.sin_port);

		// steamworks handle
		if(pData[0] == 0xFFFFFFFF)
		{
			// pass message to steamworks
			//printf("passed to steamworks\n");
			*pIsRaw = true;
			pBuf->setBuf(pInBuff, rb);
			if(ppFrom)
			{
				*ppFrom = NULL;
			}
			return(true);
		}
		if(pData[0] == 0x00000000) // unauthorized
		{
			// handle msg from unauthorized user.
			//printf("unauth\n");
			*pIsRaw = true;
			pBuf->setBuf(pInBuff, rb);
			if(ppFrom)
			{
				*ppFrom = NULL;
			}
			return(true);
		}
		if(rb < PACKET_HDR_SIZE)
		{
			continue;
		}

		//push msg to buffer
		*pIsRaw = false;


	}
	return(false);
}

void CNetChannel::recvMessages()
{
	
}

void CNetChannel::sendMessages()
{

}

void CNetChannel::processMessages()
{

}

ID CNetChannel::acceptClient(CNetPeer *pNetPeer, CNetUser **ppNetUser)
{

	// gen sport
	uint8_t u8SourcePort = rand() & 0xFF;
	uint8_t u8StartSrcPort = u8SourcePort;
	bool repeat = true;
	while(repeat)
	{
		repeat = false;
		for(int i = 0, l = m_apUsers.size(); i < l; ++i)
		{
			if(m_apUsers[i] && m_apUsers[i]->m_netPeer.m_uIP == pNetPeer->m_uIP && m_apUsers[i]->m_u8SourcePort == u8SourcePort)
			{
				++u8SourcePort;
				if(u8SourcePort == u8StartSrcPort)
				{
					LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to accept client from %hhu.%hhu.%hhu.%hhu. Max clients per IP reached!", 
						((byte*)&(pNetPeer->m_uIP))[0],
						((byte*)&(pNetPeer->m_uIP))[1],
						((byte*)&(pNetPeer->m_uIP))[2],
						((byte*)&(pNetPeer->m_uIP))[3]
						);
					return(-1);
				}
				repeat = true;
				break;
			}
		}
	}

	CNetUser *pNetUser = new CNetUser(pNetPeer);
	pNetUser->m_u8SourcePort = u8SourcePort;
	ID idUser = -1;
	for(int i = 0, l = m_apUsers.size(); i < l; ++i)
	{
		if(!m_apUsers[i])
		{
			idUser = i;
			m_apUsers[i] = pNetUser;
			break;
		}
	}

	if(idUser == -1)
	{
		idUser = m_apUsers.size();
		m_apUsers.push_back(pNetUser);
	}
	pNetUser->m_id = idUser;

	if(ppNetUser)
	{
		*ppNetUser = pNetUser;
	}

	byte resp[5] = {0, 0, 0, 0, u8SourcePort};
	sendRaw(pNetPeer, resp, 5);

	return(idUser);
}
