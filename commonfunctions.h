#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H
#define TILE_SIZE 8

#include "raylib.h"

void DrawTetraBlock(int xPos, int yPos, int blockColor, Texture2D tetraBlocksColors, float resScaleFactor)
{
    Vector2 pos = {(float)xPos, (float)yPos};
    Rectangle sourceRect = {0, 0, TILE_SIZE, TILE_SIZE};
    sourceRect.x = TILE_SIZE * blockColor;
    Rectangle destRect = {pos.x * resScaleFactor, pos.y * resScaleFactor, TILE_SIZE * resScaleFactor, TILE_SIZE * resScaleFactor};
    Vector2 origin = {0, 0};
    DrawTexturePro(tetraBlocksColors, sourceRect, destRect,origin,0.0f,WHITE);
}

#endif