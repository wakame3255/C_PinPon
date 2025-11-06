#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "GameMode.h"
#include "CPUController.h"

// 前方宣言
class Ball;
class Paddle;
class Renderer;
class InputSystem;

// ゲーム状態の定義
enum class GameState {
    Playing,    // ゲーム中
    Paused,     // ポーズ中
    GameOver,   // ゲームオーバー
    Menu        // メニュー画面
};

enum class MenuState
{
	MainMenu,
	GameModeSelection,
	InGame
};

class Game
{
private:
	// ゲームコンポーネント
	std::unique_ptr<Renderer> mRenderer;        // レンダラー
	std::unique_ptr<InputSystem> mInputSystem;  // 入力システム
	std::unique_ptr<CPUController> mLeftCPUController;  // 左パドル用CPU
	std::unique_ptr<CPUController> mRightCPUController; // 右パドル用CPU
	
	// ゲームオブジェクト
	std::unique_ptr<Ball> mBall;                // ボール
	std::unique_ptr<Paddle> mLeftPaddle;        // 左パドル
	std::unique_ptr<Paddle> mRightPaddle;       // 右パドル
	
	// ゲーム状態
	bool mIsRunning;      // ゲームが実行中かどうか
	Uint32 mTicksCount;   // 前フレームからの経過時間
	float mLastDeltaTime; // 直近フレームのデルタタイム
	GameState mGameState; // 現在のゲーム状態

	//CPU関連変数
	GameMode mGameMode;   // ゲームモード（シングルプレイヤー or マルチプレイヤー
	CPUDifficulty mCPUDifficulty; // CPUの難易度

	// メニュー関連
	MenuState mMenuState; // メニュー状態
	int mSelectedMenuIndex; // メニューで選択されている項目のインデックス
	int mGameOverSelectedIndex; // ゲームオーバー時の選択（0:リスタート 1:メニューに戻る）
	
	// スコア
	int mLeftScore;       // 左プレイヤーのスコア
	int mRightScore;      // 右プレイヤーのスコア
	
	// タイマー関連
	float mScoreResetTimer;    // スコア後の待機タイマー
	bool mIsWaitingForReset;   // リセット待機中かどうか
	
	// プライベートメソッド
	void ProcessInput();   // 入力処理
	void UpdateGame();     // ゲーム更新処理
	void GenerateOutput(); // 描画処理
	
	// ゲーム状態別の処理
	void ProcessGameInput();   // ゲーム中の入力処理
	void ProcessPauseInput();  // ポーズ中の入力処理
	void ProcessGameOverInput(); // ゲームオーバー時の入力処理

	//CPU関連
	void InitiaizeGameMode(GameMode mode, CPUDifficulty difficulty); // ゲームモードの初期化
	
	void UpdateGameplay(float deltaTime);  // ゲームプレイの更新
	void UpdatePause(float deltaTime);     // ポーズ状態の更新
	void UpdateGameOver(float deltaTime);  // ゲームオーバー状態の更新
	
	void RenderGame();      // ゲーム画面の描画
	void RenderPause();     // ポーズ画面の描画
	void RenderGameOver();  // ゲームオーバー画面の描画
	void RenderMenu();      // メニュー画面の描画

	//メニューメソッド
	void ProcessMenuInput(); // メニュー画面の入力処理
	void StartGame(GameMode mode, CPUDifficulty dificulty = CPUDifficulty::Normal);
	
	// ボールのリセット判定
	void CheckBallReset();
	
	// パドルの入力処理
	void HandlePaddleInput();
	
	// ゲーム状態の変更
	void SetGameState(GameState newState);
	
	// ゲームのリセット
	void ResetGame();
	
	// 勝利判定
	void CheckWinCondition();

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