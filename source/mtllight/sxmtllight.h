
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxmtllight - библиотеки освещения и материалов
*/

/*! \defgroup sxmtllight sxmtllight - библиотека освещения и материалов
@{
*/

#ifndef __sxmtlight
#define __sxmtlight

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

/*! \name Базовые функции библиотеки
@{*/

SX_LIB_API long SML_0GetVersion();			//!< версия подсистемы
SX_LIB_API void SML_Dbg_Set(report_func rf);//!< установить функцию обработки сообщений

//! инициализция подсистемы
SX_LIB_API void SML_0Create(
	const char* name,				//!< присваиваемое имя
	IDirect3DDevice9* device,		//!< dx устройство (уже инициализированное)
	const char* std_path_material,	//!< абсолютный путь до директории с материалами
	const char* std_path_mesh,		//!< абсолютный путь до директории с мешами
	float2_t* winsize,				//!< размер окна в который происходит рендер
	float projfov,					//!< fov проекции
	bool is_unic = true				//!< должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SML_0Kill();	//!< уничтожить подсистему

//!@}

/*! \name Обработка потери/восстановления устройства
@{*/

SX_LIB_API void SML_OnLostDevice();	//!< вызывать при потрете устройства

//! вызывать при восстановлении устройства, с передачей параметров области в которую рисуем
SX_LIB_API void SML_OnResetDevice(
	int width,		//!< ширина окна
	int heigth,		//!< высота окна
	float projfov	//!< fov угол
	);

//!@}

//! обновление данных всей библиотеки
SX_LIB_API void SML_Update(
	DWORD timeDelta,	//!< время рендера кадра в млсек
	float2_t* winsize,	//!< размер окна рендера
	float2_t* nearfar,	//!< ближняя и дальняя плоскости отсечения камеры
	float3* poscam,		//!< позиция наблюдателя
	float4x4* mview,	//!< матрица вида наблюдателя
	float projfov		//!< fov угол
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

/*! \name Сила света 
Cила света это вклад освещенности от данного источника.
Если светит солнце, и оно максимально по силе (#LIGHTS_GLOBAL_MAX_POWER), то локальные источники света будут вносить ничтожный вклад,
однако, так как это real-time просчеты, были внесены упрощения:
 сила света в шейдеры передается как sqrt(power/#LIGHTS_GLOBAL_MAX_POWER), дабы сократить разрыв между экстремумами сил света.
Таким образом и самый слабый и самый сильный в по одиночке будут вносить свой вклад,
однако будет видно некоторое дополнение освещения к глобальному (при максимальной силе) от локальных источников сила которых близка к максимальной (#LIGHTS_LOCAL_MAX_POWER)
@{*/

#define LIGHTS_GLOBAL_MAX_POWER 200.f	/*!< максимальная сила глобального света */
#define LIGHTS_LOCAL_MAX_POWER 100.f	/*!< максимальная сила локального света */

//!@}

#define LIGHTS_GLOBAL_STD_RADIUS 999999	/*!< радиус глобального источника */
#define LIGHTS_POS_G_MAX 1000	/*!< отдаленность глобального света (нужно для корректности теней) */

//! типы источников света
enum LightsTypeLight
{
	ltl_none = -1,	//!< не установленный
	ltl_global,		//!< глобальный
	ltl_point,		//!< точечный
	ltl_direction	//!< направленный
};

//! типы теней источников света
enum LightsTypeShadow
{
	lts_none = -1,	//!< без теней
	lts_static,		//!< статичные тени, тень обновляется первые несколько кадров (для корректности), далее не обновляется
	lts_dynamic,	//!< полностью динамические тени
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

/*! \name Загрузка/сохранение списка света
 \note Файл загрузки/сохранения света это простой конфигурационный файл (ini) который можно обрабатывать посредством #ISXLConfig
 \todo Расписать организацию конфигурационного файла загрузки/сохранения для списка света
@{*/

SX_LIB_API void SML_LigthsSave(const char* path);	//!< сохранить список света
SX_LIB_API void SML_LigthsLoad(const char* path);	//!< загрузить список света

//!@}

SX_LIB_API void SML_LigthsClear();		//!< очистить список света
SX_LIB_API long SML_LigthsGetCount();	//!< общее количество света
SX_LIB_API ID SML_LigthsGetIDOfKey(ID key);	//!< получить id по ключу (порядковому номеру (key) в пределах [0, #SML_LigthsGetCount)), к примеру в случае полного обхода массива

//! создать точечный свет (светит во все стороны)
SX_LIB_API ID SML_LigthsCreatePoint(
	float3* center,		//!< центр света (мировая)
	float power,		//!< сила света
	float dist,			//!< дистанция на которую распространяется свет
	float3* color,		//!< цвет в пределах 0-1
	bool isglobal,		//!< глобальный ли свет (типа солнца)
	bool is_shadowed	//!< отбрасвыает ли тени свет
	);

//! создать направленный свет
SX_LIB_API ID SML_LigthsCreateDirection(
	float3* pos,		//!< позиция света (мировая)
	float power,		//!< сила света
	float dist,			//!< дистанция на которую распространяется свет
	float3* color,		//!< цвет в пределах 0-1
	float3* dir,		//!< направление взгляда (базис float3(0,-1,0))
	float top_radius,	//!< верхний (начальный) радиус
	float angle,		//!< угол
	bool is_shadow		//!< отбрасвыает ли тени свет
	);

SX_LIB_API LightsTypeLight SML_LigthsGetType(ID id);	//!< возвращает тип света

SX_LIB_API ID SML_LigthsCreateCopy(ID id);		//!< создать копию объекта и вернуть на нее id
SX_LIB_API void SML_LigthsDeleteLight(ID id);	//!< удаление света

SX_LIB_API char* SML_LigthsGetName(ID id);					//!< возвращает имя света
SX_LIB_API void SML_LigthsSetName(ID id, const char* name);	//!< устанавливает имя света

/*! пересоздание света, аргументы аналогичны #SML_LigthsCreatePoint, id - идентификатор пересоздаваемого света, либо значение <0 чтобы создать новый свет
 \return Если id>=0 (то есть идет пересоздание света) то вернет этот же id, иначе будет создан новый свет и функция вернет его идентификатор
*/
SX_LIB_API ID SML_LigthsReCreatePoint(ID id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed);

/*! пересоздание света, аргументы аналогичны #SML_LigthsCreateDirection, id - идентификатор пересоздаваемого света, либо значение <0 чтобы создать новый свет
\return Если id>=0 (то есть идет пересоздание света) то вернет этот же id, иначе будет создан новый свет и функция вернет его идентификатор
*/
SX_LIB_API ID SML_LigthsReCreateDirection(ID id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow);

/*! \name Работа с мешем источника света
Привязка осуществляется статической загружаемой модели к позиции света (а не света к мешу источника).
Привязывается центр определенной (выбранной, а по умолчанию нулевой) подгруппы.
@{*/

SX_LIB_API bool SML_LigthsExistsSource(ID id);					//!< есть ли источник у света
SX_LIB_API void SML_LigthsBindToGroupSource(ID id, ID group);	//!< привязка (центрирование) подгруппы источнка к позиции света
SX_LIB_API ID SML_LigthsGetBindedGroupSource(ID id);			//!< номер подгруппы которая привязана к позиции света
SX_LIB_API long SML_LigthsGetCountGroupSource(ID id);			//!< общее количество подгрупп в меше
SX_LIB_API const char* SML_LigthsGetPathNameSource(ID id);		//!< относительный путь до файла с мешем
SX_LIB_API const char* SML_LigthsGetNameGroupSource(ID id, int group);//!< имя подгруппы меша, group - номер подгруппы (нумерация с нуля)
SX_LIB_API void SML_LigthsLoadSource(ID id, const char* path);	//!< загрузить меш по относительнмоу пути path, и привязать по дефолту 0 подгруппу

//!@}

/*! \name Рендер
@{*/
//{

/*! отрисовка всех источников света со всеми подгруппами, за ислючение подгруппы источника которая привязана к поции света */
SX_LIB_API void SML_LigthsRenderAllExceptGroupSource(ID id, DWORD timeDelta);

/*! отрисовка меша источника света, render_bind_group - рисовать ли ту группу которая привязана к свету */
SX_LIB_API void SML_LigthsRenderSource(ID id, bool render_bind_group, DWORD timeDelta);

/*! отрисовка ограничивающего объема */
SX_LIB_API void SML_LigthsRender(ID id, DWORD timeDelta);

//!@}


SX_LIB_API void SML_LigthsGetColor(ID id, float3* color);	//!< в color записывает цвет света
SX_LIB_API void SML_LigthsSetColor(ID id, float3* color);	//!< устанавливает цвет света

//! получить в pos позицию света
SX_LIB_API void SML_LigthsGetPos(
	ID id,				//!< идентификатор света
	float3* pos,		//!< инициализированный вектор в который будет записана позиция
	bool for_mesh,		//!< true - позиция меша источника, false - позиция света
	bool greal = false	//!< true - нужны реальные координаты, false - нужны углы, только для глобального света, во всех остальных случаях не имеет смысла
	);

//! установить позицию свету
SX_LIB_API void SML_LigthsSetPos(
	ID id,				//!< идентификатор света
	float3* pos,		//!< позиция в которую будет перемещен свет
	bool greal = false	//!< true - установка реальных координат, false - установка углов, только для глобального света, во всех остальных случаях не имеет смысла
	);

/*! установка силы света, необходимо ориентироваться пределами: 
 - для глобальных не более #LIGHTS_GLOBAL_MAX_POWER
 - для локальных не более #LIGHTS_LOCAL_MAX_POWER
*/
SX_LIB_API void SML_LigthsSetPower(ID id, float power);	
SX_LIB_API float SML_LigthsGetPower(ID id);		//!< получить силу света
SX_LIB_API float SML_LigthsGetPowerDiv(ID id);	//!< возвращает "гиперболизированное значение силы света" для отправки в шейдер

SX_LIB_API float SML_LigthsGetDist(ID id);		//!< возвращает дистанцию на которую распространяется свет

//! установка новой дистанции
SX_LIB_API void SML_LigthsSetDist(
	ID id,					//!< идентификатор света
	float dist,				//!< новая дистанция(радиус для точечного и высота для направленного)
	bool is_create = true	//!< нужно ли пересоздание объема? (если новая дистанция больше чем текущий ограничивающий объем то надо, в других случаях по усмотрению)
	);

//! получить повороты света/источника
SX_LIB_API void SML_LigthsGetRot(
	ID id,			//!< идентификатор света
	float3* rot,	//!< инициализирвоанный вектор в который будут записаны значения углов поворотов (в радианах) для каждой оси
	bool rot_mesh	//!< true - поворот для меша источника света, false - поворот света
	);

//! установить поворот для света/источника
SX_LIB_API void SML_LigthsSetRot(
	ID id,			//!< идентификатор света
	float3* rot,	//!< вектор с углами поворотов (в радианах) для каждой из осей
	bool rot_mesh	//!< true - поворот для меша источника света, false - поворот света
	);

//! получить направление взгляда света/истоника
SX_LIB_API void SML_LigthsGetDir(
	ID id,			//!< идентификатор света
	float3* dir,	//!< инициализирвоанный вектор в который будет записано направление взгляда
	bool rot_mesh	//!< true - вектора направления взгляда меша источника света, false - вектор света
	);

//! установить направление взгляда света/источника
SX_LIB_API void SML_LigthsSetDir(
	ID id,			//!< идентификатор света
	float3* dir,	//!< вектор направления взгляда
	bool rot_mesh	//!< true - вектора направления взгляда меша источника света, false - вектор света
	);

/*! \name Управление видимостью
 \note Отдельный просчет - считает индивидуально для света и тут же возвращает результат просчетов
 \note Предварительный просчет - просчет для всего света (к примеру в другом потоке)
@{*/

SX_LIB_API bool SML_LigthsComVisibleForFrustum(ID id, ISXFrustum* frustum);	//!< виден ли свет в данном фрустуме frustum (отдельный просчет)
SX_LIB_API float SML_LigthsComDistFor(ID id, float3* viewpos);				//!< дистанция от viewpos до начала света (отдельный просчет)

//! просчет видимости и расстояния от наблюдателя viewpos до начала света, для каждого света (предварительный просчет)
SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* viewpos);
SX_LIB_API bool SML_LigthsGetVisibleForFrustum(ID id);	//!< виден ли свет в просчитанном фрустуме
SX_LIB_API float SML_LigthsGetDistFor(ID id);			//!< просчитанная дистанция от позиции наблюдателя до начала света

//!@}

//включение/выключение света
SX_LIB_API bool SML_LigthsIsEnable(ID id);				//!< включен ли свет с идентификатором id
SX_LIB_API void SML_LigthsSetEnable(ID id, bool val);	//!< установить всключен (true) или отключен (false) свет  с идентификатором id

SX_LIB_API ID SML_LigthsGetGlobal();					//!< возвращает идентификатор глобального источника света, если он есть, иначе -1

/*! \name Настройки для направленного света
@{*/
//угол для направленного источника
SX_LIB_API float SML_LigthsGetAngle(ID id);				//!< возвращает угол, актуально только для направленного и света
SX_LIB_API void SML_LigthsSetAngle(ID id, float angle);	//!< установить угол, актуально только для направленного и света

//верхний радиус для направленного источника
SX_LIB_API float SML_LigthsGetTopRadius(ID id);					//!< возвращает верхний радиус, актуально только для направленного и света
SX_LIB_API void SML_LigthsSetTopRadius(ID id, float top_radius);//!< устанавливает верхний радиус, актуально только для направленного и света

//!@}

/*! \name Настройки для точечного света
@{*/

/*! установить "рисовать ли в указанную грань cubemap тени"
 \warning Актуально только когда свет может отбрасывать тени
 */
SX_LIB_API void SML_LigthsSetEnableCubeEdge(
	ID id,			//!< идентификатор света
	int edge,		//!< номер стороны/грани cubemap [0,5]
	bool enable		//!< true - рисовать, false - не рисовать
	);

//! получить текущее значение "рисовать ли в указанную грань cubemap тени"
SX_LIB_API bool SML_LigthsGetEnableCubeEdge(
	ID id,		//!< идентификатор света
	int edge	//!< номер стороны/грани cubemap [0,5]
	);

//!@}


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

SX_LIB_API bool SML_LigthsIsShadow(ID id);					//!< отбрасывает ли свет тени
SX_LIB_API void SML_LigthsSetShadow(ID id, bool is_shadow);	//!< установка наличия теней, автоматическое определение типа техники теней

SX_LIB_API bool SML_LigthsGet4Or3SplitsG(ID id);				//!< возвращает true В случае если установлено 4 сплита для PSSM, иначе (установлено 3 сплита) false
SX_LIB_API void SML_LigthsSet4Or3SplitsG(ID id, bool bf4or3);	//!< установка количества сплитов (false - 3, true - 4) для PSSM

/*! устанавливает шейдеры и передает им данные на основании типа материала, поддерживаются все типы MTL_TYPE_*/
SX_LIB_API void SML_LigthsShadowSetShaderOfTypeMat(ID id, int typemat, float4x4* wmat);

/*! \name Управление обновлениями теней
 \note Так как основная задумка это real-time рендер то для этого нужны упрощения, одним из таковых является обновление теней от света, которое может происходить не каждый кадр
@{*/

//! установка динамики обработки теней
SX_LIB_API void SML_LigthsSetTypeShadowed(
	ID id,					//!< идентификатор света
	LightsTypeShadow type	//!< значение из перечисления #LightsTypeShadow
	);

SX_LIB_API LightsTypeShadow SML_LigthsGetTypeShadowed(ID id);	//!< возвращает тип динамики теней (одно из значений #LightsTypeShadow)

//! производит обработку данных затем возвращает разрешено ли обновлять тени, увеличивает внутрений счетчик, поэтому необходимо вызывать только когда надо
SX_LIB_API bool SML_LigthsUpdateCountUpdate(
	ID id,				//!< идентификатор света
	float3* viewpos,	//!< позиция камеры для которой обновляем, на основании расстояния будет произведен расчет обновления
	int ghow = -1		//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! разрешено ли обновлять тени, в отличии от #SML_LigthsUpdateCountUpdate не увеличивает внутренний счетчик
SX_LIB_API bool SML_LigthsAllowedRender(
	ID id,			//!< идентификатор света
	int ghow = -1	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

SX_LIB_API void SML_LigthsSetNullCountUpdate(ID id);	//!< сбросить счетчик обновлений и обновлять до предела тени

//!@}


/*! \name Управление циклами рендера в карты глубин
 Для локального направленного света:
 \code
 SML_LigthsInRenderBegin(id);
 //рендер того что должно отбросить тени
 SML_LigthsInRenderEnd(id);
 \endcode
 Для глобального и точечного:
 \code
 SML_LigthsInRenderBegin(id);
 SML_LigthsInRenderPre(id, cube);
 //рендер того что должно отбросить тени
 SML_LigthsInRenderPost(id, cube);
 SML_LigthsInRenderEnd(id);
 \endcode
 Где cube для точечного это номер стороны cubemap [0-5] в которую будет осуществлен рендер, а для глобального это номер сплита.
@{*/

//
SX_LIB_API void SML_LigthsInRenderBegin(ID id);	//!< подготовительные процедуры
SX_LIB_API void SML_LigthsInRenderEnd(ID id);	//!< завершающие процедуры

//! подготовительные процедур перед рендером для сплита/стороны
SX_LIB_API void SML_LigthsInRenderPre(
	ID id,		//!< идентификатор света
	int cube	//!< для точечного света - сторона cubemap [0-5] для глобального света номер сплита
	);	

//! завершающие процедур перед рендером для сплита/стороны
SX_LIB_API void SML_LigthsInRenderPost(
	ID id,		//!< идентификатор света
	int cube	//!< для точечного света - сторона cubemap [0-5] для глобального света номер сплита
	);

//!@}

SX_LIB_API void SML_LigthsNullingShadow();			//!< обнуление карты теней, необходимо вызывать перед генерацией теней посредством функции #SML_LigthsGenShadow
SX_LIB_API void SML_LigthsGenShadow(ID id);			//!< генерация тени на основе уже отработанной информации о затенении, перед вызовов этой функции необходимо вызывать #SML_LigthsNullingShadow
SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow();//!< возвращает текстуру с тенями (если вызывались функции сглаживания) то вренутся сглаженная

//! сглаживание текущей тени
SX_LIB_API void SML_LigthsSoftShadow(
	bool randomsam,			//!< использовать ли смещение по случайному направлению
	float size,				//!< коэфициент увеличения размера пикселя для размытия
	bool isfirst = false	//!< впервые ли вызывается размытие для текущей тени, каждый раз когда тень еще не сглажена но сгенерирована нужно указывать false
	);

//! возвращает указатель фрустума света
SX_LIB_API ISXFrustum* SML_LigthsGetFrustum(
	ID id,	//!< идентификатор света
	int how	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! обновление фрустума глобального света
SX_LIB_API void SML_LigthsUpdateFrustumsG(
	ID id,		//!< идентификатор света
	int split,	//!< номер сплита
	float3* pos,//!< позиция наблюдателя
	float3* dir	//!< направление взгляда наблюдателя
	);

/*! \name Управление настройками для генерации глубины/тени
@{*/

SX_LIB_API float SML_LigthsGetBias(ID id);				//!< получить текущее смещение (bias) при генерации глубины
SX_LIB_API void SML_LigthsSetBias(ID id, float bias);	//!< установить текущее смещение (bias) при генерации глубины

SX_LIB_API void SML_LigthsSetBlurPixel(ID id, float blur_pixel);	//!< установить размерность размытия (в пикселя) при геренации тени
SX_LIB_API float SML_LigthsGetBlurPixel(ID id);						//!< получить текущее значение размерности (в пикселях) при генерации тени

SX_LIB_API void SML_LigthsSetShadowLocalNear(ID id, float slnear);	//!< установить значение ближней плоскостью отсечения для теней (локальный свет)
SX_LIB_API float SML_LigthsGetShadowLocalNear(ID id);				//!< получить текущее значение ближней плоскостью отсечения для теней (локальный свет)

/*! установка дальней плоскостью отсечения для теней (локальный свет)
 \note Иногда возникают артефакты и если отрегулировать это значение то все становится на свои места, тесты показали что в основном при ошибках, увеличение этого значения решало проблемы
*/
SX_LIB_API void SML_LigthsSetShadowLocalFar(
	ID id,		//!< идентификатор света
	float slfar	//!< значение дальней плоскости отсечения (дальность видимости теней), обычно это значение = дальность света
	);

SX_LIB_API float SML_LigthsGetShadowLocalFar(ID id);	//!< возвращает значение дальней плоскости отсечения (дальности видимости теней)

//!@}


/*! \name Управление массивами обработки данных для рендера (к примеру от геометрии, растительности и прочего)
 Cделано для того чтобы отделить просчеты видимости (если они нужны) от рендера. 
 К примеру можно рассчитать видимости в одном потоке а рисовать в основном.
@{
*/

//! возвращает идентификатор в случае успеха, иначе <0
SX_LIB_API ID SML_LigthsGetIDArr(
	ID id,		//!< идентификатор света
	ID inid,	//!< идентификатор объекта/системы вообще того что выдало идентификатор массива
	int how		//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	);

//! установить идентификатор
SX_LIB_API void SML_LigthsSetIDArr(
	ID id,		//!< идентификатор света
	ID inid,	//!< идентификатор объекта/системы вообще того что выдало идентификатор массива
	int how,	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
	ID id_arr	//!< номер массива обработки от объекта который надо будет обрабатывать на видимость
	);

//!@}

//!@} sxmtllight_light_shadow

//! обработка hdr
SX_LIB_API void SML_LigthsComHDR(
	DWORD timeDelta,			//!< время рендера кадра в млсек
	float factor_adapted = 10	//!< на сколько быстро "глаз" привыкнет к освещению
	);

/*! \name Функции обработки удаленного света
 При удалении света он помещается в специальный массив удаленных объектов, 
это связано с тем что свет может содержать в себе данные которые требуют очистки памяти вне данной реализации, 
к примеру идентификаторы массивов просчетов видимости. \n
 Поэтому необходимо вручную удалять (уже после удаленяи функцией #SML_LigthsDeleteLight) удаленные объекты света.
 \note Доступ к элементам массива удаленных объектов света осуществляется посредством порядкового номера в массиве (ключа key), то есть можно пройтись по массиву циклом.
@{*/

SX_LIB_API long SML_LigthsDelGetCount();						//!< возвращает количество удаленных объектов света которые требуют удаления
SX_LIB_API LightsTypeLight SML_LigthsDelGetType(ID key);		//!< возвращает тип удаленного объекта света, значения из #LightsTypeLight
SX_LIB_API void SML_LigthsDelDel(ID key);						//!< удаляет (окончательно) удаленный объект света

//! возвращает идентификатор, если был передан, иначе <0
SX_LIB_API ID SML_LigthsDelGetIDArr(
	ID key,	//!< порядковый номер объекта света в массиве удаленного света
	ID inid,//!< идентификатор того что выдало идентификатор массива
	int how	//!< номер сплита для глобального, 0 для направленного, и номер стороны для точечного
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
	ds_rt_color = 0,		//!< цвет rgba8
	ds_rt_normal,			//!< нормали (включая микрорельеф) rfb10a2
	ds_rt_param,			//!< параметры освещения rgba8
	ds_rt_depth,			//!< глубина r16f
	ds_rt_depth0,			//!< глубина r16f
	ds_rt_depth1,			//!< глубина r16f

	ds_rt_ambient_diff,		//!< эмбиент цвет (цвет света rgb) и диффузное освещение (a) rgba16f
	ds_rt_specular,			//!< блики r16f
	ds_rt_scene_light_com,	//!< освещенная сцена rgba16
	ds_rt_scene_light_com2,	//!< освещенная сцена rgba16

	ds_rt_adapted_lum_curr	//!< текущий rt адаптации глаза к освещению r16f
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
enum MtlTypeReflect
{
	mtr_none,			//!< нет отражения
	mtr_plane,			//!< плоское
	mtr_cube_dynamic,	//!< объемное динамическое
	mtr_cube_static		//!< объемное статическое, обработка идет первые несколько кадров, затем не обновляется
};

//! типы прозрачности
enum MtlTypeTransparency
{
	mtt_none,			//!< нет прозрачности
	mtt_alpha_lighting,	//!< освещаяемая прозрачность
};

//! тип модели материала
enum MtlTypeModel
{
	tms_static = 0,	//!< статическая геометрия
	tms_grass,		//!< растительность трава
	tms_tree,		//!< растительность дерево
	tms_skin,		//!< анимационная модель

	//! значение по умолчанию
	tms_default = tms_static
};

//! физический тип материала
enum MtlPhysicType
{
	mpt_concrete = 0,	//!< бетон
	mpt_metal,			//!< металл
	mpt_glass,			//!< стекло
	mpt_plastic,		//!< пластик
	mpt_tree,			//!< дерево
	mpt_flesh,			//!< плоть
	mpt_ground_sand,	//!< земля/песок

	//! значение по умолчанию
	mpt_default = mpt_concrete,
};

//! данные отправляемые в шейдеры
enum MtlTransShaderStd
{
	mtss_mat_w = 0,	//!< мировая матрица (трансформации модели), world
	mtss_mat_v,		//!< матрица вида, view
	mtss_mat_p,		//!< матрица проекции, projection
	mtss_mat_wv,	//!< world * view
	mtss_mat_wvp,	//!< world * view * projection
	mtss_campos,	//!< позиция камеры/наблюдателя
	mtss_timedelta,	//!< данные о времени float2(CountTimeRender,TimeDelta)
	mtss_winsize,	//!< размеры окна рендера

	mtss_ud			//!< пользовательские данные (float4)
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
	const char* name,	//!< имя_материала.расширение
	MtlTypeModel mtl_type = MtlTypeModel::tms_static	//!< тип модели материала на случай если материал не будет загружен/найден
	);

//! перезагрузка материала
SX_LIB_API void SML_MtlReloadMaterial(
	ID id,					//!< идентификатор материала
	const char* name = 0	//!< на случай замены существующего материала другим
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
	bool clear_ref_del	//!< очищать ли очередь удаления отражений, на тот случай если идентификаторы в отражениях больше не действительны
	);	

SX_LIB_API long SML_MtlGetCount();					//!< возвращает общее количество материалов
SX_LIB_API MtlTypeModel SML_MtlGetTypeModel(ID id);	//!< возвращает тип модели материала по id

/*! установка типа модели материала по id
 \warning меняется только внутренний флаг (определение)!!! все остальное для данного типа надо загружать вручную, сделано для больших возможностей построения материалов
*/
SX_LIB_API void SML_MtlSetTypeModel(ID id, MtlTypeModel type_model);

//! установка параметров материала по id, вызывается перед DIP
SX_LIB_API void SML_MtlRender(
	ID id,			//!< идентификатор материала
	float4x4* world	//!< указатель на мировую матрицу модели, либо 0 - тогда будет принята единичная матрица
	);

//! стандартная отрисовка материала, используются стандартные шейдеры, нужно для теней, отражений и прочего
SX_LIB_API void SML_MtlRenderStd(
	MtlTypeModel type,			//!< тип материала из MtlTypeModel
	float4x4* world,			//!< мировая матрица трансформации, либо 0 и будет применена единичная матрица
	ID slot,					//!< текстурный слот в который установить текстуру
	ID id_mtl					//!< идентификатор материала из которого будет браться текстура
	);

//! установка параметров материала для рендера источника света
SX_LIB_API void SML_MtlRenderLight(
	float4_t* color,	//!< rgb - цвет, w зарезервирован 
	float4x4* world		//!< аналогично #SML_MtlRender
	);

SX_LIB_API void SML_MtlSetMainTexture(ID slot, ID id);	//!< установить текстуру из материала id в текстурный слот slot

//физический тип материала
SX_LIB_API void SML_MtlSetPhysicMaterial(ID id, MtlPhysicType type);	//!< установка физического типа материала
SX_LIB_API MtlPhysicType SML_MtlGetPhysicMaterial(ID id);				//!< возвращает текущий тип физического материала

//! возвращает id стандартного материала для определенной модели материалов указанной в #MtlTypeModel 
SX_LIB_API ID SML_MtlGetStdMtl(MtlTypeModel type_model);

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
SX_LIB_API void SML_MtlSetCurrCountSurf(int count);	//!< установить текущее количество отрисованных полупрозрачных поверхностей

/*! принудительное включение альфа теста (isat - true - включить, false - выключить) при устновке параметров материала, 
к примеру для рендера полупрозрачных поверхностей, чтобы отсечь (условно) полностью прозрачные пиксели
*/
SX_LIB_API void SML_MtlSetForceblyAlphaTest(bool isat);	
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
	int cube,	//!< условно - сторона куба/сплита для которой записываем 
	ID idarr	//!< идентификатор который надо записать
	);

//! возвращает идентификатор который был записан для сущности (inid) для стороны куба/сплита (cube), если вообще записывался, если не записывался то <0
SX_LIB_API ID SML_MtlRefGetIDArr(
	ID id,		//!< идентификатор материала
	ID inid,	//!< идентификатор сущности для которой было записывано (геометрия, растительность и прочее)
	int cube	//!< условно - сторона куба/сплита для которой было записывано 
	);

/*! \name Управление удаленными отражениями
@{*/

SX_LIB_API int SML_MtlDelRefGetCount();	//!< возвращает количество удаленных материалов с отражениями
//! возвращает идентификатор который был записан для сущности (inid) для стороны куба/сплита (cube)
SX_LIB_API ID SML_MtlDelRefGetIDArr(
	ID key,		//!< порядковый номер удаленного материала с отражением
	ID inid,	//!< идентификатор сущности для которой было записано (геометрия, растительность и прочее)
	int cube	//!< условно - сторона куба/сплита для которой было записывано
	);
SX_LIB_API void SML_MtlDelRefClear();	//!< очистка массива удаленных материалов с отражениями

//!@}

//! обработка статистики рендера для текущего матриала с отражением
SX_LIB_API bool SML_MtlRefUpdateCountUpdate(
	ID id,				//!< идентификатора материала
	float3_t* viewpos	//!< позиция наблюдателя, viewpos == 0 вернет false
	);

/*! обнуление статистикки рендера для данного материала с отражением, 
полезно в случаях каких-либо изменений, чтобы эти изменения были видны и в отражениях (актуально для статических отражений)
*/
SX_LIB_API void SML_MtlRefNullingCountUpdate(ID id);

SX_LIB_API void SML_MtlRefSetMinMax(ID id, float3_t* min, float3_t* max);//!< установка экстремумов материала

//! возвращает фрустум отражения, cube - сторона куба
SX_LIB_API ISXFrustum* SML_MtlRefGetfrustum(
	ID id,		//!< идентификатора материала
	int cube	//!< сторона куба, если отражение плоское то необходимо указать в cube передать 0, иначе вернет NULL
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
	D3DXPLANE* plane	//!< плоскость относительно которой будет считаться отражение
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
	float3_t* center	//!< центр (позиция) подгруппы модели - откуда будет рендер отражений
	);	

//! установка частных настроек для стороны куба (cube)
SX_LIB_API void SML_MtlRefCubePreRender(
	ID id,			//!< идентификатор материала
	int cube,		//!< сторона куба [0,5]
	float4x4* world	//!< мировая матрица модели 
	);

//между этим набором функций (Begin-pre ... post-End) необходимо помещать код рендера всего того что необходимо поместить в текстуру отражений
SX_LIB_API void SML_MtlRefCubePostRender(ID id, int cube);	//!< завершающие частные моменты для стороны куба

/*! завершающие общие моменты
 \note если viewpos == 0 обновления статисктики рендера для текущего материала с отражением не произойдет, 
и дабы рационально обрабатывать и статические и динамические отражения необходимо до следующего рендера отражений вызвать SML_MtlRefUpdateCountUpdate
*/
SX_LIB_API void SML_MtlRefCubeEndRender(
	ID id,				//!< идентификатор материала
	float3_t* viewpos	//!< позиция наблюдателя, необходимо для обработки разрешения на следующий рендер
	);	

SX_LIB_API IDirect3DCubeTexture9* SML_MtlRefCubeGetTex(ID id);	//!< возвращает cube текстуру отражением (если оно есть, иначе 0)

//!@}

//!@} sxmtllight_mtl_reflect


/*! \defgroup sxmtllight_mtl_propslayer Cвойства поверхности
 \ingroup sxmtllight_mtl
@{*/

//основная текстура которая накладывается на модель/подгруппу
SX_LIB_API void SML_MtlSetTexture(ID id, const char* path_tex);	//!< установка основной текстуры (загрузка текстуры по имени)
SX_LIB_API void SML_MtlGetTexture(ID id, char* name);			//!< в name записывает имя текстуры


/*! \name Шейдеры для рендера материала
 \warning В текущей версии все шейдеры загружаются в версии 3.0 и требуют в качестве старта указывать функцию main
 \warning Рекомендуется устанавливать и вершинный и пиксельный шейдеры, так как во врем тестов одно из устройств требовало наличия обоих шейдеров либо отсутствия обоих
 \note В аргументах path_vs/path_ps указывается только имя шейдера с расширением, есесно возвращается тоже самое
@{*/

SX_LIB_API void SML_MtlSetVS(ID id, const char* path_vs);	//!< установка вершинного шейдера
SX_LIB_API void SML_MtlGetVS(ID id, char* name);			//!< в name записывает имя текущего вершинного шейдера
SX_LIB_API void SML_MtlSetPS(ID id, const char* path_ps);	//!< установка пиксельного шейдера
SX_LIB_API void SML_MtlGetPS(ID id, char* name);			//!< в name записывает имя текущего пикельного шейдера

//!@}

//использование альфа теста для материала
SX_LIB_API bool SML_MtlGetUsingAlphaTest(ID id);				//!< используется ли альфа тест для материала
SX_LIB_API void SML_MtlSetUsingAlphaTest(ID id, bool is_using);	//!< установка использования альфа теста

/*! освещение материала id
 \note если материал не освещается то будет выведен в цвете накладываемым материалом
*/
SX_LIB_API void SML_MtlSetLighting(ID id, bool is_lighting);	
SX_LIB_API bool SML_MtlGetLighting(ID id);	//!< освещается ли материал

/*! \name Параметры освещения.
 \note Для более точной/тонкой настройки освещения материала рекомендуется использовать текстуру с параметрами освещения 
(к прмиеру когда в одной поверхности разные материалы, к примеру дерево, метал и камень, то в текстуре можно указать параметры освещения для каждого пикселя), 
однако если текстуры нет то будут использованы установленные парметры освещения для всей поверхности
@{*/

//! установка использования текстуры с параметрами освещения
SX_LIB_API void SML_MtlSetIsTextureLighting(ID id, bool is_tex_lighting);
SX_LIB_API bool SML_MtlGetIsTextureLighting(ID id);	//!< используется ли текстура с параметрами для освещения

SX_LIB_API void SML_MtlSetTextureLighting(ID id, const char* path_tex);	//!< загрузка текстуры с парамтерами освещения
SX_LIB_API void SML_MtlGetTextureLighting(ID id, char* path_tex);		//!< в path_tex записывает имя текстуры с параметрами освещения

SX_LIB_API void SML_MtlSetRoughness(ID id, float roughness);	//!< установка шероховатости (0-1)
SX_LIB_API float SML_MtlGetRoughness(ID id);					//!< возвращает текущее значение шероховатости поверхности

SX_LIB_API void SML_MtlSetThickness(ID id, float thickness);	
SX_LIB_API float SML_MtlGetThickness(ID id);

//f0 - отражательная способность поверхности (0-1)
SX_LIB_API void SML_MtlSetF0(ID id, float f0);	//!< установка отражательной способности поверхности (0-1)
SX_LIB_API float SML_MtlGetF0(ID id);			//!< возвращает текущее значение отражательной способности поверхности

//!@}

/*! \name Полупрозрачность, типы из #MtlTypeTransparency
Под полупрозрачными материалами следует понимать такие материалы, прозрачность которых в среднем находится в пределах 0.5 и минимальный процент полупрозрачного периметра 50%
@{*/

SX_LIB_API void SML_MtlSetTypeTransparency(ID id, MtlTypeTransparency type);	//!< установка типа полупрозрачности
SX_LIB_API MtlTypeTransparency SML_MtlGetTypeTransparency(ID id);				//!< возвращает текущий тип полупрозрачности для материала

//!@}

/*! \name Отражения окружения, типы из #MtlTypeReflect
@{*/

SX_LIB_API void SML_MtlSetTypeReflection(ID id, MtlTypeReflect type);	//!< установка типа отражений
SX_LIB_API MtlTypeReflect SML_MtlGetTypeReflection(ID id);				//!< возвращает текущий тип отражений для материала

//!@}

/*! \name Детальность и микрорельеф
Поверхность может содержать до 4 детальных текстур и до 4 текстур с микрорельефом. 
Данные для определения наложения текстур (детальных и микрорельефных) указываются в маске - 
на каждый канал текстуры маски по одной детальной и микрорельефной текстуре, 
то есть канал маски определет связанные между собой и детальную и микрорельефную текстуры
 \note В аргументах channel - RGBA - 0,1,2,3
@{*/

//маска наложения
SX_LIB_API void SML_MtlSetMaskTex(ID id, const char* path_tex);	//!< загрузка маски наложения
SX_LIB_API void SML_MtlGetMaskTex(ID id, char* path_tex);		//!< в path_tex записывает текущее имя маски наложения

//! загрузка текстуры микрорельефа, на каждый канал
SX_LIB_API void SML_MtlSetMRTex(
	ID id,					//!< идентификатор материала
	int channel,			//!< номер канала RGBA - 0,1,2,3
	const char* path_tex	//!< имя текстуры с расширением
	);

//! в path_tex запишет текущее имя текстуры микрорельефа для канала channel
SX_LIB_API void SML_MtlGetMRTex(ID id, int channel, char* path_tex);

//! загрузка детальной текстуры, на каждый канал
SX_LIB_API void SML_MtlSetDTex(
	ID id,					//!< идентификатор материала
	int channel,			//!< номер канала RGBA - 0,1,2,3
	const char* path_tex	//!< имя текстуры с расширением
	);

//! в path_tex запишет текущее имя детальной текстуры для канала channel
SX_LIB_API void SML_MtlGetDTex(ID id, int channel, char* path_tex);

//!@}

/*! \name Стандартные параметры для отправки в шейдер (в вершинный и пиксельный раздельно), данные из #MtlTransShaderStd
@{*/

//! отправка стадартных данных в вершинный шейдер
SX_LIB_API void SML_MtlSetSTDVS(
	ID id,					//!< идентификатор материала
	MtlTransShaderStd type,	//!< тип значения которое нужно отправить, одно из значений #MtlTransShaderStd
	bool is_send			//!< true - отправлять, false - не отправлять
	);

//! установлена ли отпрвка значения type в вершинный шейдер
SX_LIB_API bool SML_MtlGetSTDVS(ID id, MtlTransShaderStd type);

//! отправка стадартных данных в пиксельный шейдер
SX_LIB_API void SML_MtlSetSTDPS(
	ID id,					//!< идентификатор материала
	MtlTransShaderStd type,	//!< тип значения которое нужно отправить, одно из значений #MtlTransShaderStd
	bool is_send			//!< true - отправлять, false - не отправлять
	);

//! установлена ли отпрвка значения type в пиксельный шейдер
SX_LIB_API bool SML_MtlGetSTDPS(ID id, MtlTransShaderStd type);

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
	int component,	//!< компонента вектора xyzw - 0,1,2,3
	float val		//!< значение компоненты
	);

//! возвращает значение компоненты вектора, который отправляется в вершинный шейдер
SX_LIB_API float SML_MtlGetUDVS(ID id, int component);

//! установка отправки float4 вектора, предназначенного для вершинного шейдера, в пиксельный шейдер
SX_LIB_API void SML_MtlSetUDVS_InPS(ID id, bool is_send_ps);
SX_LIB_API bool SML_MtlGetUDVS_InPS(ID id);	//!< отправляется ли float4 вектор, предназначенный для вершинного шейдера, в пиксельный шейдер


//! установка значения компоненты вектора для пиксельного шейдера
SX_LIB_API void SML_MtlSetUDPS(
	ID id,			//!< идентификатор материала
	int component,	//!< компонента вектора xyzw - 0,1,2,3
	float val		//!< значение компоненты
	);

//! возвращает значение компоненты вектора, который отправляется в пиксельный шейдер
SX_LIB_API float SML_MtlGetUDPS(ID id, int component);

//! установка отправки float4 вектора, предназначенного для пиксельного шейдера, в вершинный шейдер
SX_LIB_API void SML_MtlSetUDPS_InVS(ID id, bool is_send_vs);
SX_LIB_API bool SML_MtlGetUDPS_InVS(ID id);	//!< отправляется ли float4 вектор, предназначенный для пиксельного шейдера, в вершинный шейдер

//!@}

//!@} sxmtllight_mtl_propslayer

//!@} sxmtllight_mtl

#endif

//!@} sxmtllight