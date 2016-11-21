

class Light : public Bound
{
public:
	Light();
	Light(Light& ll);
	~Light();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void CreatePoint(float3* center,float radius,bool isglobal,bool is_shadowed,const char* bound_volume);
	void CreateDirection(float3* pos,float3* dir,float top_radius,float angle,float height,bool is_shadow,const char* bound_volume);
	void Render();

	void SetShadowCube();
	void SetShadowSM();

	inline void SetNullUpdateShadow();

	void RenderSource(bool is_material);

	void LoadSource(const char* path);

	void ChangeVolume(const char* path);
	void ChangeAngle(float angle,bool is_create);
	void ChangeTopRadius(float top_radius);
	void ChangeRadiusHeight(float radius_height,bool is_create);
	void ChangeShadow(bool is_shadow);

	inline void SetEnable(bool enable){IsEnable = enable;SetNullUpdateShadow();};
	inline bool GetEnable(){return IsEnable;};

	inline void SetEnableCubeEdge(int edge,bool enable);
	inline bool GetEnableCubeEdge(int edge);

	bool GetVisibleForFrustum(Core::ControllMoving::Frustum* frustum);
	inline bool GetVisible(){return IsVisible;};
	inline void SetVisible(bool is_visible){IsVisible = is_visible;};

	inline void SetBias(float bias);
	inline float GetBias();

	inline void SetShadowLocalFar(float slfar);
	inline float GetShadowLocalFar();

	inline void SetBlurPixel(float blur_pixel);
	inline float GetBlurPixel();

	inline void SetRenderGreen(bool is_render_green);
	inline bool GetRenderGreen();

	void GetWVP(float4x4* wvp);
	IDirect3DTexture9* GetShadow();

	//если источник глобальный, то x,y координаты это углы (в градусах) вращения относительно мира, ибо источник может быть только на отдалении
	//иначе посылаются координаты
	inline void GetPosition(float4* pos);		//для использования пользователю
	inline void GetPositionSys(float4* pos);	//используется движковыми элементами
	inline void SetPosition(float4* pos);

	inline void GetRotation(float3* rot);
	inline void SetRotation(float3* rot);

	inline void GetDirection(float3* dir);
	inline void SetDirection(float3* dir);

	inline void SetRays(const char* model_rays);

	PSSM* ShadowPSSM;
	ShadowMapTech* ShadowSM;
	ShadowMapCubeTech* ShadowCube;

	int CountRenderInSec;
	int CountRenderShadowInSec;
	float4 Color;

	float2_t TopBottomRadius;
	float Angle;
	int TypeLight;
	char Name[256];
	char PathVolume[1024];
	char PathRays[1024];
	bool IsShadow;
	bool IsGlobal;
	ID3DXMesh* Mesh;
	Model* MeshRays;
	float4x4 WorldMat;
	float DistForCamera;
	Model* SourceLight;
	DWORD IDEffect;

protected:

	float PosLigthMaxValue;
	float AngleY;
	float AngleX;

	float4x4 MatRot;

	bool IsTransformation;
	bool IsVisible;
	
	bool IsEnable;
	float4 Position;
	float3 Direction;
	float3 Rotation;

	float3 Center2;
};


class ManagerLight
{
public:
	ManagerLight();
	~ManagerLight();
	void AddPoint(float3* center,float4* color,float radius,bool isglobal,bool is_shadowed,const char* bound_volume=0);
	void AddDirection(float3* pos,float3* dir,float4* color,float top_radius,float angle,float height,bool is_shadow,const char* bound_volume=0);
	void Render();
	void RenderSource(bool is_material,bool is_enable);
	void RenderSelected();

	bool TraceBeam(const float3 & start, const float3 & dir, float3 * _res,DWORD* idlight);

	void OnLostDevice();
	void OnResetDevice();

	void Delete(DWORD id);

	inline void SetEnable(DWORD id,bool enable);
	inline bool GetEnable(DWORD id);

	inline void SetSelected(DWORD id);
	inline DWORD GetSelected();
	inline DWORD GetCount();

	inline void SetName(DWORD id,const char* text);
	inline void GetName(DWORD id,char* text);

	inline void SetVolume(DWORD id,const char* text);
	inline void GetVolume(DWORD id,char* text);

	inline DWORD GetType(DWORD id);


//private:
	DWORD NumSelected;	//id выделенного света
	Array<Light*> Arr;
};