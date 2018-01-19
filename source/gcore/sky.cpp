
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sky.h"

CSkyBox::CSkyBox()
{
	m_szTexActive[0] = 0;
	m_szTexSecond[0] = 0;

	D3DVERTEXELEMENT9 layoutskybox[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	g_pDXDevice->CreateVertexDeclaration(layoutskybox, &m_pVertexDeclarationSkyBox);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sky_box.vs", "sky_box.vs", SHADER_CHECKDOUBLE_NAME);
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_box.ps", "sky_box.ps", SHADER_CHECKDOUBLE_NAME);

	m_vColor = float4(0, 0, 0, 0);
	m_fRotaionY = 0.f;
	m_mMatRotation = SMMatrixIdentity();
	g_pDXDevice->CreateVertexBuffer(
								8 * sizeof(CSkyBoxVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&m_pVertices,
								0
								);

	g_pDXDevice->CreateIndexBuffer(
               36 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
			   &m_pIndeces,
               0);

	float X = 1.0f * 200;
	float Y = 1.0f * 200;
	float Z = 1.0f * 200;
	float tmpy = 45;

	CSkyBoxVertex* tmpVertices;
	m_pVertices->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = CSkyBoxVertex( X,  Y-tmpy, Z, 1.0f, 1.0f, 1.0f);
	tmpVertices[1] = CSkyBoxVertex(-X,  Y-tmpy, Z,-1.0f, 1.0f, 1.0f);
	tmpVertices[2] = CSkyBoxVertex( X, -tmpy, Z, 1.0f,-1.0f, 1.0f);

	tmpVertices[3] = CSkyBoxVertex( X,  Y-tmpy,-Z, 1.0f, 1.0f,-1.0f);
	tmpVertices[4] = CSkyBoxVertex(-X, -tmpy, Z,-1.0f,-1.0f, 1.0f);
	tmpVertices[5] = CSkyBoxVertex( X, -tmpy,-Z, 1.0f,-1.0f,-1.0f);

	tmpVertices[6] = CSkyBoxVertex(-X,  Y-tmpy,-Z,-1.0f, 1.0f,-1.0f);
	tmpVertices[7] = CSkyBoxVertex(-X, -tmpy,-Z,-1.0f,-1.0f,-1.0f);

	m_pVertices->Unlock();


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

	WORD* indices = 0;
	m_pIndeces->Lock(0, 0, (void**)&indices, 0);

	memcpy(indices,indices_tmp,36 * sizeof(WORD));

	m_pIndeces->Unlock();

	m_pTexture = 0;
	m_pTexture2 = 0;

	m_isChange = false;
	m_isChangeMainTex = false;
	m_fFactorBlend = 0.0f;
};

CSkyBox::~CSkyBox()
{
	mem_release_del(m_pVertices);
	mem_release_del(m_pIndeces);

	mem_release_del(m_pTexture);
	mem_release_del(m_pTexture2);

	mem_release_del(m_pVertexDeclarationSkyBox);
}

void CSkyBox::loadTextures(const char *texture)
{
	mem_release_del(m_pTexture);
	if (!texture)
	{
		mem_release_del(m_pTexture2);

		m_isChange = false;
		m_isChangeMainTex = false;
		m_fFactorBlend = 0.0f;

		return;
	}

	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), texture);
	if (FAILED(D3DXCreateCubeTextureFromFile(g_pDXDevice, tmppath, &m_pTexture)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load cube texture '%s'", GEN_MSG_LOCATION, tmppath);
	}
}

bool CSkyBox::isLoadTex()
{
	return (m_pTexture != 0 || m_pTexture2 != 0);
}

void CSkyBox::changeTexture(const char *texture)
{
	mem_release_del((m_isChangeMainTex ? m_pTexture : m_pTexture2));
	char tmpsb1[1024];
	sprintf(tmpsb1, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), texture);
	if (!FAILED(D3DXCreateCubeTextureFromFile(g_pDXDevice, tmpsb1, (m_isChangeMainTex ? &m_pTexture : &m_pTexture2))))
			m_isChange = true;
		else
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load cube texture '%s'", GEN_MSG_LOCATION, tmpsb1);
		}
}

void CSkyBox::getActiveTexture(char *texture)
{
	if (texture)
		strcpy(texture, m_szTexActive);
}

void CSkyBox::getSecondTexture(char *texture)
{
	if (texture)
		strcpy(texture, m_szTexSecond);
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

	//D3DXMATRIX tmpdxView, tmpdxProjection;
	//g_pDXDevice->GetTransform(D3DTS_PROJECTION, &tmpdxProjection);
	//g_pDXDevice->GetTransform(D3DTS_VIEW, &tmpdxView);
	float4x4 View;// = float4x4(tmpdxView);
	float4x4 Proj;// = float4x4(tmpdxProjection);

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &View);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &Proj);

	float4x4 WVP = World * View * Proj;

	WVP = SMMatrixTranspose(WVP);

	if (m_isChange && m_fFactorBlend >= 1.0)
	{
		m_fFactorBlend = 0.0;
		m_isChangeMainTex = !m_isChangeMainTex;
		m_isChange = false;
	}

	if (m_isChangeMainTex)
	{
		g_pDXDevice->SetTexture(0, m_pTexture2);
		g_pDXDevice->SetTexture(1, m_pTexture);
	}
	else
	{
		g_pDXDevice->SetTexture(0, m_pTexture);
		g_pDXDevice->SetTexture(1, m_pTexture2);
	}

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "WorldViewProjection", &WVP);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "Color", &m_vColor);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "BlendFactor", &m_fFactorBlend);
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_idVS);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_idPS);

	g_pDXDevice->SetStreamSource(0, m_pVertices, 0, sizeof(CSkyBoxVertex));
	g_pDXDevice->SetIndices(m_pIndeces);
	g_pDXDevice->SetVertexDeclaration(m_pVertexDeclarationSkyBox);
	g_pDXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	SGCore_ShaderUnBind();
};

//##########################################################################

CSkyClouds::CSkyClouds()
{
	D3DVERTEXELEMENT9 layoutclouds[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	g_pDXDevice->CreateVertexDeclaration(layoutclouds, &m_pVertexDeclarationClouds);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sky_clouds.vs", "sky_clouds.vs", SHADER_CHECKDOUBLE_NAME);
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_clouds.ps", "sky_clouds.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_SHADOW[] = { { "SHADOW", "" }, { 0, 0 } };
	m_idPS_Shadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sky_clouds.ps", "sky_clouds_shadow.ps", SHADER_CHECKDOUBLE_NAME, Defines_SHADOW);

	m_fRotaionY = 0;
	m_fAlpha = 1.f;
	m_vColor = float4_t(0, 0, 0, 0);
	m_mMatRotation = SMMatrixIdentity();
	g_pDXDevice->CreateVertexBuffer(
								4 * sizeof(CSkyCloudsVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&m_pSkyCloudsVertices,
								0
								);

	g_pDXDevice->CreateIndexBuffer(
               6 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
			   &m_pSkyCloudsIndeces,
               0);

	float X = 800;
	float Y = 0;
	float Z = 800;

	m_pSkyCloudsVertices->Lock(0, 0, (void**)&m_pVertices, 0);

	m_pVertices[3] = CSkyCloudsVertex(-X, Y, -Z, 0.0f, 2.0f);
	m_pVertices[2] = CSkyCloudsVertex(-X, Y, Z, 0.0f, 0.0f);
	m_pVertices[1] = CSkyCloudsVertex(X, Y, Z, 2.0f, 0.0f);
	m_pVertices[0] = CSkyCloudsVertex(X, Y, -Z, 2.0f, 2.0f);

	m_pSkyCloudsVertices->Unlock();


	WORD indices_tmp[] =
    {
        0,1,2,
        0,2,3
    };


	WORD* indices = 0;
	m_pSkyCloudsIndeces->Lock(0, 0, (void**)&indices, 0);

	memcpy(indices,indices_tmp,6 * sizeof(WORD));

	m_pSkyCloudsIndeces->Unlock();

	m_fBias = 0.f;
	m_fSpeed = 0.01f;

	m_pSkyCloudsTex = 0;
	m_pSkyCloudsTex2 = 0;

	m_isChange = false;
	m_isChangeMainTex = false;
	m_fFactorBlend = 0.0f;
}

CSkyClouds::~CSkyClouds()
{
	mem_release_del(m_pSkyCloudsTex);
	mem_release_del(m_pSkyCloudsTex2);

	mem_release_del(m_pSkyCloudsVertices);
	mem_release_del(m_pSkyCloudsIndeces);

	mem_release_del(m_pVertexDeclarationClouds);
}

void CSkyClouds::changeTexture(const char *texture)
{
	mem_release_del((m_isChangeMainTex ? m_pSkyCloudsTex : m_pSkyCloudsTex2));
	char tmpsb1[1024];
	sprintf(tmpsb1, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), texture);
	if (!FAILED(D3DXCreateTextureFromFile(g_pDXDevice, tmpsb1, (m_isChangeMainTex ? &m_pSkyCloudsTex : &m_pSkyCloudsTex2))))
			m_isChange = true;
		else
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, tmpsb1);
		}
}

void CSkyClouds::setWidthHeightPos(float width,float height, const float3* pos)
{
	m_pSkyCloudsVertices->Lock(0, 0, (void**)&m_pVertices, 0);

	float X = pos->x + (width*0.5f);
	float Y = pos->y;
	float Z = pos->z + (height*0.5f);

	m_vWidthHeight.x = width;
	m_vWidthHeight.y = height;

	//float2_t tmpwh = float2_t(width*0.5f,height*0.5f);

	m_pVertices[3] = CSkyCloudsVertex(-X, Y, -Z, 0.0f, 2.0f);
	m_pVertices[2] = CSkyCloudsVertex(-X, Y, Z, 0.0f, 0.0f);
	m_pVertices[1] = CSkyCloudsVertex(X, Y, Z, 2.0f, 0.0f);
	m_pVertices[0] = CSkyCloudsVertex(X, Y, -Z, 2.0f, 2.0f);

	m_pSkyCloudsVertices->Unlock();
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

void CSkyClouds::loadTextures(const char *texture)
{
	mem_release_del(m_pSkyCloudsTex);
	if (!texture)
	{
		mem_release_del(m_pSkyCloudsTex2);

		m_isChange = false;
		m_isChangeMainTex = false;
		m_fFactorBlend = 0.0f;

		return;
	}

	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), texture);
	if (FAILED(D3DXCreateTextureFromFile(g_pDXDevice, tmppath, &m_pSkyCloudsTex)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed load texture '%s'", GEN_MSG_LOCATION, tmppath);
	}
}

bool CSkyClouds::isLoadTex()
{
	return (m_pSkyCloudsTex != 0 || m_pSkyCloudsTex2 != 0);
}

void CSkyClouds::render(DWORD timeDelta, const float3* pos,bool is_shadow)
{
	if (m_isChange)
		m_fFactorBlend += timeDelta * 0.001f * 0.2f;

	m_fBias += timeDelta * 0.001f * m_fSpeed;

	if (m_fBias >= 2.f)
		m_fBias = 0.f;

	//float4x4 World = SMMatrixTranslation(Core::Data::ConstCurrentCameraPosition.x, Core::Data::ConstCurrentCameraPosition.y+50, Core::Data::ConstCurrentCameraPosition.z);
	//float4x4 World = SMMatrixTranslation(pos->x, pos->y+50, pos->z);
	float4x4 World = SMMatrixTranslation(0, pos->y, 0);

	if (m_isChange && m_fFactorBlend >= 1.0)
		{
		m_fFactorBlend = 0.0;
			m_isChangeMainTex = !m_isChangeMainTex;
			m_isChange = false;
		}

	if (m_isChangeMainTex)
	{
		g_pDXDevice->SetTexture(0, m_pSkyCloudsTex2);
		g_pDXDevice->SetTexture(1, m_pSkyCloudsTex);
	}
	else
	{
		g_pDXDevice->SetTexture(0, m_pSkyCloudsTex);
		g_pDXDevice->SetTexture(1, m_pSkyCloudsTex2);
	}

	if(!is_shadow)
	{
		float4x4 View;// = float4x4(tmpdxView);
		float4x4 Proj;// = float4x4(tmpdxProjection);

		Core_RMatrixGet(G_RI_MATRIX_VIEW, &View);
		Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &Proj);

		float4x4 WVP = (m_mMatRotation * World) * View * Proj;

		WVP = SMMatrixTranspose(WVP);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "WorldViewProjection", &WVP);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "BlendFactorBias", &float2(m_fFactorBlend, m_fBias));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "Color", &m_vColor);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS, "Alpha", &m_fAlpha);
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_idVS);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_idPS);
	}
	else
	{
		float4x4 ViewProj;// = float4x4(tmpdxViewProj);
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &ViewProj);
		float4x4 WVP = (m_mMatRotation * World) * ViewProj;

		WVP = SMMatrixTranspose(WVP);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "WorldViewProjection", &WVP);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS_Shadow, "BlendFactorBias", &float2(m_fFactorBlend, m_fBias));
		//SGCore_ShaderSetVRF(1, PS_RenderSkyCloudsShadow, "Color", &Color);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS_Shadow, "Alpha", &m_fAlpha);
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_idVS);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_idPS_Shadow);
	}
	
	g_pDXDevice->SetStreamSource(0, m_pSkyCloudsVertices, 0, sizeof(CSkyCloudsVertex));
	g_pDXDevice->SetIndices(m_pSkyCloudsIndeces);
	g_pDXDevice->SetVertexDeclaration(m_pVertexDeclarationClouds);

	g_pDXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	SGCore_ShaderUnBind();
}