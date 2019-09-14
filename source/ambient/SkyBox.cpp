
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "SkyBox.h"


void CSkyBox::setDevice(IGXDevice *pDevice)
{
	m_pDevice = pDevice;

	if(m_pDevice)
	{
		GXVertexElement layoutskybox[] =
		{
			{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
			{0, 12, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD},
			GX_DECL_END()
		};

		m_pVertexDeclarationSkyBox = m_pDevice->createVertexDeclaration(layoutskybox);

		//! @todo fix that! r_far cvar
		float fFar = 800.0f * 0.57735f;


		float X = fFar;
		float Y = fFar;
		float Z = fFar;

		CSkyBoxVertex tmpVertices[] = {
			CSkyBoxVertex(X, Y, Z, 1.0f, 1.0f, 1.0f),
			CSkyBoxVertex(-X, Y, Z, -1.0f, 1.0f, 1.0f),
			CSkyBoxVertex(X, -Y, Z, 1.0f, -1.0f, 1.0f),

			CSkyBoxVertex(X, Y, -Z, 1.0f, 1.0f, -1.0f),
			CSkyBoxVertex(-X, -Y, Z, -1.0f, -1.0f, 1.0f),
			CSkyBoxVertex(X, -Y, -Z, 1.0f, -1.0f, -1.0f),

			CSkyBoxVertex(-X, Y, -Z, -1.0f, 1.0f, -1.0f),
			CSkyBoxVertex(-X, -Y, -Z, -1.0f, -1.0f, -1.0f)
		};

		IGXVertexBuffer *pVertices = m_pDevice->createVertexBuffer(8 * sizeof(CSkyBoxVertex), GXBUFFER_USAGE_STATIC, tmpVertices);
		m_pRenderBuffer = m_pDevice->createRenderBuffer(1, &pVertices, m_pVertexDeclarationSkyBox);
		mem_release(pVertices);

		WORD indices_tmp[] =
		{
			4, 1, 0,
			2, 4, 0,

			7, 5, 3,
			6, 7, 3,

			2, 0, 3,
			5, 2, 3,

			7, 6, 1,
			4, 7, 1,

			6, 3, 0,
			1, 6, 0,

			7, 4, 2,
			5, 7, 2,
		};
		m_pIndeces = m_pDevice->createIndexBuffer(36 * sizeof(WORD), GXBUFFER_USAGE_STATIC, GXIT_UINT16, indices_tmp);
	}
	else
	{
		mem_release(m_pSky1);
		mem_release(m_pSky2);
		mem_release(m_pIndeces);
		mem_release(m_pRenderBuffer);

		mem_release(m_pVertexDeclarationSkyBox);
	}
}
void CSkyBox::setMaterialSystem(IXMaterialSystem *pMaterialSystem)
{
	m_pMaterialSystem = pMaterialSystem;
}


void CSkyBox::setTexture(const char *szTexture)
{
	assert(m_pMaterialSystem);
	assert(szTexture);
	
	mem_release(m_pSky1);
	XSHADER_DEFAULT_DESC shDesc;
	shDesc.szFilePS = "sky_box.ps";
	shDesc.szFileVS = "sky_box.vs";
	m_pMaterialSystem->loadMaterial(szTexture, &m_pSky1, &shDesc);

	if(!m_pSky1)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, szTexture);
	}
}

bool CSkyBox::isEnabled()
{
	return m_isEnabled;
}
void CSkyBox::enable(bool yesNo)
{
	m_isEnabled = yesNo;
}

void CSkyBox::changeTexture(const char *texture)
{
#if 0
	ID idNew = -1;
	if(m_isChanging)
	{
		m_idTex1 = SGCore_LoadTexAddName(texture, LOAD_TEXTURE_TYPE_CONST);
		idNew = m_idTex1;
	}
	else
	{
		m_idTex2 = SGCore_LoadTexAddName(texture, LOAD_TEXTURE_TYPE_CONST);
		idNew = m_idTex2;
	}

	if(ID_VALID(idNew))
	{
		m_isChanging = true;
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, texture);
	}
#endif
}

void CSkyBox::setRotation(float angle)
{
	m_fRotaionY = angle;
	m_mMatRotation = SMMatrixRotationY(m_fRotaionY);
}

float CSkyBox::getRotation()
{
	return m_fRotaionY;
}

void CSkyBox::setColor(const float4 &vColor)
{
	m_vColor = vColor;
}

const float4_t& CSkyBox::getColor()
{
	return(m_vColor);
}

void CSkyBox::render()
{
	if(!m_isEnabled || !m_pSky1)
	{
		return;
	}

	float4x4 World = SMMatrixTranspose(m_mMatRotation/* * SMMatrixTranslation(pos->x, pos->y, pos->z)*/);

	m_pMaterialSystem->bindMaterial(m_pSky1);
#if 0
	if (/*m_isChangingMainTex*/m_isChanging)
	{
		m_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex2));
		m_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex1), 1);
	}
	else
	{
		m_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex1));
		m_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex2), 1);
	}
#endif

	//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "g_mWVP", &WVP);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_vColor", &m_vColor);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_fBlendFactor", &m_fBlendFactor); 
	
	//static ID s_idShader = SGCore_ShaderCreateKit(m_idVS, m_idPS);
	//SGCore_ShaderBind(s_idShader);

	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setIndexBuffer(m_pIndeces);
	pCtx->setRenderBuffer(m_pRenderBuffer);
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);
	pCtx->drawIndexed(8, 12, 0, 0);

	//SGCore_ShaderUnBind();
};

void CSkyBox::update(float fDT)
{
	if(m_isChanging)
	{
		m_fBlendFactor += fDT * 0.001f * 0.2f;

		if(m_fBlendFactor >= 1.0f)
		{
			m_fBlendFactor = 0.0;
			m_isChanging = false;
		}
	}
}

#if 0
//##########################################################################

CSkyClouds::CSkyClouds()
{
	GXVertexElement layoutclouds[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		GX_DECL_END()
	};

	m_pVertexDeclarationClouds = g_pDevice->createVertexDeclaration(layoutclouds);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sky_clouds.vs");
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_clouds.ps");

	GXMacro Defines_SHADOW[] = { { "SHADOW", "" }, { 0, 0 } };
	m_idPS_Shadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_clouds.ps", "sky_clouds_shadow.ps", Defines_SHADOW);

	m_fRotaionY = 0;
	m_fAlpha = 1.f;
	m_vColor = float4_t(0, 0, 0, 0);
	m_mMatRotation = SMMatrixIdentity();
	//@FIXME: Consider to make it static
	m_pVertices = g_pDevice->createVertexBuffer(4 * sizeof(CSkyCloudsVertex), GXBUFFER_USAGE_DYNAMIC);
	
	float X = 800;
	float Y = 0;
	float Z = 800;

	CSkyCloudsVertex *pVertices;
	if(m_pVertices->lock((void**)&pVertices, GXBL_WRITE))
	{
		pVertices[3] = CSkyCloudsVertex(-X, Y, -Z, 0.0f, 2.0f);
		pVertices[2] = CSkyCloudsVertex(-X, Y, Z, 0.0f, 0.0f);
		pVertices[1] = CSkyCloudsVertex(X, Y, Z, 2.0f, 0.0f);
		pVertices[0] = CSkyCloudsVertex(X, Y, -Z, 2.0f, 2.0f);

		m_pVertices->unlock();
	}

	WORD indices_tmp[] =
    {
        0,1,2,
        0,2,3
    };
	m_pIndeces = g_pDevice->createIndexBuffer(6 * sizeof(WORD), GXBUFFER_USAGE_STATIC, GXIT_UINT16, indices_tmp);

	m_pRenderBuffer = g_pDevice->createRenderBuffer(1, &m_pVertices, m_pVertexDeclarationClouds);
	
	m_fBias = 0.f;
	m_fSpeed = 0.01f;

	m_idTex1 = -1;
	m_idTex1 = -1;

	m_isChanging = false;
	//m_isChangingMainTex = false;
	m_fBlendFactor = 0.0f;

	m_isUse = true;
}

CSkyClouds::~CSkyClouds()
{
	mem_release(m_pVertices);
	mem_release(m_pIndeces);
	mem_release(m_pRenderBuffer);

	mem_release(m_pVertexDeclarationClouds);
}

bool CSkyClouds::getUse()
{
	return m_isUse;
}

void CSkyClouds::setUse(bool isUse)
{
	m_isUse = isUse;
}

void CSkyClouds::changeTexture(const char *texture)
{
	ID idNew;
	if (/*m_isChangingMainTex*/m_isChanging)
	{
		m_idTex1 = SGCore_LoadTexAddName(texture, LOAD_TEXTURE_TYPE_CONST);
		idNew = m_idTex1;
	}
	else
	{
		m_idTex2 = SGCore_LoadTexAddName(texture, LOAD_TEXTURE_TYPE_CONST);
		idNew = m_idTex2;
	}

	if (idNew != -1)
		m_isChanging = true;
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, texture);
}

void CSkyClouds::setWidthHeightPos(float width,float height, const float3* pos)
{
	CSkyCloudsVertex *pVertices;
	if(m_pVertices->lock((void**)&pVertices, GXBL_WRITE))
	{
		float X = pos->x + (width*0.5f);
		float Y = pos->y;
		float Z = pos->z + (height*0.5f);

		m_vWidthHeight.x = width;
		m_vWidthHeight.y = height;

		//float2_t tmpwh = float2_t(width*0.5f,height*0.5f);

		pVertices[3] = CSkyCloudsVertex(-X, Y, -Z, 0.0f, 2.0f);
		pVertices[2] = CSkyCloudsVertex(-X, Y, Z, 0.0f, 0.0f);
		pVertices[1] = CSkyCloudsVertex(X, Y, Z, 2.0f, 0.0f);
		pVertices[0] = CSkyCloudsVertex(X, Y, -Z, 2.0f, 2.0f);

		m_pVertices->unlock();
	}
}

void CSkyClouds::setRotation(float angle)
{
	m_fRotaionY = angle;
	m_mMatRotation = SMMatrixRotationY(m_fRotaionY);
}

float CSkyClouds::getRotation()
{
	return m_fRotaionY;
}

void CSkyClouds::setAlpha(float alpha)
{
	m_fAlpha = alpha;
}

float CSkyClouds::getAlpha()
{
	return m_fAlpha;
}

void CSkyClouds::setColor(const float4_t* color)
{
	m_vColor = *color;
}

void CSkyClouds::getColor(float4_t* color)
{
	*color = m_vColor;
}

void CSkyClouds::setSpeed(float speed)
{
	m_fSpeed = speed;
}

float CSkyClouds::getSpeed()
{
	return m_fSpeed;
}

void CSkyClouds::loadTexture(const char *texture)
{
	m_idTex1 = 0;
	if (!texture)
	{
		m_idTex2 = 0;

		m_isChanging = false;
		//m_isChangingMainTex = false;
		m_fBlendFactor = 0.0f;

		return;
	}

	m_idTex1 = SGCore_LoadTexAddName(texture, LOAD_TEXTURE_TYPE_CONST);

	if (m_idTex1 == -1)
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, texture);
}

bool CSkyClouds::isLoadTex()
{
	return (m_idTex1 != -1 || m_idTex2 != -1);
}

void CSkyClouds::render(DWORD timeDelta, const float3* pos,bool is_shadow)
{
	if (m_isChanging)
		m_fBlendFactor += timeDelta * 0.001f * 0.2f;

	m_fBias += timeDelta * 0.001f * m_fSpeed;

	if (m_fBias >= 2.f)
		m_fBias = 0.f;

	float4x4 World = SMMatrixTranslation(0, pos->y, 0);

	if (m_isChanging && m_fBlendFactor >= 1.0)
	{
		m_fBlendFactor = 0.0;
		//m_isChangingMainTex = !m_isChangingMainTex;
		m_isChanging = false;
	}

	if (/*m_isChangingMainTex*/m_isChanging)
	{
		g_pDevice->setTexture(SGCore_LoadTexGetTex(m_idTex2));
		g_pDevice->setTexture(SGCore_LoadTexGetTex(m_idTex1), 1);
	}
	else
	{
		g_pDevice->setTexture(SGCore_LoadTexGetTex(m_idTex1));
		g_pDevice->setTexture(SGCore_LoadTexGetTex(m_idTex2), 1);
	}

	if(!is_shadow)
	{
		float4x4 View;
		float4x4 Proj;

		Core_RMatrixGet(G_RI_MATRIX_VIEW, &View);
		Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &Proj);

		float4x4 WVP = (m_mMatRotation * World) * View * Proj;

		WVP = SMMatrixTranspose(WVP);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "g_mWVP", &WVP);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_vBlendFactorBias", &float2(m_fBlendFactor, m_fBias));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_vColor", &m_vColor);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_fAlpha", &m_fAlpha);

		static ID s_idShader = SGCore_ShaderCreateKit(m_idVS, m_idPS);

		SGCore_ShaderBind(s_idShader);
	}
	else
	{
		float4x4 ViewProj;
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &ViewProj);
		float4x4 WVP = (m_mMatRotation * World) * ViewProj;

		WVP = SMMatrixTranspose(WVP);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "g_mWVP", &WVP);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS_Shadow, "g_vBlendFactorBias", &float2(m_fBlendFactor, m_fBias));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS_Shadow, "g_fAlpha", &m_fAlpha);

		static ID s_idShader = SGCore_ShaderCreateKit(m_idVS, m_idPS_Shadow);
		SGCore_ShaderBind(s_idShader);
	}
	
	g_pDevice->setIndexBuffer(m_pIndeces);
	g_pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);
	g_pDevice->setRenderBuffer(m_pRenderBuffer);

	g_pDevice->drawIndexed(4, 2, 0, 0);

	SGCore_ShaderUnBind();
}

#endif
