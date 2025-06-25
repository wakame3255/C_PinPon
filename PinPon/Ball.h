#pragma once
#include "Vector2.h"
#include "SDL.h"
#include "UtilityData.h"

class Ball
{
private:
	Vector2 _position;  // ボールの現在位置
	Vector2 _velocity;  // ボールの速度ベクトル
	float _radius;      // ボールの半径

public:
	// コンストラクタ
	Ball();
	
	// 初期化処理（ゲーム開始時に呼ばれる）
	void Entry();
	
	// 更新処理（毎フレーム呼ばれる）
	void Update(float deltaTime);
	
	// 描画処理
	void Draw(SDL_Renderer* renderer);
	
	// リセット処理（得点時などに呼ばれる）
	void Reset();
	
	// ゲッター
	const Vector2& GetPosition() const { return _position; }
	const Vector2& GetVelocity() const { return _velocity; }
	float GetRadius() const { return _radius; }
	
	// セッター
	void SetPosition(const Vector2& pos) { _position = pos; }
	void SetVelocity(const Vector2& vel) { _velocity = vel; }
	
	// 速度の反転処理
	void ReverseVelocityX() { _velocity.x = -_velocity.x; }
	void ReverseVelocityY() { _velocity.y = -_velocity.y; }
};