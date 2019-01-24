
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "material.h"

CMaterials::CMaterials()
{
	CurrFirstFree = -1;
	m_uiCountTimeDelta = 0;
	m_uiCurrTimeDelta = 0;
	m_useForceblyAlphaTest = false;
	if(mtrl_data::pDXDevice)
	{
		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "mtrlgeom_base.vs", "mtrlgeom_base.vs", SHADER_CHECKDOUBLE_PATH);
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "mtrlgeom_base.ps", "mtrlgeom_base.ps", SHADER_CHECKDOUBLE_PATH);

		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "mtrlgreen_tree_base.vs", "mtrlgreen_tree_base.vs", SHADER_CHECKDOUBLE_PATH);
		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "mtrlgreen_grass_base.vs", "mtrlgreen_grass_base.vs", SHADER_CHECKDOUBLE_PATH);
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "mtrlgreen_base.ps", "mtrlgreen_base.ps", SHADER_CHECKDOUBLE_PATH);

		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "mtrlgeom_light.ps", "mtrlgeom_light.ps", SHADER_CHECKDOUBLE_PATH);

		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "mtrlskin_base.vs", "mtrlskin_base.vs", SHADER_CHECKDOUBLE_PATH);
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "mtrlskin_base.ps", "mtrlskin_base.ps", SHADER_CHECKDOUBLE_PATH);
	}
	m_useCountSurface = false;
	m_idCurrIdSurface = 0;

	CUnitMaterial* tmpumtl = new CUnitMaterial();
	CMaterial* tmpMtlDefaultLight = new CMaterial();
	addMaterial(tmpMtlDefaultLight);
	tmpumtl->m_pMtrl = tmpMtlDefaultLight;
	tmpMtlDefaultLight->m_sName = String(MTL_VIRTUAL_DIR_STD_MTL) + "_light";
	if(mtrl_data::pDXDevice)
	{
		tmpMtlDefaultLight->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgeom_base.vs");
		tmpMtlDefaultLight->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgeom_light.ps");
	}

	tmpMtlDefaultLight->m_oMainGraphics.m_idMainTexture = -1;
	tmpMtlDefaultLight->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = true;
	tmpMtlDefaultLight->m_oMainGraphics.m_oDataVS.m_isTransWorld = true;

	tmpMtlDefaultLight->m_oMainGraphics.m_isUnlit = true;
	tmpMtlDefaultLight->m_oLightParam.m_idTexParam = -1;
	tmpMtlDefaultLight->m_oLightParam.m_isTextureParam = false;
	if(mtrl_data::pDXDevice)
	{
		tmpMtlDefaultLight->m_oLightParam.m_idTexParamHand = createTexParamLighting(MTL_LIGHTING_DEFAULT_ROUGHNESS, MTL_LIGHTING_DEFAULT_F0, MTL_LIGHTING_DEFAULT_THICKNESS);
	}

	m_idMtrlDefLight = addUnitMaterial(tmpumtl);
	addName(tmpMtlDefaultLight->m_sName.c_str(), m_idMtrlDefLight);



	tmpumtl = new CUnitMaterial();
	tmpMtlDefaultLight = new CMaterial();
	addMaterial(tmpMtlDefaultLight);
	tmpumtl->m_pMtrl = tmpMtlDefaultLight;
	tmpMtlDefaultLight->m_sName = String(MTL_VIRTUAL_DIR_STD_MTL) + "_base";
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgeom_base.vs");
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgeom_base.ps");
	}
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = true;

	tmpumtl->m_pMtrl->m_oMainGraphics.m_idMainTexture = -1;
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = true;
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = true;

	tmpumtl->m_pMtrl->m_oLightParam.m_idTexParam = -1;
	tmpumtl->m_pMtrl->m_oLightParam.m_isTextureParam = false;
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oLightParam.m_idTexParamHand = createTexParamLighting(MTL_LIGHTING_DEFAULT_ROUGHNESS, MTL_LIGHTING_DEFAULT_F0, MTL_LIGHTING_DEFAULT_THICKNESS);
	}

	m_idMtrlDefStatic = addUnitMaterial(tmpumtl);
	addName(tmpumtl->m_pMtrl->m_sName.c_str(), m_idMtrlDefStatic);


	tmpumtl = new CUnitMaterial();
	tmpMtlDefaultLight = new CMaterial();
	addMaterial(tmpMtlDefaultLight);
	tmpumtl->m_pMtrl = tmpMtlDefaultLight;
	tmpMtlDefaultLight->m_sName = String(MTL_VIRTUAL_DIR_STD_MTL) + "_tree";
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgreen_tree_base.vs");
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgreen_base.ps");
	}

	tmpumtl->m_pMtrl->m_oMainGraphics.m_idMainTexture = -1;
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = true;

	tmpumtl->m_pMtrl->m_oLightParam.m_idTexParam = -1;
	tmpumtl->m_pMtrl->m_oLightParam.m_isTextureParam = false;
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oLightParam.m_idTexParamHand = createTexParamLighting(MTL_LIGHTING_DEFAULT_ROUGHNESS, MTL_LIGHTING_DEFAULT_F0, MTL_LIGHTING_DEFAULT_THICKNESS);
	}

	m_idMtrlDefTree = addUnitMaterial(tmpumtl);
	addName(tmpumtl->m_pMtrl->m_sName.c_str(), m_idMtrlDefTree);


	tmpumtl = new CUnitMaterial();
	tmpMtlDefaultLight = new CMaterial();
	addMaterial(tmpMtlDefaultLight);
	tmpumtl->m_pMtrl = tmpMtlDefaultLight;
	tmpMtlDefaultLight->m_sName = String(MTL_VIRTUAL_DIR_STD_MTL) + "_grass";
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgreen_grass_base.vs");
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgreen_base.ps");
	}

	tmpumtl->m_pMtrl->m_oMainGraphics.m_idMainTexture = -1;
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = true;

	tmpumtl->m_pMtrl->m_oLightParam.m_idTexParam = -1;
	tmpumtl->m_pMtrl->m_oLightParam.m_isTextureParam = false;
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oLightParam.m_idTexParamHand = createTexParamLighting(MTL_LIGHTING_DEFAULT_ROUGHNESS, MTL_LIGHTING_DEFAULT_F0, MTL_LIGHTING_DEFAULT_THICKNESS);
	}

	m_idMtrlDefGrass = addUnitMaterial(tmpumtl);
	addName(tmpumtl->m_pMtrl->m_sName.c_str(), m_idMtrlDefGrass);



	tmpumtl = new CUnitMaterial();
	tmpMtlDefaultLight = new CMaterial();
	addMaterial(tmpMtlDefaultLight);
	tmpumtl->m_pMtrl = tmpMtlDefaultLight;
	tmpMtlDefaultLight->m_sName = String(MTL_VIRTUAL_DIR_STD_MTL) + "_skin";
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlskin_base.vs");
		tmpumtl->m_pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlskin_base.ps");
	}

	tmpumtl->m_pMtrl->m_oMainGraphics.m_idMainTexture = -1;
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = true;
	tmpumtl->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = true;

	tmpumtl->m_pMtrl->m_oLightParam.m_idTexParam = -1;
	tmpumtl->m_pMtrl->m_oLightParam.m_isTextureParam = false;
	if(mtrl_data::pDXDevice)
	{
		tmpumtl->m_pMtrl->m_oLightParam.m_idTexParamHand = createTexParamLighting(MTL_LIGHTING_DEFAULT_ROUGHNESS, MTL_LIGHTING_DEFAULT_F0, MTL_LIGHTING_DEFAULT_THICKNESS);
	}

	m_idMtrlDefSkin = addUnitMaterial(tmpumtl);
	addName(tmpumtl->m_pMtrl->m_sName.c_str(), m_idMtrlDefSkin);

	m_idBeginNonDef = m_aUnitMtrls.size();
}

CMaterials::~CMaterials()
{
	for (int i = 0; i < m_aMtrlPathes.size(); ++i)
	{
		for (int k = 0; k < m_aMtrlPathes[i]->m_aObjects.size(); ++k)
		{
			mem_delete(m_aMtrlPathes[i]->m_aObjects[k]);
		}

		mem_delete(m_aMtrlPathes[i]);
	}

	for (int i = 0; i < m_aUnitMtrls.size(); ++i)
	{
		mem_delete(m_aUnitMtrls[i]);
	}

	for (int i = 0; i < m_aMtrls.size(); ++i)
	{
		mem_delete(m_aMtrls[i]);
	}
}

//**************************************************************************

CMaterials::CMaterial::CMaterial()
{
	nulling();
}

void CMaterials::CMaterial::nulling()
{
	m_sName = "";
	
	m_isDelete = false;

	m_oMainGraphics.m_idMainTexture = -1;
	m_oMainGraphics.m_idShaderVS = -1;
	m_oMainGraphics.m_idShaderPS = -1;
	m_oMainGraphics.m_useDestColor = false;
	
	m_oMicroDetail = CMaskDetailMicroRelief();
	m_oLightParam = CLightParam();

	m_oMainGraphics = CMainGraphics();

	m_oPhysics = CPhysics();
}

CMaterials::CMaterial::~CMaterial()
{
	if (m_oMainGraphics.m_idMainTexture >= 0)
		SGCore_LoadTexDelete(m_oMainGraphics.m_idMainTexture);

	if (m_oLightParam.m_idTexParam >= 0)
		SGCore_LoadTexDelete(m_oLightParam.m_idTexParam);

	if (m_oLightParam.m_idTexParamHand >= 0)
		SGCore_LoadTexDelete(m_oLightParam.m_idTexParamHand);

	if (m_oMicroDetail.m_idMask >= 0)
		SGCore_LoadTexDelete(m_oMicroDetail.m_idMask);

	for (int i = 0; i < 4; ++i)
	{
		if (m_oMicroDetail.m_aDetail[i] >= 0)
			SGCore_LoadTexDelete(m_oMicroDetail.m_aDetail[i]);

		if (m_oMicroDetail.m_aMicroRelief[i] >= 0)
			SGCore_LoadTexDelete(m_oMicroDetail.m_aMicroRelief[i]);
	}
}

//**************************************************************************

CMaterials::CMaterial::CMainGraphics::CMainGraphics()
{
	m_idMainTexture = -1;
	m_idShaderVS = -1;
	m_idShaderPS = -1;
	m_isUnlit = false;
	m_useAlphaTest = false;
	m_useDestColor = false;
	m_typeModel = MTLTYPE_MODEL::MTLTYPE_MODEL_DEFAULT;
	m_oDataVS = CMainGraphics::СDataShader();
	m_oDataPS = CMainGraphics::СDataShader();
	m_oDataVS.m_isTransUserDataInOtherShader = false;
	m_oDataPS.m_isTransUserDataInOtherShader = false;
};

CMaterials::CMaterial::CMainGraphics::~CMainGraphics()
{

};

//**************************************************************************

CMaterials::CMaterial::CMainGraphics::СDataShader::СDataShader()
{
	m_isTransWorld = false;
	m_isTransView = false;
	m_isTransProjection = false;
	m_isTransWorldView = false;
	m_isTransWorldViewProjection = false;
	m_isTransPosCam = false;
	m_isTransTimeDelta = false;
	m_isTransUserData = false;
	m_isTransWinSize = false;
	m_vUserData = float4(0, 0, 0, 0);
};

CMaterials::CMaterial::CMainGraphics::СDataShader::~СDataShader()
{

};

//**************************************************************************

CMaterials::CMaterial::CLightParam::CLightParam()
{
	m_idTexParam = -1;
	m_idTexParamHand = -1;

	m_fRoughness = 0.f;
	m_fF0 = 0.f;
	m_fThickness = 1.f;
	m_isTransparent = false;

	m_typeReflect = MTLTYPE_REFLECT_NONE;
	m_isTextureParam = true;
}

CMaterials::CMaterial::CLightParam::~CLightParam()
{

}

//**************************************************************************

CMaterials::CMaterial::CMaskDetailMicroRelief::CMaskDetailMicroRelief()
{
	m_idMask = -1;
	m_aDetail[0] = -1;
	m_aDetail[1] = -1;
	m_aDetail[2] = -1;
	m_aDetail[3] = -1;

	m_aMicroRelief[0] = -1;
	m_aMicroRelief[1] = -1;
	m_aMicroRelief[2] = -1;
	m_aMicroRelief[3] = -1;
}

CMaterials::CMaterial::CMaskDetailMicroRelief::~CMaskDetailMicroRelief()
{

}

//**************************************************************************

CMaterials::CMaterial::CPhysics::CPhysics()
{
	m_typePhysics = MTLTYPE_PHYSIC_CONCRETE;

	m_fDurability = 10.f;
	m_fHitChance = 1.f;
	m_fDensity = 1000;
}

CMaterials::CMaterial::CPhysics::~CPhysics()
{

}

//**************************************************************************

CMaterials::CUnitMaterial::CUnitMaterial()
{
	m_pMtrl = 0;
	m_pReflect = 0;
}

CMaterials::CUnitMaterial::~CUnitMaterial()
{
	m_pMtrl->m_isDelete = true;
	m_pReflect = 0;
	//mem_delete(Reflect);
}

//##########################################################################

ID CMaterials::exists(const char* name)
{
	char tmp_path[MTL_MAX_SIZE_DIR];//папка
	char tmp_name[MTL_MAX_SIZE_NAME];//само имя текстыр с расширением
	int id = -1;
	bool IsTruePath = false;
	//обрезаем имя текстуры и папку
	for (int i = 0; i<strlen(name); i++)
	{
		if (name[i] == '_')
		{
			memcpy(tmp_path, name, sizeof(char)*i);
			tmp_path[i] = 0;
			sprintf(tmp_name, "%s", name + i + 1);
			IsTruePath = true;
			break;
		}
	}

	if (!IsTruePath)
	{
		return -2;
		//LibReport(REPORT_MSG_LEVEL_ERROR, "%s - wrong texture name [%s]!!!", gen_msg_location, name);
	}

	int tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем
	for (int i = 0; i<m_aMtrlPathes.size(); ++i)
	{
		//TLPath* tmptlpath = m_aMtrlPathes[i];
		if (m_aMtrlPathes[i] && strcmp(m_aMtrlPathes[i]->m_sPath.c_str(), tmp_path) == 0)
		{
			tmpkey = i;
			break;
		}
	}

	//если мы не нашли совпадений значит путь уникален ...
	if (tmpkey == -1)
	{
		return -1;
	}

	//проверяем записано ли уже имя текстуры
	for (int i = 0; i<m_aMtrlPathes[tmpkey]->m_aObjects.size(); i++)
	{
		if (strcmp(m_aMtrlPathes[tmpkey]->m_aObjects[i]->m_sName.c_str(), tmp_name) == 0)
		{
			id = m_aMtrlPathes[tmpkey]->m_aObjects[i]->m_id;
			break;
		}
	}

	return id;
}

ID CMaterials::addUnitMaterial(CUnitMaterial* umtl)
{
	ID tmpid = -1;
	if (CurrFirstFree + 1 == m_aUnitMtrls.size())
	{
		m_aUnitMtrls.push_back(umtl);
		CurrFirstFree = tmpid = m_aUnitMtrls.size() - 1;
	}
	else
	{
		if (CurrFirstFree < 0 || m_aUnitMtrls[CurrFirstFree] != 0)
		{
			if (CurrFirstFree < 0)
				CurrFirstFree = 0;

			for (int i = CurrFirstFree; i < m_aUnitMtrls.size(); ++i)
			{
				if (m_aUnitMtrls[i] == 0)
				{
					CurrFirstFree = i;
					break;
				}
			}
		}

		m_aUnitMtrls[CurrFirstFree] = umtl;
		tmpid = CurrFirstFree;
		++CurrFirstFree;
	}

	return tmpid;
}

ID CMaterials::addMaterial(CMaterial* mtl)
{
	m_aMtrls.push_back(mtl);
	return m_aMtrls.size() - 1;
}

void CMaterials::addName(const char *szName, ID id)
{
	String sDir = StrSubstrSpre(szName, "_");
	String sName = StrSubstrSpost(szName, "_");

	ID idTex = -1;
	for (int i = 0; i<m_aMtrlPathes.size(); ++i)
	{
		if (m_aMtrlPathes[i] && strcmp(m_aMtrlPathes[i]->m_sPath.c_str(), sDir.c_str()) == 0)
		{
			idTex = i;
			break;
		}
	}

	//если мы не нашли совпадений значит путь уникален ...
	if (idTex == -1)
	{
		idTex = m_aMtrlPathes.size();
		CPath* tmpntlp = new CPath();
		m_aMtrlPathes.push_back(tmpntlp);
		m_aMtrlPathes[idTex]->m_sPath = sDir;
	}

	m_aMtrlPathes[idTex]->m_aObjects.push_back(new CPath::CObject(id, sName.c_str()));
}

bool CMaterials::loadMtl(const char *szName, CMaterial **ppMtrl)
{
	char* ArrRGBA[4] = { "r", "g", "b", "a" };

	String sVS, sPS, sMask, sNormal, sDetail, sTexParamLight;

	String sDir = StrSubstrSpre(szName, "_");
	String sName = StrSubstrSpre(szName, ".");

	CMaterial *pMtrl = *ppMtrl;

	char szPath[1024];
	sprintf(szPath, "%s%s/%s.mtl", Core_RStringGet(G_RI_STRING_PATH_GS_MTRLS), sDir.c_str(), sName.c_str());

	if (FileExistsFile(szPath))
	{
		ISXConfig *pConfig = Core_OpConfig(szPath);

		//если в конфиге указана текстура то берем ее
		if (pConfig->keyExists(sName.c_str(), "texture"))
			pMtrl->m_oMainGraphics.m_idMainTexture = SGCore_LoadTexAddName(pConfig->getKey(sName.c_str(), "texture"), LOAD_TEXTURE_TYPE_LOAD);
		else //если нет то тогда берем имя материала, может быть он имя текстуры, иначе будет -1
			pMtrl->m_oMainGraphics.m_idMainTexture = SGCore_LoadTexAddName(szName, LOAD_TEXTURE_TYPE_LOAD);

		pMtrl->m_sName = sName.c_str();

		if (pConfig->keyExists(sName.c_str(), "use_dest_color"))
			pMtrl->m_oMainGraphics.m_useDestColor = String(pConfig->getKey(sName.c_str(), "use_dest_color")).toBool();
		else 
			pMtrl->m_oMainGraphics.m_useDestColor = false;

		if (pConfig->keyExists(sName.c_str(), "vs"))
			sVS = pConfig->getKey(sName.c_str(), "vs");

		if (pConfig->keyExists(sName.c_str(), "ps"))
			sPS = pConfig->getKey(sName.c_str(), "ps");


		if (pConfig->keyExists(sName.c_str(), "lighted"))
			pMtrl->m_oMainGraphics.m_isUnlit = String(pConfig->getKey(sName.c_str(), "lighted")).toBool();
		else
			pMtrl->m_oMainGraphics.m_isUnlit = false;

		if (pConfig->keyExists(sName.c_str(), "model"))
			pMtrl->m_oMainGraphics.m_typeModel = (MTLTYPE_MODEL)String(pConfig->getKey(sName.c_str(), "model")).toInt();
		else
			pMtrl->m_oMainGraphics.m_typeModel = MTLTYPE_MODEL_DEFAULT;

		if (pConfig->keyExists(sName.c_str(), "physmaterial"))
			pMtrl->m_oPhysics.m_typePhysics = (MTLTYPE_PHYSIC)String(pConfig->getKey(sName.c_str(), "physmaterial")).toInt();
		else
			pMtrl->m_oPhysics.m_typePhysics = MTLTYPE_PHYSIC_DEFAULT;


		if (STR_VALIDATE(sVS.c_str()))
			pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, sVS.c_str(), sVS.c_str(), SHADER_CHECKDOUBLE_PATH);
		else
			pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgeom_base.vs");

		if (STR_VALIDATE(sPS.c_str()))
			pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, sPS.c_str(), sPS.c_str(), SHADER_CHECKDOUBLE_PATH);
		else
			pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgeom_base.ps");

		if (pConfig->keyExists(sName.c_str(), "microrelief_r"))
			sNormal = pConfig->getKey(sName.c_str(), "microrelief_r");

		if (STR_VALIDATE(sNormal.c_str()))
			pMtrl->m_oMicroDetail.m_aMicroRelief[0] = SGCore_LoadTexAddName(sNormal.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aMicroRelief[0] = -1;


		if (pConfig->keyExists(sName.c_str(), "microrelief_g"))
			sNormal = pConfig->getKey(sName.c_str(), "microrelief_g");

		if (STR_VALIDATE(sNormal.c_str()))
			pMtrl->m_oMicroDetail.m_aMicroRelief[1] = SGCore_LoadTexAddName(sNormal.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aMicroRelief[1] = -1;


		if (pConfig->keyExists(sName.c_str(), "microrelief_b"))
			sNormal = pConfig->getKey(sName.c_str(), "microrelief_b");
		if (STR_VALIDATE(sNormal.c_str()))
			pMtrl->m_oMicroDetail.m_aMicroRelief[2] = SGCore_LoadTexAddName(sNormal.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aMicroRelief[2] = -1;

		
		if (pConfig->keyExists(sName.c_str(), "microrelief_a"))
			sNormal = pConfig->getKey(sName.c_str(), "microrelief_a");
		if (STR_VALIDATE(sNormal.c_str()))
			pMtrl->m_oMicroDetail.m_aMicroRelief[3] = SGCore_LoadTexAddName(sNormal.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aMicroRelief[3] = -1;


		if (pConfig->keyExists(sName.c_str(), "detail_r"))
			sDetail = pConfig->getKey(sName.c_str(), "detail_r");
		if (STR_VALIDATE(sDetail.c_str()))
			pMtrl->m_oMicroDetail.m_aDetail[0] = SGCore_LoadTexAddName(sDetail.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aDetail[0] = -1;

		
		if (pConfig->keyExists(sName.c_str(), "detail_g"))
			sDetail = pConfig->getKey(sName.c_str(), "detail_g");
		if (STR_VALIDATE(sDetail.c_str()))
			pMtrl->m_oMicroDetail.m_aDetail[1] = SGCore_LoadTexAddName(sDetail.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aDetail[1] = -1;


		if (pConfig->keyExists(sName.c_str(), "detail_b"))
			sDetail = pConfig->getKey(sName.c_str(), "detail_b");
		if (STR_VALIDATE(sDetail.c_str()))
			pMtrl->m_oMicroDetail.m_aDetail[2] = SGCore_LoadTexAddName(sDetail.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aDetail[2] = -1;

		
		if (pConfig->keyExists(sName.c_str(), "detail_a"))
			sDetail = pConfig->getKey(sName.c_str(), "detail_a");
		if (STR_VALIDATE(sDetail.c_str()))
			pMtrl->m_oMicroDetail.m_aDetail[3] = SGCore_LoadTexAddName(sDetail.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_aDetail[3] = -1;

		if (pConfig->keyExists(sName.c_str(), "mask"))
			sMask = pConfig->getKey(sName.c_str(), "mask");
		if (STR_VALIDATE(sMask.c_str()))
			pMtrl->m_oMicroDetail.m_idMask = SGCore_LoadTexAddName(sMask.c_str(), LOAD_TEXTURE_TYPE_LOAD);
		else
			pMtrl->m_oMicroDetail.m_idMask = -1;


		pMtrl->m_oLightParam.m_fRoughness = MTL_LIGHTING_DEFAULT_ROUGHNESS;
		pMtrl->m_oLightParam.m_fF0 = MTL_LIGHTING_DEFAULT_F0;
		pMtrl->m_oLightParam.m_fThickness = MTL_LIGHTING_DEFAULT_THICKNESS;

		if (pConfig->keyExists(sName.c_str(), "roughness"))
			pMtrl->m_oLightParam.m_fRoughness = String(pConfig->getKey(sName.c_str(), "roughness")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "f0"))
			pMtrl->m_oLightParam.m_fF0 = String(pConfig->getKey(sName.c_str(), "f0")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "thickness"))
			pMtrl->m_oLightParam.m_fThickness = String(pConfig->getKey(sName.c_str(), "thickness")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "durability"))
			pMtrl->m_oPhysics.m_fDurability = String(pConfig->getKey(sName.c_str(), "durability")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "hit_chance"))
			pMtrl->m_oPhysics.m_fHitChance = String(pConfig->getKey(sName.c_str(), "hit_chance")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "density"))
			pMtrl->m_oPhysics.m_fDensity = String(pConfig->getKey(sName.c_str(), "density")).toDouble();

		pMtrl->m_oLightParam.m_idTexParamHand = createTexParamLighting(pMtrl->m_oLightParam.m_fRoughness, pMtrl->m_oLightParam.m_fF0, pMtrl->m_oLightParam.m_fThickness);

		//говорим что не установлено использовать ли текстуру или нет
		int istexparam = -1;

		//если есть ключ использования текстуры то грузим
		if (pConfig->keyExists(sName.c_str(), "use_texparamlight"))
			istexparam = String(pConfig->getKey(sName.c_str(), "use_texparamlight")).toBool();

		
		if (pConfig->keyExists(sName.c_str(), "texparamligth"))
			sTexParamLight = pConfig->getKey(sName.c_str(), "texparamligth");

		//если текстура с параметрами освещения была определена
		if (STR_VALIDATE(sTexParamLight.c_str()))
		{
			pMtrl->m_oLightParam.m_idTexParam = SGCore_LoadTexAddName(sTexParamLight.c_str(), LOAD_TEXTURE_TYPE_LOAD);
			//если использование параметров освещения из текстуры не было определено
			if (istexparam == -1)
				pMtrl->m_oLightParam.m_isTextureParam = true;
			else
				pMtrl->m_oLightParam.m_isTextureParam = istexparam;
		}
		//если использование параметров освещения из текстуры не было определено
		else if (istexparam == -1)
		{
			pMtrl->m_oLightParam.m_isTextureParam = false;
		}


		if (pConfig->keyExists(sName.c_str(), "transparent"))
			pMtrl->m_oLightParam.m_isTransparent = String(pConfig->getKey(sName.c_str(), "transparent")).toBool();
		else
			pMtrl->m_oLightParam.m_isTransparent = false;


		pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = pMtrl->m_oMainGraphics.m_oDataVS.m_isTransView = pMtrl->m_oMainGraphics.m_oDataVS.m_isTransProjection =
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView = pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = pMtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam =
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransTimeDelta = pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWinSize = pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserData = false;

		if (pConfig->keyExists(sName.c_str(), "vs_world"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = String(pConfig->getKey(sName.c_str(), "vs_world")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_view"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransView = String(pConfig->getKey(sName.c_str(), "vs_view")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_projection"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransProjection = String(pConfig->getKey(sName.c_str(), "vs_projection")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_worldview"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView = String(pConfig->getKey(sName.c_str(), "vs_worldview")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_worldviewprojection"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = String(pConfig->getKey(sName.c_str(), "vs_worldviewprojection")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_poscam"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam = String(pConfig->getKey(sName.c_str(), "vs_poscam")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_timedelta"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransTimeDelta = String(pConfig->getKey(sName.c_str(), "vs_timedelta")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_winsize"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWinSize = String(pConfig->getKey(sName.c_str(), "vs_winsize")).toInt();

		if (pConfig->keyExists(sName.c_str(), "vs_userdata"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserData = String(pConfig->getKey(sName.c_str(), "vs_userdata")).toInt();


		pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData = float4(0, 0, 0, 0);
		if (pConfig->keyExists(sName.c_str(), "vs_userdata_x"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData.x = String(pConfig->getKey(sName.c_str(), "vs_userdata_x")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "vs_userdata_y"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData.y = String(pConfig->getKey(sName.c_str(), "vs_userdata_y")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "vs_userdata_z"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData.z = String(pConfig->getKey(sName.c_str(), "vs_userdata_z")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "vs_userdata_w"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData.w = String(pConfig->getKey(sName.c_str(), "vs_userdata_w")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "vs_userdata_trans_ps"))
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserDataInOtherShader = String(pConfig->getKey(sName.c_str(), "vs_userdata_trans_ps")).toBool();
		else
			pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserDataInOtherShader = false;


		pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld = pMtrl->m_oMainGraphics.m_oDataPS.m_isTransView = pMtrl->m_oMainGraphics.m_oDataPS.m_isTransProjection =
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView = pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection = pMtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam =
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransTimeDelta = pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWinSize = pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData = false;


		if (pConfig->keyExists(sName.c_str(), "ps_world"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld = String(pConfig->getKey(sName.c_str(), "ps_world")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_view"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransView = String(pConfig->getKey(sName.c_str(), "ps_view")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_projection"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransProjection = String(pConfig->getKey(sName.c_str(), "ps_projection")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_worldview"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView = String(pConfig->getKey(sName.c_str(), "ps_worldview")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_worldviewprojection"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection = String(pConfig->getKey(sName.c_str(), "ps_worldviewprojection")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_poscam"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam = String(pConfig->getKey(sName.c_str(), "ps_poscam")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_timedelta"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransTimeDelta = String(pConfig->getKey(sName.c_str(), "ps_timedelta")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_winsize"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWinSize = String(pConfig->getKey(sName.c_str(), "ps_winsize")).toInt();

		if (pConfig->keyExists(sName.c_str(), "ps_userdata"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData = String(pConfig->getKey(sName.c_str(), "ps_userdata")).toInt();

		pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData = float4(0, 0, 0, 0);

		if (pConfig->keyExists(sName.c_str(), "ps_userdata_x"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData.x = String(pConfig->getKey(sName.c_str(), "ps_userdata_x")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "ps_userdata_y"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData.y = String(pConfig->getKey(sName.c_str(), "ps_userdata_y")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "ps_userdata_z"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData.z = String(pConfig->getKey(sName.c_str(), "ps_userdata_z")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "ps_userdata_w"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData.w = String(pConfig->getKey(sName.c_str(), "ps_userdata_w")).toDouble();

		if (pConfig->keyExists(sName.c_str(), "ps_userdata_trans_vs"))
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserDataInOtherShader = String(pConfig->getKey(sName.c_str(), "ps_userdata_trans_vs")).toBool();
		else
			pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserDataInOtherShader = false;

		if (pConfig->keyExists(sName.c_str(), "reflection"))
			pMtrl->m_oLightParam.m_typeReflect = (MTLTYPE_REFLECT)String(pConfig->getKey(sName.c_str(), "reflection")).toInt();

		if (pConfig->keyExists(sName.c_str(), "alphatest"))
			pMtrl->m_oMainGraphics.m_useAlphaTest = String(pConfig->getKey(sName.c_str(), "alphatest")).toBool();
		else
			pMtrl->m_oMainGraphics.m_useAlphaTest = false;

		return true;
	}

	return false;
}

ID CMaterials::createTexParamLighting(float roughness, float f0, float thickness)
{
	uint32_t tmpColor = GXCOLOR_ARGB(255, DWORD(roughness*255.f), DWORD(f0*255.f), DWORD(thickness*255.f));
	IGXTexture2D* TexMaterial = mtrl_data::pDXDevice->createTexture2D(1, 1, 1, 0, GXFMT_A8R8G8B8, &tmpColor);

	//SGCore_LoadTexLoadTextures();

	char nametex[64];
	sprintf(nametex, "%c_%d_%d_%d", MTL_VIRTUAL_DIR_HAND_MTL, m_aUnitMtrls.size(), rand() % 9999, rand() % 9999);

	return SGCore_LoadTexCreate(nametex, TexMaterial);
}

void CMaterials::createMtl(const char* name, CMaterial** mtl, MTLTYPE_MODEL type)
{
	CMaterial* pMtrl = *mtl;
	new(pMtrl)CMaterial*;
	//если такого материала не существует, то мы должны были задать примерный тип материала
	pMtrl->m_oMainGraphics.m_typeModel = type;
	//обычна¤ геометри¤
	if (type == MTLTYPE_MODEL_STATIC)
	{
		pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgeom_base.vs");
		pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgeom_base.ps");
		pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = true;
	}
	//деревь¤
	else if (type == MTLTYPE_MODEL_TREE)
	{
		pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgreen_tree_base.vs");
		pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgreen_base.ps");
		//pMtrl->RenderStates.m_oMainGraphics.m_useAlphaTest = true;
	}
	//трава
	else if (type == MTLTYPE_MODEL_GRASS)
	{
		pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlgreen_grass_base.vs");
		pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlgreen_base.ps");
		//pMtrl->RenderStates.m_oMainGraphics.m_useAlphaTest = true;
	}
	//анимационная модель
	else if (type == MTLTYPE_MODEL_SKIN)
	{
		pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(SHADER_TYPE_VERTEX, "mtrlskin_base.vs");
		pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(SHADER_TYPE_PIXEL, "mtrlskin_base.ps");
		pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = true;
	}
	//источник света
	/*else if (type == MTL_LIGHT)
	{
	pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderExistsName(0, "mtrlgeom_base.vs");
	pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderExistsName(1, "mtrlgeom_light.ps");
	pMtrl->IsForwardRender = true;
	pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData = true;
	pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData = float4(0, 0, 0, 0);
	}*/

	pMtrl->m_oMainGraphics.m_idMainTexture = SGCore_LoadTexAddName(name, LOAD_TEXTURE_TYPE_LOAD);
	pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = true;

	pMtrl->m_oLightParam.m_fRoughness = MTL_LIGHTING_DEFAULT_ROUGHNESS;
	pMtrl->m_oLightParam.m_fF0 = MTL_LIGHTING_DEFAULT_F0;
	pMtrl->m_oLightParam.m_fThickness = MTL_LIGHTING_DEFAULT_THICKNESS;

	pMtrl->m_oLightParam.m_idTexParam = -1;
	pMtrl->m_oLightParam.m_isTextureParam = false;
	pMtrl->m_oLightParam.m_idTexParamHand = createTexParamLighting(pMtrl->m_oLightParam.m_fRoughness, pMtrl->m_oLightParam.m_fF0, pMtrl->m_oLightParam.m_fThickness);

	//char path[1024];
	char tmp_name[256];
	bool IsTruePath = false;

	for (DWORD k = 0; k<strlen(name); k++)
	{
		if (name[k] == '.')
		{
			sprintf(tmp_name, "%s", name);
			tmp_name[k] = 0;
			IsTruePath = true;
			break;
		}
	}

	pMtrl->m_sName = tmp_name;
}

ID CMaterials::mtlLoad(const char* name, MTLTYPE_MODEL type)
{
	ID IsLoad = exists(name);

	if (IsLoad >= 0)
	{
		CUnitMaterial* tmpumtl = new CUnitMaterial();
		tmpumtl->m_pMtrl = m_aUnitMtrls[IsLoad]->m_pMtrl;

		if (m_aUnitMtrls[IsLoad]->m_pReflect){
			CUnitMaterial* tmpmtl = m_aUnitMtrls[IsLoad];
			tmpumtl->m_pReflect = new CReflection();
			tmpumtl->m_pReflect->init(m_aUnitMtrls[IsLoad]->m_pReflect->getTypeReflect());
		}

		return addUnitMaterial(tmpumtl);
	}
	else
	{
		CUnitMaterial* tmpumtl = new CUnitMaterial();
		tmpumtl->m_pMtrl = new CMaterial();
		addMaterial(tmpumtl->m_pMtrl);
		if (!loadMtl(name, &(tmpumtl->m_pMtrl)))
		{
			createMtl(name, &(tmpumtl->m_pMtrl), type);
		}
		else
		{
			if (tmpumtl->m_pMtrl->m_oLightParam.m_typeReflect != MTLTYPE_REFLECT_NONE)
			{
				tmpumtl->m_pReflect = new CReflection();
				tmpumtl->m_pReflect->init(tmpumtl->m_pMtrl->m_oLightParam.m_typeReflect);
			}
		}

		ID tmpid = addUnitMaterial(tmpumtl);

		addName(name, tmpid);
		return tmpid;
	}
}

void CMaterials::mtlReLoad(ID id, const char* name)
{
	MTL_PRE_COND_ID(id);
	CUnitMaterial* tmpumtl = m_aUnitMtrls[id];
	char tmpname[1024];
	if (name == 0)
		sprintf(tmpname, "%s.dds", tmpumtl->m_pMtrl->m_sName.c_str());
	else
		sprintf(tmpname, "%s", name);

	tmpumtl->m_pMtrl->nulling();
	if (!loadMtl(tmpname, &(tmpumtl->m_pMtrl)))
	{
		createMtl(name, &(tmpumtl->m_pMtrl), tmpumtl->m_pMtrl->m_oMainGraphics.m_typeModel);
	}
	else
	{
		if (tmpumtl->m_pMtrl->m_oLightParam.m_typeReflect != MTLTYPE_REFLECT_NONE)
		{
			tmpumtl->m_pReflect = new CReflection();
			tmpumtl->m_pReflect->init(tmpumtl->m_pMtrl->m_oLightParam.m_typeReflect);
		}
	}
}

void CMaterials::mtlSave(ID id)
{
	MTL_PRE_COND_ID(id, _VOID);

	char* ArrRGBA[4] = { "r", "g", "b", "a" };

	CMaterial* mtrl = m_aUnitMtrls[id]->m_pMtrl;

	String sDir = StrSubstrSpre(mtrl->m_sName.c_str(), "_");
	String sPath = String(Core_RStringGet(G_RI_STRING_PATH_GS_MTRLS)) + sDir + "/";

	FileCreateDir(Core_RStringGet(G_RI_STRING_PATH_GS_MTRLS));
	FileCreateDir(sPath.c_str());
	sPath += FileSetStrExt(mtrl->m_sName.c_str(), ".mtl");

	FILE* file = 0;
	file = fopen(sPath.c_str(), "w");

	fprintf(file, "[%s]\n", mtrl->m_sName.c_str());

	char tmpPathVSName[1024];
	char tmpPathPSName[1024];

	char tmpPathMaskName[1024];

	SGCore_ShaderGetPath(SHADER_TYPE_VERTEX, mtrl->m_oMainGraphics.m_idShaderVS, tmpPathVSName);
	SGCore_ShaderGetPath(SHADER_TYPE_PIXEL, mtrl->m_oMainGraphics.m_idShaderPS, tmpPathPSName);

	fprintf(file, "model = %d\n", mtrl->m_oMainGraphics.m_typeModel);
	fprintf(file, "lighted = %d\n", mtrl->m_oMainGraphics.m_isUnlit);
	fprintf(file, "physmaterial = %d\n", mtrl->m_oPhysics.m_typePhysics);

	fprintf(file, "vs = %s\n", tmpPathVSName);
	fprintf(file, "ps = %s\n", tmpPathPSName);

	fprintf(file, "alphatest = %d\n", mtrl->m_oMainGraphics.m_useAlphaTest);

	char namebasetex[256];
	namebasetex[0] = '0';
	namebasetex[1] = '\0';
	SGCore_LoadTexGetName(mtrl->m_oMainGraphics.m_idMainTexture, namebasetex);
	fprintf(file, "texture = %s\n", namebasetex);
	fprintf(file, "use_dest_color = %d\n\n", mtrl->m_oMainGraphics.m_useDestColor);

	if (mtrl->m_oMicroDetail.m_idMask == -1)
	{
		tmpPathMaskName[0] = '0';
		tmpPathMaskName[1] = 0;
	}
	else
		SGCore_LoadTexGetName(mtrl->m_oMicroDetail.m_idMask, tmpPathMaskName);

	fprintf(file, "mask = %s\n", tmpPathMaskName);

	for (int k = 0; k<4; k++)
	{
		if (mtrl->m_oMicroDetail.m_aMicroRelief[k] == -1)
		{
			tmpPathMaskName[0] = '0';
			tmpPathMaskName[1] = 0;
		}
		else
			SGCore_LoadTexGetName(mtrl->m_oMicroDetail.m_aMicroRelief[k], tmpPathMaskName);

		fprintf(file, "microrelief_%s = %s\n", ArrRGBA[k], tmpPathMaskName);
	}

	for (int k = 0; k<4; k++)
	{
		if (mtrl->m_oMicroDetail.m_aDetail[k] == -1)
		{
			tmpPathMaskName[0] = '0';
			tmpPathMaskName[1] = 0;
		}
		else
			SGCore_LoadTexGetName(mtrl->m_oMicroDetail.m_aDetail[k], tmpPathMaskName);

		fprintf(file, "detail_%s = %s\n", ArrRGBA[k], tmpPathMaskName);
	}

	fprintf(file, "\n");

	fprintf(file, "vs_world = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld);
	fprintf(file, "vs_view = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransView);
	fprintf(file, "vs_projection = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransProjection);
	fprintf(file, "vs_worldview = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView);
	fprintf(file, "vs_worldviewprojection = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection);

	fprintf(file, "vs_poscam = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam);
	fprintf(file, "vs_timedelta = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransTimeDelta);
	fprintf(file, "vs_winsize = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransWinSize);
	fprintf(file, "vs_userdata = %d\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransUserData);
	fprintf(file, "vs_userdata_x = %f\nvs_userdata_y = %f\nvs_userdata_z = %f\nvs_userdata_w = %f\n", mtrl->m_oMainGraphics.m_oDataVS.m_vUserData.x, mtrl->m_oMainGraphics.m_oDataVS.m_vUserData.y, mtrl->m_oMainGraphics.m_oDataVS.m_vUserData.z, mtrl->m_oMainGraphics.m_oDataVS.m_vUserData.w);
	fprintf(file, "vs_userdata_trans_ps = %d\n\n", mtrl->m_oMainGraphics.m_oDataVS.m_isTransUserDataInOtherShader);

	fprintf(file, "ps_world = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld);
	fprintf(file, "ps_view = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransView);
	fprintf(file, "ps_projection = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransProjection);
	fprintf(file, "ps_worldview = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView);
	fprintf(file, "ps_worldviewprojection = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection);

	fprintf(file, "ps_poscam = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam);
	fprintf(file, "ps_timedelta = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransTimeDelta);
	fprintf(file, "ps_winsize = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransWinSize);
	fprintf(file, "ps_userdata = %d\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData);
	fprintf(file, "ps_userdata_x = %f\nps_userdata_y = %f\nps_userdata_z = %f\nps_userdata_w = %f\n", mtrl->m_oMainGraphics.m_oDataPS.m_vUserData.x, mtrl->m_oMainGraphics.m_oDataPS.m_vUserData.y, mtrl->m_oMainGraphics.m_oDataPS.m_vUserData.z, mtrl->m_oMainGraphics.m_oDataPS.m_vUserData.w);
	fprintf(file, "ps_userdata_trans_vs = %d\n\n", mtrl->m_oMainGraphics.m_oDataPS.m_isTransUserDataInOtherShader);

	if (mtrl->m_oLightParam.m_idTexParam == -1)
	{
		tmpPathMaskName[0] = '0';
		tmpPathMaskName[1] = 0;
	}
	else
		SGCore_LoadTexGetName(mtrl->m_oLightParam.m_idTexParam, tmpPathMaskName);

	if (tmpPathMaskName[0] == MTL_VIRTUAL_DIR_HAND_MTL)
		sprintf(tmpPathMaskName, "0");
	//else
		fprintf(file, "use_texparamlight = %d\n", mtrl->m_oLightParam.m_isTextureParam);

	fprintf(file, "texparamligth = %s\n", tmpPathMaskName);

	fprintf(file, "roughness = %f\n", mtrl->m_oLightParam.m_fRoughness);
	fprintf(file, "f0 = %f\n", mtrl->m_oLightParam.m_fF0);
	fprintf(file, "thickness = %f\n", mtrl->m_oLightParam.m_fThickness);
	fprintf(file, "durability = %f\n", mtrl->m_oPhysics.m_fDurability);
	fprintf(file, "hit_chance = %f\n", mtrl->m_oPhysics.m_fHitChance);
	fprintf(file, "density = %f\n\n", mtrl->m_oPhysics.m_fDensity);

	fprintf(file, "transparent = %d\n", mtrl->m_oLightParam.m_isTransparent);
	fprintf(file, "reflection = %d\n", mtrl->m_oLightParam.m_typeReflect);

	fclose(file);
}

//##########################################################################

int CMaterials::delRefCount()
{
	return m_aReflections.size();
}

ID CMaterials::delRefGetIDArr(ID key, ID inid, int cube)
{
	if (!(key >= 0 && key < m_aReflections.size()))
		return -1;

	return m_aReflections[key]->getIDvisCalcObj(inid, cube);
}

void CMaterials::delRefAllDel()
{
	for (int i = 0; i < m_aReflections.size(); ++i)
	{
		mem_delete(m_aReflections[i]);
	}

	m_aReflections.clear();
}

//##########################################################################

void CMaterials::clear(bool isClearRefDel)
{
	int tmpcountdel = 0;
	
	for (int i = 0; i < m_aMtrlPathes.size(); ++i)
	{
		if (strcmp(m_aMtrlPathes[i]->m_sPath.c_str(), MTL_VIRTUAL_DIR_STD_MTL) != 0)
		{
			for (int k = 0; k < m_aMtrlPathes[i]->m_aObjects.size(); ++k)
			{
				mem_delete(m_aMtrlPathes[i]->m_aObjects[k]);
				m_aMtrlPathes[i]->m_aObjects.erase(k);
				--k;
				++tmpcountdel;
			}

			if (m_aMtrlPathes.size() <= 0)
			{
				mem_delete(m_aMtrlPathes[i]);
				m_aMtrlPathes.erase(i);
				--i;
			}
		}
	}

	tmpcountdel = 0;

	for (int i = m_idBeginNonDef; i < m_aUnitMtrls.size(); ++i)
	{
		//если есть отражения, то записываем в очередь на удаление
		if (m_aUnitMtrls[i]->m_pReflect)
			m_aReflections.push_back(m_aUnitMtrls[i]->m_pReflect);

		mem_delete(m_aUnitMtrls[i]);
		++tmpcountdel;
	}

	CurrFirstFree = m_idBeginNonDef;

	tmpcountdel = 0;

	for (int i = 0; i < m_aMtrls.size(); ++i)
	{
		if (m_aMtrls[i]->m_isDelete)
		{
			mem_delete(m_aMtrls[i]);
			m_aMtrls.erase(i);
			--i;
			++tmpcountdel;
		}
	}

	if (isClearRefDel)
		delRefAllDel();
}

//##########################################################################

void CMaterials::onLostDevice()
{
	for (int i = 0; i < m_aUnitMtrls.size(); ++i)
	{
		if (m_aUnitMtrls[i] && m_aUnitMtrls[i]->m_pReflect)
			m_aUnitMtrls[i]->m_pReflect->onLostDevice();
	}
}

void CMaterials::onResetDevice()
{
	for (int i = 0; i < m_aUnitMtrls.size(); ++i)
	{
		if (m_aUnitMtrls[i] && m_aUnitMtrls[i]->m_pReflect)
			m_aUnitMtrls[i]->m_pReflect->onResetDevice();
	}
}

//##########################################################################

ID CMaterials::getStdMtl(MTLTYPE_MODEL m_typeModel)
{
	if (m_typeModel == MTLTYPE_MODEL_STATIC)
		return m_idMtrlDefStatic;
	else if (m_typeModel == MTLTYPE_MODEL_GRASS)
		return m_idMtrlDefGrass;
	else if (m_typeModel == MTLTYPE_MODEL_TREE)
		return m_idMtrlDefTree;
	else if (m_typeModel == MTLTYPE_MODEL_SKIN)
		return m_idMtrlDefSkin;

	return(-1);
}

MTLTYPE_MODEL CMaterials::getTypeModel(ID id)
{
	MTL_PRE_COND_ID(id, MTLTYPE_MODEL_STATIC);
	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_typeModel;
}

ID CMaterials::getLightMtrl()
{
	return m_idMtrlDefLight;
}

void CMaterials::setTypeModel(ID id, MTLTYPE_MODEL typeModel)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_typeModel = typeModel;
}

void CMaterials::update(UINT timeDelta)
{
	m_uiCurrTimeDelta = timeDelta;
	m_uiCountTimeDelta += timeDelta;
}

void CMaterials::setMainTexture(ID slot, ID id)
{
	if (id >= 0 && id < m_aUnitMtrls.size() && m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idMainTexture != -1)
		mtrl_data::pDXDevice->setTexture(SGCore_LoadTexGetTex(m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idMainTexture), slot);
	else
		mtrl_data::pDXDevice->setTexture(0, slot);
}

ID CMaterials::getID(const char* name)
{
	for (int i = 0; i < m_aUnitMtrls.size(); i++)
	{
		if (strcmp(m_aUnitMtrls[i]->m_pMtrl->m_sName.c_str(), name) == 0)
			return i;
	}
	return -1;
}

int CMaterials::getCount()
{
	return m_aUnitMtrls.size();
}


void CMaterials::setForceblyAlphaTest(bool isat)
{
	m_useForceblyAlphaTest = isat;
	if (!isat)
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

bool CMaterials::getForceblyAlphaTest()
{
	return m_useForceblyAlphaTest;
}

//##########################################################################

bool CMaterials::mtlGetUsingAlphaTest(ID id)
{
	MTL_PRE_COND_ID(id, false);
	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_useAlphaTest;
}

void CMaterials::mtlSetUsingAlphaTest(ID id, bool is_using)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_useAlphaTest = is_using;
}

MTLTYPE_REFLECT CMaterials::mtlTypeReflection(ID id)
{
	MTL_PRE_COND_ID(id, MTLTYPE_REFLECT_NONE);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_typeReflect;
}

//**************************************************************************

void CMaterials::mtlRefSetIDArr(ID id, ID inid, int cube, ID idarr)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->setIDvisCalcObj(inid, cube, idarr);
}

ID CMaterials::mtlRefGetIDArr(ID id, ID inid, int cube)
{
	MTL_REF_PRE_COND_ID(id,- 1);
	return m_aUnitMtrls[id]->m_pReflect->getIDvisCalcObj(inid, cube);
}

void CMaterials::mtlRefPreRenderPlane(ID id, SMPLANE* plane)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->preRenderRefPlane(plane);
}

const IFrustum* CMaterials::mtlRefGetfrustum(ID id, int cube)
{
	MTL_REF_PRE_COND_ID(id, 0);
	if (cube == 0 || (cube > 0 && (m_aUnitMtrls[id]->m_pReflect->getTypeReflect() == MTLTYPE_REFLECT_CUBE_STATIC || m_aUnitMtrls[id]->m_pReflect->getTypeReflect() == MTLTYPE_REFLECT_CUBE_DYNAMIC)))
		return m_aUnitMtrls[id]->m_pReflect->getFrustum(cube);
	return 0;
}

void CMaterials::mtlRefPostRenderPlane(ID id)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->postRenderRefPlane();
}

IGXTexture2D* CMaterials::mtlRefPlaneGetTex(ID id)
{
 	MTL_REF_PRE_COND_ID(id, 0);
	return m_aUnitMtrls[id]->m_pReflect->getRefPlaneTex();
}


void CMaterials::mtlRefSetMinMax(ID id, const float3_t *pMin, const float3_t *pMax)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->setMinMax(pMin, pMax);
}

bool CMaterials::mtlRefIsAllowedRender(ID  id)
{
	MTL_REF_PRE_COND_ID(id, false);
	if (m_aUnitMtrls[id]->m_pReflect)
		return m_aUnitMtrls[id]->m_pReflect->allowedRender();

	return false;
}


void CMaterials::mtlRefCubeBeginRender(ID id, const float3_t *pCenter)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->beginRenderRefCube(pCenter);
}

void CMaterials::mtlRefCubePreRender(ID id, int iCube, const float4x4 *pWorld)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->preRenderRefCube(iCube, pWorld);
}

void CMaterials::mtlRefCubePostRender(ID id, int iCube)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->postRenderRefCube(iCube);
}

void CMaterials::mtlRefCubeEndRender(ID id, const float3_t *pViewPos)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->endRenderRefCube(pViewPos);
}

bool CMaterials::mtlRefUpdateCountUpdate(ID id, const float3_t *pViewPos)
{
	MTL_REF_PRE_COND_ID(id, false);
	return m_aUnitMtrls[id]->m_pReflect->updateCountUpdate(pViewPos);
}

void CMaterials::mtlRefNullingCountUpdate(ID id)
{
	MTL_REF_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pReflect->nullingCountUpdate();
}

IGXTextureCube* CMaterials::refCubeGetTex(ID id)
{
	MTL_REF_PRE_COND_ID(id, 0);
	return m_aUnitMtrls[id]->m_pReflect->getRefCubeTex();
}

//**************************************************************************

UINT CMaterials::mtlGetSort(ID id)
{
	MTL_PRE_COND_ID(id, 0);
	
	UINT uiSort = 0;
	uiSort |= (m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_isTransparent ? MTLTYPE_TRANSPARENCY : MTLSORT_OPAQUE);
	uiSort |= (m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_isUnlit ? MTLTYPE_UNLIT : MTLSORT_LIGHTED);
	return uiSort;
}

void CMaterials::mtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type)
{
	MTL_PRE_COND_ID(id);
	m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_typePhysics = type;
}

MTLTYPE_PHYSIC CMaterials::mtlGetPhysicMaterial(ID id)
{
	MTL_PRE_COND_ID(id, MTLTYPE_PHYSIC_CONCRETE);
	return m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_typePhysics;
}

void CMaterials::mtlSetTexture(ID id, const char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idMainTexture = SGCore_LoadTexAddName(path_tex, LOAD_TEXTURE_TYPE_LOAD);
}

void CMaterials::mtlGetTexture(ID id, char* name)
{
	MTL_PRE_COND_ID(id, _VOID);

	if (name && m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idMainTexture >= 0)
		SGCore_LoadTexGetName(m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idMainTexture, name);
}

void CMaterials::mtlSetUseDestColor(ID id, bool useDestColor)
{
	MTL_PRE_COND_ID(id, _VOID);

	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_useDestColor = useDestColor;
}

bool CMaterials::mtlGetUseDestColor(ID id)
{
	MTL_PRE_COND_ID(id, false);

	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_useDestColor;
}

ID CMaterials::mtlGetTextureID(ID id)
{
	MTL_PRE_COND_ID(id, -1);

	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idMainTexture;
}

void CMaterials::mtlSetVS(ID id, const char* path_vs)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, path_vs, path_vs, SHADER_CHECKDOUBLE_PATH);
}

void CMaterials::mtlGetVS(ID id, char* name)
{
	MTL_PRE_COND_ID(id, _VOID);
	SGCore_ShaderGetPath(SHADER_TYPE_VERTEX, m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idShaderVS, name);
}

void CMaterials::mtlSetPS(ID id, const char* path_ps)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, path_ps, path_ps, SHADER_CHECKDOUBLE_PATH);
}

void CMaterials::mtlGetPS(ID id, char* name)
{
	MTL_PRE_COND_ID(id, _VOID);
	SGCore_ShaderGetPath(SHADER_TYPE_PIXEL, m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_idShaderPS, name);
}


void CMaterials::mtlSetLighting(ID id, bool is_lighting)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_isUnlit = !is_lighting;
}

bool CMaterials::mtlGetLighting(ID id)
{
	MTL_PRE_COND_ID(id, false);
	return !m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_isUnlit;
}


void CMaterials::mtlSetIsTextureLighting(ID id, bool is_tex_lighting)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_isTextureParam = is_tex_lighting;
}

bool CMaterials::mtlGetIsTextureLighting(ID id)
{
	MTL_PRE_COND_ID(id, false);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_isTextureParam;
}


void CMaterials::mtlSetTextureLighting(ID id, const char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_idTexParam = SGCore_LoadTexAddName(path_tex, LOAD_TEXTURE_TYPE_LOAD);
}

void CMaterials::mtlGetTextureLighting(ID id, char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	if (path_tex && m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_idTexParam >= 0)
	{
		SGCore_LoadTexGetName(m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_idTexParam, path_tex);
		if (path_tex[0] == MTL_VIRTUAL_DIR_HAND_MTL)
			path_tex[0] = 0;
	}
	else if (path_tex)
		path_tex[0] = 0;
}


void CMaterials::mtlSetRoughness(ID id, float roughness)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_fRoughness = roughness;
}

float CMaterials::mtlGetRoughness(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_fRoughness;
}


void CMaterials::mtlSetF0(ID id, float f0)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_fF0 = f0;
}

float CMaterials::mtlGetF0(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_fF0;
}


void CMaterials::mtlSetThickness(ID id, float thickness)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_fThickness = thickness;
}

float CMaterials::mtlGetThickness(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_fThickness;
}


void CMaterials::mtlSetDurability(ID id, float fDurability)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_fDurability = fDurability;
}

float CMaterials::mtlGetDurability(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_fDurability;
}


void CMaterials::mtlSetHitChance(ID id, float fHitChance)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_fHitChance = fHitChance;
}

float CMaterials::mtlGetHitChance(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_fHitChance;
}


void CMaterials::mtlSetDensity(ID id, float fDensity)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_fDensity = fDensity;
}

float CMaterials::mtlGetDensity(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oPhysics.m_fDensity;
}



void CMaterials::mtlSetTransparency(ID id, bool isTransparent)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_isTransparent = isTransparent;
}

bool CMaterials::mtlGetTransparency(ID id)
{
	MTL_PRE_COND_ID(id, false);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_isTransparent;
}


void CMaterials::mtlSetTypeReflection(ID id, MTLTYPE_REFLECT type)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_typeReflect = type;

	if (m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_typeReflect != MTLTYPE_REFLECT_NONE)
	{
		if (!m_aUnitMtrls[id]->m_pReflect)
			m_aUnitMtrls[id]->m_pReflect = new CReflection();

		m_aUnitMtrls[id]->m_pReflect->init(m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_typeReflect);
	}
}

MTLTYPE_REFLECT CMaterials::mtlGetTypeReflection(ID id)
{
	MTL_PRE_COND_ID(id, MTLTYPE_REFLECT_NONE);
	return m_aUnitMtrls[id]->m_pMtrl->m_oLightParam.m_typeReflect;
}


void CMaterials::mtlSetMaskTex(ID id, const char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_idMask = SGCore_LoadTexAddName(path_tex, LOAD_TEXTURE_TYPE_LOAD);
}

void CMaterials::mtlGetMaskTex(ID id, char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	if (path_tex && m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_idMask >= 0)
		SGCore_LoadTexGetName(m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_idMask, path_tex);
	else if (path_tex)
		path_tex[0] = 0;
}

void CMaterials::mtlSetMRTex(ID id, int channel, const char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_aMicroRelief[channel] = SGCore_LoadTexAddName(path_tex, LOAD_TEXTURE_TYPE_LOAD);
}

void CMaterials::mtlGetMRTex(ID id, int channel, char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	if (path_tex && m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_aMicroRelief[channel] >= 0)
		SGCore_LoadTexGetName(m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_aMicroRelief[channel], path_tex);
	else if (path_tex)
		path_tex[0] = 0;
}


void CMaterials::mtlSetDTex(ID id, int channel, const char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_aDetail[channel] = SGCore_LoadTexAddName(path_tex, LOAD_TEXTURE_TYPE_LOAD);
}

void CMaterials::mtlGetDTex(ID id, int channel, char* path_tex)
{
	MTL_PRE_COND_ID(id, _VOID);
	if (path_tex && m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_aDetail[channel] >= 0)
		SGCore_LoadTexGetName(m_aUnitMtrls[id]->m_pMtrl->m_oMicroDetail.m_aDetail[channel], path_tex);
	else if (path_tex)
		path_tex[0] = 0;
}


void CMaterials::mtlSetStdVS(ID id, MTL_SHADERSTD type, bool is_send)
{
	MTL_PRE_COND_ID(id, _VOID);
	if (type == MTL_SHADERSTD_MATRIX_WORLD)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_VIEW)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransView = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_PROJECTION)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransProjection = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEW)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection = is_send;

	else if (type == MTL_SHADERSTD_CAMPOS)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam = is_send;
	else if (type == MTL_SHADERSTD_TIMEDELTA)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransTimeDelta = is_send;

	else if (type == MTL_SHADERSTD_USERDATA)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserData = is_send;

	else if (type == MTL_SHADERSTD_WINSIZE)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWinSize = is_send;
}

bool CMaterials::mtlGetStdVS(ID id, MTL_SHADERSTD type)
{
	MTL_PRE_COND_ID(id, false);
	if (type == MTL_SHADERSTD_MATRIX_WORLD)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld;
	else if (type == MTL_SHADERSTD_MATRIX_VIEW)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransView;
	else if (type == MTL_SHADERSTD_MATRIX_PROJECTION)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransProjection;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEW)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection;

	else if (type == MTL_SHADERSTD_CAMPOS)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam;
	else if (type == MTL_SHADERSTD_TIMEDELTA)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransTimeDelta;

	else if (type == MTL_SHADERSTD_USERDATA)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserData;

	else if (type == MTL_SHADERSTD_WINSIZE)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWinSize;

	return(false);
}

void CMaterials::mtlSetStdPS(ID id, MTL_SHADERSTD type, bool is_send)
{
	MTL_PRE_COND_ID(id, _VOID);
	if (type == MTL_SHADERSTD_MATRIX_WORLD)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_VIEW)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransView = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_PROJECTION)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransProjection = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEW)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView = is_send;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection = is_send;

	else if (type == MTL_SHADERSTD_CAMPOS)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam = is_send;
	else if (type == MTL_SHADERSTD_TIMEDELTA)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransTimeDelta = is_send;

	else if (type == MTL_SHADERSTD_USERDATA)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData = is_send;

	else if (type == MTL_SHADERSTD_WINSIZE)
		m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWinSize = is_send;
}

bool CMaterials::mtlGetStdPS(ID id, MTL_SHADERSTD type)
{
	MTL_PRE_COND_ID(id, false);
	if (type == MTL_SHADERSTD_MATRIX_WORLD)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld;
	else if (type == MTL_SHADERSTD_MATRIX_VIEW)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransView;
	else if (type == MTL_SHADERSTD_MATRIX_PROJECTION)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransProjection;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEW)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView;
	else if (type == MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection;

	else if (type == MTL_SHADERSTD_CAMPOS)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam;
	else if (type == MTL_SHADERSTD_TIMEDELTA)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransTimeDelta;

	else if (type == MTL_SHADERSTD_USERDATA)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData;

	else if (type == MTL_SHADERSTD_WINSIZE)
		return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWinSize;
	return(false);
}


void CMaterials::mtlSetUserDataVS(ID id, int component, float val)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData[component] = val;
}

float CMaterials::mtlGetUserDataVS(ID id, int component)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData[component];
}

void CMaterials::mtlSetUserDataVS_InPS(ID id, bool is_send_ps)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserDataInOtherShader = is_send_ps;
}

bool CMaterials::mtlGetUserDataVS_InPS(ID id)
{
	MTL_PRE_COND_ID(id, false);
	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserDataInOtherShader;
}


void CMaterials::mtlSetUserDataPS(ID id, int component, float val)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData[component] = val;
}

float CMaterials::mtlGetUserDataPS(ID id, int component)
{
	MTL_PRE_COND_ID(id, -1);
	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData[component];
}

void CMaterials::mtlSetUserDataPS_InPS(ID id, bool is_send_vs)
{
	MTL_PRE_COND_ID(id, _VOID);
	m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserDataInOtherShader = is_send_vs;
}

bool CMaterials::mtlGetUserDataPS_InPS(ID id)
{
	MTL_PRE_COND_ID(id, false);
	return m_aUnitMtrls[id]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserDataInOtherShader;
}

//##########################################################################

void CMaterials::setIsIncrCountSurf(bool bf)
{
	m_useCountSurface = bf;
}

bool CMaterials::getIsIncrCountSurf()
{
	return m_useCountSurface;
}

void CMaterials::nullingCurrCountSurf()
{
	m_idCurrIdSurface = 0;
}

int CMaterials::getCurrCountSurf()
{
	return m_idCurrIdSurface;
}

void CMaterials::setCurrCountSurf(int count)
{
	m_idCurrIdSurface = count;
}

//##########################################################################

void CMaterials::renderStd(MTLTYPE_MODEL type, const float4x4 *pWorld, ID idSlot, ID idMtl)
{
	//может быть случай когда текстура в которую сейчас рисуем еще стоит в текстурных слотах
	//из-за этого может быть необъяснимое поводенеие и как результат непонятные артефакты в самой текстуре в которую сейчас рисуем
	//поэтому нужно обнулить слот в котором возможно была текстура
	//такое явление может быть в случае когда в кадре только один материал который отражает
	mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_REFLECTION, 0);

	if (idMtl >= 0 && idMtl < m_aUnitMtrls.size())
		setMainTexture(idSlot, idMtl);

	if (type == MTLTYPE_MODEL_STATIC)
	{
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, mtrl_data::shader_id::vs::idStdGeom);

		float4x4 wmat = (pWorld ? (*pWorld) : SMMatrixIdentity());
		float4x4 wvpmat;
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &wvpmat);
		wvpmat = SMMatrixTranspose(wmat * wvpmat);
		wmat = SMMatrixTranspose(wmat);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, mtrl_data::shader_id::vs::idStdGeom, "g_mWVP", &wvpmat);
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, mtrl_data::shader_id::vs::idStdGeom, "g_mW", &wmat);

		if (Core_RBoolGet(G_RI_BOOL_CLIPPLANE0))
		{
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGeomCP);

			float3 tmpnormal, tmppoint;

			Core_RFloat3Get(G_RI_FLOAT3_CLIPPLANE0_NORMAL, &tmpnormal);
			Core_RFloat3Get(G_RI_FLOAT3_CLIPPLANE0_POINT, &tmppoint);

			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGeomCP, "g_vPlaneNormal", &tmpnormal);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGeomCP, "g_vPlanePoint", &tmppoint);
		}
		else
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGeom);
	}
	else if (type == MTLTYPE_MODEL_GRASS || type == MTLTYPE_MODEL_TREE)
	{
		ID tmpvs = (type == MTLTYPE_MODEL_GRASS ? mtrl_data::shader_id::vs::idStdGrass : mtrl_data::shader_id::vs::idStdTree);
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, tmpvs);

		float4x4 wmat = (pWorld ? (*pWorld) : SMMatrixIdentity());
		float4x4 wvpmat;
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &wvpmat);
		wvpmat = SMMatrixTranspose(wmat * wvpmat);
		wmat = SMMatrixTranspose(wmat);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, tmpvs, "g_mWVP", &wvpmat);
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, tmpvs, "g_mW", &wmat);

		if (Core_RBoolGet(G_RI_BOOL_CLIPPLANE0))
		{
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGreenCP);

			float3 tmpnormal, tmppoint;

			Core_RFloat3Get(G_RI_FLOAT3_CLIPPLANE0_NORMAL, &tmpnormal);
			Core_RFloat3Get(G_RI_FLOAT3_CLIPPLANE0_POINT, &tmppoint);

			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGreenCP, "g_vPlaneNormal", &tmpnormal);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGreenCP, "g_vPlanePoint", &tmppoint);
		}
		else
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdGreen);
	}
	else if (type == MTLTYPE_MODEL_SKIN)
	{
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, mtrl_data::shader_id::vs::idStdSkin);

		float4x4 wmat = (pWorld ? (*pWorld) : SMMatrixIdentity());
		float4x4 wvpmat;
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &wvpmat);
		wvpmat = SMMatrixTranspose(wmat * wvpmat);
		wmat = SMMatrixTranspose(wmat);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, mtrl_data::shader_id::vs::idStdSkin, "g_mWVP", &wvpmat);
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, mtrl_data::shader_id::vs::idStdSkin, "g_mW", &wmat);

		if (Core_RBoolGet(G_RI_BOOL_CLIPPLANE0))
		{
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdSkinCP);

			float3 tmpnormal, tmppoint;

			Core_RFloat3Get(G_RI_FLOAT3_CLIPPLANE0_NORMAL, &tmpnormal);
			Core_RFloat3Get(G_RI_FLOAT3_CLIPPLANE0_POINT, &tmppoint);

			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdSkinCP, "g_vPlaneNormal", &tmpnormal);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdSkinCP, "g_vPlanePoint", &tmppoint);
		}
		else
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, mtrl_data::shader_id::ps::idStdSkin);
	}
}

void CMaterials::render(ID id, const float4x4 *pWorld, const float4 *pColor)
{
	MTL_PRE_COND_ID(id, _VOID);

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	m_mWorld = (pWorld ? (*pWorld) : SMMatrixIdentity());

	CMaterial *pMtrl = m_aUnitMtrls[id]->m_pMtrl;

	//если есть то устанавливаем текстуру материала
	if (pMtrl->m_oMainGraphics.m_idMainTexture != -1)
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_MAIN, SGCore_LoadTexGetTex(pMtrl->m_oMainGraphics.m_idMainTexture));

	//если нет отражени¤ то отправл¤ем 0
	if (pMtrl->m_oLightParam.m_typeReflect == 0)
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_REFLECTION, 0);
	else
	{
		if (m_aUnitMtrls[id]->m_pReflect->getTypeReflect() == MTLTYPE_REFLECT_PLANE)
			mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_REFLECTION, m_aUnitMtrls[id]->m_pReflect->getRefPlaneTex());
		else if (m_aUnitMtrls[id]->m_pReflect->getTypeReflect() == MTLTYPE_REFLECT_CUBE_STATIC || m_aUnitMtrls[id]->m_pReflect->getTypeReflect() == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_REFLECTION, m_aUnitMtrls[id]->m_pReflect->getRefCubeTex());
	}

	mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_CURR_DEPTH, SGCore_GbufferGetRT(DS_RT_DEPTH0));

	//если есть рефаркци¤, а она идет вторым проходом, то отправл¤ем, иначе посылаем 0
	/*if (pMtrl->m_oLightParam.m_type_transparency)
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_REFRACTION, SGCore_RTGetTexture(SML_DSGetRT_ID(DS_RT_SCENE_LIGHT_COM_REF)));
	else
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_REFRACTION, 0);*/

	if (pMtrl->m_oMicroDetail.m_idMask != -1)
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_MASK, SGCore_LoadTexGetTex(pMtrl->m_oMicroDetail.m_idMask));

	for (int k = 0; k<4; k++)
	{
		if (pMtrl->m_oMicroDetail.m_aDetail[k] != -1)
			mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_DETAIL + k, SGCore_LoadTexGetTex(pMtrl->m_oMicroDetail.m_aDetail[k]));
		else
			mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_DETAIL + k, 0);

		if (pMtrl->m_oMicroDetail.m_aMicroRelief[k] != -1)
			mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_MICRO + k, SGCore_LoadTexGetTex(pMtrl->m_oMicroDetail.m_aMicroRelief[k]));
		else
			mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_MICRO + k, 0);
	}

	//если есть текстура с параметрами освещени¤ и установлено что берем параметры из текстуры, то отправл¤ем текстуру с параметрами
	if (pMtrl->m_oLightParam.m_idTexParam != -1 && pMtrl->m_oLightParam.m_isTextureParam)
	{
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_PARAM_LIGHT, SGCore_LoadTexGetTex(pMtrl->m_oLightParam.m_idTexParam));
	}
	//иначе если берем параметры из ... редактора
	else //if (!pMtrl->m_oLightParam.m_isTextureParam)
	{
		if (pMtrl->m_oLightParam.m_fOldRoughness != pMtrl->m_oLightParam.m_fRoughness || pMtrl->m_oLightParam.m_fOldF0 != pMtrl->m_oLightParam.m_fF0 || pMtrl->m_oLightParam.m_fOldThickness != pMtrl->m_oLightParam.m_fThickness)
		{
			//блокируем текстуру 1х1 котора¤ есть параметры освещени¤, и запсиываем туда то что настроили
			D3DLOCKED_RECT LockedRect;
			IDirect3DTexture9* ParamLightModelTex = SGCore_LoadTexGetTex(pMtrl->m_oLightParam.m_idTexParamHand);
			ParamLightModelTex->LockRect(0, &LockedRect, 0, 0);
			DWORD *param = (DWORD*)LockedRect.pBits;
			//DWORD param = D3DCOLOR_ARGB(0,0,0,0);
			param[0] = D3DCOLOR_ARGB(255, DWORD(pMtrl->m_oLightParam.m_fRoughness*255.f), DWORD(pMtrl->m_oLightParam.m_fF0*255.f), DWORD(pMtrl->m_oLightParam.m_fThickness*255.f));
			ParamLightModelTex->UnlockRect(0);

			pMtrl->m_oLightParam.m_fOldRoughness = pMtrl->m_oLightParam.m_fRoughness;
			pMtrl->m_oLightParam.m_fOldF0 = pMtrl->m_oLightParam.m_fF0;
			pMtrl->m_oLightParam.m_fOldThickness = pMtrl->m_oLightParam.m_fThickness;
		}

		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_PARAM_LIGHT, SGCore_LoadTexGetTex(pMtrl->m_oLightParam.m_idTexParamHand));
	}
	/*else
		mtrl_data::pDXDevice->SetTexture(MTL_TEX_R_PARAM_LIGHT, SGCore_LoadTexGetTex(mtrl_data::IDsTexs::NullMaterial));*/


	if (pMtrl->m_oMainGraphics.m_idShaderVS != -1)
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS);

	if (pMtrl->m_oMainGraphics.m_idShaderPS != -1)
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS);

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld || pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView || pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection)
		m_mWorldTrans = SMMatrixTranspose(m_mWorld);

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView || pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection)
	{
		Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mViewTrans);
		m_mViewTrans = SMMatrixTranspose(m_mViewTrans);

		Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mProjTrans);
		m_mProjTrans = SMMatrixTranspose(m_mProjTrans);
	}

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorld)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_mW", &m_mWorldTrans);

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorld)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_mW", &m_mWorldTrans);

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransView)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_mV", &m_mViewTrans);

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransView)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_mV", &m_mViewTrans);

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransProjection)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_mP", &m_mProjTrans);

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransProjection)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_mP", &m_mProjTrans);


	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView)
	{
		float4x4 wv = m_mViewTrans * m_mWorldTrans;

		if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldView)
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_mWV", &wv);

		if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldView)
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_mWV", &wv);
	}

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection)
	{
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mViewProj);
		
		float4x4 mWVP = m_mWorld * m_mViewProj;
		mWVP = SMMatrixTranspose(mWVP);

		if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWorldViewProjection)
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_mWVP", &mWVP);

		if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWorldViewProjection)
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_mWVP", &mWVP);
	}

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam || pMtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam)
	{
		float3 vObserverPos;
		Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

		if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransPosCam)
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_vPosCam", &vObserverPos);

		if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransPosCam)
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vPosCam", &vObserverPos);
	}


	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserData)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_vParam", &(pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData));

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransUserDataInOtherShader)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vParamVS", &(pMtrl->m_oMainGraphics.m_oDataVS.m_vUserData));

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserData)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vParam", &(pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData));

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransUserDataInOtherShader)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_vParamPS", &(pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData));

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransTimeDelta)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_vTimeDelta", &float2(m_uiCountTimeDelta, float(m_uiCurrTimeDelta) * 0.001f));

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransTimeDelta)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vTimeDelta", &float2(m_uiCountTimeDelta, float(m_uiCurrTimeDelta) * 0.001f));

	if (pMtrl->m_oMainGraphics.m_oDataVS.m_isTransWinSize)
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pMtrl->m_oMainGraphics.m_idShaderVS, "g_vWinSize", &float4_t(*r_win_width, *r_win_height, 1.f / (*r_win_width), 1.f / (*r_win_height)));

	if (pMtrl->m_oMainGraphics.m_oDataPS.m_isTransWinSize)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vWinSize", &float4_t(*r_win_width, *r_win_height, 1.f / (*r_win_width), 1.f / (*r_win_height)));

	if (pMtrl->m_oMainGraphics.m_useDestColor && pMtrl->m_oMainGraphics.m_idShaderPS >= 0 && pColor)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vDestColor", (void*)pColor);

	//если материалом назначен альфа тест и не включен принудительный
	if (pMtrl->m_oMainGraphics.m_useAlphaTest && !m_useForceblyAlphaTest)
	{
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHAREF, MTL_ALPHATEST_FREE_VALUE);
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	//если не включен принудительный альфа тест
	else if (!m_useForceblyAlphaTest)
	{
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	//иначе включен принудительный альфа тест
	else
	{
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHAREF, MTL_ALPHATEST_FORCEBLY_VALUE);
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	/*if (pMtrl->m_oLightParam.m_fThickness < 1.f)
	{
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		mtrl_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		mtrl_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		mtrl_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		mtrl_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
		mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/

	//почти во всех пиксельных шейдерах материалов есть данна¤ NearFar, необходима¤ д¤л записи глубины
	if (pMtrl->m_oMainGraphics.m_idShaderPS != -1)
	{	
		//освещаемый ли тип материала или нет? Ппрозрачный освещаемый?
		//0,0.33,0.66,1
		float fLayer;
		if (pMtrl->m_oMainGraphics.m_isUnlit)
		{
			if (!(pMtrl->m_oLightParam.m_isTransparent))
				fLayer = MTLTYPE_UNLIT;
			else
				fLayer = MTLTYPE_UNLIT;
		}
		else
		{
			if (!(pMtrl->m_oLightParam.m_isTransparent))
				fLayer = MTLTYPE_LIGHT;
			else
				fLayer = MTLTYPE_LIGHT;
		}

		if (m_useCountSurface && pMtrl->m_oLightParam.m_isTransparent)
		{
			if (m_idCurrIdSurface == 1)
				m_idCurrIdSurface += 2;
			else
				++(m_idCurrIdSurface);
		}

		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pMtrl->m_oMainGraphics.m_idShaderPS, "g_vNearFarLayers", &float4_t(*r_near, *r_far, fLayer, float(m_idCurrIdSurface)));
	}
}

void CMaterials::renderLight(const float4_t *pColor, const float4x4 *pWorld)
{
	MTL_PRE_COND_ID(m_idMtrlDefLight, _VOID);

	m_aUnitMtrls[m_idMtrlDefLight]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData = *pColor;
	render(m_idMtrlDefLight, pWorld);
	m_aUnitMtrls[m_idMtrlDefLight]->m_pMtrl->m_oMainGraphics.m_oDataPS.m_vUserData = float4(0, 0, 0, 0);
}