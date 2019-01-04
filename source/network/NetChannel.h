#ifndef NETCHANNEL_H__
#define NETCHANNEL_H__

#include "sxnetwork.h"
#include "NETbuff.h"
#include "unet.h"

struct CNetPeer
{
	uint32_t m_uIP;
	uint16_t m_uPort;
};

class CNetUser
{
	friend class CNetChannel;

public:
	CNetUser(CNetPeer *pNetPeer):
		m_netPeer(*pNetPeer)
	{
	}

protected:
	CNetPeer m_netPeer;
	ID m_id = -1;
	uint8_t m_u8SourcePort;

	UINT m_iSeq = 0; //!< Last sent sequence
	UINT m_iAck = 0; //!< Last received ack

	int m_iLoss = 0;
	int m_iLatency = 0;

	CNETbuff m_bufOut;
};


class CNetChannel
{
public:
	CNetChannel(int iSocket);

	void update();

	void sendRaw(CNetPeer *pNetPeer, byte *pData, int iSize);
	//void sendMessage(CNetUser *pNetUser, INETbuff *pBuf, bool isReliable = false);

	bool readPacket(INETbuff *pBuf, CNetUser **ppFrom, CNetPeer *pNetPeer, bool *pIsRaw);

	ID acceptClient(CNetPeer *pNetPeer, CNetUser **ppNetUser = NULL);

protected:
	int m_iSocket;

	Array<CNetUser*> m_apUsers;

	void recvMessages();
	void sendMessages();
	void processMessages();
};

#endif
