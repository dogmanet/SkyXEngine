
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл model_sim - симуляционной модели имитируещей все сущности поддерживающие материальный рендер
*/

/*! \defgroup manager_render_model_sim model_sim - симуляционная модель имитируещая все сущности поддерживающие материальный рендер
 \ingroup managed_render
@{*/

#ifndef __SIMULATION_MODEL_H
#define __SIMULATION_MODEL_H

#include <common/array.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <geom/sxgeom.h>
#include <mtrl/sxmtrl.h>
#include <anim/sxanim.h>
#include <render/gdata.h>

//! симуляционная модель всех возможных моделей материалов для редактора материалов
class CSimulationModel
{
public:
	CSimulationModel();
	~CSimulationModel();

	SX_ALIGNED_OP_MEM

	void add(const char *szPath);

	void render(DWORD timeDelta);

	MTLTYPE_MODEL m_type_model;
	int m_iCurrRenderModel;
	float3 m_vRotation;

	ID getIdMtl();
	void getPlane(D3DXPLANE *pPlane);
	void getCenter(float3_t *pCenter);

protected:

	void renderStatic(DWORD timeDelta);
	void renderGreen(DWORD timeDelta);
	void renderSkin(DWORD timeDelta);

	IDirect3DVertexDeclaration9 *m_pVertexDeclarationStatic;
	IDirect3DVertexDeclaration9 *m_pVertexDeclarationGreen;
	IDirect3DVertexDeclaration9 *m_pVertexDeclarationSkin;

	struct CModel
	{
		CModel(){ m_pModel = 0; }
		CModel(ISXDataStaticModel *pModel, float3_t *pCenter, float3_t *pMax, float3_t *pMin, D3DXPLANE *pPlane)
		{
			m_pModel = pModel; m_vCenter = *pCenter; m_vMax = *pMax; m_vMin = *pMin; m_oPlane = *pPlane;
		}

		ISXDataStaticModel *m_pModel;
		IDirect3DVertexBuffer9 *m_pAnim;
		float3_t m_vMin, m_vMax, m_vCenter;
		D3DXPLANE m_oPlane;
	};

	Array<CModel*> m_aModels;
	IDirect3DVertexBuffer9 *m_pTransVertBufGreen;
	CGreenDataVertex m_oGreen;
	ID m_idMtrl;
	float4x4 m_mWorld;
};

#endif

//!@} manager_render_model_sim
