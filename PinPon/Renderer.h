#pragma once
#include "SDL.h"
#include "UtilityData.h"
#include <vector>

// 前方宣言
class Ball;
class Paddle;

class Renderer
{
private:
	SDL_Window* mWindow;      // SDLウィンドウ
	SDL_Renderer* mRenderer;  // SDL2Dレンダラー
	
public:
	// コンストラクタ・デストラクタ
	Renderer();
	~Renderer();
	
	// 初期化処理
	bool Initialize();
	
	// 終了処理
	void Shutdown();
	
	// フレーム開始時の処理（画面クリア）
	void Clear();
	
	// フレーム終了時の処理（画面更新）
	void Present();
	
	// 描画処理
	void DrawPaddle(const Paddle& paddle);
	void DrawBall(const Ball& ball);
	void DrawWalls();
	void DrawScore(int leftScore, int rightScore);
	void DrawCenterLine();
	
	// ゲッター
	SDL_Renderer* GetSDLRenderer() { return mRenderer; }
};