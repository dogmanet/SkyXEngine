#ifndef INET_H
#define INET_H

#include "INETbuff.h"
#include "IMasterClient.h"

namespace NET
{

	class INET
	{
	public:
		virtual ~INET()
		{
		}

		virtual void Update() = 0;
		virtual void Syncronize() = 0;

		virtual INETbuff * CreateBuffer() = 0;
		virtual void ReleaseBuffer(INETbuff * buf) = 0;

		virtual IMasterClient * CreateMasterClient() = 0;
		virtual void ReleaseMasterClient(IMasterClient *) = 0;
	};
	typedef INET*(*PFNINITINSTANCE)();
};

#endif
