#include "Paddle.h"

void Paddle::Update(float deltaTime) {

    // すべてのパドルの更新
    for (int i = 0; i < 2; i++) {
        mPaddlesPos[i].y += mPaddlesDir[i].y * GameConstants::PADDLE_SPEED * deltaTime;
        //パドルの高さの最小値
        if (mPaddlesPos[i].y > GameConstants::UNDER_WALL - GameConstants::PADDLE_HEIGHT) {
            mPaddlesPos[i].y = GameConstants::UNDER_WALL - GameConstants::PADDLE_HEIGHT;
        }
        //パドルの高さの最大値
        else if (mPaddlesPos[i].y < GameConstants::UPPER_WALL) {
            mPaddlesPos[i].y = GameConstants::UPPER_WALL;
        }
    }
}

void Paddle::Entry() {
    // パドルの初期位置設定
    Vector2 paddle1Pos{ 50.0f, GameConstants::WINDOW_HEIGHT / 2.0f };
    Vector2 paddle2Pos{ GameConstants::WINDOW_WIDTH - 50.0f, GameConstants::WINDOW_HEIGHT / 2.0f };
    mPaddlesPos = { paddle1Pos, paddle2Pos };
    mPaddlesDir = { {0,0}, {0,0} };
}