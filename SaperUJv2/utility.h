#pragma once

typedef struct Cell
{
	int i, j;
	int bombsAround = 0;
	bool isBomb = false;
	bool isRevealed = false;
	bool isFlag = false;
}Cell;

Cell** fnMemoryAlloc(int nSizeX, int nSizeY, Cell** aTab);
void fnMemoryFree(int sizeX, Cell** aTab);
void CellIndex(Cell** grid, int sizeX, int sizeY);
bool CheckIndex(int x, int y, int sizeX, int sizeY);
int FlagCount(Cell** grid, int a, int b, int sizeX, int sizeY);
void CheckWin(int bombs, int revealed, int* status);
int** fnMemoryAllocInt(int nSizeX, int nSizeY, int** aTab);
void fnMemoryFreeInt(int sizeX, int** aTab);
void updateRanks(int time, int** ranking, int mode);
int countCorrect(Cell** grid, int sizeX, int sizeY);
void windowSnap(int width, int height);


