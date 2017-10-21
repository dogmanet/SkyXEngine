
#ifndef __SHADOW_H
#define __SHADOW_H

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/Array.h>
#include <d3d9.h>
#include <gcore/sxgcore.h>
#include "sxmtllight.h"
#include "ml_data.h"

class PSSM
{
public:

	PSSM();
	~PSSM();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Init();

	void UpdateFrustums(int split, const float3* poscam, const float3* dircam);
	void PreRender(int spilt);
	void Begin();
	void End();

	void GenShadow2(IDirect3DTexture9* shadowmap);
	void GenShadowAll(IDirect3DTexture9* shadowmap);
	
	void SetPosition(float3* pos);
	void GetPosition(float3* pos);

	void SetBlurPixel(float blur_pixel);
	bool GetBlurPixel();

	void Set4Or3Splits(bool is4);
	bool Get4Or3Splits();

	void SetIDArr(ID id, int split, ID idarr);
	long GetCountIDArrs();
	ID GetIDArr(ID id, int split);

	ISXFrustum* Frustums[5];
	//long IDArr[5];

	int IsUpdate[5];
	float2 NearFar[5];
	IDirect3DTexture9*	DepthMaps[5];

protected:

	Array<ID*> IDArr;
	float3 Position;

	void Flickering(float4x4 *matLVP,float size_x,float size_y);

	float BlurPixel;
	
	float2 FovRatio;

	//генерация теней для 4 сплитов, иначе для 3
	bool Generating4Slits;
	
	IDirect3DSurface9*	DepthSurfaces[5];
	IDirect3DSurface9*	DepthStencilSurface;
	float4x4 Views[5];
	float4x4 Projs[5];
	float4x4 ViewProj[5];
	float4x4 OldView,OldProj,OldViewProj;
	float4x4 ScaleBiasMat;
	
	IDirect3DSurface9* OldDepthStencilSurface;
	IDirect3DSurface9* OldColorSurface;
};

////////////////////////

class ShadowMapTech
{
public:
	ShadowMapTech();
	~ShadowMapTech();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Init();

	void SetBias(float bias);
	float GetBias();

	void SetBlurPixel(float blur_pixel);
	float GetBlurPixel();

	void Begin();
	void End();

	void GenShadow2(IDirect3DTexture9* shadowmap);

	void SetPosition(const float3* pos);
	void GetPosition(float3* pos);

	void SetDirection(const float3* dir);
	void GetDirection(float3* dir);

	void SetAngleNearFar(const float3* anf);
	void GetAngleNearFar(float3* anf);

	void SetFar(float sfar);
	float GetFar();

	void SetNear(float snear);
	float GetNear();

	void SetAngle(float sangle);
	float GetAngle();

	void SetIDArr(long id, long idarr);
	long GetCountIDArrs();
	long GetIDArr(long id);

	ISXFrustum* Frustum;
	//long IDArr;

	float4x4 View;
	float4x4 Proj;

private:

	Array<long> IDArr;
	float Bias;
	float BlurPixel;

	float3 Position;
	float3 Direction;
	float3 AngleNearFar;

	IDirect3DTexture9*	DepthMap;
	IDirect3DSurface9*	DepthSurface;
	IDirect3DSurface9*	DepthStencilSurface;
	
	float4x4 OldView,OldProj,OldViewProj;
	float4x4 ScaleBiasMat;

	IDirect3DSurface9* OldDepthStencilSurface;
	IDirect3DSurface9*  OldColorSurface;
};


class ShadowMapCubeTech
{
public:
	ShadowMapCubeTech();
	~ShadowMapCubeTech();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void SetEnableCubeEdge(int edge,bool enable);
	bool GetEnableCubeEdge(int edge);

	void SetBias(float bias);
	float GetBias();

	void SetBlurPixel(float blur_pixel);
	bool GetBlurPixel();

	void Init();

	void SetPosition(float3* pos);
	void GetPosition(float3* pos);

	void SetNearFar(float2* nf);
	void GetNearFar(float2* nf);

	void SetNear(float val);
	float GetNear();

	void SetFar(float val);
	float GetFar();
	
	void Begin();
	void Pre(int cube);
	void Post(int cube);
	void End();

	void GenShadow2(IDirect3DTexture9* shadowmap);

	void SetIDArr(long id, int split, long idarr);
	long GetCountIDArrs();
	long GetIDArr(long id, int split);

	ISXFrustum* Frustums[6];
	//long IDArr[6];

private:

	Array<long*> IDArr;
	float Bias;
	float BlurPixel;
	IDirect3DCubeTexture9*	DepthMap;
	IDirect3DSurface9*	DepthSurface[6];
	IDirect3DSurface9*	DepthStencilSurface;
	float4x4 View[6];
	float4x4 Proj[6];

	bool EnableEdge[6];
	bool EnableEdgeNulled[6];

	float4x4 OldView,OldProj,OldViewProj;

	IDirect3DSurface9* OldDepthStencilSurface;
	IDirect3DSurface9*  OldColorSurface;

	float3 Position;
	float2 NearFar;
};

#endif