#ifndef _ANIMATED_H_
#define _ANIMATED_H_


#include "ModelFile.h"

#define BLEND_MAX 3



typedef AssotiativeArray<String, ModelBoneName*> ModelBoneList;
typedef AssotiativeArray<int, Array<int>> ModelBoneChildrenList;
typedef AssotiativeArray<int, ModelBoneName*> ModelBoneIdList;
typedef AssotiativeArray<int, int> ModelBoneRelinkList;
typedef AssotiativeArray<String, ModelBoneName> ModelBones;

class Animation;

class ModelFile
{
	friend class Animation;
public:
	ModelFile(const char * name);
	~ModelFile();

	SX_ALIGNED_OP_MEM

	void Save(const char * name);
	
	//0 - простой рендер, 1 - с материалом, 2 - pssm, 3 - cube
	void Render(int howrender,int render_forward,SMMATRIX * mWorld, UINT nSkin = 0, UINT nLod = 0);

	void Assembly();

	void BuildMeshBuffers();

	const ModelSequence * GetSequence(UINT id) const;
	const ModelBoneController * GetController(UINT id) const;
	UINT GetBoneCount() const;
	UINT GetSequenceCount() const;
	UINT GetControllersCount() const;

	UINT GetBoneID(const String & name);

	ModelMatrial ** m_iMaterials;
	ModelLoD * m_pLods;
	SkyXEngine::Graphics::ThreeD::Bound BoundVol;
protected:

	
	//void BuildHitboxes();
	void Load(const char * name);
	
	

	void MergeModel(const ModelFile * mdl);
	void MergeBones(
		ModelBoneChildrenList & child_remote,
		ModelBoneList & local,
		ModelBoneIdList & id_remote,
		ModelBoneRelinkList & relink_remote,
		ModelBones & res
		);
	void DoMerge(
		int startId,
		ModelBoneChildrenList & child_remote,
		ModelBoneIdList & id_remote,
		ModelBoneRelinkList & relink_remote,
		ModelBones & res
		);

	void MergeActivities(const ModelFile * mdl, ModelBoneRelinkList & relink);
	void MergeMaterials(const ModelFile * mdl, ModelBoneRelinkList & relink);
	void MergeLods(const ModelFile * mdl, ModelBoneRelinkList & relink);

	void ImportControllers(const ModelFile * mdl, ModelBoneRelinkList * relink);
	void ImportSequences(const ModelFile * mdl, ModelBoneRelinkList * relink, ModelBoneRelinkList & relink_activities);

	ModelHeader m_hdr;
	ModelHeader2 m_hdr2;

	IDirect3DIndexBuffer9 ** m_ppIndexBuffer;
	IDirect3DVertexBuffer9 ** m_ppVertexBuffer;

	const ModelFile ** m_pDeps;

	ModelActivity * pActivities;

	ModelBoneName * m_pBones;
	ModelBone * m_pBonesBindPoseInv;

	ModelBoneController * m_pControllers;

	ModelSequence * m_pSequences;

	//AssotiativeArray<String, ModelBoneController> m_mfBoneControllers;
	//AssotiativeArray<String, UINT> m_mSeqNames;

	

	

	bool m_bAssemblied;
	bool m_bLoaded;
	bool m_bInitPosInvSet;
};

class Animation
{
public:
	Animation();
	~Animation();

	SX_ALIGNED_OP_MEM

	void Advance(unsigned long int dt);
	bool IsVisibleFrustum(Core::ControllMoving::Frustum* frustum);
	//0 - простой рендер, 1 - с материалом, 2 - pssm, 3 - cube
	void Render(int howrender = 0,bool render_forward = false);

	void SetModel(const char * file);

	void PlayAnimation(const char * name, UINT iFadeTime, UINT slot = 0); // name: Animation name; changeTime: time to fade to this animation from previous

	void StartActivity(const String & name, UINT iFadeTime);

	//const SXmodel::SXmodelBoneController * GetBoneController(const String & name); // Get and Set values from/to controllable bones
	void SetBoneController(const String & name, float value, MODEL_BONE_CTL what);

	SMMATRIX GetBoneTransform(UINT id);

	inline bool PlayingAnimations();
	inline bool PlayingAnimations(const char* name);
	void StopAnimations();

	void FillBoneMatrix();

	void UpdateControllers();

	int GetActiveSkin();
	void SetSkin(int n);

	void SetPos(const float3 & pos);
	float3 GetPos();

	void SetOrient(const SMQuaternion & pos);
	SMQuaternion GetOrient();


	SMMATRIX GetWorldTM();

	bool IsVisible();
	void SetVisible(bool bdo);

	bool IsRenderForShadow();
	void SetRenderForShadow(bool bdo);

	ModelFile * m_pMdl;
protected:
	
	float3 jcenter2,jcenter;
	float jradius;
	bool m_bIsAnimationPlaying[BLEND_MAX];
	bool m_bNewAnimPlayed[BLEND_MAX];

	UINT m_iAnimFrameCount[BLEND_MAX];
	UINT m_iCurrentFrame[BLEND_MAX];

	ModelBoneShader * m_pBoneMatrix;

	UINT m_iFadeTime[BLEND_MAX];
	UINT m_iFadeCurTime[BLEND_MAX];
	bool m_bInFade[BLEND_MAX];

	float3_t m_vPosition;
	SMQuaternion m_vOrientation;

	unsigned long int m_iCurTime;

	UINT m_iBoneCount;


	ModelBone * m_CurrentBones[BLEND_MAX];
	ModelBone * m_FinalBones;
	ModelBone * m_LastFrameBones[BLEND_MAX];
	UINT m_iPlayingAnim[BLEND_MAX];

	ModelBoneCrontrollerValue * m_pBoneControllers;
	ModelBoneCrontrollerValue * m_mfBoneControllerValues;

	AssotiativeArray<String, UINT> m_mSeqIds;
	AssotiativeArray<String, UINT> m_mfBoneControllers;

	bool m_bBoneMatrixReFilled;

	int ctlCount;

	int m_iCurrentSkin;

	UINT myId;

	bool m_bVisible;
	bool m_bRenderForShadow;
	UINT t[BLEND_MAX];
};

class AnimationManager
{
public:
	static const ModelFile * LoadModel(const char * name);

	static void Init();

	static UINT Register(Animation * pAnim);
	static void UnRegister(UINT id);

	//0 - простой рендер, 1 - с материалом, 2 - pssm, 3 - cube
	static void Render(int howrender = 0,int render_forward=0,Core::ControllMoving::Frustum* frustum=0);
	static void Update();
	static void ComVisible();

	static void SetVertexDeclaration(MODEL_VERTEX_TYPE nDecl);
protected:
	static AssotiativeArray<String, ModelFile*> m_pModels;
	static void InitVertexDeclarations();

	static IDirect3DVertexDeclaration9 * pVertexDeclaration[MVT_SIZE];

	
	static Array<bool> ArrIsVisible;
	static Array<Animation*> m_pAnimatedList;

};


#endif
