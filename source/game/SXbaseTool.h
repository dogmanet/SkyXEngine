#ifndef _SXbaseTool_H_
#define _SXbaseTool_H_

#include "SXbaseItem.h"

class SXbaseTool: public SXbaseItem
{
	DECLARE_CLASS(SXbaseTool, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	virtual void PrimaryAction(BOOL st);
	virtual void SecondaryAction(BOOL st);
	virtual void Reload();

	//virtual void Deploy() = 0;

	void SetNextUse(float time);
	bool CanUse();

	void SetIsWorldModel(bool b);

protected:

	bool m_bInPrimaryAction;
	bool m_bInSecondaryAction;

	bool m_bWorldModel;

	bool m_bCanUse;

	float m_fZoomTime;

	void _AllowUse(float dt)
	{
		m_bCanUse = true;
	}

	float m_fReloadTime;
};

#endif
