#ifndef __IXRENDERABLE_H
#define __IXRENDERABLE_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <mtrl/IXMaterialSystem.h>

// {FDD30038-7D32-4EC6-911F-635376B1450D}
#define IXRENDERABLE_GUID DEFINE_XGUID(0xfdd30038, 0x7d32, 0x4ec6, 0x91, 0x1f, 0x63, 0x53, 0x76, 0xb1, 0x45, 0xd)
#define IXRENDERABLE_VERSION 1

//! Стадии рендера
enum X_RENDER_STAGE
{
	XRS_PREPARE           = 0x00000001, //!< Подготовка, здесь можно обновить необходимые рендертаргеты и прочее
	XRS_GBUFFER           = 0x00000002, //!< Рендер в ГБуфер
	XRS_SHADOWS           = 0x00000004, //!< Рендер теней
	XRS_GI                = 0x00000008, //!< Рендер глобального освещения (?)
	XRS_POSTPROCESS_MAIN  = 0x00000010, //!< Основной постпроцесс (рисуется до прозрачных материалов)
	XRS_TRANSPARENT       = 0x00000020, //!< Прозрачные материалы
	XRS_POSTPROCESS_FINAL = 0x00000040, //!< Финальный постпроцесс

	XRS_EDITOR_2D         = 0x00000080  //!< Рендер для редактора в виде 2D
};
DEFINE_ENUM_FLAG_OPERATORS(X_RENDER_STAGE);

class IXOcclusionCuller;
class ICamera;

//! Данные о видимости для конкретной системы
class IXRenderableVisibility: public IXUnknown
{
public:
	//! Установка отсекателя по перекрытию
	virtual void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller) = 0;

	//! Выполнение проверки видимости для заданной камеры
	virtual void updateForCamera(ICamera *pCamera) = 0;

	/*! Выполнение проверки видимости для заданной камеры в режиме оптимизации для многопоточности. 
	    Возвращает ID задачи из менеджера задач для отслеживания завершения внутренних задач
	*/
	virtual ID updateForCameraThreaded(ICamera *pCamera)
	{
	}
};

class IXRenderable: public IXUnknown
{
public:
	virtual UINT getVersion()
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
