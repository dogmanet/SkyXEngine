
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxanim - библиотека скелетной анимации
*/

/*! \defgroup sxanim sxanim - библиотека скелетной анимации
@{
*/


#ifndef _SXANIM_H_
#define _SXANIM_H_


#include "ModelFile.h"

#include <common/assotiativearray.h>
#include <common/array.h>
#include <common/string.h>

#define SX_DLL

#include <gdefines.h>

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

typedef void(*AnimStateCB)(int slot, ANIM_STATE as, IAnimPlayer * pAnim);      //!< коллбэк для определения изменения состояния воспроизведения
typedef void(*AnimProgressCB)(int slot, float progress, IAnimPlayer * pAnim);  //!< коллбэк для определения изменения прогресса воспроизведения

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
	virtual void Advance(unsigned long int dt) = 0;

#ifndef _SERVER
	//! Выполняет отрисовку модели
	virtual void Render() = 0;
#endif

	/*! Загружает новую модель для воспроизведения
		@param[in] file Имя файла модели
	*/
	virtual void SetModel(const char * file) = 0;

	/*! Запускает воспроизведения анимации
		@param[in] name Имя анимации
		@param[in] iFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] slot Слот для воспроизведения. От 0 до BLEND_MAX
	*/
	virtual void Play(const char * name, UINT iFadeTime = 0, UINT slot = 0) = 0;

	/*! Останавливает воспроизведения для указанного слота
		@param[in] slot Слот для остановки. От 0 до BLEND_MAX
	*/
	virtual void Stop(UINT slot = 0) = 0;

	/*! Возобновляет воспроизведения для указанного слота
		@param[in] slot Слот для возобновления. От 0 до BLEND_MAX
	*/
	virtual void Resume(UINT slot = 0) = 0;

	/*! Устанавливает прогресс воспроизведения в конкретном слоте
		@warning Если в конкретном слоте не происходит воспроизведения, не будет эффекта
		@param[in] progress Значение прогресса. От 0 до 1
		@param[in] slot Слот. От 0 до BLEND_MAX
	*/
	virtual void SetProgress(float progress, UINT slot = 0) = 0;

	/*! Разрешает/запрещает обновление конкретного слота
		@param[in] set разрешить/запретить
		@param[in] slot Слот. От 0 до BLEND_MAX
	*/
	virtual void SetAdvance(bool set, UINT slot = 0) = 0;

	/*! Запускает воспроизведение указанной активности в заданном слоте
		@param[in] name Имя анимации
		@param[in] iFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] slot Слот для воспроизведения. От 0 до BLEND_MAX

		@warning В данный момент не работат
	*/
	virtual void StartActivity(const String & name, UINT iFadeTime = 0, UINT slot = 0) = 0;

	/*! Устанавливает значение для заданного контроллера
		@param[in] name Имя контроллера
		@param[in] value Значение от 0 до 1
		@param[in] what Какую часть контроллера изменить

		@warning В данный момент не работат
	*/
	virtual void SetBoneController(const String & name, float value, MODEL_BONE_CTL what) = 0;

	/*! Возвращает трансформацию указанной кости
		@param[in] id Номер кости
		@return Матрица трансформации кости
	*/
	virtual SMMATRIX GetBoneTransform(UINT id) = 0;

	/*! Возвращает идентификатор указанной кости
		@param[in] str Имя кости
		@return Номер кости
	*/
	virtual UINT GetBone(const char * str) = 0;

	/*! Возвращает количество костей
		@return Количество костей
	*/
	virtual UINT GetBoneCount() const = 0;

	/*! Возвращает имя указанной кости
		@param[in] id Номер кости
		@param[out] name Буфер для записи имени
		@param[out] len Размер буфера для записи
	*/
	virtual void GetBoneName(UINT id, char * name, int len) const = 0;

	/*! Проверяет, воспроизводится ли анимация
	*/
	virtual inline bool PlayingAnimations() = 0;

	/*! Проверяет, воспроизводится ли анимация
		@param[in] name Имя анимации
	*/
	virtual inline bool PlayingAnimations(const char* name) = 0;

	/*! Останавливает воспроизведение всех анимаций
	*/
	virtual void StopAll() = 0;

	/*! Возвращает номер текущего скина
	*/
	virtual int GetActiveSkin() = 0;

	/*! Устанавливает активный скин
		@param[in] n Номер скина
	*/
	virtual void SetSkin(int n) = 0;

	/*! Устанавливает мировую позицию модели
		@param[in] pos позиция
	*/
	virtual void SetPos(const float3 & pos) = 0;

	/*! Возвращает мировую позицию модели
	*/
	virtual float3 GetPos() = 0;

	/*! Устанавливает вращение модели
		@param[in] pos позиция
	*/
	virtual void SetOrient(const SMQuaternion & pos) = 0;

	/*! Возвращает вращение модели
	*/
	virtual SMQuaternion GetOrient() = 0;

	/*! Получает мировую матрицу модели
	*/
	virtual SMMATRIX GetWorldTM() = 0;

	/*! Устанавливает коллбэк на изменение состояния объекта
	*/
	virtual AnimStateCB SetCallback(AnimStateCB cb) = 0;

	/*! Устанавливает коллбэк на изменение состояния воспроизведения
	*/
	virtual AnimProgressCB SetProgressCB(AnimProgressCB cb) = 0;

	/*! Получает текущую воспроизводимую анимацию в слоте
	*/
	virtual ModelSequence const * GetCurAnim(int slot) = 0;

	/*! Получает ограничивающий объем модели
	*/
	virtual const ISXBound * GetBound() const = 0;
};

//! \name Функции управления анимацией
//!@{

/*! Инициализирует библиотеку
*/
SX_LIB_API void SXAnim_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXAnim_0Kill();

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
	const ISXFrustum * frustum,	//!< фрустум для которого считаем видимость моделей
	const float3 * viewpos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//!< идентификатор массива информации о видимости для фрустума, создается через SXAnim_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

SX_LIB_API ID SXAnim_ModelsAddArrForCom();				//!< добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SXAnim_ModelsDelArrForCom(ID id_arr);	//!< удалить массив просчетов с номером id_arr

/*! @} sxanim */

#endif

/*! @} */

