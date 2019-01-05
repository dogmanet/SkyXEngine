#include "NetChannel.h"
#include "unet.h"

//##########################################################################

void CNetUser::sendMessage(byte *pData, int iLength, bool isReliable)
{
	if(isReliable)
	{
		m_bufOutRel.addBuf(pData, iLength);
	}
	else
	{
		m_bufOut.addBuf(pData, iLength);
	}
}
void CNetUser::sendMessage(CNETbuff *pNetBuff, bool isReliable)
{
	sendMessage((byte*)pNetBuff->getPointer(), pNetBuff->getSize(), isReliable);
}

//##########################################################################

CNetChannel::CNetChannel(int iSocket):
	m_iSocket(iSocket)
{
}

void CNetChannel::update()
{
	sendMessages();
}

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
		//printf(COLOR_LRED "Recieved %d bytes from %hhu.%hhu.%hhu.%hhu" COLOR_RESET "\n", rb, addr.sin_addr.S_un.S_un_b.s_b1,
		//	addr.sin_addr.S_un.S_un_b.s_b2,
		//	addr.sin_addr.S_un.S_un_b.s_b3,
		//	addr.sin_addr.S_un.S_un_b.s_b4);
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
		
		msg_header_s *pHeader = (msg_header_s*)pInBuff;

		CNetUser *pNetUser = findUser(pNetPeer, pHeader->i8SPort);
		if(!pNetUser)
		{
			continue;
		}
		if(ppFrom)
		{
			*ppFrom = pNetUser;
		}

		if(pHeader->isReliable)
		{
			// Handle reliable packet
			if(pHeader->usSeq == 0xFFFF) // this is confirmation
			{
				if(pNetUser->m_uSeqRel <= pHeader->usAck)
				{
					pNetUser->m_uSeqRel = pHeader->usAck;
					if(pNetUser->m_uSeqRel + 1 == pNetUser->m_uReliableCount)
					{
						pNetUser->m_isAllReliableSent = true;
					}
				}
			}
			else // this is data message
			{
				if(pNetUser->m_uReliableExpected == 0)
				{
					/* if packet received */
					const UINT c_uDataSize = PACKET_MAX_SIZE - PACKET_HDR_SIZE;
					if((pHeader->usSeq * c_uDataSize + rb - sizeof(msg_header_s)) >= pNetUser->m_bufInRel.getSize() &&
						!memcmp(pInBuff + sizeof(msg_header_s), (byte*)pNetUser->m_bufInRel.getPointer() + pHeader->usSeq * c_uDataSize, rb - sizeof(msg_header_s)))
					{
						msg_header_s hdr;
						memset(&hdr, 0, sizeof(hdr));
						hdr.usSeq = 0xFFFF;
						hdr.usAck = pHeader->usSeq;
						hdr.isReliable = true;
						hdr.i8SPort = pNetUser->m_u8SourcePort;
						sendRaw(pNetPeer, (byte*)&hdr, sizeof(hdr));
					}
					else if(pHeader->usSeq == 0) // Begin of reliable sequence
					{
						pNetUser->m_uReliableExpected = pHeader->usAck;
						pNetUser->m_bufInRel.setBuf(pInBuff + sizeof(msg_header_s), rb - sizeof(msg_header_s));
						pNetUser->m_uAckRel = pHeader->usSeq;

						msg_header_s hdr;
						memset(&hdr, 0, sizeof(hdr));
						hdr.usSeq = 0xFFFF;
						hdr.usAck = pNetUser->m_uAckRel;
						hdr.isReliable = true;
						hdr.i8SPort = pNetUser->m_u8SourcePort;
						sendRaw(pNetPeer, (byte*)&hdr, sizeof(hdr)); //? 
					}
				}
				else
				{
					if(pHeader->usSeq == pNetUser->m_uAckRel + 1)
					{
						pNetUser->m_bufInRel.addBuf(pInBuff + sizeof(msg_header_s), rb - sizeof(msg_header_s));
						pNetUser->m_uAckRel = pHeader->usSeq;
						msg_header_s hdr;
						memset(&hdr, 0, sizeof(hdr));
						hdr.usSeq = 0xFFFF;
						hdr.usAck = pNetUser->m_uAckRel;
						hdr.isReliable = true;
						hdr.i8SPort = pNetUser->m_u8SourcePort;
						sendRaw(pNetPeer, (byte*)&hdr, sizeof(hdr));
					}
				}
				if(pNetUser->m_uReliableExpected == pNetUser->m_uAckRel + 1)
				{
					pNetUser->m_uReliableExpected = 0;
					pBuf->setBuf((byte*)pNetUser->m_bufInRel.getPointer(), pNetUser->m_bufInRel.getSize());
					return(true);
				}
			}
			continue;
		}
		else
		{
			if(!isSequenceGreater(pHeader->usSeq, pNetUser->m_uAck))
			{
				continue;
			}

			pNetUser->m_pbLoss[pNetUser->m_u8LossIdx % NET_LOSS_COUNT_FRAMES] = getSequenceDelta(pHeader->usSeq, pNetUser->m_uAck) - 1;
			++pNetUser->m_u8LossIdx;
			if((pNetUser->m_u8LossIdx & 0xF) == 0)
			{
				int iLoss = 0;
				for(int i = 0; i < NET_LOSS_COUNT_FRAMES; ++i)
				{
					iLoss += pNetUser->m_pbLoss[i];
				}
				pNetUser->m_iLoss = iLoss / NET_LOSS_COUNT_FRAMES;
			}

			pNetUser->m_uAck = pHeader->usSeq;

			// Count latency
			pNetUser->m_iLatency = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - 
				pNetUser->m_pSentPackets[pHeader->usAck % NET_PACKET_BUFFER_SIZE].m_time
			).count();
			
			pBuf->setBuf(pInBuff + sizeof(msg_header_s), rb - sizeof(msg_header_s));
			return(true);
		}
	}
	return(false);
}

bool CNetChannel::isSequenceGreater(UINT uFirst, UINT uSecond)
{
	const UINT c_uRange = 0xFFFF;

	return(uFirst > uSecond || (((uFirst + c_uRange) - uSecond) < c_uRange / 4));
}

UINT CNetChannel::getSequenceDelta(UINT uFirst, UINT uSecond)
{
	const UINT c_uRange = 0xFFFF;

	if(uFirst > uSecond)
	{
		return(uFirst - uSecond);
	}
	return((uFirst + c_uRange) - uSecond);
}

void CNetChannel::sendMessages()
{
	CNETbuff outBuf;
	msg_header_s hdrMsg;
	memset(&hdrMsg, 0, sizeof(hdrMsg));
	for(int i = 0, l = m_apUsers.size(); i < l; ++i)
	{
		CNetUser *pNetUser = m_apUsers[i];
		if(pNetUser)
		{
			if(pNetUser->m_bufOut.getSize() < 16)
			{
				pNetUser->m_bufOut.addBuf((byte*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16 - pNetUser->m_bufOut.getSize());
			}
			++pNetUser->m_uSeq;
			hdrMsg.usSeq = pNetUser->m_uSeq;
			hdrMsg.usAck = pNetUser->m_uAck;
			hdrMsg.isReliable = 0;
			hdrMsg.i8SPort = pNetUser->m_u8SourcePort;
			outBuf.writeBytes((byte*)&hdrMsg, sizeof(hdrMsg));
			outBuf.writeBytes((byte*)pNetUser->m_bufOut.getPointer(), pNetUser->m_bufOut.getSize());
			sendRaw(&pNetUser->m_netPeer, (byte*)outBuf.getPointer(), outBuf.getSize());
			pNetUser->m_pSentPackets[pNetUser->m_uSeq % NET_PACKET_BUFFER_SIZE].m_time = std::chrono::high_resolution_clock::now();
			pNetUser->m_bufOut.reset();
			outBuf.reset();

			const UINT c_uDataSize = PACKET_MAX_SIZE - PACKET_HDR_SIZE;
			if(pNetUser->m_isAllReliableSent)
			{
				if(pNetUser->m_bufOutRel.getSize() > 0)
				{
					// split to packets
					pNetUser->m_uReliableCount = pNetUser->m_bufOutRel.getSize() / c_uDataSize + (pNetUser->m_bufOutRel.getSize() % c_uDataSize ? 1 : 0);
					hdrMsg.usAck = pNetUser->m_uReliableCount;
					hdrMsg.isReliable = 1;
					pNetUser->m_uSeqRel = 0;
					for(UINT j = 0; j < pNetUser->m_uReliableCount; ++j)
					{
						UINT uChunkSize = (j == pNetUser->m_uReliableCount - 1) ? c_uDataSize : (pNetUser->m_bufOutRel.getSize() % c_uDataSize);
						pNetUser->m_aabOutgoindRels[j][uChunkSize - 1] = 0;
						memcpy(&(pNetUser->m_aabOutgoindRels[j][0]), (byte*)pNetUser->m_bufOutRel.getPointer() + c_uDataSize * j, uChunkSize);

						hdrMsg.usSeq = j;

						outBuf.writeBytes((byte*)&hdrMsg, sizeof(hdrMsg));
						outBuf.writeBytes(&(pNetUser->m_aabOutgoindRels[j][0]), uChunkSize);
						sendRaw(&pNetUser->m_netPeer, (byte*)outBuf.getPointer(), outBuf.getSize());
						outBuf.reset();
					}
					


					pNetUser->m_isAllReliableSent = false;
					pNetUser->m_bufOutRel.reset();
				}
			}
			else
			{
				// send again
				hdrMsg.usAck = pNetUser->m_uReliableCount;
				hdrMsg.isReliable = 1;
				for(UINT j = pNetUser->m_uSeqRel; j < pNetUser->m_uReliableCount; ++j)
				{
					hdrMsg.usSeq = j;

					outBuf.writeBytes((byte*)&hdrMsg, sizeof(hdrMsg));
					outBuf.writeBytes(&(pNetUser->m_aabOutgoindRels[j][0]), pNetUser->m_aabOutgoindRels[j].size());
					sendRaw(&pNetUser->m_netPeer, (byte*)outBuf.getPointer(), outBuf.getSize());
					outBuf.reset();
				}
			}
		}
	}
}

ID CNetChannel::acceptClient(CNetPeer *pNetPeer, CNetUser **ppNetUser, int iForceSourcePort)
{

	// gen sport
	uint8_t u8SourcePort;
	if(iForceSourcePort >= 0)
	{
		u8SourcePort = iForceSourcePort;
	}
	else
	{
		u8SourcePort = rand() & 0xFF;
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

CNetUser *CNetChannel::findUser(CNetPeer *pNetPeer, uint8_t u8SourcePort)
{
	for(int i = 0, l = m_apUsers.size(); i < l; ++i)
	{
		CNetUser *pNetUser = m_apUsers[i];
		if(pNetUser && pNetUser->m_netPeer.m_uIP == pNetPeer->m_uIP && pNetUser->m_u8SourcePort == u8SourcePort)
		{
			return(pNetUser);
		}
	}
	return(NULL);
}
