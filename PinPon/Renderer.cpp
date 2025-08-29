#include "Renderer.h"
#include "Ball.h"
#include "Paddle.h"
#include <string>
#include <cmath>
#include "SDL_ttf.h"

// コンストラクタ
Renderer::Renderer()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mFont(nullptr)
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
		"PinPon Game - Enhanced Edition",
		100,
		100,
		static_cast<int>(GameConstants::WINDOW_WIDTH),
		static_cast<int>(GameConstants::WINDOW_HEIGHT),
		0
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

	// SDL_ttf の初期化
	if (TTF_Init() == -1) {
		SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
		return false;  // フォントが必須の場合は false を返す
	}

	// フォントの読み込み（WindowsのメイリオまたはMS Gothic）
	mFont = TTF_OpenFont("fonts/arial.ttf", 24);
	if (!mFont) {
		// フォールバック：MS Gothic を試す
		mFont = TTF_OpenFont("C:\\Windows\\Fonts\\msgothic.ttc", 24);
		if (!mFont) {
			SDL_Log("Failed to open font: %s", TTF_GetError());
			return false;
		}
	}

	return true;
}

// 終了処理
// 終了処理
void Renderer::Shutdown()
{
	// フォントの破棄
	if (mFont) {
		TTF_CloseFont(mFont);
		mFont = nullptr;
	}

	// SDL_ttf の終了
	TTF_Quit();

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
	// 背景色を濃い青に設定
	SetRenderColor(20, 30, 60);
	SDL_RenderClear(mRenderer);
}

// 画面更新
void Renderer::Present()
{
	SDL_RenderPresent(mRenderer);
}

// 色設定のヘルパー関数
void Renderer::SetRenderColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
}

// パドルの描画
void Renderer::DrawPaddle(const Paddle& paddle)
{
	// パドルを明るい青色で描画
	if (paddle.IsLeftPaddle()) {
		SetRenderColor(GameConstants::Colors::BLUE.r, 
		               GameConstants::Colors::BLUE.g, 
		               GameConstants::Colors::BLUE.b);
	} else {
		SetRenderColor(GameConstants::Colors::RED.r, 
		               GameConstants::Colors::RED.g, 
		               GameConstants::Colors::RED.b);
	}
	
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
	
	// パドルの境界線を白で描画
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	SDL_RenderDrawRect(mRenderer, &paddleRect);
}

// ボールの描画（円として描画）
void Renderer::DrawBall(const Ball& ball)
{
	Vector2 pos = ball.GetPosition();
	float radius = ball.GetRadius();
	
	// ボールを黄色で描画
	SetRenderColor(GameConstants::Colors::YELLOW.r, 
	               GameConstants::Colors::YELLOW.g, 
	               GameConstants::Colors::YELLOW.b);
	
	// 円として描画
	DrawFilledCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(radius));
	
	// ボールの境界線を白で描画
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(radius));
}

// 円の描画（境界線のみ）
void Renderer::DrawCircle(int centerX, int centerY, int radius) {
	int x = radius;
	int y = 0;
	int decisionParameter = 1 - radius;
	
	while (x >= y) {
		// 8つの対称点を描画
		SDL_RenderDrawPoint(mRenderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(mRenderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(mRenderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(mRenderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(mRenderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(mRenderer, centerX - y, centerY + x);
		SDL_RenderDrawPoint(mRenderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(mRenderer, centerX - y, centerY - x);
		
		y++;
		
		if (decisionParameter <= 0) {
			decisionParameter += 2 * y + 1;
		} else {
			x--;
			decisionParameter += 2 * (y - x) + 1;
		}
	}
}

// 塗りつぶされた円の描画
void Renderer::DrawFilledCircle(int centerX, int centerY, int radius) {
	for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
			int dx = radius - w;
			int dy = radius - h;
			if ((dx * dx + dy * dy) <= (radius * radius)) {
				SDL_RenderDrawPoint(mRenderer, centerX + dx, centerY + dy);
			}
		}
	}
}

// 壁の描画
void Renderer::DrawWalls()
{
	// 壁を緑色で描画
	SetRenderColor(GameConstants::Colors::GREEN.r, 
	               GameConstants::Colors::GREEN.g, 
	               GameConstants::Colors::GREEN.b);
	
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
	
	// 境界線を白で描画
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	SDL_RenderDrawRect(mRenderer, &upperWall);
	SDL_RenderDrawRect(mRenderer, &lowerWall);
}

// スコアの描画
void Renderer::DrawScore(int leftScore, int rightScore)
{
	// スコアを白色で描画
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	
	// 左スコアの描画（画面左上）
	DrawDigit(leftScore, 50, 0, 4);
	
	// 右スコアの描画（画面右上）
	DrawDigit(rightScore, static_cast<int>(GameConstants::WINDOW_WIDTH) - 100, 0, 4);
	
	// 勝利スコアの表示
	std::string winText = "First to " + std::to_string(GameConstants::WINNING_SCORE);
	DrawSimpleText(winText, static_cast<int>(GameConstants::WINDOW_WIDTH / 2) - 100, 30, 1);
}

// 数字の描画（0-9用の簡易フォント）
void Renderer::DrawDigit(int digit, int x, int y, int scale) {
	if (digit < 0 || digit > 9) return;
	
	int pixelSize = 2 * scale;
	
	// 7セグメントディスプレイ風の数字描画
	bool segments[10][7] = {
		{1,1,1,1,1,1,0}, // 0
		{0,1,1,0,0,0,0}, // 1
		{1,1,0,1,1,0,1}, // 2
		{1,1,1,1,0,0,1}, // 3
		{0,1,1,0,0,1,1}, // 4
		{1,0,1,1,0,1,1}, // 5
		{1,0,1,1,1,1,1}, // 6
		{1,1,1,0,0,0,0}, // 7
		{1,1,1,1,1,1,1}, // 8
		{1,1,1,1,0,1,1}  // 9
	};
	
	int segmentWidth = 8 * scale;
	int segmentHeight = 3 * scale;
	
	// 各セグメントの描画
	if (segments[digit][0]) { // 上
		SDL_Rect rect = {x, y, segmentWidth, segmentHeight};
		SDL_RenderFillRect(mRenderer, &rect);
	}
	if (segments[digit][1]) { // 右上
		SDL_Rect rect = {x + segmentWidth - segmentHeight, y, segmentHeight, segmentWidth};
		SDL_RenderFillRect(mRenderer, &rect);
	}
	if (segments[digit][2]) { // 右下
		SDL_Rect rect = {x + segmentWidth - segmentHeight, y + segmentWidth, segmentHeight, segmentWidth};
		SDL_RenderFillRect(mRenderer, &rect);
	}
	if (segments[digit][3]) { // 下
		SDL_Rect rect = {x, y + 2 * segmentWidth - segmentHeight, segmentWidth, segmentHeight};
		SDL_RenderFillRect(mRenderer, &rect);
	}
	if (segments[digit][4]) { // 左下
		SDL_Rect rect = {x, y + segmentWidth, segmentHeight, segmentWidth};
		SDL_RenderFillRect(mRenderer, &rect);
	}
	if (segments[digit][5]) { // 左上
		SDL_Rect rect = {x, y, segmentHeight, segmentWidth};
		SDL_RenderFillRect(mRenderer, &rect);
	}
	if (segments[digit][6]) { // 中央
		SDL_Rect rect = {x, y + segmentWidth - segmentHeight/2, segmentWidth, segmentHeight};
		SDL_RenderFillRect(mRenderer, &rect);
	}
}

// 簡易テキスト描画
// SDL_ttf による高品質テキスト描画
void Renderer::DrawSimpleText(const std::string& text, int x, int y, int scale) {
	// フォントが初期化されていない場合はフォールバック
	if (!mFont) {
		// 既存の簡易描画（数字のみ対応）
		int currentX = x;
		for (char c : text) {
			if (c >= '0' && c <= '9') {
				DrawDigit(c - '0', currentX, y, scale);
				currentX += 12 * scale;
			}
			else if (c == ' ') {
				currentX += 6 * scale;
			}
			else {
				// 文字として認識できるように簡易的な点を描画
				SDL_Rect rect = { currentX + 2 * scale, y + 5 * scale, 2 * scale, 2 * scale };
				SDL_RenderFillRect(mRenderer, &rect);
				currentX += 10 * scale;
			}
		}
		return;
	}

	// 現在の描画色を取得
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(mRenderer, &r, &g, &b, &a);
	SDL_Color color = { r, g, b, 255 };

	// テキストサーフェスを作成（UTF-8対応）
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended(mFont, text.c_str(), color);
	if (!textSurface) {
		SDL_Log("テキストのレンダリングに失敗: %s", TTF_GetError());
		return;
	}

	// サーフェスからテクスチャを作成
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	if (!textTexture) {
		SDL_Log("テクスチャの作成に失敗: %s", SDL_GetError());
		SDL_FreeSurface(textSurface);
		return;
	}

	// 描画先の矩形を設定（スケール適用）
	SDL_Rect renderQuad = {
		x,
		y,
		textSurface->w * scale,
		textSurface->h * scale
	};

	// テクスチャを描画
	SDL_RenderCopy(mRenderer, textTexture, nullptr, &renderQuad);

	// リソースの解放
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}

// センターラインの描画
void Renderer::DrawCenterLine()
{
	// センターラインを明るい白色で描画
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	
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

// ポーズメッセージの描画
void Renderer::DrawPauseMessage() {
	// 暗いオーバーレイ（半透明は使わない）
	SetRenderColor(50, 50, 50);
	SDL_Rect overlay = {0, 0, static_cast<int>(GameConstants::WINDOW_WIDTH), static_cast<int>(GameConstants::WINDOW_HEIGHT)};
	SDL_RenderFillRect(mRenderer, &overlay);
	
	// ポーズメッセージ
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	
	int centerX = static_cast<int>(GameConstants::WINDOW_WIDTH / 2);
	int centerY = static_cast<int>(GameConstants::WINDOW_HEIGHT / 2);
	
	DrawSimpleText("PAUSED", centerX - 80, centerY - 30, 3);
	DrawSimpleText("Press ESC P or SPACE to resume", centerX - 200, centerY + 30, 1);
}

// ゲームオーバーメッセージの描画
void Renderer::DrawGameOverMessage(int leftScore, int rightScore) {
	// 暗いオーバーレイ（半透明は使わない）
	SetRenderColor(40, 40, 40);
	SDL_Rect overlay = {0, 0, static_cast<int>(GameConstants::WINDOW_WIDTH), static_cast<int>(GameConstants::WINDOW_HEIGHT)};
	SDL_RenderFillRect(mRenderer, &overlay);
	
	int centerX = static_cast<int>(GameConstants::WINDOW_WIDTH / 2);
	int centerY = static_cast<int>(GameConstants::WINDOW_HEIGHT / 2);
	
	// ゲームオーバーメッセージ
	SetRenderColor(GameConstants::Colors::YELLOW.r, 
	               GameConstants::Colors::YELLOW.g, 
	               GameConstants::Colors::YELLOW.b);
	
	DrawSimpleText("GAME OVER", centerX - 150, centerY - 100, 3);
	
	// 勝者の表示
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
	
	if (leftScore > rightScore) {
		SetRenderColor(GameConstants::Colors::BLUE.r, 
		               GameConstants::Colors::BLUE.g, 
		               GameConstants::Colors::BLUE.b);
		DrawSimpleText("LEFT PLAYER WINS", centerX - 200, centerY - 10, 2);
	} else {
		SetRenderColor(GameConstants::Colors::RED.r, 
		               GameConstants::Colors::RED.g, 
		               GameConstants::Colors::RED.b);
		DrawSimpleText("RIGHT PLAYER WINS", centerX - 200, centerY - 10, 2);
	}
	
	// リスタート指示
	SetRenderColor(GameConstants::Colors::WHITE.r, 
	               GameConstants::Colors::WHITE.g, 
	               GameConstants::Colors::WHITE.b);
}

void Renderer::DrawGameOverOptions(int selectedIndex)
{
    int centerX = static_cast<int>(GameConstants::WINDOW_WIDTH / 2);
    int centerY = static_cast<int>(GameConstants::WINDOW_HEIGHT / 2);

    // オプション表示
    const char* optRestart = "Restart";
    const char* optMenu = "Back to Menu";

    // 選択中の方を強調（黄色）、非選択は白
    if (selectedIndex == 0) {
        SetRenderColor(GameConstants::Colors::YELLOW.r, GameConstants::Colors::YELLOW.g, GameConstants::Colors::YELLOW.b);
        DrawSimpleText(optRestart, centerX - 80, centerY + 40, 2);
        SetRenderColor(GameConstants::Colors::WHITE.r, GameConstants::Colors::WHITE.g, GameConstants::Colors::WHITE.b);
        DrawSimpleText(optMenu, centerX - 150, centerY + 90, 2);
    } else {
        SetRenderColor(GameConstants::Colors::WHITE.r, GameConstants::Colors::WHITE.g, GameConstants::Colors::WHITE.b);
        DrawSimpleText(optRestart, centerX - 80, centerY + 40, 2);
        SetRenderColor(GameConstants::Colors::YELLOW.r, GameConstants::Colors::YELLOW.g, GameConstants::Colors::YELLOW.b);
        DrawSimpleText(optMenu, centerX - 150, centerY + 90, 2);
    }
}

// メニュー描画
void Renderer::DrawMenu(const std::vector<std::string>& items, int selectedIndex, const std::string& title)
{
    // 背景クリアは呼び出し側で実行済みの前提
    int centerX = static_cast<int>(GameConstants::WINDOW_WIDTH / 2);
    int startY = static_cast<int>(GameConstants::WINDOW_HEIGHT / 3);

    // タイトル
    if (!title.empty()) {
        SetRenderColor(GameConstants::Colors::YELLOW.r,
                       GameConstants::Colors::YELLOW.g,
                       GameConstants::Colors::YELLOW.b);
        DrawSimpleText(title, centerX - 250, startY - 80, 2);
    }

    // メニュー項目
    for (size_t i = 0; i < items.size(); ++i) {
        bool selected = static_cast<int>(i) == selectedIndex;
        if (selected) {
            SetRenderColor(GameConstants::Colors::BLUE.r,
                           GameConstants::Colors::BLUE.g,
                           GameConstants::Colors::BLUE.b);
        } else {
            SetRenderColor(GameConstants::Colors::WHITE.r,
                           GameConstants::Colors::WHITE.g,
                           GameConstants::Colors::WHITE.b);
        }
        int y = startY + static_cast<int>(i) * 40;
        DrawSimpleText(items[i], centerX - 250, y, selected ? 2 : 2);
    }

    // 操作ヒント
    SetRenderColor(GameConstants::Colors::WHITE.r,
                   GameConstants::Colors::WHITE.g,
                   GameConstants::Colors::WHITE.b);
    DrawSimpleText("Use UP/DOWN and ENTER", centerX - 160, startY + static_cast<int>(items.size()) * 40 + 40, 1);
}