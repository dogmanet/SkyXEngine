

class PSSM
{
public:

	PSSM();
	~PSSM();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Init();

	bool IsVisibleForCamera();	//находится ли зона покрытия в видимости камеры

	void Update(DWORD spilt,float3* poscam,float3* dircam);
	void Begin();
	void End();

	void GenShadow();
	void NullingShadow();
	void SoftShadow(bool randomsam,float size);
	IDirect3DTexture9* GetShadow();

	inline void SetPosition(float3* pos){Position = *pos;};
	inline void GetPosition(float3* pos){*pos = Position;};

	inline void SetBlurPixel(float blur_pixel);
	inline bool GetBlurPixel();

	inline void Set4Or3Splits(bool is4){Generating4Slits = is4;};
	inline bool Get4Or3Splits(){return Generating4Slits;};

	ISXFrustum* Frustums[5];

	int IsUpdate[5];
	float2 NearFar[5];
	IDirect3DTexture9*	DepthMaps[5];

protected:

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
	D3DXMATRIX OldView,OldProj,OldViewProj;
	float4x4 ScaleBiasMat;
	
	IDirect3DTexture9* ShadowMap;
	IDirect3DTexture9* ShadowMap2;
	int HowShadow; //внутренний релатайм флаг
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

	inline void SetStatic(bool is_static){IsStatic = is_static;};
	inline bool GetStatic(){return IsStatic;};

	inline void SetUpdate(bool is_update){IsUpdate = is_update;};
	inline bool GetUpdate(){return IsUpdate;};

	inline void SetBias(float bias);
	inline float GetBias();

	inline void SetBlurPixel(float blur_pixel);
	inline float GetBlurPixel();

	void Begin();
	void End();

	void GenShadow();
	void SoftShadow(bool randomsam,float size);
	IDirect3DTexture9* GetShadow();

	void SetPosition(float3* pos);
	void GetPosition(float3* pos);

	void SetDirection(float3* dir);
	void GetDirection(float3* dir);

	inline void SetAngleNearFar(float3* anf);
	inline void GetAngleNearFar(float3* anf);

	inline void SetFar(float sfar);
	inline float GetFar();

	inline void SetNear(float snear);
	inline float GetNear();

	inline void SetAngle(float sangle);
	inline float GetAngle();

	ISXFrustum* Frustum;

	float4x4 View;
	float4x4 Proj;
	bool IsRenderGreen;

private:

	float Bias;
	float BlurPixel;

	bool IsStatic;
	bool IsUpdate;

	float3 Position;
	float3 Direction;
	float3 AngleNearFar;

	IDirect3DTexture9*	DepthMap;
	IDirect3DSurface9*	DepthSurface;
	IDirect3DSurface9*	DepthStencilSurface;
	

	D3DXMATRIX OldView,OldProj,OldViewProj;
	float4x4 ScaleBiasMat;

	IDirect3DTexture9* ShadowMap;
	IDirect3DTexture9* ShadowMap2;
	int HowShadow; 
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

	inline void SetStatic(bool is_static){if(!IsStatic && is_static)CountRender = 0; IsStatic = is_static; };
	inline bool GetStatic(){return IsStatic;};

	inline void SetUpdate(bool is_update){IsUpdate = is_update;if(!IsUpdate)CountRender = 0;};
	inline bool GetUpdate(){return IsUpdate;}

	inline void SetEnableCubeEdge(int edge,bool enable);
	inline bool GetEnableCubeEdge(int edge);

	inline void SetBias(float bias){Bias = bias;};
	inline float GetBias(){return Bias;};

	inline void SetBlurPixel(float blur_pixel){BlurPixel = blur_pixel;};
	inline bool GetBlurPixel(){return BlurPixel;};

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

	void GenShadow();
	void SoftShadow(bool randomsam,float size);
	IDirect3DTexture9* GetShadow();

	ISXFrustum* Frustum;
	bool IsRenderGreen;

private:

	WORD CountRender;
	float Bias;
	float BlurPixel;
	IDirect3DCubeTexture9*	DepthMap;
	IDirect3DSurface9*	DepthSurface[6];
	IDirect3DSurface9*	DepthStencilSurface;
	float4x4 View[6];
	float4x4 Proj[6];

	bool EnableEdge[6];

	D3DXMATRIX OldView,OldProj,OldViewProj;

	bool IsStatic;
	bool IsUpdate;

	IDirect3DTexture9* ShadowMap;
	IDirect3DTexture9* ShadowMap2;
	int HowShadow; 
	IDirect3DSurface9* OldDepthStencilSurface;
	IDirect3DSurface9*  OldColorSurface;

	float3 Position;
	float2 NearFar;
};