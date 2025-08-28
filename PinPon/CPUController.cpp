#include "CPUController.h"
#include <cmath>

CPUController::CPUController(Paddle* paddle, CPUDifficulty difficulty)
	: mControlledPaddle(paddle)
	, mDifficultyLevel(difficulty)
	, mReactionTimer(0.0f)
{
	SetDifficulty(difficulty);
}

void CPUController::SetDifficulty(CPUDifficulty difficulty)
{
	mDifficultyLevel = difficulty;
	switch (mDifficultyLevel)
	{
	case CPUDifficulty::Easy:   mReactionDelay = 0.30f; break;
	case CPUDifficulty::Normal: mReactionDelay = 0.15f; break;
	case CPUDifficulty::Hard:   mReactionDelay = 0.05f; break;
	}
}

bool CPUController::ShouldReact(float deltaTime)
{
	mReactionTimer += deltaTime;
	if (mReactionTimer >= mReactionDelay) {
		mReactionTimer = 0.0f;
		return true;
	}
	return false;
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
Vector2 CPUController::PredictBallPosition(const Ball& ball, float timeAhead)
{
	Vector2 currentPos = ball.GetPosition();
	Vector2 velocity = ball.GetVelocity();

	return Vector2(
		currentPos.x + velocity.x * timeAhead,
		currentPos.y + velocity.y * timeAhead
	);
}