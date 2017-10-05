#ifndef _SXbaseAnimating_H_
#define _SXbaseAnimating_H_

#include "SXbaseEntity.h"
#include <anim/sxanim.h>

class SXbaseAnimating: public SXbaseEntity
{
	DECLARE_CLASS(SXbaseAnimating, SXbaseEntity);
	DECLARE_PROPTABLE();
public:
	SXbaseAnimating(EntityManager * pMgr);
	~SXbaseAnimating();

	void GetMinMax(float3 * min, float3 * max);
	void GetSphere(float3 * center, float * radius);

	bool SetKV(const char * name, const char * value);

	void SetModel(const char * mdl);

	float3 GetAttachmentPos(int id);
	SMQuaternion GetAttachmentRot(int id);

	void OnSync();

	void PlayAnimation(const char * name, UINT iFadeTime = 0, UINT slot = 0);
	void PlayActivity(const char * name, UINT iFadeTime = 0, UINT slot = 0);


	bool PlayingAnimations(const char* name);
	void SetPos(const float3 & pos);
	void SetOrient(const SMQuaternion & q);

protected:
	IAnimPlayer * m_pAnimPlayer;
	const char * m_szModelFile;
	float m_fBaseScale;

	virtual void InitPhysics();
	virtual void CreatePhysBody();
	void ReleasePhysics();
	virtual void RemovePhysBody();

	btCollisionShape * m_pCollideShape;
	btRigidBody * m_pRigidBody;
};

#endif
