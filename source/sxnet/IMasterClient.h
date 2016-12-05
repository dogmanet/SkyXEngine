#ifndef IMasterClient_H
#define IMasterClient_H

#include "INETbuff.h"
#include "INETservice.h"
#include "JSON.h"

#ifdef SendMessage
#undef SendMessage
#endif

namespace NET
{
	enum M2C
	{
		M2C_BAD = 0,
		M2C_SUBID = 1,
		M2C_MSG = 2,
		M2C_KEEPALIVE = 3,
		M2C_LOGIN = 4

		, M2C_LAST
	};

	enum C2M
	{
		C2M_BAD = 0,
		C2M_SUB = 1,
		C2M_UNSUB = 2,
		C2M_KEEPALIVE = 4,
		C2M_LOGIN = 5

		, C2M_LAST
	};

	enum MSDISCONNECT
	{
		MSDISCONNECT_CLEAN,
		MSDISCONNECT_ERROR
	};

	enum MSLOGINSTATE
	{
		MSL_BADPASS = 0,
		MSL_SUCCESS = 1,
		MSL_ERROR = 2,
		MSL_BADTOKEN = 3,
		MSL_ACCEPTOKEN = 4
	};

	class IMasterClient;
	typedef void(*PFNMSCMESSAGE)(BYTE, INETbuff*, IMasterClient*);
	typedef void(*PFNMSCINFO)(UINT);
	typedef void(*PFNMSCLOGIN)(UINT, const StringW &);

	typedef void(*PFNMSCEVENT)(const JSON *);



	class IMasterClient: public INETservice
	{
	public:

		virtual void Connect(const char * srv) = 0;

		virtual void SendMessage(INETbuff * buf) = 0;
		virtual void DispatchMessage(INETbuff * buf) = 0;

		virtual void RegisterMessage(BYTE msg, PFNMSCMESSAGE fn) = 0;
		virtual void UnregisterMessage(BYTE msg) = 0;

		virtual void OnDisconnect(PFNMSCINFO) = 0;
		virtual void OnConnected(PFNMSCINFO) = 0;

		virtual void Login(const StringW & login, const StringW & pass) = 0;
		virtual void SetLoginCallback(PFNMSCLOGIN cb) = 0;
		virtual void SetAuthToken(const StringW & tok) = 0;

		virtual void AddEvent(const StringW & ev, PFNMSCEVENT cb) = 0;
	};
};

#endif
