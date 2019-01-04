
#include <stdio.h>
#include <memory.h>
#include <sys/types.h>

#if defined(_LINUX) || defined(_MAC)
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define closesocket close
#endif



//#define LOG(t, ...) printf("L: " t "\n", __VA_ARGS__)
#define LOG(t, ...) 

#include "MasterClient.h"
#include "NETbuff.h"

#include "CJSON.h"

#define CHK_NULL(x) /*if ((x)==NULL) exit (1)*/
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); }

#if defined(_WINDOWS)
#	ifdef errno
#		undef errno
#		define errno WSAGetLastError()
#	endif
#	define EINPROGRESS WSAEINPROGRESS
#	define EWOULDBLOCK WSAEWOULDBLOCK
#endif

namespace NET
{

	void fnM2C_KEEPALIVE(BYTE msg, INETbuff * buf, IMasterClient* mc)
	{
		NETbuff tmp;
		tmp.writeInt8(C2M_KEEPALIVE);
		mc->SendMessage(&tmp);
	}

	void MasterClient::fnM2C_MSG(BYTE msg, INETbuff * buf, IMasterClient* _mc)
	{
		MasterClient * mc = (MasterClient*)_mc;

		BYTE eid = buf->readInt8();
		StringW eprm = buf->readPString();

		Array<PFNMSCEVENT> * evts = &(mc->m_mvEventList[mc->m_vEventIds[eid]]);
		CJSON json(eprm.c_str());

		for(UINT i = 0, l = evts->size(); i < l; ++i)
		{
			evts[0][i](&json);
		}
	}

	void MasterClient::fnM2C_SUBID(BYTE msg, INETbuff * buf, IMasterClient* _mc)
	{
		MasterClient * mc = (MasterClient*)_mc;
		StringW evt = buf->readPString();
		BYTE sid = buf->readInt8();

		mc->m_vEventIds[sid] = evt;
	}

	void fnM2C_LOGIN(BYTE msg, INETbuff * buf, IMasterClient* _mc)
	{
		MasterClient * mc = (MasterClient*)_mc;
		BYTE res = buf->readInt8();
		mc->m_bLoggingin = FALSE;
		StringW token = buf->readPString();
		if(mc->m_pfnLogin)
		{
			mc->m_pfnLogin(res, token);
		}
		if(res == MSL_SUCCESS)
		{
			wprintf(L"AuthToken: %s\n", token.c_str());
		}
	}

	MasterClient::MasterClient():
		m_pSrv(NULL),
		m_state(MCS_DISCONNECTED),
		m_pfnDisconnect(NULL),
		m_pfnConnected(NULL),
		m_pBuf(new NETbuff()),
		m_inMsgState(INM_NONE),
		m_pSendBuf(new NETbuff()),
		m_pOutBuf(new NETbuff()),
		m_bOutSent(TRUE),
		m_pfnLogin(NULL),
		m_bLoggingin(FALSE)
	{
		memset(m_pMessages, 0, sizeof(m_pMessages));
		m_pMessages[M2C_KEEPALIVE] = fnM2C_KEEPALIVE;
		m_pMessages[M2C_MSG] = fnM2C_MSG;
		m_pMessages[M2C_SUBID] = fnM2C_SUBID;
		m_pMessages[M2C_LOGIN] = fnM2C_LOGIN;


#if defined(_WINDOWS)
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			printf("WSAStartup failed!\n");
		}
#endif

		SSLeay_add_ssl_algorithms();
		SSL_load_error_strings();
	}

	void MasterClient::DispatchMessage(INETbuff * buf)
	{
		BYTE msg = buf->readInt8();
		printf("M2C_%d\n", (int)msg);
		if(m_pMessages[msg])
		{
			m_pMessages[msg](msg, buf, this);
		}
	}

	void MasterClient::RegisterMessage(BYTE msg, PFNMSCMESSAGE fn)
	{
		if(msg < M2C_LAST)
		{
			return;
		}
		m_pMessages[msg] = fn;
	}
	void MasterClient::UnregisterMessage(BYTE msg)
	{
		if(msg < M2C_LAST)
		{
			return;
		}
		m_pMessages[msg] = NULL;
	}

	void MasterClient::OnDisconnect(PFNMSCINFO fn)
	{
		m_pfnDisconnect = fn;
	}
	void MasterClient::OnConnected(PFNMSCINFO fn)
	{
		m_pfnConnected = fn;
	}


	void MasterClient::Connect(const char * srv)
	{
		LOG("connect");
		int err;
		struct sockaddr_in sa;
		X509*    server_cert;
		char*    str;
		char     buf[4096];
		const SSL_METHOD *meth;

		if(m_pSrv && m_pSrv != srv)
		{
			delete[]m_pSrv;
			m_pSrv = new char[strlen(srv) + 1];
			strcpy(m_pSrv, srv);
		}
		

		//CHK_SSL(err);

		/* ----------------------------------------------- */
		/* Create a socket and connect to server using normal socket calls. */

		m_sock = socket(AF_INET, SOCK_STREAM, 0);

#if defined(_WINDOWS)
		u_long dw = 1;
		err = ioctlsocket(m_sock, FIONBIO, &dw);
#else
		int flags = fcntl(m_sock, F_GETFL, 0);
		//if(flags < 0) return false;
		flags |= O_NONBLOCK;
		fcntl(m_sock, F_SETFL, flags);
#endif

		CHK_ERR(m_sock, "socket");

		memset(&sa, '\0', sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr("138.201.120.30");   /* Server IP */
		sa.sin_port = htons(443);          /* Server Port number */

		err = connect(m_sock, (struct sockaddr*) &sa, sizeof(sa));
		LOG("connect returns %d, errno= %d", err, errno);
		if(err < 0)
		{
			if(errno == EINPROGRESS || errno == EWOULDBLOCK)
			{
				m_state = MCS_CONNECTING;
				LOG("S=MCS_CONNECTING");
			}
			else
			{
				CHK_ERR(err, "connect");
				m_state = MCS_DISCONNECTED;
				LOG("S=MCS_DISCONNECTED");
			}
		}
		if(err == 0)
		{
			m_state = MCS_CONNECTED;
			LOG("S=MCS_CONNECTED");
		}
		return;
#if 0
			Sleep(5000);

			int result_len = sizeof(err);
			if(getsockopt(m_sock, SOL_SOCKET, SO_ERROR, (char*)&err, &result_len) < 0)
			{
				perror("getsockopt");
				// error, fail somehow, close socket
				return;
			}

			if(err != 0)
			{
				printf("Err: %d\n", err);
				// connection failed; error code is in 'result'
				return;
			}

			/* ----------------------------------------------- */
			/* Now we have TCP conncetion. Start SSL negotiation. */

			m_pSsl = SSL_new(m_pSslCtx);                         CHK_NULL(ssl);
			SSL_set_fd(m_pSsl, m_sock);
			SSL_set_tlsext_host_name(m_pSsl, "ds-servers.com");
			err = SSL_connect(m_pSsl);                     CHK_SSL(err);
			ERR_print_errors_fp(stderr);
			Sleep(5000);
			Sleep(5000);
			Sleep(5000);
			ERR_print_errors_fp(stderr);
			/* Following two steps are optional and not required for
			data exchange to be successful. */

			/* Get the cipher - opt */
			const int nCode = SSL_get_error(m_pSsl, err);
			printf("SSL connection using %s\n", SSL_get_cipher(m_pSsl));

			/* Get server's certificate (note: beware of dynamic allocation) - opt */

			server_cert = SSL_get_peer_certificate(m_pSsl);       CHK_NULL(server_cert);
			printf("Server certificate:\n");

			str = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0);
			CHK_NULL(str);
			printf("\t subject: %s\n", str);
			OPENSSL_free(str);

			str = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0);
			CHK_NULL(str);
			printf("\t issuer: %s\n", str);
			OPENSSL_free(str);

			/* We could do all sorts of certificate verification stuff here before
			deallocating the certificate. */

			X509_free(server_cert);

			/* --------------------------------------------------- */
			/* DATA EXCHANGE - Send a message and receive a reply. */
			char * req = "GET /connectors/registry/ HTTP/1.1\r\n"
				"Host: ds-servers.com\r\n"
				"User-Agent: Mozilla/5.0 (compatible; SIP-Game client sip-game.su)\r\n"
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
				"Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
				"Accept-Encoding: gzip, deflate, br\r\n"
				"Sec-WebSocket-Version: 13\r\n"
				"Origin: https://ds-servers.com\r\n"
				"Sec-WebSocket-Key: XgLvSDOcKtt0uW98pJmFWw==\r\n"
				"Cookie: DS-SESSID=2idgmtv3r33643t8bqmamftcf3\r\n"
				"Connection: keep-alive, Upgrade\r\n"
				"Pragma: no-cache\r\n"
				"Cache-Control: no-cache\r\n"
				"Upgrade: websocket\r\n"
				"\r\n";
			err = SSL_write(m_pSsl, req, strlen(req));  CHK_SSL(err);

			Sleep(1000);
			err = SSL_read(m_pSsl, buf, sizeof(buf) - 1);                     CHK_SSL(err);
			buf[err] = '\0';
			printf("Got %d chars:'%s'\n", err, buf);


			NETbuff buff;
			buff.writeInt8(0x82);
			BYTE len = 1;
			buff.writeInt8(len & 0x7F);
			buff.writeInt8(C2M_KEEPALIVE);

			_SendMessage(&buff);
#endif
	}

	void MasterClient::Update()
	{
		int err;
		const char * req;
		char*    str;
		X509*    server_cert;
		char     buf[4097];

		switch(m_state)
		{
		case MCS_CONNECTING:
			{
				fd_set fd;
				FD_ZERO(&fd);
				FD_SET(m_sock, &fd);

				struct timeval tv;
				tv.tv_sec = 0;
				tv.tv_usec = 0;

				// check if the socket is connected

#if defined(_WINDOWS)
				if(select(1, NULL, &fd, NULL, &tv) == SOCKET_ERROR)
#else
				if(select(m_sock + 1, NULL, &fd, NULL, &tv) < 0)
#endif
				{
					perror("select");
					return;
				}
				if(!FD_ISSET(m_sock, &fd))
				{
					return;
				}

				int result_len = sizeof(err);
				if(getsockopt(m_sock, SOL_SOCKET, SO_ERROR, (char*)&err, &result_len) < 0)
				{
					perror("getsockopt");
					// error, fail somehow, close socket
					return;
				}
				LOG("SO_ERROR= %d", err);
				if(err != 0)
				{
					/* socket has a non zero error status */
					fprintf(stderr, "socket error: %s\n", strerror(err));
				}

				const SSL_METHOD *meth;
				meth = SSLv23_client_method();
				m_pSslCtx = SSL_CTX_new(meth);                        CHK_NULL(m_pSslCtx);

				m_pSsl = SSL_new(m_pSslCtx);                         CHK_NULL(ssl);
				SSL_set_fd(m_pSsl, m_sock);
				SSL_set_tlsext_host_name(m_pSsl, "ds-servers.com");
				m_state = MCS_CONNECTED;
				LOG("S=MCS_CONNECTED");
			}
			break;

		case MCS_CONNECTED:
			
			err = SSL_connect(m_pSsl);
			LOG("SSL_connect= %d", err);
			if(err < 0)
			{
				err = SSL_get_error(m_pSsl, err);
				if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE))
					Disconnect(MSDISCONNECT_ERROR);
				LOG("SSL_get_error= %d", err);
				//Sleep(100);
				//err = SSL_connect(m_pSsl);                     CHK_SSL(err);
				ERR_print_errors_fp(stderr);
			}
			else if(err != 0)
			{
				m_state = MCS_SECURING;
				LOG("S=MCS_SECURING");
			}
			break;

		case MCS_SECURING:
			printf("SSL connection using %s\n", SSL_get_cipher(m_pSsl));
			server_cert = SSL_get_peer_certificate(m_pSsl);       CHK_NULL(server_cert);
			printf("Server certificate:\n");

			str = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0);
			CHK_NULL(str);
			printf("\t subject: %s\n", str);
			OPENSSL_free(str);

			str = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0);
			CHK_NULL(str);
			printf("\t issuer: %s\n", str);
			OPENSSL_free(str);

			/* We could do all sorts of certificate verification stuff here before
			deallocating the certificate. */

			X509_free(server_cert);

			m_state = MCS_SECURED;
			LOG("S=MCS_SECURED");
			break;

		case MCS_SECURED:
			req = GetReqStr().c_str();
			err = SSL_write(m_pSsl, req, strlen(req));
			if(err < 0)
			{
				err = SSL_get_error(m_pSsl, err);
				if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE))
					Disconnect(MSDISCONNECT_ERROR);
				//Sleep(100);
				//err = SSL_connect(m_pSsl);                     CHK_SSL(err);
				ERR_print_errors_fp(stderr);
			}
			else if(err > 0)
			{

				m_state = MCS_HANDSHAKE;
				LOG("S=MCS_HANDSHAKE");
			}
			else
			{
				Disconnect(MSDISCONNECT_ERROR);
			}
			break;

		case MCS_HANDSHAKE:
			err = SSL_read(m_pSsl, buf, sizeof(buf) - 1);
			if(err < 0)
			{
				err = SSL_get_error(m_pSsl, err);
				if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE))
					Disconnect(MSDISCONNECT_ERROR);
				ERR_print_errors_fp(stderr);
			}
			else if(err > 0)
			{
			//	buf[err] = '\0';
			//	printf("Got %d chars:'%s'\n", err, buf);
				m_state = MCS_READY;
				LOG("S=MCS_READY");
			}
			else
			{
				Disconnect(MSDISCONNECT_ERROR);
			}
			break;
			
		case MCS_READY:
			err = SSL_read(m_pSsl, buf, sizeof(buf) - 1);
			if(err < 0)
			{
				err = SSL_get_error(m_pSsl, err);
				if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE))
					Disconnect(MSDISCONNECT_ERROR);
				ERR_print_errors_fp(stderr);
			}
			else if(err > 0)
			{
				BYTE * pBuf = (BYTE*)buf;
				while(err > 0)
				{
					if(m_inMsgState == INM_NONE || m_inMsgState == INM_NEXT)
					{
						m_wsOP = pBuf[0] & 0x0F;
						m_wsIsLastFrame = pBuf[0] & 0x80;
						m_inMsgState = INM_OPC;
						++pBuf;
						--err;
					}
					if(m_inMsgState == INM_OPC && err > 0)
					{
						m_wsUseMask = pBuf[0] & 0x80;
						m_wsSize = pBuf[0] & 0x7F;
						m_wsLenByteRead = 0;
						if(m_wsSize == 126)
						{
							m_inMsgState = INM_SIZE;
							m_wsLenByteRead = 2;
							m_wsSize = 0;
						}
						else if(m_wsSize == 127)
						{
							m_inMsgState = INM_SIZE;
							m_wsLenByteRead = 8;
							m_wsSize = 0;
						}
						else
						{
							m_inMsgState = m_wsUseMask ? INM_MASK : INM_DATA;
							m_wsLenByteRead = 4;
						}

						++pBuf;
						--err;
					}

					if(m_inMsgState == INM_SIZE)
					{
						for(; m_wsLenByteRead > 0 && err > 0; --m_wsLenByteRead)
						{
							m_wsSize <<= 8;
							m_wsSize |= pBuf[0];
							++pBuf;
							--err;
						}
						if(m_wsLenByteRead == 0)
						{
							m_inMsgState = m_wsUseMask ? INM_MASK : INM_DATA;
							m_wsLenByteRead = 4;
						}
					}

					if(m_inMsgState == INM_MASK && err > 0)
					{
						for(; m_wsLenByteRead > 0 && err > 0; --m_wsLenByteRead)
						{
							++pBuf;
							--err;
						}
						if(m_wsLenByteRead == 0)
						{
							m_inMsgState = INM_DATA;
						}
					}

					if(m_inMsgState == INM_DATA && err > 0)
					{
						UINT len = min((UINT)err, m_wsSize);
						m_pBuf->addBuf(pBuf, len);
						err -= len;
						m_wsSize -= len;
						pBuf += len;

						m_inMsgState = m_wsSize == 0 ? m_wsIsLastFrame ? INM_PARSE : INM_NEXT : INM_DATA;
					}

					if(m_inMsgState == INM_PARSE)
					{
						m_inMsgState = INM_NONE;
						switch(m_wsOP)
						{
						case WSOP_CONTINUE:
							printf("Cannot parse continuation frame, shouldn't get there\n");
							break;

						case WSOP_TEXT:
							wprintf(L"%s\n", m_pBuf->readPString().c_str());
							break;

						case WSOP_BLOB:
							DispatchMessage(m_pBuf);
							break;

						case WSOP_CLOSE:
							Disconnect(MSDISCONNECT_REMOTE);
							return;

						default:
							printf("WSOP_%d\n", m_wsOP);
							break;
						}
						m_pBuf->setBuf(NULL, 0);
					}
				}
			}
			else
			{
				Disconnect(MSDISCONNECT_ERROR);
				return;
			}
			_SendMessage();
			break;

		case MCS_DISCONNECTING:
			err = SSL_shutdown(m_pSsl);
			if(err < 0)
			{
				err = SSL_get_error(m_pSsl, err);
				if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE))
					Disconnect(MSDISCONNECT_FORCE);
				ERR_print_errors_fp(stderr);
			}
			else
			{
				m_state = MCS_FREEING;
				LOG("S=MCS_FREEING");
			}
			break;

		case MCS_FREEING:
			closesocket(m_sock);
			SSL_free(m_pSsl);
			SSL_CTX_free(m_pSslCtx);

			m_state = MCS_DISCONNECTED;
			LOG("S=MCS_DISCONNECTED");
			break;

		case MCS_DISCONNECTED:
			Connect(m_pSrv);
			ReSub();
			return;
		}
		/*
		//read from net
		//do dispatch ready packets
		*/
	}
	void MasterClient::Syncronize()
	{
		//copy from out queue to working queue
		if(m_bOutSent && m_pOutBuf->GetSize() != 0)
		{
			INETbuff * tmp = m_pSendBuf;
			m_pSendBuf = m_pOutBuf;
			m_pOutBuf = tmp;
			m_pOutBuf->setBuf(NULL, 0);
			m_bOutSent = FALSE;
		}
	}

	void MasterClient::SendMessage(INETbuff * buf)
	{
		//@TODO: check parallel access

		UINT len = buf->GetSize();

		m_pOutBuf->movE();
		m_pOutBuf->writeInt8(WSOP_BLOB | 0x80);


		if(len < 126)
		{
			m_pOutBuf->writeInt8(len);
		}
		else if(len <= 0xFFFF)
		{
			m_pOutBuf->writeInt8(126);
			m_pOutBuf->writeInt16(len);
		}
		else
		{
			m_pOutBuf->writeInt8(127);
			m_pOutBuf->writeInt64(len);
		}

		m_pOutBuf->addBuf((BYTE*)buf->GetPointer(), len);
	}

	void MasterClient::_SendMessage()
	{
		if(!m_bOutSent && m_pSendBuf->GetSize())
		{
			int err = SSL_write(m_pSsl, m_pSendBuf->GetPointer(), m_pSendBuf->GetSize());
			if(err < 0)
			{
				err = SSL_get_error(m_pSsl, err);
				if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE))
					Disconnect(MSDISCONNECT_ERROR);
				ERR_print_errors_fp(stderr);
			}
			else if(err > 0)
			{
				m_bOutSent = TRUE;
			}
			else
			{
				Disconnect(MSDISCONNECT_ERROR);
			}
		}
	}

	void MasterClient::Disconnect(UINT how)
	{
		m_bReconnect = FALSE;
		m_state = MCS_DISCONNECTING;
		LOG("S=MCS_DISCONNECTING");

		switch(how)
		{
		case MSDISCONNECT_CLEAN:
			LOG("Disconnect(MSDISCONNECT_CLEAN)");
			break;

		case MSDISCONNECT_ERROR:
			LOG("Disconnect(MSDISCONNECT_ERROR)");
			break;

		case MSDISCONNECT_REMOTE:
			LOG("Disconnect(MSDISCONNECT_REMOTE)");
			break;

		case MSDISCONNECT_FORCE:
			LOG("Disconnect(MSDISCONNECT_FORCE)");
			break;

		}

		if(how == MSDISCONNECT_FORCE)
		{
			m_state = MCS_FREEING;
			LOG("S=MCS_FREEING");
		}

		m_inMsgState = INM_NONE;

		if(m_pfnDisconnect)
		{
			m_pfnDisconnect(how);
		}
		//m_state = MCS_DISCONNECTED;
		//LOG("S=MCS_DISCONNECTED");
		if(how != MSDISCONNECT_CLEAN)
		{
			m_bReconnect = TRUE;
		}
	}

	void MasterClient::Login(const StringW & login, const StringW & pass)
	{
		if(m_bLoggingin)
		{
			return;
		}
		m_bLoggingin = TRUE;
		NETbuff buf;
		buf.writeInt8(C2M_LOGIN);
		buf.writePString(login);
		buf.writePString(pass);
		SendMessage(&buf);
	}

	const String & MasterClient::GetReqStr()
	{
		static String s;

		s = String("GET /connectors/registry/ HTTP/1.1\r\n"
			"Host: ds-servers.com\r\n"
			"User-Agent: Mozilla/5.0 (compatible; SIP-Game client sip-game.su)\r\n"
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
			"Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
			"Accept-Encoding: gzip, deflate, br\r\n"
			"Sec-WebSocket-Version: 13\r\n"
			"Origin: https://ds-servers.com\r\n"
			"Sec-WebSocket-Key: XgLvSDOcKtt0uW98pJmFWw==\r\n"
			"Cookie: AUTH_TOKEN=") + ((String)m_wsAuthToken) + "\r\n"
			"Connection: keep-alive, Upgrade\r\n"
			"Pragma: no-cache\r\n"
			"Cache-Control: no-cache\r\n"
			"Upgrade: websocket\r\n"
			"\r\n";

		return(s);
	}

	void MasterClient::SetAuthToken(const StringW & tok)
	{
		m_wsAuthToken = tok;
	}

	void MasterClient::SetLoginCallback(PFNMSCLOGIN cb)
	{
		m_pfnLogin = cb;
	}

	void MasterClient::AddEvent(const StringW & ev, PFNMSCEVENT cb)
	{
		if(!cb)
		{
			return;
		}
		BOOL exists = m_mvEventList.KeyExists(ev);
		m_mvEventList[ev].push_back(cb);

		if(!exists)
		{
			NETbuff buf;
			buf.writeInt8(C2M_SUB);
			buf.writePString(ev);
			SendMessage(&buf);
		}
	}

	void MasterClient::ReSub()
	{
		NETbuff buf;
		for(AssotiativeArray<StringW, Array<PFNMSCEVENT>>::Iterator i = m_mvEventList.begin(); i; i++)
		{
			buf.writeInt8(C2M_SUB);
			buf.writePString(*i.first);
		}
		SendMessage(&buf);
	}

	MasterClient::~MasterClient()
	{
		if(m_pSrv)
		{
			delete[]m_pSrv;
		}
		if(m_pBuf)
		{
			delete m_pBuf;
		}
		if(m_pSendBuf)
		{
			delete m_pSendBuf;
		}
		if(m_pOutBuf)
		{
			delete m_pOutBuf;
		}
	}
};