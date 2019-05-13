#ifndef __IXMODEL_H
#define __IXMODEL_H

#include <gdefines.h>
#include <anim/ModelFile.h>

class IXStaticModel;
class IXDynamicModel;
class IXAnimatedModel;
// No direct implementation
class IXModel: public IXUnknown
{
public:
	virtual IXAnimatedModel * XMETHODCALLTYPE asAnimatedModel() = 0;
	virtual IXDynamicModel * XMETHODCALLTYPE asDynamicModel() = 0;
	virtual IXStaticModel * XMETHODCALLTYPE asStaticModel() = 0;
};

// Implemented in geom plugin
class IXStaticModel: public IXModel
{
public:
	virtual float3 XMETHODCALLTYPE getPosition() const = 0;
	virtual void XMETHODCALLTYPE setPosition(const float3 &vPos) = 0;

	virtual SMQuaternion XMETHODCALLTYPE getOrientation() const = 0;
	virtual void XMETHODCALLTYPE setOrientation(const SMQuaternion &qRot) = 0;

	virtual UINT XMETHODCALLTYPE getSkin() const = 0;
	virtual void XMETHODCALLTYPE setSkin(UINT uSkin) = 0;

	virtual ISXBound * XMETHODCALLTYPE getLocalBound() const = 0;

	virtual float4 XMETHODCALLTYPE getColor() const = 0;
	virtual void XMETHODCALLTYPE setColor(const float4 &vColor) = 0;
};

// Implemented in anim plugin
class IXDynamicModel: public IXStaticModel
{
public:
};

// Implemented in anim plugin
class IXAnimatedModel: public IXDynamicModel
{
public:
	virtual const ModelHitbox * XMETHODCALLTYPE getHitbox(UINT id) const = 0;
	virtual UINT XMETHODCALLTYPE getHitboxCount() const = 0;

	/*! Запускает воспроизведения анимации
		@param[in] szName Имя анимации
		@param[in] uFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] uSlot Слот для воспроизведения. От 0 до BLEND_MAX
		@param[in] bReplaceActivity Остановить ли активность в заданном слоте?
	*/
	virtual void XMETHODCALLTYPE play(const char *szName, UINT uFadeTime = 0, UINT uSlot = 0, bool bReplaceActivity = true) = 0;

	/*! Останавливает воспроизведения для указанного слота
		@param[in] uSlot Слот для остановки. От 0 до BLEND_MAX
	*/
	virtual void XMETHODCALLTYPE stop(UINT uSlot = 0) = 0;

	/*! Возобновляет воспроизведения для указанного слота
		@param[in] uSlot Слот для возобновления. От 0 до BLEND_MAX
	*/
	virtual void XMETHODCALLTYPE resume(UINT uSlot = 0) = 0;

	/*! Устанавливает прогресс воспроизведения в конкретном слоте
		@warning Если в конкретном слоте не происходит воспроизведения, не будет эффекта
		@param[in] fProgress Значение прогресса. От 0 до 1
		@param[in] uSlot Слот. От 0 до BLEND_MAX
	*/
	virtual void XMETHODCALLTYPE setProgress(float fProgress, UINT uSlot = 0) = 0;

	/*! Разрешает/запрещает обновление конкретного слота
		@param[in] bAllow разрешить/запретить
		@param[in] slot Слот. От 0 до BLEND_MAX
	*/
	virtual void XMETHODCALLTYPE allowAdvance(bool bAllow, UINT slot = 0) = 0;

	/*! Запускает воспроизведение указанной активности в заданном слоте
		@param[in] szName Имя анимации
		@param[in] uFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] uSlot Слот для воспроизведения. От 0 до BLEND_MAX
	*/
	virtual void XMETHODCALLTYPE startActivity(const char *szName, UINT uFadeTime = 0, UINT uSlot = 0) = 0;

	/*! Возвращает смещение указанной кости
		@param[in] id Номер кости
		@return Смещение кости
	*/
	virtual float3 XMETHODCALLTYPE getBoneTransformPos(UINT id) = 0;

	/*! Возвращает вращение указанной кости
		@param[in] id Номер кости
		@return Вращение кости
	*/
	virtual SMQuaternion XMETHODCALLTYPE getBoneTransformRot(UINT id) = 0;

	/*! Возвращает трансформацию указанной кости
		@param[in] id Номер кости
		@return Матрица трансформации кости
	*/
	virtual SMMATRIX XMETHODCALLTYPE getBoneTransform(UINT id) = 0;

	/*! Возвращает идентификатор указанной кости
		@param[in] szName Имя кости
		@return Номер кости
	*/
	virtual UINT XMETHODCALLTYPE getBoneId(const char *szName) = 0;

	/*! Возвращает количество костей
		@return Количество костей
	*/
	virtual UINT XMETHODCALLTYPE getBoneCount() const = 0;

	/*! Возвращает имя указанной кости
		@param[in] id Номер кости
	*/
	virtual const char * XMETHODCALLTYPE getBoneName(UINT id) const = 0;

	/*! Проверяет, воспроизводится ли анимация
	*/
	virtual bool XMETHODCALLTYPE isPlayingAnimations() = 0;

	/*! Проверяет, воспроизводится ли анимация
		@param[in] szName Имя анимации
	*/
	virtual bool XMETHODCALLTYPE isPlayingAnimation(const char *szName) = 0;

	/*! Останавливает воспроизведение всех анимаций
	*/
	virtual void XMETHODCALLTYPE stopAll() = 0;

	/*! Устанавливает значение для заданного контроллера
		@param[in] id Идентификатор контроллера
		@param[in] fValue Значение от 0 до 1
		@param[in] what Какую часть контроллера изменить
	*/
	virtual void XMETHODCALLTYPE setController(UINT id, float fValue, MODEL_BONE_CTL what) = 0;

	virtual UINT XMETHODCALLTYPE getControllersCount() const = 0;
	virtual const char * XMETHODCALLTYPE getControllerName(UINT id);
	virtual UINT XMETHODCALLTYPE getControllerId(const char *szName);

	// Коллбек на изменение состояния анимации!
};

#endif
