#include "Game.h"
#include "Ball.h"
#include "Paddle.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "UtilityData.h"

// コンストラクタ
Game::Game()
    : mRenderer(nullptr)
    , mInputSystem(nullptr)
    , mBall(nullptr)
    , mLeftPaddle(nullptr)
    , mRightPaddle(nullptr)
    , mIsRunning(true)
    , mTicksCount(0)
    , mGameState(GameState::Playing)
    , mLeftScore(0)
    , mRightScore(0)
    , mScoreResetTimer(0.0f)
    , mIsWaitingForReset(false)
{
    // メンバー変数の初期化
}

// デストラクタ
Game::~Game()
{
    // スマートポインタが自動的にメモリを解放
}

// ゲームの初期化
bool Game::Initialize() {
    // レンダラーの作成と初期化
    mRenderer = std::make_unique<Renderer>();
    if (!mRenderer->Initialize()) {
        SDL_Log("Failed to initialize renderer");
        return false;
    }
    
    // 入力システムの作成
    mInputSystem = std::make_unique<InputSystem>();
    
    // ボールの作成と初期化
    mBall = std::make_unique<Ball>();
    mBall->Entry();
    
    // パドルの作成と初期化
    mLeftPaddle = std::make_unique<Paddle>(true);   // 左パドル
    mLeftPaddle->Entry();
    
    mRightPaddle = std::make_unique<Paddle>(false); // 右パドル
    mRightPaddle->Entry();
    
    // ゲーム状態を初期化
    mGameState = GameState::Playing;
    mIsRunning = true;
    
    return true;
}

void Game::InitiaizeGameMode(GameMode mode, CPUDifficulty difficulty) 
{
	mGameMode = mode;
	mCPUDifficulty = difficulty;

    if (mIsCPUMode)
    {
		mCPUController = std::make_unique<CPUController>(mRightPaddle.get(), difficulty);
    }
}

// ゲームのメインループ
void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

// 入力処理
void Game::ProcessInput() {
    // 入力システムの更新
    mInputSystem->ProcessInput();
    
    // 終了判定
    if (mInputSystem->ShouldQuit()) {
        mIsRunning = false;
        return;
    }
    
    // ゲーム状態に応じた入力処理
    switch (mGameState) {
        case GameState::Playing:
            ProcessGameInput();
            break;
        case GameState::Paused:
            ProcessPauseInput();
            break;
        case GameState::GameOver:
            ProcessGameOverInput();
            break;
        default:
            break;
    }
}

void Game::ProcessMenuInput()
{
    if (mInputSystem->IsKeyJustPressed(SDL_SCANCODE_UP)) {
        mSelectedMenuIndex = (mSelectedMenuIndex - 1 + 3) % 3; // 3つのオプション
    }
    else if (mInputSystem->IsKeyJustPressed(SDL_SCANCODE_DOWN)) {
        mSelectedMenuIndex = (mSelectedMenuIndex + 1) % 3;
    }
    else if (mInputSystem->IsKeyJustPressed(SDL_SCANCODE_RETURN)) {
        switch (mSelectedMenuIndex) {
        case 0: // Player vs Player
            StartGame(GameMode::PlayerVsPlayer);
            break;
        case 1: // Player vs CPU (Easy)
            StartGame(GameMode::PlayerVsCPU, CPUDifficulty::Easy);
            break;
        case 2: // Player vs CPU (Normal)
            StartGame(GameMode::PlayerVsCPU, CPUDifficulty::Normal);
            break;
        }
    }
}

// ゲーム中の入力処理
void Game::ProcessGameInput() {
    // ポーズキー（ESCまたはP）- JustPressedを使用
    if (mInputSystem->IsKeyJustPressed(SDL_SCANCODE_ESCAPE) || 
        mInputSystem->IsKeyJustPressed(SDL_SCANCODE_P)) {
        SetGameState(GameState::Paused);
        return;
    }
    
    // パドルの入力処理
    HandlePaddleInput();
}

// ポーズ中の入力処理
void Game::ProcessPauseInput() {
    // ポーズ解除（ESC、P、またはSPACE）- JustPressedを使用
    if (mInputSystem->IsKeyJustPressed(SDL_SCANCODE_ESCAPE) ||
        mInputSystem->IsKeyJustPressed(SDL_SCANCODE_P) ||
        mInputSystem->IsKeyJustPressed(SDL_SCANCODE_SPACE)) {
        SetGameState(GameState::Playing);
    }
}

// ゲームオーバー時の入力処理
void Game::ProcessGameOverInput() {
    // リスタート（SPACE、ENTER、またはR）- JustPressedを使用
    if (mInputSystem->IsKeyJustPressed(SDL_SCANCODE_SPACE) ||
        mInputSystem->IsKeyJustPressed(SDL_SCANCODE_RETURN) ||
        mInputSystem->IsKeyJustPressed(SDL_SCANCODE_R)) {
        ResetGame();
    }
}

// パドルの入力処理
void Game::HandlePaddleInput() {
    // ゲーム中でない場合は入力を無視
    if (mGameState != GameState::Playing) {
        return;
    }
    
    // 左パドルの操作（W/Sキー）
    if (mInputSystem->IsKeyPressed(SDL_SCANCODE_W)) {
        mLeftPaddle->SetDirectionY(-1.0f);  // 上に移動
    }
    else if (mInputSystem->IsKeyPressed(SDL_SCANCODE_S)) {
        mLeftPaddle->SetDirectionY(1.0f);   // 下に移動
    }
    else {
        mLeftPaddle->SetDirectionY(0.0f);   // 停止
    }

	// CPUモードの場合、右パドルをCPUに制御させる
	if (mIsCPUMode) {
		mCPUController->Update(mTicksCount / 1000.0f, *mBall);
		return;
	}
    
    // 右パドルの操作（上下矢印キー）
    if (mInputSystem->IsKeyPressed(SDL_SCANCODE_UP)) {
        mRightPaddle->SetDirectionY(-1.0f); // 上に移動
    }
    else if (mInputSystem->IsKeyPressed(SDL_SCANCODE_DOWN)) {
        mRightPaddle->SetDirectionY(1.0f);  // 下に移動
    }
    else {
        mRightPaddle->SetDirectionY(0.0f);  // 停止
    }
}

// ゲーム更新処理
void Game::UpdateGame() {
    // フレームレート制限（60FPS）
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    
    // デルタタイムの計算
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    
    // デルタタイムの上限設定（スローダウン対策）
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    
    mTicksCount = SDL_GetTicks();
    
    // ゲーム状態に応じた更新処理
    switch (mGameState) {
        case GameState::Playing:
            UpdateGameplay(deltaTime);
            break;
        case GameState::Paused:
            UpdatePause(deltaTime);
            break;
        case GameState::GameOver:
            UpdateGameOver(deltaTime);
            break;
        default:
            break;
    }
}

// ゲームプレイの更新
void Game::UpdateGameplay(float deltaTime) {
    // スコア後の待機処理
    if (mIsWaitingForReset) {
        mScoreResetTimer -= deltaTime;
        if (mScoreResetTimer <= 0.0f) {
            mIsWaitingForReset = false;
            mBall->Reset();
        }
        return;
    }
    
    // ゲームオブジェクトの更新
    mBall->Update(deltaTime);
    mLeftPaddle->Update(deltaTime);
    mRightPaddle->Update(deltaTime);
    
    // パドルとボールの衝突判定
    mLeftPaddle->HandleBallCollision(*mBall);
    mRightPaddle->HandleBallCollision(*mBall);
    
    // ボールのリセット判定
    CheckBallReset();
    
    // 勝利判定
    CheckWinCondition();
}

// ポーズ状態の更新
void Game::UpdatePause(float deltaTime) {
    // ポーズ中は何も更新しない
}

// ゲームオーバー状態の更新
void Game::UpdateGameOver(float deltaTime) {
    // ゲームオーバー中は何も更新しない
}

// ボールのリセット判定
void Game::CheckBallReset() {
    Vector2 ballPos = mBall->GetPosition();
    
    // ボールが左側の画面外に出た場合
    if (ballPos.x < 0) {
        mRightScore++;  // 右プレイヤーの得点
        mScoreResetTimer = GameConstants::SCORE_RESET_DELAY;
        mIsWaitingForReset = true;
        SDL_Log("Right player scores! Score: %d - %d", mLeftScore, mRightScore);
    }
    // ボールが右側の画面外に出た場合
    else if (ballPos.x > GameConstants::WINDOW_WIDTH) {
        mLeftScore++;   // 左プレイヤーの得点
        mScoreResetTimer = GameConstants::SCORE_RESET_DELAY;
        mIsWaitingForReset = true;
        SDL_Log("Left player scores! Score: %d - %d", mLeftScore, mRightScore);
    }
}

// 勝利判定
void Game::CheckWinCondition() {
    if (mLeftScore >= GameConstants::WINNING_SCORE || 
        mRightScore >= GameConstants::WINNING_SCORE) {
        SetGameState(GameState::GameOver);
    }
}

// ゲーム状態の変更
void Game::SetGameState(GameState newState) {
    mGameState = newState;
    
    switch (newState) {
        case GameState::Playing:
            SDL_Log("Game resumed");
            break;
        case GameState::Paused:
            SDL_Log("Game paused");
            break;
        case GameState::GameOver:
            SDL_Log("Game over! Final score: %d - %d", mLeftScore, mRightScore);
            if (mLeftScore > mRightScore) {
                SDL_Log("Left player wins!");
            } else {
                SDL_Log("Right player wins!");
            }
            break;
        default:
            break;
    }
}

// ゲームのリセット
void Game::ResetGame() {
    mLeftScore = 0;
    mRightScore = 0;
    mScoreResetTimer = 0.0f;
    mIsWaitingForReset = false;
    
    // ゲームオブジェクトをリセット
    mBall->Reset();
    mLeftPaddle->Entry();
    mRightPaddle->Entry();
    
    // ゲーム状態をプレイ中に変更
    SetGameState(GameState::Playing);
    
    SDL_Log("Game reset!");
}

// 描画処理
void Game::GenerateOutput() {
    // 画面のクリア
    mRenderer->Clear();
    
    // ゲーム状態に応じた描画
    switch (mGameState) {
        case GameState::Playing:
            RenderGame();
            break;
        case GameState::Paused:
            RenderPause();
            break;
        case GameState::GameOver:
            RenderGameOver();
            break;
        default:
            break;
    }
    
    // 画面の更新
    mRenderer->Present();
}

// ゲーム画面の描画
void Game::RenderGame() {
    // ゲームオブジェクトの描画
    mRenderer->DrawWalls();          // 壁の描画
    mRenderer->DrawCenterLine();     // センターラインの描画
    mRenderer->DrawPaddle(*mLeftPaddle);  // 左パドルの描画
    mRenderer->DrawPaddle(*mRightPaddle); // 右パドルの描画
    
    // ボールの描画（待機中でなければ）
    if (!mIsWaitingForReset) {
        mRenderer->DrawBall(*mBall);
    }
    
    mRenderer->DrawScore(mLeftScore, mRightScore); // スコアの描画
}

// ポーズ画面の描画
void Game::RenderPause() {
    // ゲーム画面をそのまま描画
    RenderGame();
    
    // ポーズメッセージの描画
    mRenderer->DrawPauseMessage();
}

// ゲームオーバー画面の描画
void Game::RenderGameOver() {
    // ゲーム画面をそのまま描画
    RenderGame();
    
    // ゲームオーバーメッセージの描画
    mRenderer->DrawGameOverMessage(mLeftScore, mRightScore);
}

// ゲームの終了処理
void Game::Shutdown()
{
    // レンダラーの終了処理
    if (mRenderer) {
        mRenderer->Shutdown();
    }
    
    // SDLの終了
    SDL_Quit();
}