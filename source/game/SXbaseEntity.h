#ifndef _SXbaseEntity_H_
#define _SXbaseEntity_H_

#include <gdefines.h>
#include <common/SXMath.h>
#include <physics/sxphysics.h>

#include "EntityFactory.h"
#include "EntityManager.h"

#include "proptable.h"

#pragma pointers_to_members(full_generality, virtual_inheritance)

/*enum DS_ENT_FLAGS
{
	DS_ENT_RENDERABLE = 0x001,
	DS_ENT_NORMALTIMEFLOW = 0x002
};*/

class SXbaseEntity
{
	DECLARE_CLASS_NOBASE(SXbaseEntity);
	DECLARE_PROPTABLE();

	friend class EntityManager;

private:
	void SetClassName(const char * name);

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

	virtual void OnSync();
	virtual void OnPostLoad();
	
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

	bool SetKV(const char * name, const char * value);
	bool GetKV(const char * name, char * out, int bufsize);


	void SetParent(SXbaseEntity * pEnt);
	SXbaseEntity * GetParent();
};

#endif
