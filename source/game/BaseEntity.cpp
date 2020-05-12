
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <stdio.h>

#include "BaseEntity.h"

#include "EntityManager.h"

#include "GameData.h"

/*! \skydocent base_entity
Базовый объект. От него наследуются все остальные объекты
*/

BEGIN_PROPTABLE_NOBASE(CBaseEntity)
	//! Имя объекта
	DEFINE_FIELD_STRING(m_szName, 0, "name", "Name", EDITOR_TEXTFIELD)
	//! Позиция в мире
	DEFINE_FIELD_VECTORFN(m_vPosition, 0, "origin", "Origin", setPos, EDITOR_TEXTFIELD)
	//! Ориентация в мире, углы эйлера или кватернион
	DEFINE_FIELD_ANGLES(m_vOrientation, 0, "rotation", "Rotation", EDITOR_TEXTFIELD)
	//! Родительский объект в иерархии движения
	DEFINE_FIELD_PARENT(m_pParent, 0, "parent", "Parent entity", EDITOR_TEXTFIELD)
	//! Флаги объекта
	DEFINE_FIELD_FLAGS(m_iFlags, 0, "flags", "Flags", EDITOR_FLAGS)
	//! Объект-владелец
	DEFINE_FIELD_ENTITY(m_pOwner, PDFF_NOEXPORT | PDFF_NOEDIT, "owner", "", EDITOR_NONE)
	//! Здоровье
	DEFINE_FIELD_FLOAT(m_fHealth, PDFF_NOEXPORT | PDFF_NOEDIT, "health", "", EDITOR_NONE)

	//DEFINE_FIELD_STRING(m_szName, 0, "some opt", "Option", EDITOR_COMBOBOX)
	//	COMBO_OPTION("Option 1", "value 1")
	//	COMBO_OPTION("Option 2", "value 2")
	//	COMBO_OPTION("Option 3", "value 3")
	//EDITOR_COMBO_END()
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseEntity, base_entity);

void CBaseEntity::setDefaults()
{
	proptable_t * pt = getPropTable();
	const char * estr = GetEmptyString();
	while(pt)
	{
		for(int i = 0; i < pt->numFields; ++i)
		{
			if(pt->pData[i].type == PDF_STRING && !(pt->pData[i].flags & PDFF_INPUT))
			{
				this->*((const char * ThisClass::*)pt->pData[i].pField) = estr;
			}
		}
		pt = pt->pBaseProptable;
	}
}

CBaseEntity::CBaseEntity(CEntityManager * pWorld):
	m_iId(0),
	m_iFlags(0),
	m_pMgr(pWorld),
	m_szClassName(NULL),
	m_szName(NULL),
	m_pParent(NULL),
	m_iParentAttachment(-1),
	m_pOwner(NULL),
	m_fHealth(100.0f)/*,
	m_vDiscreteLinearVelocity(float3_t(0.0f, 0.0f, 0.0f))*/
	, m_bSynced(false)
{
	m_iId = pWorld->reg(this);
	m_pLightSystem = GameData::m_pLightSystem;
}

/*void CBaseEntity::setDefaults()
{
	proptable_t * pt = getPropTable();
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

CBaseEntity::~CBaseEntity()
{
	_releaseEditorBoxes();

	m_pMgr->unreg(m_iId);

	proptable_t * pt = getPropTable();
	const char * estr = GetEmptyString();
	while(pt)
	{
		for(int i = 0; i < pt->numFields; ++i)
		{
			if(pt->pData[i].type == PDF_STRING && !(pt->pData[i].flags & PDFF_INPUT))
			{
				const char * str = this->*((const char * ThisClass::*)pt->pData[i].pField);
				if(str && str != estr)
				{
					delete[] str;
				}
			}
		}
		pt = pt->pBaseProptable;
	}
}

void CBaseEntity::setClassName(const char * name)
{
	m_szClassName = name;
}

const char * CBaseEntity::getName()
{
	return(m_szName);
}

const char * CBaseEntity::getClassName()
{
	return(m_szClassName);
}

void CBaseEntity::getMinMax(float3 * min, float3 * max)
{
	if (min)
		min->x = min->y = min->z = 0;

	if (max)
		max->x = max->y = max->z = 0;
}

void CBaseEntity::getSphere(float3 * center, float * radius)
{
	if (center)
		center->x = center->y = center->z = 0;

	if (radius)
		radius = 0;
}

void CBaseEntity::setPos(const float3 & pos)
{
	m_vPosition = pos;

	if(m_pEditorRigidBody)
	{
		m_pEditorRigidBody->getWorldTransform().setOrigin(F3_BTVEC(m_vPosition));
		SPhysics_GetDynWorld()->updateSingleAabb(m_pEditorRigidBody);
	}
}

float3 CBaseEntity::getPos()
{
	return(m_vPosition);
}

void CBaseEntity::setOrient(const SMQuaternion & q)
{
	m_vOrientation = q;
}
void CBaseEntity::setOffsetOrient(const SMQuaternion & q)
{
	m_vOffsetOrient = q;
}

void CBaseEntity::setOffsetPos(const float3 & pos)
{
	m_vOffsetPos = pos;
}

float3 CBaseEntity::getOffsetPos()
{
	return(m_vOffsetPos);
}

SMQuaternion CBaseEntity::getOrient()
{
	return(m_vOrientation);
}


ID CBaseEntity::getId()
{
	return(m_iId);
}

UINT CBaseEntity::getFlags()
{
	return(m_iFlags);
}

void CBaseEntity::setFlags(UINT f)
{
	m_iFlags = f;
}

SMMATRIX CBaseEntity::getWorldTM()
{
	return(m_vOrientation.GetMatrix() * SMMatrixTranslation(m_vPosition));
}

bool CBaseEntity::setKV(const char * name, const char * value)
{
	propdata_t * field = getField(name);
	if(!field || (field->flags & PDFF_INPUT))
	{
		return(false);
	}
	float3_t f3;
	float4_t f4;
	SMQuaternion q;
	int d;
	float f;
	CBaseEntity * pEnt;
	switch(field->type)
	{
	case PDF_INT:
		if(1 == sscanf(value, "%d", &d))
		{
			if(field->fnSet.i)
			{
				(this->*(field->fnSet.i))(d);
			}
			else
			{
				this->*((int ThisClass::*)field->pField) = d;
			}
			return(true);
		}
		return(false);
	case PDF_FLOAT:
		if(1 == sscanf(value, "%f", &f))
		{
			if(field->fnSet.f)
			{
				(this->*(field->fnSet.f))(f);
			}
			else
			{
				this->*((float ThisClass::*)field->pField) = f;
			}
			return(true);
		}
		return(false);
	case PDF_VECTOR:
		if(3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
		{
			if(field->fnSet.v3)
			{
				(this->*(field->fnSet.v3))(f3);
			}
			else
			{
				this->*((float3_t ThisClass::*)field->pField) = f3;
			}
			return(true);
		}
		return(false);
	case PDF_VECTOR4:
		{
			int iPrm = sscanf(value, "%f %f %f %f", &f4.x, &f4.y, &f4.z, &f4.w);
			if(iPrm > 2)
			{
				if(iPrm == 3)
				{
					f4.w = 1.0f;
				}
				if(field->fnSet.v4)
				{
					(this->*(field->fnSet.v4))(f4);
				}
				else
				{
					this->*((float4_t ThisClass::*)field->pField) = f4;
				}
				return(true);
			}
		}
		return(false);
	case PDF_BOOL:
		if(1 == sscanf(value, "%d", &d))
		{
			if(field->fnSet.b)
			{
				(this->*(field->fnSet.b))(d != 0);
			}
			else
			{
				this->*((bool ThisClass::*)field->pField) = d != 0;
			}
			return(true);
		}
		return(false);
	case PDF_STRING:
		if(field->fnSet.sz)
		{
			(this->*(field->fnSet.sz))(value);
		}
		else
		{
			_setStrVal(&(this->*((const char * ThisClass::*)field->pField)), value);
		}
		break;
	case PDF_ANGLES:
		if(4 == sscanf(value, "%f %f %f %f", &q.x, &q.y, &q.z, &q.w))
		{
			if(field->fnSet.q)
			{
				(this->*(field->fnSet.q))(q);
			}
			else
			{
				this->*((SMQuaternion ThisClass::*)field->pField) = q;
			}
			return(true);
		}
		if(3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
		{
			SMQuaternion q;
			q = SMQuaternion(SMToRadian(f3.x), 'x') * SMQuaternion(SMToRadian(f3.y), 'y') * SMQuaternion(SMToRadian(f3.z), 'z');
			if(field->fnSet.q)
			{
				(this->*(field->fnSet.q))(q);
			}
			else
			{
				this->*((SMQuaternion ThisClass::*)field->pField) = q;
			}
			return(true);
		}
		return(false);
	case PDF_ENTITY:
	case PDF_PARENT:
		pEnt = m_pMgr->findEntityByName(value);
		if(pEnt || !value[0])
		{
			if(field->type == PDF_PARENT)
			{
				setParent(pEnt);
			}
			else
			{
				if(field->fnSet.e)
				{
					(this->*(field->fnSet.e))(pEnt);
				}
				else
				{
					this->*((CBaseEntity * ThisClass::*)field->pField) = pEnt;
				}
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
			updateFlags();
			return(true);
		}
		return(false);
	case PDF_OUTPUT:
		{
			size_t len = sizeof(char)* (1 + strlen(value));
			char * str = new char[len];
			memcpy(str, value, len);
			int iConns = parse_str(str, NULL, 0, ',');
			char ** parts = (char**)alloca(sizeof(char*)*iConns);
			iConns = parse_str(str, parts, iConns, ',');

			output_t *pOutput = &(this->*((output_t ThisClass::*)field->pField));
			mem_delete_a(pOutput->pOutputs);
			{
				char *pTmpData = (char*)pOutput->pData;
				mem_delete_a(pTmpData);
			}
			pOutput->pData = str;
			pOutput->pOutputs = new named_output_t[iConns];
			pOutput->bDirty = true;

			int curr = 0;
			for(int i = 0; i < iConns; ++i)
			{
				char * fields[4];
				int iParts = parse_str(parts[i], fields, 4, ':');
				char * param = NULL;
				if(iParts != 4 && iParts != 3)
				{
					printf(COLOR_LRED "Unable to parse output '%s' ent %s\n" COLOR_RESET, name, m_szName);
					continue;
				}
				if(iParts == 4)
				{
					param = fields[3];
				}
				float fDelayFrom;
				float fDelayTo;
				switch(sscanf(fields[2], "%f-%f", &fDelayFrom, &fDelayTo))
				{
				case 1:
					fDelayTo = fDelayFrom;
					break;
				case 2:
					break;
				default:
					printf(COLOR_LRED "Unable to parse output delay '%s' ent %s\n" COLOR_RESET, name, m_szName);
					continue;
				}
				pOutput->pOutputs[curr].fDelay = fDelayFrom;
				pOutput->pOutputs[curr].fDelayTo = fDelayTo;
				if(fDelayFrom < fDelayTo)
				{
					pOutput->pOutputs[curr].useRandomDelay = true;
				}
				if(fDelayFrom > fDelayTo)
				{
					printf(COLOR_LRED "Unable to parse output delay '%s' ent %s. min delay cannot be more than max delay\n" COLOR_RESET, name, m_szName);
					continue;
				}

				pOutput->pOutputs[curr].szTargetName = fields[0];
				pOutput->pOutputs[curr].szTargetInput = fields[1];
				pOutput->pOutputs[curr].szTargetData = param;
				++curr;
			}
			pOutput->iOutCount = curr;
		}
		// target_name:input:delay:parameter\n<repeat>
		return(false);
	default:
		return(false);
	}
	return(true);
}
bool CBaseEntity::getKV(const char * name, char * out, int bufsize)
{
	propdata_t * field = getField(name);
	if(!field)
	{
		return(false);
	}
	SMQuaternion q;
	float3_t f3;
	CBaseEntity * pEnt;
	switch(field->type)
	{
	case PDF_INT:
		sprintf_s(out, bufsize, "%d", this->*((int ThisClass::*)field->pField));
		break;
	case PDF_FLAGS:
		sprintf_s(out, bufsize, "%u", this->*((int ThisClass::*)field->pField) & 0xFFFF0000);
		break;
	case PDF_FLOAT:
		sprintf_s(out, bufsize, "%f", this->*((float ThisClass::*)field->pField));
		break;
	case PDF_VECTOR:
		sprintf_s(out, bufsize, "%f %f %f", (this->*((float3_t ThisClass::*)field->pField)).x, (this->*((float3_t CBaseEntity::*)field->pField)).y, (this->*((float3_t CBaseEntity::*)field->pField)).z);
		break;
	case PDF_VECTOR4:
		sprintf_s(out, bufsize, "%f %f %f %f", (this->*((float4_t ThisClass::*)field->pField)).x, (this->*((float4_t CBaseEntity::*)field->pField)).y, (this->*((float4_t CBaseEntity::*)field->pField)).z, (this->*((float4_t CBaseEntity::*)field->pField)).w);
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
		pEnt = this->*((CBaseEntity * ThisClass::*)field->pField);
		if(!pEnt)
		{
			sprintf_s(out, bufsize, "");
		}
		else
		{
			sprintf_s(out, bufsize, "%s", pEnt->getName());
		}
		break;
	case PDF_OUTPUT:
		{
			output_t *pOutput = &(this->*((output_t ThisClass::*)field->pField));
			int iWritten = 0;
			char * szOutBuf = out;
			if(pOutput->iOutCount == 0)
			{
				*out = 0;
			}
			for(int i = 0; i < pOutput->iOutCount; ++i)
			{
				if(i > 0)
				{
					*szOutBuf = ',';
					++szOutBuf;
				}
				int c;
				if(pOutput->pOutputs[i].useRandomDelay)
				{
					c = _snprintf(szOutBuf, bufsize - iWritten, "%s:%s:%f-%f:%s", 
						pOutput->pOutputs[i].szTargetName, 
						pOutput->pOutputs[i].szTargetInput, 
						pOutput->pOutputs[i].fDelay,
						pOutput->pOutputs[i].fDelayTo,
						pOutput->pOutputs[i].szTargetData ? pOutput->pOutputs[i].szTargetData : ""
						);
				}
				else
				{
					c = _snprintf(szOutBuf, bufsize - iWritten, "%s:%s:%f:%s", 
						pOutput->pOutputs[i].szTargetName, 
						pOutput->pOutputs[i].szTargetInput, 
						pOutput->pOutputs[i].fDelay, 
						pOutput->pOutputs[i].szTargetData ? pOutput->pOutputs[i].szTargetData : ""
						);
				}
				iWritten += c + 1;
				szOutBuf += c;
				//iBufSize += 1 + _snprintf(NULL, 0, "%s:%s:%f:%s", pOutput->pOutputs[i].szTargetName, pOutput->pOutputs[i].szTargetInput, pOutput->pOutputs[i].fDelay, pOutput->pOutputs[i].szTargetData ? pOutput->pOutputs[i].szTargetData : "");
			}
		}
		break;
	default:
		return(false);
	}
	return(true);
}

void CBaseEntity::setParent(CBaseEntity * pEnt, int attachment)
{
	m_pParent = pEnt;
	if(pEnt)
	{
		if(attachment >= 0)
		{
			m_vOffsetPos = (float3)(m_vPosition - m_pParent->getAttachmentPos(attachment));
			m_vOffsetOrient = m_vOrientation * m_pParent->getAttachmentRot(attachment).Conjugate();
		}
		else
		{
			m_vOffsetPos = m_pParent->getOrient().Conjugate() * (float3)(m_vPosition - m_pParent->getPos());
			m_vOffsetOrient = m_vOrientation * m_pParent->getOrient().Conjugate();
		}
		m_iParentAttachment = attachment;
	}
}

CBaseEntity * CBaseEntity::getParent()
{
	return(m_pParent);
}

/*void CBaseEntity::updateDiscreteLinearVelocity(int step, float dt)
{
	if(step == 0)
	{
		m_vDiscreteLinearVelocity = (float3)((m_vPosition - m_vOldPosition) / dt);
		m_vOldPosition = m_vPosition;
	}
	return;
	if(m_pParent || 1)
	{
		if(step) // post sync
		{
			m_vDiscreteLinearVelocity = (float3)((m_vPosition - m_vOldPosition) / dt);
		}
		else // pre sync
		{
			m_vOldPosition = m_vPosition;
		}
	}
	else
	{
		if(step) // post sync
		{
			m_vOldPosition = m_vPosition;
		}
		else // pre sync
		{
			m_vDiscreteLinearVelocity = (float3)((m_vPosition - m_vOldPosition) / dt);
		}
	}
}*/

void CBaseEntity::onSync()
{
	if(m_pParent)
	{
		if(!m_pParent->m_bSynced)
		{
			m_pParent->onSync();
		}
		if(m_iParentAttachment >= 0)
		{
			m_vPosition = (float3)(m_pParent->getAttachmentPos(m_iParentAttachment) + m_vOffsetPos);
			m_vOrientation = m_pParent->getAttachmentRot(m_iParentAttachment) * m_vOffsetOrient;
		}
		else
		{
			m_vPosition = (float3)(m_pParent->getPos() + m_pParent->getOrient() * m_vOffsetPos);
			m_vOrientation = m_vOffsetOrient * m_pParent->getOrient();
		}
		//if(m_pPhysObj)
		//{
		//	m_pPhysObj->setPos(m_vPosition);
		//	m_pPhysObj->setOrient(m_vOrientation);
		//}
	}
	//else if(m_pPhysObj)
	//{
	//	m_vPosition = m_pPhysObj->GetPos();
	//	m_vOrientation = m_pPhysObj->getOrient();
	//}

	m_bSynced = true;
}

void CBaseEntity::onPostLoad()
{
	updateOutputs();
	updateFlags();
}

float3 CBaseEntity::getAttachmentPos(int id)
{
	return(float3());
}

SMQuaternion CBaseEntity::getAttachmentRot(int id)
{
	return(SMQuaternion());
}

void CBaseEntity::setOwner(CBaseEntity * pEnt)
{
	m_pOwner = pEnt;
}

CBaseEntity * CBaseEntity::getOwner()
{
	return(m_pOwner);
}

void CBaseEntity::_setStrVal(const char ** to, const char * value)
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

CEntityManager * CBaseEntity::getManager()
{
	return(m_pMgr);
}

/*const float3_t & CBaseEntity::getDiscreteLinearVelocity() const
{
	return(m_vDiscreteLinearVelocity);
}*/

CBaseEntity *CBaseEntity::getEntByName(const char *szName, CBaseEntity *pStartFrom)
{
	if(!strcmp(szName, "!this"))
	{
		return(pStartFrom ? NULL : this);
	}
	if(!strcmp(szName, "!parent"))
	{
		return(pStartFrom ? NULL : getParent());
	}

	return(m_pMgr->findEntityByName(szName, pStartFrom));
}

int CBaseEntity::countEntByName(const char *szName)
{
	if(!strcmp(szName, "!this"))
	{
		return(1);
	}
	if(!strcmp(szName, "!parent"))
	{
		return(getParent() ? 1 : 0);
	}

	return(m_pMgr->countEntityByName(szName));
}

void CBaseEntity::updateOutputs()
{
	proptable_t * pt = getPropTable();
	while(pt)
	{
		for(int i = 0; i < pt->numFields; ++i)
		{
			if(pt->pData[i].type == PDF_OUTPUT)
			{
				output_t *pOutput = &(this->*((output_t ThisClass::*)pt->pData[i].pField));

				for(int j = 0, jl = pOutput->iOutCount; j < jl; ++j)
				{
					named_output_t * pOut = &pOutput->pOutputs[j];
					mem_delete_a(pOut->pOutputs);

					pOut->iOutCount = countEntByName(pOut->szTargetName);
					if(!pOut->iOutCount)
					{
						printf(COLOR_CYAN "Broken output target '%s' source '%s'.'%s'\n" COLOR_RESET, pOut->szTargetName, getClassName(), m_szName);
						continue;
					}
					pOut->pOutputs = new input_t[pOut->iOutCount];
					memset(pOut->pOutputs, 0, sizeof(input_t) * pOut->iOutCount);


					CBaseEntity * pEnt = NULL;
					int c = 0;
					while((pEnt = getEntByName(pOut->szTargetName, pEnt)))
					{
						propdata_t * pField = pEnt->getField(pOut->szTargetInput);
						if(!pField || !(pField->flags & PDFF_INPUT))
						{
							printf(COLOR_CYAN "Class '%s' has no input '%s', obj '%s'\n" COLOR_RESET, pEnt->getClassName(), pOut->szTargetInput, pOut->szTargetName);
							--pOut->iOutCount;
							continue;
						}

						pOut->pOutputs[c].fnInput = pField->fnInput;
						pOut->pOutputs[c].pTarget = pEnt;
						pOut->pOutputs[c].data.type = pField->type;
						{
							float3_t f3;
							float4_t f4;
							SMQuaternion q;
							int d;
							float f;
							const char * value = pOut->szTargetData;
							bool bParsed = false;
							switch(pField->type)
							{
							case PDF_NONE:
								bParsed = true;
								break;
							case PDF_INT:
								if(1 == sscanf(value, "%d", &d))
								{
									pOut->pOutputs[c].data.parameter.i = d;
									bParsed = true;
								}
								break;
							case PDF_FLOAT:
								if(1 == sscanf(value, "%f", &f))
								{
									pOut->pOutputs[c].data.parameter.f = f;
									bParsed = true;
								}
								break;
							case PDF_VECTOR:
								if(3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
								{
									pOut->pOutputs[c].data.v3Parameter = f3;
									bParsed = true;
								}
								break;
							case PDF_VECTOR4:
								{
									int iPrm = sscanf(value, "%f %f %f %f", &f4.x, &f4.y, &f4.z, &f4.w);
									if(iPrm > 2)
									{
										if(iPrm == 3)
										{
											f4.w = 1.0f;
										}
										pOut->pOutputs[c].data.v4Parameter = f4;
										bParsed = true;
									}
								}
								break;
							case PDF_BOOL:
								if(1 == sscanf(value, "%d", &d))
								{
									pOut->pOutputs[c].data.parameter.b = d != 0;
									bParsed = true;
								}
								break;
							case PDF_STRING:
								_setStrVal(&pOut->pOutputs[c].data.parameter.str, value);
								bParsed = true;
								break;
							}

							if(!bParsed)
							{
								printf(COLOR_CYAN "Cannot parse input parameter '%s', class '%s', input '%s', obj '%s'\n" COLOR_RESET, value, pEnt->getClassName(), pOut->szTargetInput, pOut->szTargetName);
								--pOut->iOutCount;
								continue;
							}
						}

						++c;
					}
				}
			}
		}
		pt = pt->pBaseProptable;
	}
}

void CBaseEntity::dispatchDamage(CTakeDamageInfo &takeDamageInfo)
{
	float fHealth = takeDamageInfo.m_fDamage * 0.1f;
	if(takeDamageInfo.m_pInflictor)
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "%s damaged (" COLOR_LRED "%.2f" COLOR_RESET ") by " COLOR_YELLOW "%s\n", getClassName(), fHealth, takeDamageInfo.m_pInflictor->getClassName());
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "%s damaged (" COLOR_LRED "%.2f" COLOR_RESET ")\n", getClassName(), fHealth);
	}
	takeHealth(fHealth, takeDamageInfo.m_pAttacker, takeDamageInfo.m_pInflictor);
}

void CBaseEntity::takeHealth(float fVal, CBaseEntity *pAttacker, CBaseEntity *pInflictor)
{
	if(m_fHealth <= 0.0f)
	{
		return;
	}
	m_fHealth -= fVal;
	if(m_fHealth <= 0.0f)
	{
		if(!pInflictor)
		{
			pInflictor = pAttacker;
		}
		onDeath(pAttacker, pInflictor);
	}
}

void CBaseEntity::onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor)
{
	LibReport(REPORT_MSG_LEVEL_NOTICE, "Entity %s died!\n", getClassName());
	// do nothing
}

void CBaseEntity::broadcastMessage(const char * szInputName, float fArg, float fRadius)
{
	inputdata_t inputData;
	memset(&inputData, 0, sizeof(inputData));
	inputData.type = PDF_FLOAT;
	inputData.parameter.f = fArg;
	broadcastMessage(szInputName, inputData, fRadius);
}

void CBaseEntity::broadcastMessage(const char * szInputName, int iArg, float fRadius)
{
	inputdata_t inputData;
	memset(&inputData, 0, sizeof(inputData));
	inputData.type = PDF_INT;
	inputData.parameter.i = iArg;
	broadcastMessage(szInputName, inputData, fRadius);
}
void CBaseEntity::broadcastMessage(const char * szInputName, bool bArg, float fRadius)
{
	inputdata_t inputData;
	memset(&inputData, 0, sizeof(inputData));
	inputData.type = PDF_BOOL;
	inputData.parameter.b = bArg;
	broadcastMessage(szInputName, inputData, fRadius);
}
void CBaseEntity::broadcastMessage(const char * szInputName, const char *szArg, float fRadius)
{
	inputdata_t inputData;
	memset(&inputData, 0, sizeof(inputData));
	inputData.type = PDF_STRING;
	inputData.parameter.str = szArg;
	broadcastMessage(szInputName, inputData, fRadius);
}
void CBaseEntity::broadcastMessage(const char * szInputName, const float3_t &f3Arg, float fRadius)
{
	inputdata_t inputData;
	memset(&inputData, 0, sizeof(inputData));
	inputData.type = PDF_VECTOR;
	inputData.v3Parameter = f3Arg;
	broadcastMessage(szInputName, inputData, fRadius);
}
void CBaseEntity::broadcastMessage(const char * szInputName, float fRadius)
{
	inputdata_t inputData;
	memset(&inputData, 0, sizeof(inputData));
	inputData.type = PDF_NONE;
	broadcastMessage(szInputName, inputData, fRadius);
}

void CBaseEntity::broadcastMessage(const char * szInputName, inputdata_t inputData, float fRadius)
{
	inputData.pActivator = this;
	inputData.pInflictor = this;

	CBaseEntity *pEnt = NULL;
	while((pEnt = m_pMgr->findEntityInSphere(getPos(), fRadius, pEnt)))
	{
		if(pEnt == this) continue;
		propdata_t * pField = pEnt->getField(szInputName);
		if(pField && (pField->flags & PDFF_MESSAGE))
		{
			(pEnt->*(pField->fnInput))(&inputData);
		}
	}
}

void CBaseEntity::_cleanup()
{
}

void CBaseEntity::onUse(CBaseEntity *pUser)
{
}

void CBaseEntity::_initEditorBoxes()
{
	if(m_pEditorRigidBody)
	{
		return;
	}

	float3 vBoxHalfSize = m_vEditorBoxSize * 0.5f;
	m_pEditorCollideShape = new btBoxShape(F3_BTVEC(vBoxHalfSize));

	btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(Q4_BTQUAT(SMQuaternion()), F3_BTVEC(m_vPosition)));
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		0,                  // mass
		motionState,        // initial position
		m_pEditorCollideShape,    // collision shape of body
		btVector3(0,0,0)  // local inertia
		);
	m_pEditorRigidBody = new btRigidBody(rigidBodyCI);
	//m_pEditorRigidBody->getInvMass();
	m_pEditorRigidBody->setUserPointer(this);

	SPhysics_AddShapeEx(m_pEditorRigidBody, CG_DEFAULT, CG_BULLETFIRE);
}

void CBaseEntity::_releaseEditorBoxes()
{
	if(m_pEditorRigidBody)
	{
		SPhysics_RemoveShape(m_pEditorRigidBody);
	}
	mem_delete(m_pEditorRigidBody);
	mem_delete(m_pEditorCollideShape);
}

float3 CBaseEntity::getEditorBoxSize()
{
	return(m_vEditorBoxSize);
}

void CBaseEntity::renderEditor(bool is3D)
{

}
