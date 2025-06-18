#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 pos;
	Vector2 vel;
};

class Game
{
public: 
	Game();
	//ゲームの初期化
	bool Initialize();
	//ゲームのメインループ
	void RunLoop();
	//ゲームの終了処理
	void Shutdown();
private:
	//ゲームに必要な関数群
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDLのウィンドウ
	SDL_Window* mWindow;
	//SDLの2Dレンダラー
	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	//ゲームの状態
	bool mIsRunning;


	std::vector<Ball> mBalls;
	
	std::vector<Vector2> mPaddlesDir;

	std::vector<Vector2> mPaddlesPos;
};