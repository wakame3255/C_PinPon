#pragma once
#include <memory>
#include <vector>
#include "SDL.h"

// 前方宣言
class Ball;
class Paddle;
class Renderer;
class InputSystem;

class Game
{
private:
	// ゲームコンポーネント
	std::unique_ptr<Renderer> mRenderer;        // レンダラー
	std::unique_ptr<InputSystem> mInputSystem;  // 入力システム
	
	// ゲームオブジェクト
	std::unique_ptr<Ball> mBall;                // ボール
	std::unique_ptr<Paddle> mLeftPaddle;        // 左パドル
	std::unique_ptr<Paddle> mRightPaddle;       // 右パドル
	
	// ゲーム状態
	bool mIsRunning;      // ゲームが実行中かどうか
	Uint32 mTicksCount;   // 前フレームからの経過時間
	
	// スコア
	int mLeftScore;       // 左プレイヤーのスコア
	int mRightScore;      // 右プレイヤーのスコア
	
	// プライベートメソッド
	void ProcessInput();   // 入力処理
	void UpdateGame();     // ゲーム更新処理
	void GenerateOutput(); // 描画処理
	
	// ボールのリセット判定
	void CheckBallReset();
	
	// パドルの入力処理
	void HandlePaddleInput();
	
public:
	// コンストラクタ・デストラクタ
	Game();
	~Game();
	
	// ゲームの初期化
	bool Initialize();
	
	// ゲームのメインループ
	void RunLoop();
	
	// ゲームの終了処理
	void Shutdown();
};