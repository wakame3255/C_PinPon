#pragma once
#include "SDL.h"
#include <unordered_map>
#include <vector>

class InputSystem
{
private:
	// キーボードの状態を保持
	const Uint8* mKeyboardState;
	std::vector<Uint8> mPreviousKeyboardState;
	
	// ゲーム終了フラグ
	bool mShouldQuit;
	
	// キーボードのキー数
	int mNumKeys;
	
public:
	// コンストラクタ
	InputSystem();
	
	// 入力処理の更新
	void ProcessInput();
	
	// キーが押されているかチェック
	bool IsKeyPressed(SDL_Scancode key) const;
	
	// 特定のキーが今フレームで押されたかチェック
	bool IsKeyJustPressed(SDL_Scancode key) const;
	
	// 特定のキーが今フレームで離されたかチェック
	bool IsKeyJustReleased(SDL_Scancode key) const;
	
	// ゲーム終了が要求されたかチェック
	bool ShouldQuit() const { return mShouldQuit; }
	
	// 終了フラグをリセット
	void ResetQuitFlag() { mShouldQuit = false; }
	
private:
	// 前フレームのキーボード状態を保存
	void SavePreviousKeyboardState();
};