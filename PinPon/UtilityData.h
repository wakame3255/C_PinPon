#pragma once
class GameConstants
{
public:
    // ウィンドウの幅と高さ
    static constexpr float WINDOW_WIDTH = 1024.0f;
    static constexpr float WINDOW_HEIGHT = 768.0f;

    static constexpr float BALL_SPEED = 200.0f;

    //パドル情報
    static constexpr float PADDLE_SPEED = 300.0f;
	static constexpr float PADDLE_WIDTH = 20.0f;  
	static constexpr float PADDLE_HEIGHT = 50.0f; 
	static constexpr float PADDLE_SPACE = 20.0f; // パドルの間隔

	static constexpr float RIGHT_PADDLE_X = WINDOW_WIDTH - (PADDLE_SPACE + PADDLE_WIDTH);
	static constexpr float LEFT_PADDLE_X = PADDLE_SPACE + PADDLE_WIDTH;

    //壁の厚さ
    static constexpr float WALL_HEIGHT = 20.0f;    
	// 画面端と壁のスペース
    static constexpr float WALL_SPACE = 50.0f;

    //上壁の位置
    static constexpr float UPPER_WALL = WALL_SPACE + WALL_HEIGHT;
	// 下壁の位置
	static constexpr float UNDER_WALL = WINDOW_HEIGHT - (WALL_SPACE + WALL_HEIGHT);
};
