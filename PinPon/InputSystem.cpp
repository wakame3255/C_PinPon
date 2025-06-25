#include "InputSystem.h"

// コンストラクタ
InputSystem::InputSystem()
	: mKeyboardState(nullptr)
	, mShouldQuit(false)
{
	// キーボード状態の初期取得
	mKeyboardState = SDL_GetKeyboardState(nullptr);
}

// 入力処理の更新
void InputSystem::ProcessInput() {
	// SDLイベントの処理
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			// ウィンドウの閉じるボタンが押された
			mShouldQuit = true;
			break;
			
		case SDL_KEYDOWN:
			// キーが押された際の処理
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				// ESCキーで終了
				mShouldQuit = true;
			}
			break;
		}
	}
	
	// キーボード状態を更新
	// SDL_PumpEvents(); // SDL_PollEventで自動的に呼ばれるため不要
	mKeyboardState = SDL_GetKeyboardState(nullptr);
}

// キーが押されているかチェック
bool InputSystem::IsKeyPressed(SDL_Scancode key) const {
	if (mKeyboardState) {
		return mKeyboardState[key] != 0;
	}
	return false;
}

// 特定のキーが今フレームで押されたかチェック（簡易実装）
bool InputSystem::IsKeyJustPressed(SDL_Scancode key) const {
	// TODO: 前フレームの状態と比較する実装が必要
	// 現在は単純に押されているかどうかを返す
	return IsKeyPressed(key);
}