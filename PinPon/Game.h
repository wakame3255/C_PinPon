#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 pos;
	Vector2 vel;
};

class Game
{
public: 
	Game();
	//�Q�[���̏�����
	bool Initialize();
	//�Q�[���̃��C�����[�v
	void RunLoop();
	//�Q�[���̏I������
	void Shutdown();
private:
	//�Q�[���ɕK�v�Ȋ֐��Q
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDL�̃E�B���h�E
	SDL_Window* mWindow;
	//SDL��2D�����_���[
	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	//�Q�[���̏��
	bool mIsRunning;


	std::vector<Ball> mBalls;
	
	std::vector<Vector2> mPaddlesDir;

	std::vector<Vector2> mPaddlesPos;
};