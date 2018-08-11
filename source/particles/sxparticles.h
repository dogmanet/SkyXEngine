
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxparticles - библиотека системы частиц (эффектов)
*/

/*! \defgroup sxparticles sxparticles - система частиц

 \note Система частиц состоит из эффектов. Эффекты состоят из эмиттеров (испускатели).
 \note Идентфиикация эффекта происходит на основании его числового идентификатора выдаваемого создающими функциями (#SPE_EffectAdd) и является константным, идентификатор не является порядковым номером, для идентификации эффекта по порядковому номеру можно воспользоваться функцией #SPE_EffectIdOfKey
 \note Идентификация эмиттера в эффекте происходит на основании его числового идентификатора выдаваемого создающеми функциями (#SPE_EmitterAdd) и не является константным (удаление самого нижнего приведет к смещению всех верхних), идентификатор является порядковым номером
 \note Часицы эмиттера могут быть освещены (только ambient)
 \note Часицы эмиттера могут искажать (refraction)
 \note Часицы эмиттера могут быть мягкими (soft), для этого надо после инициализации библиотеки необходимо передать идентификатор render target глубины сцены #SPE_RTDepthSet
@{*/

#ifndef __SXPARTICLES_H
#define __SXPARTICLES_H

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxmtllight_d.lib")
#else
#pragma comment(lib, "sxmtllight.lib")
#endif

#include <mtllight/sxmtllight.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//##########################################################################

/*! \name Базовые функции библиотеки
@{*/

//! версия подсистемы
SX_LIB_API long SPE_0GetVersion();			

//! установить функцию обработки сообщений
SX_LIB_API void SPE_Dbg_Set(report_func rf);

//! инициализция подсистемы
SX_LIB_API void SPE_0Create(
	const char* name,			//!< присваиваемое имя
	bool is_unic = true			//!< должна ли подсистема быть уникальной по имени
	);

//! установка илентификатора render target глубины сцены (для маягких/soft частиц)
SX_LIB_API void SPE_RTDepthSet(ID id);	

//! уничтожить подсистему
SX_LIB_API void SPE_AKill();	

//!@}

//**************************************************************************

//! тип функции для определения пересечения, на вход идет предыдущая и будущая позиции цачицы, возвращает true в случае пересечения, иначе false
typedef bool(*g_particles_phy_collision) (const float3 * lastpos, const float3* nextpos, float3* coll_pos, float3* coll_nrm);

/*! переназначение g_aiquad_phy_navigate, обязательное действие для работы с сеткой */
SX_LIB_API void SPE_SetFunc_ParticlesPhyCollision(g_particles_phy_collision func);

//**************************************************************************

/*! \name Обработка потери/восстановления устройства
@{*/

//! вызывать при потрете устройства
SX_LIB_API void SPE_OnLostDevice();	

//! вызывать при восстановлении устройства
SX_LIB_API void SPE_OnResetDevice();

//!@}

//#############################################################################

/*! базовое направления взгляда эффекта */
#define SXPARTICLES_BASE_DIR float3(0,0,1)

/*! базовое направления взгляда следа */
#define SXPARTICLES_TRACK_BASE_DIR float3(0,1,0)

/*! время в течении которого эффект полность затухнет в случае установки ложного состояния жизни #SPE_EffectAlifeSet */
#define SXPARTICLES_DEADTH_TIME 1000

/*! количество новых резервных копий эффекта (в пуле) в случае нехватки */
#define SXPARTICLES_POOL_RESERVE 8

/*! на сколько отдаляем частицу от геометрии при отрисовке следа */
#define SXPARTICLES_TRACK_ADD	0.05f

//#############################################################################

//! тип фигуры частицы
enum PARTICLESTYPE_FIGURE
{
	PARTICLESTYPE_FIGURE_QUAD,			//!< обычный квадрат
	PARTICLESTYPE_FIGURE_BILLBOARD,		//!< билборд (квадрат, лицевая сторона которого всегда направлена на наблюдателя)
	PARTICLESTYPE_FIGURE_QUAD_COMPOSITE,//!< составной квадрат (фигура из нескольких квадратов)
};

//! тип смешивания
enum PARTICLESTYPE_ALPHABLEND
{
	PARTICLESTYPE_ALPHABLEND_ALPHA,	//!< на основе альфа канала
	PARTICLESTYPE_ALPHABLEND_ADD	//!< аддитивное смешивание
};

//! тип ограничивающего объема
enum PARTICLESTYPE_BOUND
{
	PARTICLESTYPE_BOUND_NONE,	//!< нет объема
	PARTICLESTYPE_BOUND_SPHERE,	//!< сфера
	PARTICLESTYPE_BOUND_BOX,	//!< параллелепипед
	PARTICLESTYPE_BOUND_CONE	//!< конус
};

//! тип спавна частиц по позиции
enum PARTICLESTYPE_SPAWNPOS
{
	PARTICLESTYPE_SPAWNPOS_STRICTLY,	//!< строгий в указанном месте
	PARTICLESTYPE_SPAWNPOS_BOUND,		//!< по ограничивающему объему
};

//! тип поворотов частиц
enum PARTICLESTYPE_ROT
{
	PARTICLESTYPE_ROT_NONE,	//!< нет поворота
	PARTICLESTYPE_ROT_NORMAL,	//!< обычный поворот
	PARTICLESTYPE_ROT_RAND,	//!< рандомный поворот
	PARTICLESTYPE_ROT_EVENLY,	//!< равномерный поворот
};

//! тип зависимости "чего либо" в частице от времени ее жизни
enum PARTICLESTYPE_DEPEND
{
	PARTICLESTYPE_DEPEND_NONE,		//!< нет зависиомсти
	PARTICLESTYPE_DEPEND_DIRECT,	//!< прямая зависимость, чем старше тем меньше
	PARTICLESTYPE_DEPEND_INVERSE,	//!< обратная зависимость, чем старше тем больше
};

//! ось
enum PARTICLES_AXIS
{
	PARTICLES_AXIS_X,
	PARTICLES_AXIS_Y,
	PARTICLES_AXIS_Z,
};

//! тип отклонения при движении частицы
enum PARTICLESTYPE_DEVIATION
{
	PARTICLESTYPE_DEVIATION_RAND,	//!< случайное
	PARTICLESTYPE_DEVIATION_ALONG,	//!< вдоль оси
	PARTICLESTYPE_DEVIATION_WAVE,	//!< волновое
};

//#############################################################################

/*! структура описывающая партиклы (характеристики и поведение) частиц
 \note Данные структуры могут быть:
 - оффлайн изменяемые, требуют переустановки структуры данных для обновления внутренних данных системы частиц
 - онлайн изменяемые, изменения вступаю в силу сразу же после изменения значения переменной.\n
 Однако, также существуют данные, действия которых вступят в силу только при создании новых частиц (креационные).
 \note Отрицательная дисперсия (neg) лежит в пределах (-coef_disp*0.5,coef_disp*0.5), Положительная дисперсия (установлена по умолчанию тогда когда разрешена дисперсия) лежит в пределах (0,coef_disp)
*/
struct ParticlesData
{
	ParticlesData()
	{
		ZeroMemory(this, sizeof(ParticlesData));
		TransparencyCoef = 1.0;
		ColorCoef = 1.f;
	}

	SX_ALIGNED_OP_MEM

	//**************************************************************************

	/*! \name Ограничивающий объем
	 \note Box: Vector1 x y z – минимум, Vector2 x y z – максимум \n
	Sphere: Vector1 x y z – центр сферы, w – радиус \n
	Cone: Vector1 x y z – нижняя точка конуса, w – радиус нижней точки, Vector2 y координата Y верхней точки конуса, w – радиус верхней точки \n
	@{*/
	PARTICLESTYPE_BOUND BoundType;	//!< тип ограничивающего объема, для уничтожения партикла при выходе за пределы

	float4 BoundVec1;
	float4 BoundVec2;

	//!@}

	//**************************************************************************

	/*! \name Параметры создания частиц
	 \note Возможна привязка рандомного создания частиц к осям, для ограничивающих объемов (SpawnBoundBindCreate)
	 \note Bound volume - ограничивающий объем из #PARTICLESTYPE_BOUND
	@{*/
	PARTICLESTYPE_SPAWNPOS SpawnPosType;	//!< тип спавна партиклов

	float3 SpawnOrigin;					//!< точка из которой идут частицы (обязательно должна быть точка откуда идет спавн частиц если нет ограничивающего объема)
	float SpawnOriginDisp;				//!< дисперсия для #SpawnOrigin

	bool SpawnBoundBindCreateXNeg;		//!< привязка к BV x-
	bool SpawnBoundBindCreateXPos;		//!< привязка к BV x+
	bool SpawnBoundBindCreateYNeg;		//!< привязка к BV y-
	bool SpawnBoundBindCreateYPos;		//!< привязка к BV y+
	bool SpawnBoundBindCreateZNeg;		//!< привязка к BV z-
	bool SpawnBoundBindCreateZPos;		//!< привязка к BV z+

	UINT SpawnNextTime;					//!< промежуток времени через который будет спавниться следующая частица, если 0 - спавним все сразу
	UINT SpawnNextTimeDisp;				//!< дисперсия для #SpawnNextTime

	//!@}

	//**************************************************************************

	/*! \name Анимированная текстура
	@{*/

	WORD AnimTexCountCadrsX;	//!< количество кадров по оси X
	WORD AnimTexCountCadrsY;	//!< количество кадров по оси Y

	WORD AnimTexRate;			//!< частота обновления в млсек
	WORD AnimTexRateDisp;		//!< разброс для #AnimTexRate
	WORD AnimTexStartCadr;		//!< стартовый кадр
	WORD AnimTexStartCadrDisp;	//!< разброс для #AnimTexStartCadr

	//!@}

	//**************************************************************************

	/*! \name Время жизни частицы, временные зависимости, размеры
	@{*/

	DWORD TimeLife;						//!< время жизни частиц, если time_life == 0 тогда частицы живут все время, иначе это время в млсекундах
	DWORD TimeLifeDisp;					//!< дисперсия для #TimeLife

	PARTICLESTYPE_DEPEND AlphaDependAge;	//!< зависит ли альфа компонента частицы от ее возраста
	
	float2_t Size;						//!< размер частиц
	float SizeDisp;						//!< разброс для размера части

	PARTICLESTYPE_DEPEND SizeDependAge;	//!< зависит ли размер частицы от ее возраста

	//!@}

	//**************************************************************************

	/*! \name Скорость
	@{*/

	float3 Velocity;		//!< скорость (по осям)
	float VelocityDisp;		//!< дисперсия для #Velocity

	bool VelocityDispXNeg;	//!< отрицательная дисперсия по оси X
	bool VelocityDispYNeg;	//!< отрицательная дисперсия по оси Y
	bool VelocityDispZNeg;	//!< отрицательная дисперсия по оси Z

	//!@}

	//**************************************************************************

	/*! \name Ускорение
	@{*/

	//ускорение
	float3 Acceleration;		//!< ускорение (по осям)
	float AccelerationDisp;		//!< дисперсия для #Acceleration

	bool AccelerationDispXNeg;	//!< отрицательная дисперсия по оси X
	bool AccelerationDispYNeg;	//!< отрицательная дисперсия по оси Y
	bool AccelerationDispZNeg;	//!< отрицательная дисперсия по оси Z

	//!@}

	//**************************************************************************

	/*! \name Характер: круговое движение
	@{*/

	bool CharacterCircle;				//!< испольовать ли использовать круговое движение
	PARTICLES_AXIS CharacterCircleAxis;	//!< ось для кругового движения
	float CharacterCircleAngle;			//!< угол (в радианах)
	float CharacterCircleAngleDisp;		//!< дисперсия для #CharacterCircleAngle
	bool CharacterCircleAngleDispNeg;	//!< разрешено ли использовать отрицательную дисперсию

	//!@}

	//**************************************************************************

	/*! \name Характер вращение
	@{*/

	bool CharacterRotate;				//!< испольовать ли использовать вращение
	float CharacterRotateAngle;			//!< угол (в радианах)
	float CharacterRotateAngleDisp;		//!< дисперсия для #CharacterRotateAngle
	bool CharacterRotateAngleDispNeg;	//!< разрешено ли использовать отрицательную дисперсию

	//!@}

	//**************************************************************************

	/*! \name Характер: отклонения при движении
	@{*/

	bool CharacterDeviation;						//!< использовать ли отклонения при движении
	PARTICLESTYPE_DEVIATION CharacterDeviationType;	//!< тип смещения
	float CharacterDeviationAmplitude;				//!< амплитуда, может быть отрицательна
	float CharacterDeviationCoefAngle;				//!< коэфициент на который будет умножен угол, только для ParticlesDeviationType::ptd_wave
	PARTICLES_AXIS CharacterDeviationAxis;			//!< ось на основании которой будет вычислен угол поворота, только для ParticlesDeviationType::ptd_wave
	DWORD CharacterDeviationCountDelayMls;			//!< время обновления для #ParticlesDeviationType::ptd_rnd и #ParticlesDeviationType::pdt_along

	float CharacterDeviationCoefAngleDisp;		//!< коэфициент дисперсии, для #CharacterDeviationCoefAngle если #ParticlesDeviationType::ptd_wave, для #CharacterDeviationAmplitude в других случаях
	bool CharacterDeviationCoefAngleDispNeg;	//!< возможна ли отрицательная дисперсия

	bool CharacterDeviationTapX;	//!< задействовать ли координату X при просчете смещения
	bool CharacterDeviationTapY;	//!< задействовать ли координату Y при просчете смещения
	bool CharacterDeviationTapZ;	//!< задействовать ли координату Z при просчете смещения

	//!@}

	//**************************************************************************

	PARTICLESTYPE_FIGURE FigureType;	//!< тип фигуры партикла
	int FigureCountQuads;			//!< количество квадаратов в случае ParticlesFigureType::pft_quad_composite
	bool FigureRotRand;				//!< true - случайно генерировать углы поворотов, false - равномерно поворачивать
	bool FigureTapX;				//!< поворачивать ли по оси X
	bool FigureTapY;				//!< поворачивать ли по оси Y
	bool FigureTapZ;				//!< поворачивать ли по оси Z

	//**************************************************************************

	//! отрисовка следа от столкновения, только для эмиттеров у которых CollisionDelete == true, рисуется квад и альфа канал уменьшатсья с течением времени 
	bool Track;
	float TrackSize;	//!< размер следа, в метрах
	DWORD TrackTime;	//!< время исчезновения в млсек

	//**************************************************************************

	PARTICLESTYPE_ALPHABLEND AlphaBlendType;	//!< тип смешивания
	float ColorCoef;		//!< коэфициент на который будет домножен цвет
	float4_t Color;
	int ReCreateCount;		//!< количество создваваемых/пересоздаваемых, 0< - пересоздание в случае нехватки, 0> - единственное создание при запуске

	bool Soft;				//!< мягкие ли частицы
	float SoftCoef;			//!< коэфициент для смягчения партиклов [0-1]

	bool Refraction;		//!< частицы преломляют пространство
	float RefractionCoef;	//!< коэфициент для преломления партиклов [0-1]

	float TransparencyCoef;	//!< коэфициент прозрачности [0-1], компонента альфа канала будет домножена на это значение

	bool Lighting;			//!< освещаемые ли частицы

	bool CollisionDelete;	//!< удалять при столкновении с миром
};

//#############################################################################

/*! \defgroup sxparticles_eff Эффекты
 \ingroup sxparticles
 \warning Обращение (идентификация) конкретного эффекта осуществляется посредством идентификатора который выдает создающая функция #SPE_EffectAdd.
Получить идентификатор уже созданного эффекта можно, посредством ключа (key) который лежит в пределах [0, #SPE_EffectCountGet),
передав этот ключ #SPE_EffectIdOfKey, которая в случае успеха вернет идентификатор эффекта.
@{*/

//! загрузка информации об эффектах и эмиттерах из файла
SX_LIB_API void SPE_EffectLoad(const char* path);	

//! сохранение информации об эффектах и эмиттерах в файл
SX_LIB_API void SPE_EffectSave(const char* path);	

//! очистка всего списка эффектов и эмиттеров
SX_LIB_API void SPE_EffectsClear();	


//! получить копию эффекта по имени
SX_LIB_API ID SPE_EffectInstanceByName(const char* name);	

//! получить копию эффекта по его идентификатору
SX_LIB_API ID SPE_EffectInstanceByID(ID id);				

//!< получить эффект по имени (первый найденый)
SX_LIB_API ID SPE_EffectGetByName(const char* name);		


//! добавить эффект и установить ему имя
SX_LIB_API ID SPE_EffectAdd(const char* name);	

//! возвращает количество эффектов
SX_LIB_API int SPE_EffectCountGet();			

//!< возвращает идентификатор эффекта по порядковому номеру key
SX_LIB_API ID SPE_EffectIdOfKey(ID key);		


//! удаляет эффект
SX_LIB_API void SPE_EffectDelete(ID id);		

//! установка имени 
SX_LIB_API void SPE_EffectNameSet(ID id, const char* name);	

//! в name записывает имя эффекта
SX_LIB_API void SPE_EffectNameGet(ID id, char* name);		


//! просчет и обновление всех партиклов в эффекте
SX_LIB_API void SPE_EffectCompute(ID id);					

//! просчет света для все частиц эффекта
SX_LIB_API void SPE_EffectComputeLighting(ID id);			

//! рендер эффекта, timeDelta - время рендера кадра
SX_LIB_API void SPE_EffectRender(ID id, DWORD timeDelta);	


//! просчет и обновление всех партиклов во всех эффектах
SX_LIB_API void SPE_EffectComputeAll();					

//! просчет света для все частиц во всех эффектах
SX_LIB_API void SPE_EffectComputeLightingAll();			

//!< отрисовка всех эффектов
SX_LIB_API void SPE_EffectRenderAll(DWORD timeDelta);	


//! жив ли эффект, или все частицы в нем уже умерли?
SX_LIB_API bool SPE_EffectAlifeGet(ID id);				

//<! установка состояния жизни (время затухания #SXPARTICLES_DEADTH_TIME)
SX_LIB_API void SPE_EffectAlifeSet(ID id, bool alife);	


//! включен ли эффект
SX_LIB_API bool SPE_EffectEnableGet(ID id);				

//! устанавливает состяние включен/выключен для эффекта
SX_LIB_API void SPE_EffectEnableSet(ID id, bool enable);


//! воспроизвести эффект используя при этом копию эффекта с идентификатором id
SX_LIB_API void SPE_EffectPlayByID(
	ID id,			//!< идентификатор оригинального эффекта
	float3* pos,	//!< позиция эффекта воспроизведения
	float3* dir		//!< направление взгляда эффекта
	);

//! воспроизвести эффект используя при этом копию эффекта с именем name
SX_LIB_API void SPE_EffectPlayByName(
	const char* name,	//!< идентификатор оригинального эффекта
	float3* pos,		//!< позиция эффекта воспроизведения
	float3* dir			//!< направление взгляда эффекта
	);


//! установка позиции эффекту
SX_LIB_API void SPE_EffectPosSet(ID id, float3* pos);	

//! установка направления взгляда эффекта
SX_LIB_API void SPE_EffectDirSet(ID id, float3* dir);	

//! установка поворотов эффекта (в радианах)
SX_LIB_API void SPE_EffectRotSet(ID id, float3* rot);	

//! установка поворотов эффекта
SX_LIB_API void SPE_EffectRotSetQ(ID id, const SMQuaternion & rot);	


//! в pos записывает позицию эффекта
SX_LIB_API void SPE_EffectPosGet(ID id, float3* pos);	

//! в dir записывает  направление взгляда эффекта
SX_LIB_API void SPE_EffectDirGet(ID id, float3* dir);	

//! в rot записывает повороты эффекта (в радианах)
SX_LIB_API void SPE_EffectRotGet(ID id, float3* rot);	


//! просчет видимости эффекта для фрустума frustum, и расчет расстояния от позиции наблюдателя view до эффекта, возвращает виден ли фруустуму эффект
SX_LIB_API bool SPE_EffectVisibleCom(ID id, const IFrustum* frustum, float3* view);	

//! просчет видимости всех эффектов для фрустума frustum, и расчет расстояний от позиции наблюдателя view до всех эффектов
SX_LIB_API void SPE_EffectVisibleComAll(const IFrustum* frustum, float3* view);


//! виден ли эффект? по результатам просчетов #SPE_EffectVisibleComAll
SX_LIB_API bool SPE_EffectVisibleGet(ID id);		

//! возвращает расстояние от наблюдателя до эффекта, по результатам просчетов #SPE_EffectVisibleComAll
SX_LIB_API float SPE_EffectDistToViewGet(ID id);	

//!@} sxparticles_eff

//#############################################################################

/*! \defgroup sxparticles_part Частицы
 \ingroup sxparticles
 \warning Идентификаторы партиклов не являются константными, и являются по сути порядковым номером.
 @{*/

/*! \name Get/Set членов структуры ParticlesData для системы частиц в эффекте
 Так как данные структуры #ParticlesData могут быть как оффлайн так и онлайн изменяемы, а их полное изменение может быть немного громоздким, то для этого сделаны следующие макрофункции.
 \note Аргументы:
 - id_eff - идентификатор эффекта
 - id_part - идентификатор системы частиц
 - var - имя переменной из структуры #ParticlesData
 - val - значение для var
 @{*/

//! обновление переменной с переустановкой структуры, для оффлайн данных
#define SPE_EmitterSetR(id_eff, id_part, var, val) {SPE_EmitterGetData(id_eff, id_part)->var = val; SPE_EmitterReInit(id_eff, id_part, 0);}

//! обновление переменной, для онлайн данных
#define SPE_EmitterSet(id_eff, id_part, var, val) {SPE_EmitterGetData(id_eff, id_part)->var = val;}

//! получение значения переменной
#define SPE_EmitterGet(id_eff, id_part, var) SPE_EmitterGetData(id_eff, id_part)->var

//!@}

//**************************************************************************

//! добавляет систему партиклов к эффекту
SX_LIB_API ID SPE_EmitterAdd(ID id, ParticlesData* data);		

//! возвращает количество систем партиклов у эффекта
SX_LIB_API int SPE_EmitterSCountGet(ID id);						

//! удаляет из эффекта id систему частиц id_part
SX_LIB_API void SPE_EmitterDelete(ID id, ID id_part);			

//! возвращает структуру данных системы частиц id_part из эффекта id
SX_LIB_API ParticlesData* SPE_EmitterGetData(ID id, ID id_part);


//! переустановка структуры данных data для системы частиц id_part у эффекта id, в data можно отправить 0 в случае если необходимо обновить состояния оффлайн данных
SX_LIB_API void SPE_EmitterReInit(ID id, ID id_part, ParticlesData* data);

//! установка общего количества частиц в эмиттере эффекта
SX_LIB_API void SPE_EmitterCountSet(ID id, ID id_part, int count);

//! возвращает общее количество частиц в эмиттере эффекта
SX_LIB_API int SPE_EmitterCountGet(ID id, ID id_part);

//! возвращает количество живых частиц в эмиттере эффекта
SX_LIB_API int SPE_EmitterCountLifeGet(ID id, ID id_part);

//! установка состояния воспроизведения (проиграть/остановить) эмиттеру эффекта
SX_LIB_API void SPE_EmitterEnableSet(ID id, ID id_part, bool enable);

//! возвращает состояние воспроизведения (проиграть/остановить) эмиттера эффекта
SX_LIB_API bool SPE_EmitterEnableGet(ID id, ID id_part);

//! установка текстуры (идентификатора на нее) системе частиц id_part эффекта id
SX_LIB_API void SPE_EmitterTextureSet(ID id, ID id_part, const char* tex);
SX_LIB_API void SPE_EmitterTextureSetID(ID id, ID id_part, ID tex);
SX_LIB_API ID SPE_EmitterTextureGetID(ID id, ID id_part);
SX_LIB_API void SPE_EmitterTextureGet(ID id, ID id_part, char* tex);


SX_LIB_API void SPE_EmitterTextureTrackSet(ID id, ID id_part, const char* tex);
SX_LIB_API void SPE_EmitterTextureTrackSetID(ID id, ID id_part, ID tex);
SX_LIB_API ID SPE_EmitterTextureTrackGetID(ID id, ID id_part);
SX_LIB_API void SPE_EmitterTextureTrackGet(ID id, ID id_part, char* tex);

//! установка имени системе частиц
SX_LIB_API void SPE_EmitterNameSet(ID id, ID id_part, const char* name);	

//! в name записывает имя системы частиц
SX_LIB_API void SPE_EmitterNameGet(ID id, ID id_part, char* name);			

SX_LIB_API int SPE_EmitterTrackCountGet(ID id, ID id_part);
SX_LIB_API int SPE_EmitterTrackPosGet(ID id, ID id_part, float3** arr, int count);

//!@} sxparticles_part

#endif

//!@} sxparticles