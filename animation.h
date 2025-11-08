#include "raylib.h"

void Animate(Texture2D texture, float posX, float posY, int cropDivision,int currentFrame, float scaleFactor)
{
    int textHeight = texture.height;
    int textWidth = texture.width;
    Vector2 pos = {(float)posX, (float)posY};
    Rectangle sourceRect = {0, 0, (float)textWidth/cropDivision, (float)textHeight};
    sourceRect.x = (textWidth/cropDivision) * currentFrame;
    Rectangle destRect = {pos.x * scaleFactor, pos.y * scaleFactor, ((float)textWidth/cropDivision) * scaleFactor, ((float)textHeight) * scaleFactor};
    Vector2 origin = {0, 0};
    DrawTexturePro(texture, sourceRect, destRect,origin,0.0f,WHITE);
}