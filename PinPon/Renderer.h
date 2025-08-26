#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "UtilityData.h"
#include <vector>
#include <string>

// 前方宣言
class Ball;
class Paddle;

class Renderer
{
private:
	SDL_Window* mWindow;      // SDLウィンドウ
	SDL_Renderer* mRenderer;  // SDL2Dレンダラー
	TTF_Font* mFont;          // フォント（将来の拡張用）
	
	// 円を描画するためのヘルパー関数
	void DrawCircle(int centerX, int centerY, int radius);
	void DrawFilledCircle(int centerX, int centerY, int radius);
	
	// テキスト描画のヘルパー関数（簡易ASCII文字描画）
	void DrawSimpleText(const std::string& text, int x, int y, int scale = 2);
	void DrawDigit(int digit, int x, int y, int scale = 2);
	
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
	
	// メッセージ描画
	void DrawPauseMessage();
	void DrawGameOverMessage(int leftScore, int rightScore);
	
	// 色設定のヘルパー関数
	void SetRenderColor(int r, int g, int b, int a = 255);
	
	// ゲッター
	SDL_Renderer* GetSDLRenderer() { return mRenderer; }
};