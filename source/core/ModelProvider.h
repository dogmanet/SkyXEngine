#ifndef __MODELPROVIDER_H
#define __MODELPROVIDER_H

#include <xcommon/resource/IXModelProvider.h>
#include "Core.h"

class CModelProvider: public IXModelProvider
{
public:
	CModelProvider(CCore *pCore);

	// static only
	bool XMETHODCALLTYPE createStaticModel(const IXResourceModel *pResource, IXStaticModel **ppModel) override;

	// can be dynamic or animated
	bool XMETHODCALLTYPE createDynamicModel(const IXResourceModel *pResource, IXDynamicModel **ppModel) override;

protected:
	CCore *m_pCore;
};

#endif
