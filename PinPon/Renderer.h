#pragma once
#include "Game.h"
#include "SDL.h"
#include "UtilityData.h"

class Renderer
{
	//SDLのウィンドウ
	SDL_Window* mWindow;
	//SDLの2Dレンダラー
	SDL_Renderer* mRenderer;

public:
	void DrawPaddles(const std::vector<SDL_Rect>& paddles);
	void DrawWalls();
	void DrawScore();
	void Clear();
	void Present();
};