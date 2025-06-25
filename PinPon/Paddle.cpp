#include "Paddle.h"

// コンストラクタ
Paddle::Paddle(bool isLeft)
	: _position({ 0.0f, 0.0f })
	, _direction({ 0.0f, 0.0f })
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
		_position.x = GameConstants::RIGHT_PADDLE_X - _width;
	}
	// Y座標は画面中央に配置
	_position.y = GameConstants::WINDOW_HEIGHT / 2.0f - _height / 2.0f;
	
	// 初期の移動方向はゼロ
	_direction = { 0.0f, 0.0f };
}

// 更新処理
void Paddle::Update(float deltaTime) {
	// パドルの位置を更新
	_position.y += _direction.y * _speed * deltaTime;
	
	// パドルが画面上端を超えないようにする
	if (_position.y < GameConstants::UPPER_WALL) {
		_position.y = GameConstants::UPPER_WALL;
	}
	// パドルが画面下端を超えないようにする
	else if (_position.y + _height > GameConstants::UNDER_WALL) {
		_position.y = GameConstants::UNDER_WALL - _height;
	}
}

// 描画処理
void Paddle::Draw(SDL_Renderer* renderer) {
	// パドルを白色で描画
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// パドルの矩形を作成
	SDL_Rect paddleRect{
		static_cast<int>(_position.x),
		static_cast<int>(_position.y),
		static_cast<int>(_width),
		static_cast<int>(_height)
	};
	
	// パドルを描画
	SDL_RenderFillRect(renderer, &paddleRect);
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
	
	// パドルの中心からの距離に応じてY方向の速度を調整
	float paddleCenter = _position.y + _height / 2.0f;
	float diff = ballPos.y - paddleCenter;
	
	// 差分に応じてY方向の速度を設定（スピン効果）
	ballVel = ball.GetVelocity();
	ballVel.y = diff * 2.0f;  // 差分に比例した速度を設定
	ball.SetVelocity(ballVel);
}