
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
#pragma comment(lib, "sxlight_d.lib")
#else
#pragma comment(lib, "sxlight.lib")
#endif

#include <light/sxlight.h>

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
SX_LIB_API void SPE_Dbg_Set(report_func fnFunc);

//! инициализция подсистемы
SX_LIB_API void SPE_0Create(
	const char *szName,			//!< присваиваемое имя
	bool isUnic = true			//!< должна ли подсистема быть уникальной по имени
	);

//! установка илентификатора render target глубины сцены (для маягких/soft частиц)
SX_LIB_API void SPE_RTSetDepth(ID id);

//! уничтожить подсистему
SX_LIB_API void SPE_AKill();	

//!@}

//**************************************************************************

//! тип функции для определения пересечения, на вход идет предыдущая и будущая позиции цачицы, возвращает true в случае пересечения, иначе false
typedef bool(*g_particles_phy_collision) (const float3 *pLastPos, const float3 *pNextPos, float3 *pResultPos, float3 *pResultNormal);

/*! переназначение g_aiquad_phy_navigate, обязательное действие для работы с сеткой */
SX_LIB_API void SPE_SetFunc_ParticlesPhyCollision(g_particles_phy_collision fnFunc);

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
	//! обычный квадрат
	PARTICLESTYPE_FIGURE_QUAD,			

	//! билборд (квадрат, лицевая сторона которого всегда направлена на наблюдателя)
	PARTICLESTYPE_FIGURE_BILLBOARD,		

	//! составной квадрат (фигура из нескольких квадратов)
	PARTICLESTYPE_FIGURE_QUAD_COMPOSITE,
};

//! тип смешивания
enum PARTICLESTYPE_ALPHABLEND
{
	//! на основе альфа канала
	PARTICLESTYPE_ALPHABLEND_ALPHA,	

	//! аддитивное смешивание
	PARTICLESTYPE_ALPHABLEND_ADD	
};

//! тип ограничивающего объема
enum PARTICLESTYPE_BOUND
{
	//! нет объема
	PARTICLESTYPE_BOUND_NONE,	

	//! сфера
	PARTICLESTYPE_BOUND_SPHERE,	

	//! параллелепипед
	PARTICLESTYPE_BOUND_BOX,	

	//! конус
	PARTICLESTYPE_BOUND_CONE	
};

//! тип спавна частиц по позиции
enum PARTICLESTYPE_SPAWNPOS
{
	//! строгий в указанном месте
	PARTICLESTYPE_SPAWNPOS_STRICTLY,	

	//! по ограничивающему объему
	PARTICLESTYPE_SPAWNPOS_BOUND,		
};

//! тип поворотов частиц
enum PARTICLESTYPE_ROT
{
	//! нет поворота
	PARTICLESTYPE_ROT_NONE,	

	//! обычный поворот
	PARTICLESTYPE_ROT_NORMAL,	

	//! рандомный поворот
	PARTICLESTYPE_ROT_RAND,	

	//! равномерный поворот
	PARTICLESTYPE_ROT_EVENLY,	
};

//! тип зависимости "чего либо" в частице от времени ее жизни
enum PARTICLESTYPE_DEPEND
{
	//! нет зависиомсти
	PARTICLESTYPE_DEPEND_NONE,		

	//! прямая зависимость, чем старше тем меньше
	PARTICLESTYPE_DEPEND_DIRECT,	

	//! обратная зависимость, чем старше тем больше
	PARTICLESTYPE_DEPEND_INVERSE,	
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
	//! случайное
	PARTICLESTYPE_DEVIATION_RAND,	

	//! вдоль оси
	PARTICLESTYPE_DEVIATION_ALONG,	

	//! волновое
	PARTICLESTYPE_DEVIATION_WAVE,	
};

//#############################################################################

/*! структура описывающая партиклы (характеристики и поведение) частиц
 \note Данные структуры могут быть:
 - оффлайн изменяемые, требуют переустановки структуры данных для обновления внутренних данных системы частиц
 - онлайн изменяемые, изменения вступаю в силу сразу же после изменения значения переменной.\n
 Однако, также существуют данные, действия которых вступят в силу только при создании новых частиц (креационные).
 \note Отрицательная дисперсия (neg) лежит в пределах (-coef_disp*0.5,coef_disp*0.5), Положительная дисперсия (установлена по умолчанию тогда когда разрешена дисперсия) лежит в пределах (0,coef_disp)
*/
struct CParticlesData
{
	CParticlesData()
	{
		ZeroMemory(this, sizeof(CParticlesData));
		m_fTransparencyCoef = 1.0;
		m_fColorCoef = 1.f;
	}

	SX_ALIGNED_OP_MEM

	//**************************************************************************

	/*! \name Ограничивающий объем
	 \note Box: Vector1 x y z – минимум, Vector2 x y z – максимум \n
	Sphere: Vector1 x y z – центр сферы, w – радиус \n
	Cone: Vector1 x y z – нижняя точка конуса, w – радиус нижней точки, Vector2 y координата Y верхней точки конуса, w – радиус верхней точки \n
	@{*/

	//! тип ограничивающего объема, для уничтожения партикла при выходе за пределы
	PARTICLESTYPE_BOUND m_typeBound;	

	float4 m_vBoundVec1;
	float4 m_vBoundVec2;

	//!@}

	//**************************************************************************

	/*! \name Параметры создания частиц
	 \note Возможна привязка рандомного создания частиц к осям, для ограничивающих объемов (SpawnBoundBindCreate)
	 \note Bound volume - ограничивающий объем из #PARTICLESTYPE_BOUND
	@{*/

	//! тип спавна партиклов
	PARTICLESTYPE_SPAWNPOS m_typeSpawnPos;	

	//! точка из которой идут частицы (обязательно должна быть точка откуда идет спавн частиц если нет ограничивающего объема)
	float3 m_vSpawnOrigin;					

	//! дисперсия для #m_vSpawnOrigin
	float m_fSpawnOriginDisp;				


	//! привязка к BV x-
	bool m_shouldSpawnBoundBindCreateXNeg;		

	//! привязка к BV x+
	bool m_shouldSpawnBoundBindCreateXPos;		

	//! привязка к BV y-
	bool m_shouldSpawnBoundBindCreateYNeg;		

	//! привязка к BV y+
	bool m_shouldSpawnBoundBindCreateYPos;		

	//! привязка к BV z-
	bool m_shouldSpawnBoundBindCreateZNeg;		

	//! привязка к BV z+
	bool m_shouldSpawnBoundBindCreateZPos;		


	//! промежуток времени через который будет спавниться следующая частица, если 0 - спавним все сразу
	UINT m_uiSpawnNextTime;					

	//! дисперсия для #SpawnNextTime
	UINT m_uiSpawnNextTimeDisp;				

	//!@}

	//**************************************************************************

	/*! \name Анимированная текстура
	@{*/

	//! количество кадров по оси X
	int m_iAnimTexCountCadrsX;	

	//! количество кадров по оси Y
	int m_iAnimTexCountCadrsY;	


	//! частота обновления в млсек
	int m_iAnimTexRate;			

	//! разброс для #m_iAnimTexRate
	int m_iAnimTexRateDisp;		

	//! стартовый кадр
	int m_iAnimTexStartCadr;		

	//! разброс для #m_iAnimTexStartCadr
	int m_iAnimTexStartCadrDisp;	

	//!@}

	//**************************************************************************

	/*! \name Время жизни частицы, временные зависимости, размеры
	@{*/

	//! время жизни частиц, если time_life == 0 тогда частицы живут все время, иначе это время в млсекундах
	UINT m_uiTimeLife;						

	//! дисперсия для #TimeLife
	UINT m_uiTimeLifeDisp;					

	//! зависит ли альфа компонента частицы от ее возраста
	PARTICLESTYPE_DEPEND m_typeAlphaDependAge;	
	

	//! размер частиц
	float2_t m_vSize;						

	//! разброс для размера части
	float m_fSizeDisp;						


	//! зависит ли размер частицы от ее возраста
	PARTICLESTYPE_DEPEND m_typeSizeDependAge;	

	//!@}

	//**************************************************************************

	/*! \name Скорость
	@{*/

	//! скорость (по осям)
	float3 m_vVelocity;		

	//! дисперсия для #Velocity
	float m_fVelocityDisp;		


	//! отрицательная дисперсия по оси X
	bool m_shouldVelocityDispXNeg;	

	//! отрицательная дисперсия по оси Y
	bool m_shouldVelocityDispYNeg;	

	//! отрицательная дисперсия по оси Z
	bool m_shouldVelocityDispZNeg;	

	//!@}

	//**************************************************************************

	/*! \name Ускорение
	@{*/

	//! ускорение (по осям)
	float3 m_vAcceleration;		

	//! дисперсия для #m_vAcceleration
	float m_fAccelerationDisp;		


	//! отрицательная дисперсия по оси X
	bool m_shouldAccelerationDispXNeg;	

	//! отрицательная дисперсия по оси Y
	bool m_shouldAccelerationDispYNeg;	

	//! отрицательная дисперсия по оси Z
	bool m_shouldAccelerationDispZNeg;	

	//!@}

	//**************************************************************************

	/*! \name Характер: круговое движение
	@{*/

	//! испольовать ли использовать круговое движение
	bool m_useCharacterCircle;				

	//! ось для кругового движения
	PARTICLES_AXIS m_typeCharacterCircleAxis;	

	//! угол (в радианах)
	float m_fCharacterCircleAngle;			

	//! дисперсия для #CharacterCircleAngle
	float m_fCharacterCircleAngleDisp;		

	//! разрешено ли использовать отрицательную дисперсию
	bool m_useCharacterCircleAngleDispNeg;	

	//!@}

	//**************************************************************************

	/*! \name Характер вращение
	@{*/

	//! испольовать ли использовать вращение
	bool m_useCharacterRotate;				

	//! угол (в радианах)
	float m_fCharacterRotateAngle;			

	//! дисперсия для #CharacterRotateAngle
	float m_fCharacterRotateAngleDisp;		

	//! разрешено ли использовать отрицательную дисперсию
	bool m_useCharacterRotateAngleDispNeg;	

	//!@}

	//**************************************************************************

	/*! \name Характер: отклонения при движении
	@{*/

	//! использовать ли отклонения при движении
	bool m_useCharacterDeviation;						

	//! тип смещения
	PARTICLESTYPE_DEVIATION m_typeCharacterDeviation;	

	//! амплитуда, может быть отрицательна
	float m_fCharacterDeviationAmplitude;				

	//! коэфициент на который будет умножен угол, только для #PARTICLESTYPE_DEVIATION_WAVE
	float m_fCharacterDeviationCoefAngle;				

	//! ось на основании которой будет вычислен угол поворота, только для PARTICLESTYPE_DEVIATION_WAVE
	PARTICLES_AXIS m_typeCharacterDeviationAxis;			

	//! время обновления для #PARTICLESTYPE_DEVIATION_RAND и #PARTICLESTYPE_DEVIATION_ALONG
	UINT m_uiCharacterDeviationCountDelayMls;			

	//! коэфициент дисперсии, для #m_fCharacterDeviationCoefAngle если #PARTICLESTYPE_DEVIATION_WAVE, для #m_fCharacterDeviationAmplitude в других случаях
	float m_fCharacterDeviationCoefAngleDisp;		

	//! возможна ли отрицательная дисперсия
	bool m_useCharacterDeviationCoefAngleDispNeg;	

	//! задействовать ли координату X при просчете смещения
	bool m_useCharacterDeviationTapX;	

	//! задействовать ли координату Y при просчете смещения
	bool m_useCharacterDeviationTapY;	

	//! задействовать ли координату Z при просчете смещения
	bool m_useCharacterDeviationTapZ;	

	//!@}

	//**************************************************************************

	//! тип фигуры партикла
	PARTICLESTYPE_FIGURE m_typeFigure;	

	//! количество квадаратов в случае ParticlesFigureType::pft_quad_composite
	int m_iFigureCountQuads;			

	//! true - случайно генерировать углы поворотов, false - равномерно поворачивать
	bool m_useFigureRotRand;				

	//! поворачивать ли по оси X
	bool m_useFigureTapX;				

	//! поворачивать ли по оси Y
	bool m_useFigureTapY;				

	//! поворачивать ли по оси Z
	bool m_useFigureTapZ;				

	//**************************************************************************

	//! отрисовка следа от столкновения, только для эмиттеров у которых CollisionDelete == true, рисуется квад и альфа канал уменьшатсья с течением времени 
	bool m_useTrack;

	//! размер следа, в метрах
	float m_fTrackSize;	

	//! время исчезновения в млсек
	UINT m_uiTrackTime;	

	//**************************************************************************

	//! тип смешивания
	PARTICLESTYPE_ALPHABLEND m_typeAlphaBlend;	

	//! коэфициент на который будет домножен цвет
	float m_fColorCoef;	

	float4_t m_vColor;

	//! количество создваваемых/пересоздаваемых, 0< - пересоздание в случае нехватки, 0> - единственное создание при запуске
	int m_iReCreateCount;		

	//! мягкие ли частицы
	bool m_isSoft;				

	//! коэфициент для смягчения партиклов [0-1]
	float m_fSoftCoef;			

	//! частицы преломляют пространство
	bool m_useRefraction;		

	//! коэфициент для преломления партиклов [0-1]
	float m_fRefractionCoef;	

	//! коэфициент прозрачности [0-1], компонента альфа канала будет домножена на это значение
	float m_fTransparencyCoef;	

	//! освещаемые ли частицы
	bool m_isLighting;			

	//! удалять при столкновении с миром
	bool m_useCollisionDelete;	
};

//#############################################################################

/*! \defgroup sxparticles_eff Эффекты
 \ingroup sxparticles
 \warning Обращение (идентификация) конкретного эффекта осуществляется посредством идентификатора который выдает создающая функция #SPE_EffectAdd.
Получить идентификатор уже созданного эффекта можно, посредством ключа (key) который лежит в пределах [0, #SPE_EffectCountGet),
передав этот ключ #SPE_EffectIdOfKey, которая в случае успеха вернет идентификатор эффекта.
@{*/

//! загрузка информации об эффектах и эмиттерах из файла
SX_LIB_API void SPE_EffectLoad(const char *szPath);	

//! сохранение информации об эффектах и эмиттерах в файл
SX_LIB_API void SPE_EffectSave(const char *szPath);

//! очистка всего списка эффектов и эмиттеров
SX_LIB_API void SPE_EffectsClear();	


//! получить копию эффекта по имени
SX_LIB_API ID SPE_EffectInstanceByName(const char *szName);	

//! получить копию эффекта по его идентификатору
SX_LIB_API ID SPE_EffectInstanceByID(ID id);				

//!< получить эффект по имени (первый найденый)
SX_LIB_API ID SPE_EffectGetByName(const char *szName);		


//! добавить эффект и установить ему имя
SX_LIB_API ID SPE_EffectAdd(const char *szName);	

//! возвращает количество эффектов
SX_LIB_API int SPE_EffectGetCount();

//! возвращает идентификатор эффекта по порядковому номеру key
SX_LIB_API ID SPE_EffectGetIdOfKey(int iKey);		


//! удаляет эффект
SX_LIB_API void SPE_EffectDelete(ID id);		

//! установка имени 
SX_LIB_API void SPE_EffectSetName(ID id, const char *szName);

//! в name записывает имя эффекта
SX_LIB_API void SPE_EffectGetName(ID id, char *szName);


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
SX_LIB_API void SPE_EffectSetAlife(ID id, bool isAlife);


//! включен ли эффект
SX_LIB_API bool SPE_EffectGetEnable(ID id);

//! устанавливает состяние включен/выключен для эффекта
SX_LIB_API void SPE_EffectSetEnable(ID id, bool isEnable);


//! воспроизвести эффект используя при этом копию эффекта с идентификатором id
SX_LIB_API void SPE_EffectPlayByID(
	ID id,				//!< идентификатор оригинального эффекта
	const float3 *pPos,	//!< позиция эффекта воспроизведения
	const float3 *pDir	//!< направление взгляда эффекта
	);

//! воспроизвести эффект используя при этом копию эффекта с именем name
SX_LIB_API void SPE_EffectPlayByName(
	const char *szName,	//!< идентификатор оригинального эффекта
	const float3 *pPos,	//!< позиция эффекта воспроизведения
	const float3 *pDir	//!< направление взгляда эффекта
	);


//! установка позиции эффекту
SX_LIB_API void SPE_EffectSetPos(ID id, const float3 *pPos);

//! установка направления взгляда эффекта
SX_LIB_API void SPE_EffectSetDir(ID id, const float3 *pDir);

//! установка поворотов эффекта (в радианах)
SX_LIB_API void SPE_EffectSetRot(ID id, const float3 *pRot);

//! установка поворотов эффекта
SX_LIB_API void SPE_EffectSetRotQ(ID id, const SMQuaternion &oRot);


//! в pos записывает позицию эффекта
SX_LIB_API void SPE_EffectGetPos(ID id, float3 *pPos);

//! в dir записывает  направление взгляда эффекта
SX_LIB_API void SPE_EffectGetDir(ID id, float3 *pDir);

//! в rot записывает повороты эффекта (в радианах)
SX_LIB_API void SPE_EffectGetRot(ID id, float3 *pRot);


//! просчет видимости эффекта для фрустума frustum, и расчет расстояния от позиции наблюдателя view до эффекта, возвращает виден ли фруустуму эффект
SX_LIB_API bool SPE_EffectVisibleCom(ID id, const IFrustum *pFrustum, const float3 *pView);	

//! просчет видимости всех эффектов для фрустума frustum, и расчет расстояний от позиции наблюдателя view до всех эффектов
SX_LIB_API void SPE_EffectVisibleComAll(const IFrustum *pFrustum, const float3 *pView);


//! виден ли эффект? по результатам просчетов #SPE_EffectVisibleComAll
SX_LIB_API bool SPE_EffectGetVisible(ID id);

//! возвращает расстояние от наблюдателя до эффекта, по результатам просчетов #SPE_EffectVisibleComAll
SX_LIB_API float SPE_EffectGetDistToView(ID id);

//!@} sxparticles_eff

//#############################################################################

/*! \defgroup sxparticles_part Частицы
 \ingroup sxparticles
 \warning Идентификаторы партиклов не являются константными, и являются по сути порядковым номером.
 @{*/

/*! \name Get/Set членов структуры CParticlesData для системы частиц в эффекте
 Так как данные структуры #CParticlesData могут быть как оффлайн так и онлайн изменяемы, а их полное изменение может быть немного громоздким, то для этого сделаны следующие макрофункции.
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
SX_LIB_API ID SPE_EmitterAdd(ID id, CParticlesData *pData);

//! возвращает количество систем партиклов у эффекта
SX_LIB_API int SPE_EmitterGetSCount(ID id);

//! удаляет из эффекта id систему частиц idPart
SX_LIB_API void SPE_EmitterDelete(ID id, ID idPart);			

//! возвращает структуру данных системы частиц id_part из эффекта id
SX_LIB_API CParticlesData* SPE_EmitterGetData(ID id, ID idPart);


//! переустановка структуры данных data для системы частиц id_part у эффекта id, в data можно отправить 0 в случае если необходимо обновить состояния оффлайн данных
SX_LIB_API void SPE_EmitterReInit(ID id, ID idPart, CParticlesData *pData);

//! установка общего количества частиц в эмиттере эффекта
SX_LIB_API void SPE_EmitterSetCount(ID id, ID idPart, int iCount);

//! возвращает общее количество частиц в эмиттере эффекта
SX_LIB_API int SPE_EmitterGetCount(ID id, ID idPart);

//! возвращает количество живых частиц в эмиттере эффекта
SX_LIB_API int SPE_EmitterGetCountLife(ID id, ID idPart);

//! установка состояния воспроизведения (проиграть/остановить) эмиттеру эффекта
SX_LIB_API void SPE_EmitterSetEnable(ID id, ID idPart, bool isEnable);

//! возвращает состояние воспроизведения (проиграть/остановить) эмиттера эффекта
SX_LIB_API bool SPE_EmitterGetEnable(ID id, ID idPart);

//! установка текстуры (идентификатора на нее) системе частиц idPart эффекта id
SX_LIB_API void SPE_EmitterSetTexture(ID id, ID idPart, const char *szTex);
SX_LIB_API void SPE_EmitterSetTextureID(ID id, ID idPart, ID idTex);
SX_LIB_API ID SPE_EmitterGetTextureID(ID id, ID idPart);
SX_LIB_API void SPE_EmitterGetTexture(ID id, ID idPart, char *szTex);


SX_LIB_API void SPE_EmitterSetTextureTrack(ID id, ID idPart, const char *szTex);
SX_LIB_API void SPE_EmitterSetTextureTrackID(ID id, ID idPart, ID idTex);
SX_LIB_API ID SPE_EmitterGetTextureTrackID(ID id, ID idPart);
SX_LIB_API void SPE_EmitterGetTextureTrack(ID id, ID idPart, char *szTex);

//! установка имени системе частиц
SX_LIB_API void SPE_EmitterSetName(ID id, ID idPart, const char *szName);

//! в name записывает имя системы частиц
SX_LIB_API void SPE_EmitterGetName(ID id, ID idPart, char *szName);

SX_LIB_API int SPE_EmitterGetTrackCount(ID id, ID idPart);
SX_LIB_API int SPE_EmitterGetTrackPos(ID id, ID idPart, float3 **ppArr, int iCount);

//!@} sxparticles_part

#endif

//!@} sxparticles