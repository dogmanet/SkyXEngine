#ifndef __COMBINER_H
#define __COMBINER_H

#include <xcommon/editor/IXEditable.h>

class CEditable;
class CEditorObject;
class IXCore;

class CCombiner final
{
public:
	CCombiner(CEditable *pEditable);
	~CCombiner();

	void build();

	UINT getResourceCount();
	void getResourceAt(UINT idx, IXResourceModel **ppOut);

private:
	void clean();

private:
	CEditable *m_pEditable;
	Array<IXResourceModel*> m_aResources;
};

#endif
