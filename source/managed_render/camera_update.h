
/*!
\file
Заголвочный файл camera_update - пространства имен для управления навигацией
*/

/*! \defgroup managed_render_camera_update camera_update - пространство имен для управления навигацией
 \ingroup managed_render
@{*/

#ifndef __camera_update
#define __camera_update

//! пространство имен для управления навигацией
namespace CameraUpdate
{
	//! обработка вводы информации с клавиатуры
	void UpdateInputKeyBoard(DWORD timeDelta);

	//! обработка движения мыши вправо и влево
	void UpdateInputMouseRotate(DWORD timeDelta);

	//! обработка движения мыши вверх вниз
	void UpdateInputMouseUpDown(DWORD timeDelta);

	//! центрирвоание курсора мыши
	void CentererCursor();

	//! обновление навигации режим "редактор"
	void UpdateEditorial(DWORD timeDelta);
};

#endif

//!@} managed_render_camera_update