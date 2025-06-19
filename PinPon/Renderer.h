#pragma once
#include "SDL.h"

class Renderer
{
	SDL_Renderer* sdlRenderer;

public:
	void DrawWalls();
	void DrawScore();
	void Clear();
	void Present();
};