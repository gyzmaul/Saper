#pragma 
#include "raylib.h"

#define POLE 20
#define SPACE 2
#define MARGINES 2

#define EASYX 10
#define EASYY 16
#define EASYB 10
#define MEDX 16
#define MEDY 24
#define MEDB 56
#define HARDX 26
#define HARDY 38
#define HARDB 230

#define MONITORHEIGHT 1080
#define MONITORWIDTH 1920

void CellDraw(Cell** grid, int status);
void CellSetBombs(int sizeX, int sizeY, int nUJ, Cell** aTab, int startX, int startY);
void CellFillNumbers(int sizeX, int sizeY, Cell** grid);
void CellReveal(Cell** grid, int i, int j, int sizeX, int sizeY, int*status, int* revealed);
void CellShuffle(int sizeX, int sizeY, int nUJ, Cell** grid, int correct);

void sizeEasy(int* x, int* y, int* b);
void sizeMed(int* x, int* y, int* b);
void sizeHard(int* x, int* y, int* b);

void OpenWindow(const char* caption, int x, int y);
void CloseWin();
void DrawTaskbar(int bombsLeft, int time);
void DrawBottomBar(int musicIsPlaying, int highlight);
void DrawMenu(int menu);
void DrawRanking(int** nRanking);
void DrawSettings(int cellColor);

void LoadTexturesGame();
void LoadTexturesMenu();
void LoadFonts();
void UnloadTexturesGame();
void UnloadTexturesMenu();
void UnloadFonts();

void changeColor(int* cellColor);
void setColors(int cellColor);
void PlayAudio(Music* OST, const char* filename);
