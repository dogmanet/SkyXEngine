
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sky.h"

CSkyBox::CSkyBox()
{
	GXVERTEXELEMENT layoutskybox[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD},
		GXDECL_END()
	};

	m_pVertexDeclarationSkyBox = g_pDevice->createVertexDeclaration(layoutskybox);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sky_box.vs", "sky_box.vs", SHADER_CHECKDOUBLE_NAME);
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_box.ps", "sky_box.ps", SHADER_CHECKDOUBLE_NAME);

	m_vColor = float4(0, 0, 0, 0);
	m_fRotaionY = 0.f;
	m_mMatRotation = SMMatrixIdentity();

	
	float X = SXGC_SKYBOX_SIZE * 0.5;
	float Y = SXGC_SKYBOX_SIZE * 0.5;
	float Z = SXGC_SKYBOX_SIZE * 0.5;
	//float tmpy = 45;

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

	m_pVertices = g_pDevice->createVertexBuffer(8 * sizeof(CSkyBoxVertex), GX_BUFFER_USAGE_STATIC, tmpVertices);

	WORD indices_tmp[] =
    {
        4,1,0,
        2,4,0,

        7,5,3,
        6,7,3,

        2,0,3,
        5,2,3,

		7,6,1,
		4,7,1,

		6,3,0,
		1,6,0,

		7,4,2,
		5,7,2,
    };

	m_pIndeces = g_pDevice->createIndexBuffer(36 * sizeof(WORD), GX_BUFFER_USAGE_STATIC, GXIT_USHORT, indices_tmp);

	m_pRenderBuffer = g_pDevice->createRenderBuffer(1, &m_pVertices, m_pVertexDeclarationSkyBox);
	
	m_idTex1 = -1;
	m_idTex2 = -1;

	m_isChange = false;
	//m_isChangeMainTex = false;
	m_fFactorBlend = 0.0f;

	m_isUse = true;
};

CSkyBox::~CSkyBox()
{
	mem_release(m_pVertices);
	mem_release(m_pIndeces);
	mem_release(m_pRenderBuffer);

	mem_release(m_pVertexDeclarationSkyBox);
}

void CSkyBox::loadTexture(const char *texture)
{
	m_idTex1 = 0;
	if (!texture)
	{
		m_idTex2 = 0;

		m_isChange = false;
		//m_isChangeMainTex = false;
		m_fFactorBlend = 0.0f;

		return;
	}

	m_idTex1 = SGCore_LoadTexAddName(texture, LOAD_TEXTURE_TYPE_CONST);

	if (m_idTex1 == -1)
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, texture);
}

bool CSkyBox::isLoadTex()
{
	return (m_idTex1 != -1 || m_idTex2 != -1);
}

bool CSkyBox::getUse()
{
	return m_isUse;
}

void CSkyBox::setUse(bool isUse)
{
	m_isUse = isUse;
}

void CSkyBox::changeTexture(const char *texture)
{
	ID idNew = -1;
	if (/*m_isChangeMainTex*/m_isChange)
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
		m_isChange = true;
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, texture);
}

void CSkyBox::getActiveTexture(char *texture)
{
	if (texture)
		SGCore_LoadTexGetName((/*m_isChangeMainTex*/m_isChange ? m_idTex2 : m_idTex1), texture);
}

void CSkyBox::getSecondTexture(char *texture)
{
	if (texture)
		SGCore_LoadTexGetName((/*m_isChangeMainTex*/m_isChange ? m_idTex1 : m_idTex2), texture);
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

void CSkyBox::setColor(const float4_t* color)
{
	m_vColor = *color;
}

void CSkyBox::getColor(float4_t* color)
{
	*color = m_vColor;
}

void CSkyBox::render(float timeDelta, const float3* pos,bool is_shadow)
{
	if (m_isChange)
		m_fFactorBlend += timeDelta * 0.001f * 0.2f;

	float4x4 World = m_mMatRotation * SMMatrixTranslation(pos->x, pos->y, pos->z);

	float4x4 View;
	float4x4 Proj;

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &View);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &Proj);

	float4x4 WVP = World * View * Proj;

	WVP = SMMatrixTranspose(WVP);

	if (m_isChange && m_fFactorBlend >= 1.0)
	{
		m_fFactorBlend = 0.0;
		//m_isChangeMainTex = !m_isChangeMainTex;
		m_isChange = false;
	}

	if (/*m_isChangeMainTex*/m_isChange)
	{
		g_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex2));
		g_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex1), 1);
	}
	else
	{
		g_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex1));
		g_pDevice->setTexture(SGCore_LoadTexGetTexCube(m_idTex2), 1);
	}

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "g_mWVP", &WVP);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_vColor", &m_vColor);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_fBlendFactor", &m_fFactorBlend); 
	
	static ID s_idShader = SGCore_ShaderCreateKit(m_idVS, m_idPS);
	SGCore_ShaderBind(s_idShader);

	g_pDevice->setIndexBuffer(m_pIndeces);
	g_pDevice->setRenderBuffer(m_pRenderBuffer);
	g_pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);
	g_pDevice->drawIndexed(8, 12, 0, 0);

	SGCore_ShaderUnBind();
};

//##########################################################################

CSkyClouds::CSkyClouds()
{
	GXVERTEXELEMENT layoutclouds[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		GXDECL_END()
	};

	m_pVertexDeclarationClouds = g_pDevice->createVertexDeclaration(layoutclouds);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sky_clouds.vs", "sky_clouds.vs", SHADER_CHECKDOUBLE_NAME);
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_clouds.ps", "sky_clouds.ps", SHADER_CHECKDOUBLE_NAME);

	GXMACRO Defines_SHADOW[] = { { "SHADOW", "" }, { 0, 0 } };
	m_idPS_Shadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_clouds.ps", "sky_clouds_shadow.ps", SHADER_CHECKDOUBLE_NAME, Defines_SHADOW);

	m_fRotaionY = 0;
	m_fAlpha = 1.f;
	m_vColor = float4_t(0, 0, 0, 0);
	m_mMatRotation = SMMatrixIdentity();
	//@FIXME: Consider to make it static
	m_pVertices = g_pDevice->createVertexBuffer(4 * sizeof(CSkyCloudsVertex), GX_BUFFER_USAGE_DYNAMIC);
	
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
	m_pIndeces = g_pDevice->createIndexBuffer(6 * sizeof(WORD), GX_BUFFER_USAGE_STATIC, GXIT_USHORT, indices_tmp);

	m_pRenderBuffer = g_pDevice->createRenderBuffer(1, &m_pVertices, m_pVertexDeclarationClouds);
	
	m_fBias = 0.f;
	m_fSpeed = 0.01f;

	m_idTex1 = -1;
	m_idTex1 = -1;

	m_isChange = false;
	//m_isChangeMainTex = false;
	m_fFactorBlend = 0.0f;

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
	if (/*m_isChangeMainTex*/m_isChange)
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
		m_isChange = true;
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

		m_isChange = false;
		//m_isChangeMainTex = false;
		m_fFactorBlend = 0.0f;

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
	if (m_isChange)
		m_fFactorBlend += timeDelta * 0.001f * 0.2f;

	m_fBias += timeDelta * 0.001f * m_fSpeed;

	if (m_fBias >= 2.f)
		m_fBias = 0.f;

	float4x4 World = SMMatrixTranslation(0, pos->y, 0);

	if (m_isChange && m_fFactorBlend >= 1.0)
	{
		m_fFactorBlend = 0.0;
		//m_isChangeMainTex = !m_isChangeMainTex;
		m_isChange = false;
	}

	if (/*m_isChangeMainTex*/m_isChange)
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
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "g_vBlendFactorBias", &float2(m_fFactorBlend, m_fBias));
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
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS_Shadow, "g_vBlendFactorBias", &float2(m_fFactorBlend, m_fBias));
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
