#include <assert.h>
#include "InputManager.h"
#include "Engine.h"

#include "ScratchSpace.h"

InputMan *InputMan::pInputMan = 0;

void InputMan::Create()
{
	InputMan::privCreate();
}

void InputMan::deleteInputs()
{                                                          
	delete pInputMan;
}

InputMan::InputMan()
{
	this->pKeyboard = new Keyboard();
	assert(this->pKeyboard);
	this->pMouse = new Mouse();
	assert(this->pMouse);
}

void InputMan::privCreate()
{
	InputMan::pInputMan = new InputMan();
	assert(pInputMan);
}

InputMan::~InputMan()
{
	delete pKeyboard;
	delete pMouse;
}

Keyboard* InputMan::GetKeyboard()
{
	return privInstance()->pKeyboard;
}

Mouse* InputMan::GetMouse()
{
	return privInstance()->pMouse;
}
//
//Engine* InputMan::GetEngine()
//{
//	return privInstance()->pEngine;
//}

InputMan* InputMan::privInstance()
{
	if (pInputMan == nullptr)
	{
		InputMan::privCreate();
	}
	return pInputMan;
}