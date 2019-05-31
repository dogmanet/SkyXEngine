#ifndef __IXRENDERABLE_H
#define __IXRENDERABLE_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <mtrl/IXMaterialSystem.h>
#include "IXRenderPipeline.h"
#include "render/IFrustum.h"

// {FDD30038-7D32-4EC6-911F-635376B1450D}
#define IXRENDERABLE_GUID DEFINE_XGUID(0xfdd30038, 0x7d32, 0x4ec6, 0x91, 0x1f, 0x63, 0x53, 0x76, 0xb1, 0x45, 0xd)
#define IXRENDERABLE_VERSION 1

DEFINE_ENUM_FLAG_OPERATORS(X_RENDER_STAGE);

class IXOcclusionCuller;
class ICamera;
class IFrustum;

//! Данные о видимости для конкретной системы
class IXRenderableVisibility: public IXUnknown
{
public:
	virtual ID getPluginId() = 0;

	//! Установка отсекателя по перекрытию
	virtual void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller) = 0;

	//! Выполнение проверки видимости для заданной камеры. Если pReference задан - проверка ограничивается только уже рассчитанным множеством
	virtual void updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference = NULL) = 0;

	//! Выполнение проверки видимости для заданного фрустума. Если pReference задан - проверка ограничивается только уже рассчитанным множеством
	virtual void updateForFrustum(const IFrustum *pFrustum, const IXRenderableVisibility *pReference = NULL) = 0;

	/*! Выполнение проверки видимости для заданной камеры в режиме оптимизации для многопоточности. 
	    Возвращает ID задачи из менеджера задач для отслеживания завершения внутренних задач
	*/
//	virtual ID updateForCameraThreaded(ICamera *pCamera) = 0;
};

class IXRenderable: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getVersion()
	{
		return(IXRENDERABLE_VERSION);
	}

	//! Возвращает поддерживаемые стадии рендера
	virtual X_RENDER_STAGE getStages() = 0;

	//! Возвращает приоритет рендера внутри стадии. Чем меньше число - тем раньше будет рендер
	virtual UINT getPriorityForStage(X_RENDER_STAGE stage) = 0;

	//! Выполняет отрисовку согласно заданной стадии с учетом видимости. Если pVisibility == NULL - рисуется все
	virtual void renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility) = 0;

	virtual void startup(IGXContext *pDevice, IXMaterialSystem *pMaterialSystem) = 0;
	virtual void shutdown() = 0;

	//! Создает новый объект просчета видимости для системы
	virtual void newVisData(IXRenderableVisibility **ppVisibility) = 0;

	//! Оптимизирован ли алгоритм расчета видимости для параллельного выполнения?
	virtual bool isVisThreadOptimized()
	{
		return(false);
	}
};

#endif
