
#ifndef __DIALOG_SELECT_DIR_H
#define __DIALOG_SELECT_DIR_H

#include "sxgui.h"
#include <common/file_utils.h>


//! ширина своего диалога выбора директории
#define DIALOG_OWN_SELDIR_WIDTH		295

//! высота своего диалога выбора директории
#define DIALOG_OWN_SELDIR_HEIGHT	400

#define DIALOG_OWN_SELDIR_PREVIEW_WIDTH	400
#define DIALOG_OWN_SELDIR_PREVIEW_HEIGHT	250

//! ширина своего диалога выбора директории с предпросмотром
#define DIALOG_OWN_SELDIR_WITH_PREVIEW_WIDTH		(10 + DIALOG_OWN_SELDIR_WIDTH + DIALOG_OWN_SELDIR_PREVIEW_WIDTH)


//! инициализация своего диалога выбора папки
void DialogOwnSelectDirInit();

//! запуск своего диалога выбора папки
bool DialogOwnSelectDirRun(char *szOutName, char *szOutPath, const char *szStartPath, const char *szDialogName = 0, bool canExplore = true, bool canCreateNew = true, const char *szDownPath = 0, HandlerDialogOwndGetPreview lpfnHandlerPreview = 0);


#endif