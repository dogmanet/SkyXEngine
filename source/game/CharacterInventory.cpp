#include "CharacterInventory.h"

#include "BaseCharacter.h"
#include "HUDcontroller.h"

CCharacterInventory::CCharacterInventory(CBaseCharacter * pOwner, int iSlotCount):
	m_pOwner(pOwner),
	m_iSlotCount(iSlotCount)
{
	assert(iSlotCount > 0);

	m_ppSlots = new CBaseItem*[iSlotCount];
	memset(m_ppSlots, 0, sizeof(CBaseItem*) * iSlotCount);
}

CCharacterInventory::~CCharacterInventory()
{
	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i])
		{
			REMOVE_ENTITY(m_ppSlots[i]);
		}
	}
	mem_delete_a(m_ppSlots);
}

bool CCharacterInventory::hasItems(const char * szClassName, int iCount)
{
	assert(iCount > 0);

	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i] && !fstrcmp(m_ppSlots[i]->getClassName(), szClassName))
		{
			iCount -= m_ppSlots[i]->m_iInvStackCurSize;
			if(iCount <= 0)
			{
				return(true);
			}
		}
	}
	return(iCount <= 0);
}

int CCharacterInventory::consumeItems(const char *szClassName, int iCount)
{
	assert(iCount > 0);

	int iConsumed = 0;

	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i] && !fstrcmp(m_ppSlots[i]->getClassName(), szClassName))
		{
			if(m_ppSlots[i]->m_iInvStackCurSize >= iCount)
			{
				iConsumed += iCount;
				m_ppSlots[i]->m_iInvStackCurSize -= iCount;
				if(m_ppSlots[i]->m_iInvStackCurSize <= 0)
				{
					REMOVE_ENTITY(m_ppSlots[i]);
					m_ppSlots[i] = 0;
				}
				break;
			}
			else
			{
				iConsumed += m_ppSlots[i]->m_iInvStackCurSize;
				iCount -= m_ppSlots[i]->m_iInvStackCurSize;
				
				REMOVE_ENTITY(m_ppSlots[i]);
				m_ppSlots[i] = 0;
			}
		}
	}
	m_pOwner->onInventoryChanged();
	return(iConsumed);
}

void CCharacterInventory::putItems(const char *szClassName, int iCount)
{
	assert(iCount > 0);
	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i] && !fstrcmp(m_ppSlots[i]->getClassName(), szClassName) && m_ppSlots[i]->m_bInvStackable)
		{
			int iCanAdd = m_ppSlots[i]->m_iInvStackMaxSize - m_ppSlots[i]->m_iInvStackCurSize;
			if(iCanAdd > 0)
			{
				if(iCanAdd >= iCount)
				{
					m_ppSlots[i]->m_iInvStackCurSize += iCount;
					iCount = 0;
					break;
				}
				else
				{
					iCount -= iCanAdd;
					m_ppSlots[i]->m_iInvStackCurSize += iCanAdd;
				}
			}
		}
	}

	if(iCount > 0)
	{
		for(int i = 0; i < m_iSlotCount; ++i)
		{
			if(!m_ppSlots[i])
			{
				if((m_ppSlots[i] = (CBaseItem*)CREATE_ENTITY(szClassName, m_pOwner->getManager())))
				{
					int iCanAdd = m_ppSlots[i]->m_iInvStackMaxSize - m_ppSlots[i]->m_iInvStackCurSize;
					if(iCanAdd > 0)
					{
						if(iCanAdd >= iCount)
						{
							m_ppSlots[i]->m_iInvStackCurSize += iCount;
							iCount = 0;
							break;
						}
						else
						{
							iCount -= iCanAdd;
							m_ppSlots[i]->m_iInvStackCurSize += iCanAdd;
						}
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	m_pOwner->onInventoryChanged();
}

void CCharacterInventory::putItem(CBaseItem *pItem)
{
	assert(pItem);

	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i] && pItem->m_bInvStackable && !fstrcmp(m_ppSlots[i]->getClassName(), pItem->getClassName()))
		{
			int iCanAdd = m_ppSlots[i]->m_iInvStackMaxSize - m_ppSlots[i]->m_iInvStackCurSize;
			if(iCanAdd > 0)
			{
				if(iCanAdd >= pItem->m_iInvStackCurSize)
				{
					m_ppSlots[i]->m_iInvStackCurSize += pItem->m_iInvStackCurSize;
					pItem->m_iInvStackCurSize = 0;
					break;
				}
				else
				{
					pItem->m_iInvStackCurSize -= iCanAdd;
					m_ppSlots[i]->m_iInvStackCurSize += iCanAdd;
				}
			}
		}
	}

	if(pItem->m_iInvStackCurSize > 0 || !pItem->m_bInvStackable)
	{
		for(int i = 0; i < m_iSlotCount; ++i)
		{
			if(!m_ppSlots[i])
			{
				if(pItem->m_bInvStackable)
				{
					if((m_ppSlots[i] = (CBaseItem*)CREATE_ENTITY(pItem->getClassName(), m_pOwner->getManager())))
					{
						int iCanAdd = m_ppSlots[i]->m_iInvStackMaxSize - m_ppSlots[i]->m_iInvStackCurSize;
						if(iCanAdd > 0)
						{
							if(iCanAdd >= pItem->m_iInvStackCurSize)
							{
								m_ppSlots[i]->m_iInvStackCurSize += pItem->m_iInvStackCurSize;
								pItem->m_iInvStackCurSize = 0;
								break;
							}
							else
							{
								pItem->m_iInvStackCurSize -= iCanAdd;
								m_ppSlots[i]->m_iInvStackCurSize += iCanAdd;
							}
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					m_ppSlots[i] = pItem;
					break;
				}
			}
		}
	}

	CHUDcontroller *pHUD = m_pOwner->getHUDcontroller();

	if(pHUD)
	{
		char str[128];
		sprintf(str, "Найден предмет: %s", pItem->m_szInvName);
		pHUD->chatMsg(str);
	}

	m_pOwner->onInventoryChanged();
}

int CCharacterInventory::getSlotCount() const
{
	return(m_iSlotCount);
}

CBaseItem * CCharacterInventory::getSlot(int i) const
{
	assert(i >= 0 && i < m_iSlotCount);

	return(m_ppSlots[i]);
}

float CCharacterInventory::getTotalWeight() const
{
	float fTotal = 0.0f;

	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i])
		{
			fTotal += m_ppSlots[i]->getWeight() * m_ppSlots[i]->m_iInvStackCurSize;
		}
	}

	return(fTotal);
}

int CCharacterInventory::getItemCount(const char * szClassName)
{
	int iCount = 0;
	for(int i = 0; i < m_iSlotCount; ++i)
	{
		if(m_ppSlots[i] && !fstrcmp(m_ppSlots[i]->getClassName(), szClassName))
		{
			iCount += m_ppSlots[i]->m_iInvStackCurSize;
		}
	}
	return(iCount);
}
