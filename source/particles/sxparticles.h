
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxparticles - библиотека системы частиц (эффектов)
*/

/*! \defgroup sxparticles sxparticles - система частиц
@{*/

#ifndef __sxparticles
#define __sxparticles

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxmtllight_d.lib")
#else
#pragma comment(lib, "sxmtllight.lib")
#endif
#include <mtllight\\sxmtllight.h>

/*! \name Базовые функции библиотеки
@{*/

SX_LIB_API long SPE_0GetVersion();			//!< версия подсистемы
SX_LIB_API void SPE_Dbg_Set(report_func rf);//!< установить функцию обработки сообщений

//! инициализция подсистемы
SX_LIB_API void SPE_0Create(
	const char* name,			//!< присваиваемое имя
	IDirect3DDevice9* device,	//!< dx устройство (уже инициализированное)
	bool is_unic = true			//!< должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SPE_0Kill();	//!< уничтожить подсистему

//!@}

/*! \name Обработка потери/восстановления устройства
@{*/

SX_LIB_API void SPE_OnLostDevice();	//!< вызывать при потрете устройства

//! вызывать при восстановлении устройства, с передачей параметров области в которую рисуем
SX_LIB_API void SPE_OnResetDevice();

//!@}

/*! базис направления взгляда эффекта*/
#define SXPARTICLES_BASIS_DIR float3(0,0,1)

#define SXPARTICLES_DEADTH_TIME 1000
#define SXPARTICLES_POOL_RESERVE 8

//#############################################################################

//! тип фигуры частицы
enum ParticlesFigureType
{
	pft_quad,			//!< обычный квадрат
	pft_billboard,		//!< билборд (квадрат, лицевая сторона которого всегда направлена на наблюдателя)
	pft_quad_composite,	//!< составной квадрат (фигура из нескольких квадратов)
};

//! тип смешивания
enum ParticlesAlphaBlendType
{
	pabt_alpha,			//!< на основе альфа канала
	pabt_add			//!< аддитивное смешивание
};

//! тип ограничивающего объема
enum ParticlesBoundType
{
	pbt_none,	//!< нет объема
	pbt_sphere,	//!< сфера
	pbt_box,	//!< параллелепипед
	pbt_cone	//!< конус
};

//! тип спавна частиц по позиции
enum ParticlesSpawnPosType
{
	pspt_strictly,	//!< строгий в указанном месте
	pspt_bound,		//!< по ограничивающему объему
};

//! тип поворотов частиц
enum ParticlesRotType
{
	prt_none,	//!< нет поворота
	prt_normal,	//!< обычный поворот
	prt_rnd,	//!< рандомный поворот
	prt_evenly,	//!< равномерный поворот
};

//! тип зависимости "чего либо" в частице от времени ее жизни
enum ParticlesDependType
{
	padt_none,		//!< нет зависиомсти
	padt_direct,	//!< прямая зависимость, чем старше тем меньше
	padt_inverse,	//!< обратная зависимость, чем старше тем больше
};

//! ось
enum ParticlesAxis
{
	pa_x,
	pa_y,
	pa_z,
};

//! тип отклонения при движении частицы
enum ParticlesDeviationType
{
	pdt_rnd,	//!< случайное
	pdt_along,	//!< вдоль оси
	pdt_wave,	//!< волновое
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

	////////////////////////////////////////////

	/*! \name Ограничивающий объем
	@{*/
	ParticlesBoundType BoundType;	//!< тип ограничивающего объема, для уничтожения партикла при выходе за пределы

	float4 BoundVec1;
	float4 BoundVec2;
	/*float3 BoundSphereCenter;		//!< центр сферы
	float BoundSphereRadius;		//!< радиус сферы

	float3 BoundBoxMin;				//!< минимум параллелепипеда
	float3 BoundBoxMax;				//!< максимум параллелепипеда

	float3 BoundConeBeginPos;		//!< верхняя позиция конуса
	float3 BoundConeEndPos;			//!< нижняя позиция конуса
	float BoundConeBeginRadius;		//!< верхний радиус конуса
	float BoundConeEndRadius;		//!< нижний радиус конуса*/

	//!@}

	////////////////////////////////////////////

	/*! \name Параметры создания частиц
	 \note Возможна привязка рандомного создания частиц к осям, для ограничивающих объемов (SpawnBoundBindCreate)
	 \note Bound volume - ограничивающий объем из #ParticlesBoundType
	@{*/
	ParticlesSpawnPosType SpawnPosType;	//!< тип спавна партиклов

	float3 SpawnOrigin;					//!< точка из которой идут частицы (обязательно должна быть точка откуда идет спавн частиц если нет BV)
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

	////////////////////////////////////////////

	/*! \name Анимированная текстура
	@{*/

	WORD AnimTexCountCadrsX;	//!< количество кадров по оси X
	WORD AnimTexCountCadrsY;	//!< количество кадров по оси Y

	WORD AnimTexRate;			//!< частота обновления в млсек
	WORD AnimTexRateDisp;		//!< разброс для #AnimTexRate
	WORD AnimTexStartCadr;		//!< стартовый кадр
	WORD AnimTexStartCadrDisp;	//!< разброс для #AnimTexStartCadr

	//!@}

	////////////////////////////////////////////

	/*! \name Время жизни частицы, временные зависимости, размеры
	@{*/

	DWORD TimeLife;						//!< время жизни частиц, если time_life == 0 тогда частицы живут все время, иначе это время в млсекундах
	DWORD TimeLifeDisp;					//!< дисперсия для #TimeLife

	ParticlesDependType AlphaDependAge;	//!< зависит ли альфа компонента частицы от ее возраста
	
	float2_t Size;						//!< размер частиц
	float SizeDisp;						//!< разброс для размера части

	ParticlesDependType SizeDependAge;	//!< зависит ли альфа компонента частицы от ее возраста

	//!@}

	////////////////////////////////////////////

	/*! \name Скорость
	@{*/

	float3 Velocity;		//!< скорость (по осям)
	float VelocityDisp;		//!< дисперсия для #Velocity

	bool VelocityDispXNeg;	//!< отрицательная дисперсия по оси X
	bool VelocityDispYNeg;	//!< отрицательная дисперсия по оси Y
	bool VelocityDispZNeg;	//!< отрицательная дисперсия по оси Z

	//!@}

	////////////////////////////////////////////

	/*! \name Ускорение
	@{*/

	//ускорение
	float3 Acceleration;		//!< ускорение (по осям)
	float AccelerationDisp;		//!< дисперсия для #Acceleration

	bool AccelerationDispXNeg;	//!< отрицательная дисперсия по оси X
	bool AccelerationDispYNeg;	//!< отрицательная дисперсия по оси Y
	bool AccelerationDispZNeg;	//!< отрицательная дисперсия по оси Z

	//!@}

	////////////////////////////////////////////

	/*! \name Характер: круговое движение
	@{*/

	bool CharacterCircle;				//!< разрешено ли использовать круговое движение
	ParticlesAxis CharacterCircleAxis;	//!< ось для кругового движения
	float CharacterCircleAngle;			//!< угол
	float CharacterCircleAngleDisp;		//!< дисперсия для #CharacterCircleAngle
	bool CharacterCircleAngleDispNeg;	//!< разрешено ли использовать отрицательную дисперсию

	//!@}

	////////////////////////////////////////////

	/*! \name Характер вращение
	@{*/

	bool CharacterRotate;				//!< разрешено ли использовать вращение
	float CharacterRotateAngle;			//!< угол
	float CharacterRotateAngleDisp;		//!< дисперсия для #CharacterRotateAngle
	bool CharacterRotateAngleDispNeg;	//!< разрешено ли использовать отрицательную дисперсию

	//!@}

	////////////////////////////////////////////

	/*! \name Характер: отклонения при движении
	@{*/

	bool CharacterDeviation;
	ParticlesDeviationType CharacterDeviationType;	//!< тип смещения
	float CharacterDeviationAmplitude;				//!< амплитуда, может быть отрицательна
	float CharacterDeviationCoefAngle;				//!< коэфициент на который будет умножен угол, только для ParticlesDeviationType::ptd_wave
	ParticlesAxis CharacterDeviationAxis;			//!< ось на основании которой будет вычислен угол поворота, только для ParticlesDeviationType::ptd_wave
	DWORD CharacterDeviationCountDelayMls;			//!< время обновления для #ParticlesDeviationType::ptd_rnd и #ParticlesDeviationType::pdt_along

	float CharacterDeviationCoefAngleDisp;		//!< коэфициент дисперсии, для #CharacterDeviationCoefAngle если #ParticlesDeviationType::ptd_wave, для #CharacterDeviationAmplitude в других случаях
	bool CharacterDeviationCoefAngleDispNeg;	//!< возможна ли отрицательная дисперсия

	bool CharacterDeviationTapX;	//!< задействовать ли координату X при просчете смещения
	bool CharacterDeviationTapY;	//!< задействовать ли координату Y при просчете смещения
	bool CharacterDeviationTapZ;	//!< задействовать ли координату Z при просчете смещения

	//!@}

	////////////////////////////////////////////

	ParticlesFigureType FigureType;	//!< тип фигуры партикла
	int FigureCountQuads;			//!< количество квадаратов в случае ParticlesFigureType::pft_quad_composite
	bool FigureRotRand;				//!< true - случайно генерировать углы поворотов, false - равномерно поворачивать
	bool FigureTapX;				//!< поворачивать ли по оси X
	bool FigureTapY;				//!< поворачивать ли по оси Y
	bool FigureTapZ;				//!< поворачивать ли по оси Z

	ParticlesAlphaBlendType AlphaBlendType;	//!< тип смешивания
	float ColorCoef;		//!< коэфициент на который будет домножен цвет

	int ReCreateCount;		//!< количество создваваемых/пересоздаваемых, 0< - пересоздание в случае нехватки, 0> - единственное создание при запуске

	bool Soft;				//!< мягкие ли частицы
	float SoftCoef;			//!< коэфициент для смягчения партиклов (0-1)

	bool Refraction;		//!< частицы преломляют пространство
	float RefractionCoef;	//!< коэфициент для преломления партиклов (0-1)

	float TransparencyCoef;	//!< коэфициент прозрачности (0-1), компонента альфа канала будет домножена на это значение

	bool Lighting;			//!< освещаемые ли частицы
};

//#############################################################################

/*! \defgroup sxparticles_eff Эффекты
 \ingroup sxparticles
 \warning Обращение (идентификация) конкретного эффекта осуществляется посредством идентификатора который выдает создающая функция #SPE_EffectAdd.
Получить идентификатор уже созданного эффекта можно, посредством ключа (key) который лежит в пределах [0, #SPE_EffectCountGet),
передав этот ключ #SPE_EffectIdOfKey, которая в случае успеха вернет идентификатор эффекта.
@{*/

SX_LIB_API void SPE_EffectLoad(const char* path);	//!< загрузка информации об эффектах и эмиттерах из файла
SX_LIB_API void SPE_EffectSave(const char* path);	//!< сохранение информации об эффектах и эмиттерах в файл
SX_LIB_API void SPE_EffectsClear();	//!< очистка всего списка эффектов и эмиттеров

SX_LIB_API ID SPE_EffectInstanceByName(const char* name);
SX_LIB_API ID SPE_EffectInstanceByID(ID id);
SX_LIB_API ID SPE_EffectGetByName(const char* name);

SX_LIB_API ID SPE_EffectAdd(const char* name);	//!< добавить эффект и установить ему имя
SX_LIB_API int SPE_EffectCountGet();			//!< возвращает количество эффектов
SX_LIB_API ID SPE_EffectIdOfKey(ID key);		//!< возвращает идентификатор эффекта по порядковому номеру key
SX_LIB_API void SPE_EffectDelete(ID id);		//!< удаляет эффект
SX_LIB_API void SPE_EffectNameSet(ID id, const char* name);	//!< установка имени 
SX_LIB_API void SPE_EffectNameGet(ID id, char* name);		//!< в name записывает имя эффекта

SX_LIB_API void SPE_EffectCompute(ID id);					//!< просчет и обновление всех партиклов в эффекте
SX_LIB_API void SPE_EffectComputeLighting(ID id);			//!< просчет света для все частиц эффекта
SX_LIB_API void SPE_EffectRender(ID id, DWORD timeDelta);	//!< рендер эффекта, timeDelta - время рендера кадра

SX_LIB_API void SPE_EffectComputeAll();					//!< просчет и обновление всех партиклов во всех эффектах
SX_LIB_API void SPE_EffectComputeLightingAll();			//!< просчет света для все частиц во всех эффектах
SX_LIB_API void SPE_EffectRenderAll(DWORD timeDelta);	//!< отрисовка всех эффектов

SX_LIB_API bool SPE_EffectAlifeGet(ID id);				//<! жив ли эффект, или все частицы в нем уже умерли?
SX_LIB_API void SPE_EffectAlifeSet(ID id, bool alife);	//<! установка состояния жизни

SX_LIB_API bool SPE_EffectEnableGet(ID id);				//<! включен ли эффект
SX_LIB_API void SPE_EffectEnableSet(ID id, bool enable);//<! устанавливает состяние включен/выключен для эффекта

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

SX_LIB_API void SPE_EffectPosSet(ID id, float3* pos);	//<! установка позиции эффекту
SX_LIB_API void SPE_EffectDirSet(ID id, float3* dir);	//<! установка направления взгляда эффекта
SX_LIB_API void SPE_EffectRotSet(ID id, float3* rot);	//<! установка поворотов эффекта (в радианах)

SX_LIB_API void SPE_EffectPosGet(ID id, float3* pos);	//<! в pos записывает позицию эффекта
SX_LIB_API void SPE_EffectDirGet(ID id, float3* dir);	//<! в dir записывает  направление взгляда эффекта
SX_LIB_API void SPE_EffectRotGet(ID id, float3* rot);	//<! в rot записывает повороты эффекта (в радианах)

//! просчет видимости эффекта для фрустума frustum, и расчет расстояния от позиции наблюдателя view до эффекта, возвращает виден ли фруустуму эффект
SX_LIB_API bool SPE_EffectVisibleCom(ID id, ISXFrustum* frustum, float3* view);	

//! просчет видимости всех эффектов для фрустума frustum, и расчет расстояний от позиции наблюдателя view до всех эффектов
SX_LIB_API void SPE_EffectVisibleComAll(ISXFrustum* frustum, float3* view);

SX_LIB_API bool SPE_EffectVisibleGet(ID id);		//!< виден ли эффект? по результатам просчетов #SPE_EffectVisibleComAll
SX_LIB_API float SPE_EffectDistToViewGet(ID id);	//!< возвращает расстояние от наблюдателя до эффекта, по результатам просчетов #SPE_EffectVisibleComAll

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

SX_LIB_API ID SPE_EmitterAdd(ID id, ParticlesData* data);	//!< добавляет систему партиклов к эффекту
SX_LIB_API int SPE_EmitterSCountGet(ID id);				//!< возвращает количество систем партиклов у эффекта
SX_LIB_API void SPE_EmitterDelete(ID id, ID id_part);		//!< удаляет из эффекта id систему частиц id_part
SX_LIB_API ParticlesData* SPE_EmitterGetData(ID id, ID id_part);	//!< возвращает структуру данных системы частиц id_part из эффекта id

//! переустановка структуры данных data для системы частиц id_part у эффекта id, в data можно отправить 0 в случае если необходимо обновить состояния оффлайн данных
SX_LIB_API void SPE_EmitterReInit(ID id, ID id_part, ParticlesData* data);

SX_LIB_API void SPE_EmitterCountSet(ID id, ID id_part, int count);
SX_LIB_API int SPE_EmitterCountGet(ID id, ID id_part);
SX_LIB_API int SPE_EmitterCountLifeGet(ID id, ID id_part);

SX_LIB_API void SPE_EmitterEnableSet(ID id, ID id_part, bool enable);
SX_LIB_API bool SPE_EmitterEnableGet(ID id, ID id_part);

//! установка текстуры (идентификатора на нее) системе частиц id_part эффекта id
SX_LIB_API void SPE_EmitterTextureSet(ID id, ID id_part, const char* tex);
SX_LIB_API void SPE_EmitterTextureSetID(ID id, ID id_part, ID tex);
SX_LIB_API ID SPE_EmitterTextureGetID(ID id, ID id_part);
SX_LIB_API void SPE_EmitterTextureGet(ID id, ID id_part, char* tex);

SX_LIB_API void SPE_EmitterNameSet(ID id, ID id_part, const char* name);	//!< установка имени системе частиц
SX_LIB_API void SPE_EmitterNameGet(ID id, ID id_part, char* name);		//!< в name записывает имя системы частиц

//!@} sxparticles_part

#endif

//!@} sxparticles