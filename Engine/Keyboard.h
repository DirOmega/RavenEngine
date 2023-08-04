#pragma once

#include "KeyEnum.h"

//Enum adds safety and auto complete

class Keyboard
{

public:

	Keyboard();

	bool GetKeyState(KeyEnum key);

	bool OKeyPrev;
	bool LBKeyPrev;
	bool RBKeyPrev;
	bool pad[1];
};