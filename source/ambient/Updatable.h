#ifndef __UPDATABLE_H
#define __UPDATABLE_H

#include <xcommon/IXUpdatable.h>
#include "SkyBox.h"

class CUpdatable: public IXUpdatable
{
public:
	CUpdatable(CSkyBox *pSkyBox);

	UINT startup() override;
	void shutdown() override;

	ID run(float fDelta) override;
	void sync() override;

protected:
	CSkyBox *m_pSkyBox;
};

#endif
