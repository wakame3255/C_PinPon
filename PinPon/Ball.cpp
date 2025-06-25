#include "Ball.h"

// コンストラクタ
Ball::Ball()
	: _position({ 0.0f, 0.0f })
	, _velocity({ 0.0f, 0.0f })
	, _radius(5.0f)
{
	// デフォルトコンストラクタ：初期値を設定
}

// 初期化処理
void Ball::Entry() {
	// ボールを画面中央に配置
	_position = { GameConstants::WINDOW_WIDTH / 2, GameConstants::WINDOW_HEIGHT / 2 };
	// 左方向への初速度を設定
	_velocity = { -GameConstants::BALL_SPEED, 0.0f };
	// ボールの半径を設定
	_radius = 5.0f;
}

// 更新処理
void Ball::Update(float deltaTime) {
	// ボールの位置を速度に基づいて更新
	_position.x += _velocity.x * deltaTime;
	_position.y += _velocity.y * deltaTime;
	
	// 上下の壁との衝突判定
	if (_position.y - _radius < GameConstants::UPPER_WALL) {
		// 上の壁に衝突した場合
		_position.y = GameConstants::UPPER_WALL + _radius;
		ReverseVelocityY();  // Y方向の速度を反転
	}
	else if (_position.y + _radius > GameConstants::UNDER_WALL) {
		// 下の壁に衝突した場合
		_position.y = GameConstants::UNDER_WALL - _radius;
		ReverseVelocityY();  // Y方向の速度を反転
	}
}

// 描画処理
void Ball::Draw(SDL_Renderer* renderer) {
	// ボールを白色で描画
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// ボールを矩形として描画（簡易的な実装）
	SDL_Rect ballRect{
		static_cast<int>(_position.x - _radius),
		static_cast<int>(_position.y - _radius),
		static_cast<int>(_radius * 2),
		static_cast<int>(_radius * 2)
	};
	SDL_RenderFillRect(renderer, &ballRect);
}

// リセット処理
void Ball::Reset() {
	// ボールを初期位置に戻す
	Entry();
}