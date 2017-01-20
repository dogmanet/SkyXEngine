
#ifndef __camera_update
#define __camera_update

namespace CameraUpdate
{
	//обработка вводы информации с клавиатуры
	void UpdateInputKeyBoard(DWORD timeDelta);

	//обработка движения мыши вправо и влево
	void UpdateInputMouseRotate(DWORD timeDelta);

	//обработка движения мыши вверх вниз
	void UpdateInputMouseUpDown(DWORD timeDelta);

	//центрирвоание курсора мыши
	void CentererCursor();

	void UpdateEditorial(DWORD timeDelta);
};

#endif