#pragma once
#include "Ball.h"
#include "Paddle.h"

class CollisionChecker
{
public:
	static bool CheckWallCollision(const Ball& ball);
	static bool CheckPaddleCollision(const Ball& ball, const Paddle& paddle);
};