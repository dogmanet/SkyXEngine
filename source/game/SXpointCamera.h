#ifndef _SXpointCamera_H_
#define _SXpointCamera_H_

#include <gcore/sxgcore.h>

#include "SXpointEntity.h"

class SXpointCamera: public SXpointEntity
{
	DECLARE_CLASS(SXpointCamera, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	SXpointCamera(EntityManager * pMgr);
	~SXpointCamera();

	ISXCamera * GetCamera();

protected:
	ISXCamera * m_pSXC;

	void OnSync();
};

#endif
