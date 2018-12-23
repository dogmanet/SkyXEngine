#ifndef __CHARACTER_INVENTORY_H
#define __CHARACTER_INVENTORY_H

#include "BaseItem.h"

class CBaseCharacter;

class CCharacterInventory
{
public: 
	CCharacterInventory(CBaseCharacter * pOwner, int iSlotCount = 8 * 16);
	~CCharacterInventory();

	bool hasItems(const char * szClassName, int iCount = 1);
	int consumeItems(const char *szClassName, int iCount = 1);
	int getItemCount(const char *szClassName);

	void putItems(const char *szClassName, int iCount = 1);
	void putItem(CBaseItem *pItem);
	//void putItems(CBaseItem *pItem);
	
	int getSlotCount() const;
	CBaseItem * getSlot(int i) const;

	float getTotalWeight() const;

protected:

	CBaseCharacter * m_pOwner;
	CBaseItem ** m_ppSlots;
	int m_iSlotCount;
};

#endif
