#ifndef __IXRENDERABLE_H
#define __IXRENDERABLE_H

#include <gdefines.h>



class IXStaticModel;
class IXDynamicModel;
class IXAnimatedModel;
class IXModel: public IXUnknown
{
public:
	virtual IXAnimatedModel *asAnimatedModel() = 0;
	virtual IXDynamicModel *asDynamicModel() = 0;
	virtual IXStaticModel *asStaticModel() = 0;


};

class IXStaticModel: public IXModel
{
public:
	virtual float3 getPosition() const = 0;
	virtual SMQuaternion getOrientation() const = 0;

	virtual UINT getSkin() const = 0;
	virtual void setSkin(UINT uSkin) = 0;

	virtual ISXBound *getLocalBound() const = 0;

	virtual float4 getColor() const = 0;
	virtual setColor(const float4 vColor) = 0;
};

class IXDynamicModel: public IXModel
{
public:
	virtual float3 getPosition() const = 0;
	virtual void setPosition(const float3 &vPos) = 0;

	virtual SMQuaternion getOrientation() const = 0;
	virtual void setOrientation(const SMQuaternion &qRot) = 0;

	virtual UINT getSkin() const = 0;
	virtual void setSkin(UINT uSkin) = 0;

	virtual ISXBound *getLocalBound() const = 0;

	virtual float4 getColor() const = 0;
	virtual setColor(const float4 vColor) = 0;
};

class IXAnimatedModel: public IXDynamicModel
{
public:
	virtual const ModelHitbox *getHitbox(UINT id) const = 0;
	virtual UINT getHitboxCount() const = 0;

	/*! Запускает воспроизведения анимации
		@param[in] szName Имя анимации
		@param[in] uFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] uSlot Слот для воспроизведения. От 0 до BLEND_MAX
		@param[in] bReplaceActivity Остановить ли активность в заданном слоте?
	*/
	virtual void play(const char *szName, UINT uFadeTime = 0, UINT uSlot = 0, bool bReplaceActivity = true) = 0;

	/*! Останавливает воспроизведения для указанного слота
		@param[in] uSlot Слот для остановки. От 0 до BLEND_MAX
	*/
	virtual void stop(UINT uSlot = 0) = 0;

	/*! Возобновляет воспроизведения для указанного слота
		@param[in] uSlot Слот для возобновления. От 0 до BLEND_MAX
	*/
	virtual void resume(UINT uSlot = 0) = 0;

	/*! Устанавливает прогресс воспроизведения в конкретном слоте
		@warning Если в конкретном слоте не происходит воспроизведения, не будет эффекта
		@param[in] fProgress Значение прогресса. От 0 до 1
		@param[in] uSlot Слот. От 0 до BLEND_MAX
	*/
	virtual void setProgress(float fProgress, UINT uSlot = 0) = 0;

	/*! Разрешает/запрещает обновление конкретного слота
		@param[in] bAllow разрешить/запретить
		@param[in] slot Слот. От 0 до BLEND_MAX
	*/
	virtual void allowAdvance(bool bAllow, UINT slot = 0) = 0;

	/*! Запускает воспроизведение указанной активности в заданном слоте
		@param[in] szName Имя анимации
		@param[in] uFadeTime Время перехода от предыдущей анимации к новой в ms
		@param[in] uSlot Слот для воспроизведения. От 0 до BLEND_MAX
	*/
	virtual void startActivity(const char *szName, UINT uFadeTime = 0, UINT uSlot = 0) = 0;

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
		@return Матрица трансформации кости
	*/
	virtual SMMATRIX getBoneTransform(UINT id) = 0;

	/*! Возвращает идентификатор указанной кости
		@param[in] szName Имя кости
		@return Номер кости
	*/
	virtual UINT getBoneId(const char *szName) = 0;

	/*! Возвращает количество костей
		@return Количество костей
	*/
	virtual UINT getBoneCount() const = 0;

	/*! Возвращает имя указанной кости
		@param[in] id Номер кости
	*/
	virtual const char *getBoneName(UINT id) const = 0;

	/*! Проверяет, воспроизводится ли анимация
	*/
	virtual bool isPlayingAnimations() = 0;

	/*! Проверяет, воспроизводится ли анимация
		@param[in] szName Имя анимации
	*/
	virtual bool isPlayingAnimation(const char *szName) = 0;

	/*! Останавливает воспроизведение всех анимаций
	*/
	virtual void stopAll() = 0;

	/*! Устанавливает значение для заданного контроллера
		@param[in] id Идентификатор контроллера
		@param[in] fValue Значение от 0 до 1
		@param[in] what Какую часть контроллера изменить
	*/
	virtual void setController(UINT id, float fValue, MODEL_BONE_CTL what) = 0;

	virtual UINT getControllersCount() const = 0;
	virtual const char *getControllerName(UINT id);
	virtual UINT getControllerId(const char *szName);

	// Коллбек на изменение состояния анимации!
};



class IXStaticModelProvider: public IXUnknown
{
public:
};
class IXDynamicModelProvider: public IXUnknown
{
public:
};
class IXAnimatedModelProvider: public IXUnknown
{
public:
};

class IXModelProvider: public IXUnknown
{
public:
	virtual bool loadStaticModel(const char *szName, IXStaticModel **ppStaticModel) = 0;
	virtual bool loadDynamicModel(const char *szName, IXDynamicModel **ppStaticModel) = 0;
};


#endif
