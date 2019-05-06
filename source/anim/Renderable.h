#ifndef __RENDERABLE_H
#define __RENDERABLE_H

#include <xcommon/IXRenderable.h>

class CRenderable: public IXRenderable
{
public:
	//! Возвращает поддерживаемые стадии рендера
	X_RENDER_STAGE getStages();

	//! Возвращает приоритет рендера внутри стадии. Чем меньше число - тем раньше будет рендер
	UINT getPriorityForStage(X_RENDER_STAGE stage);

	//! Выполняет отрисовку согласно заданной стадии с учетом видимости. Если pVisibility == NULL - рисуется все
	void renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility);

	void startup(IGXContext *pDevice, IXMaterialSystem *pMaterialSystem);
	void shutdown();

	//! Создает новый объект просчета видимости для системы
	void newVisData(IXRenderableVisibility **ppVisibility);

	IXMaterialSystem *getMaterialSystem();
protected:
	IGXContext *m_pDevice = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;
};

#endif
