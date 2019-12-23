
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxmtrl - библиотеки материалов
*/

/*! \defgroup sxmtrl sxmtrl - библиотека материалов
@{
*/

#ifndef __SXMTRL_H
#define __SXMTRL_H

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#include <gdefines.h>

#include <mtrl/IXMaterialSystem.h>

/*! \name Базовые функции библиотеки
@{*/

//! версия подсистемы
SX_LIB_API long SMtrl_0GetVersion();			

//! установить функцию обработки сообщений
SX_LIB_API void SMtrl_Dbg_Set(report_func rf);

SX_LIB_API void SMtrl_DevSet(IGXDevice *pDev);

//! инициализция подсистемы
SX_LIB_API void SMtrl_0Create(
	const char *szName,				//!< присваиваемое имя
	bool isUnic = true,				//!< должна ли подсистема быть уникальной по имени
	bool isServerMode = false		//!< серверный режим (без графики)
	);

//! уничтожить подсистему
SX_LIB_API void SMtrl_AKill();

//! возвращает статус инициализации библиотеки?
SX_LIB_API bool SMtrl_AisInit();

//!@}

/*! \name Обработка потери/восстановления устройства
@{*/

//! вызывать при потрете устройства
SX_LIB_API void SMtrl_OnLostDevice();

//! вызывать при восстановлении устройства
SX_LIB_API void SMtrl_OnResetDevice();

//!@}

//! обновление данных всей библиотеки
SX_LIB_API void SMtrl_Update(
	DWORD timeDelta	//!< время рендера кадра в млсек
	);

//#############################################################################

/*! \defgroup sxmtrl_mtl Система материалов
 \ingroup sxmtrl
@{*/

/*! максимальная длина имени директории материала */
#define MTL_MAX_SIZE_DIR 64		

/*! максимальная длина имени материала (без директории) */
#define MTL_MAX_SIZE_NAME 64	

/*! максимальное количество обновлений статических отражений, в первый момент рендера */
#define MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC 3	

/*! \name Дистанции при которых будут сниженно или повышено количество отрисовок для отражений
меньше #MTL_REF_UPDATE_L0_DIST - обновление будет каждый кадр
больше #MTL_REF_UPDATE_L2_DIST - обновления не будет
@{*/

#define MTL_REF_UPDATE_L0_DIST 10	
#define MTL_REF_UPDATE_L1_DIST 30	
#define MTL_REF_UPDATE_L2_DIST 70	

//!@}

/*! \name Параметры альфа теста
 В данной реалзиации 2 вида альфа теста, принудительный (#SMtrl_MtlSetForceblyAlphaTest) и назначаемый непосредственно материалом
 \note функция альфа теста D3DCMP_GREATEREQUAL
@{*/

/*! значение свободного альфа теста (назначаемого материалом) */
#define MTL_ALPHATEST_FREE_VALUE 64		

/*! значение принудительного альфа теста */
#define MTL_ALPHATEST_FORCEBLY_VALUE 8	

//!@}

/*! \name Значения параметров освещения по умолчанию
 Данные значения по умолчанию будут применены к материалами которые не удалось загрузить, либо к стандартным материалам
@{*/

//! занчение шероховатости по умолчанию
#define MTL_LIGHTING_DEFAULT_ROUGHNESS 250.f/255.f

//! значение f0 (отражательной способности поверхности) по умолчанию
#define MTL_LIGHTING_DEFAULT_F0 2.f/255.f

//! значение thickness (толщина - на сколько будет использована диффузная составляющая, для твердых поверхностей - 1, для листьев деревьев и травы <1) по умолчанию
#define MTL_LIGHTING_DEFAULT_THICKNESS 255.f/255.f

//!@}

/*! \name Параметры отражений
@{*/

/*! размер текстур с отражениями */
#define MTL_REF_TEX_SIZE 512	

/*! ближняя плоскость отсечения */
#define MTl_REF_PROJ_NEAR 0.25	

/*! дальняя плоскость отсечения, или дистанция на которую будет рендер всего в текстуру отражений */
#define MTl_REF_PROJ_FAR 100	

//!@}

/*! \name Виртуальные директории
 \warning Имя виртуальной директории должно быть единичным символом (сделано для простоты сравнения), 
и как следствие нежелательно использоавть реальную директорию с таким именем для текстур либо материалов, 
и реальные директории не должны начинаться с данных символов
@{*/

//! имя виртуальной директории по умолчанию для "ручных" текстур с параметрами освещения
#define MTL_VIRTUAL_DIR_HAND_MTL '0'

//! имя виртуальной директории по умолчанию для стандартных (встроенных) материалов
#define MTL_VIRTUAL_DIR_STD_MTL "mtrldef"

//!@}

/*! \name Текстурные регистры
Распределение регистров между текстурами которые будут отправляться для формирования данных
@{*/

/*!< основная текстура */
#define MTL_TEX_R_MAIN		0	

/*!< маска */
#define MTL_TEX_R_MASK		1	

/*! стартовый регистр для детальных текстур
 \warning 4 следующих (включая указанный) будует заполнены детальными текстурами, поэтому никакие другие текстуры в [MTL_TEX_R_DETAIL, MTL_TEX_R_DETAIL+3] не должны отправляться
 \note Текстуры для каждого канала будут отправляться последовательно: r, g, b, a -  MTL_TEX_R_DETAIL, MTL_TEX_R_DETAIL + 1, MTL_TEX_R_DETAIL + 2, MTL_TEX_R_DETAIL + 3
*/
#define MTL_TEX_R_DETAIL	2

/*! стартовый регистр для микрорельефных текстур
\warning 4 следующих (включая указанный) будует заполнены микрорельефными текстурами, поэтому никакие другие текстуры в [MTL_TEX_R_MICRO, MTL_TEX_R_MICRO+3] не должны отправляться
\note Текстуры для каждого канала будут отправляться последовательно: r, g, b, a -  MTL_TEX_R_MICRO, MTL_TEX_R_MICRO + 1, MTL_TEX_R_MICRO + 2, MTL_TEX_R_MICRO + 3
*/
#define MTL_TEX_R_MICRO		6


/*! текстура с параметрами освещения */
#define MTL_TEX_R_PARAM_LIGHT	10	

/*! текстура с отражениями */
#define MTL_TEX_R_REFLECTION	12	

/*! текстура с преломлениями */
#define MTL_TEX_R_REFRACTION	13	

/*! текстура с текущей глубиной */
#define MTL_TEX_R_CURR_DEPTH	14	

//!@}

//! типы отражений
enum MTLTYPE_REFLECT
{
	//! нет отражения
	MTLTYPE_REFLECT_NONE,			

	//! плоское
	MTLTYPE_REFLECT_PLANE,			

	//! объемное динамическое
	MTLTYPE_REFLECT_CUBE_DYNAMIC,	

	//! объемное статическое, обработка идет первые несколько кадров, затем не обновляется
	MTLTYPE_REFLECT_CUBE_STATIC		
};

//! сорт материала
enum MTLSORT
{
	//! непрозрачный
	MTLSORT_OPAQUE = 0x00000001,

	//! прозрачный
	MTLTYPE_TRANSPARENCY = 0x00000010,

	//! непрозрачный
	MTLSORT_LIGHTED = 0x00000100,

	//! прозрачный
	MTLTYPE_UNLIT = 0x00001000,

	//! непрозрачный освещаемый
	/*MTLSORT_OPAQUE_LIGHTED,

	//! непрозрачный не освещаемый
	MTLSORT_OPAQUE_UNLIT,*/


	//! полупрозрачный освещаемый
	/*MTLTYPE_TRANSPARENCY_LIGHTED,	

	//! полупрозрачный не освещаемый
	MTLTYPE_TRANSPARENCY_UNLIT,*/
};

/*! \name Возможные слои
@{*/

//! прозрачные неосвещаемые
#define MTLTYPE_UNLIT	0

//! непрозрачные освещаемые
#define MTLTYPE_LIGHT	1

//!@}


//! данные отправляемые в шейдеры
enum MTL_SHADERSTD
{
	//! мировая матрица (трансформации модели), world
	MTL_SHADERSTD_MATRIX_WORLD = 0,		

	//! матрица вида, view
	MTL_SHADERSTD_MATRIX_VIEW,			

	//! матрица проекции, projection
	MTL_SHADERSTD_MATRIX_PROJECTION,	

	//! world * view
	MTL_SHADERSTD_MATRIX_WORLDVIEW,		

	//! world * view * projection
	MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ,	

	//! позиция камеры/наблюдателя
	MTL_SHADERSTD_CAMPOS,				

	//! данные о времени float2(CountTimeRender,TimeDelta)
	MTL_SHADERSTD_TIMEDELTA,			

	//! размеры окна рендера
	MTL_SHADERSTD_WINSIZE,				

	//! пользовательские данные (float4)
	MTL_SHADERSTD_USERDATA				
};

/*! \name Загрузка/сохранение
 \note Файл загрузки/сохранения представлен простым конфигурационным файлом (ini) который можно обрабатывать посредством #ISXLConfig
 \todo Расписать организацию конфигурационного файла загрузки/сохранения для материала
@{*/

/*! загрузка материала, возвращает уникальный идентификатора материала, 
 \note все материалы оборачиваются в специальную структуру, поэтому дубликатов среди идентификаторов материалов нет
 \note любой повторно загружаемый материал не загружается, а лишь дублирует обертку, со ссылкой на внутренности оберкти
 */

SX_LIB_API ID SMtrl_MtlGetId(
	const char *szName								//!< имя_материала.расширение
	);

SX_LIB_API ID SMtrl_MtlLoad2(
	const char *szName,								//!< имя_материала.расширение
	XSHADER_DEFAULT_DESC *pDefaultShaders, 
	UINT uVariantCount, 
	XSHADER_VARIANT_DESC *pVariantsDesc
	);

//! перезагрузка материала
SX_LIB_API void SMtrl_MtlReloadMaterial(
	ID id,					//!< идентификатор материала
	const char *szName = 0	//!< на случай замены существующего материала другим
	);

/*! сохранить материал по id
 \note Сохранение происходит по отноительному пути в директория/директория_имяфайла.mtl
*/
SX_LIB_API void SMtrl_MtlSave(ID id);	

//!@}

/*! очитска, удаляет все загруженные материалы (но место в массиве под них остается), все стандартные остаются
 \warning Отражения не удаляются а помещаются в очередь удаления, так как могут сожеражть в себе идентификаторы, которые необходимы очистить, 
для полной очистки необходимо вызывать функцию #SMtrl_MtlDelRefClear. Однако во избежания утечек памяти необходимо очистить идентификаторы, для их получения вызыва функцию #SMtrl_MtlDelRefGetIDArr
*/
SX_LIB_API void SMtrl_MtlClear(
	bool isClearRefDel	//!< очищать ли очередь удаления отражений, на тот случай если идентификаторы в отражениях больше не действительны
	);	

//! возвращает общее количество материалов
SX_LIB_API long SMtrl_MtlGetCount();					

//! возвращает id дефолтного материала света
SX_LIB_API ID SMtrl_MtlGetLightMtrl();

//! возвращает сорт материала по id
//SX_LIB_API UINT SMtrl_MtlGetSort(ID id);

SX_LIB_API bool SMtrl_MtlIsTransparency(ID id);

//! установка параметров материала по id, вызывается перед DIP
SX_LIB_API void SMtrl_MtlRender(
	ID id,						//!< идентификатор материала
	const float4x4 *pWorld=0,	//!< указатель на мировую матрицу модели, либо 0 - тогда будет принята единичная матрица
	const float4 *pColor=0		//!< указатель на принимаемый цвет
	);

SX_LIB_API void SMtrl_MtlPixelShaderOverride(ID id);
SX_LIB_API void SMtrl_MtlGeometryShaderOverride(ID id);

//! установка параметров материала для рендера источника света
SX_LIB_API void SMtrl_MtlRenderLight(
	const float4_t *pColor,	//!< rgb - цвет, w зарезервирован 
	const float4x4 *pWorld	//!< аналогично #SMtrl_MtlRender
	);

//! установить текстуру из материала id в текстурный слот slot
SX_LIB_API void SMtrl_MtlSetMainTexture(ID idSlot, ID id);	


//! установка физического типа материала
SX_LIB_API void SMtrl_MtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type);	

//! возвращает текущий тип физического материала
SX_LIB_API MTLTYPE_PHYSIC SMtrl_MtlGetPhysicMaterial(ID id);				

/*! \name Управление полупрозрачными поверхностями
Каждый выводимый пиксель помечается номером поверхности к которой он относится
 - 0 - пусто
 - 1 - непрозрачная геометрия
 - все что больше 1 значит полупрозрачная геометрия
 \note Если инкремент установлен то отрисовка каждого материала будет инкрементировать внутренний счетчик, присваивая каждому пикселю поверхности идентификатор поверхности. 
 \note На данный момент максимальное значение полупрозрачных поверхностей может быть только 255 (8 бит)
@{*/

/*! установка инкремента идентификатора поверхностей
 \note К примеру для диференциации (разделения) пикселей при смешивании всех слоев изображения, 
без разделения по принадлжености к определнной поверхности будет трудно/невозможно (пока выхода нет) корректно смешать все слои
*/
SX_LIB_API void SMtrl_MtlSetIsIncrCountSurf(bool bf);

//! установлен ли инкремент поверхностей
SX_LIB_API bool SMtrl_MtlGetIsIncrCountSurf();		

//! обнуление идентификатора отрисованных поверхностей (к примеру при новом кадре)
SX_LIB_API void SMtrl_MtlNullingCurrCountSurf();		

//! текущее количество отрисованных полупрозрачных поверхностей
SX_LIB_API int SMtrl_MtlGetCurrCountSurf();			

//! установить текущее количество отрисованных полупрозрачных поверхностей
SX_LIB_API void SMtrl_MtlSetCurrCountSurf(int iCount);	

/*! принудительное включение альфа теста (isat - true - включить, false - выключить) при устновке параметров материала, 
к примеру для рендера полупрозрачных поверхностей, чтобы отсечь (условно) полностью прозрачные пиксели
*/
SX_LIB_API void SMtrl_MtlSetForceblyAlphaTest(bool isEnable);	

//! включен ли принудительный альфа тест
SX_LIB_API bool SMtrl_MtlGetForceblyAlphaTest();			

//!@}

/*! \defgroup sxmtrl_mtl_reflect Отражения
 \ingroup sxmtrl_mtl
 \note Поддерживается 2 вида отражений:
  - плоские
  - объемные/кубические
@{*/
//{{

//! записать в отражения идентификатор (idArr) для определнной стороны куба/сплита (iCube), соотнести его с идентификатором сущности (idOwner)
SX_LIB_API void SMtrl_RefSetIDArr(
	ID id,		//!< идентификатор материала
	ID idOwner,	//!< идентификатор сущности для которой записываем (геометрия, растительность и прочее)
	int iCube,	//!< условно - сторона куба/сплита для которой записываем 
	ID idArr	//!< идентификатор который надо записать
	);

//! возвращает идентификатор который был записан для сущности (idOwner) для стороны куба/сплита (iCube), если вообще записывался, если не записывался то <0
SX_LIB_API ID SMtrl_RefGetIDArr(
	ID id,		//!< идентификатор материала
	ID idOwner,	//!< идентификатор сущности для которой было записывано (геометрия, растительность и прочее)
	int iCube	//!< условно - сторона куба/сплита для которой было записывано 
	);

/*! \name Управление удаленными отражениями
@{*/

//! возвращает количество удаленных материалов с отражениями
SX_LIB_API int SMtrl_RefDelGetCount();

//! возвращает идентификатор который был записан для сущности (inid) для стороны куба/сплита (cube)
SX_LIB_API ID SMtrl_RefDelGetIDArr(
	ID idKey,	//!< порядковый номер удаленного материала с отражением
	ID idOwner,	//!< идентификатор сущности для которой было записано (геометрия, растительность и прочее)
	int iCube	//!< условно - сторона куба/сплита для которой было записывано
	);

//! очистка массива удаленных материалов с отражениями
SX_LIB_API void SMtrl_RefDelClear();

//!@}

//! обработка статистики рендера для текущего матриала с отражением
SX_LIB_API bool SMtrl_RefUpdateCountUpdate(
	ID id,						//!< идентификатора материала
	const float3_t *pViewPos	//!< позиция наблюдателя, viewpos == 0 вернет false
	);

/*! обнуление статистикки рендера для данного материала с отражением, 
полезно в случаях каких-либо изменений, чтобы эти изменения были видны и в отражениях (актуально для статических отражений)
*/
SX_LIB_API void SMtrl_RefNullingCountUpdate(ID id);

//! установка экстремумов материала
SX_LIB_API void SMtrl_RefSetMinMax(ID id, const float3_t *pMin, const float3_t *pMax);

//! возвращает фрустум отражения, cube - сторона куба
SX_LIB_API const IFrustum* SMtrl_RefGetfrustum(
	ID id,		//!< идентификатора материала
	int iCube	//!< сторона куба, если отражение плоское то необходимо указать в cube передать 0, иначе вернет NULL
	);

SX_LIB_API bool SMtrl_RefIsAllowedRender(ID id);	//!< разрешен ли рендер отражений для данного материала в текущий момент

/*! \name Рендер плоского отражения
 \note Перед началом рендера плоских отражений необходимо в регистры матриц в #G_RI_MATRIX_VIEW поместить матрицу вида наблюдателя
 \note Порядок рендера:
 \code
 SMtrl_MtlRefPreRenderPlane(id, plane);
 //код рендера всего того что необходимо поместить в текстуру отражений
 SMtrl_MtlRefPostRenderPlane(id);
 \endcode
@{*/

//! первоначальные настройки данных для генерации отражения
SX_LIB_API void SMtrl_RefPreRenderPlane(
	ID id,				//!< идентификатор материала
	SMPLANE *pPlane	//!< плоскость относительно которой будет считаться отражение
	);	

//! завершающие операции
SX_LIB_API void SMtrl_RefPostRenderPlane(ID id);			

//! возвращает текстуру с плоским отражением (если оно есть, иначе 0)
SX_LIB_API IGXTexture2D* SMtrl_RefGetTexPlane(ID id);	

//!@}

/*! \name Рендер объемного (кубического) отражения
 \note Порядок рендера:
 \code
 SMtrl_RefCubeBeginRender(id, center);
 SMtrl_RefCubePreRender(id, cube, world);
 //код рендера всего того что необходимо поместить в текстуру отражений
 SMtrl_RefCubePostRender(id, cube);
 SMtrl_RefCubeEndRender(id, viewpos);
 \endcode
@{*/

//! установка общих настроек
SX_LIB_API void SMtrl_RefCubeBeginRender(
	ID id,					//!< идентификатор материала
	const float3_t *pCenter	//!< центр (позиция) подгруппы модели - откуда будет рендер отражений
	);	

//! установка частных настроек для стороны куба (cube)
SX_LIB_API void SMtrl_RefCubePreRender(
	ID id,					//!< идентификатор материала
	int iCube,				//!< сторона куба [0,5]
	const float4x4 *pWorld	//!< мировая матрица модели 
	);

//! завершающие частные моменты для стороны куба
SX_LIB_API void SMtrl_RefCubePostRender(ID id, int iCube);	

/*! завершающие общие моменты
 \note если viewpos == 0 обновления статисктики рендера для текущего материала с отражением не произойдет, 
и дабы рационально обрабатывать и статические и динамические отражения необходимо до следующего рендера отражений вызвать SMtrl_MtlRefUpdateCountUpdate
*/
SX_LIB_API void SMtrl_RefCubeEndRender(
	ID id,						//!< идентификатор материала
	const float3_t *pViewPos	//!< позиция наблюдателя, необходимо для обработки разрешения на следующий рендер
	);	

//! возвращает cube текстуру отражением (если оно есть, иначе 0)
SX_LIB_API IGXTextureCube* SMtrl_RefCubeGetTex(ID id);	

//!@}

//!@} sxmtrl_mtl_reflect


/*! \defgroup sxmtrl_mtl_propslayer Cвойства поверхности
 \ingroup sxmtrl_mtl
@{*/

//! установка основной текстуры (загрузка текстуры по имени)
SX_LIB_API void SMtrl_MtlSetTexture(ID id, const char *szPathTex);	

//! в name записывает имя текстуры
SX_LIB_API void SMtrl_MtlGetTexture(ID id, char *szName);			

//! возвращает id текстуры материала
SX_LIB_API ID SMtrl_MtlGetTextureID(ID id);		


//! установка свойства окрашивания в принимаемый цвет
SX_LIB_API void SMtrl_MtlSetUseDestColor(ID id, bool useDestColor);

//! возвращает значение свойства окрашивания в принимаемый цвет
SX_LIB_API bool SMtrl_MtlGetUseDestColor(ID id);


/*! \name Шейдеры для рендера материала
 \warning В текущей версии все шейдеры загружаются в версии 3.0 и требуют в качестве старта указывать функцию main
 \warning Рекомендуется устанавливать и вершинный и пиксельный шейдеры, так как во врем тестов одно из устройств требовало наличия обоих шейдеров либо отсутствия обоих
 \note В аргументах path_vs/path_ps указывается только имя шейдера с расширением, есесно возвращается тоже самое
@{*/

//! установка вершинного шейдера
SX_LIB_API void SMtrl_MtlSetVS(ID id, const char *szPathVS);

//! в name записывает имя текущего вершинного шейдера
SX_LIB_API void SMtrl_MtlGetVS(ID id, char *szName);

//! Получает ID вершинного шейдера
SX_LIB_API ID SMtrl_MtlGetVSID(ID id);

//! установка пиксельного шейдера
SX_LIB_API void SMtrl_MtlSetPS(ID id, const char *szPathPS);

//! в name записывает имя текущего пикельного шейдера
SX_LIB_API void SMtrl_MtlGetPS(ID id, char *szName);

//! Получает ID пикельного шейдера
SX_LIB_API ID SMtrl_MtlGetPSID(ID id);

//!@}

//! возвращает коэффициент пробиваемости [0, ], чем больше тем сложнее пробить
SX_LIB_API float SMtrl_MtlGetDurability(ID id);

//! устанавливает коэффициент пробиваемости [0, ], чем больше тем сложнее пробить
SX_LIB_API void SMtrl_MtlSetDurability(ID id, float fDurability);


//! возвращает шанс пробиваемости [0 - пуля летит насквозь, 1 - пуля ударяется]
SX_LIB_API float SMtrl_MtlGetHitChance(ID id);

//! устанавливает шанс пробиваемости [0 - пуля летит насквозь, 1 - пуля ударяется]
SX_LIB_API void SMtrl_MtlSetHitChance(ID id, float fHitChance);


//! возвращает плотность материала кг/м3
SX_LIB_API float SMtrl_MtlGetDensity(ID id);

//! устанавливает плотность материала кг/м3
SX_LIB_API void SMtrl_MtlSetDensity(ID id, float fDensity);


//! используется ли альфа тест для материала
SX_LIB_API bool SMtrl_MtlGetUsingAlphaTest(ID id);				

//! установка использования альфа теста
SX_LIB_API void SMtrl_MtlSetUsingAlphaTest(ID id, bool isUsing);	

/*! освещение материала id
 \note если материал не освещается то будет выведен в цвете накладываемым материалом
*/
SX_LIB_API void SMtrl_MtlSetLighting(ID id, bool isLighting);	

//! освещается ли материал
SX_LIB_API bool SMtrl_MtlGetLighting(ID id);	

/*! \name Параметры освещения.
 \note Для более точной/тонкой настройки освещения материала рекомендуется использовать текстуру с параметрами освещения 
(к прмиеру когда в одной поверхности разные материалы, к примеру дерево, метал и камень, то в текстуре можно указать параметры освещения для каждого пикселя), 
однако если текстуры нет то будут использованы установленные парметры освещения для всей поверхности
@{*/

//! установка использования текстуры с параметрами освещения
SX_LIB_API void SMtrl_MtlSetIsTextureLighting(ID id, bool isTexLighting);

//! используется ли текстура с параметрами для освещения
SX_LIB_API bool SMtrl_MtlGetIsTextureLighting(ID id);	


//! загрузка текстуры с парамтерами освещения
SX_LIB_API void SMtrl_MtlSetTextureLighting(ID id, const char *szPathTex);	

//! в path_tex записывает имя текстуры с параметрами освещения
SX_LIB_API void SMtrl_MtlGetTextureLighting(ID id, char *szPathTex);		


//! установка шероховатости (0-1)
SX_LIB_API void SMtrl_MtlSetRoughness(ID id, float fRoughness);	

//! возвращает текущее значение шероховатости поверхности
SX_LIB_API float SMtrl_MtlGetRoughness(ID id);					


//! установка просвечиваемости (толщины для графики (0, 1)), 0 - не принимает затенение, 1 - полностью принимает затенение
SX_LIB_API void SMtrl_MtlSetThickness(ID id, float fThickness);	

//! возвращает просвечиваемости (толщины для графики (0, 1)), 0 - не принимает затенение, 1 - полностью принимает затенение
SX_LIB_API float SMtrl_MtlGetThickness(ID id);					

//! установка отражательной способности поверхности (0-1)
SX_LIB_API void SMtrl_MtlSetF0(ID id, float fF0);	

//! возвращает текущее значение отражательной способности поверхности
SX_LIB_API float SMtrl_MtlGetF0(ID id);			

//!@}

/*! \name Полупрозрачность
Под полупрозрачными материалами следует понимать такие материалы, прозрачность которых в среднем находится в пределах [0.5, 1) и минимальный процент полупрозрачного периметра 50%
@{*/

//! установка свойства полупрозрачности
SX_LIB_API void SMtrl_MtlSetTransparency(ID id, bool isTransparent);

//! возвращает текущее значение свойства полупрозрачности для материала
SX_LIB_API bool SMtrl_MtlGetTransparency(ID id);
SX_LIB_API bool SMtrl_MtlGetRefractivity(ID id);

//!@}

/*! \name Отражения окружения, типы из #MtlTypeReflect
@{*/

//! установка типа отражений
SX_LIB_API void SMtrl_MtlSetTypeReflection(ID id, MTLTYPE_REFLECT type);	

//!@}

/*! \name Детальность и микрорельеф
Поверхность может содержать до 4 детальных текстур и до 4 текстур с микрорельефом. 
Данные для определения наложения текстур (детальных и микрорельефных) указываются в маске - 
на каждый канал текстуры маски по одной детальной и микрорельефной текстуре, 
то есть канал маски определет связанные между собой и детальную и микрорельефную текстуры
 \note В аргументах channel - RGBA - 0,1,2,3
@{*/

//! загрузка маски наложения
SX_LIB_API void SMtrl_MtlSetMaskTex(ID id, const char *szPathTex);	

//! в path_tex записывает текущее имя маски наложения
SX_LIB_API void SMtrl_MtlGetMaskTex(ID id, char *szPathTex);		

//! загрузка текстуры микрорельефа, на каждый канал
SX_LIB_API void SMtrl_MtlSetMRTex(
	ID id,					//!< идентификатор материала
	int iChannel,			//!< номер канала RGBA - 0,1,2,3
	const char *szPathTex	//!< имя текстуры с расширением
	);

//! в path_tex запишет текущее имя текстуры микрорельефа для канала channel
SX_LIB_API void SMtrl_MtlGetMRTex(ID id, int iChannel, char *szPathTex);

//! загрузка детальной текстуры, на каждый канал
SX_LIB_API void SMtrl_MtlSetDTex(
	ID id,					//!< идентификатор материала
	int iChannel,			//!< номер канала RGBA - 0,1,2,3
	const char *szPathTex	//!< имя текстуры с расширением
	);

//! в path_tex запишет текущее имя детальной текстуры для канала channel
SX_LIB_API void SMtrl_MtlGetDTex(ID id, int iChannel, char *szPathTex);

//!@}

/*! \name Стандартные параметры для отправки в шейдер (в вершинный и пиксельный раздельно), данные из #MtlTransShaderStd
@{*/

//! отправка стадартных данных в вершинный шейдер
SX_LIB_API void SMtrl_MtlSetStdVS(
	ID id,				//!< идентификатор материала
	MTL_SHADERSTD type,	//!< тип значения которое нужно отправить, одно из значений #MtlTransShaderStd
	bool isSend			//!< true - отправлять, false - не отправлять
	);

//! установлена ли отпрвка значения type в вершинный шейдер
SX_LIB_API bool SMtrl_MtlGetStdVS(ID id, MTL_SHADERSTD type);

//! отправка стадартных данных в пиксельный шейдер
SX_LIB_API void SMtrl_MtlSetStdPS(
	ID id,				//!< идентификатор материала
	MTL_SHADERSTD type,	//!< тип значения которое нужно отправить, одно из значений #MtlTransShaderStd
	bool isSend			//!< true - отправлять, false - не отправлять
	);

//! установлена ли отпрвка значения type в пиксельный шейдер
SX_LIB_API bool SMtrl_MtlGetStdPS(ID id, MTL_SHADERSTD type);

//!@}

/*! \name Дополнительные пользовательские данные в виде float4 (предел [0-1] для стандарта, иное вне стандарта, но допустимо)
 \note Данные для вершинного и пиксельного шейдера раздельны. Отправляется float4.
Однако данные и из пиксельного могут быть отправленны в вершинный и наоборот.
В итоге в шейдере может быть 2 float4 пользовательских вектора с данными.
Но если вектор определен для пиксельного то он все-равно будет отправлен в пиксельный шейдер, даже если и придназначалася для вершинного.
 \note В аргументах component - xyzw - 0,1,2,3
@{*/

//! установка значения компоненты вектора для вершинного шейдера
SX_LIB_API void SMtrl_MtlSetUserDataVS(
	ID id,			//!< идентификатор материала
	int iComponent,	//!< компонента вектора xyzw - 0,1,2,3
	float fValue	//!< значение компоненты
	);

//! возвращает значение компоненты вектора, который отправляется в вершинный шейдер
SX_LIB_API float SMtrl_MtlGetUserDataVS(ID id, int iComponent);

//! установка отправки float4 вектора, предназначенного для вершинного шейдера, в пиксельный шейдер
SX_LIB_API void SMtrl_MtlSetUserDataVS_InPS(ID id, bool isSendPs);

//! отправляется ли float4 вектор, предназначенный для вершинного шейдера, в пиксельный шейдер
SX_LIB_API bool SMtrl_MtlGetUserDataVS_InPS(ID id);


//! установка значения компоненты вектора для пиксельного шейдера
SX_LIB_API void SMtrl_MtlSetUserDataPS(
	ID id,			//!< идентификатор материала
	int iComponent,	//!< компонента вектора xyzw - 0,1,2,3
	float fValue	//!< значение компоненты
	);

//! возвращает значение компоненты вектора, который отправляется в пиксельный шейдер
SX_LIB_API float SMtrl_MtlGetUserDataPS(ID id, int iComponent);

//! установка отправки float4 вектора, предназначенного для пиксельного шейдера, в вершинный шейдер
SX_LIB_API void SMtrl_MtlSetUserDataPS_InVS(ID id, bool isSendVs);

//! отправляется ли float4 вектор, предназначенный для пиксельного шейдера, в вершинный шейдер
SX_LIB_API bool SMtrl_MtlGetUserDataPS_InVS(ID id);

//!@}

//!@} sxmtrl_mtl_propslayer

//!@} sxmtrl_mtl

#endif

//!@} sxmtrl
