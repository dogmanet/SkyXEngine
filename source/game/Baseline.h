
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "EntityManager.h"
#include <common/assotiativearray.h>

#ifndef __BASELINE_H
#define __BASELINE_H

class CBaseline
{
	friend class CEntityManager;

	struct ent_record_s
	{
		XGUID m_guid;
		String m_sClassname;
		AssotiativeArray<String, String> m_mProps;
	};

public:

	ID getId()
	{
		return(m_id);
	}

protected:
	ID m_id;
	Array<ent_record_s> m_aRecords;

	CBaseline(ID id):m_id(id)
	{
	}
	~CBaseline()
	{
	}
};

#endif
