#pragma once
#include "Vector2.h"
#include "SDL.h"
#include "Ball.h"

class Paddle
{
	Vector2 _position;
	Vector2 _direction;
	float _speed;
	float _width;
	float _height;

public:
	void Update(float deltaTime);
	void Draw(SDL_Renderer* renderer);
	bool CheckCollision(const Ball& ball);
};