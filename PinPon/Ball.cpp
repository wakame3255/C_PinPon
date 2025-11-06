#include "Ball.h"
#include <cstdlib>
#include <ctime>

// コンストラクタ
Ball::Ball()
	: _position(Vector2::Zero())
	, _velocity(Vector2::Zero())
	, _radius(GameConstants::BALL_RADIUS)
{
	// 乱数シードの初期化
	static bool seedInitialized = false;
	if (!seedInitialized) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		seedInitialized = true;
	}
}

// 初期化処理
void Ball::Entry() {
	// ボールを画面中央に配置
	_position = Vector2(GameConstants::WINDOW_WIDTH / 2, GameConstants::WINDOW_HEIGHT / 2);
	
	// ランダムな方向への初速度を設定
	float angle = (std::rand() % 120 - 60) * 3.14159f / 180.0f; // -60度から+60度
	float direction = (std::rand() % 2 == 0) ? -1.0f : 1.0f;    // 左右ランダム
	
	_velocity = Vector2(
		direction * GameConstants::BALL_SPEED * std::cos(angle),
		GameConstants::BALL_SPEED * std::sin(angle)
	);
	
	// ボールの半径を設定
	_radius = GameConstants::BALL_RADIUS;
}

// 更新処理
void Ball::Update(float deltaTime) {
	// ボールの位置を速度に基づいて更新
	_position += _velocity * deltaTime;
	
	// 上下の壁との衝突判定
	if (_position.y - _radius < GameConstants::UPPER_WALL) {
		// 上の壁に衝突した場合
		_position.y = GameConstants::UPPER_WALL + _radius;
		ReverseVelocityY();  // Y方向の速度を反転
		
		// 軽微な速度増加（ゲームプレイを活発にする）
		_velocity *= GameConstants::BALL_SPEED_INCREASE;
		
		// 速度制限
		float maxSpeed = GameConstants::BALL_SPEED * 2.0f;
		if (_velocity.Length() > maxSpeed) {
			_velocity = _velocity.Normalized() * maxSpeed;
		}
	}
	else if (_position.y + _radius > GameConstants::UNDER_WALL) {
		// 下の壁に衝突した場合
		_position.y = GameConstants::UNDER_WALL - _radius;
		ReverseVelocityY();  // Y方向の速度を反転
		
		// 軽微な速度増加
		_velocity *= GameConstants::BALL_SPEED_INCREASE;
		
		// 速度制限
		float maxSpeed = GameConstants::BALL_SPEED * 2.0f;
		if (_velocity.Length() > maxSpeed) {
			_velocity = _velocity.Normalized() * maxSpeed;
		}
	}
}

// リセット処理
void Ball::Reset() {
	// ボールを初期位置に戻す
	Entry();
}