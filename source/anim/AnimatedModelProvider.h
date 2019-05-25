#ifndef __ANIMATEDMODELPROVIDER_H
#define __ANIMATEDMODELPROVIDER_H

#include <xcommon/resource/IXModelProvider.h>
#include "AnimatedModel.h"
#include "AnimatedModelShared.h"
#include <xcommon/IXCore.h>
#include <mtrl/IXMaterialSystem.h>

class CAnimatedModelProvider: public IXAnimatedModelProvider
{
public:
	CAnimatedModelProvider(IXCore *pCore);

	bool XMETHODCALLTYPE createModel(UINT uResourceCount, const IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel) override;

	void onSharedModelRelease(CAnimatedModelShared *pShared);
	void onModelRelease(CAnimatedModel *pModel);
	IXMaterialSystem *getMaterialSystem();

protected:
	AssotiativeArray<const IXResourceModelAnimated*, Array<CAnimatedModelShared*>> m_mModels;

	Array<CAnimatedModel*> m_apModels;

	IXCore *m_pCore;
};

#endif
