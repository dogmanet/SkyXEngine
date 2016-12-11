#include "NET.h"
#include "NETbuff.h"
#include "MasterClient.h"

namespace NET
{
	Array<INETservice*> NET::m_pServices;

	void NET::Update()
	{
		_Update();
	}
	void NET::Syncronize()
	{
		_Syncronize();
	}

	void NET::_Update()
	{
		INETservice * svc;
		for(UINT i = 0, l = m_pServices.size(); i < l; ++i)
		{
			if((svc = m_pServices[i]))
			{
				svc->Update();
			}
		}
	}
	void NET::_Syncronize()
	{
		INETservice * svc;
		for(UINT i = 0, l = m_pServices.size(); i < l; ++i)
		{
			if((svc = m_pServices[i]))
			{
				svc->Syncronize();
			}
		}
	}

	UINT NET::RegSvc(INETservice * pSvc)
	{
		UINT ret = m_pServices.size();
		m_pServices.push_back(pSvc);
		return(ret);
	}
	void NET::DelSvc(UINT id)
	{
		m_pServices[id] = NULL;
	}

	INETbuff * NET::CreateBuffer()
	{
		return(new NETbuff());
	}
	void NET::ReleaseBuffer(INETbuff * buf)
	{
		if(buf)
		{
			delete buf;
		}
	}

	IMasterClient * NET::CreateMasterClient()
	{
		return(new MasterClient());
	}
	void NET::ReleaseMasterClient(IMasterClient * cl)
	{
		if(cl)
		{
			delete cl;
		}
	}

};


NET::INET * InitInstance()
{
	NET::NET * pGui = new NET::NET();

	return(pGui);
}