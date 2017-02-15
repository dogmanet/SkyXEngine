
#ifndef __sxmlight
#define __sxmlight

#include <gdefines.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <sxmath.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "DxErr9.lib")
#pragma comment(lib, "d3dx9.lib")

//типы источников света
#define LIGHTS_TYPE_NONE -1		//не установленный
#define LIGHTS_TYPE_GLOBAL 0	//глобальный
#define LIGHTS_TYPE_POINT 1		//точечный
#define LIGHTS_TYPE_DIRECTION 2	//направленный

#define LIGHTS_TYPE_SHADOWED_STATIC 0		//статический источник света, тень обновляется первые несколько кадров (для корректности), далее не обновляется
#define LIGHTS_TYPE_SHADOWED_DYNAMIC 1		//полностью динамические тени

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

//ОБРАБОТКА ПОТЕРИ И ВОССТАНОВЛЕНИЯ УСТРОЙТСВА
SX_LIB_API void SML_OnLostDevice();	//вызывать при потрете устройства
//вызывать при восстановлении устройства, с передачей параметров области в которую рисуем
SX_LIB_API void SML_OnResetDevice(
	int width,		//ширина окна
	int heigth,		//высота окна
	float projfov	//fov угол
	);

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
SX_LIB_API void SML_LigthsUpdate(float3* const_cam_pos,float4x4* camview);

SX_LIB_API void SML_LigthsSave(const char* path);	//сохранить список света
SX_LIB_API void SML_LigthsLoad(const char* path);	//загрузить список света

SX_LIB_API void SML_LigthsClear();		//очистить список света
SX_LIB_API long SML_LigthsGetCount();	//общее количество света
SX_LIB_API long SML_LigthsGetIDOfKey(long key);	//получить id по ключу (порядковому номеру), к примеру в случае полного обхода массива

//создать точечный свет (светит во все стороны)
SX_LIB_API long SML_LigthsCreatePoint(
	float3* center,		//центр света (мировая)
	float power,		//сила света
	float dist,			//дистанция на которую распространяется свет
	float3* color,		//цвет в пределах 0-1
	bool isglobal,		//глобальный ли свет (типа солнца)
	bool is_shadowed	//отбрасвыает ли тени свет
	);

//создать направленный свет
SX_LIB_API long SML_LigthsCreateDirection(
	float3* pos,		//позиция света (мировая)
	float power,		//сила света
	float dist,			//дистанция на которую распространяется свет
	float3* color,		//цвет в пределах 0-1
	float3* dir,		//направление взгляда (базис float3(0,-1,0))
	float top_radius,	//верхний (начальный) радиус
	float angle,		//угол
	bool is_shadow		//отбрасвыает ли тени свет
	);

SX_LIB_API int SML_LigthsGetType(long id);	//возвращает тип света

SX_LIB_API long SML_LigthsCreateCopy(long id);	//создать копию объекта и вернуть на нее id
SX_LIB_API void SML_LigthsDeleteLight(long id);	//удаление света

//имя света, name - выделенный массив
SX_LIB_API char* SML_LigthsGetName(long id);
SX_LIB_API void SML_LigthsSetName(long id, const char* name);

//пересоздание света, аргументы аналогичны создающим функциям
SX_LIB_API long SML_LigthsReCreatePoint(long id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed);
SX_LIB_API long SML_LigthsReCreateDirection(long id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow);

//РАБОТА С  МЕШЕМ ИСТОЧНИКА СВЕТА
//{
//привязка осуществляется статической загружаемой модели к позиции света
//привязывается центр определенной (выбранной, а по умолчанию нулевой) подгруппы
SX_LIB_API bool SML_LigthsExistsSource(long id);					//есть ли источник у света
SX_LIB_API void SML_LigthsBindToGroupSource(long id, int group);	//привязка (центрирование) подгруппы источнка к позиции света
SX_LIB_API long SML_LigthsGetBindedGroupSource(long id);			//номер подгруппы которая привязана к позиции света
SX_LIB_API long SML_LigthsGetCountGroupSource(long id);				//общее количество подгрупп в меше
SX_LIB_API const char* SML_LigthsGetPathNameSource(long id);		//относительный путь до файла с мешем
SX_LIB_API const char* SML_LigthsGetNameGroupSource(long id, int group);//имя подгруппы меша, group - номер подгруппы (нумерация с нуля)
SX_LIB_API void SML_LigthsLoadSource(long id, const char* path);	//загрузить меш по относительнмоу пути path, и привязать по дефолту 0 подгруппу
//}

//РЕНДЕР
//{
//отрисовка всех источников света со всеми подгруппами, 
//за ислючение подгруппы источника которая привязана к поции света
SX_LIB_API void SML_LigthsRenderAllExceptGroupSource(long id, DWORD timeDelta);

//отрисовка меша источника света, render_bind_group - рисовать ли ту группу которая привязана к свету
SX_LIB_API void SML_LigthsRenderSource(long id, bool render_bind_group, DWORD timeDelta);

//отрисовка ограничивающего объема
SX_LIB_API void SML_LigthsRender(long id, DWORD timeDelta);
//}

//цвет света
SX_LIB_API void SML_LigthsGetColor(long id, float3* color);
SX_LIB_API void SML_LigthsSetColor(long id, float3* vec);

//позиция света
//for_mesh - позиция меша источника
//greal получить реальные координаты, в случае если у нас глобальный источник света
SX_LIB_API void SML_LigthsGetPos(long id, float3* vec, bool for_mesh, bool greal = false);
SX_LIB_API void SML_LigthsSetPos(long id, float3* vec, bool greal = false);

//сила света
SX_LIB_API void SML_LigthsSetPower(long id, float power);
SX_LIB_API float SML_LigthsGetPower(long id);
SX_LIB_API float SML_LigthsGetPowerDiv(long id);	//возвращает "гиперболизированное значение силы" для отправки в шейдер

SX_LIB_API float SML_LigthsGetDist(long id);	//возвращает дистанцию на которую распространяется свет
//установка новой дистанции
//dist - новая дистанция (радиус для точечного и высота для направленного)
//is_create нужно ли пересоздание объема?
SX_LIB_API void SML_LigthsSetDist(long id, float dist, bool is_create = true);

//повороты света, rot_mesh - повороты меша источника света
SX_LIB_API void SML_LigthsGetRot(long id, float3* vec, bool rot_mesh);
SX_LIB_API void SML_LigthsSetRot(long id, float3* vec, bool rot_mesh);

//направление взгляда, rot_mesh - направление меша источника света
SX_LIB_API void SML_LigthsGetDir(long id, float3* vec, bool rot_mesh);
SX_LIB_API void SML_LigthsSetDir(long id, float3* vec, bool rot_mesh);

//УПРАВЛЕНИЕ ВИДИМОСТЬЮ
//{
//отдельные просчеты
SX_LIB_API bool SML_LigthsComVisibleForFrustum(long id, ISXFrustum* frustum);	//виден ли свет в данном фрустуме frustum
SX_LIB_API float SML_LigthsComDistFor(long id, float3* viewpos);				//дистанция от viewpos до начала света

//предварительные просчеты (к примеру в другом потоке)
//просчет видимости и расстояния от наблюдателя viewpos до начала света, для каждого света
SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* viewpos);
SX_LIB_API bool SML_LigthsGetVisibleForFrustum(long id);//виден ли свет в просчитанном фрустуме
SX_LIB_API float SML_LigthsGetDistFor(long id);			//просчитанная дистанция от позиции наблюдателя до начала света
//}

//включение/выключение света
SX_LIB_API int SML_LigthsIsEnable(long id);
SX_LIB_API void SML_LigthsSetEnable(long id, bool val);

//угол для направленного источника
SX_LIB_API float SML_LigthsGetAngle(long id);
SX_LIB_API void SML_LigthsSetAngle(long id, float angle);

//верхний радиус для направленного источника
SX_LIB_API float SML_LigthsGetTopRadius(long id);
SX_LIB_API void SML_LigthsSetTopRadius(long id, float top_radius);

//УПРАВЛЕНИЕ ТЕНЯМИ
//{{
//для генерации теней используется один render target full screen
//для сглаживания используется еще один аналогичный render target и первый render target
//при сглаживании внутри меняется только счетчик, который меняет их местами
//так как используется всего один render target для генерации теней 
//то есесно в текущий момент времени можно сгенерировать и сгладить тень только для одного света

SX_LIB_API bool SML_LigthsIsShadow(long id);	//отбрасывает ли тени
SX_LIB_API void SML_LigthsSetShadow(long id, bool is_shadow);	//установка наличия теней

SX_LIB_API bool SML_LigthsGet4Or3SplitsG(long id);	//
SX_LIB_API void SML_LigthsSet4Or3SplitsG(long id, bool bf4or3);	//

SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow();	//возвращает текстуру с тенями (если вызывались функции сглаживания) то вренутся сглаженная
SX_LIB_API void SML_LigthsNullingShadow();	//обнуление карты теней

//управление циклами рендера в карты глубин
//{
//для всех видов света, необходимо вызывать для каждого света
SX_LIB_API void SML_LigthsInRenderBegin(long id);	//подготовительные процедуры
SX_LIB_API void SML_LigthsInRenderEnd(long id);		//завершающие процедуры

//для глобального и локального света, необходимо вызывать для каждого света
SX_LIB_API void SML_LigthsInRenderPre(long id,int cube);	//подготовительные процедур перед рендером для сплита/стороны
SX_LIB_API void SML_LigthsInRenderPost(long id,int cube);	//завершающие процедур перед рендером для сплита/стороны
//}

SX_LIB_API void SML_LigthsShadowSetShaderOfTypeMat(long id, int typemat, float4x4* wmat);
SX_LIB_API void SML_LigthsGenShadow(long id);	//генерация тени на основе уже отработанной информации о затенении

//сглаживание текущей тени
//randomsam - использовать ли смещение по случайному направлению
//size - коэфициент увеличения размера пикселя дял размытия
//isfirst - впервые ли вызывается размытие для текущей тени, каждый разкогда тень еще не сглажена но сгенерированна нудо указывать false
SX_LIB_API void SML_LigthsSoftShadow(bool randomsam, float size, bool isfirst = false);

//возвращает указатель на фрустум 
//how - номер сплита для глобального, 0 для направленного, и номер стороны для точечного
SX_LIB_API ISXFrustum* SML_LigthsGetFrustum(long id, int how);

//обновление фрустума глобального света, split - номер сплита, pos - позиция наблюдателя, dir - направление взгляда наблюдателя
SX_LIB_API void SML_LigthsUpdateFrustumsG(long id, int split, float3* pos, float3* dir);

//управление смещением при генерации теней
SX_LIB_API float SML_LigthsGetBias(long id);
SX_LIB_API void SML_LigthsSetBias(long id, float bias);

//управление размытием при генерации теней
SX_LIB_API void SML_LigthsSetBlurPixel(long id, float blur_pixel);
SX_LIB_API float SML_LigthsGetBlurPixel(long id);

//управление ближней плоскостью отсечения для теней (локальный свет)
SX_LIB_API void SML_LigthsSetShadowLocalNear(long id, float slnear);
SX_LIB_API float SML_LigthsGetShadowLocalNear(long id);

//управление дальней плоскостью отсечения для теней (локальный свет)
//!!! иногда возникают артефакты и если отрегулировать это значение то все становится на свои места
SX_LIB_API void SML_LigthsSetShadowLocalFar(long id, float slfar);
SX_LIB_API float SML_LigthsGetShadowLocalFar(long id);

//управление сторонами для точечног освета (тени)
SX_LIB_API void SML_LigthsSetEnableCubeEdge(long id, int edge, bool enable);
SX_LIB_API bool SML_LigthsGetEnableCubeEdge(long id, int edge);

//управление массивами обработки данных дл рендера (к примеру от геометрии, растительности и прочего)
//сделано для того чтобы отделить просчеты видимости (если они нужны) от рендера
//к примеру можно рассчитать видимости в одном потоке а рисовать в основном
//inid - идентификатор объекта/системы вообще того что выдало идентификатор массива
//how - номер сплита для глобального, 0 для направленного, и номер стороны для точечного
//id_arr - номер массива обработки от объекта который надо будет обрабатывать на видимость
SX_LIB_API long SML_LigthsGetIDArr(long id, long inid, int how);
SX_LIB_API void SML_LigthsSetIDArr(long id, long inid, int how, long id_arr);

//управление динамиченостью обработки теней (LIGHTS_TYPE_SHADOWED_DYNAMIC, LIGHTS_TYPE_SHADOWED_STATIC)
SX_LIB_API void SML_LigthsSetTypeShadowed(long id, int type);
SX_LIB_API int SML_LigthsGetTypeShadowed(long id);

//управление обновлениями теней
//{
//производит обработку данных затем возвращает разрешено ли обновлять тени
//viewpos - позиция камеры для которой обновляем, на основании расстояния будет произведен расчет обновления
//ghow - номер сплита для глобального, 0 для направленного, и номер стороны для точечного
SX_LIB_API bool SML_LigthsUpdateCountUpdate(long id, float3* viewpos, int ghow = -1);

//разрешено ли обновлять тени, ghow аналогичен SML_LigthsUpdateCountUpdate
SX_LIB_API bool SML_LigthsAllowedRender(long id, int ghow = -1);
SX_LIB_API void SML_LigthsSetNullCountUpdate(long id);	//сбросить счетчик обновлений и обновлять до предела тени
//}
//}}

//обработка hdr, factor_adapted - на сколько быстро "глаз" привыкнет
SX_LIB_API void SML_LigthsComHDR(DWORD timeDelta, float factor_adapted = 10);


SX_LIB_API long SML_LigthsDelGetCount();
SX_LIB_API int SML_LigthsDelGetType(long key);
SX_LIB_API void SML_LigthsDelDel(long key);
SX_LIB_API long SML_LigthsDelGetIDArr(long key, long inid, int how);

/////////

//типы render targets
#define DS_RT_COLOR 0	//цвет	rgba8
#define DS_RT_NORMAL 1	//нормали (включая микрорельеф)	rfb10a2
#define DS_RT_PARAM 2	//параметры освещения	r5g6b5
#define DS_RT_DEPTH 3	//глубина	r16f
#define DS_RT_AMBIENT_DIFF 4	//эмбиент цвет (цвет света) и диффузное освещение rgba16f
#define DS_RT_SPECULAR 5		//блики	r16f
#define DS_RT_SCENE_LIGHT_COM 6	//освещенная сцена	rgb10a2
#define DS_RT_ADAPTED_LUM_CURR 7//текущий rt адаптации глаза к освещению r16f

SX_LIB_API DWORD SML_DSGetRT_ID(int type);	//id render target по его типу
SX_LIB_API IDirect3DTexture9*  SML_DSGetRT(int type);	//текстура render target по его типу

/////////

#define MTL_MAX_SIZE_DIR 64
#define MTL_MAX_SIZE_NAME 64

//типы материалов
#define MTL_GEOM 0
#define MTL_GRASS 1
#define MTL_TREE 2
#define MTL_ANIM 3

SX_LIB_API long SML_MtlLoad(const char* name, int mtl_type = MTL_GEOM);
SX_LIB_API long SML_MtlGetCount();
SX_LIB_API int SML_MtlGetType(long id);
SX_LIB_API void SML_MtlRender(long id, float4x4* world);
SX_LIB_API void SML_MtlSetMainTexture(DWORD slot, long id);

#endif