
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CLIENT_H
#define __CLIENT_H

#include "sxnetwork.h"
#include "NetChannel.h"

class CClient
{
public:
	CClient();
	~CClient();

	void connect(const char *szIp, unsigned short usPort);
	void disconnect();

	bool isConnected();

	void update();

	void getAuthTicket(byte **ppOut, uint16_t *pusLength);
	void freeAuthTicket(byte *pData, uint16_t usLength);

	enum CONN_STATE
	{
		CS_DISCONNECTED = 0,

		CS_BEGIN,
		CS_CHALLENGE_REQUESTED,
		CS_CHALLENGE_RECEIVED,
		CS_CHALLENGE_SENT,
		CS_CHALLENGE_READY,
	};

protected:
	int m_iListener;
	CNetChannel *m_pNetChannel;
	CNetPeer m_server;
	bool m_isConnected = false;
	CONN_STATE m_connState = CS_DISCONNECTED;
	uint32_t m_uChallenge = 0;
	CNetUser *m_pNetUser = NULL;
};

#endif
