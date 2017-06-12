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

class SXGAME_EXPORT SXbaseEntity
{
	DECLARE_CLASS_NOBASE(SXbaseEntity);
	DECLARE_PROPTABLE();

	friend class EntityManager;

private:
	void SetClassName(const char * name);
	void SetDefaults();

	const char * m_szClassName;

protected:
	EntityManager * m_pMgr;

	float3_t m_vPosition;
	float3_t m_vSpeed;
	SMQuaternion m_vOrientation;


	float3_t m_vOffsetPos;
	SMQuaternion m_vOffsetOrient;

	ID m_iId;

	UINT m_iFlags;

	const char * m_szName;

	SXbaseEntity * m_pParent;
	int m_iParentAttachment;

	SXbaseEntity * m_pOwner;

	virtual void OnSync();
	virtual void OnPostLoad();
	
	virtual float3 GetAttachmentPos(int id);
	virtual SMQuaternion GetAttachmentRot(int id);

	void _SetStrVal(const char ** to, const char * value);
public:
	SXbaseEntity(EntityManager * pMgr);
	virtual ~SXbaseEntity();

	const char * GetClassName();
	const char * GetName();

	virtual void SetPos(const float3 & pos);
	float3 GetPos();

	ID GetId();

	UINT GetFlags();
	void SetFlags(UINT f);

	SMMATRIX GetWorldTM();

	void SetOrient(const SMQuaternion & q);
	SMQuaternion GetOrient();

	virtual bool SetKV(const char * name, const char * value);
	virtual bool GetKV(const char * name, char * out, int bufsize);


	void SetParent(SXbaseEntity * pEnt, int attachment = -1);
	SXbaseEntity * GetParent();

	void SetOwner(SXbaseEntity * pEnt);
	SXbaseEntity * GetOwner();
};

#pragma warning(pop)

#endif
