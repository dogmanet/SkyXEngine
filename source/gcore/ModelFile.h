#ifndef _ModelFile_H_
#define _ModelFile_H_

#define SX_MODEL_VERSION_OLD 6
#define SX_MODEL_VERSION 7
#define SX_MODEL_MAGICK 30510779525321540

#define MODEL_MAX_NAME 32
#define MODEL_BONE_MAX_NAME 32
#define MODEL_CTRL_MAX_BONES 64

struct model_vertex
{

};

struct vertex_static: public model_vertex
{
	float3_t Pos;
	float2_t Tex;
	float3_t Norm;
};

struct vertex_static_ex: public model_vertex
{
	float3_t Pos;
	float2_t Tex;
	float3_t Norm;
	float3_t Tangent;
	float3_t Binorm;
};

struct vertex_animated: public model_vertex
{
	float3_t Pos;
	float2_t Tex;
	float3_t Norm;
	unsigned char BoneIndices[4];
	float4_t BoneWeights;
};

struct vertex_animated_ex: public model_vertex
{
	float3_t Pos;
	float2_t Tex;
	float3_t Norm;
	float3_t Tangent;
	float3_t Binorm;
	unsigned char BoneIndices[4];
	float4_t BoneWeights;
};

enum MODEL_VERTEX_TYPE
{
	MVT_STATIC = 0,
	MVT_STATIC_EX,
	MVT_DYNAMIC,
	MVT_DYNAMIC_EX,

	//count
	MVT_SIZE
};

enum MODEL_FLAG
{
	MODEL_FLAG_COMPILED = 0x001, // is a full model file
	MODEL_FLAG_SOURCED = 0x002, // is a part of the model file

	MODEL_FLAG_SOURCE_ANIMATION = 0x004,
	MODEL_FLAG_SOURCE_MESH = 0x008,
	// MODEL_FLAG_SOURCE_LOD_MESH = 0x010,
	// MODEL_FLAG_SOURCE_PHYSBOX = 0x020,
	MODEL_FLAG_ANIMATED = 0x040,
	MODEL_FLAG_PHYSIC = 0x080,
	MODEL_FLAG_STATIC = 0x100,
	MODEL_FLAG_HAS_BONES = 0x200,
	MODEL_FLAG_HAS_GIBS = 0x400,
	MODEL_FLAG_HAS_PHYSBOX = 0x800,
	MODEL_FLAG_HAS_HITBOXES = 0x1000,
	MODEL_FLAG_HAS_TANGENT_BINORM = 0x2000 // should never been defined
};

enum MODEL_PT_TOPOLOGY
{
	MDLPT_TRIANGLELIST = 0,
	MDLPT_TRIANGLESTRIP = 1,
	MDLPT_TRIANGLEFAN = 2,

	MDLPT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};

struct ModelHeader
{
	unsigned long long int Magick;
	UINT iVersion; // At present should be 4
	UINT iFlags;
	// char[64] szName; // model name without extension
	UINT iSkinCount; // Count of used skins
	UINT iMaterialCount; // Count of used materials
	unsigned long long iMaterialsOffset; // SXmodelMaterialRangeHeader
	UINT iLODcount; // Count of Levels of detail
	unsigned long long iLODoffset; // SXmodelLODheader
	UINT iBoneCount; // Count of model bones
	unsigned long long iBonesOffset; // SXmodelBone
	UINT iAnimationCount; // Count of animation sequences
	unsigned long long iAnimationsOffset; // SXmodelAnimationHeader
	unsigned long long iSecondHeaderOffset;
};

struct ModelHeader2
{
	UINT iControllersCount;
	unsigned long long iControllersOffset;
	UINT iDepsCount;
	unsigned long long iDependensiesOffset;

	UINT iBoneTableCount;
	unsigned long long iBoneTableOffset;
	UINT iActivitiesTableCount;
	unsigned long long iActivitiesTableOffset;

	unsigned long long iPhysicsDataOffset;

	MODEL_PT_TOPOLOGY topology;

	UINT iHitboxCount;
	unsigned long long iHitboxesOffset;

	unsigned long long iBboxInfoOffset;

	unsigned long long iThirdHeaderOffset;
};

struct ModelBoneController
{
	UINT iBoneCount;
	float3_t fMinRot;
	float3_t fMaxRot;
	float3_t fMinTrans;
	float3_t fMaxTrans;
	char szName[MODEL_MAX_NAME];
	UINT bones[MODEL_CTRL_MAX_BONES];
};

struct ModelBoneShader
{
	float4_t position;
	SMQuaternion orient;
};

struct ModelBone
{
	int id;
	int pid;
	SMQuaternion orient;
	float3_t position;
};

struct ModelBoneName
{
	ModelBone bone;
	char szName[MODEL_BONE_MAX_NAME];
};

struct ModelLoDSubset
{
	unsigned int iMaterialID;
	unsigned int iVectexCount;
	unsigned int iIndexCount;
	model_vertex * pVertices;
	UINT * pIndices;
	UINT iStartIndex;
	UINT iStartVertex;
};

struct ModelLoD
{
	unsigned int iSubMeshCount;
	ModelLoDSubset * pSubLODmeshes;
	ModelLoD():pSubLODmeshes(NULL), iSubMeshCount(0)
	{
	};
};// typedef SXmodelLODheader;
#define MODEL_LOD_STRUCT_SIZE sizeof(unsigned int)

struct ModelMatrial
{
	UINT iMat;
	char szName[MODEL_MAX_NAME];
};

struct ModelSequence
{
	char name[MODEL_MAX_NAME];
	bool bLooped;
	UINT framerate;
	UINT activity;
	UINT iNumFrames;
	UINT act_chance;
	ModelBone ** m_vmAnimData;
};

#define MODEL_SEQUENCE_STRUCT_SIZE (sizeof(char) * MODEL_MAX_NAME + sizeof(bool) + sizeof(UINT) * 4)

struct ModelDependensy
{
	char szName[MODEL_MAX_NAME];
};

struct ModelActivity
{
	char szName[MODEL_MAX_NAME];
	//UINT chance;
};

enum MODEL_BONE_CTL
{
	MBCTL_ROT_X = 0,
	MBCTL_ROT_Y,
	MBCTL_ROT_Z,
	MBCTL_TRANS_X,
	MBCTL_TRANS_Y,
	MBCTL_TRANS_Z
};

struct ModelBoneCrontrollerValue
{
	SMQuaternion rot;
	float3_t tr;
};

#endif
