#ifndef __IXUPDATABLE_H
#define __IXUPDATABLE_H

#include <gdefines.h>

// {4287479D-1A8D-4321-87D1-6D09A8978C63}
#define IXUPDATABLE_GUID DEFINE_XGUID(0x4287479d, 0x1a8d, 0x4321, 0x87, 0xd1, 0x6d, 0x9, 0xa8, 0x97, 0x8c, 0x63)

class IXUpdatable: public IXUnknown
{
public:
	//! Инициирует компонент, возвращает приоритет обновления. чем меньше - тем раньше
	virtual UINT startup() = 0;
	virtual void shutdown() = 0;

	//! Запускает обновление, возвращает ID задачи при параллельной обработке, либо -1, если задача не создавалась
	virtual ID run(float fDelta) = 0;

	//! Выполняется в момент синхронизации в однопоточном режиме
	virtual void sync() = 0;
};

#endif
