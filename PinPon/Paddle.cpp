#include "Paddle.h"

void Paddle::Update(float deltaTime) {

    // ���ׂẴp�h���̍X�V
    for (int i = 0; i < 2; i++) {
        mPaddlesPos[i].y += mPaddlesDir[i].y * GameConstants::PADDLE_SPEED * deltaTime;
        //�p�h���̍����̍ŏ��l
        if (mPaddlesPos[i].y > GameConstants::UNDER_WALL - GameConstants::PADDLE_HEIGHT) {
            mPaddlesPos[i].y = GameConstants::UNDER_WALL - GameConstants::PADDLE_HEIGHT;
        }
        //�p�h���̍����̍ő�l
        else if (mPaddlesPos[i].y < GameConstants::UPPER_WALL) {
            mPaddlesPos[i].y = GameConstants::UPPER_WALL;
        }
    }
}

void Paddle::Entry() {
    // �p�h���̏����ʒu�ݒ�
    Vector2 paddle1Pos{ 50.0f, GameConstants::WINDOW_HEIGHT / 2.0f };
    Vector2 paddle2Pos{ GameConstants::WINDOW_WIDTH - 50.0f, GameConstants::WINDOW_HEIGHT / 2.0f };
    mPaddlesPos = { paddle1Pos, paddle2Pos };
    mPaddlesDir = { {0,0}, {0,0} };
}