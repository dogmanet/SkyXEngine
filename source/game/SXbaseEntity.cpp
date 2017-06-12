#include "SXbaseEntity.h"

#include "EntityManager.h"

BEGIN_PROPTABLE_NOBASE(SXbaseEntity)
	DEFINE_FIELD_STRING(m_szName, 0, "name", "Name", EDITOR_TEXTFIELD)
	DEFINE_FIELD_VECTOR(m_vPosition, 0, "origin", "Origin", EDITOR_TEXTFIELD)
	DEFINE_FIELD_ANGLES(m_vOrientation, 0, "rotation", "Rotation", EDITOR_TEXTFIELD)
	DEFINE_FIELD_PARENT(m_pParent, 0, "parent", "Parent entity", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLAGS(m_iFlags, 0, "flags", "Flags", EDITOR_TEXTFIELD)
	DEFINE_FIELD_ENTITY(m_pOwner, PDFF_NOEXPORT | PDFF_NOEDIT, "owner", "", EDITOR_NONE)

	//DEFINE_FIELD_STRING(m_szName, 0, "some opt", "Option", EDITOR_COMBOBOX)
	//	COMBO_OPTION("Option 1", "value 1")
	//	COMBO_OPTION("Option 2", "value 2")
	//	COMBO_OPTION("Option 3", "value 3")
	//EDITOR_COMBO_END()
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseEntity, base_entity);

void SXbaseEntity::SetDefaults()
{
	proptable_t * pt = GetPropTable();
	const char * estr = GetEmptyString();
	while(pt)
	{
		for(int i = 0; i < pt->numFields; ++i)
		{
			if(pt->pData[i].type == PDF_STRING)
			{
				this->*((const char * ThisClass::*)pt->pData[i].pField) = estr;
			}
		}
		pt = pt->pBaseProptable;
	}
}

SXbaseEntity::SXbaseEntity(EntityManager * pWorld):
	m_iId(0),
	m_iFlags(0),
	m_pMgr(pWorld),
	m_szClassName(NULL),
	m_szName(NULL),
	m_pParent(NULL),
	m_iParentAttachment(-1),
	m_pOwner(NULL)
{
	m_iId = pWorld->Register(this);
}

/*void SXbaseEntity::SetDefaults()
{
	proptable_t * pt = GetPropTable();
	const char * estr = GetEmptyString();
	const char * defstr = NULL;
	while(pt)
	{
		for(int i = 0; i < pt->numFields; ++i)
		{
			if(pt->pData[i].type == PDF_STRING)
			{
				this->*((const char * ThisClass::*)pt->pData->pField) = estr;
			}
		}
		pt = pt->pBaseProptable;
	}
}*/

SXbaseEntity::~SXbaseEntity()
{
	m_pMgr->Unregister(m_iId);

	proptable_t * pt = GetPropTable();
	const char * estr = GetEmptyString();
	while(pt)
	{
		for(int i = 0; i < pt->numFields; ++i)
		{
			if(pt->pData[i].type == PDF_STRING)
			{
				const char * str = this->*((const char * ThisClass::*)pt->pData->pField);
				if(str && str != estr)
				{
					delete[] str;
				}
			}
		}
		pt = pt->pBaseProptable;
	}
}

void SXbaseEntity::SetClassName(const char * name)
{
	m_szClassName = name;
}

const char * SXbaseEntity::GetName()
{
	return(m_szName);
}

const char * SXbaseEntity::GetClassName()
{
	return(m_szClassName);
}

void SXbaseEntity::SetPos(const float3 & pos)
{
	m_vPosition = pos;
}

float3 SXbaseEntity::GetPos()
{
	return(m_vPosition);
}

void SXbaseEntity::SetOrient(const SMQuaternion & q)
{
	m_vOrientation = q;
}

SMQuaternion SXbaseEntity::GetOrient()
{
	return(m_vOrientation);
}


ID SXbaseEntity::GetId()
{
	return(m_iId);
}

UINT SXbaseEntity::GetFlags()
{
	return(m_iFlags);
}

void SXbaseEntity::SetFlags(UINT f)
{
	m_iFlags = f;
}

SMMATRIX SXbaseEntity::GetWorldTM()
{
	return(m_vOrientation.GetMatrix() * SMMatrixTranslation(m_vPosition));
}

bool SXbaseEntity::SetKV(const char * name, const char * value)
{
	propdata_t * field = GetField(name);
	if(!field)
	{
		return(false);
	}
	float3_t f3;
	SMQuaternion q;
	int d;
	float f;
	SXbaseEntity * pEnt;
	switch(field->type)
	{
	case PDF_INT:
		if(1 == sscanf(value, "%d", &d))
		{
			this->*((int ThisClass::*)field->pField) = d;
			return(true);
		}
		return(false);
	case PDF_FLOAT:
		if(1 == sscanf(value, "%f", &f))
		{
			this->*((float ThisClass::*)field->pField) = f;
			return(true);
		}
		return(false);
	case PDF_VECTOR:
		if(3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
		{
			this->*((float3_t ThisClass::*)field->pField) = f3;
			return(true);
		}
		return(false);
	case PDF_BOOL:
		if(1 == sscanf(value, "%d", &d))
		{
			this->*((bool ThisClass::*)field->pField) = d != 0;
			return(true);
		}
		return(false);
	case PDF_STRING:
		_SetStrVal(&(this->*((const char * ThisClass::*)field->pField)), value);
		break;
	case PDF_ANGLES:
		if(4 == sscanf(value, "%f %f %f %f", &q.x, &q.y, &q.z, &q.w))
		{
			this->*((SMQuaternion ThisClass::*)field->pField) = q;
			return(true);
		}
		if(3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
		{
			SMQuaternion q;
			q = SMQuaternion(SMToRadian(f3.x), 'x') * SMQuaternion(SMToRadian(f3.y), 'y') * SMQuaternion(SMToRadian(f3.z), 'z');
			this->*((SMQuaternion ThisClass::*)field->pField) = q;
			return(true);
		}
		return(false);
	case PDF_ENTITY:
	case PDF_PARENT:
		pEnt = m_pMgr->FindEntityByName(value);
		if(pEnt || !value[0])
		{
			if(field->type == PDF_PARENT)
			{
				SetParent(pEnt);
			}
			else
			{
				this->*((SXbaseEntity * ThisClass::*)field->pField) = pEnt;
			}
			return(true);
		}
		return(false);
	case PDF_FLAGS:
		if(1 == sscanf(value, "%d", &d))
		{
			UINT * f = &(this->*((UINT ThisClass::*)field->pField));
			*f &= 0xFFFF;
			*f |= d & 0xFFFF0000;
			return(true);
		}
		return(false);
	default:
		return(false);
	}
	return(true);
}
bool SXbaseEntity::GetKV(const char * name, char * out, int bufsize)
{
	propdata_t * field = GetField(name);
	if(!field)
	{
		return(false);
	}
	SMQuaternion q;
	float3_t f3;
	SXbaseEntity * pEnt;
	switch(field->type)
	{
	case PDF_INT:
		sprintf_s(out, bufsize, "%d", this->*((int ThisClass::*)field->pField));
		break;
	case PDF_FLAGS:
		sprintf_s(out, bufsize, "%d", this->*((int ThisClass::*)field->pField) & 0xFFFF0000);
		break;
	case PDF_FLOAT:
		sprintf_s(out, bufsize, "%f", this->*((float ThisClass::*)field->pField));
		break;
	case PDF_VECTOR:
		sprintf_s(out, bufsize, "%f %f %f", (this->*((float3_t ThisClass::*)field->pField)).x, (this->*((float3_t SXbaseEntity::*)field->pField)).y, (this->*((float3_t SXbaseEntity::*)field->pField)).z);
		break;
	case PDF_BOOL:
		sprintf_s(out, bufsize, "%d", this->*((bool ThisClass::*)field->pField) ? 1 : 0);
		break;
	case PDF_STRING:
		sprintf_s(out, bufsize, "%s", this->*((const char * ThisClass::*)field->pField));
		break;
	case PDF_ANGLES:
		q = this->*((SMQuaternion ThisClass::*)field->pField);
		//f3 = SMMatrixToEuler(q.GetMatrix());
		sprintf_s(out, bufsize, "%f %f %f %f", q.x, q.y, q.z, q.w);
		break;
	case PDF_ENTITY:
	case PDF_PARENT:
		pEnt = this->*((SXbaseEntity * ThisClass::*)field->pField);
		if(!pEnt)
		{
			sprintf_s(out, bufsize, "");
		}
		else
		{
			sprintf_s(out, bufsize, "%s", pEnt->GetName());
		}
		break;
	default:
		return(false);
	}
	return(true);
}

void SXbaseEntity::SetParent(SXbaseEntity * pEnt, int attachment)
{
	m_pParent = pEnt;
	if(pEnt)
	{
		if(attachment >= 0)
		{
			m_vOffsetPos = (float3)(m_vPosition - m_pParent->GetAttachmentPos(attachment));
			m_vOffsetOrient = m_vOrientation * m_pParent->GetAttachmentRot(attachment).Conjugate();
		}
		else
		{
			m_vOffsetPos = m_pParent->GetOrient().Conjugate() * (float3)(m_vPosition - m_pParent->GetPos());
			m_vOffsetOrient = m_vOrientation * m_pParent->GetOrient().Conjugate();
		}
		m_iParentAttachment = attachment;
	}
}

SXbaseEntity * SXbaseEntity::GetParent()
{
	return(m_pParent);
}

void SXbaseEntity::OnSync()
{
	if(m_pParent)
	{
		m_pParent->OnSync();
		if(m_iParentAttachment >= 0)
		{
			m_vPosition = (float3)(m_pParent->GetAttachmentPos(m_iParentAttachment) + m_vOffsetPos);
			m_vOrientation = m_pParent->GetAttachmentRot(m_iParentAttachment) * m_vOffsetOrient;
		}
		else
		{
			m_vPosition = (float3)(m_pParent->GetPos() + m_pParent->GetOrient() * m_vOffsetPos);
			m_vOrientation = m_vOffsetOrient * m_pParent->GetOrient();
		}
		//if(m_pPhysObj)
		//{
		//	m_pPhysObj->SetPos(m_vPosition);
		//	m_pPhysObj->SetOrient(m_vOrientation);
		//}
	}
	//else if(m_pPhysObj)
	//{
	//	m_vPosition = m_pPhysObj->GetPos();
	//	m_vOrientation = m_pPhysObj->GetOrient();
	//}
}

void SXbaseEntity::OnPostLoad()
{
}

float3 SXbaseEntity::GetAttachmentPos(int id)
{
	return(float3());
}

SMQuaternion SXbaseEntity::GetAttachmentRot(int id)
{
	return(SMQuaternion());
}

void SXbaseEntity::SetOwner(SXbaseEntity * pEnt)
{
	m_pOwner = pEnt;
}

SXbaseEntity * SXbaseEntity::GetOwner()
{
	return(m_pOwner);
}

void SXbaseEntity::_SetStrVal(const char ** to, const char * value)
{
	char * str = (char*)*to;
	if(str && !fstrcmp(str, value))
	{
		return;
	}
	if(!value[0])
	{
		if(str && str != GetEmptyString())
		{
			delete[] str;
		}
		*to = GetEmptyString();
	}
	else
	{
		size_t len = strlen(value);
		if(!str || strlen(str) < len)
		{
			if(str && str != GetEmptyString())
			{
				delete[] str;
			}
			str = new char[len + 1];
		}
		memcpy(str, value, sizeof(char)* (len + 1));
		*to = str;
	}
}
