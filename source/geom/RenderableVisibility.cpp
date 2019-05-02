#include "RenderableVisibility.h"
#include "sxgeom.h"
#include <gcore/sxgcore.h>

#include "models.h"

extern CModels *g_pModels;

CRenderableVisibility::CRenderableVisibility(ID idPlugin):
	m_idPlugin(idPlugin)
{
	m_idVisCalcObj = g_pModels->addVisCaclObj();
	assert(ID_VALID(m_idVisCalcObj));
}

CRenderableVisibility::~CRenderableVisibility()
{
	g_pModels->deleteVisCaclObj(m_idVisCalcObj);
}

ID CRenderableVisibility::getPluginId()
{
	return(m_idPlugin);
}

void CRenderableVisibility::setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller)
{
	assert(!"Not implemented!");
}

void CRenderableVisibility::updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference)
{
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == -1);
		pRef = (CRenderableVisibility*)pReference;
	}

	assert(!pRef && "Referencing is not supported!");

	float3 vCamPos;
	pCamera->getPosition(&vCamPos);
	g_pModels->comVisible(pCamera->getFrustum(), &vCamPos, m_idVisCalcObj);
}

void CRenderableVisibility::updateForFrustum(IFrustum *pFrustum, const IXRenderableVisibility *pReference)
{
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == -1);
		pRef = (CRenderableVisibility*)pReference;
	}

	assert(!pRef && "Referencing is not supported!");

	assert(!"Not implemented!");
}
