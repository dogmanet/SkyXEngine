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
	MODEL_FLAG_HAS_TANGENT_BINORM = 0x2000, // should never been defined
	MODEL_FLAG_NEW_STYLE_DEPS = 0x4000
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
	uint32_t iVersion; // версия файла
	uint32_t iFlags; // флаги
	uint32_t iSkinCount; // Количество скинов в модели
	uint32_t iMaterialCount; // Количество сабсетов
	uint64_t iMaterialsOffset; // Смещение до блока списка материалов
	uint32_t iLODcount; // Количество лодов
	uint64_t iLODoffset; // Смещение до блока лодов
	uint32_t iBoneCount; // Количество костей в скелете
	uint64_t iBonesOffset; // Смещение до блока костей
	uint32_t iAnimationCount; // Количество анимаций в файле
	uint64_t iAnimationsOffset; // Смещение до блока анимаций
	uint64_t iSecondHeaderOffset; // Смещение до второго заголовка
};

struct ModelHeader2
{
	uint32_t iControllersCount; // Количество контроллеров
	uint64_t iControllersOffset; // Смещение до блока контроллеров
	uint32_t iDepsCount; // Количество включаемых файлов
	uint64_t iDependensiesOffset; // Смещение до блока включаемых файлов

	uint32_t iBoneTableCount; // Количество костей в таблице костей
	uint64_t iBoneTableOffset; // Смещение до таблицы костей
	uint32_t iActivitiesTableCount; // Количество записей в таблице активностей
	uint64_t iActivitiesTableOffset; // Смещение до таблицы активностей

	uint64_t iPhysicsDataOffset; // Смещение до блока физических данных

	MODEL_PT_TOPOLOGY topology; // Используемая топология примитивов

	uint32_t iHitboxCount; // Количество хитбоксов
	uint64_t iHitboxesOffset; // Смещение до блока хитбоксов

	uint64_t iBboxInfoOffset; // Смещение до блока информации об ограничивающем объеме

	uint64_t iThirdHeaderOffset; // Смещение до третьего заголовка
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

#endif
