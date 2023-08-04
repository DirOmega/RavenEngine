#pragma once

#include "MouseBTNEnum.h"


class Mouse
{
public:

	Mouse();

	bool GetKeyState(MouseBTN button);

	void GetCursor(float &xPos, float &yPos);

};