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
#define HARDB 310

void CellDraw(Cell** grid, int i, int j, int status, Texture2D AGHFlag, Texture2D UJbomb);
void CellSetUJ(int sizeX, int sizeY, int nUJ, Cell** aTab, int startX, int startY);
void CellFillNumbers(int sizeX, int sizeY, Cell** grid);
void CellReveal(Cell** grid, int i, int j, int sizeX, int sizeY, int*status, int* revealed);
void DrawTaskbar(int bombsLeft, int time);
void sizeEasy(int* x, int* y, int* b);
void sizeMed(int* x, int* y, int* b);
void sizeHard(int* x, int* y, int* b);
void OpenWindow(const char* caption);
void DrawMenu(Texture2D, int menu);
void DrawEndgameWin(int time);
void DrawEndgameLose();