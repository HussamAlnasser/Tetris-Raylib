#ifndef GLOBALS_H
#define GLOBALS_H
#include "raylib.h"
#include "enums.h"

typedef struct _Tetrimino
{
    int color;
    int type;
    Vector2 position[4];
}Tetrimino;

int score = 0;
int numberOfLinesCleared = 0;
int level = 0;
int previousLevel = 0;
float scaleFactor = 4;
Font mainFont;
Texture2D tetraBlocksColors;
Texture2D tetrisBackground;

Sound snapSound;

int currentGameMode = TITLE_PAGE;
int nextGameMode = TITLE_PAGE;

#endif