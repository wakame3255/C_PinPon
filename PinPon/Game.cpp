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
    , mLeftScore(0)
    , mRightScore(0)
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
    
    // ゲーム実行フラグを設定
    mIsRunning = true;
    
    return true;
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
    
    // パドルの入力処理
    HandlePaddleInput();
}

// パドルの入力処理
void Game::HandlePaddleInput() {
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
    
    // ゲームオブジェクトの更新
    mBall->Update(deltaTime);
    mLeftPaddle->Update(deltaTime);
    mRightPaddle->Update(deltaTime);
    
    // パドルとボールの衝突判定
    mLeftPaddle->HandleBallCollision(*mBall);
    mRightPaddle->HandleBallCollision(*mBall);
    
    // ボールのリセット判定
    CheckBallReset();
}

// ボールのリセット判定
void Game::CheckBallReset() {
    Vector2 ballPos = mBall->GetPosition();
    
    // ボールが左側の画面外に出た場合
    if (ballPos.x < 0) {
        mRightScore++;  // 右プレイヤーの得点
        mBall->Reset(); // ボールをリセット
        SDL_Log("Right player scores! Score: %d - %d", mLeftScore, mRightScore);
    }
    // ボールが右側の画面外に出た場合
    else if (ballPos.x > GameConstants::WINDOW_WIDTH) {
        mLeftScore++;   // 左プレイヤーの得点
        mBall->Reset(); // ボールをリセット
        SDL_Log("Left player scores! Score: %d - %d", mLeftScore, mRightScore);
    }
}

// 描画処理
void Game::GenerateOutput() {
    // 画面のクリア
    mRenderer->Clear();
    
    // ゲームオブジェクトの描画
    mRenderer->DrawWalls();          // 壁の描画
    mRenderer->DrawCenterLine();     // センターラインの描画
    mRenderer->DrawPaddle(*mLeftPaddle);  // 左パドルの描画
    mRenderer->DrawPaddle(*mRightPaddle); // 右パドルの描画
    mRenderer->DrawBall(*mBall);          // ボールの描画
    mRenderer->DrawScore(mLeftScore, mRightScore); // スコアの描画
    
    // 画面の更新
    mRenderer->Present();
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