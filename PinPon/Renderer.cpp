#include "Renderer.h"

 void Renderer::DrawPaddles(const std::vector<SDL_Rect>& paddles)
{
     // ƒpƒhƒ‹‚Ì•`‰æ
     for (const auto& paddlePos : paddles) {
         SDL_Rect paddle{
             static_cast<int>(paddlePos.x - 10),
             static_cast<int>(paddlePos.y),
             GameConstants::PADDLE_WIDTH,
             GameConstants::PADDLE_HEIGHT
         };
         SDL_RenderFillRect(mRenderer, &paddle);
     }
}