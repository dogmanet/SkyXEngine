#ifndef MasterClient_H
#define MasterClient_H

#include "IMasterClient.h"
#include "INETbuff.h"


#if defined(_WINDOWS)
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#ifdef DispatchMessage
#undef DispatchMessage
#endif

#ifdef SendMessage
#undef SendMessage
#endif

namespace NET
{
	class MasterClient: public IMasterClient
	{
	public:
		enum CONN_STATE
		{
			MCS_DISCONNECTED = 0,
			MCS_CONNECTING,
			MCS_CONNECTED,
			MCS_SECURING,
			MCS_SECURED,
			MCS_HANDSHAKE,
			MCS_READY
		};

		enum WS_OPCODE
		{
			WSOP_CONTINUE = 0x00,
			WSOP_TEXT = 0x01,
			WSOP_BLOB = 0x02,
			WSOP_CLOSE = 0x08,
			WSOP_PING = 0x09,
			WSOP_PONG = 0x0A
		};

		enum IN_MSG_STATE
		{
			INM_NONE,
			INM_OPC,
			INM_SIZE,
			INM_MASK,
			INM_DATA,
			INM_NEXT,
			INM_PARSE
		};

		MasterClient();
		~MasterClient();

		void Connect(const char * srv);
		void Disconnect(UINT how = MSDISCONNECT_CLEAN);

		void SendMessage(INETbuff * buf);
		
		void DispatchMessage(INETbuff * buf);

		void RegisterMessage(BYTE msg, PFNMSCMESSAGE fn);
		void UnregisterMessage(BYTE msg);

		void OnDisconnect(PFNMSCINFO);
		void OnConnected(PFNMSCINFO);

		void Update();
		void Syncronize();

		void Login(const StringW & login, const StringW & pass);
		void SetLoginCallback(PFNMSCLOGIN cb);
		void SetAuthToken(const StringW & tok);

		void AddEvent(const StringW & ev, PFNMSCEVENT cb);

		PFNMSCLOGIN m_pfnLogin;
		BOOL m_bLoggingin;

	protected:
		void _SendMessage();

		PFNMSCMESSAGE m_pMessages[256];

		PFNMSCINFO m_pfnDisconnect;
		PFNMSCINFO m_pfnConnected;

		SSL * m_pSsl;
		SSL_CTX * m_pSslCtx;
		SOCKET m_sock;

		char * m_pSrv;

		CONN_STATE m_state;

		INETbuff * m_pBuf;

		BYTE m_wsOP;
		BOOL m_wsIsLastFrame;
		UINT m_wsSize; //<@FIXME: could it be int64?
		BOOL m_wsUseMask;
		BYTE m_wsLenByteRead;

		IN_MSG_STATE m_inMsgState;

		INETbuff * m_pSendBuf;
		INETbuff * m_pOutBuf;

		BOOL m_bOutSent;

		StringW m_wsAuthToken;

	private:
		const String & GetReqStr();

		AssotiativeArray<StringW, Array<PFNMSCEVENT>> m_mvEventList;
		Array<StringW> m_vEventIds;

		void ReSub();

		static void fnM2C_MSG(BYTE msg, INETbuff * buf, IMasterClient* mc);
		static void fnM2C_SUBID(BYTE msg, INETbuff * buf, IMasterClient* mc);
	};
};

#endif
