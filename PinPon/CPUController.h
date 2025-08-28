#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "vector2.h"
#include "GameMode.h"

class  CPUController
{
private:
	Paddle* mControlledPaddle;
	CPUDifficulty mDifficultyLevel;
	
	float mReactionDelay; // 反応時間（秒）
	float mReactionTimer; // 反応タイマー（秒）
	float mPredictionAccuracy; // 予測精度（0.0 - 1.0）

	enum class CPUState
	{
		Idle,
		Tracking,
		Predicting,
		Returning
	};
	CPUState mCurrentState;

public:
	 CPUController(Paddle* paddle, CPUDifficulty difficulty);

	void Update(float deltaTime, const Ball& ball);

	void SetDifficulty(CPUDifficulty difficulty);

private:
	Vector2 PredictBallPosition(const Ball& ball, float timeAhead);

	bool ShouldReact(float deltaTime);

	void TrackBall(const Ball& ball);
};
