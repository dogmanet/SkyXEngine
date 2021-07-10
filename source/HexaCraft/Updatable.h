#ifndef __UPDATABLE_H
#define __UPDATABLE_H

#include <xcommon/IXUpdatable.h>
#include "ChunkManager.h"

class CUpdatable: public IXUnknownImplementation<IXUpdatable>
{
public:
	CUpdatable(CChunkManager *pChunkManager);

	UINT startup() override;
	void shutdown() override;

	ID run(float fDelta) override;
	void sync() override;

protected:
	CChunkManager *m_pChunkManager = NULL;
};

#endif
