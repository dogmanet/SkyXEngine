
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxanim - библиотека скелетной анимации
*/

/*! \defgroup sxanim sxanim - библиотека скелетной анимации
@{
*/


#ifndef __SXANIM_H
#define __SXANIM_H

#include "ModelFile.h"

#include <common/assotiativearray.h>
#include <common/array.h>
#include <common/string.h>

#ifndef SX_DLL
#define SX_DLL
#endif

#define SX_ANIM_DEFAULT_VISCALCOBJ 0

#include <gdefines.h>

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#ifdef _SERVER
#	define IDirect3DDevice9 void
#else
#	define D3D_DEBUG_INFO
#	include <d3d9.h>
#	include <d3dx9.h>
#	pragma comment(lib, "d3dx9.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

class IAnimPlayer;
class ISXFrustum;
class ISXBound;
class CBaseAnimating;

typedef void(*AnimStateCB)(int slot, ANIM_STATE as, IAnimPlayer * pAnim);      //!< коллбэк для определения изменения состояния воспроизведения
typedef void(CBaseAnimating::*AnimStateEntCB)(int slot, ANIM_STATE as);      //!< коллбэк для определения изменения состояния воспроизведения
typedef void(*AnimProgressCB)(int slot, float progress, IAnimPlayer * pAnim);  //!< коллбэк для определения изменения прогресса воспроизведения

//! Интерфейс рэгдолла
class IAnimRagdoll
{
public:
	IAnimRagdoll()
	{
	};
	virtual ~IAnimRagdoll()
	{
	};

	virtual bool isBoneAffected(ID id) = 0;
	virtual float3 getBoneOffset(ID id) = 0;
	virtual SMQuaternion getBoneRotation(ID id) = 0;
};

//! Интерфейс проигрывателя анимаций
class IAnimPlayer
{
public:
	IAnimPlayer()
	{
	};
	virtual ~IAnimPlayer()
	{
	};

	SX_ALIGNED_OP_MEM;

	/*! Выполняет обновление состояния воспроизведения анимации
		@param[in] dt Текущее абсолютное время в микросекундах
	*/
	virtual void advance(unsigned long int dt) = 0;

#ifndef _SERVER
	//! Выполняет отрисовку модели
	virtual void render() = 0;
#endif

	/*! Загружает новую модель для воспроизведения
		@param[in] file Имя файла модели
	*/
	virtual void setModel(const char * file) = 0;

	/*! Запускает воспроизведения анимации
		@param[in] name Имя анимации
		@param[in] iFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] slot Слот для воспроизведения. От 0 до BLEND_MAX
	*/
	virtual void play(const char * name, UINT iFadeTime = 0, UINT slot = 0, bool bReplaceActivity=true) = 0;

	/*! Останавливает воспроизведения для указанного слота
		@param[in] slot Слот для остановки. От 0 до BLEND_MAX
	*/
	virtual void stop(UINT slot = 0) = 0;

	/*! Возобновляет воспроизведения для указанного слота
		@param[in] slot Слот для возобновления. От 0 до BLEND_MAX
	*/
	virtual void resume(UINT slot = 0) = 0;

	/*! Устанавливает прогресс воспроизведения в конкретном слоте
		@warning Если в конкретном слоте не происходит воспроизведения, не будет эффекта
		@param[in] progress Значение прогресса. От 0 до 1
		@param[in] slot Слот. От 0 до BLEND_MAX
	*/
	virtual void setProgress(float progress, UINT slot = 0) = 0;

	/*! Разрешает/запрещает обновление конкретного слота
		@param[in] set разрешить/запретить
		@param[in] slot Слот. От 0 до BLEND_MAX
	*/
	virtual void setAdvance(bool set, UINT slot = 0) = 0;

	/*! Запускает воспроизведение указанной активности в заданном слоте
		@param[in] name Имя анимации
		@param[in] iFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] slot Слот для воспроизведения. От 0 до BLEND_MAX

		@warning В данный момент не работат
	*/
	virtual void startActivity(const char * name, UINT iFadeTime = 0, UINT slot = 0) = 0;

	/*! Устанавливает значение для заданного контроллера
		@param[in] name Имя контроллера
		@param[in] value Значение от 0 до 1
		@param[in] what Какую часть контроллера изменить

		@warning В данный момент не работат
	*/
	virtual void setBoneController(const String & name, float value, MODEL_BONE_CTL what) = 0;

	/*! Возвращает смещение указанной кости
		@param[in] id Номер кости
		@return Смещение кости
	*/
	virtual float3 getBoneTransformPos(UINT id) = 0;

	/*! Возвращает вращение указанной кости
		@param[in] id Номер кости
		@return Вращение кости
	*/
	virtual SMQuaternion getBoneTransformRot(UINT id) = 0;

	/*! Возвращает трансформацию указанной кости
		@param[in] id Номер кости
		@param[in] bWithScale Учитывать ли масштабирование
		@return Матрица трансформации кости
	*/
	virtual SMMATRIX getBoneTransform(UINT id, bool bWithScale = false) = 0;

	/*! Возвращает идентификатор указанной кости
		@param[in] str Имя кости
		@return Номер кости
	*/
	virtual UINT getBone(const char * str) = 0;

	/*! Возвращает количество костей
		@return Количество костей
	*/
	virtual UINT getBoneCount() const = 0;

	/*! Возвращает имя указанной кости
		@param[in] id Номер кости
		@param[out] name Буфер для записи имени
		@param[out] len Размер буфера для записи
	*/
	virtual void getBoneName(UINT id, char * name, int len) const = 0;

	/*! Проверяет, воспроизводится ли анимация
	*/
	virtual inline bool playingAnimations() = 0;

	/*! Проверяет, воспроизводится ли анимация
		@param[in] name Имя анимации
	*/
	virtual inline bool playingAnimations(const char* name) = 0;

	/*! Останавливает воспроизведение всех анимаций
	*/
	virtual void stopAll() = 0;

	/*! Возвращает номер текущего скина
	*/
	virtual int getActiveSkin() = 0;

	/*! Устанавливает активный скин
		@param[in] n Номер скина
	*/
	virtual void setSkin(int n) = 0;

	/*! Устанавливает мировую позицию модели
		@param[in] pos позиция
	*/
	virtual void setPos(const float3 & pos) = 0;

	/*! Возвращает мировую позицию модели
	*/
	virtual float3 getPos() = 0;

	/*! Устанавливает вращение модели
		@param[in] pos позиция
	*/
	virtual void setOrient(const SMQuaternion & pos) = 0;

	/*! Возвращает вращение модели
	*/
	virtual SMQuaternion getOrient() = 0;

	/*! Получает мировую матрицу модели
	*/
	virtual SMMATRIX getWorldTM() = 0;

	/*! Устанавливает коллбэк на изменение состояния объекта
	*/
	virtual AnimStateCB setCallback(AnimStateCB cb) = 0;

	/*! Устанавливает коллбэк на изменение состояния объекта
	*/
	virtual void setCallback(CBaseAnimating *pEnt, AnimStateEntCB cb) = 0;

	/*! Устанавливает коллбэк на изменение состояния воспроизведения
	*/
	virtual AnimProgressCB setProgressCB(AnimProgressCB cb) = 0;

	/*! Получает текущую воспроизводимую анимацию в слоте
	*/
	virtual ModelSequence const * getCurAnim(int slot) = 0;

	/*! Получает ограничивающий объем модели
	*/
	virtual const ISXBound * getBound() const = 0;

	/*! Уничтожает объект
	*/
	virtual void Release() = 0;

	/*! Устанавливает масштаб
	*/
	virtual void setScale(float fScale) = 0;
	virtual float getScale() = 0;

	virtual void getPhysData(
		int32_t * piShapeCount,
		HITBOX_TYPE ** phTypes,
		float3_t *** pppfData,
		int32_t ** ppfDataLen
		) = 0;

	virtual void freePhysData(
		int32_t iShapeCount,
		HITBOX_TYPE * hTypes,
		float3_t ** ppfData,
		int32_t * pfDataLen
		) = 0;


	virtual const ModelFile * addModel(const char * mdl, UINT flags = MI_ALL, char * name = "No name") = 0;
	virtual void addModel(const ModelFile * mdl, UINT flags = MI_ALL, char * name = "No name") = 0;
	virtual int addModel(ModelPart * mp) = 0;
	virtual void delModel(ModelPart * mp) = 0;
	virtual void assembly() = 0;

	virtual const ModelHitbox * getHitbox(uint32_t id) const = 0;
	virtual uint32_t getHitboxCount() const = 0;

	virtual void setOverrideMaterial(const char * name) = 0;
	virtual void enable(bool enable) = 0;

	virtual void setRagdoll(IAnimRagdoll * pRagdoll) = 0;

	virtual bool isVisibleFor(ID id) = 0;
};

//! \name Функции управления анимацией
//!@{

/*! Инициализирует библиотеку
*/
SX_LIB_API void SXAnim_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXAnim_AKill();

/*! Устанавливает функцию сообщений
*/
SX_LIB_API void SXAnim_Dbg_Set(report_func rf);

/*! Устанавливает количество потоков обновления анимации
*/
SX_LIB_API void SXAnim_UpdateSetThreadNum(int num);

/*! Выполняет обновление анимаций
	@param[in] thread Номер потока, выполняющего обновление
*/
SX_LIB_API void SXAnim_Update(int thread = 0);

/*! Выполняет синхронизацию. Для многопоточного обновления
*/
SX_LIB_API void SXAnim_Sync();

/*! Выполняет рендер всех анимаций с учетом видимости
*/
SX_LIB_API void SXAnim_Render(ID for_id=0);

/*! Создает проигрыватель анимации. 
	@param[in] mdl Имя модели для загрузки. Может быть NULL
*/
SX_LIB_API IAnimPlayer * SXAnim_CreatePlayer(const char * mdl = NULL);

//! просчитать видимость всех моделей для фрустума 
SX_LIB_API void SXAnim_ModelsComVisible(
	const IFrustum * frustum,	//!< фрустум для которого считаем видимость моделей
	const float3 * viewpos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//!< идентификатор массива информации о видимости для фрустума, создается через SXAnim_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

SX_LIB_API ID SXAnim_ModelsAddArrForCom();				//!< добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SXAnim_ModelsDelArrForCom(ID id_arr);	//!< удалить массив просчетов с номером id_arr

/*! @} sxanim */

#endif

/*! @} */

