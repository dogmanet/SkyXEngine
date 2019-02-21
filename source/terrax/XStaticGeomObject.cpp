#include "XStaticGeomObject.h"
#include <render/sxrender.h>
#include <mtrl/sxmtrl.h>
#include "terrax.h"

CXStatixGeomObject::CXStatixGeomObject(ID idModel)
{
	assert(ID_VALID(idModel));

	m_idModel = idModel;
	m_vPos = *SGeom_ModelGetPosition(idModel);
}

void CXStatixGeomObject::setPos(const float3_t &pos)
{
	SGeom_ModelSetPosition(m_idModel, &float3(pos));
	BaseClass::setPos(pos);
}

float3_t CXStatixGeomObject::getScale()
{
	return(*SGeom_ModelGetScale(m_idModel));
}
void CXStatixGeomObject::setScale(const float3_t &pos)
{
	SGeom_ModelSetScale(m_idModel, &float3(pos));
}

SMQuaternion CXStatixGeomObject::getOrient()
{
	const float3 *rot = SGeom_ModelGetRotation(m_idModel);
	
	return(SMQuaternion(rot->x, 'x') * SMQuaternion(rot->y, 'y') * SMQuaternion(rot->z, 'z'));
}
void CXStatixGeomObject::setOrient(const SMQuaternion &orient)
{
	SGeom_ModelSetRotation(m_idModel, &SMMatrixToEuler(orient.GetMatrix()));
}

void CXStatixGeomObject::getBound(float3 *pvMin, float3 *pvMax)
{
	SGeom_ModelGetMinMax(m_idModel, pvMin, pvMax);
}

void CXStatixGeomObject::renderSelection(bool is3D)
{
	IGXContext *pDevice = SGCore_GetDXDevice();
	IGXBlendState *pOldBlendState = pDevice->getBlendState();
	IGXRasterizerState *pOldRS = pDevice->getRasterizerState();

	pDevice->setTexture(SGCore_LoadTexGetTex(SRender_EditorGetWhiteTex()));
	pDevice->setBlendState(g_xRenderStates.pBlendColorFactor);

	if(is3D)
	{
		pDevice->setBlendFactor(GXCOLOR_ARGB(70, 255, 0, 0));
		SGeom_RenderSingly(0, m_idModel, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));
	}

	pDevice->setRasterizerState(g_xRenderStates.pRSWireframe);
	pDevice->setBlendFactor(GXCOLOR_ARGB(255, 255, 255, 0));
	SGeom_RenderSingly(0, m_idModel, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));

	pDevice->setBlendState(pOldBlendState);
	pDevice->setRasterizerState(pOldRS);
	mem_release(pOldBlendState);
	mem_release(pOldRS);
}

bool CXStatixGeomObject::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl)
{
	return(SGeom_TraceBeamId(m_idModel, &vStart, &vEnd, pvOut, pidMtrl));
}
