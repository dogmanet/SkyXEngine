
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

//#define SM_D3D_CONVERSIONS
#include <common/Math.h>
#include <geom/sxgeom.h>
#include <green/sxgreen.h>
#include <mtrl/sxmtrl.h>
#include <anim/sxanim.h>
#include <render/gdata.h>

//! симуляционная модель всех возможных моделей материалов для редактора материалов
class CSimulationModel
{
public:
	CSimulationModel();
	~CSimulationModel();

	SX_ALIGNED_OP_MEM2();

	//! добавить модель (статическую)
	void add(ISXDataStaticModel *pModel);

	//! рендер (на основании установленного типа модели)
	void render(DWORD timeDelta);

	//! возвращает id материала
	ID getIdMtl();

	//! записывает плоскость (на основании первого треугольника модели)
	void getPlane(SMPLANE *pPlane);

	//! записывает центр
	void getCenter(float3_t *pCenter);

	//**********************************************************************

	//! тип модели
	MTLTYPE_MODEL m_typeModel;

	//! текущий номер модели для рендера
	int m_iCurrRenderModel;

	//! углы поворотов модели
	float3 m_vRotation;

	//**********************************************************************

protected:

	//! рисовать модель как статичекую
	void renderStatic(DWORD timeDelta);

	//! рисовать модель как растительность
	void renderGreen(DWORD timeDelta);

	//! рисовать модель как анимационную
	void renderSkin(DWORD timeDelta);

	//**********************************************************************

	//! структура модели
	struct CModel
	{
		CModel(){ m_pModel = 0; }
		CModel(ISXDataStaticModel *pModel, float3_t *pCenter, float3_t *pMax, float3_t *pMin, SMPLANE *pPlane)
		{
			m_pModel = pModel; m_vCenter = *pCenter; m_vMax = *pMax; m_vMin = *pMin; m_oPlane = *pPlane;
		}

		//! указатель на статическую модель
		ISXDataStaticModel *m_pModel;

		//! врешинный буфер для анимацонной модели
		IGXVertexBuffer *m_pAnim;
		
		//! минимум и максимум модели
		float3_t m_vMin, m_vMax;

		//! центр модели
		float3_t m_vCenter;

		//! плоскость (на основании первого треугольника модели)
		SMPLANE m_oPlane;
	};

	//**********************************************************************

	//! вершинная декларация для статической модели
	IGXVertexDeclaration *m_pVertexDeclarationStatic;

	//! вершинная декларация для растительности
	IGXVertexDeclaration *m_pVertexDeclarationGreen;

	//! вершинная декларация для анимационное модели
	IGXVertexDeclaration *m_pVertexDeclarationSkin;


	//! массив моделей
	Array<CModel*> m_aModels;

	//! вершинный буфер с трансформациями для растительности (всего одна вершина)
	IGXVertexBuffer *m_pTransVertBufGreen;

	//! данные о орастительности
	CGreenDataVertex m_oGreen;

	//! id материала
	ID m_idMtrl;

	//! мировая матрица
	float4x4 m_mWorld;
};

#endif

//!@} manager_render_model_sim
