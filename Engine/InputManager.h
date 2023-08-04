#pragma once

#include "Keyboard.h"
#include "Mouse.h"

class InputMan
{

public:

	static void Create();
	static Keyboard *GetKeyboard();
	static Mouse    *GetMouse();
	static void deleteInputs();
	//static Engine   *GetEngine();
	//static GLFWwindow* GetWindow();
	~InputMan();

private:

	static InputMan *privInstance();
	static void      privCreate();
	InputMan();

	//Data
	static InputMan *pInputMan;
	//Engine			*pEngine; //TODO:: move to a manager or something
	Keyboard		*pKeyboard;
	Mouse			*pMouse;

};