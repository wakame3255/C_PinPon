#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "Vector2.h"
#include "GameMode.h"

class  CPUController
{
private:
	Paddle* mControlledPaddle;
	CPUDifficulty mDifficultyLevel;

	float mReactionDelay;
	float mReactionTimer;

public:
	 CPUController(Paddle* paddle, CPUDifficulty difficulty);

	void Update(float deltaTime, const Ball& ball);

	void SetDifficulty(CPUDifficulty difficulty);

private:
	bool ShouldReact(float deltaTime);
	bool IsBallApproaching(const Ball& ball) const;
	float DeadZone() const;
	Vector2 PredictBallPosition(const Ball& ball, float timeAhead);
};
