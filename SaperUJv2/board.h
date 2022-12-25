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
#define MEDB 96
#define HARDX 26
#define HARDY 38
#define HARDB 230

void CellDraw(Cell** grid, int i, int j, int status);
void CellSetUJ(int sizeX, int sizeY, int nUJ, Cell** aTab, int startX, int startY);
void CellFillNumbers(int sizeX, int sizeY, Cell** grid);
void CellReveal(Cell** grid, int i, int j, int sizeX, int sizeY, int*status, int* revealed);
void sizeEasy(int* x, int* y, int* b);
void sizeMed(int* x, int* y, int* b);
void sizeHard(int* x, int* y, int* b);
void OpenWindow(const char* caption);
void DrawTaskbar(int bombsLeft, int time);
void DrawMenu(int menu);
//void DrawEndgameWin(int time);
//void DrawEndgameLose();
void DrawRanking(int** nRanking);
void LoadTexturesGame();
void LoadTexturesMenu();
void UnloadTexturesGame();
void UnloadTexturesMenu();