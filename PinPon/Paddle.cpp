#include "Paddle.h"
#include "Ball.h"
#include <algorithm>
#include <cmath>

// コンストラクタ
Paddle::Paddle(bool isLeft)
	: _position(Vector2::Zero())
	, _direction(Vector2::Zero())
	, _speed(GameConstants::PADDLE_SPEED)
	, _width(GameConstants::PADDLE_WIDTH)
	, _height(GameConstants::PADDLE_HEIGHT)
	, _isLeftPaddle(isLeft)
{
	// パドルの初期化
}

// 初期化処理
void Paddle::Entry() {
	// パドルの初期位置を設定
	if (_isLeftPaddle) {
		// 左側のパドル
		_position.x = GameConstants::LEFT_PADDLE_X;
	}
	else {
		// 右側のパドル
		_position.x = GameConstants::RIGHT_PADDLE_X;
	}
	// Y座標は画面中央に配置
	_position.y = GameConstants::WINDOW_HEIGHT / 2.0f - _height / 2.0f;
	
	// 初期の移動方向はゼロ
	_direction = Vector2::Zero();
}

// 更新処理
void Paddle::Update(float deltaTime) {
	// パドルの位置を更新
	_position += _direction * _speed * deltaTime;
	
	// パドルが画面上端を超えないようにする
	if (_position.y < GameConstants::UPPER_WALL) {
		_position.y = GameConstants::UPPER_WALL;
	}
	// パドルが画面下端を超えないようにする
	else if (_position.y + _height > GameConstants::UNDER_WALL) {
		_position.y = GameConstants::UNDER_WALL - _height;
	}
}


// ボールとの衝突判定
bool Paddle::CheckCollision(const Ball& ball) {
	// ボールの情報を取得
	Vector2 ballPos = ball.GetPosition();
	float ballRadius = ball.GetRadius();
	
	// ボールの境界ボックスを計算
	float ballLeft = ballPos.x - ballRadius;
	float ballRight = ballPos.x + ballRadius;
	float ballTop = ballPos.y - ballRadius;
	float ballBottom = ballPos.y + ballRadius;
	
	// パドルの境界ボックス
	float paddleLeft = _position.x;
	float paddleRight = _position.x + _width;
	float paddleTop = _position.y;
	float paddleBottom = _position.y + _height;
	
	// 衝突判定（AABBアルゴリズム）
	if (ballRight >= paddleLeft && ballLeft <= paddleRight &&
		ballBottom >= paddleTop && ballTop <= paddleBottom) {
		return true;
	}
	
	return false;
}

// パドルとボールの衝突時の処理
void Paddle::HandleBallCollision(Ball& ball) {
	if (!CheckCollision(ball)) {
		return;  // 衝突していない場合は何もしない
	}
	
	// ボールの位置と速度を取得
	Vector2 ballPos = ball.GetPosition();
	Vector2 ballVel = ball.GetVelocity();
	float ballRadius = ball.GetRadius();
	
	// ボールの位置を調整（パドルに埋まらないようにする）
	if (_isLeftPaddle) {
		// 左パドルの場合：ボールを右側に押し出す
		ballPos.x = _position.x + _width + ballRadius;
	}
	else {
		// 右パドルの場合：ボールを左側に押し出す
		ballPos.x = _position.x - ballRadius;
	}
	
	// ボールの位置を更新
	ball.SetPosition(ballPos);
	
	// X方向の速度を反転
	ball.ReverseVelocityX();
	
	// パドルの中心からの距離に応じてY方向の速度を調整（改善された計算）
	float paddleCenter = _position.y + _height / 2.0f;
	float relativeIntersectY = ballPos.y - paddleCenter;
	float normalizedRelativeIntersection = relativeIntersectY / (_height / 2.0f);
	
	// 反射角度を計算（最大45度）
	float maxBounceAngle = 60.0f * 3.14159f / 180.0f; // 60度をラジアンに変換
	float bounceAngle = normalizedRelativeIntersection * maxBounceAngle;
	
	// 新しい速度を計算
	float currentSpeed = ballVel.Length();
	float newSpeedX = currentSpeed * std::cos(bounceAngle);
	float newSpeedY = currentSpeed * std::sin(bounceAngle);
	
	// 方向を考慮して速度を設定
	if (_isLeftPaddle) {
		newSpeedX = std::abs(newSpeedX);  // 右方向
	} else {
		newSpeedX = -std::abs(newSpeedX); // 左方向
	}
	
	// パドルの移動による追加の効果
	if (_direction.y != 0.0f) {
		newSpeedY += _direction.y * _speed * 0.1f; // パドルの移動速度の10%を追加
	}
	
	// 速度を適用
	ball.SetVelocity(Vector2(newSpeedX, newSpeedY));
	
	// 速度増加効果
	Vector2 finalVel = ball.GetVelocity();
	finalVel *= GameConstants::BALL_SPEED_INCREASE;
	
	// 速度制限
	float maxSpeed = GameConstants::BALL_SPEED * 2.5f;
	if (finalVel.Length() > maxSpeed) {
		finalVel = finalVel.Normalized() * maxSpeed;
	}
	
	// 最小速度保証
	float minSpeed = GameConstants::BALL_SPEED * 0.8f;
	if (finalVel.Length() < minSpeed) {
		finalVel = finalVel.Normalized() * minSpeed;
	}
	
	ball.SetVelocity(finalVel);
}