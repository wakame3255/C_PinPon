#pragma once

// ゲーム全体で使用する定数を定義
class GameConstants
{
public:
    // ウィンドウサイズ
    static constexpr float WINDOW_WIDTH = 1024.0f;
    static constexpr float WINDOW_HEIGHT = 768.0f;

    // ボールの設定
    static constexpr float BALL_SPEED = 200.0f;
    static constexpr float BALL_RADIUS = 8.0f;
    static constexpr float BALL_SPEED_INCREASE = 1.05f; // 速度上昇率

    // パドルの設定
    static constexpr float PADDLE_SPEED = 300.0f;
    static constexpr float PADDLE_WIDTH = 20.0f;  
    static constexpr float PADDLE_HEIGHT = 80.0f;  // パドルを少し大きく
    static constexpr float PADDLE_SPACE = 30.0f;   // パドルの間隔

    // パドルの位置計算
    static constexpr float RIGHT_PADDLE_X = WINDOW_WIDTH - (PADDLE_SPACE + PADDLE_WIDTH);
    static constexpr float LEFT_PADDLE_X = PADDLE_SPACE;

    // 壁の設定
    static constexpr float WALL_HEIGHT = 20.0f;    
    static constexpr float WALL_SPACE = 80.0f;     // 画面端と壁のスペース

    // 壁の位置
    static constexpr float UPPER_WALL = WALL_SPACE + WALL_HEIGHT;
    static constexpr float UNDER_WALL = WINDOW_HEIGHT - (WALL_SPACE + WALL_HEIGHT);
    
    // ゲーム設定
    static constexpr int WINNING_SCORE = 10;       // 勝利スコア
    static constexpr float SCORE_RESET_DELAY = 2.0f; // スコア後の待機時間
    
    // 色の定義（RGB）
    struct Colors {
        static constexpr struct { int r, g, b; } WHITE = {255, 255, 255};
        static constexpr struct { int r, g, b; } BLACK = {0, 0, 0};
        static constexpr struct { int r, g, b; } RED = {255, 100, 100};
        static constexpr struct { int r, g, b; } BLUE = {100, 150, 255};
        static constexpr struct { int r, g, b; } GREEN = {100, 255, 150};
        static constexpr struct { int r, g, b; } YELLOW = {255, 255, 100};
    };
};
