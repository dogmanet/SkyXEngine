
#ifndef __particles_h
#define __particles_h

//структура описывающая партикл
struct CommonParticle
{
	SX_ALIGNED_OP_MEM

	CommonParticle()
	{
		Age = 0;
		TimeLife = 0;
		AnimTexRateMls = 0;
		AnimTexCurrentMls = 0;
		AnimTexCurrentCadr = 0;
		AlphaAgeDependCoef = 1;
		AlphaDeath = 1;
		IsAlife = false;
		AnimTexSizeCadrAndBias = float4(1, 1, 0, 0);

		Ambient = float4(1, 1, 1, 1);

		rot_1.x = sinf(0);
		rot_1.y = cosf(0);

		CharacterRotateAngleZ = 0;
		CharacterDeviationCountDelayMls2 = 0;

		LightingIntens = 1;

		Track = false;
		TrackTime = 0;
	}

	float3 PosCreate;	//позиция создания
	float3 Pos;			//текущая позиция
	DWORD Age;			//возраст частицы в млсек
	DWORD TimeLife;		//время в млсек которое проживет частица

	float4 Ambient;

	float2_t Size;
	float4 AnimTexSizeCadrAndBias;

	WORD AnimTexCurrentCadr;
	DWORD AnimTexRateMls;
	DWORD AnimTexCurrentMls;

	float CharacterRotateAngleZ;
	DWORD CharacterDeviationCountDelayMls2;

	float2_t rot_1;

	float3 DeviationVector;

	float3 Velocity;		//скорость
	float3 Acceleration;	//ускорение

	float AlphaAgeDependCoef;
	float AlphaDeath;
	float LightingIntens;
	bool IsAlife;


	bool Track;
	float3 TrackPos;
	float3 TrackNormal;
	DWORD TrackTime;
};

//основное представление частицы в шейдере
struct CommonParticleDecl
{
	CommonParticleDecl(float posx, float posy, float posz, float texx, float texy)
	{
		pos.x = posx;
		pos.y = posy;
		pos.z = posz;

		tex.x = texx;
		tex.y = texy;
	}

	float3_t pos;
	float2_t tex;
};

//представление частицы для инстансинга в шейдере
struct CommonParticleDecl2
{
	float3_t pos;
	float4_t tex;
	float size;
	float alpha;

	float2_t rot_1;

	float lighting;
};



//#############################################################################

class Emitter
{
public:

	Emitter();
	Emitter(Emitter& part);
	~Emitter();

	SX_ALIGNED_OP_MEM

	void OnLostDevice();
	void OnResetDevice();

	void Init(ParticlesData* data);
	ParticlesData* GetData();

	inline void NameSet(const char* name);
	inline void NameGet(char* name);

	inline void CountSet(int count);
	inline int CountGet();
	inline int CountLifeGet();

	inline void EnableSet(bool enable);
	inline bool EnableGet();

	void GeomDataCreate();
	
	void Compute(const float4x4 * mat);
	void ComputeLighting();
	void Render(DWORD timeDelta, float4x4* matrot, float4x4* matpos);

	void TextureSet(const char* tex);
	void TextureSetID(ID tex);
	ID TextureGetID();
	void TextureGet(char* tex);

	void TextureTrackSet(const char* tex);
	void TextureTrackSetID(ID tex);
	ID TextureTrackGetID();
	void TextureTrackGet(char* tex);

	void AnimTexDataInit();

	inline void AlifeSet(bool alife);
	inline bool AlifeGet();

	inline int TrackCountGet();
	int TrackPosGet(float3** arr, int count);

	float3_t CurrMin;
	float3_t CurrMax;
	float SizeAdd;

protected:

	inline void NullingInit();

	char Name[OBJECT_NAME_MAX_LEN];

	inline bool IsPointInCone(float3* point);
	inline bool IsPointInSphere(float3* point);
	inline bool IsPointInBox(float3* point);

	void CreateParticles();
	inline void ReCreateParticles(WORD id);

	void VertexBuffModify();

	void UpdateAnimTex(WORD idparticle, DWORD tmptime);
	float2_t AnimTexSize;	//размер текстуры
	float4 AnimSizeCadr;//размер одного кадра, xy - в пикселях, zw - в процентном соотношении к размеру текстуры

	ID IDTex;			//id текстуры, общей на все партиклы текущего организатора
	ID IDTexTrack;		//id текстуры следа, общей на все партиклы текущего организатора
	DWORD OldTime;		//прошлое время с которого началось обработка
	DWORD TimeNextSpawnParticle;	//время спавна будущего партикла
	
	long CountReCreate2;//сколько переродили

	int CountLifeParticle;	//количество живых частиц
	bool Alife;					//живо ли сие? если false то все партиклы умерли
	float GTransparency;
	DWORD TimerDeath;
	bool Enable;

	CommonParticle* Arr;
	int Count;

	IDirect3DVertexBuffer9* TransVertBuf;

	IDirect3DVertexBuffer9* VertexBuff;
	IDirect3DIndexBuffer9* IndexBuff;

	IDirect3DVertexBuffer9* VertexBuffQuad;
	IDirect3DIndexBuffer9* IndexBuffQuad;

	ParticlesData Data;

	float2_t OldSize;
};

#endif
