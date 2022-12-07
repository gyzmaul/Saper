#include "utility.h"
#include "board.h"
#include "raylib.h"
#include <iostream>



Cell** fnMemoryAlloc(int nSizeX, int nSizeY, Cell** aTab)
{
	aTab = (Cell**)calloc(nSizeX, sizeof(Cell*));
	if (aTab == NULL)
	{
		printf("MEMORY ERROR --code A1");
		free(aTab);
		system("pause");
		exit(-1);
	}

	for (int i = 0; i < nSizeX; i++)
	{
		*(aTab + i) = (Cell*)calloc(nSizeY, sizeof(Cell));
		if (*aTab == NULL)
		{
			printf("MEMORY ERROR --code B%d", i);
			free(aTab);
			system("pause");
			exit(-1);
		}
	}

	return aTab;
}

void fnMemoryFree(int sizeX, Cell** aTab)
{
	for (int i = 0; i < sizeX; i++) free(aTab[i]);
	free(aTab);
}

void CellIndex(Cell** grid, int sizeX, int sizeY)
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			grid[i][j].i = i;
			grid[i][j].j = j;
		}
	}
}

bool CheckIndex(int x, int y, int sizeX, int sizeY)
{
	return (x >= 0 && x < sizeX&& y >= 0 && y < sizeY);
}

int FlagCount(Cell** grid, int a, int b, int sizeX, int sizeY)
{
	int flags = 0;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (CheckIndex(a + x, b + y, sizeX, sizeY))
			{
				if (grid[a + x][b + y].isFlag == true) flags++;
			}
		}
	}

	return flags;
}
	