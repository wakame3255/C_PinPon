#pragma once
#include "Vector2.h"
#include "SDL.h"

class Ball
{
	Vector2 _position;
	Vector2 _velocity;
	float _radius;

public:
	void Update(float deltaTime);
	void Draw(SDL_Renderer* renderer);
	void Reset();
};