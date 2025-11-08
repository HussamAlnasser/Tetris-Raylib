#ifndef MAINGAME_H
#define MAINGAME_H
#include "raylib.h"
#include "enums.h"
#include "globals.h"
#include "arraylist.h"
#include "animation.h"
#include "commonfunctions.h"
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 19
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224
#define TILE_SIZE 8

extern int score;
extern int numberOfLinesCleared;
extern int level;
extern int previousLevel;
extern Font mainFont;
extern Texture2D tetraBlocksColors;
extern Texture2D tetrisBackground;
extern Sound snapSound;

//Graphics

Texture2D blockExplosionTexture;
Texture2D gameoverLabel;

//Sound
Music tetrisMusic;
Music gameoverMusic;
Sound turnSound;
Sound rotateSound;
Sound clearLineSound;


Tetrimino tetrominos[7] =
{
    {T_YELLOW, 0, {{5,0}, {6,0}, {5,1}, {6,1}}}, //O
    {T_GREEN, 1, {{5,0}, {5,1}, {6,1}, {7,1}}}, //J
    {T_BLUE, 2, {{6,0}, {6,1}, {5,1}, {4,1}}}, //L
    {T_RED, 3, {{4,0}, {5,0}, {6,0}, {7,0}}}, //I
    {T_BLUE, 4, {{4,1}, {5,1}, {5,0}, {6,0}}}, //S
    {T_GREEN, 5, {{6,1}, {5,1}, {5,0}, {4,0}}}, //Z
    {T_YELLOW, 6, {{5,0}, {5,1}, {4,1}, {6,1}}}, //T
};

Tetrimino currentBlock;
Tetrimino nextBlock;

int gameover = -1;
int canRotate = 0;
int exitGame = -1;

//These three virables are used to delay the snap of the tetromino
int delaySnap = -1;
int timeToDelaySnapLimit = 30;
int timeToDelaySnap = 0;
//These are used to delay playing while clearing lines
int delayPlaying = -1;
int timeToDelayPlayingLimit = 40;
int timeToDelayPlaying = 0;
array_list linesCompleted;

long frameCountLimit = 60;
long frameCount = 1;

long frameCountShiftLimit = 10;
long frameCountShift = 0;

int currentExplosionFrame;

Vector2 gameOverCurrentLocation;
Vector2 gameOverFinalLocation;
float gameOverOneSecond = 60.0f;
float gameOverOneFrame = 1.0f/60.0f;
float gameOverspeedframe;
float gameOverTimeLimit = 720.0f;
float gameOverTime = 0.0f;


int tetrominoIndex;

unsigned char field[FIELD_HEIGHT][FIELD_WIDTH]=
{
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,'.','.','.','.','.','.','.','.','.','.',T_GRAY},
    {T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY,T_GRAY},
};

//Module Methods
static void UpdateMainGame();
static void DrawFieldLines();
static void DrawField();
static void DrawMainGame();
static void DrawCurrentBlock();
static void DrawScoreBoard();
static void DrawBackground();
static void DrawGameOver();

int checkBounderies(Tetrimino *currentBlock, int action)
{
    //To check bounderies, we need to check what we want to do
    //If action == 0, we are going down
    //If action == 1, we are going left
    //If action == 2, we are going right
    //If action == 3, we are rotating
    for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
    {
        int x = currentBlock->position[i].x;
        int y = currentBlock->position[i].y;
        switch (action){
            case 0:
                
                //If the field space below is not empty, then we return -1 to show that we cannot move down
                if(field[(y + 1)] [x] != '.')
                {
                    return -1;
                }
                break;
            case 1:
                //If the field space on the left is not empty or has a field boundary, then we return -1 to show that we cannot move left
                if(field[(y)]  [(x-1)] != '.')
                {
                    return -1;
                }
                break;
            case 2:
                //If the field space on the right is not empty or has a field boundary, then we return -1 to show that we cannot move left
                if(field[(y)] [(x+1)] != '.')
                {
                    return -1;
                }
                break;
            case 3:
                //If the field space to rotate to not empty or has a field boundary, then we return 1 to show that we cannot move left and 2 to show that we cannot move right, and -1 to show that we cannot move at all
                if(i != 1)
                {
                    int xOrigin = currentBlock->position[1].x;
                    int yOrigin = currentBlock->position[1].y;
                    if(x == xOrigin)
                    {
                        int yDiff = yOrigin - y;
                        if(field[(yOrigin)] [(xOrigin + yDiff)] != '.')
                        {
                            if((xOrigin + yDiff) < x)
                            {
                                return 1;
                            }
                            if((xOrigin + yDiff) > x)
                            {
                                return 2;
                            }
                            return -1;
                        }
                    }
                    else if(y == yOrigin)
                    {
                        int xDiff = xOrigin - x;
                        if(field[(yOrigin - xDiff)]  [xOrigin] != '.')
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        int yDiff = yOrigin - y;
                        int xDiff = xOrigin - x;
                        int slope = yDiff/xDiff;

                        //Since the Y value increases downwards in console screens, the slope is less then zero when it rises, and the slope is above zero when it descends
                        if(slope < 0)
                        {
                            if(field[(yOrigin - xDiff)] [x] != '.')
                            {
                                if((yOrigin - xDiff) < x)
                                {
                                    return 1;
                                }
                                if((yOrigin - xDiff) > x)
                                {
                                    return 2;
                                }
                                return -1;
                            }
                        }
                        else
                        {
                            if(field[(y)] [(xOrigin + yDiff)] != '.')
                            {
                                return -1;
                            }
                        }
                    }
                }
                
                break;
        }

    }
    return 0;
}
int checkBounderiesWhileRotating(Tetrimino *currentBlock)
{
    int checkRotationBounderies = checkBounderies(currentBlock, 3);
    if(checkRotationBounderies != 0)
    {
        if(checkRotationBounderies == 1)
        {
            if(checkBounderies(currentBlock,2)!= 0)
            {
                return -1;
            }
            else
            {
                //Move left and check rotation bounderies
                for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                {
                    currentBlock->position[i].x++;
                }
                if(checkBounderies(currentBlock,3)!= 0)
                {
                    //Check further right
                    if(checkBounderies(currentBlock,2)!= 0)
                    {
                        //If can't go right, move back
                        if(checkBounderies(currentBlock,3)!= 0)
                        {
                            for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                            {
                                currentBlock->position[i].x -= 1;
                            }
                            return -1;
                        }
                    }
                    else
                    {

                        //Move further right and check rotation bounderies again
                        for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                        {
                            currentBlock->position[i].x++;
                        }
                        //If still can't rotate, move back
                        if(checkBounderies(currentBlock,3)!= 0)
                        {
                            for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                            {
                                currentBlock->position[i].x -= 2;
                            }
                            return -1;
                        }

                    }
                    
                    
                }

            }
        }
        else if(checkRotationBounderies == 2)
        {
            if(checkBounderies(currentBlock,1)!= 0)
            {
                return -1;
            }
            else
            {
                //Move left and check rotation bounderies
                for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                {
                    currentBlock->position[i].x--;
                }
                if(checkBounderies(currentBlock,3)!= 0)
                {
                    //Check further left
                    if(checkBounderies(currentBlock,1)!= 0)
                    {
                        //If can't move further left, move back
                        for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                        {
                            currentBlock->position[i].x += 1;
                        }
                        return -1;
                    }
                    else
                    {
                        //Move left and check rotation bounderies
                        for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                        {
                            currentBlock->position[i].x--;
                        }
                        //If still can't rotate, move back
                        if(checkBounderies(currentBlock,3)!= 0)
                        {
                            for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
                            {
                                currentBlock->position[i].x += 2;
                            }
                            return -1;
                        }
                        
                    }
                    
                }
            }
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

void rotate(Tetrimino *currentBlock)
{
    //If we cannot rotate, we check if moving it left or right would fix the problem. If it does and we can move it left or right, we move it left or right
    if(checkBounderiesWhileRotating(currentBlock) != 0)
    {
        return;
    }
    
    //We use the second tetra block as the point of origin when rotating the rest of the blocks
    int xOrigin = currentBlock->position[1].x;
    int yOrigin = currentBlock->position[1].y;
    //We loop through all tetra blocks
    for(size_t i = 0; i < sizeof(currentBlock->position)/sizeof(currentBlock->position[0]); i++)
    {
        //Make sure NOT to rotate second tetromino
        if(i != 1)
        {
            //Get coordinates of current tetromino
            int x = currentBlock->position[i].x;
            int y = currentBlock->position[i].y;
            
            //If X coordinate is the same as the X coordinate of point of origin, then slope is zero and we move the tetromino above the tetromino of origin
            if(x == xOrigin)
            {
                int yDiff = yOrigin - y;
                currentBlock->position[i].x = xOrigin + yDiff;
                currentBlock->position[i].y = yOrigin;
            }
            //If Y coordinate is the same as the Y coordinate of point of origin, then slope is undefined and we move the tetromino next to the tetromino of origin
            else if(y == yOrigin)
            {
                int xDiff = xOrigin - x;
                currentBlock->position[i].x = xOrigin;
                currentBlock->position[i].y = yOrigin - xDiff;
            }
            //If the slope is not zero or undefined, we must define the slope of the point of origin to the current tetromino
            else
            {
                int yDiff = yOrigin - y;
                int xDiff = xOrigin - x;
                int slope = yDiff/xDiff;

                //Since the Y value increases downwards in console screens, the slope is less then zero when it rises, and the slope is above zero when it descends
                if(slope < 0)
                {
                    currentBlock->position[i].y = yOrigin - xDiff ;
                }
                else
                {
                    currentBlock->position[i].x = xOrigin + yDiff;
                }
            }
            
        }
        
    }
}

void initializeMainGame()
{

    //Load graphics
    blockExplosionTexture = LoadTexture("res/TetraExplosion.png");
    gameoverLabel = LoadTexture("res/GameOver.png");

    //Load audio
    tetrisMusic = LoadMusicStream("res/dalezy-tetris.xm");
    gameoverMusic = LoadMusicStream("res/gameover-tetris.xm");
    SetMusicVolume(tetrisMusic, 0.25f);
    SetMusicVolume(gameoverMusic, 0.5f);
    turnSound = LoadSound("res/TurnSoundTetris.ogg");
    rotateSound = LoadSound("res/TurnSoundTetris.ogg");
    clearLineSound = LoadSound("res/TetrisClearLine.ogg");
    


    gameOverspeedframe = gameOverOneFrame * 15.0f;
    gameOverFinalLocation = (Vector2) {(((float)SCREEN_WIDTH/2) - ((float)gameoverLabel.width/2) ), (((float)SCREEN_HEIGHT/2) - ((float)gameoverLabel.height/2) ) };
    gameOverCurrentLocation = (Vector2) {(((float)SCREEN_WIDTH/2) - ((float)gameoverLabel.width/2) ), (0.0f - ((float)gameoverLabel.height) ) };

    tetrominoIndex = rand() % 7;

    currentBlock = tetrominos[1];

    tetrominoIndex = rand() % 7;

    nextBlock = tetrominos[tetrominoIndex];

    array_list_create(&linesCompleted);
}

void UpdateMainGame()
{
    if(gameover < 0)
    {
        if(!IsMusicStreamPlaying(tetrisMusic))
        {
            PlayMusicStream(tetrisMusic);
        }
        UpdateMusicStream(tetrisMusic);
        int goDown = -1;

        if(frameCount % frameCountLimit == 0)
        {
            goDown = 0;
            frameCount = 1;
        }

        if(delayPlaying < 0)
        {
            if (IsKeyPressed(KEY_UP)&& canRotate == 0)
            {
                canRotate = -1;
                rotate(&currentBlock);
                if(!IsSoundPlaying(rotateSound))
                {
                    PlaySound(rotateSound);
                }
            }
            else if(!IsKeyPressed(KEY_UP) && canRotate < 0)
            {
                canRotate = 0;
            }
            //Action == 1 to go left
            if(IsKeyPressed(KEY_LEFT) &&  !IsKeyDown(KEY_RIGHT) && checkBounderies(&currentBlock,1) == 0)
            {
                for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
                {
                    currentBlock.position[i].x--;
                    
                }
                frameCountShift++;
                if(!IsSoundPlaying(turnSound))
                {
                    PlaySound(turnSound);
                }
            }
            if(IsKeyDown(KEY_LEFT) &&  !IsKeyDown(KEY_RIGHT) && checkBounderies(&currentBlock,1) == 0)
            {
                if(frameCountShift%frameCountShiftLimit == 0)
                {
                    for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
                    {
                        currentBlock.position[i].x--;
                    }
                }
                
                frameCountShift++;
                if(!IsSoundPlaying(turnSound))
                {
                    PlaySound(turnSound);
                }
            }
            if(IsKeyReleased(KEY_LEFT) &&  !IsKeyDown(KEY_RIGHT))
            {
                frameCountShift = 1;
            }
            //Action == 2 to go right
            if(!IsKeyDown(KEY_LEFT) &&  IsKeyPressed(KEY_RIGHT) && checkBounderies(&currentBlock,2) == 0)
            {
                for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
                {
                    currentBlock.position[i].x++;
                }
                frameCountShift++;
                if(!IsSoundPlaying(turnSound))
                {
                    PlaySound(turnSound);
                }
            }
            if(!IsKeyDown(KEY_LEFT) &&  IsKeyDown(KEY_RIGHT) && checkBounderies(&currentBlock,2) == 0)
            {
                if(frameCountShift%frameCountShiftLimit == 0)
                {
                    for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
                    {
                        currentBlock.position[i].x++;
                    }
                }
                
                frameCountShift++;
                if(!IsSoundPlaying(turnSound))
                {
                    PlaySound(turnSound);
                }
            }
            if(!IsKeyDown(KEY_LEFT) &&  IsKeyReleased(KEY_RIGHT))
            {
                frameCountShift = 1;
            }
            if(checkBounderies(&currentBlock,0) == 0)
            {
                if(IsKeyDown(KEY_DOWN))
                {
                    for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
                    {
                        currentBlock.position[i].y++;
                    }
                }
                else if(goDown == 0)
                {
                    for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
                    {
                        currentBlock.position[i].y++;
                    }
                }
            }
            else
            {
                //Delay fitting the piece for a last chance
                if(delaySnap < 0 && timeToDelaySnap == 0)
                {
                    delaySnap = 0;
                    timeToDelaySnap++;
                }
                else if(delaySnap == 0)
                {
                    timeToDelaySnap++;
                }
                if(timeToDelaySnap % timeToDelaySnapLimit == 0)
                {
                    for(int i = 0; i < 4; i++)
                    {
                        int x = currentBlock.position[i].x;
                        int y = currentBlock.position[i].y;
                        field[y][x] = currentBlock.color;
                    }
                    tetrominoIndex = rand() % 7;
                    currentBlock = nextBlock;
                    nextBlock = tetrominos[tetrominoIndex];

                    for(int i = 0; i < 4; i++)
                    {
                        int x = currentBlock.position[i].x;
                        int y = currentBlock.position[i].y;

                        if(field[y][x] != '.' && field[y][x] != T_GRAY)
                        {
                            gameover = 0;
                            break;
                        } 
                    }
                    if(gameover == 0)
                    {
                        return;
                    }
                    timeToDelaySnap = 0;
                    delaySnap = -1;

                     for(int y = 0; y < FIELD_HEIGHT-1; y++)
                    {
                        int numberOfTetraBlocks = 0;
                        for(int x = 1; x < FIELD_WIDTH-1; x++)
                        {
                            if( field[y][x] != '.' && field[y][x] != T_GRAY)
                            {
                                numberOfTetraBlocks++;
                            }
                        }
                        if(numberOfTetraBlocks == 10)
                        {
                            array_list_append(&linesCompleted,y);
                            for(int x = 1; x < FIELD_WIDTH-1; x++)
                            {
                                field[y][x] = '=';
                            }
                        }
                    }

                    if(linesCompleted.size > 0)
                    {
                        delayPlaying = 0;
                        timeToDelayPlaying++;
                        currentExplosionFrame = 0;
                        if(!IsSoundPlaying(clearLineSound))
                        {
                            PlaySound(clearLineSound);
                        }

                        numberOfLinesCleared += linesCompleted.size;

                        level = numberOfLinesCleared/10;

                        if(level > previousLevel)
                        {
                            frameCountLimit -= 4;
                            frameCount = 1;
                            previousLevel = level;
                        }

                        if (linesCompleted.size == 1) {
                            score += (40 * (level + 1));
                        }
                        if (linesCompleted.size == 2) {
                            score += (100 * (level + 1));
                        }
                        if (linesCompleted.size == 3) {
                            score += (300 * (level + 1));
                        }
                        if (linesCompleted.size == 4) {
                            score += (1200 * (level + 1));
                        }
                    }
                    else
                    {
                        if(!IsSoundPlaying(snapSound))
                        {
                            PlaySound(snapSound);
                        }
                    }

                }
            }
        }   
        else
        {
            if(timeToDelayPlaying%timeToDelayPlayingLimit != 0)
            {
                timeToDelayPlaying++;
                if(timeToDelayPlaying%5 == 0 && timeToDelayPlaying < 40)
                {
                    currentExplosionFrame++;
                }
            }
            else
            {
                //Clear the lines and cascade above lines
                for(int i = 0; i < linesCompleted.size; i++)
                {
                    int y = linesCompleted.content[i];

                    for(int j = y; j > 0; j--)
                    {
                        for(int x = 1; x < FIELD_WIDTH-1; x++)
                        {
                            field[j][x] = field[(j - 1)] [x];
                        }
                        
                    }
                }
                array_list_clear(&linesCompleted);
                timeToDelayPlaying = 0;
                delayPlaying = -1;
                canRotate = 0;
            }
        }
        frameCount++;
    }
    else
    {
        if(!IsMusicStreamPlaying(gameoverMusic)&& gameOverTime < gameOverTimeLimit-5)
        {
            PlayMusicStream(gameoverMusic);
        }
        if(gameOverTime < gameOverTimeLimit-5)
        {
            gameOverTime++;
            UpdateMusicStream(gameoverMusic);
        }
        
        if(gameOverCurrentLocation.y < gameOverFinalLocation.y)
        {
            
            gameOverCurrentLocation.y += gameOverspeedframe;
        }
    }
}

void DrawMainGame()
{
    BeginDrawing();
        DrawBackground();
        DrawFieldLines();
        DrawScoreBoard();
        DrawCurrentBlock();
        DrawField();
        if(gameover >= 0)
        {
            DrawGameOver();
        }
    EndDrawing();
}

void DrawBackground()
{
    Rectangle sourceRect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    Rectangle destRect = {0, 0, SCREEN_WIDTH * scaleFactor, SCREEN_HEIGHT * scaleFactor};
    Vector2 origin = {0, 0};
    DrawTexturePro(tetrisBackground, sourceRect, destRect,origin,0.0f,WHITE);
}

void DrawCurrentBlock()
{
    int startingPointX = (((SCREEN_WIDTH/TILE_SIZE) / 2) - ((FIELD_WIDTH)/2)) * TILE_SIZE;
    int startingPointY = (((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2)) * TILE_SIZE;

    for(size_t i = 0; i < sizeof(currentBlock.position)/sizeof(currentBlock.position[0]); i++)
    {
        int xPos = (currentBlock.position[i].x * TILE_SIZE) + startingPointX;
        int yPos = (currentBlock.position[i].y * TILE_SIZE) + startingPointY;

        DrawTetraBlock(xPos,yPos,currentBlock.color,tetraBlocksColors, scaleFactor);
    }
    
}

void DrawField()
{
    for(int y = 0; y < FIELD_HEIGHT; y++)
    {
        for(int x = 0; x < FIELD_WIDTH; x++)
        {
            int startingPointX = (((SCREEN_WIDTH/TILE_SIZE) / 2) - ((FIELD_WIDTH)/2)) * TILE_SIZE;
            int startingPointY = (((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2)) * TILE_SIZE;

            int xPos = startingPointX + (TILE_SIZE * x);
            int yPos = startingPointY + (TILE_SIZE * y);

            if(field[y][x] != '.' && field[y][x] != T_GRAY)
            {
                if(field[y][x] == '=')
                {
                    Animate(blockExplosionTexture, xPos,yPos, 8,currentExplosionFrame, scaleFactor);
                }
                else
                {
                    DrawTetraBlock(xPos,yPos,field[y][x],tetraBlocksColors,scaleFactor);
                }
                
            }
            else if(field[y][x] == T_GRAY || x == 0 || x == FIELD_WIDTH-1)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
        }
    }
}

void DrawFieldLines()
{
        int startingPointX = ((SCREEN_WIDTH/TILE_SIZE) / 2) - (FIELD_WIDTH/2);
        int endPointX = ((SCREEN_WIDTH/TILE_SIZE) / 2) + (FIELD_WIDTH/2);
        int startingPointY = ((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2);
        int endPointY = ((SCREEN_HEIGHT/TILE_SIZE) / 2) + (FIELD_HEIGHT/2);
        //Draw black background
        DrawRectangle((float)(TILE_SIZE * scaleFactor) * (startingPointX - 1), (float)(TILE_SIZE * scaleFactor) * (startingPointY - 1), (float)(TILE_SIZE * scaleFactor) * (FIELD_WIDTH + 2), (float)(TILE_SIZE * scaleFactor) * (FIELD_HEIGHT + 2), BLACK);

        //Draw lines
        for(int i = startingPointX; i <= endPointX; i++)
        {
            DrawLineV((Vector2){(float)(TILE_SIZE * scaleFactor) * i, (float)(TILE_SIZE * scaleFactor) * startingPointY}, (Vector2){(float)(TILE_SIZE * scaleFactor) * i, (float)(TILE_SIZE * scaleFactor) * endPointY}, LIGHTGRAY);
        }

        
        for(int i = startingPointY; i <= endPointY; i++)
        {
            DrawLineV((Vector2){ (float)(TILE_SIZE * scaleFactor) * startingPointX, (float)(TILE_SIZE * scaleFactor) * i}, (Vector2){ (float)(TILE_SIZE * scaleFactor) * endPointX, (float)(TILE_SIZE * scaleFactor) * i}, LIGHTGRAY);
        }
    
}

void DrawScoreBoard()
{
    //Draw score area
    int scoreStartingPointY = (((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2)) * TILE_SIZE;
    int scoreStartingPointX = (((SCREEN_WIDTH/TILE_SIZE) / 2) + (FIELD_WIDTH/2) + 1) * TILE_SIZE;
    DrawRectangle((float)(scaleFactor) * (scoreStartingPointX- (TILE_SIZE) ), (float)(scaleFactor) * (scoreStartingPointY- (TILE_SIZE) ), (float)(TILE_SIZE * scaleFactor) * 9, (float)(TILE_SIZE * scaleFactor) * (FIELD_HEIGHT + 2), BLACK);
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            
            int xPos = scoreStartingPointX + (TILE_SIZE * x);
            int yPos = scoreStartingPointY + (TILE_SIZE * y);
            if(x == 0 || x == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            else if(y == 0 || y == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            
        }
    }

    Vector2 scoreLabelPos = {(float)scaleFactor * (scoreStartingPointX + (TILE_SIZE)), (float)scaleFactor * (scoreStartingPointY + (TILE_SIZE * 2))};

    DrawTextEx(mainFont,TextFormat("Score\n\n%06d", score), scoreLabelPos, 8 * scaleFactor,0,WHITE);

    //Draw Next area
    int nextStartingPointY = (((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2) + 10) * (TILE_SIZE);
    int nextStartingPointX = (((SCREEN_WIDTH/TILE_SIZE) / 2) + (FIELD_WIDTH/2) + 1) * TILE_SIZE;

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            
            int xPos = nextStartingPointX + (TILE_SIZE * x);
            int yPos = nextStartingPointY + (TILE_SIZE * y);
            if(x == 0 || x == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            else if(y == 0 || y == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            
        }
    }
    Vector2 nextLabelPos = {(float)scaleFactor * (nextStartingPointX + (TILE_SIZE)), (float)scaleFactor * (nextStartingPointY + (TILE_SIZE * 2))};
    DrawTextEx(mainFont,"NEXT", nextLabelPos, 8 * scaleFactor,0,WHITE);

    //Draw next block
    for(size_t i = 0; i < sizeof(nextBlock.position)/sizeof(nextBlock.position[0]); i++)
    {
        int xPos = ((nextBlock.position[i].x - 1) * TILE_SIZE) + (nextStartingPointX);
        int yPos = ((nextBlock.position[i].y) * TILE_SIZE) + (nextStartingPointY + (4 * TILE_SIZE));

        DrawTetraBlock(xPos,yPos,nextBlock.color,tetraBlocksColors,scaleFactor);
    }

    //Draw Level area
    int levelStartingPointY = (((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2)) * TILE_SIZE;
    int levelStartingPointX = TILE_SIZE;
    
    DrawRectangle((float)(scaleFactor) * (levelStartingPointX ), (float)(scaleFactor) * (levelStartingPointY- (TILE_SIZE) ), (float)(TILE_SIZE * scaleFactor) * 9, (float)(TILE_SIZE * scaleFactor) * (FIELD_HEIGHT + 2), BLACK);

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            
            int xPos = levelStartingPointX + (TILE_SIZE * x);
            int yPos = levelStartingPointY + (TILE_SIZE * y);
            if(x == 0 || x == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            else if(y == 0 || y == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            
        }
    }

    Vector2 levelLabelPos = {(float)scaleFactor * (levelStartingPointX + (TILE_SIZE)), (float)scaleFactor * (levelStartingPointY + (TILE_SIZE * 2))};

    DrawTextEx(mainFont,TextFormat("Level\n\n%02d", level), levelLabelPos, 8 * scaleFactor,0,WHITE);

    //Draw Lines area
    int linesStartingPointY = (((SCREEN_HEIGHT/TILE_SIZE) / 2) - (FIELD_HEIGHT/2)  + 10) * TILE_SIZE;
    int linesStartingPointX = TILE_SIZE;
    

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            
            int xPos = linesStartingPointX + (TILE_SIZE * x);
            int yPos = linesStartingPointY + (TILE_SIZE * y);
            if(x == 0 || x == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            else if(y == 0 || y == 7)
            {
                DrawTetraBlock(xPos,yPos,T_GRAY,tetraBlocksColors,scaleFactor);
            }
            
        }
    }

    Vector2 linesLabelPos = {(float)scaleFactor * (linesStartingPointX + (TILE_SIZE)), (float)scaleFactor * (linesStartingPointY + (TILE_SIZE * 2))};

    DrawTextEx(mainFont,TextFormat("Lines\n\n%03d", numberOfLinesCleared), linesLabelPos, 8 * scaleFactor,0,WHITE);
}

void DrawGameOver()
{
    Rectangle sourceRect = {0,0,gameoverLabel.width, gameoverLabel.height};
    Rectangle destRect = {gameOverCurrentLocation.x * scaleFactor, gameOverCurrentLocation.y * scaleFactor, gameoverLabel.width * scaleFactor, gameoverLabel.height * scaleFactor};
    Vector2 origin = {0, 0};
    DrawTexturePro(gameoverLabel, sourceRect, destRect,origin,0.0f,WHITE);
}
#endif