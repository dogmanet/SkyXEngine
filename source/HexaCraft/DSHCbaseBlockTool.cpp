
#include "BaseBlockTool.h"

void CBaseBlockTool::advance(float t)
{
#if 0
	UINT dt = t - m_iCurTime;

	if(m_bInPrimaryAttack)
	{
		if(CanUse())
		{
			SetNextUse(t + 150);
			//PlayAnimation("Shoot", 0);
			m_pOwner->PlaceBlock(m_BlockType);
		}
	}

	if(m_bInSecondaryAttack)
	{
		if(CanUse())
		{
			SetNextUse(t + 150);
			//PlayAnimation("Shoot", 0);
			m_pOwner->RemoveBlock();
		}
	}

	BaseClass::Advance(t);
#endif
};