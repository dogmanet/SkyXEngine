#ifndef _ModelFileAnim_H_
#define _ModelFileAnim_H_

#include <common/sxtypes.h>
#include <common/SXMath.h>

#include <gcore/ModelFile.h>

#define MODEL_MAX_FILE	128

#define BLEND_MAX 3




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

struct ModelSequence
{
	char name[MODEL_MAX_NAME];
	byte bLooped;
	int32_t framerate;
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

enum MODEL_IMPORT
{
	MI_MESH        = 0x00000001,
	MI_ANIMATIONS  = 0x00000002,
	MI_SKINS       = 0x00000004,
	MI_CONTROLLERS = 0x00000008,
	MI_HITBOXES    = 0x00000010,

	MI_ALL         = 0xFFFFFFFF
};

enum MODEL_ATTACH
{
	MA_SKIN = 0x00000001, // Merge skeletons
	MA_BONE = 0x00000002 // Join skeletons
};


enum ANIM_STATE
{
	AS_STOP,
	AS_PLAY,
	AS_LOOP
};

enum MODEL_PART_FLAGS
{
	MP_NONE = 0x00000001,
	MP_HIDDEN = 0x00000002,
	MP_COLLISIONS = 0x00000004,
	MP_RAYTRACE = 0x00000008,
	MP_IMPORTED = 0x00000010
};

struct ModelAttachDesc
{
	MODEL_ATTACH type;
	float3_t v3Offs;
	char szBone[MODEL_BONE_MAX_NAME];
};

class ModelFile;
struct ModelPart
{
	const ModelFile * pMdl;
	UINT uImportFlags;
	UINT uFlags;
	ModelAttachDesc attachDesc;
	char name[MODEL_MAX_NAME];
	char file[MODEL_MAX_FILE];
};

enum MBERR
{
	MBE_SUCCESS = 0,
	MBE_NO_ROOT,
	MBE_BAD_HIERARCY
};

enum HITBOX_TYPE
{
	HT_BOX = 0,
	HT_CYLINDER,
	HT_CAPSULE,
	HT_ELIPSOID
};

enum HITBOX_BODYPART
{
	HBP_DEFAULT = 0,
	HBP_HEAD,
	HBP_CHEST,
	HBP_STOMACH,
	HBP_LEFTARM,
	HBP_RIGHTARM,
	HBP_LEFTLEG,
	HBP_RIGHTLEG
};

struct ModelHitbox
{
	HITBOX_TYPE type;
	HITBOX_BODYPART part;
	float3_t lwh;
	float3_t pos;
	float3_t rot;
	char bone[MODEL_BONE_MAX_NAME];
	int bone_id;
	char name[MODEL_MAX_NAME];
};

#endif
