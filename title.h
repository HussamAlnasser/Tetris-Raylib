#ifndef TITLE_H
#define TITLE_H
#include "raylib.h"
#include "enums.h"
#include "math.h"
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224
#define RECT_WIDTH 240
#define RECT_HEIGHT 208

extern Font mainFont;
extern Texture2D tetraBlocksColors;
extern Texture2D tetrisBackground;
extern Sound snapSound;

int rectExpansionAnimFrames;
int rectExpansionAnimFramesLimit;

int currentTitleStep;

int titleBlocks[5][20]=
{
    {T_YELLOW,T_YELLOW,T_YELLOW,4,T_BLUE,T_BLUE,4,T_YELLOW,T_YELLOW,T_YELLOW,4,T_GREEN,T_GREEN,4,4,T_RED,4,T_BLUE,T_BLUE,T_BLUE},
    {4,T_YELLOW,4,4,T_BLUE,4,4,4,T_YELLOW,4,4,T_GREEN,4,T_GREEN,4,T_RED,4,T_BLUE,4,4},
    {4,T_YELLOW,4,4,T_BLUE,T_BLUE,4,4,T_YELLOW,4,4,T_GREEN,T_GREEN,4,4,T_RED,4,T_BLUE,T_BLUE,T_BLUE},
    {4,T_YELLOW,4,4,T_BLUE,4,4,4,T_YELLOW,4,4,T_GREEN,4,T_GREEN,4,T_RED,4,4,4,T_BLUE},
    {4,T_YELLOW,4,4,T_BLUE,T_BLUE,4,4,T_YELLOW,4,4,T_GREEN,4,T_GREEN,4,T_RED,4,T_BLUE,T_BLUE,T_BLUE}
};

Vector2 titleBlocksYLocation[5][20];

Vector2 blocksLocationSpeed;
Vector2 previousTitleBlocksLocation;
int countBlock = 0;
Vector2 finalTitleBlocksLocation;
int numberOfTitleBlocksReachedLocation = 0;

int showPressStart;
int flashFrames;

float frameTime;

Music titleMusic;

static void DrawTitleBackground();
static void DrawBlackRectangle();
static void DrawTitleBlocks();
static void WritePressStart();

void InitializeTitlePage()
{

    tetraBlocksColors = LoadTexture("res/TetraBlocks.png");
    tetrisBackground = LoadTexture("res/TetrisBackground.png");

    SetTextureFilter(tetrisBackground, TEXTURE_FILTER_POINT);
    SetTextureFilter(tetraBlocksColors, TEXTURE_FILTER_POINT);

    mainFont = LoadFont("res/MegamaxJonathanToo.ttf");
    snapSound = LoadSound("res/TetrisSnap.ogg");
    rectExpansionAnimFrames = 0;
    rectExpansionAnimFramesLimit = 60;
    
    currentTitleStep = BLACK_RECT;

    blocksLocationSpeed = (Vector2) {(float)0,1};
    previousTitleBlocksLocation = (Vector2) {(float)(6*TILE_SIZE),(float) (-5*TILE_SIZE)};
    finalTitleBlocksLocation = (Vector2) {(float)(6*TILE_SIZE),(float) (3*TILE_SIZE)};
    frameTime = 0;
    showPressStart = 0;
    flashFrames = 0;
    for(int y = 4; y >= 0; y--)
    {
        for(int x = 0; x < 20; x++)
        {
            titleBlocksYLocation[y][x] = previousTitleBlocksLocation;
        }
    }
    titleMusic = LoadMusicStream("res/tetris_b_theme.xm");
    SetMusicVolume(titleMusic,0.2);
}

void UpdateTitle()
{
    //Drawing the black rectangle is the first step
    if(currentTitleStep == BLACK_RECT)
    {
        if((rectExpansionAnimFrames == 0 || rectExpansionAnimFrames%rectExpansionAnimFramesLimit != 0) && (rectExpansionAnimFrames < rectExpansionAnimFramesLimit))
        {
            rectExpansionAnimFrames++;
        }
        else
        {
            currentTitleStep = TITLE;
        }
    }
    
    //Next, we animate the title
    else if(currentTitleStep == TITLE)
    {
        frameTime += GetFrameTime();
        double remainder = round(fmod((double)frameTime,0.2) * 10);
        if( remainder == 0)
        {
            countBlock++;
        }
        for(int y = 4; y >= 0; y--)
        {
            for(int x = 0; x < 20; x++)
            {
                if(countBlock >= ((4-y) + x))
                {
                    if(titleBlocksYLocation[y][x].y < finalTitleBlocksLocation.y)
                    {
                        titleBlocksYLocation[y][x].y += blocksLocationSpeed.y;
                        if(titleBlocksYLocation[y][x].y == finalTitleBlocksLocation.y)
                        {
                                PlaySound(snapSound);
                                numberOfTitleBlocksReachedLocation++;
                        }
                    }
                     
                }
            }
        }
        if(numberOfTitleBlocksReachedLocation == 100)
        {
            currentTitleStep = PRESS_START;
        }
    }

    else if(currentTitleStep == PRESS_START)
    {
        flashFrames++;
        if(flashFrames%30 == 0)
        {
            if(showPressStart == 0)
            {
                showPressStart = 1;
            }
            else
            {
                showPressStart = 0;
            }
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            nextGameMode = MAIN_GAME_PAGE;
        }
        if(!IsMusicStreamPlaying(titleMusic))
        {
            PlayMusicStream(titleMusic);
        }
        UpdateMusicStream(titleMusic);
    }
}

void DrawTitle()
{
    BeginDrawing();
        DrawTitleBackground();
        DrawBlackRectangle();
        if(currentTitleStep > BLACK_RECT)
        {
            DrawTitleBlocks();
            if(currentTitleStep > TITLE)
            {
                WritePressStart();
            }
        }
    EndDrawing();
}

void DrawTitleBackground()
{
    Rectangle sourceRect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    Rectangle destRect = {0, 0, SCREEN_WIDTH * scaleFactor, SCREEN_HEIGHT * scaleFactor};
    Vector2 origin = {0, 0};
    DrawTexturePro(tetrisBackground, sourceRect, destRect,origin,0.0f,WHITE);
}

void DrawBlackRectangle()
{
    int startingPointX = SCREEN_WIDTH/2;
    int startingPointY = SCREEN_HEIGHT/2;
    float scaleRatio = (float)rectExpansionAnimFrames/rectExpansionAnimFramesLimit;
    DrawRectangle((float)(startingPointX * scaleFactor) - (scaleRatio * ((RECT_WIDTH/2) * scaleFactor)), (float)(startingPointY * scaleFactor) - (scaleRatio * ((RECT_HEIGHT/2) * scaleFactor)), (float)((RECT_WIDTH) * scaleRatio * scaleFactor), (float)((RECT_HEIGHT) * scaleRatio * scaleFactor), BLACK);
}

void DrawTitleBlocks()
{
    
    for(int y = 4; y >= 0; y--)
    {
        for(int x = 0; x < 20; x++)
        {
            float xPoint = titleBlocksYLocation[y][x].x + (TILE_SIZE * x);
            float yPoint = titleBlocksYLocation[y][x].y + (TILE_SIZE * y);
            
            
            
            if(titleBlocks[y][x] < 4)
            {
                DrawTetraBlock(xPoint,yPoint,titleBlocks[y][x],tetraBlocksColors,scaleFactor);
            }
            
        }
    }
}

void WritePressStart()
{
    if(showPressStart == 1)
    {
        Vector2 textPos = (Vector2) {(float)(6*TILE_SIZE) * scaleFactor,(float) (SCREEN_HEIGHT-5*TILE_SIZE) * scaleFactor};
        DrawTextEx(mainFont,"PRESS SPACE BAR TO START", textPos, 8 * scaleFactor,0,WHITE);
    }
    
}

#endif