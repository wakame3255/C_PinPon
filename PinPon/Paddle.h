#pragma once
#include "Vector2.h"
#include "SDL.h"
#include "Ball.h"
#include "Renderer.h"
#include "UtilityData.h"

class Paddle
{
private:
	Vector2 _position;     // パドルの位置（左上座標）
	Vector2 _direction;    // パドルの移動方向（-1, 0, 1）
	float _speed;          // パドルの移動速度
	float _width;          // パドルの幅
	float _height;         // パドルの高さ
	bool _isLeftPaddle;    // 左側のパドルかどうか

public:
	// コンストラクタ
	Paddle(bool isLeft);
	
	// 初期化処理
	void Entry();
	
	// 更新処理
	void Update(float deltaTime);
	
	// 描画処理
	void Draw(SDL_Renderer* renderer);
	
	// ボールとの衝突判定
	bool CheckCollision(const Ball& ball);
	
	// パドルとボールの衝突時の処理
	void HandleBallCollision(Ball& ball);
	
	// ゲッター
	const Vector2& GetPosition() const { return _position; }
	const Vector2& GetDirection() const { return _direction; }
	float GetWidth() const { return _width; }
	float GetHeight() const { return _height; }
	bool IsLeftPaddle() const { return _isLeftPaddle; }
	
	// セッター
	void SetDirection(const Vector2& dir) { _direction = dir; }
	void SetDirectionY(float y) { _direction.y = y; }
};