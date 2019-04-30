#ifndef __IXRENDERPIPELINE_H
#define __IXRENDERPIPELINE_H

#include <gdefines.h>

//! Стадии рендера
enum X_RENDER_STAGE
{
	XRS_PREPARE = 0x00000001, //!< Подготовка, здесь можно обновить необходимые рендертаргеты и прочее
	XRS_GBUFFER = 0x00000002, //!< Рендер в ГБуфер
	XRS_SHADOWS = 0x00000004, //!< Рендер теней
	XRS_GI = 0x00000008, //!< Рендер глобального освещения (?)
	XRS_POSTPROCESS_MAIN = 0x00000010, //!< Основной постпроцесс (рисуется до прозрачных материалов)
	XRS_TRANSPARENT = 0x00000020, //!< Прозрачные материалы
	XRS_POSTPROCESS_FINAL = 0x00000040, //!< Финальный постпроцесс

	XRS_EDITOR_2D = 0x00000080  //!< Рендер для редактора в виде 2D
};

class IGXContext;
class IXRenderableVisibility;
class IXRenderPipeline: public IXUnknown
{
public:
	virtual void resize(UINT uWidth, UINT uHeight, bool isWindowed = true) = 0;

	virtual void renderFrame() = 0;
	virtual void endFrame() = 0;
	virtual void updateVisibility() = 0;

	virtual void renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility = NULL) = 0;

	virtual IGXContext *getDevice() = 0;

	virtual void newVisData(IXRenderableVisibility **ppVisibility) = 0;

protected:

	virtual void renderPrepare() = 0;
	virtual void renderGBuffer() = 0;
	virtual void renderShadows() = 0;
	virtual void renderGI() = 0;
	virtual void renderPostprocessMain() = 0;
	virtual void renderTransparent() = 0;
	virtual void renderPostprocessFinal() = 0;
	virtual void renderEditor2D() = 0;
};

#endif
