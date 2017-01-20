#ifndef _ModelFile_H_
#define _ModelFile_H_

#define SX_MODEL_VERSION 7
#define SX_MODEL_MAGICK 30510779525321540

#define MODEL_MAX_NAME 32
#define MODEL_BONE_MAX_NAME 32
#define MODEL_CTRL_MAX_BONES 64

#include <sxtypes.h>
#include <SXMath.h>

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
	byte BoneIndices[4];
	float4_t BoneWeights;
};

struct vertex_animated_ex: public model_vertex
{
	float3_t Pos;
	float2_t Tex;
	float3_t Norm;
	float3_t Tangent;
	float3_t Binorm;
	byte BoneIndices[4];
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
	uint64_t Magick;
	uint32_t iVersion; // At present should be 4
	uint32_t iFlags;
	// char[64] szName; // model name without extension
	uint32_t iSkinCount; // Count of used skins
	uint32_t iMaterialCount; // Count of used materials
	uint64_t iMaterialsOffset; // SXmodelMaterialRangeHeader
	uint32_t iLODcount; // Count of Levels of detail
	uint64_t iLODoffset; // SXmodelLODheader
	uint32_t iBoneCount; // Count of model bones
	uint64_t iBonesOffset; // SXmodelBone
	uint32_t iAnimationCount; // Count of animation sequences
	uint64_t iAnimationsOffset; // SXmodelAnimationHeader
	uint64_t iSecondHeaderOffset;
};

struct ModelHeader2
{
	uint32_t iControllersCount;
	uint64_t iControllersOffset;
	uint32_t iDepsCount;
	uint64_t iDependensiesOffset;

	uint32_t iBoneTableCount;
	uint64_t iBoneTableOffset;
	uint32_t iActivitiesTableCount;
	uint64_t iActivitiesTableOffset;

	uint64_t iPhysicsDataOffset;

	MODEL_PT_TOPOLOGY topology;

	uint32_t iHitboxCount;
	uint64_t iHitboxesOffset;

	uint64_t iBboxInfoOffset;

	uint64_t iThirdHeaderOffset;
};

struct ModelBoneController
{
	uint32_t iBoneCount;
	float3_t fMinRot;
	float3_t fMaxRot;
	float3_t fMinTrans;
	float3_t fMaxTrans;
	char szName[MODEL_MAX_NAME];
	uint32_t bones[MODEL_CTRL_MAX_BONES];
};

struct ModelBoneShader
{
	float4_t position;
	SMQuaternion orient;
};

struct ModelBone
{
	int32_t id;
	int32_t pid;
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
	uint32_t iMaterialID;
	uint32_t iVectexCount;
	uint32_t iIndexCount;
	model_vertex * pVertices;
	uint32_t * pIndices;
	uint32_t iStartIndex;
	uint32_t iStartVertex;
};

struct ModelLoD
{
	uint32_t iSubMeshCount;
	ModelLoDSubset * pSubLODmeshes;
	ModelLoD():pSubLODmeshes(NULL), iSubMeshCount(0)
	{
	};
};// typedef SXmodelLODheader;
#define MODEL_LOD_STRUCT_SIZE sizeof(uint32_t)

struct ModelMatrial
{
	uint32_t iMat;
	char szName[MODEL_MAX_NAME];
};

struct ModelSequence
{
	char name[MODEL_MAX_NAME];
	byte bLooped;
	uint32_t framerate;
	uint32_t activity;
	uint32_t iNumFrames;
	uint32_t act_chance;
	ModelBone ** m_vmAnimData;
};

#define MODEL_SEQUENCE_STRUCT_SIZE (sizeof(char) * MODEL_MAX_NAME + sizeof(byte) + sizeof(uint32_t) * 4)

struct ModelDependensy
{
	char szName[MODEL_MAX_NAME];
};

struct ModelActivity
{
	char szName[MODEL_MAX_NAME];
	//uint32_t chance;
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
