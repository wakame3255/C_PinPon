#pragma once
#include "SDL.h"

class InputSystem
{
public:
	void ProcessInput();
	bool IsKeyPressed(SDL_Scancode key);
};