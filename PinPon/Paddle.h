#pragma once
#include "Vector2.h"
#include "SDL.h"
#include "Ball.h"
#include "Renderer.h"
#include "UtilityData.h"

class Paddle
{
	float _speed;
	float _width;
	float _height;

	std::vector<Vector2> mPaddlesPos;

	std::vector<Vector2> mPaddlesDir;

public:
	void Entry();
	void Update(float deltaTime);
	bool CheckCollision(const Ball& ball);
};