#include "INETservice.h"
#include "NET.h"

namespace NET
{
	INETservice::INETservice()
	{
		m_iID = NET::RegSvc(this);
	}

	INETservice::~INETservice()
	{
		NET::DelSvc(m_iID);
	}
};
