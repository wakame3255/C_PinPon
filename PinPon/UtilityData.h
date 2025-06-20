#pragma once
class GameConstants
{
public:
    // �E�B���h�E�̕��ƍ���
    static constexpr float WINDOW_WIDTH = 1024.0f;
    static constexpr float WINDOW_HEIGHT = 768.0f;

    static constexpr float BALL_SPEED = 200.0f;

    //�p�h�����
    static constexpr float PADDLE_SPEED = 300.0f;
	static constexpr float PADDLE_WIDTH = 20.0f;  
	static constexpr float PADDLE_HEIGHT = 50.0f; 
	static constexpr float PADDLE_SPACE = 20.0f; // �p�h���̊Ԋu

	static constexpr float RIGHT_PADDLE_X = WINDOW_WIDTH - (PADDLE_SPACE + PADDLE_WIDTH);
	static constexpr float LEFT_PADDLE_X = PADDLE_SPACE + PADDLE_WIDTH;

    //�ǂ̌���
    static constexpr float WALL_HEIGHT = 20.0f;    
	// ��ʒ[�ƕǂ̃X�y�[�X
    static constexpr float WALL_SPACE = 50.0f;

    //��ǂ̈ʒu
    static constexpr float UPPER_WALL = WALL_SPACE + WALL_HEIGHT;
	// ���ǂ̈ʒu
	static constexpr float UNDER_WALL = WINDOW_HEIGHT - (WALL_SPACE + WALL_HEIGHT);
};
