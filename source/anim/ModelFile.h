
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Файл определений для работы со скелетной анимацией
*/

/*! \defgroup sxanim_defs Определения для работы с файлами анимации
    \ingroup sxanim
@{
*/

#ifndef __MODELFILE_H
#define __MODELFILE_H

#include <common/types.h>
#include <common/Math.h>

#include <gcore/ModelFile.h>

#define MODEL_MAX_FILE	128 /*!< Максимальная длина имени файла модели */

#define BLEND_MAX 3 /*!< Количество слотов смешения анимации */



/*! Структура контроллера
*/
struct ModelBoneController
{
	uint32_t iBoneCount; /*!< Количество костей, которыми управляет контроллер */
	float3_t fMinRot; /*!< Вращение ОТ */
	float3_t fMaxRot; /*!< Вращение ДО */
	float3_t fMinTrans; /*!< Перемещение ОТ */
	float3_t fMaxTrans; /*!< Перемещение ДО */
	char szName[MODEL_MAX_NAME];  /*!< Имя контроллера */
	uint32_t bones[MODEL_CTRL_MAX_BONES];  /*!< Массив индексов костей */
};

/*! Структура кости для передачи в шейдер
*/
struct ModelBoneShader
{
	float4_t position;   /*!< Позиция */
	SMQuaternion orient; /*!< Вращение */
};

/*! Структура кости
*/
struct ModelBone
{
	int32_t id;          /*!< Номер кости */
	int32_t pid;         /*!< Номер родительской кости */
	SMQuaternion orient; /*!< Вращение */
	float3_t position;   /*!< Позиция */
};

/*! Структура с именем кости
*/
struct ModelBoneName
{
	ModelBone bone; /*!< Кость */
	char szName[MODEL_BONE_MAX_NAME]; /*!< Имя */
};

/*! Структура дескриптор анимации
*/
#pragma pack(push, 1)
struct ModelSequence
{
	char name[MODEL_MAX_NAME]; /*!< Имя */
	byte bLooped; /*!< Анимация зациклена */
	char _padding[3];
	int32_t framerate; /*!< Скорость кадров в секунду */
	uint32_t activity; /*!< Идентификатор активности */
	uint32_t iNumFrames; /*!< Количество кадров */
	uint32_t act_chance; /*!< Шанс воспроизведения этой анимации в активности */
	ModelBone ** m_vmAnimData; /*!< Данные анимации */
};
#pragma pack(pop)
#define MODEL_SEQUENCE_STRUCT_SIZE_OLD (sizeof(char) * MODEL_MAX_NAME + sizeof(byte) + sizeof(uint32_t) * 4) /*!< Файловый размер структуры дескриптора анимации */
#define MODEL_SEQUENCE_STRUCT_SIZE (MODEL_SEQUENCE_STRUCT_SIZE_OLD + 3)

/*! Устарела, используется для обратной совместимости
*/
struct ModelDependensy
{
	char szName[MODEL_MAX_NAME];
};

/*! Структура описатель активности
*/
struct ModelActivity
{
	char szName[MODEL_MAX_NAME];
	//uint32_t chance;
};

/*! Управление частями контроллера
*/
enum MODEL_BONE_CTL
{
	MBCTL_ROT_X = 0, /*!< Вращение по X */
	MBCTL_ROT_Y,     /*!< Вращение по Y */
	MBCTL_ROT_Z,     /*!< Вращение по Z */
	MBCTL_TRANS_X,   /*!< Перемещение по X */
	MBCTL_TRANS_Y,   /*!< Перемещение по Y */
	MBCTL_TRANS_Z    /*!< Перемещение по Z */
};

/*! Значение контроллера кости
*/
struct ModelBoneCrontrollerValue
{
	SMQuaternion rot;
	float3_t tr;
};


/*! Флаги импорта файла моделм анимации
*/
enum MODEL_IMPORT
{
	MI_MESH        = 0x00000001, /*!< Импортировать сетку */
	MI_ANIMATIONS  = 0x00000002, /*!< Импортировать анимации */
	MI_SKINS       = 0x00000004, /*!< Импортировать скины */
	MI_CONTROLLERS = 0x00000008, /*!< Импортировать контроллеры */
	MI_HITBOXES    = 0x00000010, /*!< Импортировать хитбоксы */

	MI_ALL         = 0xFFFFFFFF  /*!< Импортировать все */
};

/*! Типы соединения частей модели
*/
enum MODEL_ATTACH
{
	MA_SKIN = 0x00000001, /*!< Объединение скелетов */
	MA_BONE = 0x00000002  /*!< Присоединение иерархии к одной из костей */
};

/*! Изменение состояния воспроизведения анимации
*/
enum ANIM_STATE
{
	AS_STOP, /*!< Остановлено */
	AS_PLAY, /*!< Воспроизведено */
	AS_LOOP  /*!< Повтор заного */
};

/*! Флаги управления частями модели
*/
enum MODEL_PART_FLAGS
{
	MP_NONE = 0x00000001,       /*!< нет */
	MP_HIDDEN = 0x00000002,     /*!< Часть спрятана */
	MP_COLLISIONS = 0x00000004, /*!< Проверять столкновения */
	MP_RAYTRACE = 0x00000008,   /*!< Проверять трассировки луча */
	MP_IMPORTED = 0x00000010    /*!< Часть импортируется из внешнего файла */
};

/*! Дескриптор аттачмента модели
*/
struct ModelAttachDesc
{
	MODEL_ATTACH type; /*!< Тип соединения */
	float3_t v3Offs;   /*!< Смещение */
	char szBone[MODEL_BONE_MAX_NAME]; /*!< Кость для присоединения (только для MA_BONE) */
};

class ModelFile;

/*! Дескриптор части модели
*/
struct ModelPart
{
	const ModelFile * pMdl;     /*!< Модель */
	UINT uImportFlags;          /*!< Флаги импорта MODEL_IMPORT */
	UINT uFlags;                /*!< Флаги части MODEL_PART_FLAGS */
	ModelAttachDesc attachDesc; /*!< Дескриптор аттачмента */
	char name[MODEL_MAX_NAME];  /*!< Имя части */
	char file[MODEL_MAX_FILE];  /*!< Имя файла модели */
};

/*! Типы ошибок объединения скелетов
*/
enum MBERR
{
	MBE_SUCCESS = 0, /*!< Нет ошибки */
	MBE_NO_ROOT,     /*!< Не найден корень */
	MBE_BAD_HIERARCY /*!< Несовместимые иерархии костей */
};

/*! Типы хитбоксов
*/
enum HITBOX_TYPE
{
	HT_BOX = 0,  /*!< Параллелепипед */
	HT_CYLINDER, /*!< Цилиндр */
	HT_CAPSULE,  /*!< Капсула */
	HT_ELIPSOID,  /*!< Эллипсойд */
	HT_CONVEX
};

/*! Типы частей тела для хитбоксов
*/
enum HITBOX_BODYPART
{
	HBP_DEFAULT = 0, /*!< нет */
	HBP_HEAD,        /*!< Голова */
	HBP_CHEST,       /*!< Грудь */
	HBP_STOMACH,     /*!< Живот */
	HBP_LEFTARM,     /*!< Левая рука */
	HBP_RIGHTARM,    /*!< Правая рука */
	HBP_LEFTLEG,     /*!< Левая нога */
	HBP_RIGHTLEG     /*!< Правая нога */
};

/*! Дескриптор хитбокса
*/
struct ModelHitbox
{
	HITBOX_TYPE type;               /*!< Тип хитбокса */
	HITBOX_BODYPART part;           /*!< Часть тела */
	float3_t lwh;                   /*!< Размеры */
	float3_t pos;                   /*!< Положение */
	float3_t rot;                   /*!< Ориентация */
	char bone[MODEL_BONE_MAX_NAME]; /*!< Имя кости */
	int bone_id;                    /*!< Идентификатор кости */
	char name[MODEL_MAX_NAME];      /*!< Имя хитбокса */
};






struct ModelPhyspartData
{
};

#pragma pack(push, 1)
struct ModelPhyspartDataConvex: public ModelPhyspartData
{
	uint32_t iVertCount = 0;
	float3_t *pVerts = NULL;
};
#pragma pack(pop)
#define MODEL_PHYSPART_DATA_CONVEX_STRUCT_SIZE sizeof(uint32_t)

/*! Дескриптор хитбокса
*/
#pragma pack(push, 1)
struct ModelPhyspart
{
	HITBOX_TYPE type;               /*!< Тип хитбокса */
	float3_t lwh;                   /*!< Размеры */
	float3_t pos;                   /*!< Положение */
	float4_t rot;                   /*!< Ориентация */
	char bone[MODEL_BONE_MAX_NAME]; /*!< Имя кости */
	int bone_id;                    /*!< Идентификатор кости */
	char name[MODEL_MAX_NAME];      /*!< Имя хитбокса */
	uint64_t iDataOffset = 0;
	//-----------------------
	ModelPhyspartData *pData = NULL;
};
#pragma pack(pop)
#define MODEL_PHYSPART_STRUCT_SIZE (sizeof(ModelPhyspart) - sizeof(ModelPhyspartData*))

#pragma pack(push, 1)
struct ModelPhysData
{
	float fWeight = 1.0f;
	uint32_t iPhyspartCount = 0;
	uint64_t iPartsOffset = 0;
	//-----------------------
	ModelPhyspart *pParts = NULL;
};
#pragma pack(pop)
#define MODEL_PHYSDATA_STRUCT_SIZE (sizeof(ModelPhysData) - sizeof(ModelPhyspart*))

/*! @} sxanim_defs */

#endif

/*! @} */
