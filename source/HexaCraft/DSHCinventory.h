#ifndef _DSHCinventory_H_
#define _DSHCinventory_H_

#include "DSbase.h"

class DSHCplayer;
class DSbaseTool;

class DSHCinventory
{
	DSHCplayer * m_pOwner;

	struct Item
	{
		DSbaseTool * pTool;
		UINT nAmmount;
		Item():pTool(NULL), nAmmount(0)
		{
		}
	};

	bool m_bIsExtended;

	Item m_pHands[10];
	Item m_pStandard[32];
	Item m_pExtended[64];

	int m_nCurrentHandSlot;

public:

	DSbaseTool * GetActiveTool();

	void SwitchNext();
	void SwitchPrev();

	void SetIsExtended(bool b);

	bool GetIsExtended();
};

#endif
