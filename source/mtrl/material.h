
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <common/String.h>
#include <common/Array.h>
#include <common/file_utils.h>
#include "reflection.h"
#include <direct.h>

#define MTL_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < m_aUnitMtrls.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - material: unresolved index of access '%d'", GEN_MSG_LOCATION, id); return stdval; }\
else if (!m_aUnitMtrls[id])\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - material: material '%d' is not init", GEN_MSG_LOCATION, id); return stdval; }

#define MTL_REF_PRE_COND_ID(id,stdval) \
MTL_PRE_COND_ID(id, stdval)\
if (!(m_aUnitMtrls[id]->m_pReflect))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - material: material id = '%d', name = '%s' unsupported reflection", GEN_MSG_LOCATION, id, m_aUnitMtrls[id]->m_pMtrl->m_sName.c_str()); return stdval; }

//##########################################################################

class CMaterials
{
public:
	CMaterials();
	~CMaterials();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM

	void clear(bool isClearRefDel);
	void update(UINT timeDelta);
	void setMainTexture(ID idSlot, ID idTexure);
	void render(ID id, const float4x4 *pWorld=0, const float4 *pColor=0);
	void renderStd(MTLTYPE_MODEL type, const float4x4 *pWorld, ID idSlot, ID idMtl);
	void renderLight(const float4_t *pColor, const float4x4 *pWorld);
	int getCount();

	void setForceblyAlphaTest(bool useAlphaTest);
	bool getForceblyAlphaTest();

	void setIsIncrCountSurf(bool bf);
	bool getIsIncrCountSurf();
	void nullingCurrCountSurf();
	int getCurrCountSurf();
	void setCurrCountSurf(int iCount);

	ID mtlLoad(const char *szName, MTLTYPE_MODEL type = MTLTYPE_MODEL_STATIC);
	void mtlReLoad(ID id, const char *szName = 0);
	void mtlSave(ID id);

	ID getStdMtl(MTLTYPE_MODEL typeModel);
	ID exists(const char *szName);
	MTLTYPE_MODEL getTypeModel(ID id);
	ID getLightMtrl();
	void setTypeModel(ID id, MTLTYPE_MODEL typeModel);
	ID getID(const char *szName);

	//######################################################################

	bool mtlGetUsingAlphaTest(ID id);
	void mtlSetUsingAlphaTest(ID id, bool isUsing);
	MTLTYPE_REFLECT mtlTypeReflection(ID id);

	UINT mtlGetSort(ID id);

	void mtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type);
	MTLTYPE_PHYSIC mtlGetPhysicMaterial(ID id);

	void mtlSetTexture(ID id, const char *szTexture);
	void mtlGetTexture(ID id, char *szName);

	void mtlSetUseDestColor(ID id, bool useDestColor);
	bool mtlGetUseDestColor(ID id);

	ID mtlGetTextureID(ID id);
	void mtlSetVS(ID id, const char *szNameVS);
	void mtlGetVS(ID id, char *szNamePS);
	ID mtlGetVSID(ID id);
	void mtlSetPS(ID id, const char *szNamePS);
	void mtlGetPS(ID id, char *szNamePS);
	ID mtlGetPSID(ID id);


	void mtlSetLighting(ID id, bool isLighting);
	bool mtlGetLighting(ID id);

	void mtlSetIsTextureLighting(ID id, bool isTexLighting);
	bool mtlGetIsTextureLighting(ID id);

	void mtlSetTextureLighting(ID id, const char *szTex);
	void mtlGetTextureLighting(ID id, char *szTex);

	void mtlSetRoughness(ID id, float fRroughness);
	float mtlGetRoughness(ID id);

	void mtlSetF0(ID id, float fF0);
	float mtlGetF0(ID id);

	void mtlSetThickness(ID id, float fThickness);
	float mtlGetThickness(ID id);

	void mtlSetDurability(ID id, float fDurability);
	float mtlGetDurability(ID id);

	void mtlSetHitChance(ID id, float fHitChance);
	float mtlGetHitChance(ID id);

	void mtlSetDensity(ID id, float fDensity);
	float mtlGetDensity(ID id);

	void mtlSetTransparency(ID id, bool isTransparent);
	bool mtlGetTransparency(ID id);

	void mtlSetTypeReflection(ID id, MTLTYPE_REFLECT type);
	MTLTYPE_REFLECT mtlGetTypeReflection(ID id);


	void mtlSetMaskTex(ID id, const char *szTexture);
	void mtlGetMaskTex(ID id, char *szTexture);

	void mtlSetMRTex(ID id, int iChannel, const char *szTexture);
	void mtlGetMRTex(ID id, int iChannel, char *szTexture);

	void mtlSetDTex(ID id, int iChannel, const char *szTexture);
	void mtlGetDTex(ID id, int iChannel, char *szTexture);


	void mtlSetStdVS(ID id, MTL_SHADERSTD type, bool isSend);
	bool mtlGetStdVS(ID id, MTL_SHADERSTD type);
	void mtlSetStdPS(ID id, MTL_SHADERSTD type, bool isSend);
	bool mtlGetStdPS(ID id, MTL_SHADERSTD type);


	void mtlSetUserDataVS(ID id, int iComponent, float val);
	float mtlGetUserDataVS(ID id, int iComponent);
	void mtlSetUserDataVS_InPS(ID id, bool isSendPS);
	bool mtlGetUserDataVS_InPS(ID id);
	void mtlSetUserDataPS(ID id, int iComponent, float fValue);
	float mtlGetUserDataPS(ID id, int iComponent);
	void mtlSetUserDataPS_InPS(ID id, bool isSendVS);
	bool mtlGetUserDataPS_InPS(ID id);

	//**********************************************************************

	void mtlRefSetIDArr(ID id, ID idOwner, int iCube, ID idArr);
	ID mtlRefGetIDArr(ID id, ID idOwner, int iCube);

	void mtlRefPreRenderPlane(ID id, SMPLANE *pPlane);
	const IFrustum* mtlRefGetfrustum(ID id, int iCube);
	void mtlRefPostRenderPlane(ID id);
	IGXTexture2D* mtlRefPlaneGetTex(ID id);

	void mtlRefSetMinMax(ID id, const float3_t *pMin, const float3_t *pMax);
	bool mtlRefIsAllowedRender(ID id);

	void mtlRefCubeBeginRender(ID id, const float3_t *pCenter);
	void mtlRefCubePreRender(ID id, int iCube, const float4x4 *pWorld);
	void mtlRefCubePostRender(ID id, int iCube);
	void mtlRefCubeEndRender(ID id, const float3_t *pViewPos);
	bool mtlRefUpdateCountUpdate(ID id, const float3_t *pViewPos);
	void mtlRefNullingCountUpdate(ID id);
	IGXTextureCube* refCubeGetTex(ID id);

	//######################################################################

	int delRefCount();
	ID delRefGetIDArr(ID idKey, ID idOwner, int iCube);
	void delRefAllDel();

	//######################################################################

	struct CMaterial
	{
		CMaterial();
		~CMaterial();

		SX_ALIGNED_OP_MEM

		//! обнуление всех данных в материале
		void nulling();

		//! удален ли материал
		bool m_isDelete;

		//! основные графические свойства
		struct CMainGraphics
		{
			CMainGraphics();
			~CMainGraphics();

			void updateShaderKit(bool bComplete);

			//! основная текстура
			ID m_idMainTexture;

			//! вершинный шейдер
			ID m_idShaderVS;

			//! пиксельный шейдер
			ID m_idShaderPS;

			//! набор шейдеров
			ID m_idShaderKit = -1;

			//! неосвещаемый материал
			bool m_isUnlit;

			//! использовать ли альфа тест
			bool m_useAlphaTest;

			//! используется ли принимаемый цвет?
			bool m_useDestColor;

			//! тип модели для рендера
			MTLTYPE_MODEL m_typeModel;

			//! отправляемые данные в шейдеры
			struct СDataShader
			{
				СDataShader();
				~СDataShader();

				SX_ALIGNED_OP_MEM2();

				//! отправлять ли мировую матрицу
				bool m_isTransWorld;

				//! отправлять ли видову матрицу
				bool m_isTransView;

				//! отправлять ли проекционную матрицу
				bool m_isTransProjection;

				//! отправлять ли world * view
				bool m_isTransWorldView;

				//! отправлять ли world * view * projection
				bool m_isTransWorldViewProjection;

				//! отправлять ли позицию камеры/наблюдателя
				bool m_isTransPosCam;

				//! отправлять время кадра
				bool m_isTransTimeDelta;

				//! отправлять ли пользовательские данные
				bool m_isTransUserData;

				//! отправлять ли пользовательские данные в другой шейдер
				bool m_isTransUserDataInOtherShader;

				//! отправлять ли размер окна рендера
				bool m_isTransWinSize;

				//! пользовательские данные, значения [0, 1]
				float4 m_vUserData;
			};

			//! отправляемые данные в вершинный шейдер
			СDataShader m_oDataVS;

			//! отправляемые данные в пиксельный шейдер
			СDataShader m_oDataPS;
		};

		//! детализированные свойства, маска и 4 детальных и 4 микрорельефных карты
		struct CMaskDetailMicroRelief
		{
			CMaskDetailMicroRelief();
			~CMaskDetailMicroRelief();

			//! идентификатор текстуры маски, где к каждому каналу привязаны 4 детальных и микрорельефных текстуры
			ID m_idMask;

			//! массив идентификаторов детальных текстур, для каждого канала маски
			ID m_aDetail[4];

			//! массив идентификаторов микрорельефных текстур (normal map), для каждого канала маски
			ID m_aMicroRelief[4];
		};

		//! световые свойсвта, основные характеристики просчета освещения
		struct CLightParam
		{
			CLightParam();
			~CLightParam();

			//! текстура с параметрами материала (созданная пользователем)
			ID m_idTexParam;

			//! текстура с параметрами материала, размер 1х1, параметры взяты из текущей структуры
			ID m_idTexParamHand;

			//! назначена ли (true) текстура для параметров материала (или данные берем из параметров и кладем в рабочую текстуру)
			bool m_isTextureParam;

			//! шероховатость [0 - гладкий, 1 - полностью шершавый]
			float m_fRoughness;

			//! отражательная способность [0 - ничего не отражает, 1 - все отражает]
			float m_fF0;

			//! толщина/просвечиваемость [0 - просвечивается, 1 - не просвечивается]
			float m_fThickness;

			float m_fOldRoughness;
			float m_fOldF0;
			float m_fOldThickness;

			//! тип отражений
			MTLTYPE_REFLECT m_typeReflect;

			//! прозрачный ли материал
			bool m_isTransparent;
		};

		//! физические свойства
		struct CPhysics
		{
			CPhysics();
			~CPhysics();

			//! тип физического материала
			MTLTYPE_PHYSIC m_typePhysics;

			//! коэффициент пробиваемости [0, ], чем больше тем сложнее пробить
			float m_fDurability;

			//! шанс пробиваемости [0 - пуля летит насквозь, 1 - пуля ударяется]
			float m_fHitChance;

			//! плотность материала кг/м3
			float m_fDensity;
		};

		//! имя материала
		String m_sName;

		//! основные графические свойства
		CMainGraphics m_oMainGraphics;
		
		//! параметры детальности
		CMaskDetailMicroRelief m_oMicroDetail;

		//! параметры освещения
		CLightParam m_oLightParam;

		//! физические свойства
		CPhysics m_oPhysics;
	};

	//! юнит материала
	struct CUnitMaterial
	{
		CUnitMaterial();
		~CUnitMaterial();

		//! указатель материала
		CMaterial *m_pMtrl;

		//! объект отражений
		CReflection *m_pReflect;
	};

protected:

	//! использовать ли принудительный альфа тест
	bool m_useForceblyAlphaTest;

	bool loadMtl(const char *szName, CMaterial **ppMtrl);
	void createMtl(const char *szName, CMaterial **ppMtrl, MTLTYPE_MODEL type);
	ID createTexParamLighting(float fRoughness, float fF0, float fThickness);

	void addName(const char *szName, ID id);
	ID addUnitMaterial(CUnitMaterial *pUnitMtrl);
	ID addMaterial(CMaterial *pMtrl);

	//! структура описывающая папку и все текстуры в ней, у каждой свой id для доступа
	struct CPath
	{
		CPath(){}

		//! путь
		String m_sPath;

		struct CObject
		{
			CObject(){ m_id = -1; }
			CObject(ID id, const char *szName){ m_id = id; m_sName = szName; }

			//! идентификатор
			ID m_id;

			//! имя
			String m_sName;
		};

		//! массив CObject
		Array<CObject*> m_aObjects;
	};

	//! массив путей до текстур/материалов
	Array<CPath*> m_aMtrlPathes;

	//! массив юнитов материалов
	Array<CUnitMaterial*> m_aUnitMtrls;

	//! массив материалов
	Array<CMaterial*> m_aMtrls;

	//! массив отражений
	Array<CReflection*> m_aReflections;

	//! использовать ли подсчет поверхностей
	bool m_useCountSurface;

	//! текущий индентификатор поверхности
	ID m_idCurrIdSurface;

	int CurrFirstFree;

	//! текущее время кадра в млсек
	UINT m_uiCurrTimeDelta;

	//! общее время кадров в млсек
	UINT m_uiCountTimeDelta;

	ID m_idMtrlDefLight;
	ID m_idMtrlDefTree;
	ID m_idMtrlDefGrass;
	ID m_idMtrlDefStatic;
	ID m_idMtrlDefSkin;
	ID m_idBeginNonDef;

	float4x4 m_mWorld, m_mViewProj, m_mWorldTrans, m_mViewTrans, m_mProjTrans;
};

#endif