
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый игровой объект
*/

/*! \defgroup cbaseentity Объекты. Базовый объект
\ingroup sxgame
Базовый объект, от которого наследуются все остальные классы объектов.
@{
*/

#ifndef __BASE_ENTITY_H
#define __BASE_ENTITY_H

#include <gdefines.h>
#include <common/SXMath.h>
#include <physics/sxphysics.h>
#include <score/sxscore.h>
#include <network/sxnetwork.h>

#include "EntityFactory.h"
#include "EntityManager.h"

#include "proptable.h"

#include "TakeDamageInfo.h"

#pragma pointers_to_members(full_generality, virtual_inheritance)

#pragma warning(push)
#pragma warning(disable:4251)

//! Базовый объект, от которого наследуются все остальные классы объектов.
class SXGAME_EXPORT CBaseEntity
{
	DECLARE_CLASS_NOBASE(CBaseEntity);
	DECLARE_PROPTABLE();

	friend class CEntityManager;

public:
	//! Конструктор
	CBaseEntity(CEntityManager * pMgr);
	virtual ~CBaseEntity();

	//! Возвращает имя движкового класса объекта
	const char * getClassName();
	
	//! Возвращает имя объекта
	const char * getName();
	
	//! Возвращает баунд объекта
	virtual void getMinMax(float3 * min, float3 * max);
	
	//! Возвращает баунд сферу объекта
	virtual void getSphere(float3 * center, float * radius);

	//! Устанавливает мировую позицию объекта
	virtual void setPos(const float3 & pos);
	//! Устанавливает относительное смещение объекта
	virtual void setOffsetPos(const float3 & pos);
	//! Получает мировую позицию объекта
	float3 getPos();
	float3 getOffsetPos();

	//! Получает ID объекта в системе
	ID getId();

	//! Получает флаги объекта
	UINT getFlags();
	//! Устанавливает флаги объекта
	void setFlags(UINT f);

	//! Получает мировую матрицу трансформации для объекта
	SMMATRIX getWorldTM();

	//! Устанавливает вращение объекта
	virtual void setOrient(const SMQuaternion & q);
	//! Устанавливает относительное вращение объекта
	virtual void setOffsetOrient(const SMQuaternion & q);
	//! Возвращает вращение объекта
	SMQuaternion getOrient();

	//! Устанавливает свойство объекта
	virtual bool setKV(const char * name, const char * value);
	//! Получает свойство объекта
	virtual bool getKV(const char * name, char * out, int bufsize);

	//! Устанавливает родительский объект для объекта
	void setParent(CBaseEntity * pEnt, int attachment = -1);
	//! Возвращает родительский объект объекта
	CBaseEntity * getParent();

	//! Устанавливает владельца
	void setOwner(CBaseEntity * pEnt);
	//! Возвращает владельца объекта
	CBaseEntity * getOwner();

	//! Получает объект менеджера
	CEntityManager * getManager();

	//void updateDiscreteLinearVelocity(int step, float dt);
	//const float3_t & getDiscreteLinearVelocity() const;

	virtual void dispatchDamage(CTakeDamageInfo &takeDamageInfo);

	virtual void onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor);

	void broadcastMessage(const char * szInputName, inputdata_t inputData, float fRadius);
	void broadcastMessage(const char * szInputName, float fArg, float fRadius);
	void broadcastMessage(const char * szInputName, int iArg, float fRadius);
	void broadcastMessage(const char * szInputName, bool bArg, float fRadius);
	void broadcastMessage(const char * szInputName, const char *szArg, float fRadius);
	void broadcastMessage(const char * szInputName, const float3_t &f3Arg, float fRadius);
	void broadcastMessage(const char * szInputName, float fRadius);

	virtual void onUse(CBaseEntity *pUser);

	virtual bool isBloody()
	{
		return(false);
	}

	virtual bool getMainColor(float3_t *pOut)
	{
		return(false);
	}

	//! Обновляет действие флагов в режиме редактора уровня
	virtual void updateFlags(){}


	virtual float3 getEditorBoxSize();

private:
	void setClassName(const char * name);
	void setDefaults();

	const char * m_szClassName;

	CBaseEntity *getEntByName(const char *szName, CBaseEntity *pStartFrom);
	int countEntByName(const char *szName);

protected:
	virtual void _cleanup();
	virtual void _initEditorBoxes();
	virtual void _releaseEditorBoxes();

	CEntityManager * m_pMgr;

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
	CBaseEntity * m_pParent;
	//! Индекс кости родителя
	int m_iParentAttachment;

	//! Владелец
	CBaseEntity * m_pOwner;

	//! Вызывается на стадии синхронизации
	virtual void onSync();
	//! Вызывается при создании после установки всех свойств
	virtual void onPostLoad();

	//! Получает смещение для кости
	virtual float3 getAttachmentPos(int id);
	//! Получает вращение для кости
	virtual SMQuaternion getAttachmentRot(int id);

	/*! Устанавливает значение строкового свойства
	\note только для внутреннего использования
	*/
	void _setStrVal(const char ** to, const char * value);

	/*! Обновляет выходы
	\note только для внутреннего использования
	*/
	void updateOutputs();
	
	//! здоровье [0,+inf]
	float m_fHealth;

	void takeHealth(float fVal, CBaseEntity *pAttacker, CBaseEntity *pInflictor=NULL);

	bool m_bSynced;


	//! Для редактора
	//@{
	float3_t m_vEditorBoxSize = float3_t(0.16f, 0.16f, 0.16f);
	btCollisionShape * m_pEditorCollideShape = NULL;
	btRigidBody * m_pEditorRigidBody = NULL;
	//@}
};

#pragma warning(pop)

#endif

/*! @} cbaseentity*/
