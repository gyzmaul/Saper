#pragma once

typedef struct Cell
{
	int i, j;
	int bombsAround = 0;
	bool isBomb;
	bool isRevealed = false;
	bool isFlag = false;
};

Cell** fnMemoryAlloc(int nSizeX, int nSizeY, Cell** aTab);
void fnMemoryFree(int sizeX, Cell** aTab);
void CellIndex(Cell** grid, int sizeX, int sizeY);
bool CheckIndex(int x, int y, int sizeX, int sizeY);
int FlagCount(Cell** grid, int a, int b, int sizeX, int sizeY);
