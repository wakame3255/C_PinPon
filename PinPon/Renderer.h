#pragma once
#include "Game.h"
#include "SDL.h"

class Renderer
{
	SDL_Renderer* sdlRenderer;

public:
	void DrawPaddles(const std::vector<SDL_Rect>& paddles);
	void DrawWalls();
	void DrawScore();
	void Clear();
	void Present();
};