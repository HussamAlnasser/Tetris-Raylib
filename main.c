#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "maingame.h"
#include "title.h"
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 19
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224



extern float scaleFactor;



extern int score;
extern int numberOfLinesCleared;
extern int level;
extern int previousLevel;
extern int currentGameMode;
extern int nextGameMode;

Image tetrisIcon;
static void chooseInitialization();
static void chooseUpdate();
static void chooseDraw();
static void unloadTexture();

int main()
{
    srand(time(0));
    tetrisIcon = LoadImage("res/TetrisIcon.png");
    InitWindow(SCREEN_WIDTH* scaleFactor,SCREEN_HEIGHT* scaleFactor,"Tetris");
    SetWindowIcon(tetrisIcon);
    InitAudioDevice();
    SetTargetFPS(60);

    InitializeTitlePage();
    // ToggleFullscreen();
    // ToggleBorderlessWindowed();
    while(!WindowShouldClose())
    {
        if(currentGameMode != nextGameMode)
        {
            chooseInitialization();
        }
        ClearBackground(BLANK);
        chooseUpdate();
        chooseDraw();
    }
    CloseAudioDevice();
    CloseWindow();
}

void chooseInitialization()
{
    switch(nextGameMode)
    {
        case TITLE_PAGE:
            InitializeTitlePage();
            break;
        case MAIN_GAME_PAGE:
            initializeMainGame();
            break;
    }
    currentGameMode = nextGameMode;
}

void chooseUpdate()
{
    switch(currentGameMode)
    {
        case TITLE_PAGE:
            UpdateTitle();
            break;
        case MAIN_GAME_PAGE:
            UpdateMainGame();;
            break;
    }
}

void chooseDraw()
{
    switch(currentGameMode)
    {
        case TITLE_PAGE:
            DrawTitle();
            break;
        case MAIN_GAME_PAGE:
            DrawMainGame();
            break;
    }
}

void unloadTexture()
{
    UnloadTexture(tetraBlocksColors);
    UnloadTexture(tetrisBackground);
}

