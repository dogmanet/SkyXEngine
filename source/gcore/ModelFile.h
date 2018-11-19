
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Файл определений для работы с файлами моделей
*/

/*! \defgroup sxgcore_mdl_defs Определения для работы с файлами моделей
	\ingroup sxgcore
@{
*/

#ifndef __MODEL_FILE_H
#define __MODEL_FILE_H

/*! Старая версия формата
 \todo убрать поддержку старой версии формата моделей
*/
#define SX_MODEL_VERSION_OLD 6
#define SX_MODEL_VERSION 7     /*!< Актуальная версия файла модели */
#define SX_MODEL_MAGICK 30510779525321540 /*!< Магическая константа для определения файла */

#define MODEL_MAX_NAME 32       /*!< Максимальная длина имени в файле модели */
#define MODEL_BONE_MAX_NAME 32  /*!< Максимальная длина имени кости */
#define MODEL_CTRL_MAX_BONES 64 /*!< Максимальное количество костей, затрагиваемых контроллером */

struct model_vertex
{

};

/*! Вертекс для статической модели
*/
struct vertex_static: public model_vertex
{
	float3_t Pos;  /*!< Позиция */
	float2_t Tex;  /*!< Текстурные координаты */
	float3_t Norm; /*!< Нормаль */
};

/*! Вертекс для статической модели расширенный
*/
struct vertex_static_ex: public model_vertex
{
	float3_t Pos;      /*!< Позиция */
	float2_t Tex;      /*!< Текстурные координаты */
	float3_t Norm;     /*!< Нормаль */
	float3_t Tangent;  /*!< Тангент */
	float3_t Binorm;   /*!< Бинормаль */
};

/*! Вертекс для анимированной модели
*/
struct vertex_animated: public model_vertex
{
	float3_t Pos;      /*!< Позиция */
	float2_t Tex;      /*!< Текстурные координаты */
	float3_t Norm;     /*!< Нормаль */
	byte BoneIndices[4];  /*!< Индексы костей */
	float4_t BoneWeights; /*!< Веса костей */
};

/*! Вертекс для анимированной модели расширенный
*/
struct vertex_animated_ex: public model_vertex
{
	float3_t Pos;      /*!< Позиция */
	float2_t Tex;      /*!< Текстурные координаты */
	float3_t Norm;     /*!< Нормаль */
	float3_t Tangent;  /*!< Тангент */
	float3_t Binorm;   /*!< Бинормаль */
	byte BoneIndices[4];  /*!< Индексы костей */
	float4_t BoneWeights; /*!< Веса костей */
};

/*! Типы вертексов
*/
enum MODEL_VERTEX_TYPE
{
	MVT_STATIC = 0, /*!< Вертекс для статической модели */
	MVT_STATIC_EX,  /*!< Вертекс для статической модели расширенный */
	MVT_DYNAMIC,    /*!< Вертекс для анимированной модели */
	MVT_DYNAMIC_EX, /*!< Вертекс для анимированной модели расширенный */

	//count
	MVT_SIZE        /*!< Количество типов вертексов */ 
};

/*! Флаги файлов модели
*/
enum MODEL_FLAG
{
	MODEL_FLAG_COMPILED = 0x001,            /*!< Скомпилированная модель */
	MODEL_FLAG_SOURCED = 0x002,             /*!< Модель источник */

	MODEL_FLAG_SOURCE_ANIMATION = 0x004,    /*!< Источник анимации */
	MODEL_FLAG_SOURCE_MESH = 0x008,         /*!< Источник сетки */
	// MODEL_FLAG_SOURCE_LOD_MESH = 0x010,
	// MODEL_FLAG_SOURCE_PHYSBOX = 0x020,
	MODEL_FLAG_ANIMATED = 0x040,            /*!< Анимированная модель */
	MODEL_FLAG_PHYSIC = 0x080,              /*!< Есть физическая модель */
	MODEL_FLAG_STATIC = 0x100,              /*!< Статическая модель */
	MODEL_FLAG_HAS_BONES = 0x200,           /*!< В файле есть кости */
	MODEL_FLAG_HAS_GIBS = 0x400,            /*!< Unused */
	MODEL_FLAG_HAS_PHYSBOX = 0x800,         /*!< Имеется физическая оболочка (Unused?) */
	MODEL_FLAG_HAS_HITBOXES = 0x1000,       /*!< Имеются хитбоксы */
	MODEL_FLAG_HAS_TANGENT_BINORM = 0x2000, /*!< Используются расширенные типы вертексов */
	MODEL_FLAG_NEW_STYLE_DEPS = 0x4000,     /*!< Используется новый тип импортированных моделей */
	MODEL_FLAG_NEW_PACKED_ANIMS = 0x8000,   /*!< Сохранено с использованием исправленного размера структуры ModelSequence */
	MODEL_FLAG_NORMALIZED_NORMALS = 0x10000    /*!< Сохранено с использованием исправленного размера структуры ModelSequence */
};

/*! Типф топологий модели в файле
*/
enum MODEL_PT_TOPOLOGY
{
	MDLPT_TRIANGLELIST = 0,  /*!< Списки треугольников */
	MDLPT_TRIANGLESTRIP = 1, /*!< Полоски треугольников */
	MDLPT_TRIANGLEFAN = 2,   /*!< Фаны треугольников */

	MDLPT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};

/*! Первый заголовок файла модели
*/
struct ModelHeader
{
	uint64_t Magick;
	uint32_t iVersion; //!< версия файла
	uint32_t iFlags; //!< флаги
	uint32_t iSkinCount; //!< Количество скинов в модели
	uint32_t iMaterialCount; //!< Количество сабсетов
	uint64_t iMaterialsOffset; //!< Смещение до блока списка материалов
	uint32_t iLODcount; //!< Количество лодов
	uint64_t iLODoffset; //!< Смещение до блока лодов
	uint32_t iBoneCount; //!< Количество костей в скелете
	uint64_t iBonesOffset; //!< Смещение до блока костей
	uint32_t iAnimationCount; //!< Количество анимаций в файле
	uint64_t iAnimationsOffset; //!< Смещение до блока анимаций
	uint64_t iSecondHeaderOffset; //!< Смещение до второго заголовка
};

/*! Второй заголовок файла модели
*/
struct ModelHeader2
{
	uint32_t iControllersCount; //!< Количество контроллеров
	uint64_t iControllersOffset; //!< Смещение до блока контроллеров
	uint32_t iDepsCount; //!< Количество включаемых файлов
	uint64_t iDependensiesOffset; //!< Смещение до блока включаемых файлов

	uint32_t iBoneTableCount; //!< Количество костей в таблице костей
	uint64_t iBoneTableOffset; //!< Смещение до таблицы костей
	uint32_t iActivitiesTableCount; //!< Количество записей в таблице активностей
	uint64_t iActivitiesTableOffset; //!< Смещение до таблицы активностей

	uint64_t iPhysicsDataOffset; //!< Смещение до блока физических данных

	MODEL_PT_TOPOLOGY topology; //!< Используемая топология примитивов

	uint32_t iHitboxCount; //!< Количество хитбоксов
	uint64_t iHitboxesOffset; //!< Смещение до блока хитбоксов

	uint64_t iBboxInfoOffset; //!< Смещение до блока информации об ограничивающем объеме

	uint64_t iThirdHeaderOffset; //!< Смещение до третьего заголовка
};

/*! Структура сабсета лода модели
*/
struct ModelLoDSubset
{
	uint32_t iMaterialID; //!< Идентификатор материала
	uint32_t iVectexCount; //!< Количество вертексов в сабсете
	uint32_t iIndexCount; //!< Количество индексов в сабсете
	model_vertex * pVertices; //!< Указатель на массив вертексов
	UINT * pIndices; //!< Массив индексов
	UINT iStartIndex; //!< Начальный индекс отрисовки сабсеа
	UINT iStartVertex; //!< Начальный вертекс отрисовки сабсета
};

/*! Дескриптор лода модели
*/
struct ModelLoD
{
	unsigned int iSubMeshCount; //!< Количество сабсетов
	ModelLoDSubset * pSubLODmeshes; //!< Массив сабсетов
	ModelLoD():pSubLODmeshes(NULL), iSubMeshCount(0)
	{
	};
};// typedef SXmodelLODheader;
#define MODEL_LOD_STRUCT_SIZE sizeof(unsigned int) /*!< Файловый размер структуры дескриптора лода */

/*! Дескриптор материала модели
*/
struct ModelMatrial
{
	UINT iMat;                   //!< индекс материала
	char szName[MODEL_MAX_NAME]; //!< название материала
};

/*! @} sxgcore_mdl_defs */

#endif

/*! @} */
