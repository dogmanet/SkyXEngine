
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxmtllight - библиотеки освещения и материалов
*/

/*! \defgroup sxmtllight sxmtllight - библиотека освещения и материалов
@{
*/

#ifndef __SXMTLLIGHT_H
#define __SXMTLLIGHT_H

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

/*! \name Базовые функции библиотеки
@{*/

SX_LIB_API long SML_0GetVersion();			//!< версия подсистемы
SX_LIB_API void SML_Dbg_Set(report_func rf);//!< установить функцию обработки сообщений

//! инициализция подсистемы
SX_LIB_API void SML_0Create(
	const char* name,				//!< присваиваемое имя
	bool is_unic = true				//!< должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SML_AKill();	//!< уничтожить подсистему

//!@}

/*! \name Обработка потери/восстановления устройства
@{*/

SX_LIB_API void SML_OnLostDevice();	//!< вызывать при потрете устройства

//! вызывать при восстановлении устройства, с передачей параметров области в которую рисуем
SX_LIB_API void SML_OnResetDevice();

//!@}

//! обновление данных всей библиотеки
SX_LIB_API void SML_Update(
	DWORD timeDelta	//!< время рендера кадра в млсек
	);

//#############################################################################

/*! \defgroup sxmtllight_light Освещение
 \ingroup sxmtllight
 \warning Обращение (идентификация) конкретного света осуществляется посредством идентификатора который выдают создающие функции:
  - #SML_LigthsCreatePoint
  - #SML_LigthsCreateDirection
 Получить идентификатор уже созданного света можно, посредством ключа (key) который лежит в пределах [0, #SML_LigthsGetCount),
передав этот ключ #SML_LigthsGetIDOfKey, которая в случае успеха вернет идентификатор света.
@{*/

#define LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC 3	/*!< максимальное количество обновлений статических источников света, в первый момент рендера */

/*! \name Дистанции (метрах) при которых будет сниженно или повышено количество отрисовок для локальных источников света
меньше LIGHTS_UPDATE_L0_DIST - обновление будет каждый кадр\n
больше LIGHTS_UPDATE_L2_DIST - обновления не будет
@{*/

#define LIGHTS_UPDATE_L0_DIST 10	/**/
#define LIGHTS_UPDATE_L1_DIST 30	/**/
#define LIGHTS_UPDATE_L2_DIST 70	/**/

//!@}

/*! Коэфициент обновления сплитов глобального света.
Cчитается: LIGHTS_UPDATE_PSSM_SPLIT*number_split
 \note Первый сплит обновляется каждый кадр, второй через кадр
*/
#define LIGHTS_UPDATE_PSSM_SPLIT 1		

#define LIGHTS_UPDATE_PSSM_SPLIT_5 2	/*!< аналогичнен #LIGHTS_UPDATE_PSSM_SPLIT только для 5 сплита (для теней от облаков) */

#define LIGHTS_LOCAL_STD_TOP_RADIUS 0.1	/*!< стандартное значение верхнего/начального радиуса для направленного света  */
#define LIGHTS_LOCAL_STD_NEAR 0.1		/*!< стандартное значение ближней плоскости отсечения для теней локального света  */

#define LIGHTS_LOCAL_MAX_DIST 200.f		/*!< максимальная дистанция локального света */

#define LIGHTS_GLOBAL_STD_RADIUS 999999	/*!< радиус глобального источника */
#define LIGHTS_POS_G_MAX 1000	/*!< отдаленность глобального света (нужно для корректности теней) */

#define LIGHTS_DIR_BASE float3(0, -1, 0) /*!< базовое направление направленноого источника света */

//! типы источников света
enum LTYPE_LIGHT
{
	LTYPE_LIGHT_NONE = -1,	//!< не установленный
	LTYPE_LIGHT_GLOBAL,		//!< глобальный
	LTYPE_LIGHT_POINT,		//!< точечный
	LTYPE_LIGHT_DIR			//!< направленный
};

//! типы теней источников света
enum LTYPE_SHADOW
{
	LTYPE_SHADOW_NONE = -1,	//!< без теней
	LTYPE_SHADOW_STATIC,	//!< статичные тени, тень обновляется первые несколько кадров (для корректности), далее не обновляется
	LTYPE_SHADOW_DYNAMIC,	//!< полностью динамические тени
};

/*! \name Настройки размеров для теней
 \note Коэфициента относительно размеров окна рендера
@{*/
//{
//
SX_LIB_API void SML_LigthsSettLCoefSizeDepth(float coef);	//!< установка коэфициента для локального света
SX_LIB_API void SML_LigthsSettGCoefSizeDepth(float coef);	//!< установка коэфициента для глобального света

SX_LIB_API float SML_LigthsGettLCoefSizeDepth();	//!< возвращение коэфициента для локальный свет
SX_LIB_API float SML_LigthsGettGCoefSizeDepth();	//!< возвращение коэфициента для глобальный свет

//!@}

SX_LIB_API void SML_LigthsClear();		//!< очистить список света
SX_LIB_API void SML_LigthsClearIDArr();	//!< очистка всех идентификаторов просчетов во всех источниках света
SX_LIB_API long SML_LigthsGetCount();	//!< общее количество света

SX_LIB_API bool SML_LigthsGetExists(ID id);

//! создать точечный свет (светит во все стороны)
SX_LIB_API ID SML_LigthsCreatePoint(
	const float3* center,	//!< центр света (мировая)
	float dist,				//!< дистанция на которую распространяется свет
	const float3* color,	//!< цвет в пределах 0-1
	bool isglobal,			//!< глобальный ли свет (типа солнца)
	bool is_shadowed		//!< отбрасвыает ли тени свет
	);

//! создать направленный свет
SX_LIB_API ID SML_LigthsCreateDirection(
	const float3* pos,			//!< позиция света (мировая)
	float dist,					//!< дистанция на которую распространяется свет
	const float3* color,		//!< цвет
	const SMQuaternion* orient,	//!< ориентация
	float top_radius,			//!< верхний (начальный) радиус
	float angle,				//!< угол
	bool is_shadow				//!< отбрасвыает ли тени свет
	);

SX_LIB_API LTYPE_LIGHT SML_LigthsGetType(ID id);	//!< возвращает тип света

SX_LIB_API ID SML_LigthsCreateCopy(ID id);		//!< создать копию объекта и вернуть на нее id
SX_LIB_API void SML_LigthsDeleteLight(ID id);	//!< удаление света

SX_LIB_API char* SML_LigthsGetName(ID id);					//!< возвращает имя света
SX_LIB_API void SML_LigthsSetName(ID id, const char* name);	//!< устанавливает имя света

/*! отрисовка ограничивающего объема */
SX_LIB_API void SML_LigthsRender(ID id, DWORD timeDelta);

SX_LIB_API void SML_LigthsGetColor(ID id, float3* color);		//!< в color записывает цвет света
SX_LIB_API void SML_LigthsSetColor(ID id, const float3* color);	//!< устанавливает цвет света

//! получить в pos позицию света
SX_LIB_API void SML_LigthsGetPos(
	ID id,				//!< идентификатор света
	float3* pos,		//!< инициализированный вектор в который будет записана позиция
	bool greal = false	//!< true - нужны реальные координаты, false - нужны углы, только для глобального света, во всех остальных случаях не имеет смысла
	);

//! установить позицию свету
SX_LIB_API void SML_LigthsSetPos(
	ID id,				//!< идентификатор света
	const float3* pos,	//!< позиция в которую будет перемещен свет
	bool greal = false	//!< true - установка реальных координат, false - установка углов, только для глобального света, во всех остальных случаях не имеет смысла
	);

SX_LIB_API float SML_LigthsGetPower(ID id);	//!< возвращает "значение силы света" для отправки в шейдер (максимальная компонента цвет * дистанция покрытия/LIGHTS_LOCAL_MAX_DIST)

SX_LIB_API float SML_LigthsGetDist(ID id);		//!< возвращает дистанцию на которую распространяется свет

//! установка новой дистанции
SX_LIB_API void SML_LigthsSetDist(
	ID id,					//!< идентификатор света
	float dist,				//!< новая дистанция(радиус для точечного и высота для направленного)
	bool is_create = true	//!< нужно ли пересоздание объема? (если новая дистанция больше чем текущий ограничивающий объем то надо, в других случаях по усмотрению)
	);

//! записывает в q ориентацию света
SX_LIB_API void SML_LigthsGetOrient(ID id, SMQuaternion* q);

//! установка ориентации света
SX_LIB_API void SML_LigthsSetOrient(ID id, const SMQuaternion* q);


//! включен ли свет с идентификатором id
SX_LIB_API bool SML_LigthsGetEnable(ID id);				

//! установить всключен (true) или отключен (false) свет  с идентификатором id
SX_LIB_API void SML_LigthsSetEnable(ID id, bool val);	

//! возвращает идентификатор глобального источника света, если он есть, иначе -1
SX_LIB_API ID SML_LigthsGetGlobal();					

//! возвращает установлена ли общая тень от глобального источника (все ли в тени)
SX_LIB_API bool SML_LigthsGetCastGlobalShadow();

//! установить общую тень для всего от глобального источника (все в тень)
SX_LIB_API void SML_LigthsSetCastGlobalShadow(bool isShadowed);

//-----------------------------------------------------------------------------

/*! \name Управление видимостью
 \note Отдельный просчет - считает индивидуально для света и тут же возвращает результат просчетов
 \note Предварительный просчет - просчет для всего света (к примеру в другом потоке)
@{*/

SX_LIB_API bool SML_LigthsComVisibleForFrustum(ID id, const const ISXFrustum *pFrustum);	//!< виден ли свет в данном фрустуме frustum (отдельный просчет)
SX_LIB_API float SML_LigthsComDistFor(ID id, const float3 *pViewPos);				//!< дистанция от viewpos до начала света (отдельный просчет)

//! просчет видимости и расстояния от наблюдателя viewpos до начала света, для каждого света (предварительный просчет)
SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(const ISXFrustum *pFrustum, const float3 *pViewPos);
SX_LIB_API bool SML_LigthsGetVisibleForFrustum(ID id);	//!< виден ли свет в просчитанном фрустуме
SX_LIB_API float SML_LigthsGetDistFor(ID id);			//!< просчитанная дистанция от позиции наблюдателя до начала света

//!@}

//-----------------------------------------------------------------------------

/*! \name Настройки для направленного света
@{*/
//угол для направленного источника
SX_LIB_API float SML_LigthsGetAngle(ID id);				//!< возвращает угол, актуально только для направленного и света
SX_LIB_API void SML_LigthsSetAngle(ID id, float fAngle);	//!< установить угол, актуально только для направленного и света

//верхний радиус для направленного источника
SX_LIB_API float SML_LigthsGetTopRadius(ID id);					//!< возвращает верхний радиус, актуально только для направленного и света
SX_LIB_API void SML_LigthsSetTopRadius(ID id, float fTopRadius);//!< устанавливает верхний радиус, актуально только для направленного и света

//!@}

//-----------------------------------------------------------------------------

/*! \name Настройки для точечного света
@{*/

/*! установить "рисовать ли в указанную грань cubemap тени"
 \warning Актуально только когда свет может отбрасывать тени
 */
SX_LIB_API void SML_LigthsSetCubeEdgeEnable(
	ID id,			//!< идентификатор света
	int iEdge,		//!< номер стороны/грани cubemap [0,5]
	bool iEnable		//!< true - рисовать, false - не рисовать
	);

//! получить текущее значение "рисовать ли в указанную грань cubemap тени"
SX_LIB_API bool SML_LigthsGetCubeEdgeEnable(
	ID id,		//!< идентификатор света
	int iEdge	//!< номер стороны/грани cubemap [0,5]
	);

//!@}

//-----------------------------------------------------------------------------

/*! \defgroup sxmtllight_light_shadow Управление тенями
 \ingroup sxmtllight_light
 \note Для генерации теней используется один render target full screen
 \note Для сглаживания используется еще один аналогичный render target и первый render target. При сглаживании внутри меняется только счетчик, который меняет их местами
 \note Так как используется всего один render target для генерации теней то есесно в текущий момент времени можно сгенерировать и сгладить тень только для одного света
 \note Для теней от глобального света используется техника PSSM (Parallel-Split Shadow Mapping) на 3/4 сплита для объектов уровня (3 или 4 сплита можно настроить #SML_LigthsSet4Or3SplitsG)
 \note Для теней от локального света используется техника Shadow Mapping в двух вариациях:
  - для точечного света используется cubemap
  - для направленного света - обычная техника Shadow Mapping
@{*/

SX_LIB_API bool SML_LigthsGetShadowed(ID id);						//!< отбрасывает ли свет тени

SX_LIB_API bool SML_LigthsGet4Or3SplitsG(ID id);				//!< возвращает true В случае если установлено 4 сплита для PSSM, иначе (установлено 3 сплита) false
SX_LIB_API void SML_LigthsSet4Or3SplitsG(ID id, bool is4or3);	//!< установка количества сплитов (false - 3, true - 4) для PSSM

/*! устанавливает шейдеры и передает им данные на основании типа материала, поддерживаются все типы MTL_TYPE_*/
SX_LIB_API void SML_LigthsShadowSetShaderOfTypeMat(ID id, int typemat, float4x4 *pWorldMat);

/*! \name Управление обновлениями теней
 \note Так как основная задумка это real-time рендер то для этого нужны упрощения, одним из таковых является обновление теней от света, которое может происходить не каждый кадр
@{*/

//! установка динамики обработки теней
SX_LIB_API void SML_LigthsSetTypeShadowed(
	ID id,					//!< идентификатор света
	LTYPE_SHADOW type	//!< значение из перечисления #LightsTypeShadow
	);

SX_LIB_API LTYPE_SHADOW SML_LigthsGetTypeShadowed(ID id);	//!< возвращает тип динамики теней (одно из значений #LightsTypeShadow)

//! производит обработку данных затем возвращает разрешено ли обновлять тени, увеличивает внутрений счетчик, поэтому необходимо вызывать только когда надо
SX_LIB_API bool SML_LigthsCountUpdateUpdate(
	ID id,					//!< идентификатор света
	const float3 *pViewPos,	//!< позиция камеры для которой обновляем, на основании расстояния будет произведен расчет обновления
	int iHow = -1			//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! разрешено ли обновлять тени, в отличии от #SML_LigthsUpdateCountUpdate не увеличивает внутренний счетчик
SX_LIB_API bool SML_LigthsCountUpdateAllowed(
	ID id,			//!< идентификатор света
	int iHow = -1	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

SX_LIB_API void SML_LigthsCountUpdateNull(ID id);	//!< сбросить счетчик обновлений и обновлять до предела тени

//!@}


/*! \name Управление циклами рендера в карты глубин
 Для локального направленного света:
 \code
 SML_LigthsShadowRenderBegin(id);
 //рендер того что должно отбросить тени
 SML_LigthsShadowRenderEnd(id);
 \endcode
 Для глобального и точечного:
 \code
 SML_LigthsShadowRenderBegin(id);
 SML_LigthsShadowRenderPre(id, cube);
 //рендер того что должно отбросить тени
 SML_LigthsShadowRenderPost(id, cube);
 SML_LigthsShadowRenderEnd(id);
 \endcode
 Где cube для точечного это номер стороны cubemap [0-5] в которую будет осуществлен рендер, а для глобального это номер сплита.
@{*/

//
SX_LIB_API void SML_LigthsShadowRenderBegin(ID id);	//!< подготовительные процедуры
SX_LIB_API void SML_LigthsShadowRenderEnd(ID id);	//!< завершающие процедуры

//! подготовительные процедур перед рендером для сплита/стороны
SX_LIB_API void SML_LigthsShadowRenderPre(
	ID id,		//!< идентификатор света
	int iCube	//!< для точечного света - сторона cubemap [0-5] для глобального света номер сплита
	);	

//! завершающие процедур перед рендером для сплита/стороны
SX_LIB_API void SML_LigthsShadowRenderPost(
	ID id,		//!< идентификатор света
	int iCube	//!< для точечного света - сторона cubemap [0-5] для глобального света номер сплита
	);

//!@}

SX_LIB_API void SML_LigthsShadowNull();			//!< обнуление карты теней, необходимо вызывать перед генерацией теней посредством функции #SML_LigthsGenShadow
SX_LIB_API void SML_LigthsShadowGen(ID id);		//!< генерация тени на основе уже отработанной информации о затенении, перед вызовов этой функции необходимо вызывать #SML_LigthsNullingShadow
SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow();//!< возвращает текстуру с тенями (если вызывались функции сглаживания) то вренутся сглаженная

//! сглаживание текущей тени
SX_LIB_API void SML_LigthsShadowSoft(
	bool isRandSam,			//!< использовать ли смещение по случайному направлению
	float fSize,			//!< коэфициент увеличения размера пикселя для размытия
	bool isFirst = false	//!< впервые ли вызывается размытие для текущей тени, каждый раз когда тень еще не сглажена но сгенерирована нужно указывать false
	);

//! возвращает указатель фрустума света
SX_LIB_API ISXFrustum* SML_LigthsGetFrustum(
	ID id,	//!< идентификатор света
	int iHow	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! обновление фрустума глобального света
SX_LIB_API void SML_LigthsUpdateGFrustums(
	ID id,		//!< идентификатор света
	int iSplit,	//!< номер сплита
	const float3 *pPos,	//!< позиция наблюдателя
	const float3 *pDir	//!< направление взгляда наблюдателя
	);

/*! \name Управление настройками для генерации глубины/тени
@{*/

SX_LIB_API float SML_LigthsGetShadowBias(ID id);				//!< получить текущее смещение (bias) при генерации глубины
SX_LIB_API void SML_LigthsSetShadowBias(ID id, float fBias);	//!< установить текущее смещение (bias) при генерации глубины

SX_LIB_API void SML_LigthsSetShadowBlurPixel(ID id, float fBlurPixel);	//!< установить размерность размытия (в пикселя) при геренации тени
SX_LIB_API float SML_LigthsGetShadowBlurPixel(ID id);						//!< получить текущее значение размерности (в пикселях) при генерации тени

SX_LIB_API void SML_LigthsSetShadowLocalNear(ID id, float fNear);	//!< установить значение ближней плоскостью отсечения для теней (локальный свет)
SX_LIB_API float SML_LigthsGetShadowLocalNear(ID id);				//!< получить текущее значение ближней плоскостью отсечения для теней (локальный свет)

/*! установка дальней плоскостью отсечения для теней (локальный свет)
 \note Иногда возникают артефакты и если отрегулировать это значение то все становится на свои места, тесты показали что в основном при ошибках, увеличение этого значения решало проблемы
*/
SX_LIB_API void SML_LigthsSetShadowLocalFar(
	ID id,		//!< идентификатор света
	float fFar	//!< значение дальней плоскости отсечения (дальность видимости теней), обычно это значение = дальность света
	);

SX_LIB_API float SML_LigthsGetShadowLocalFar(ID id);	//!< возвращает значение дальней плоскости отсечения (дальности видимости теней)

//!@}

//-----------------------------------------------------------------------------

/*! \name Управление массивами обработки данных для рендера (к примеру от геометрии, растительности и прочего)
 Cделано для того чтобы отделить просчеты видимости (если они нужны) от рендера. 
 К примеру можно рассчитать видимости в одном потоке а рисовать в основном.
@{
*/

//! возвращает идентификатор в случае успеха, иначе <0
SX_LIB_API ID SML_LigthsGetIDArr(
	ID id,		//!< идентификатор света
	ID inid,	//!< идентификатор объекта/системы вообще того что выдало идентификатор массива
	int iHow		//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! установить идентификатор
SX_LIB_API void SML_LigthsSetIDArr(
	ID id,		//!< идентификатор света
	ID inid,	//!< идентификатор объекта/системы вообще того что выдало идентификатор массива
	int iHow,	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	ID idArr	//!< номер массива обработки от объекта который надо будет обрабатывать на видимость
	);

//!@}

//!@} sxmtllight_light_shadow

//! обработка tone mapping
SX_LIB_API void SML_LigthsToneMappingCom(
	DWORD timeDelta,			//!< время рендера кадра в млсек
	float fFactorAdapted = 0.3	//!< на сколько быстро "глаз" привыкнет к освещению (0,1] (медленно, быстро]
	);

/*! \name Функции обработки удаленного света
 При удалении света он помещается в специальный массив удаленных объектов, 
это связано с тем что свет может содержать в себе данные которые требуют очистки памяти вне данной реализации, 
к примеру идентификаторы массивов просчетов видимости. \n
 Поэтому необходимо вручную удалять (уже после удаленяи функцией #SML_LigthsDeleteLight) удаленные объекты света.
 \note Доступ к элементам массива удаленных объектов света осуществляется посредством порядкового номера в массиве (ключа key), то есть можно пройтись по массиву циклом.
@{*/

SX_LIB_API int SML_LigthsDelGetCount();						//!< возвращает количество удаленных объектов света которые требуют удаления
SX_LIB_API LTYPE_LIGHT SML_LigthsDelGetType(ID key);		//!< возвращает тип удаленного объекта света, значения из #LightsTypeLight
SX_LIB_API void SML_LigthsDelDel(ID key);						//!< удаляет (окончательно) удаленный объект света

//! возвращает идентификатор, если был передан, иначе <0
SX_LIB_API ID SML_LigthsDelGetIDArr(
	ID key,	//!< порядковый номер объекта света в массиве удаленного света
	ID inid,//!< идентификатор того что выдало идентификатор массива
	int iHow	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//!@}

//!@} sxmtllight_light

//#############################################################################

/*! \defgroup sxmtllight_ds Render targets для deferred shading
 \ingroup sxmtllight
@{*/

//! типы render targets
enum DS_RT
{
	DS_RT_COLOR = 0,		//!< цвет rgba8
	DS_RT_NORMAL,			//!< нормали (включая микрорельеф) rfb10a2
	DS_RT_PARAM,			//!< параметры освещения rgba8
	DS_RT_DEPTH,			//!< глубина r16f, непрозрачные материалы
	DS_RT_DEPTH0,			//!< глубина r16f, непрозрачные материалы
	DS_RT_DEPTH1,			//!< глубина r16f, непрозрачные и полупрозрачные пиксели, однако прозрачные (если есть) будут перекрывать непрозрачные, и в этом случае их глубина будет 1 то есть максимальной(для идентификации)

	DS_RT_AMBIENTDIFF,		//!< эмбиент цвет (цвет света rgb) и диффузное освещение (a) rgba16f
	DS_RT_SPECULAR,			//!< блики r16f
	DS_RT_SCENELIGHT,		//!< освещенная сцена rgba16
	DS_RT_SCENELIGHT2,		//!< освещенная сцена rgba16

	DS_RT_ADAPTEDLUM		//!< текущий rt адаптации глаза к освещению r16f
};

SX_LIB_API ID SML_DSGetRT_ID(DS_RT type);				//!< id render target по его типу
SX_LIB_API IDirect3DTexture9* SML_DSGetRT(DS_RT type);	//!< текстура render target по его типу

//!@} sxmtllight_ds

//#############################################################################

/*! \defgroup sxmtllight_mtl Система материалов
 \ingroup sxmtllight
@{*/

#define MTL_MAX_SIZE_DIR SXGC_LOADTEX_MAX_SIZE_DIR		/*!< максимальная длина имени директории материала */
#define MTL_MAX_SIZE_NAME SXGC_LOADTEX_MAX_SIZE_NAME	/*!< максимальная длина имени материала (без директории) */

#define MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC 3	/*!< максимальное количество обновлений статических отражений, в первый момент рендера */

/*! \name Дистанции при которых будут сниженно или повышено количество отрисовок для отражений
меньше MTL_REF_UPDATE_L0_DIST - обновление будет каждый кадр
больше MTL_REF_UPDATE_L2_DIST - обновления не будет
@{*/

#define MTL_REF_UPDATE_L0_DIST 10	/**/
#define MTL_REF_UPDATE_L1_DIST 30	/**/
#define MTL_REF_UPDATE_L2_DIST 70	/**/

//!@}

/*! \name Параметры альфа теста
 В данной реалзиации 2 вида альфа теста, принудительный (#SML_MtlSetForceblyAlphaTest) и назначаемый непосредственно материалом
 \note функция альфа теста D3DCMP_GREATEREQUAL
@{*/

#define MTL_ALPHATEST_FREE_VALUE 64		/*!< значение свободного альфа теста (назначаемого материалом) */
#define MTL_ALPHATEST_FORCEBLY_VALUE 8	/*!< значение принудительного альфа теста */

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

#define MTL_REF_TEX_SIZE 512	/*!< размер текстур с отражениями */
#define MTl_REF_PROJ_NEAR 0.25	/*!< ближняя плоскость отсечения */
#define MTl_REF_PROJ_FAR 100	/*!< дальняя плоскость отсечения, или дистанция на которую будет рендер всего в текстуру отражений */

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

#define MTL_TEX_R_MAIN		0	/*!< основная текстура */
#define MTL_TEX_R_MASK		1	/*!< маска */

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

#define MTL_TEX_R_PARAM_LIGHT	10	/*!< текстура с параметрами освещения */
#define MTL_TEX_R_REFLECTION	12	/*!< текстура с отражениями */
#define MTL_TEX_R_REFRACTION	13	/*!< текстура с преломлениями */
#define MTL_TEX_R_CURR_DEPTH	14	/*!< текстура с текущей глубиной */

//!@}

//! типы отражений
enum MTLTYPE_REFLECT
{
	MTLTYPE_REFLECT_NONE,			//!< нет отражения
	MTLTYPE_REFLECT_PLANE,			//!< плоское
	MTLTYPE_REFLECT_CUBE_DYNAMIC,	//!< объемное динамическое
	MTLTYPE_REFLECT_CUBE_STATIC		//!< объемное статическое, обработка идет первые несколько кадров, затем не обновляется
};

//! типы прозрачности
enum MTLTYPE_TRANSPARENCY
{
	MTLTYPE_TRANSPARENCY_NONE,			//!< нет прозрачности
	MTLTYPE_TRANSPARENCY_ALPHA_LIGHT,	//!< освещаяемая прозрачность
};

//! тип модели материала
enum MTLTYPE_MODEL
{
	MTLTYPE_MODEL_STATIC = 0,	//!< статическая геометрия
	MTLTYPE_MODEL_GRASS,		//!< растительность трава
	MTLTYPE_MODEL_TREE,		//!< растительность дерево
	MTLTYPE_MODEL_SKIN,		//!< анимационная модель

	//! значение по умолчанию
	MTLTYPE_MODEL_DEFAULT = MTLTYPE_MODEL_STATIC
};

//! физический тип материала
enum MTLTYPE_PHYSIC
{
	MTLTYPE_PHYSIC_CONCRETE = 0,	//!< бетон

	//! значение по умолчанию
	MTLTYPE_PHYSIC_DEFAULT = MTLTYPE_PHYSIC_CONCRETE,

	MTLTYPE_PHYSIC_METAL,			//!< металл
	MTLTYPE_PHYSIC_GLASS,			//!< стекло
	MTLTYPE_PHYSIC_PLASTIC,			//!< пластик
	MTLTYPE_PHYSIC_TREE,			//!< дерево
	MTLTYPE_PHYSIC_FLESH,			//!< плоть
	MTLTYPE_PHYSIC_GROUD_SAND,		//!< земля/песок
	MTLTYPE_PHYSIC_WATER,			//!< вода
	MTLTYPE_PHYSIC_LEAF_GRASS,		//!< листва/трава

	//! количество типов
	MPT_COUNT
};

//! данные отправляемые в шейдеры
enum MTL_SHADERSTD
{
	MTL_SHADERSTD_MATRIX_WORLD = 0,		//!< мировая матрица (трансформации модели), world
	MTL_SHADERSTD_MATRIX_VIEW,			//!< матрица вида, view
	MTL_SHADERSTD_MATRIX_PROJECTION,	//!< матрица проекции, projection
	MTL_SHADERSTD_MATRIX_WORLDVIEW,		//!< world * view
	MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ,	//!< world * view * projection
	MTL_SHADERSTD_CAMPOS,				//!< позиция камеры/наблюдателя
	MTL_SHADERSTD_TIMEDELTA,			//!< данные о времени float2(CountTimeRender,TimeDelta)
	MTL_SHADERSTD_WINSIZE,				//!< размеры окна рендера

	MTL_SHADERSTD_USERDATA				//!< пользовательские данные (float4)
};

/*! \name Загрузка/сохранение
 \note Файл загрузки/сохранения представлен простым конфигурационным файлом (ini) который можно обрабатывать посредством #ISXLConfig
 \todo Расписать организацию конфигурационного файла загрузки/сохранения для материала
@{*/

/*! загрузка материала, возвращает уникальный идентификатора материала, 
 \note все материалы оборачиваются в специальную структуру, поэтому дубликатов среди идентификаторов материалов нет
 \note любой повторно загружаемый материал не загружается, а лишь дублирует обертку, со ссылкой на внутренности оберкти
 */
SX_LIB_API ID SML_MtlLoad(
	const char *szName,	//!< имя_материала.расширение
	MTLTYPE_MODEL mtl_type = MTLTYPE_MODEL_STATIC	//!< тип модели материала на случай если материал не будет загружен/найден
	);

//! перезагрузка материала
SX_LIB_API void SML_MtlReloadMaterial(
	ID id,					//!< идентификатор материала
	const char *szName = 0	//!< на случай замены существующего материала другим
	);

/*! сохранить материал по id
 \note Сохранение происходит по отноительному пути в директория/директория_имяфайла.mtl
*/
SX_LIB_API void SML_MtlSave(ID id);	

//!@}

/*! очитска, удаляет все загруженные материалы (но место в массиве под них остается), все стандартные остаются
 \warning Отражения не удаляются а помещаются в очередь удаления, так как могут сожеражть в себе идентификаторы, которые необходимы очистить, 
для полной очистки необходимо вызывать функцию #SML_MtlDelRefClear. Однако во избежания утечек памяти необходимо очистить идентификаторы, для их получения вызыва функцию #SML_MtlDelRefGetIDArr
*/
SX_LIB_API void SML_MtlClear(
	bool isClearRefDel	//!< очищать ли очередь удаления отражений, на тот случай если идентификаторы в отражениях больше не действительны
	);	

SX_LIB_API long SML_MtlGetCount();					//!< возвращает общее количество материалов
SX_LIB_API MTLTYPE_MODEL SML_MtlGetTypeModel(ID id);	//!< возвращает тип модели материала по id

/*! установка типа модели материала по id
 \warning меняется только внутренний флаг (определение)!!! все остальное для данного типа надо загружать вручную, сделано для больших возможностей построения материалов
*/
SX_LIB_API void SML_MtlSetTypeModel(ID id, MTLTYPE_MODEL type_model);

//! установка параметров материала по id, вызывается перед DIP
SX_LIB_API void SML_MtlRender(
	ID id,			//!< идентификатор материала
	float4x4* world	//!< указатель на мировую матрицу модели, либо 0 - тогда будет принята единичная матрица
	);

//! стандартная отрисовка материала, используются стандартные шейдеры, нужно для теней, отражений и прочего
SX_LIB_API void SML_MtlRenderStd(
	MTLTYPE_MODEL type,			//!< тип материала из MtlTypeModel
	float4x4 *pWorld,			//!< мировая матрица трансформации, либо 0 и будет применена единичная матрица
	ID idSlot,					//!< текстурный слот в который установить текстуру
	ID idMtl					//!< идентификатор материала из которого будет браться текстура
	);

//! установка параметров материала для рендера источника света
SX_LIB_API void SML_MtlRenderLight(
	float4_t *pColor,	//!< rgb - цвет, w зарезервирован 
	float4x4 *pWorld		//!< аналогично #SML_MtlRender
	);

SX_LIB_API void SML_MtlSetMainTexture(ID idSlot, ID id);	//!< установить текстуру из материала id в текстурный слот slot

//физический тип материала
SX_LIB_API void SML_MtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type);	//!< установка физического типа материала
SX_LIB_API MTLTYPE_PHYSIC SML_MtlGetPhysicMaterial(ID id);				//!< возвращает текущий тип физического материала

//! возвращает id стандартного материала для определенной модели материалов указанной в #MtlTypeModel 
SX_LIB_API ID SML_MtlGetStdMtl(MTLTYPE_MODEL type_model);

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
без разделения по принадлжености к определнной поверхности будет трудно (пока выхода нет) корректно смешать все слои
*/
SX_LIB_API void SML_MtlSetIsIncrCountSurf(bool bf);
SX_LIB_API bool SML_MtlGetIsIncrCountSurf();		//!< установлен ли инкремент поверхностей
SX_LIB_API void SML_MtlNullingCurrCountSurf();		//!< обнуление идентификатора отрисованных поверхностей (к примеру при новом кадре)
SX_LIB_API int SML_MtlGetCurrCountSurf();			//!< текущее количество отрисованных полупрозрачных поверхностей
SX_LIB_API void SML_MtlSetCurrCountSurf(int iCount);	//!< установить текущее количество отрисованных полупрозрачных поверхностей

/*! принудительное включение альфа теста (isat - true - включить, false - выключить) при устновке параметров материала, 
к примеру для рендера полупрозрачных поверхностей, чтобы отсечь (условно) полностью прозрачные пиксели
*/
SX_LIB_API void SML_MtlSetForceblyAlphaTest(bool isEnable);	
SX_LIB_API bool SML_MtlGetForceblyAlphaTest();			//!< включен ли принудительный альфа тест

//!@}

/*! \defgroup sxmtllight_mtl_reflect Отражения
 \ingroup sxmtllight_mtl
 \note Поддерживается 2 вида отражений:
  - плоские
  - объемные/кубические
@{*/
//{{

//! записать в отражения идентификатор (idarr) для определнной стороны куба/сплита (cube), соотнести его с идентификатором сущности (inid)
SX_LIB_API void SML_MtlRefSetIDArr(
	ID id,		//!< идентификатор материала
	ID inid,	//!< идентификатор сущности для которой записываем (геометрия, растительность и прочее)
	int iCube,	//!< условно - сторона куба/сплита для которой записываем 
	ID idArr	//!< идентификатор который надо записать
	);

//! возвращает идентификатор который был записан для сущности (inid) для стороны куба/сплита (cube), если вообще записывался, если не записывался то <0
SX_LIB_API ID SML_MtlRefGetIDArr(
	ID id,		//!< идентификатор материала
	ID inid,	//!< идентификатор сущности для которой было записывано (геометрия, растительность и прочее)
	int iCube	//!< условно - сторона куба/сплита для которой было записывано 
	);

/*! \name Управление удаленными отражениями
@{*/

SX_LIB_API int SML_MtlDelRefGetCount();	//!< возвращает количество удаленных материалов с отражениями
//! возвращает идентификатор который был записан для сущности (inid) для стороны куба/сплита (cube)
SX_LIB_API ID SML_MtlDelRefGetIDArr(
	ID key,		//!< порядковый номер удаленного материала с отражением
	ID inid,	//!< идентификатор сущности для которой было записано (геометрия, растительность и прочее)
	int iCube	//!< условно - сторона куба/сплита для которой было записывано
	);
SX_LIB_API void SML_MtlDelRefClear();	//!< очистка массива удаленных материалов с отражениями

//!@}

//! обработка статистики рендера для текущего матриала с отражением
SX_LIB_API bool SML_MtlRefUpdateCountUpdate(
	ID id,				//!< идентификатора материала
	float3_t *pViewPos	//!< позиция наблюдателя, viewpos == 0 вернет false
	);

/*! обнуление статистикки рендера для данного материала с отражением, 
полезно в случаях каких-либо изменений, чтобы эти изменения были видны и в отражениях (актуально для статических отражений)
*/
SX_LIB_API void SML_MtlRefNullingCountUpdate(ID id);

SX_LIB_API void SML_MtlRefSetMinMax(ID id, float3_t *pMin, float3_t *pMax);//!< установка экстремумов материала

//! возвращает фрустум отражения, cube - сторона куба
SX_LIB_API ISXFrustum* SML_MtlRefGetfrustum(
	ID id,		//!< идентификатора материала
	int iCube	//!< сторона куба, если отражение плоское то необходимо указать в cube передать 0, иначе вернет NULL
	);

SX_LIB_API bool SML_MtlRefIsAllowedRender(ID id);	//!< разрешен ли рендер отражений для данного материала в текущий момент

/*! \name Рендер плоского отражения
 \note Перед началом рендера плоских отражений необходимо в регистры матриц в #G_RI_MATRIX_VIEW поместить матрицу вида наблюдателя
 \note Порядок рендера:
 \code
 SML_MtlRefPreRenderPlane(id, plane);
 //код рендера всего того что необходимо поместить в текстуру отражений
 SML_MtlRefPostRenderPlane(id);
 \endcode
@{*/

//! первоначальные настройки данных для генерации отражения
SX_LIB_API void SML_MtlRefPreRenderPlane(
	ID id,				//!< идентификатор материала
	D3DXPLANE *pPlane	//!< плоскость относительно которой будет считаться отражение
	);	
//между этими функциями необходимо помещать код рендера всего того что необходимо поместить в текстуру отражений
SX_LIB_API void SML_MtlRefPostRenderPlane(ID id);			//!< завершающие операции
SX_LIB_API IDirect3DTexture9* SML_MtlRefGetTexPlane(ID id);	//!< возвращает текстуру с плоским отражением (если оно есть, иначе 0)

//!@}

/*! \name Рендер объемного (кубического) отражения
 \note Порядок рендера:
 \code
 SML_MtlRefCubeBeginRender(id, center);
 SML_MtlRefCubePreRender(id, cube, world);
 //код рендера всего того что необходимо поместить в текстуру отражений
 SML_MtlRefCubePostRender(id, cube);
 SML_MtlRefCubeEndRender(id, viewpos);
 \endcode
@{*/

//! установка общих настроек
SX_LIB_API void SML_MtlRefCubeBeginRender(
	ID id,				//!< идентификатор материала
	float3_t *pCenter	//!< центр (позиция) подгруппы модели - откуда будет рендер отражений
	);	

//! установка частных настроек для стороны куба (cube)
SX_LIB_API void SML_MtlRefCubePreRender(
	ID id,			//!< идентификатор материала
	int iCube,		//!< сторона куба [0,5]
	float4x4 *pWorld//!< мировая матрица модели 
	);

//между этим набором функций (Begin-pre ... post-End) необходимо помещать код рендера всего того что необходимо поместить в текстуру отражений
SX_LIB_API void SML_MtlRefCubePostRender(ID id, int iCube);	//!< завершающие частные моменты для стороны куба

/*! завершающие общие моменты
 \note если viewpos == 0 обновления статисктики рендера для текущего материала с отражением не произойдет, 
и дабы рационально обрабатывать и статические и динамические отражения необходимо до следующего рендера отражений вызвать SML_MtlRefUpdateCountUpdate
*/
SX_LIB_API void SML_MtlRefCubeEndRender(
	ID id,				//!< идентификатор материала
	float3_t *pViewPos	//!< позиция наблюдателя, необходимо для обработки разрешения на следующий рендер
	);	

SX_LIB_API IDirect3DCubeTexture9* SML_MtlRefCubeGetTex(ID id);	//!< возвращает cube текстуру отражением (если оно есть, иначе 0)

//!@}

//!@} sxmtllight_mtl_reflect


/*! \defgroup sxmtllight_mtl_propslayer Cвойства поверхности
 \ingroup sxmtllight_mtl
@{*/

//основная текстура которая накладывается на модель/подгруппу
SX_LIB_API void SML_MtlSetTexture(ID id, const char *szPathTex);	//!< установка основной текстуры (загрузка текстуры по имени)
SX_LIB_API void SML_MtlGetTexture(ID id, char *szName);			//!< в name записывает имя текстуры
SX_LIB_API ID SML_MtlGetTextureID(ID id);						//!< возвращает id текстуры материала


/*! \name Шейдеры для рендера материала
 \warning В текущей версии все шейдеры загружаются в версии 3.0 и требуют в качестве старта указывать функцию main
 \warning Рекомендуется устанавливать и вершинный и пиксельный шейдеры, так как во врем тестов одно из устройств требовало наличия обоих шейдеров либо отсутствия обоих
 \note В аргументах path_vs/path_ps указывается только имя шейдера с расширением, есесно возвращается тоже самое
@{*/

SX_LIB_API void SML_MtlSetVS(ID id, const char *szPathVS);	//!< установка вершинного шейдера
SX_LIB_API void SML_MtlGetVS(ID id, char *szName);			//!< в name записывает имя текущего вершинного шейдера
SX_LIB_API void SML_MtlSetPS(ID id, const char *szPathPS);	//!< установка пиксельного шейдера
SX_LIB_API void SML_MtlGetPS(ID id, char *szName);			//!< в name записывает имя текущего пикельного шейдера

//!@}

SX_LIB_API float SML_MtlGetPenetration(ID id);
SX_LIB_API void SML_MtlSetPenetration(ID id, float fPenetration);

SX_LIB_API float SML_MtlGetHitChance(ID id);
SX_LIB_API void SML_MtlSetHitChance(ID id, float fHitChance);

SX_LIB_API float SML_MtlGetDensity(ID id);
SX_LIB_API void SML_MtlSetDensity(ID id, float fDensity);

//использование альфа теста для материала
SX_LIB_API bool SML_MtlGetUsingAlphaTest(ID id);				//!< используется ли альфа тест для материала
SX_LIB_API void SML_MtlSetUsingAlphaTest(ID id, bool isUsing);	//!< установка использования альфа теста

/*! освещение материала id
 \note если материал не освещается то будет выведен в цвете накладываемым материалом
*/
SX_LIB_API void SML_MtlSetLighting(ID id, bool isLighting);	
SX_LIB_API bool SML_MtlGetLighting(ID id);	//!< освещается ли материал

/*! \name Параметры освещения.
 \note Для более точной/тонкой настройки освещения материала рекомендуется использовать текстуру с параметрами освещения 
(к прмиеру когда в одной поверхности разные материалы, к примеру дерево, метал и камень, то в текстуре можно указать параметры освещения для каждого пикселя), 
однако если текстуры нет то будут использованы установленные парметры освещения для всей поверхности
@{*/

//! установка использования текстуры с параметрами освещения
SX_LIB_API void SML_MtlSetIsTextureLighting(ID id, bool isTexLighting);
SX_LIB_API bool SML_MtlGetIsTextureLighting(ID id);	//!< используется ли текстура с параметрами для освещения

SX_LIB_API void SML_MtlSetTextureLighting(ID id, const char *szPathTex);	//!< загрузка текстуры с парамтерами освещения
SX_LIB_API void SML_MtlGetTextureLighting(ID id, char *szPathTex);		//!< в path_tex записывает имя текстуры с параметрами освещения

SX_LIB_API void SML_MtlSetRoughness(ID id, float fRoughness);	//!< установка шероховатости (0-1)
SX_LIB_API float SML_MtlGetRoughness(ID id);					//!< возвращает текущее значение шероховатости поверхности

SX_LIB_API void SML_MtlSetThickness(ID id, float fThickness);	//!< установка толщины (0-1)
SX_LIB_API float SML_MtlGetThickness(ID id);					//!< возвращает текущее значение толщины

//f0 - отражательная способность поверхности (0-1)
SX_LIB_API void SML_MtlSetF0(ID id, float fF0);	//!< установка отражательной способности поверхности (0-1)
SX_LIB_API float SML_MtlGetF0(ID id);			//!< возвращает текущее значение отражательной способности поверхности

//!@}

/*! \name Полупрозрачность, типы из #MtlTypeTransparency
Под полупрозрачными материалами следует понимать такие материалы, прозрачность которых в среднем находится в пределах 0.5 и минимальный процент полупрозрачного периметра 50%
@{*/

SX_LIB_API void SML_MtlSetTypeTransparency(ID id, MTLTYPE_TRANSPARENCY type);	//!< установка типа полупрозрачности
SX_LIB_API MTLTYPE_TRANSPARENCY SML_MtlGetTypeTransparency(ID id);				//!< возвращает текущий тип полупрозрачности для материала

//!@}

/*! \name Отражения окружения, типы из #MtlTypeReflect
@{*/

SX_LIB_API void SML_MtlSetTypeReflection(ID id, MTLTYPE_REFLECT type);	//!< установка типа отражений
SX_LIB_API MTLTYPE_REFLECT SML_MtlGetTypeReflection(ID id);				//!< возвращает текущий тип отражений для материала

//!@}

/*! \name Детальность и микрорельеф
Поверхность может содержать до 4 детальных текстур и до 4 текстур с микрорельефом. 
Данные для определения наложения текстур (детальных и микрорельефных) указываются в маске - 
на каждый канал текстуры маски по одной детальной и микрорельефной текстуре, 
то есть канал маски определет связанные между собой и детальную и микрорельефную текстуры
 \note В аргументах channel - RGBA - 0,1,2,3
@{*/

//маска наложения
SX_LIB_API void SML_MtlSetMaskTex(ID id, const char *szPathTex);	//!< загрузка маски наложения
SX_LIB_API void SML_MtlGetMaskTex(ID id, char *szPathTex);		//!< в path_tex записывает текущее имя маски наложения

//! загрузка текстуры микрорельефа, на каждый канал
SX_LIB_API void SML_MtlSetMRTex(
	ID id,					//!< идентификатор материала
	int iChannel,			//!< номер канала RGBA - 0,1,2,3
	const char *szPathTex	//!< имя текстуры с расширением
	);

//! в path_tex запишет текущее имя текстуры микрорельефа для канала channel
SX_LIB_API void SML_MtlGetMRTex(ID id, int iChannel, char *szPathTex);

//! загрузка детальной текстуры, на каждый канал
SX_LIB_API void SML_MtlSetDTex(
	ID id,					//!< идентификатор материала
	int iChannel,			//!< номер канала RGBA - 0,1,2,3
	const char *szPathTex	//!< имя текстуры с расширением
	);

//! в path_tex запишет текущее имя детальной текстуры для канала channel
SX_LIB_API void SML_MtlGetDTex(ID id, int iChannel, char *szPathTex);

//!@}

/*! \name Стандартные параметры для отправки в шейдер (в вершинный и пиксельный раздельно), данные из #MtlTransShaderStd
@{*/

//! отправка стадартных данных в вершинный шейдер
SX_LIB_API void SML_MtlSetSTDVS(
	ID id,				//!< идентификатор материала
	MTL_SHADERSTD type,	//!< тип значения которое нужно отправить, одно из значений #MtlTransShaderStd
	bool isSend		//!< true - отправлять, false - не отправлять
	);

//! установлена ли отпрвка значения type в вершинный шейдер
SX_LIB_API bool SML_MtlGetSTDVS(ID id, MTL_SHADERSTD type);

//! отправка стадартных данных в пиксельный шейдер
SX_LIB_API void SML_MtlSetSTDPS(
	ID id,				//!< идентификатор материала
	MTL_SHADERSTD type,	//!< тип значения которое нужно отправить, одно из значений #MtlTransShaderStd
	bool isSend		//!< true - отправлять, false - не отправлять
	);

//! установлена ли отпрвка значения type в пиксельный шейдер
SX_LIB_API bool SML_MtlGetSTDPS(ID id, MTL_SHADERSTD type);

//!@}

/*! \name Дополнительные пользовательские данные в виде float4 (предел [0-1] для стандарта, иное вне стандарта, но допустимо)
 \note Данные для вершинного и пиксельного шейдера раздельны. Отправляется float4.
Однако данные и из пиксельного могут быть отправленны в вершинный и наоборот.
В итоге в шейдере может быть 2 float4 пользовательских вектора с данными.
Но если вектор определен для пиксельного то он все-равно будет отправлен в пиксельный шейдер, даже если и придназначалася для вершинного.
 \note В аргументах component - xyzw - 0,1,2,3
@{*/

//! установка значения компоненты вектора для вершинного шейдера
SX_LIB_API void SML_MtlSetUDVS(
	ID id,			//!< идентификатор материала
	int iComponent,	//!< компонента вектора xyzw - 0,1,2,3
	float fValue	//!< значение компоненты
	);

//! возвращает значение компоненты вектора, который отправляется в вершинный шейдер
SX_LIB_API float SML_MtlGetUDVS(ID id, int iComponent);

//! установка отправки float4 вектора, предназначенного для вершинного шейдера, в пиксельный шейдер
SX_LIB_API void SML_MtlSetUDVS_InPS(ID id, bool isSendPs);
SX_LIB_API bool SML_MtlGetUDVS_InPS(ID id);	//!< отправляется ли float4 вектор, предназначенный для вершинного шейдера, в пиксельный шейдер


//! установка значения компоненты вектора для пиксельного шейдера
SX_LIB_API void SML_MtlSetUDPS(
	ID id,			//!< идентификатор материала
	int iComponent,	//!< компонента вектора xyzw - 0,1,2,3
	float fValue	//!< значение компоненты
	);

//! возвращает значение компоненты вектора, который отправляется в пиксельный шейдер
SX_LIB_API float SML_MtlGetUDPS(ID id, int iComponent);

//! установка отправки float4 вектора, предназначенного для пиксельного шейдера, в вершинный шейдер
SX_LIB_API void SML_MtlSetUDPS_InVS(ID id, bool isSendVs);
SX_LIB_API bool SML_MtlGetUDPS_InVS(ID id);	//!< отправляется ли float4 вектор, предназначенный для пиксельного шейдера, в вершинный шейдер

//!@}

//!@} sxmtllight_mtl_propslayer

//!@} sxmtllight_mtl

#endif

//!@} sxmtllight
