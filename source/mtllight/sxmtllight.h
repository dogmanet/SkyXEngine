
#ifndef __sxmlight
#define __sxmlight

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>


#define LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC 3	//максимальное количество обновлений статических источников света, с первый момент рендера

//дистанции при которых будут сниженно или повышено количество отрисовок
//меньше LIGHTS_UPDATE_L0_DIST - обновление будет каждый кадр
//больше LIGHTS_UPDATE_L2_DIST - обновления не будет
#define LIGHTS_UPDATE_L0_DIST 10	//
#define LIGHTS_UPDATE_L1_DIST 30	//
#define LIGHTS_UPDATE_L2_DIST 70	//

//коэфициент обновления сплитов глобального света
//считается: LIGHTS_UPDATE_PSSM_SPLIT*number_split
//есесно первый сплит обновляется каждый кадр, второй через кадр
#define LIGHTS_UPDATE_PSSM_SPLIT 1		
#define LIGHTS_UPDATE_PSSM_SPLIT_5 2	//аналогичный коэфициент обновления только для 5 сплита

#define LIGHTS_LOCAL_STD_TOP_RADIUS 0.1	//стандартное значение верхнего/начального радиуса для направленного света
#define LIGHTS_LOCAL_STD_NEAR 0.1		//стандартное значение ближней плоскости отсечения для теней локального света

//******************
//сила света это вклад освещенности от данного источника
//если светит солнце, и оно максимально по силе (LIGHTS_GLOBAL_MAX_POWER), то локальные источники свтеа будут вносить ничтожный вклад
//однако, так как это real-time просчеты, были внесены упрощения
//сила света в шейдеры передается как sqrt(power/LIGHTS_GLOBAL_MAX_POWER), дабы сократить разрыв между экстремумами сил света
//таким образом и самый слабый и самый сильный в по одиночке будут вносить свой вклад
//однако будет видно некоторое дополнение освещения к глобальному (при максимальнйо силе) от локальных источников сила которых близка к максимально (LIGHTS_LOCAL_MAX_POWER)
//******************
#define LIGHTS_GLOBAL_MAX_POWER 200.f	//максимальная сила глобального света
#define LIGHTS_LOCAL_MAX_POWER 100.f	//максимальная сила локального света
#define LIGHTS_GLOBAL_STD_RADIUS 999999	//радиус глобального источника

#define LIGHTS_POS_G_MAX 1000	//отдаленность глобального света (нужно для корректности теней)

SX_LIB_API long SML_0GetVersion();			//версия подсистемы
SX_LIB_API void SML_Dbg_Set(report_func rf);//установить функцию обработки сообщений

//инициализция подсистемы
SX_LIB_API void SML_0Create(
	const char* name,				//присваиваемое имя
	IDirect3DDevice9* device,		//dx устройство (уже инициализированное)
	const char* std_path_material,	//абсолютный путь до директории с материалами
	const char* std_path_mesh,		//абсолютный путь до директории с мешами
	float2_t* winsize,				//размер окна в который происходит рендер
	float projfov,					//fov проекции
	bool is_unic = true				//должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SML_0Kill();

//ОБРАБОТКА ПОТЕРИ И ВОССТАНОВЛЕНИЯ УСТРОЙТСВА
SX_LIB_API void SML_OnLostDevice();	//вызывать при потрете устройства
//вызывать при восстановлении устройства, с передачей параметров области в которую рисуем
SX_LIB_API void SML_OnResetDevice(
	int width,		//ширина окна
	int heigth,		//высота окна
	float projfov	//fov угол
	);

SX_LIB_API void SML_Update(
	DWORD timeDelta,	//
	float2_t* winsize,	//
	float2_t* nearfar,	//
	float3* poscam,		//
	float4x4* mview,		//
	float projfov		//fov угол
	);

//типы источников света
enum LightsTypeLight
{
	ltl_none = -1,	//не установленный
	ltl_global,		//глобальный
	ltl_point,		//точечный
	ltl_direction	//направленный
};

//типы теней источников света
enum LightsTypeShadow
{
	lts_none = -1,	//без теней
	lts_static,		//статичные тени, тень обновляется первые несколько кадров (для корректности), далее не обновляется
	lts_dynamic,	//полностью динамические тени
};

//НАСТРОЙКИ РАЗМЕРОВ ТЕНЕЙ ДЛЯ СВЕТА
//{
//установка
SX_LIB_API void SML_LigthsSettLCoefSizeDepth(float coef);	//для локального света
SX_LIB_API void SML_LigthsSettGCoefSizeDepth(float coef);	//для глобального света

//возвращение
SX_LIB_API float SML_LigthsGettLCoefSizeDepth();	//локальный свет
SX_LIB_API float SML_LigthsGettGCoefSizeDepth();	//глобальный свет
//}

//обновление внутреннего состояния на основании позиции камеры и матрицы вида
//SX_LIB_API void SML_LigthsUpdate(float3* const_cam_pos,float4x4* camview);

SX_LIB_API void SML_LigthsSave(const char* path);	//сохранить список света
SX_LIB_API void SML_LigthsLoad(const char* path);	//загрузить список света

SX_LIB_API void SML_LigthsClear();		//очистить список света
SX_LIB_API long SML_LigthsGetCount();	//общее количество света
SX_LIB_API long SML_LigthsGetIDOfKey(long key);	//получить id по ключу (порядковому номеру), к примеру в случае полного обхода массива

//создать точечный свет (светит во все стороны)
SX_LIB_API ID SML_LigthsCreatePoint(
	float3* center,		//центр света (мировая)
	float power,		//сила света
	float dist,			//дистанция на которую распространяется свет
	float3* color,		//цвет в пределах 0-1
	bool isglobal,		//глобальный ли свет (типа солнца)
	bool is_shadowed	//отбрасвыает ли тени свет
	);

//создать направленный свет
SX_LIB_API ID SML_LigthsCreateDirection(
	float3* pos,		//позиция света (мировая)
	float power,		//сила света
	float dist,			//дистанция на которую распространяется свет
	float3* color,		//цвет в пределах 0-1
	float3* dir,		//направление взгляда (базис float3(0,-1,0))
	float top_radius,	//верхний (начальный) радиус
	float angle,		//угол
	bool is_shadow		//отбрасвыает ли тени свет
	);

SX_LIB_API LightsTypeLight SML_LigthsGetType(ID id);	//возвращает тип света

SX_LIB_API ID SML_LigthsCreateCopy(ID id);	//создать копию объекта и вернуть на нее id
SX_LIB_API void SML_LigthsDeleteLight(ID id);	//удаление света

//имя света, name - выделенный массив
SX_LIB_API char* SML_LigthsGetName(ID id);
SX_LIB_API void SML_LigthsSetName(ID id, const char* name);

//пересоздание света, аргументы аналогичны создающим функциям
SX_LIB_API ID SML_LigthsReCreatePoint(ID id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed);
SX_LIB_API ID SML_LigthsReCreateDirection(ID id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow);

//РАБОТА С  МЕШЕМ ИСТОЧНИКА СВЕТА
//{
//привязка осуществляется статической загружаемой модели к позиции света
//привязывается центр определенной (выбранной, а по умолчанию нулевой) подгруппы
SX_LIB_API bool SML_LigthsExistsSource(ID id);					//есть ли источник у света
SX_LIB_API void SML_LigthsBindToGroupSource(ID id, int group);	//привязка (центрирование) подгруппы источнка к позиции света
SX_LIB_API long SML_LigthsGetBindedGroupSource(ID id);			//номер подгруппы которая привязана к позиции света
SX_LIB_API long SML_LigthsGetCountGroupSource(ID id);				//общее количество подгрупп в меше
SX_LIB_API const char* SML_LigthsGetPathNameSource(ID id);		//относительный путь до файла с мешем
SX_LIB_API const char* SML_LigthsGetNameGroupSource(ID id, int group);//имя подгруппы меша, group - номер подгруппы (нумерация с нуля)
SX_LIB_API void SML_LigthsLoadSource(ID id, const char* path);	//загрузить меш по относительнмоу пути path, и привязать по дефолту 0 подгруппу
//}

//РЕНДЕР
//{
//отрисовка всех источников света со всеми подгруппами, 
//за ислючение подгруппы источника которая привязана к поции света
SX_LIB_API void SML_LigthsRenderAllExceptGroupSource(ID id, DWORD timeDelta);

//отрисовка меша источника света, render_bind_group - рисовать ли ту группу которая привязана к свету
SX_LIB_API void SML_LigthsRenderSource(ID id, bool render_bind_group, DWORD timeDelta);

//отрисовка ограничивающего объема
SX_LIB_API void SML_LigthsRender(ID id, DWORD timeDelta);
//}

//цвет света
SX_LIB_API void SML_LigthsGetColor(ID id, float3* color);
SX_LIB_API void SML_LigthsSetColor(ID id, float3* vec);

//позиция света
//for_mesh - позиция меша источника
//greal получить реальные координаты, в случае если у нас глобальный источник света
SX_LIB_API void SML_LigthsGetPos(ID id, float3* vec, bool for_mesh, bool greal = false);
SX_LIB_API void SML_LigthsSetPos(ID id, float3* vec, bool greal = false);

//сила света
SX_LIB_API void SML_LigthsSetPower(ID id, float power);
SX_LIB_API float SML_LigthsGetPower(ID id);
SX_LIB_API float SML_LigthsGetPowerDiv(ID id);	//возвращает "гиперболизированное значение силы" для отправки в шейдер

SX_LIB_API float SML_LigthsGetDist(ID id);	//возвращает дистанцию на которую распространяется свет
//установка новой дистанции
//dist - новая дистанция (радиус для точечного и высота для направленного)
//is_create нужно ли пересоздание объема?
SX_LIB_API void SML_LigthsSetDist(ID id, float dist, bool is_create = true);

//повороты света, rot_mesh - повороты меша источника света
SX_LIB_API void SML_LigthsGetRot(ID id, float3* vec, bool rot_mesh);
SX_LIB_API void SML_LigthsSetRot(ID id, float3* vec, bool rot_mesh);

//направление взгляда, rot_mesh - направление меша источника света
SX_LIB_API void SML_LigthsGetDir(ID id, float3* vec, bool rot_mesh);
SX_LIB_API void SML_LigthsSetDir(ID id, float3* vec, bool rot_mesh);

//УПРАВЛЕНИЕ ВИДИМОСТЬЮ
//{
//отдельные просчеты
SX_LIB_API bool SML_LigthsComVisibleForFrustum(ID id, ISXFrustum* frustum);	//виден ли свет в данном фрустуме frustum
SX_LIB_API float SML_LigthsComDistFor(ID id, float3* viewpos);				//дистанция от viewpos до начала света

//предварительные просчеты (к примеру в другом потоке)
//просчет видимости и расстояния от наблюдателя viewpos до начала света, для каждого света
SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* viewpos);
SX_LIB_API bool SML_LigthsGetVisibleForFrustum(ID id);//виден ли свет в просчитанном фрустуме
SX_LIB_API float SML_LigthsGetDistFor(ID id);			//просчитанная дистанция от позиции наблюдателя до начала света
//}

//включение/выключение света
SX_LIB_API int SML_LigthsIsEnable(ID id);
SX_LIB_API void SML_LigthsSetEnable(ID id, bool val);

//угол для направленного источника
SX_LIB_API float SML_LigthsGetAngle(ID id);
SX_LIB_API void SML_LigthsSetAngle(ID id, float angle);

//верхний радиус для направленного источника
SX_LIB_API float SML_LigthsGetTopRadius(ID id);
SX_LIB_API void SML_LigthsSetTopRadius(ID id, float top_radius);

//УПРАВЛЕНИЕ ТЕНЯМИ
//{{
//для генерации теней используется один render target full screen
//для сглаживания используется еще один аналогичный render target и первый render target
//при сглаживании внутри меняется только счетчик, который меняет их местами
//так как используется всего один render target для генерации теней 
//то есесно в текущий момент времени можно сгенерировать и сгладить тень только для одного света

SX_LIB_API bool SML_LigthsIsShadow(ID id);	//отбрасывает ли тени
SX_LIB_API void SML_LigthsSetShadow(ID id, bool is_shadow);	//установка наличия теней

SX_LIB_API bool SML_LigthsGet4Or3SplitsG(ID id);	//
SX_LIB_API void SML_LigthsSet4Or3SplitsG(ID id, bool bf4or3);	//

SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow();	//возвращает текстуру с тенями (если вызывались функции сглаживания) то вренутся сглаженная
SX_LIB_API void SML_LigthsNullingShadow();	//обнуление карты теней

//управление циклами рендера в карты глубин
//{
//для всех видов света, необходимо вызывать для каждого света
SX_LIB_API void SML_LigthsInRenderBegin(ID id);	//подготовительные процедуры
SX_LIB_API void SML_LigthsInRenderEnd(ID id);		//завершающие процедуры

//для глобального и локального света, необходимо вызывать для каждого света
SX_LIB_API void SML_LigthsInRenderPre(ID id, int cube);	//подготовительные процедур перед рендером для сплита/стороны
SX_LIB_API void SML_LigthsInRenderPost(ID id, int cube);	//завершающие процедур перед рендером для сплита/стороны
//}

SX_LIB_API void SML_LigthsShadowSetShaderOfTypeMat(ID id, int typemat, float4x4* wmat);
SX_LIB_API void SML_LigthsGenShadow(ID id);	//генерация тени на основе уже отработанной информации о затенении

//сглаживание текущей тени
//randomsam - использовать ли смещение по случайному направлению
//size - коэфициент увеличения размера пикселя дял размытия
//isfirst - впервые ли вызывается размытие для текущей тени, каждый разкогда тень еще не сглажена но сгенерированна нудо указывать false
SX_LIB_API void SML_LigthsSoftShadow(bool randomsam, float size, bool isfirst = false);

//возвращает указатель на фрустум 
//how - номер сплита для глобального, 0 для направленного, и номер стороны для точечного
SX_LIB_API ISXFrustum* SML_LigthsGetFrustum(ID id, int how);

//обновление фрустума глобального света, split - номер сплита, pos - позиция наблюдателя, dir - направление взгляда наблюдателя
SX_LIB_API void SML_LigthsUpdateFrustumsG(ID id, int split, float3* pos, float3* dir);

//управление смещением при генерации теней
SX_LIB_API float SML_LigthsGetBias(ID id);
SX_LIB_API void SML_LigthsSetBias(ID id, float bias);

//управление размытием при генерации теней
SX_LIB_API void SML_LigthsSetBlurPixel(ID id, float blur_pixel);
SX_LIB_API float SML_LigthsGetBlurPixel(ID id);

//управление ближней плоскостью отсечения для теней (локальный свет)
SX_LIB_API void SML_LigthsSetShadowLocalNear(ID id, float slnear);
SX_LIB_API float SML_LigthsGetShadowLocalNear(ID id);

//управление дальней плоскостью отсечения для теней (локальный свет)
//!!! иногда возникают артефакты и если отрегулировать это значение то все становится на свои места
SX_LIB_API void SML_LigthsSetShadowLocalFar(ID id, float slfar);
SX_LIB_API float SML_LigthsGetShadowLocalFar(ID id);

//управление сторонами для точечног освета (тени)
SX_LIB_API void SML_LigthsSetEnableCubeEdge(ID id, int edge, bool enable);
SX_LIB_API bool SML_LigthsGetEnableCubeEdge(ID id, int edge);

//управление массивами обработки данных дл рендера (к примеру от геометрии, растительности и прочего)
//сделано для того чтобы отделить просчеты видимости (если они нужны) от рендера
//к примеру можно рассчитать видимости в одном потоке а рисовать в основном
//inid - идентификатор объекта/системы вообще того что выдало идентификатор массива
//how - номер сплита для глобального, 0 для направленного, и номер стороны для точечного
//id_arr - номер массива обработки от объекта который надо будет обрабатывать на видимость
SX_LIB_API ID SML_LigthsGetIDArr(ID id, ID inid, int how);
SX_LIB_API void SML_LigthsSetIDArr(ID id, ID inid, int how, ID id_arr);

//управление динамиченостью обработки теней (LIGHTS_TYPE_SHADOWED_DYNAMIC, LIGHTS_TYPE_SHADOWED_STATIC)
SX_LIB_API void SML_LigthsSetTypeShadowed(ID id, LightsTypeShadow type);
SX_LIB_API LightsTypeShadow SML_LigthsGetTypeShadowed(ID id);

//управление обновлениями теней
//{
//производит обработку данных затем возвращает разрешено ли обновлять тени
//viewpos - позиция камеры для которой обновляем, на основании расстояния будет произведен расчет обновления
//ghow - номер сплита для глобального, 0 для направленного, и номер стороны для точечного
SX_LIB_API bool SML_LigthsUpdateCountUpdate(ID id, float3* viewpos, int ghow = -1);

//разрешено ли обновлять тени, ghow аналогичен SML_LigthsUpdateCountUpdate
SX_LIB_API bool SML_LigthsAllowedRender(ID id, int ghow = -1);
SX_LIB_API void SML_LigthsSetNullCountUpdate(ID id);	//сбросить счетчик обновлений и обновлять до предела тени
//}
//}}

//обработка hdr, factor_adapted - на сколько быстро "глаз" привыкнет
SX_LIB_API void SML_LigthsComHDR(DWORD timeDelta, float factor_adapted = 10);


SX_LIB_API long SML_LigthsDelGetCount();
SX_LIB_API int SML_LigthsDelGetType(long key);
SX_LIB_API void SML_LigthsDelDel(long key);
SX_LIB_API ID SML_LigthsDelGetIDArr(long key, ID inid, int how);

/////////////////////////////////////////////////////////////

//типы render targets
enum DS_RT
{
	ds_rt_color = 0,		//цвет	rgba8
	ds_rt_normal,			//нормали (включая микрорельеф)	rfb10a2
	ds_rt_param,			//параметры освещения	rgba8
	ds_rt_depth,			//глубина	r16f
	ds_rt_depth0,			//глубина	r16f
	ds_rt_depth1,			//глубина	r16f

	ds_rt_ambient_diff,		//эмбиент цвет (цвет света) и диффузное освещение rgba16f
	ds_rt_specular,			//блики	r16f
	ds_rt_scene_light_com,	//освещенная сцена	rgba16
	ds_rt_scene_light_com2,	//освещенная сцена	rgba16

	ds_rt_adapted_lum_curr	//текущий rt адаптации глаза к освещению r16f
};

SX_LIB_API ID SML_DSGetRT_ID(DS_RT type);	//id render target по его типу
SX_LIB_API IDirect3DTexture9* SML_DSGetRT(DS_RT type);	//текстура render target по его типу

////////////////////////////////////////////////////////////////

#define MTL_MAX_SIZE_DIR SXGC_LOADTEX_MAX_SIZE_DIR
#define MTL_MAX_SIZE_NAME SXGC_LOADTEX_MAX_SIZE_NAME

#define MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC 3	//максимальное количество обновлений статических отражений, в первый момент рендера

//дистанции при которых будут сниженно или повышено количество отрисовок
//меньше MTL_REF_UPDATE_L0_DIST - обновление будет каждый кадр
//больше MTL_REF_UPDATE_L2_DIST - обновления не будет
#define MTL_REF_UPDATE_L0_DIST 10	//
#define MTL_REF_UPDATE_L1_DIST 30	//
#define MTL_REF_UPDATE_L2_DIST 70	//

//функция альфа теста D3DCMP_GREATEREQUAL
#define MTL_ALPHATEST_FREE_VALUE 64		//значение свободного альфа теста (назначаемого материалом)
#define MTL_ALPHATEST_FORCEBLY_VALUE 8	//значение принудительного альфа теста

//данные значения по умолчанию будут применены к материалами которые не удалось загрузить, либо к стандартным материалам
//занчение шероховатости по умолчанию
#define MTL_LIGHTING_DEFAULT_ROUGHNESS 250.f/255.f
//значение f0 (отражательной способности поверхности) по умолчанию
#define MTL_LIGHTING_DEFAULT_F0 2.f/255.f

//размер текстур с отражениями
#define MTL_REF_TEX_SIZE 512	
//ближняя плоскость отсечения
#define MTl_REF_PROJ_NEAR 0.25
//дальняя плоскость отсечения, или дистанция на которую будет рендер всего в текстуру отражений
#define MTl_REF_PROJ_FAR 100

//ВИРТУАЛЬНЫЕ ДИРЕКТОРИИ
//имя виртуальной директории должно быть единичным символом (сделано для простоты сравнения)
//и как следствие нежелательно использоавть реальную директорию с таким именем для текстур либо материалов
//и реальные директории не должны начинаться с данных символов

//имя виртуальной директории по умолчанию для "ручных" текстур с параметрами освещения
#define MTL_VIRTUAL_DIR_HAND_MTL '0'

//имя виртуальной директории по умолчанию для стандартных (встроенных) материалов
#define MTL_VIRTUAL_DIR_STD_MTL "mtrldef"

//типы отражений
enum MtlTypeReflect
{
	mtr_none,			//нет отражения
	mtr_plane,			//плоское
	mtr_cube_dynamic,	//объемное динамическое
	mtr_cube_static		//объемное статическое, обработка идет первые несколько кадров, затем не обновляется
};

//типы прозрачности
enum MtlTypeTransparency
{
	mtt_none,			//нет прозрачности
	mtt_alpha_lighting,	//освещаяемая прозрачность
};

//тип модели материала
enum MtlTypeModel
{
	tms_static = 0,	//статическая геометрия
	tms_grass,		//растительность трава
	tms_tree,		//растительность дерево
	tms_skin,		//анимационная модель

	//значение по умолчанию
	tms_default = tms_static
};

//физический тип материала
enum MtlPhysicType
{
	mpt_concrete = 0,	//бетон
	mpt_metal,			//металл
	mpt_glass,			//стекло
	mpt_plastic,		//пластик
	mpt_tree,			//дерево
	mpt_flesh,			//плоть
	mpt_ground_sand,	//земля/песок

	//значение по умолчанию
	mpt_default = mpt_concrete,
};

//данные отправляемые в шейдеры
enum MtlTransShaderStd
{
	mtss_mat_w = 0,	//мировая матрица (трансформации модели), world
	mtss_mat_v,		//матрица вида, view
	mtss_mat_p,		//матрица проекции, projection
	mtss_mat_wv,	//world * view
	mtss_mat_wvp,	//world * view * projection
	mtss_campos,	//позиция камеры/наблюдателя
	mtss_timedelta,	//данные о времени float2(CountTimeRender,TimeDelta)
	mtss_winsize,	//размеры окна рендера

	mtss_ud			//пользовательские данные (float4)
};

//загрузка материала, возвращает уникальный идентификатора материала, 
//все материалы оборачиваются в специальную структуру поэтому как таковых дубликатов материалов нет
//name - имя_материала.расширение
//mtl_type - тип модели материала на случай если материал не будет загружен/найден
SX_LIB_API ID SML_MtlLoad(const char* name, MtlTypeModel mtl_type = MtlTypeModel::tms_static);

//перезагрузка материала
//name - на случай замены существующего материала другим
SX_LIB_API void SML_MtlReloadMaterial(ID id, const char* name = 0);

//очитска, удаляет все загруженные материалы (но место в массиве под них остается), все стандартные остаются
//отраения не удаляются а помещаются в очередь удаления, так как могут сожеражть в себе идентификаторы, которые необходимы очистить
//clear_ref_del - очищать ли очередь удаления отражений, на тот случай если идентификаторы в отражениях больше не действительны
SX_LIB_API void SML_MtlClear(bool clear_ref_del);	
SX_LIB_API void SML_MtlSave(ID id);	//сохранить материал по id
SX_LIB_API long SML_MtlGetCount();	//возвращает количество общее материалов
SX_LIB_API MtlTypeModel SML_MtlGetTypeModel(ID id);	//возвращает тип модели материала по id

//установка типа модели материала по id, 
//меняется только внутренний флаг (определение)!!! все остальное для данного типа надо загружать вручную
//сделано для больших возможностей построения материалов
SX_LIB_API void SML_MtlSetTypeModel(ID id, MtlTypeModel type_model);
//SX_LIB_API void SML_MtlUpdate(DWORD timeDelta);	//обновление подсистемы материалов, timeDelta - время в млсек текущего рендера кадра

//установка параметров материала по id, вызывается перед DIP
//world - укзаатель на мировую матрицу модели, либо 0 - тогда будет принята единичная матрица
SX_LIB_API void SML_MtlRender(ID id, float4x4* world);

//установка параметров материала для рендера источника света
//color - rgb - цвет, w зарезервирован, world аналогично SML_MtlRender
SX_LIB_API void SML_MtlRenderLight(float4_t* color, float4x4* world);
SX_LIB_API void SML_MtlSetMainTexture(ID slot, ID id);	//установить текстуру из материала id в текстурный слот slot

//физический тип материала
SX_LIB_API void SML_MtlSetPhysicMaterial(ID id, MtlPhysicType type);
SX_LIB_API MtlPhysicType SML_MtlGetPhysicMaterial(ID id);

//возвращает id стандартного материала для определенной модели материалов указанной в MtlTypeModel 
SX_LIB_API ID SML_MtlGetStdMtl(MtlTypeModel type_model);

//управление количеством полупрозрачных поверхностей
//{{
//каждый выводимый пиксель помечается номером поверхности к которой он относится
//0 - непрозрачная геометрия
//все что больше 0 значит полупрозрачная геометрия

//установка инкремента идентификатора поверхностей
//к примеру для диференциации (разделения) пикселей при смешивании всех слоев изображения
//без разделения по принадлжености к определнной поверхности будет трудно (пока выхода нет) корректно смешать все слои
SX_LIB_API void SML_MtlSetIsIncrCountSurf(bool bf);
SX_LIB_API bool SML_MtlGetIsIncrCountSurf();	//установлен ли инкремент поверхностей
SX_LIB_API void SML_MtlNullingCurrCountSurf();	//обнуление идентификатора отрисованных поверхностей (к примеру при новом кадре)
SX_LIB_API int SML_MtlGetCurrCountSurf();		//текущее количество отрисованных полупрозрачных поверхностей
SX_LIB_API void SML_MtlSetCurrCountSurf(int count);	//установить текущее количество отрисованных полупрозрачных поверхностей

//принудительное включение альфа теста при устновке параметров материала
//к примеру для рендера полупрозрачных поверхностей, чтобы отсечь (условно) полностью прозрачные пиксели
SX_LIB_API void SML_MtlSetForceblyAlphaTest(bool isat);	//установка принудительного альфа теста, isat - true - включить, false - выключить
SX_LIB_API bool SML_MtlGetForceblyAlphaTest();			//включен ли принудительный альфа тест
//}

//ОТРАЖЕНИЯ/REFLECTION
//{{

SX_LIB_API void SML_MtlRefSetIDArr(ID id, ID inid, int cube, ID idarr);
SX_LIB_API ID SML_MtlRefGetIDArr(ID id, ID inid, int cube);

SX_LIB_API int SML_MtlDelRefGetCount();
SX_LIB_API ID SML_MtlDelRefGetIDArr(ID key, ID inid, int cube);
SX_LIB_API void SML_MtlDelRefClear();

//обработка статистики рендера для текущего матриала с отражением, viewpos - позиция наблюдателя, viewpos == 0 вернет false
SX_LIB_API bool SML_MtlRefUpdateCountUpdate(ID id, float3_t* viewpos);

//обнуление статистикки рендера для данного материала с отражением
//полезно в случаях каких-либо изменений, чтобы эти изменения были видны и в отражениях (актуально для статических отражений)
SX_LIB_API void SML_MtlRefNullingCountUpdate(ID id);

SX_LIB_API void SML_MtlRefSetCenter(ID id, float3_t* center);			//установка центра подгруппы
SX_LIB_API void SML_MtlRefSetMinMax(ID id, float3_t* min, float3_t* max);//экстремумы подгруппы
//возвращает фрустум отражения, cube - сторона куба
//если отражение плоское то необходимо указать в cube передать 0, иначе вернет NULL
SX_LIB_API ISXFrustum* SML_MtlRefGetfrustum(ID id, int cube);
SX_LIB_API bool SML_MtlRefIsAllowedRender(ID id);	//разрешен ли рендер отражений для данного материала в текущий момент

//плоское отражение
//{
SX_LIB_API void SML_MtlRefSetPlane(ID id, D3DXPLANE* plane);		//установка плоскости для отражения
SX_LIB_API void SML_MtlRefPreRenderPlane(ID id, float4x4* world);	//первоначальные настройки данных для генерации отражения, world - мировая матрица модели
//между этими функциями необходимо помещать код рендера всего того что необходимо поместить в текстуру отражений
SX_LIB_API void SML_MtlRefPostRenderPlane(ID id);					//завершающие операции
SX_LIB_API IDirect3DTexture9* SML_MtlRefGetTexPlane(ID id);			//возвращает текстуру с плоским отражением (если оно есть, иначе 0)
//}

//объемное (кубическое) отражение
//{
SX_LIB_API void SML_MtlRefCubeBeginRender(ID id);	//установка общих настроек
SX_LIB_API void SML_MtlRefCubePreRender(ID id, int cube, float4x4* world);//установка частных настроек для стороны куба (cube), world - мировая матрица модели 
//между этим набором функций (Begin-pre ... post-End) необходимо помещать код рендера всего того что необходимо поместить в текстуру отражений
SX_LIB_API void SML_MtlRefCubePostRender(ID id, int cube);	//завершающие частные моменты для стороны куба
//завершающие общие моменты
//viewpos - позиция наблюдателя, необходимо для обработки разрешения на следующий рендер
//если viewpos == 0 обновления статисктики рендера для текущего материала с отражением не произойдет
//и дабы рационально обрабатывать и статические и динамические отражения необходимо до следующего рендера отражений вызвать SML_MtlRefUpdateCountUpdate
SX_LIB_API void SML_MtlRefCubeEndRender(ID id, float3_t* viewpos);	

SX_LIB_API IDirect3DCubeTexture9* SML_MtlRefCubeGetTex(ID id);	//возвращает текстуру с кубмап отражением (если оно есть, иначе 0)
//}
//}}

//основная текстура которая накладывается на модель/подгруппу
SX_LIB_API void SML_MtlSetTexture(ID id,const char* path_tex);
SX_LIB_API void SML_MtlGetTexture(ID id, char* name);

//!!!в текущей версии все шейдеры загружаются в версии 3.0 и требуют в качестве старта указывать функцию main
//шейдеры для рендера материала
//!!!рекомендуется устанавливать и вершинный и пиксельный шейдеры
//так как во врем тестов одно из устройств требовало наличия обоих шейдеров либо отсутствия обоих
//path_vs/path_ps указывается только имя шейдера с расширением, есесно возвращается тоже самое
SX_LIB_API void SML_MtlSetVS(ID id, const char* path_vs);
SX_LIB_API void SML_MtlGetVS(ID id, char* name);
SX_LIB_API void SML_MtlSetPS(ID id, const char* path_ps);
SX_LIB_API void SML_MtlGetPS(ID id, char* name);


//свойства поверхности
//{{

//использование альфа теста для материала
SX_LIB_API bool SML_MtlGetUsingAlphaTest(ID id);
SX_LIB_API void SML_MtlSetUsingAlphaTest(ID id, bool is_using);

//освещение материала id
//если материал не освещается то будет выведен в цвете накладываемым материалом
SX_LIB_API void SML_MtlSetLighting(ID id, bool is_lighting);	
SX_LIB_API bool SML_MtlGetLighting(ID id);

//текстура с параметрами освещения, если текстуры нет то будет использована текстура по умолчанию
//{
//флаг использования текстуры с параметрами освещения
SX_LIB_API void SML_MtlSetIsTextureLighting(ID id, bool is_tex_lighting);
SX_LIB_API bool SML_MtlGetIsTextureLighting(ID id);

SX_LIB_API void SML_MtlSetTextureLighting(ID id, const char* path_tex);	//загрузка текстуры с парамтерами освещения path_tex
SX_LIB_API void SML_MtlGetTextureLighting(ID id, char* path_tex);		//в path_tex записывает имя текстуры с параметрами освещения
//}

//шероховатость (0-1)
SX_LIB_API void SML_MtlSetRoughness(ID id, float roughness);
SX_LIB_API float SML_MtlGetRoughness(ID id);

//f0 - отражательная способность поверхности (0-1)
SX_LIB_API void SML_MtlSetF0(ID id, float f0);
SX_LIB_API float SML_MtlGetF0(ID id);

//полупрозрачность, типы из MtlTypeTransparency
SX_LIB_API void SML_MtlSetTypeTransparency(ID id, MtlTypeTransparency type);
SX_LIB_API MtlTypeTransparency SML_MtlGetTypeTransparency(ID id);

//отражения окружения, типы из MtlTypeReflect
SX_LIB_API void SML_MtlSetTypeReflection(ID id, MtlTypeReflect type);
SX_LIB_API MtlTypeReflect SML_MtlGetTypeReflection(ID id);

//детальность и микрорельеф
//{

//поверхность может содержать до 4 детальных текстур и до 4 текстур с микрорельефом
//данные для определения наложения текстур (детальных и микрорельефных) указываются в маске
//на каждый канал текстуры маски по одной детальной и микрорельефной текстуре
//то есть канал маски определет связанные между собой и детальную и микрорельефную текстуры

//маска наложения
SX_LIB_API void SML_MtlSetMaskTex(ID id, const char* path_tex);
SX_LIB_API void SML_MtlGetMaskTex(ID id, char* path_tex);

//текстуры микрорельефа, на каждый канал
SX_LIB_API void SML_MtlSetMRTex(ID id, int channel, const char* path_tex);
SX_LIB_API void SML_MtlGetMRTex(ID id, int channel, char* path_tex);

//детальные текстуры, на каждый канал
SX_LIB_API void SML_MtlSetDTex(ID id, int channel, const char* path_tex);
SX_LIB_API void SML_MtlGetDTex(ID id, int channel, char* path_tex);
//}

//стандартные параметры для отправки в шейдер (в вершинный и пиксельный раздельно), данные из MtlTransShaderStd
//{
//для вершинного шейдера
SX_LIB_API void SML_MtlSetSTDVS(ID id, MtlTransShaderStd type, bool is_send);
SX_LIB_API bool SML_MtlGetSTDVS(ID id, MtlTransShaderStd type);

//для пиксельного шейдера
SX_LIB_API void SML_MtlSetSTDPS(ID id, MtlTransShaderStd type, bool is_send);
SX_LIB_API bool SML_MtlGetSTDPS(ID id, MtlTransShaderStd type);
//}

//дополнительные пользовательские данные в виде float4 (0-1 для стандарта, иное вне стандарта, но допустимо)
//{
//данные для вершинного и пиксельного шейдера раздельны
//однако данные и из пиксельного могут быть отправленны в вершинный и наоборот
//в итоге в шейдере может быть 2 float4 пользовательских вектора с данными
//но если вектор определен для пиксельного то он все-равно будет отправлен в пиксельный шейдер, даже если и придназначалася для вершинного

//float4 для вершинного шейдера
SX_LIB_API void SML_MtlSetUDVS(ID id, int component, float val);
SX_LIB_API float SML_MtlGetUDVS(ID id, int component);

//отправка float4 вектора, предназначенного для вершинного шейдера, в пиксельный шейдер
SX_LIB_API void SML_MtlSetUDVS_InPS(ID id, bool is_send_ps);
SX_LIB_API bool SML_MtlGetUDVS_InPS(ID id);

//float4 для пиксельного шейдера
SX_LIB_API void SML_MtlSetUDPS(ID id, int component, float val);
SX_LIB_API float SML_MtlGetUDPS(ID id, int component);

//отправка float4 вектора, предназначенного для пиксельного шейдера, в вершинный шейдер
SX_LIB_API void SML_MtlSetUDPS_InVS(ID id, bool is_send_vs);
SX_LIB_API bool SML_MtlGetUDPS_InVS(ID id);
//}
//}}

#endif