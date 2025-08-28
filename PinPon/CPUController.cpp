#include "CPUController.h"
#include <cmath>

CPUController::CPUController(Paddle* paddle, CPUDifficulty difficulty)
	: mControlledPaddle(paddle)
	, mDifficultyLevel(difficulty)
	, mReactionTimer(0.0f)
{
	SetDifficulty(difficulty);
}

void CPUController::Update(float deltatime, const Ball& ball) 
{
	if (!ShouldReact(deltatime))return;

	TrackBall(ball);
}

void CPUController::TrackBall(const Ball& ball)
{
	Vector2 ballPos = ball.GetPosition();
	Vector2 paddlePos = mControlledPaddle->GetPosition();

	float paddleCenterY = paddlePos.y + mControlledPaddle->GetHeight() / 2.0f;
	float targetY = ballPos.y - mControlledPaddle->GetHeight() / 2.0f;

	if (targetY > paddleCenterY + 10.0f)
	{
		mControlledPaddle->SetDirectionY(1.0f);
	}
	else if (targetY < paddleCenterY - 10.0f)
	{
		mControlledPaddle->SetDirectionY(-1.0f);
	}
	else
	{
		mControlledPaddle->SetDirectionY(0.0f);
	}
}					