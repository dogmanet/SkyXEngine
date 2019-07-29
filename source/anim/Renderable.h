#ifndef __RENDERABLE_H
#define __RENDERABLE_H

#include <xcommon/IXRenderable.h>

#include "AnimatedModelProvider.h"
#include "DynamicModelProvider.h"

class CRenderable: public IXRenderable
{
public:
	CRenderable(ID idPlugin, CAnimatedModelProvider *pProviderAnimated, CDynamicModelProvider *pProviderDynamic);

	//! Возвращает поддерживаемые стадии рендера
	X_RENDER_STAGE XMETHODCALLTYPE getStages() override;

	//! Возвращает приоритет рендера внутри стадии. Чем меньше число - тем раньше будет рендер
	UINT XMETHODCALLTYPE getPriorityForStage(X_RENDER_STAGE stage) override;


	//! Выполняет отрисовку согласно заданной стадии с учетом видимости. Если pVisibility == NULL - рисуется все
	void XMETHODCALLTYPE renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility) override;

	//! Отрисовка прозрачных объектов
	//! @{
	UINT XMETHODCALLTYPE getTransparentCount(IXRenderableVisibility *pVisibility) override;
	void XMETHODCALLTYPE getTransparentObject(IXRenderableVisibility *pVisibility, UINT uIndex, XTransparentObject *pObject) override;
	void XMETHODCALLTYPE renderTransparentObject(IXRenderableVisibility *pVisibility, UINT uIndex, UINT uSplitPlanes) override;
	//! @}

	void XMETHODCALLTYPE startup(IGXContext *pDevice, IXMaterialSystem *pMaterialSystem) override;
	void XMETHODCALLTYPE shutdown() override;

	//! Создает новый объект просчета видимости для системы
	void XMETHODCALLTYPE newVisData(IXRenderableVisibility **ppVisibility) override;

	IXMaterialSystem* getMaterialSystem();
protected:
	IGXContext *m_pDevice = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;
	ID m_idPlugin = -1;

	CAnimatedModelProvider *m_pAnimatedModelProvider = NULL;
	CDynamicModelProvider *m_pDynamicModelProvider = NULL;
};

#endif
