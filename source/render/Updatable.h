#ifndef __UPDATABLE_H
#define __UPDATABLE_H

#include <xcommon/IXUpdatable.h>
#include "Scene.h"

class CUpdatable: public IXUnknownImplementation<IXUpdatable>
{
public:
	CUpdatable(CScene *pScene);

	UINT startup() override;
	void shutdown() override;

	ID run(float fDelta) override;
	void sync() override;

protected:
	CScene *m_pScene;
};

#endif
