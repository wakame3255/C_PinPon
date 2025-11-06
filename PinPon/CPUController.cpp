#include "CPUController.h"
#include <cmath>
#include <algorithm>

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

bool CPUController::IsBallApproaching(const Ball& ball) const
{
	float vx = ball.GetVelocity().x;
	return (mControlledPaddle->IsLeftPaddle()) ? (vx < 0.0f) : (vx > 0.0f);
}

float CPUController::DeadZone() const
{
	switch (mDifficultyLevel)
	{
	case CPUDifficulty::Easy:   return 18.0f;
	case CPUDifficulty::Normal: return 12.0f;
	case CPUDifficulty::Hard:   return 6.0f;
	}
	return 12.0f;
}

void CPUController::Update(float deltaTime, const Ball& ball) 
{
	if (!ShouldReact(deltaTime)) return;

	if (!IsBallApproaching(ball)) {
		mControlledPaddle->SetDirectionY(0.0f);
		return;
	}

	Vector2 predictedPos = ball.GetPosition();
	if (mDifficultyLevel == CPUDifficulty::Hard) {
		Vector2 vel = ball.GetVelocity();
		float paddleX = mControlledPaddle->GetPosition().x + (mControlledPaddle->IsLeftPaddle() ? mControlledPaddle->GetWidth() : 0.0f);
		float dx = std::fabs(paddleX - predictedPos.x);
		float speedX = std::max(10.0f, std::fabs(vel.x));
		float timeAhead = std::min(0.5f, dx / speedX);
		predictedPos = PredictBallPosition(ball, timeAhead);
	}

	float paddleCenterY = mControlledPaddle->GetPosition().y + mControlledPaddle->GetHeight() / 2.0f;
	float targetY = predictedPos.y;
	float dz = DeadZone();

	if (targetY > paddleCenterY + dz)
	{
		mControlledPaddle->SetDirectionY(1.0f);
	}
	else if (targetY < paddleCenterY - dz)
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