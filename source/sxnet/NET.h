#ifndef NET_H
#define NET_H

#include <sxtypes.h>
#include "INET.h"
#include "INETservice.h"

#include <core/array.h>

namespace NET
{
	class NET: public INET
	{
	public:

		void Update();
		void Syncronize();

		static void _Update();
		static void _Syncronize();

		static UINT RegSvc(INETservice * pSvc);
		static void DelSvc(UINT id);

		INETbuff * CreateBuffer();
		void ReleaseBuffer(INETbuff * buf);

		IMasterClient * CreateMasterClient();
		void ReleaseMasterClient(IMasterClient *);

	protected:
		static Array<INETservice*> m_pServices;
	};
};

extern "C" __declspec(dllexport) NET::INET * InitInstance();


#endif
