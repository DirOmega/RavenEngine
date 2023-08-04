#include "Mouse.h"
#include "Game.h"

extern Game* pGame;

Mouse::Mouse()
{

}

bool Mouse::GetKeyState(MouseBTN button)
{
	bool value;
	
	if (pGame->GetMouseButton((int)button) == GLFW_PRESS)
	{
		value = true;
	}
	else
	{
		value = false;
	}
	return value;
}
//TODO:: this really turns out to be integers but if I wanted to do division I would likley end up with a float so consider switching this to int and letting user worry about the float conversions.
void Mouse::GetCursor(float &xPos, float&yPos)
{
	pGame->GetCursorPos(xPos, yPos);
}