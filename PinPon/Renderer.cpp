#include "Renderer.h"
#include "Ball.h"
#include "Paddle.h"
#include <string>

// コンストラクタ
Renderer::Renderer()
	: mWindow(nullptr)
	, mRenderer(nullptr)
{
	// メンバー変数の初期化
}

// デストラクタ
Renderer::~Renderer()
{
	// 自動的にShutdownが呼ばれるようにする
	Shutdown();
}

// 初期化処理
bool Renderer::Initialize()
{
	// SDLビデオサブシステムの初期化
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// ウィンドウの作成
	mWindow = SDL_CreateWindow(
		"PinPon Game",                                    // ウィンドウタイトル
		100,                                              // X座標
		100,                                              // Y座標
		static_cast<int>(GameConstants::WINDOW_WIDTH),   // 幅
		static_cast<int>(GameConstants::WINDOW_HEIGHT),  // 高さ
		0                                                 // フラグ
	);
	
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	// レンダラーの作成（垂直同期を有効化）
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	return true;
}

// 終了処理
void Renderer::Shutdown()
{
	// レンダラーの破棄
	if (mRenderer) {
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}
	
	// ウィンドウの破棄
	if (mWindow) {
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}
}

// 画面クリア
void Renderer::Clear()
{
	// 背景色を黒に設定
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

// 画面更新
void Renderer::Present()
{
	SDL_RenderPresent(mRenderer);
}

// パドルの描画
void Renderer::DrawPaddle(const Paddle& paddle)
{
	// パドルを白色で描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// パドルの矩形を作成
	Vector2 pos = paddle.GetPosition();
	SDL_Rect paddleRect{
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(paddle.GetWidth()),
		static_cast<int>(paddle.GetHeight())
	};
	
	// パドルを塗りつぶし描画
	SDL_RenderFillRect(mRenderer, &paddleRect);
}

// ボールの描画
void Renderer::DrawBall(const Ball& ball)
{
	// ボールを白色で描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// ボールの矩形を作成（円の代わりに正方形で簡易的に描画）
	Vector2 pos = ball.GetPosition();
	float radius = ball.GetRadius();
	SDL_Rect ballRect{
		static_cast<int>(pos.x - radius),
		static_cast<int>(pos.y - radius),
		static_cast<int>(radius * 2),
		static_cast<int>(radius * 2)
	};
	
	// ボールを塗りつぶし描画
	SDL_RenderFillRect(mRenderer, &ballRect);
}

// 壁の描画
void Renderer::DrawWalls()
{
	// 壁を白色で描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// 上壁の描画
	SDL_Rect upperWall{
		0,                                                     // X座標
		static_cast<int>(GameConstants::WALL_SPACE),         // Y座標
		static_cast<int>(GameConstants::WINDOW_WIDTH),       // 幅
		static_cast<int>(GameConstants::WALL_HEIGHT)         // 高さ
	};
	SDL_RenderFillRect(mRenderer, &upperWall);
	
	// 下壁の描画
	SDL_Rect lowerWall{
		0,                                                     // X座標
		static_cast<int>(GameConstants::UNDER_WALL),         // Y座標
		static_cast<int>(GameConstants::WINDOW_WIDTH),       // 幅
		static_cast<int>(GameConstants::WALL_HEIGHT)         // 高さ
	};
	SDL_RenderFillRect(mRenderer, &lowerWall);
}

// スコアの描画（現在は実装なし）
void Renderer::DrawScore(int leftScore, int rightScore)
{
	// TODO: TTFフォントを使用してスコアを描画する
	// 現在は実装を省略
}

// センターラインの描画
void Renderer::DrawCenterLine()
{
	// センターラインを白色で描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// 破線のセンターラインを描画
	int centerX = static_cast<int>(GameConstants::WINDOW_WIDTH / 2);
	int segmentHeight = 15;  // 各線分の高さ
	int gap = 10;            // 線分間のギャップ
	
	// 上壁から下壁まで破線を描画
	for (int y = static_cast<int>(GameConstants::UPPER_WALL); 
		 y < static_cast<int>(GameConstants::UNDER_WALL); 
		 y += segmentHeight + gap) {
		
		SDL_Rect segment{
			centerX - 2,      // X座標（線の太さ4ピクセル）
			y,                // Y座標
			4,                // 幅
			segmentHeight     // 高さ
		};
		SDL_RenderFillRect(mRenderer, &segment);
	}
}