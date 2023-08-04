#include "Keyboard.h"
#include "KeyEnum.h"
#include "GLibSetup.h"

#include "Game.h"

extern Game* pGame;

Keyboard::Keyboard()
{

}

bool Keyboard::GetKeyState(KeyEnum key)
	{
	bool value;
	//Engine* pEng = InputMan::GetEngine();
	if (pGame->GetKey((int)key) == GLFW_PRESS)
	{
		value = true;
	}
	else
	{
		value = false;
	}
	return value;
	}
