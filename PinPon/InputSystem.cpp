#include "InputSystem.h"

// コンストラクタ
InputSystem::InputSystem()
	: mKeyboardState(nullptr)
	, mShouldQuit(false)
	, mNumKeys(0)
{
	// キーボード状態の初期取得
	mKeyboardState = SDL_GetKeyboardState(&mNumKeys);
	
	// 前フレームの状態を初期化
	mPreviousKeyboardState.resize(mNumKeys);
	if (mKeyboardState) {
		for (int i = 0; i < mNumKeys; ++i) {
			mPreviousKeyboardState[i] = mKeyboardState[i];
		}
	}
}

// 入力処理の更新
void InputSystem::ProcessInput() {
	// 前フレームの状態を保存
	SavePreviousKeyboardState();
	
	// SDLイベントの処理
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			// ウィンドウの閉じるボタンが押された
			mShouldQuit = true;
			break;
			
		default:
			// その他のイベントは無視
			break;
		}
	}
	
	// キーボード状態を更新
	mKeyboardState = SDL_GetKeyboardState(nullptr);
}

// 前フレームのキーボード状態を保存
void InputSystem::SavePreviousKeyboardState() {
	if (mKeyboardState && mNumKeys > 0) {
		for (int i = 0; i < mNumKeys; ++i) {
			mPreviousKeyboardState[i] = mKeyboardState[i];
		}
	}
}

// キーが押されているかチェック
bool InputSystem::IsKeyPressed(SDL_Scancode key) const {
	if (mKeyboardState && key < mNumKeys) {
		return mKeyboardState[key] != 0;
	}
	return false;
}

// 特定のキーが今フレームで押されたかチェック
bool InputSystem::IsKeyJustPressed(SDL_Scancode key) const {
	if (mKeyboardState && key < mNumKeys && key < mPreviousKeyboardState.size()) {
		return (mKeyboardState[key] != 0) && (mPreviousKeyboardState[key] == 0);
	}
	return false;
}

// 特定のキーが今フレームで離されたかチェック
bool InputSystem::IsKeyJustReleased(SDL_Scancode key) const {
	if (mKeyboardState && key < mNumKeys && key < mPreviousKeyboardState.size()) {
		return (mKeyboardState[key] == 0) && (mPreviousKeyboardState[key] != 0);
	}
	return false;
}