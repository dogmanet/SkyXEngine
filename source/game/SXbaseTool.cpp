#include "SXbaseTool.h"


BEGIN_PROPTABLE(SXbaseTool)
	DEFINE_FIELD_FLOAT(m_fReloadTime, PDFF_NOEDIT | PDFF_NOEXPORT, "reload_time", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseTool, base_tool);

SXbaseTool::SXbaseTool(EntityManager * pMgr):
	BaseClass(pMgr),
	m_bInPrimaryAction(false),
	m_bInSecondaryAction(false),
	m_bWorldModel(false),
	m_bCanUse(true),
	m_fZoomTime(0.0f),
	m_fReloadTime(0.0f)
{
	m_bInvStackable = false;
}

void SXbaseTool::SetNextUse(float time)
{
	m_bCanUse = false;
	SET_TIMEOUT(_AllowUse, time);
}
bool SXbaseTool::CanUse()
{
	return(m_bCanUse);
}

void SXbaseTool::PrimaryAction(BOOL st)
{
	m_bInPrimaryAction = st != FALSE;
}

void SXbaseTool::SecondaryAction(BOOL st)
{
	m_bInSecondaryAction = st != FALSE;
}

void SXbaseTool::SetIsWorldModel(bool b)
{
	m_bWorldModel = b;
}

void SXbaseTool::Reload()
{
	if(CanUse())
	{
		SetNextUse(m_fReloadTime);
		PlayAnimation("reload");
	}
}
