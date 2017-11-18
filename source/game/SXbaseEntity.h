
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый игровой объект
*/

/*! \defgroup cbaseentity Объекты. Базовый объект
\ingroup sxgame
Базовый объект, от которого наследуются все остальные классы объектов.
@{
*/

#ifndef _SXbaseEntity_H_
#define _SXbaseEntity_H_

#include <gdefines.h>
#include <common/SXMath.h>
#include <physics/sxphysics.h>

#include "EntityFactory.h"
#include "EntityManager.h"

#include "proptable.h"

#pragma pointers_to_members(full_generality, virtual_inheritance)

#pragma warning(push)
#pragma warning(disable:4251)

//! Базовый объект, от которого наследуются все остальные классы объектов.
class SXGAME_EXPORT SXbaseEntity
{
	DECLARE_CLASS_NOBASE(SXbaseEntity);
	DECLARE_PROPTABLE();

	friend class EntityManager;

public:
	//! Конструктор
	SXbaseEntity(EntityManager * pMgr);
	virtual ~SXbaseEntity();

	//! Возвращает имя движкового класса объекта
	const char * GetClassName();
	
	//! Возвращает имя объекта
	const char * GetName();
	
	//! Возвращает баунд объекта
	virtual void GetMinMax(float3 * min, float3 * max);
	
	//! Возвращает баунд сферу объекта
	virtual void GetSphere(float3 * center, float * radius);

	//! Устанавливает мировую позицию объекта
	virtual void SetPos(const float3 & pos);
	//! Получает мировую позицию объекта
	float3 GetPos();

	//! Получает ID объекта в системе
	ID GetId();

	//! Получает флаги объекта
	UINT GetFlags();
	//! Устанавливает флаги объекта
	void SetFlags(UINT f);

	//! Получает мировую матрицу трансформации для объекта
	SMMATRIX GetWorldTM();

	//! Устанавливает вращение объекта
	virtual void SetOrient(const SMQuaternion & q);
	//! Возвращает вращение объекта
	SMQuaternion GetOrient();

	//! Устанавливает свойство объекта
	virtual bool SetKV(const char * name, const char * value);
	//! Получает свойство объекта
	virtual bool GetKV(const char * name, char * out, int bufsize);

	//! Устанавливает родительский объект для объекта
	void SetParent(SXbaseEntity * pEnt, int attachment = -1);
	//! Возвращает родительский объект объекта
	SXbaseEntity * GetParent();

	//! Устанавливает владельца
	void SetOwner(SXbaseEntity * pEnt);
	//! Возвращает владельца объекта
	SXbaseEntity * GetOwner();

	//! Получает объект менеджера
	EntityManager * getManager();

	//void updateDiscreteLinearVelocity(int step, float dt);
	//const float3_t & getDiscreteLinearVelocity() const;

private:
	void SetClassName(const char * name);
	void SetDefaults();

	const char * m_szClassName;

protected:
	EntityManager * m_pMgr;

	//! Позиция объекта
	float3_t m_vPosition;
	
	/*! Скорость
	\note вероятно, не используется
	*/
	float3_t m_vSpeed;
	
	//! вращение
	SMQuaternion m_vOrientation;
	//float3_t m_vDiscreteLinearVelocity;
	//float3_t m_vOldPosition;

	//! Позиция смещения (для иерархической структуры)
	float3_t m_vOffsetPos;
	//! Вращение смещения (для иерархической структуры)
	SMQuaternion m_vOffsetOrient;

	//! Идентификатор в системе
	ID m_iId;

	//! Флаги
	UINT m_iFlags;

	//! Имя объекта
	const char * m_szName;

	//! Родитель
	SXbaseEntity * m_pParent;
	//! Индекс кости родителя
	int m_iParentAttachment;

	//! Владелец
	SXbaseEntity * m_pOwner;

	//! Вызывается на стадии синхронизации
	virtual void OnSync();
	//! Вызывается при создании после установки всех свойств
	virtual void OnPostLoad();

	//! Получает смещение для кости
	virtual float3 GetAttachmentPos(int id);
	//! Получает вращение для кости
	virtual SMQuaternion GetAttachmentRot(int id);

	/*! Устанавливает значение строкового свойства
	\note только для внутреннего использования
	*/
	void _SetStrVal(const char ** to, const char * value);

	/*! Обновляет выходы
	\note только для внутреннего использования
	*/
	void updateOutputs();
};

#pragma warning(pop)

#endif

/*! @} cbaseentity*/
