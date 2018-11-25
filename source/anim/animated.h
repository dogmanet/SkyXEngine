
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __ANIMATED_H
#define __ANIMATED_H


#include "ModelFile.h"

#include <common/assotiativearray.h>
#include <common/array.h>
#include <common/string.h>


#ifdef _SERVER
#	define IDirect3DDevice9 void
#else
#	define D3D_DEBUG_INFO
#	include <d3d9.h>
#	include <d3dx9.h>
#	pragma comment(lib, "d3dx9.lib")
#endif

#include "sxanim.h"

class Animation;
class AnimationManager;
class ISXBound;

class ModelFile
{
	friend class Animation;
	friend class Editor;
	friend class AnimationManager;
public:
	ModelFile(const char * name, AnimationManager * pMgr);
	~ModelFile();

	SX_ALIGNED_OP_MEM;

	bool Save(const char * name);
#ifndef _SERVER
	void render(SMMATRIX * mWorld, UINT nSkin, UINT nLod=0, ID idOverrideMaterial=-1);
#endif

	void BuildMeshBuffers();

	const ModelSequence * GetSequence(UINT id) const;
	const ModelSequence * GetSequence(const char * name) const;
	const ModelBoneController * GetController(UINT id) const;
	UINT getBoneCount() const;
	UINT GetSequenceCount() const;
	UINT GetControllersCount() const;

	UINT GetBoneID(const char * name);
	void getBoneName(UINT id, char * name, int len) const;

	ModelMatrial ** m_iMaterials;
	ModelLoD * m_pLods;
	//SkyXEngine::Graphics::ThreeD::Bound BoundVol;

	const ModelHitbox * getHitbox(const char * name) const;
	const ModelHitbox * getHitbox(uint32_t id) const;
	uint32_t getHitboxCount() const;
	void AddHitbox(const ModelHitbox * hb);
	void DelHitbox(const char * name);
	void DelHitbox(uint32_t id);

	void LoadParts();

	const ISXBound * getBound() const;
	
protected:

	MBERR AppendBones(const ModelFile * mdl, char * root=NULL);
	//void BuildHitboxes();
	void Load(const char * name);

	ModelHeader m_hdr;
	ModelHeader2 m_hdr2;
#ifndef _SERVER
	IDirect3DIndexBuffer9 ** m_ppIndexBuffer;
	IDirect3DVertexBuffer9 ** m_ppVertexBuffer;
#endif

	const ModelFile ** m_pDeps;

	ModelActivity * pActivities;

	ModelBoneName * m_pBones;
	ModelBone * m_pBonesBindPoseInv;

	ModelBoneController * m_pControllers;

	ModelSequence * m_pSequences;

	ModelHitbox * m_pHitboxes;

	ModelPart * m_pParts;

	//AssotiativeArray<String, ModelBoneController> m_mfBoneControllers;
	//AssotiativeArray<String, UINT> m_mSeqNames;

	char m_szFileName[MODEL_MAX_FILE];

	

	bool m_bAssemblied;
	bool m_bLoaded;
	bool m_bInitPosInvSet;

	AnimationManager * m_pMgr;

	bool m_bIsTemp;

	ISXBound * m_pBoundBox;


};

class Animation: public IAnimPlayer
{
	friend class AnimationManager;
public:
	Animation(AnimationManager * pMgr);
	~Animation();

	SX_ALIGNED_OP_MEM

	void advance(unsigned long int dt);

#ifndef _SERVER
	void render();
#endif

	void setModel(const char * file);

	void play(const char * name, UINT iFadeTime = 0, UINT slot = 0, bool bReplaceActivity = true); // name: Animation name; changeTime: time to fade to this animation from previous
	void stop(UINT slot = 0);
	void resume(UINT slot = 0);

	void setProgress(float progress, UINT slot = 0);
	void setAdvance(bool set, UINT slot = 0);

	void startActivity(const char * name, UINT iFadeTime = 0, UINT slot = 0);

	void setBoneController(const String & name, float value, MODEL_BONE_CTL what);

	SMMATRIX getBoneTransform(UINT id, bool bWithScale = false);
	float3 getBoneTransformPos(UINT id);
	SMQuaternion getBoneTransformRot(UINT id);
	UINT getBone(const char * str);
	UINT getBoneCount() const;
	void getBoneName(UINT id, char * name, int len) const;

	inline bool playingAnimations();
	inline bool playingAnimations(const char* name);
	void stopAll();


	int getActiveSkin();
	void setSkin(int n);

	void setPos(const float3 & pos);
	float3 getPos();

	void setOrient(const SMQuaternion & pos);
	SMQuaternion getOrient();


	SMMATRIX getWorldTM();

	AnimStateCB setCallback(AnimStateCB cb);
	void setCallback(CBaseAnimating *pEnt, AnimStateEntCB cb);
	AnimProgressCB setProgressCB(AnimProgressCB cb);

	ModelSequence const * getCurAnim(int slot);

	ModelFile * m_pMdl;

	void SyncAnims();

	const ModelFile * addModel(const char * mdl, UINT flags = MI_ALL, char * name = "No name");
	void addModel(const ModelFile * mdl, UINT flags = MI_ALL, char * name="No name");
	int addModel(ModelPart * mp);
	void delModel(ModelPart * mp);
	void assembly();
	ModelPart * GetPart(UINT idx);
	UINT GetPartCount();

	const ISXBound * getBound() const;

	void SwapBoneBuffs();

	void Release();

	void setScale(float fScale)
	{
		m_fScale = fScale;
	}

	float getScale()
	{
		return(m_fScale);
	}

	void getPhysData(
		int32_t * piShapeCount,
		HITBOX_TYPE ** phTypes,
		float3_t *** pppfData,
		int32_t ** ppfDataLen);

	void freePhysData(
		int32_t iShapeCount,
		HITBOX_TYPE * hTypes,
		float3_t ** ppfData,
		int32_t * pfDataLen);

	void RenderSkeleton(int hlBone=-1);

	virtual const ModelHitbox * getHitbox(uint32_t id) const;
	virtual uint32_t getHitboxCount() const;

	virtual void setOverrideMaterial(const char *name);
	virtual void enable(bool enable);

	virtual void setRagdoll(IAnimRagdoll * pRagdoll);

	virtual bool isVisibleFor(ID id);

	//static void AssemblyMdl(ModelFile * pOut, const Array<ModelPart*> & mMdls);
protected:

	IAnimRagdoll * m_pRagdoll;

	void DownloadData();

	void FillBoneMatrix();

	void UpdateControllers();

	void PlayActivityNext(UINT slot);

	float3 jcenter2,jcenter;
	float jradius;
	bool m_bIsAnimationPlaying[BLEND_MAX];
	bool m_bNewAnimPlayed[BLEND_MAX];

	UINT m_iAnimFrameCount[BLEND_MAX];
	int m_iCurrentFrame[BLEND_MAX];

	ModelBoneShader * m_pBoneMatrix;
	ModelBoneShader * m_pBoneMatrixRender; //!< read only in update cycle

	UINT m_iFadeTime[BLEND_MAX];
	UINT m_iFadeCurTime[BLEND_MAX];
	bool m_bInFade[BLEND_MAX];
	bool m_bDoAdvance[BLEND_MAX];

	ID m_idOverrideMaterial;
	bool m_bEnabled;

	float3_t m_vPosition;
	SMQuaternion m_vOrientation;

	unsigned long int m_iCurTime;

	UINT m_iBoneCount;


	ModelBone * m_CurrentBones[BLEND_MAX];
	bool *m_pIsBoneWorld[BLEND_MAX];
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
	int t[BLEND_MAX];

	AnimationManager * m_pMgr;

	AnimStateCB m_pfnCallBack;
	CBaseAnimating *m_pCallbackEnt;
	AnimStateEntCB m_pfnCallBackEnt;
	AnimProgressCB m_pfnProgressCB;


	Array<ModelPart*> m_mMdls;
	MemAlloc<ModelPart, 8> m_aMdls;

	Array<bool> m_vIsVisibleFor;

	float m_fScale;

	int m_iCurrentActivity[BLEND_MAX];
	UINT m_iCurrentActivityFadeTime[BLEND_MAX];

	bool m_isMdlManaged;

private:
	ISXBound * m_pBoundBox;
	void AppendMesh(ModelLoDSubset * to, ModelLoDSubset * from, Array<int> & bone_relink, bool isStatic=false);
};

class AnimationManager
{
public:
	AnimationManager(IDirect3DDevice9 * dev);
	~AnimationManager();
	const ModelFile * loadModel(const char * name, bool newInst = false);
	void unloadModel(const ModelFile * mdl);

	UINT reg(Animation * pAnim);
	void unreg(UINT id);

	void render(ID for_id=-1);
	void update(int thread = 0);
	void sync();

	void setVertexDeclaration(MODEL_VERTEX_TYPE nDecl);

	UINT getMaterial(const char * mat, bool bStatic = false);

	void computeVis(const IFrustum * frustum, const float3 * viewpos, ID id_arr);

	ID getNextVisId();
	void freeVisID(ID id);

	void setThreadNum(int num);

protected:
	friend class ModelFile;
	friend class Animation;
	AssotiativeArray<String, ModelFile*> m_pModels;
	void initVertexDeclarations();

	IDirect3DVertexDeclaration9 * pVertexDeclaration[MVT_SIZE];

	Array<Animation*> m_pAnimatedList;

	IDirect3DDevice9 * m_pd3dDevice;

	Array<ID> m_vFreeVisIDs;
	ID m_iVisID;

	int m_iThreadNum;
};


#endif
