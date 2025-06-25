#pragma once
#include "SDL.h"
#include <unordered_map>

class InputSystem
{
private:
	// キーボードの状態を保持
	const Uint8* mKeyboardState;
	
	// ゲーム終了フラグ
	bool mShouldQuit;
	
public:
	// コンストラクタ
	InputSystem();
	
	// 入力処理の更新
	void ProcessInput();
	
	// キーが押されているかチェック
	bool IsKeyPressed(SDL_Scancode key) const;
	
	// 特定のキーが今フレームで押されたかチェック（未実装）
	bool IsKeyJustPressed(SDL_Scancode key) const;
	
	// ゲーム終了が要求されたかチェック
	bool ShouldQuit() const { return mShouldQuit; }
	
	// 終了フラグをリセット
	void ResetQuitFlag() { mShouldQuit = false; }
};