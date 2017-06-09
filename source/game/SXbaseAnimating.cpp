#include "SXbaseAnimating.h"


BEGIN_PROPTABLE(SXbaseAnimating)
// empty
	DEFINE_FIELD_STRING(m_szModelFile, 0, "model", "Model file", EDITOR_FILEFIELD)
		FILE_OPTION("Model file (*.dse)", "*.dse")
	EDITOR_FILE_END()

	DEFINE_FIELD_FLOAT(m_fBaseScale, 0, "scale", "Scale", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseAnimating, base_animating);

SXbaseAnimating::SXbaseAnimating(EntityManager * pMgr):
	BaseClass(pMgr),
	m_pAnimPlayer(NULL),
	m_fBaseScale(1.0f)
{
}

SXbaseAnimating::~SXbaseAnimating()
{
	mem_release(m_pAnimPlayer);
}

bool SXbaseAnimating::SetKV(const char * name, const char * value)
{
	if(!BaseClass::SetKV(name, value))
	{
		return(false);
	}
	if(!strcmp(name, "model"))
	{
		SetModel(value);
	}
	return(true);
}

void SXbaseAnimating::SetModel(const char * mdl)
{
	if(!m_pAnimPlayer)
	{
		m_pAnimPlayer = SXAnim_CreatePlayer(mdl);
	}
	else
	{
		m_pAnimPlayer->SetModel(mdl);
	}
}

float3 SXbaseAnimating::GetAttachmentPos(int id)
{
	float3 pos;
	if(!m_pAnimPlayer && id >= 0)
	{
		pos = m_pAnimPlayer->GetBoneTransformPos(id);
	}

	return(GetOrient() * pos + GetPos());
}

SMQuaternion SXbaseAnimating::GetAttachmentRot(int id)
{
	SMQuaternion rot;
	if(!m_pAnimPlayer && id >= 0)
	{
		rot = m_pAnimPlayer->GetBoneTransformRot(id);
	}

	return(GetOrient() * rot);
}

void SXbaseAnimating::OnSync()
{
	BaseClass::OnSync();
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->SetScale(m_fBaseScale);
		m_pAnimPlayer->SetPos(GetPos());
		m_pAnimPlayer->SetOrient(GetOrient());
	}
}

void SXbaseAnimating::PlayAnimation(const char * name, UINT iFadeTime, UINT slot)
{
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->Play(name, iFadeTime, slot);
	}
}
