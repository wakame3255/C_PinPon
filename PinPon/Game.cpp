#include "Game.h"

    const float PADDLE_SPEED = 300.0f;
    const float BALL_SPEED = 200.0f;

    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;

	const int WALL_HEIGHT = 20;
	const int WALL_SPACE = 300;

    bool Game::Initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            return false;
        }

        mWindow = SDL_CreateWindow(
            "PinPon",
            100,
            100,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0
        );

        if (!mWindow) {
            return false;
        }

        mRenderer = SDL_CreateRenderer(
            mWindow,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

        if (!mRenderer) {
            return false;
        }

        // パドルの初期位置設定
        Vector2 paddle1Pos{ 50.0f, WINDOW_HEIGHT/2.0f };
        Vector2 paddle2Pos{ WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT/2.0f };
        mPaddlesPos = { paddle1Pos, paddle2Pos };
        mPaddlesDir = { {0,0}, {0,0} };

        // ボールの初期化
        Ball ball;
        ball.pos = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
        ball.vel = { -BALL_SPEED, 0.0f };
        mBalls.push_back(ball);

        mIsRunning = true;
        return true;
    }

    void Game::ProcessInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    mIsRunning = false;
                    break;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        // 左パドル
        if (state[SDL_SCANCODE_W]) {
            mPaddlesDir[0].y = -1;
        }
        else if (state[SDL_SCANCODE_S]) {
            mPaddlesDir[0].y = 1;
        }
        else {
            mPaddlesDir[0].y = 0;
        }

        // 右パドル
        if (state[SDL_SCANCODE_UP]) {
            mPaddlesDir[1].y = -1;
        }
        else if (state[SDL_SCANCODE_DOWN]) {
            mPaddlesDir[1].y = 1;
        }
        else {
            mPaddlesDir[1].y = 0;
        }
    }

    void Game::UpdateGame() {
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

        float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
        if (deltaTime > 0.05f) {
            deltaTime = 0.05f;
        }
        mTicksCount = SDL_GetTicks();

        // すべてのパドルの更新
        for (int i = 0; i < 2; i++) {
            mPaddlesPos[i].y += mPaddlesDir[i].y * PADDLE_SPEED * deltaTime;
            //パドルの高さの最小値
            if (mPaddlesPos[i].y < 0) {
                mPaddlesPos[i].y = 0;
            }
            //パドルの高さの最大値
            else if (mPaddlesPos[i].y > 668.0f) {
                mPaddlesPos[i].y = 668.0f;
            }
        }

        // ボールの更新
        for (auto& ball : mBalls) {
            ball.pos.x += ball.vel.x * deltaTime;
            ball.pos.y += ball.vel.y * deltaTime;

            // 上壁との衝突(壁の厚さを考慮)
            if (ball.pos.y <= 0 + (WALL_SPACE + WALL_HEIGHT ))
            {
                ball.pos.y = 0 + (WALL_SPACE + WALL_HEIGHT);
                ball.vel.y *= -1;
            }
			// 下壁との衝突
            else if (ball.pos.y >= WINDOW_HEIGHT - (WALL_SPACE + WALL_HEIGHT )) 
            {
                ball.pos.y = WINDOW_HEIGHT - (WALL_SPACE + WALL_HEIGHT );
                ball.vel.y *= -1;
            }

            // パドルとの衝突
            float diff;
            // 左パドル
            if (ball.pos.x <= 60 && ball.pos.x >= 40 &&
                ball.pos.y >= mPaddlesPos[0].y && ball.pos.y <= mPaddlesPos[0].y + 100) {
                ball.pos.x = 60;
                ball.vel.x *= -1;
                diff = ball.pos.y - (mPaddlesPos[0].y + 50.0f);
                ball.vel.y = diff * 2.0f;
            }
            // 右パドル
            else if (ball.pos.x >= 964 && ball.pos.x <= 984 &&
                     ball.pos.y >= mPaddlesPos[1].y && ball.pos.y <= mPaddlesPos[1].y + 100) {
                ball.pos.x = 964;
                ball.vel.x *= -1;
                diff = ball.pos.y - (mPaddlesPos[1].y + 50.0f);
                ball.vel.y = diff * 2.0f;
            }

            // ボールが画面外に出たら初期位置に戻す
            if (ball.pos.x < 0 || ball.pos.x > 1024) {
                ball.pos = { 512.0f, 384.0f };
                ball.vel = { -BALL_SPEED, 0.0f };
            }
        }
    }

    void Game::GenerateOutput() {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mRenderer);

        // 上壁の描画
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_Rect upWall{
            0,
            WALL_SPACE,
            WINDOW_WIDTH,
            WALL_HEIGHT
        };
        SDL_RenderFillRect(mRenderer, &upWall);

		// 下壁の描画
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_Rect underWall{
            0,
            WINDOW_HEIGHT - (WALL_HEIGHT + WALL_SPACE),
            WINDOW_WIDTH,
            WALL_HEIGHT
        };
        SDL_RenderFillRect(mRenderer, &underWall);

        // パドルの描画
        for (const auto& paddlePos : mPaddlesPos) {
            SDL_Rect paddle{
                static_cast<int>(paddlePos.x - 10),
                static_cast<int>(paddlePos.y),
                20,
                100
            };
            SDL_RenderFillRect(mRenderer, &paddle);
        }

        // ボールの描画
        for (const auto& ball : mBalls) {
            SDL_Rect ballRect{
                static_cast<int>(ball.pos.x - 5),
                static_cast<int>(ball.pos.y - 5),
                10,
                10
            };
            SDL_RenderFillRect(mRenderer, &ballRect);
        }

        SDL_RenderPresent(mRenderer);
    }

    // 他のメンバ関数は変更なし
// 既存のコードの後に以下を追加

Game::Game()
    : mWindow(nullptr)
    , mRenderer(nullptr)
    , mTicksCount(0)
    , mIsRunning(true)
{
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    SDL_Quit();
}
// 既存のコードの前に以下を追加