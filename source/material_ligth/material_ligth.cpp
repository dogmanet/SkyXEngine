
#define SXMATERIAL_LIGTH_VERSION 1

#include <material_ligth\material_ligth.h>
#include <material_ligth\\ml_data.cpp>
#include <material_ligth\\light.cpp>
#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

Lights* ArrLights = 0;

#define ML_PRECOND(retval) if(!ArrLights){reportf(-1, "%s - sxmaterial_ligth is not init", gen_msg_location); return retval;}

long SML_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
}

void SML_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SML_0Create(const char* name, IDirect3DDevice9* device, float2_t* winsize, float projfov, DWORD id_depth, DWORD id_depth_d2, DWORD id_depth_d4, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxmaterial_ligth", gen_msg_location);
			}
			else
			{
				MLInit(device, winsize, projfov, id_depth, id_depth_d2, id_depth_d4);
				MLSet::ReCalcSize();
				ArrLights = new Lights();
			}
		}
		else
		{
			MLInit(device, winsize, projfov, id_depth, id_depth_d2, id_depth_d4);
			MLSet::ReCalcSize();
			ArrLights = new Lights();
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxmaterial_ligth", gen_msg_location);
}

void SML_OnLostDevice()
{
	ML_PRECOND();
	ArrLights->OnLostDevice();
}

void SML_OnResetDevice(int width, int heigth, float projfov)
{
	ML_PRECOND();
	MLSet::WinSize.x = width;
	MLSet::WinSize.y = heigth;
	MLSet::ProjFov = projfov;
	MLSet::ProjRatio = MLSet::WinSize.x / MLSet::WinSize.y;
	MLSet::ReCalcSize();
	ArrLights->OnResetDevice();
}

void SML_LigthsSettLCoefSizeDepth(float coef)
{
	ML_PRECOND();
}

void SML_LigthsSettLCoefSizeShadow(float coef)
{
	ML_PRECOND();
}

void SML_LigthsSettGCoefSizeDepth(float coef)
{
	ML_PRECOND();
}

void SML_LigthsSettGCoefSizeShadow(float coef)
{
	ML_PRECOND();
}

void SML_LigthsUpdate(float3* const_cam_pos, float4x4* camview)
{
	ML_PRECOND();

	MLSet::ConstCurrCamPos = *const_cam_pos;
	MLSet::MCamView = *camview;
}


long SML_LigthsGetCount()
{
	ML_PRECOND(-1);

	return ArrLights->GetCountLights();
}

long SML_LigthsCreatePoint(float4* center, float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(center, color, isglobal, is_shadowed, 0);
}

long SML_LigthsCreateDirection(float4* pos, float3* color, float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(pos, color, dir, top_radius, angle, is_shadow, 0);
}


void SML_LigthsRender(long id, DWORD timeDelta)
{
	ML_PRECOND();
	ArrLights->Render(id, timeDelta);
}

void SML_LigthsGetColor(long id, float3* color)
{
	ML_PRECOND();
	ArrLights->GetLightColor(id, color);
}

void SML_LigthsSetColor(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightColor(id, vec);
}

void SML_LigthsGetPosW(long id, float4* pos)
{
	ML_PRECOND();
	ArrLights->GetLightPosW(id, pos);
}

void SML_LigthsGetPos(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->GetLightPos(id, vec);
}

void SML_LigthsSetPos(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightPos(id, vec);
}

void SML_LigthsGetRot(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->GetLightRot(id, vec);
}

void SML_LigthsSetRot(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightRot(id, vec);
}

void SML_LigthsGetDir(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->GetLightDir(id, vec);
}

void SML_LigthsSetDir(long id, float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightDir(id, vec);
}

int SML_LigthsGetType(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetType(id);
}

int SML_LigthsComVisibleForFrustum(long id, ISXFrustum* frustum)
{
	ML_PRECOND(-1);
	return ArrLights->ComVisibleForFrustum(id, frustum);
}

bool SML_LigthsGetVisibleForFrustum(long id, ISXFrustum* frustum)
{
	ML_PRECOND(false);
	return ArrLights->GetVisibleForFrustum(id, frustum);
}

float SML_LigthsComDistFor(long id, float3* vec)
{
	ML_PRECOND(-1);
	return ArrLights->ComDistFor(id, vec);
}

float SML_LigthsGetDistFor(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetDistFor(id);
}

void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec)
{
	ML_PRECOND();
	return ArrLights->ComVisibleFrustumDistFor(frustum, vec);
}

int SML_LigthsIsEnable(long id)
{
	ML_PRECOND(-1);
	return ArrLights->IsEnable(id);
}

void SML_LigthsSetEnable(long id, bool val)
{
	ML_PRECOND();
	return ArrLights->SetEnable(id, val);
}

int SML_LigthsIsShadow(long id)
{
	ML_PRECOND(-1);
	return ArrLights->IsShadow(id);
}

IDirect3DTexture9* SML_LigthsGetShadow(long id)
{
	ML_PRECOND(0);
	return ArrLights->GetShadow(id);
}

void SML_LigthsInRenderBegin(long id)
{
	ML_PRECOND();
	ArrLights->InRenderBegin(id);
}

void SML_LigthsInRenderEnd(long id)
{
	ML_PRECOND();
	ArrLights->InRenderEnd(id);
}

void SML_LigthsInRenderPre(long id, int cube)
{
	ML_PRECOND();
	ArrLights->InRenderPre(id, cube);
}

void SML_LigthsInRenderPost(long id, int cube)
{
	ML_PRECOND();
	ArrLights->InRenderPost(id, cube);
}

ISXFrustum* SML_LigthsGetFrustum(long id)
{
	ML_PRECOND(0);
	return ArrLights->GetFrustum(id);
}

ISXFrustum* SML_LigthsGetFrustumG(long id, int split)
{
	ML_PRECOND(0);
	return ArrLights->GetFrustumG(id, split);
}

void SML_LigthsUpdateG(long id, int split, float3* pos, float3* dir)
{
	ML_PRECOND();
	return ArrLights->UpdateG(id, split,pos,dir);
}

void SML_LigthsGenShadow(long id)
{
	ML_PRECOND();
	ArrLights->GenShadow(id);
}

void SML_LigthsSoftShadow(long id, bool randomsam, float size)
{
	ML_PRECOND();
	ArrLights->SoftShadow(id, randomsam, size);
}



long SML_LigthsCreateCopy(long id)
{
	ML_PRECOND(-1);
	return ArrLights->CreateCopy(id);
}

void SML_LigthsDeleteLight(long id)
{
	ML_PRECOND();
	ArrLights->DeleteLight(id);
}

char* SML_LigthsGetName(long id)
{
	ML_PRECOND(0);
	return ArrLights->GetLightName(id);
}

void SML_LigthsSetName(long id, const char* name)
{
	ML_PRECOND();
	ArrLights->SetLightName(id, name);
}


void SML_LigthsChangeAngle(long id, float angle, bool is_create)
{
	ML_PRECOND();
	ArrLights->ChangeAngle(id, angle, is_create);
}

void SML_LigthsChangeTopRadius(long id, float top_radius)
{
	ML_PRECOND();
	ArrLights->ChangeTopRadius(id, top_radius);
}

void SML_LigthsChangeRadiusHeight(long id, float radius_height, bool is_create)
{
	ML_PRECOND();
	ArrLights->ChangeRadiusHeight(id, radius_height, is_create);
}

void SML_LigthsChangeShadow(long id, bool is_shadow)
{
	ML_PRECOND();
	ArrLights->ChangeShadow(id, is_shadow);
}


void SML_LigthsSetBlurPixel(long id, float blur_pixel)
{
	ML_PRECOND();
	ArrLights->SetBlurPixel(id, blur_pixel);
}
float SML_LigthsGetBlurPixel(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetBlurPixel(id);
}
void SML_LigthsSetShadowLocalNear(long id, float slnear)
{
	ML_PRECOND();
	ArrLights->SetShadowLocalNear(id, slnear);
}
float SML_LigthsGetShadowLocalNear(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalNear(id);
}
void SML_LigthsSetShadowLocalFar(long id, float slfar)
{
	ML_PRECOND();
	ArrLights->SetShadowLocalFar(id, slfar);
}
float SML_LigthsGetShadowLocalFar(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalFar(id);
}
void SML_LigthsSetEnableCubeEdge(long id, int edge, bool enable)
{
	ML_PRECOND();
	ArrLights->SetEnableCubeEdge(id, edge, enable);
}
bool SML_LigthsGetEnableCubeEdge(long id, int edge)
{
	ML_PRECOND(false);
	return ArrLights->GetEnableCubeEdge(id, edge);
}