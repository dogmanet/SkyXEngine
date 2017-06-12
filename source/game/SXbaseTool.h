#ifndef _SXbaseTool_H_
#define _SXbaseTool_H_

#include "SXbaseItem.h"

enum
{
	DSM_NONE,
	DSM_POS_X,
	DSM_POS_Y,
	DSM_POS_Z,
	DSM_ROT_X,
	DSM_ROT_Y,
	DSM_ROT_Z,
	DSM_PRINT
};

class SXbaseTool: public SXbaseItem
{
	DECLARE_CLASS(SXbaseTool, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~SXbaseTool();

	virtual void PrimaryAction(BOOL st);
	virtual void SecondaryAction(BOOL st);
	virtual void Reload();

	//virtual void Deploy() = 0;

	void SetNextUse(float time);
	bool CanUse();

	void SetIsWorldModel(bool b);

	void AttachHands();

	void DbgMove(int dir, float delta);

	void OnSync();

	void SetParent(SXbaseEntity * pEnt, int attachment = -1);

protected:

	bool m_bInPrimaryAction;
	bool m_bInSecondaryAction;

	bool m_bWorldModel;

	bool m_bCanUse;

	float m_fZoomTime;

	float m_fZoomProgress;

	void _AllowUse(float dt)
	{
		m_bCanUse = true;
	}

	void _Update(float dt);
	void _Rezoom();

	float m_fReloadTime;

	float3_t m_vSlotPos;
	float3_t m_vSlotPosAim;
	SMQuaternion m_qSlotRot;
	SMQuaternion m_qSlotRotAim;

	ID m_iIvalUpdate;

	ID m_iMuzzleFlash;
};

#endif
