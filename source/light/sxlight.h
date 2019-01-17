
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxlight - библиотеки освещения
*/

/*! \defgroup sxlight sxlight - библиотека освещения
@{
*/

#ifndef __SXLIGHT_H
#define __SXLIGHT_H

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

//! версия подсистемы
SX_LIB_API long SLight_0GetVersion();			

//! установить функцию обработки сообщений
SX_LIB_API void SLight_Dbg_Set(report_func rf);

//! инициализция подсистемы
SX_LIB_API void SLight_0Create(
	const char *szName,				//!< присваиваемое имя
	bool isUnic = true				//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожить подсистему
SX_LIB_API void SLight_AKill();

//!@}

/*! \name Обработка потери/восстановления устройства
@{*/

//! вызывать при потрете устройства
SX_LIB_API void SLight_OnLostDevice();

//! вызывать при восстановлении устройства
SX_LIB_API void SLight_OnResetDevice();

//!@}

//#############################################################################

/*! \defgroup sxlight_light Освещение
 \ingroup sxlight
 \warning Обращение (идентификация) конкретного света осуществляется посредством идентификатора который выдают создающие функции:
  - #SLight_CreatePoint
  - #SLight_CreateDirection
 Получить идентификатор уже созданного света можно, посредством ключа (key) который лежит в пределах [0, #SLight_GetCount),
передав этот ключ #SLight_GetIDOfKey, которая в случае успеха вернет идентификатор света.
@{*/

/*! максимальное количество обновлений статических источников света, в первый момент рендера */
#define LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC 3	

/*! \name Дистанции (метрах) при которых будет сниженно или повышено количество отрисовок для локальных источников света
меньше #LIGHTS_UPDATE_L0_DIST - обновление будет каждый кадр\n
больше #LIGHTS_UPDATE_L2_DIST - обновления не будет
@{*/

#define LIGHTS_UPDATE_L0_DIST 10	
#define LIGHTS_UPDATE_L1_DIST 30	
#define LIGHTS_UPDATE_L2_DIST 70	

//!@}

/*! Коэфициент обновления сплитов глобального света.
Cчитается: LIGHTS_UPDATE_PSSM_SPLIT*number_split
 \note Первый сплит обновляется каждый кадр, второй через кадр
*/
#define LIGHTS_UPDATE_PSSM_SPLIT 1		

/*! аналогичнен #LIGHTS_UPDATE_PSSM_SPLIT только для 5 сплита (для теней от облаков) */
#define LIGHTS_UPDATE_PSSM_SPLIT_5 2	


/*! стандартное значение верхнего/начального радиуса для направленного света  */
#define LIGHTS_LOCAL_STD_TOP_RADIUS 0.1	

/*! стандартное значение ближней плоскости отсечения для теней локального света  */
#define LIGHTS_LOCAL_STD_NEAR 0.0025		

/*! максимальная дистанция локального света */
#define LIGHTS_LOCAL_MAX_DIST 200.f		


/*! радиус глобального источника */
#define LIGHTS_GLOBAL_STD_RADIUS 999999	

/*! отдаленность глобального света (нужно для корректности теней), если не знаете, не трогайте */
#define LIGHTS_POS_G_MAX 1000	


/*! базовое направление направленноого источника света */
#define LIGHTS_DIR_BASE float3(0, -1, 0)


//! типы источников света
enum LTYPE_LIGHT
{
	//! не установленный
	LTYPE_LIGHT_NONE = -1,	

	//! глобальный
	LTYPE_LIGHT_GLOBAL,		

	//! точечный
	LTYPE_LIGHT_POINT,		

	//! направленный
	LTYPE_LIGHT_DIR			
};

//! типы теней источников света
enum LTYPE_SHADOW
{
	//! без теней
	LTYPE_SHADOW_NONE = -1,	

	//! статичные тени, тень обновляется первые несколько кадров (для корректности), далее не обновляется
	LTYPE_SHADOW_STATIC,	

	//! полностью динамические тени
	LTYPE_SHADOW_DYNAMIC,	
};

/*! \name Настройки размеров для теней
 \note Коэфициента относительно размеров окна рендера, если 1.f то размеры экрана, если 0.5f то размеры экрана * 0.5f
@{*/

//! установка коэфициента для локального света
SX_LIB_API void SLight_SettLCoefSizeDepth(float fCoef);

//! установка коэфициента для глобального света
SX_LIB_API void SLight_SettGCoefSizeDepth(float fCoef);	


//! возвращение коэфициента для локальный свет
SX_LIB_API float SLight_GettLCoefSizeDepth();	

//! возвращение коэфициента для глобальный свет
SX_LIB_API float SLight_GettGCoefSizeDepth();	

//!@}

//! очистить список света
SX_LIB_API void SLight_Clear();		

//! очистка всех идентификаторов просчетов во всех источниках света
SX_LIB_API void SLight_ClearIDArr();	

//! общее количество света
SX_LIB_API int SLight_GetCount();	

//! сброс информации об обновлении локальных источников света, для перестройки карт глубин
SX_LIB_API void SLight_ResetUpdate4Local();

//! возвращает true если свет с id существует
SX_LIB_API bool SLight_GetExists(ID id);

//! создать точечный свет (светит во все стороны)
SX_LIB_API ID SLight_CreatePoint(
	const float3 *pCenter,	//!< центр света (мировая позиция)
	float fDist,			//!< дистанция, на которую распространяется свет
	const float3 *pColor,	//!< цвет в пределах 0-1
	bool isGlobal,			//!< глобальный ли свет (типа солнца)
	bool isShadowed			//!< отбрасвыает ли тени свет
	);

//! создать направленный свет
SX_LIB_API ID SLight_CreateDirection(
	const float3 *pPos,			//!< позиция света (мировая)
	float fDist,				//!< дистанция на которую распространяется свет
	const float3 *pColor,		//!< цвет
	const SMQuaternion *qOrient,//!< ориентация
	float fTopRadius,			//!< верхний (начальный) радиус
	float fAngle,				//!< угол (в радианах)
	bool isShadow				//!< отбрасвыает ли тени свет
	);

//! возвращает тип света
SX_LIB_API LTYPE_LIGHT SLight_GetType(ID id);	

//! создать копию объекта и вернуть на нее id
SX_LIB_API ID SLight_CreateCopy(ID id);		

//! удаление света
SX_LIB_API void SLight_DeleteLight(ID id);	

//! возвращает имя света
SX_LIB_API char* SLight_GetName(ID id);					

//! устанавливает имя света
SX_LIB_API void SLight_SetName(ID id, const char *szName);	

//! отрисовка ограничивающего объема 
SX_LIB_API void SLight_Render(ID id, DWORD timeDelta);

//! в color записывает цвет света
SX_LIB_API void SLight_GetColor(ID id, float3 *pColor);		

//! устанавливает цвет света
SX_LIB_API void SLight_SetColor(ID id, const float3 *pColor);	

//! получить в pos позицию света
SX_LIB_API void SLight_GetPos(
	ID id,				//!< идентификатор света
	float3 *pPos,		//!< инициализированный вектор в который будет записана позиция
	bool isReal = false	//!< true - нужны реальные координаты, false - нужны углы, только для глобального света, во всех остальных случаях не имеет смысла
	);

//! установить позицию свету
SX_LIB_API void SLight_SetPos(
	ID id,				//!< идентификатор света
	const float3 *pPos,	//!< позиция в которую будет перемещен свет
	bool isReal = false	//!< true - установка реальных координат, false - установка углов, только для глобального света, во всех остальных случаях не имеет смысла
	);

//! возвращает "значение силы света" для отправки в шейдер (максимальная компонента цвет * дистанция покрытия/#LIGHTS_LOCAL_MAX_DIST)
SX_LIB_API float SLight_GetPower(ID id);	

//! возвращает дистанцию, на которую распространяется свет
SX_LIB_API float SLight_GetDist(ID id);		

//! установка новой дистанции
SX_LIB_API void SLight_SetDist(
	ID id,					//!< идентификатор света
	float fDist,			//!< новая дистанция(радиус для точечного и высота для направленного)
	bool isCreate = true	//!< нужно ли пересоздание объема? (если новая дистанция больше чем текущий ограничивающий объем то надо, в других случаях по усмотрению)
	);

//! записывает в q ориентацию света
SX_LIB_API void SLight_GetOrient(ID id, SMQuaternion *pQ);

//! установка ориентации света
SX_LIB_API void SLight_SetOrient(ID id, const SMQuaternion *pQ);


//! включен ли свет с идентификатором id
SX_LIB_API bool SLight_GetEnable(ID id);				

//! установить всключен (true) или отключен (false) свет  с идентификатором id
SX_LIB_API void SLight_SetEnable(ID id, bool isEnable);	

//! возвращает идентификатор глобального источника света, если он есть, иначе -1
SX_LIB_API ID SLight_GetGlobal();					

//! возвращает установлена ли общая тень от глобального источника (все ли в тени)
SX_LIB_API bool SLight_GetCastGlobalShadow();

//! установить общую тень для всего от глобального источника (все в тень)
SX_LIB_API void SLight_SetCastGlobalShadow(bool isShadowed);

//! установить интенсивность тени для локального источника света [0 - нет тени, 1 - черная тень]
SX_LIB_API void SLight_SetShadowIntensity(ID id, float m_fShadowCoef);

//! возвращает интенсивность тени локального источника света
SX_LIB_API float SLight_GetShadowIntensity(ID id);

//-----------------------------------------------------------------------------

/*! \name Управление видимостью
 \note Отдельный просчет - считает индивидуально для света и тут же возвращает результат просчетов
 \note Предварительный просчет - просчет для всего света (к примеру в другом потоке)
@{*/

//! виден ли свет в данном фрустуме (отдельный просчет)
SX_LIB_API bool SLight_ComVisibleForFrustum(ID id, const IFrustum *pFrustum);	

//! дистанция от viewpos до начала света (отдельный просчет)
SX_LIB_API float SLight_ComDistFor(ID id, const float3 *pViewPos);				

//! просчет видимости и расстояния от наблюдателя viewpos до начала света, для каждого света (предварительный просчет)
SX_LIB_API void SLight_ComVisibleFrustumDistFor(const IFrustum *pFrustum, const float3 *pViewPos);


//! виден ли свет в просчитанном фрустуме
SX_LIB_API bool SLight_GetVisibleForFrustum(ID id);	

//! просчитанная дистанция от позиции наблюдателя до начала света
SX_LIB_API float SLight_GetDistFor(ID id);			

//!@}

//-----------------------------------------------------------------------------

/*! \name Настройки для направленного света
@{*/

//! возвращает угол, актуально только для направленного и света
SX_LIB_API float SLight_GetAngle(ID id);				

//! установить угол, актуально только для направленного и света
SX_LIB_API void SLight_SetAngle(ID id, float fAngle);	

//! возвращает верхний радиус, актуально только для направленного и света
SX_LIB_API float SLight_GetTopRadius(ID id);					

//! устанавливает верхний радиус, актуально только для направленного и света
SX_LIB_API void SLight_SetTopRadius(ID id, float fTopRadius);

//!@}

//-----------------------------------------------------------------------------

/*! \name Настройки для точечного света
@{*/

/*! установить "рисовать ли в указанную грань cubemap тени"
 \warning Актуально только когда свет может отбрасывать тени
 */
SX_LIB_API void SLight_SetCubeEdgeEnable(
	ID id,			//!< идентификатор света
	int iEdge,		//!< номер стороны/грани cubemap [0,5]
	bool iEnable	//!< true - рисовать, false - не рисовать
	);

//! получить текущее значение "рисовать ли в указанную грань cubemap тени"
SX_LIB_API bool SLight_GetCubeEdgeEnable(
	ID id,		//!< идентификатор света
	int iEdge	//!< номер стороны/грани cubemap [0,5]
	);

//!@}

//-----------------------------------------------------------------------------

/*! \defgroup sxlight_light_shadow Управление тенями
 \ingroup sxlight_light
 \note Для генерации теней используется один render target full screen
 \note Для сглаживания используется еще один аналогичный render target и первый render target. При сглаживании внутри меняется только счетчик, который меняет их местами
 \note Так как используется всего один render target для генерации теней то есесно в текущий момент времени можно сгенерировать и сгладить тень только для одного света
 \note Для теней от глобального света используется техника PSSM (Parallel-Split Shadow Mapping) на 3/4 сплита для объектов уровня (3 или 4 сплита можно настроить #SLight_Set4Or3SplitsG)
 \note Для теней от локального света используется техника Shadow Mapping в двух вариациях:
  - для точечного света используется cubemap
  - для направленного света - обычная техника Shadow Mapping
@{*/

//! отбрасывает ли свет тени
SX_LIB_API bool SLight_GetShadowed(ID id);						

//! возвращает true в случае если установлено 4 сплита для PSSM, иначе (установлено 3 сплита) false
SX_LIB_API bool SLight_Get4Or3SplitsG(ID id);				

//! установка количества сплитов (false - 3, true - 4) для PSSM
SX_LIB_API void SLight_Set4Or3SplitsG(ID id, bool is4or3);	

/*! устанавливает шейдеры и передает им данные на основании типа материала, поддерживаются все типы MTL_TYPE_*/
SX_LIB_API void SLight_ShadowSetShaderOfTypeMat(ID id, int typemat, const float4x4 *pWorld);

/*! \name Управление обновлениями теней
 \note Так как основная задумка это real-time рендер то для этого нужны упрощения, одним из таковых является обновление теней от света, которое может происходить не каждый кадр
@{*/

//! установка динамики обработки теней
SX_LIB_API void SLight_SetTypeShadowed(
	ID id,				//!< идентификатор света
	LTYPE_SHADOW type	//!< значение из перечисления #LTYPE_SHADOW
	);

//! возвращает тип динамики теней (одно из значений #LTYPE_SHADOW)
SX_LIB_API LTYPE_SHADOW SLight_GetTypeShadowed(ID id);	

//! производит обработку данных затем возвращает разрешено ли обновлять тени, увеличивает внутрений счетчик, поэтому необходимо вызывать только когда надо
SX_LIB_API bool SLight_CountUpdateUpdate(
	ID id,					//!< идентификатор света
	const float3 *pViewPos,	//!< позиция камеры для которой обновляем, на основании расстояния будет произведен расчет обновления
	int iHow = -1			//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! разрешено ли обновлять тени, в отличии от #SLight_UpdateCountUpdate не увеличивает внутренний счетчик
SX_LIB_API bool SLight_CountUpdateAllowed(
	ID id,			//!< идентификатор света
	int iHow = -1	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! сбросить счетчик обновлений и обновлять до предела тени
SX_LIB_API void SLight_CountUpdateNull(ID id);	

//!@}


/*! \name Управление циклами рендера в карты глубин
 Для локального направленного света:
 \code
 SLight_ShadowRenderBegin(id);
 //рендер того что должно отбросить тени
 SLight_ShadowRenderEnd(id);
 \endcode
 Для глобального и точечного:
 \code
 SLight_ShadowRenderBegin(id);
 SLight_ShadowRenderPre(id, cube);
 //рендер того что должно отбросить тени
 SLight_ShadowRenderPost(id, cube);
 SLight_ShadowRenderEnd(id);
 \endcode
 Где cube для точечного это номер стороны cubemap [0-5] в которую будет осуществлен рендер, а для глобального это номер сплита.
@{*/

//! подготовительные процедуры
SX_LIB_API void SLight_ShadowRenderBegin(ID id);	

//! завершающие процедуры
SX_LIB_API void SLight_ShadowRenderEnd(ID id);	

//! подготовительные процедур перед рендером для сплита/стороны
SX_LIB_API void SLight_ShadowRenderPre(
	ID id,		//!< идентификатор света
	int iCube	//!< для точечного света - сторона cubemap [0-5] для глобального света номер сплита
	);	

//! завершающие процедур перед рендером для сплита/стороны
SX_LIB_API void SLight_ShadowRenderPost(
	ID id,		//!< идентификатор света
	int iCube	//!< для точечного света - сторона cubemap [0-5] для глобального света номер сплита
	);

//!@}

//! обнуление карты теней, необходимо вызывать перед генерацией теней посредством функции #SLight_GenShadow
SX_LIB_API void SLight_ShadowNull();			

//! генерация тени на основе уже отработанной информации о затенении, перед вызовов этой функции необходимо вызывать #SLight_ShadowNull
SX_LIB_API void SLight_ShadowGen(ID id);		

//! возвращает текстуру с тенями (если вызывались функции сглаживания) то вренутся сглаженная
SX_LIB_API IGXTexture2D* SLight_GetShadow();

//! сглаживание текущей тени
SX_LIB_API void SLight_ShadowSoft(
	bool isRandSam,			//!< использовать ли смещение по случайному направлению
	float fSize,			//!< коэфициент увеличения размера пикселя для размытия
	bool isFirst = false	//!< впервые ли вызывается размытие для текущей тени, каждый раз когда тень еще не сглажена, но сгенерирована, нужно указывать false
	);

//! возвращает указатель фрустума света
SX_LIB_API IFrustum* SLight_GetFrustum(
	ID id,		//!< идентификатор света
	int iHow	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! обновление фрустума глобального света
SX_LIB_API void SLight_UpdateGFrustums(
	ID id,				//!< идентификатор света
	int iSplit,			//!< номер сплита
	const float3 *pPos,	//!< позиция наблюдателя
	const float3 *pDir	//!< направление взгляда наблюдателя
	);

/*! \name Управление настройками для генерации глубины/тени
@{*/

//! получить текущее смещение (bias) при генерации глубины
SX_LIB_API float SLight_GetShadowBias(ID id);				

//! установить текущее смещение (bias) при генерации глубины
SX_LIB_API void SLight_SetShadowBias(ID id, float fBias);	


//! установить размерность размытия (в пикселя) при геренации тени
SX_LIB_API void SLight_SetShadowBlurPixel(ID id, float fBlurPixel);	

//! получить текущее значение размерности (в пикселях) при генерации тени
SX_LIB_API float SLight_GetShadowBlurPixel(ID id);						


//! установить значение ближней плоскостью отсечения для теней (локальный свет)
SX_LIB_API void SLight_SetShadowLocalNear(ID id, float fNear);	

//! получить текущее значение ближней плоскостью отсечения для теней (локальный свет)
SX_LIB_API float SLight_GetShadowLocalNear(ID id);				

/*! установка дальней плоскостью отсечения для теней (локальный свет)
 \note Иногда возникают артефакты и если отрегулировать это значение то все становится на свои места, тесты показали что в основном при ошибках, увеличение этого значения решало проблемы
*/
SX_LIB_API void SLight_SetShadowLocalFar(
	ID id,		//!< идентификатор света
	float fFar	//!< значение дальней плоскости отсечения (дальность видимости теней), обычно это значение = дальность света
	);

//! возвращает значение дальней плоскости отсечения (дальности видимости теней)
SX_LIB_API float SLight_GetShadowLocalFar(ID id);	

//!@}

//-----------------------------------------------------------------------------

/*! \name Управление массивами обработки данных для рендера (к примеру от геометрии, растительности и прочего)
 Cделано для того чтобы отделить просчеты видимости (если они нужны) от рендера. 
 К примеру можно рассчитать видимости в одном потоке а рисовать в основном.
@{
*/

//! возвращает идентификатор в случае успеха, иначе <0
SX_LIB_API ID SLight_GetIDArr(
	ID id,		//!< идентификатор света
	ID idOwner,	//!< идентификатор объекта/системы вообще того что выдало идентификатор массива
	int iHow	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! установить идентификатор
SX_LIB_API void SLight_SetIDArr(
	ID id,		//!< идентификатор света
	ID idOwner,	//!< идентификатор объекта/системы вообще того что выдало идентификатор массива
	int iHow,	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	ID idArr	//!< номер массива обработки от объекта который надо будет обрабатывать на видимость
	);

//!@}

//!@} sxlight_light_shadow

/*! \name Функции обработки удаленного света
 При удалении света он помещается в специальный массив удаленных объектов, 
это связано с тем что свет может содержать в себе данные которые требуют очистки памяти вне данной реализации, 
к примеру идентификаторы массивов просчетов видимости. \n
 Поэтому необходимо вручную удалять (уже после удаленяи функцией #SLight_DeleteLight) удаленные объекты света.
 \note Доступ к элементам массива удаленных объектов света осуществляется посредством порядкового номера в массиве (ключа key), то есть можно пройтись по массиву циклом.
@{*/

//! возвращает количество удаленных объектов света которые требуют удаления
SX_LIB_API int SLight_DelGetCount();						

//! возвращает тип удаленного объекта света, значения из #LightsTypeLight
SX_LIB_API LTYPE_LIGHT SLight_DelGetType(ID key);		

//! удаляет (окончательно) удаленный объект света
SX_LIB_API void SLight_DelDel(ID key);						

//! возвращает идентификатор, если был передан, иначе <0
SX_LIB_API ID SLight_DelGetIDArr(
	ID key,		//!< порядковый номер объекта света в массиве удаленного света
	ID idOwner,	//!< идентификатор того что выдало идентификатор массива
	int iHow	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//!@}

//!@} sxlight_light

#endif

//!@} sxlight
