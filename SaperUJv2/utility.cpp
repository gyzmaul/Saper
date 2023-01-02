#include "utility.h"
#include "board.h"
#include "raylib.h"
#include <iostream>
#include <fstream>

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

void CheckWin(int bombs, int stillHidden, int* status)
{
	if (stillHidden == bombs) *status = 5;
}

int** fnMemoryAllocInt(int nSizeX, int nSizeY, int** aTab)
{
	aTab = (int**)calloc(nSizeX, sizeof(int*));
	if (aTab == NULL)
	{
		printf("MEMORY ERROR --code A1");
		free(aTab);
		system("pause");
		exit(-1);
	}

	for (int i = 0; i < nSizeX; i++)
	{
		*(aTab + i) = (int*)calloc(nSizeY, sizeof(int));
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

void fnMemoryFreeInt(int sizeX, int** aTab)
{
	for (int i = 0; i < sizeX; i++) free(aTab[i]);
	free(aTab);
}

void updateRanks(int time, int** nRanking, int mode)
{
	if (time != nRanking[mode - 1][0] && time != nRanking[mode - 1][1] && time != nRanking[mode - 1][2])
	{
		if (time < nRanking[mode - 1][0] || nRanking[mode - 1][0] == 0)
		{
			nRanking[mode - 1][2] = nRanking[mode - 1][1];
			nRanking[mode - 1][1] = nRanking[mode - 1][0];
			nRanking[mode - 1][0] = time;
		}

		else if (time < nRanking[mode - 1][1] || nRanking[mode - 1][1] == 0)
		{
			nRanking[mode - 1][2] = nRanking[mode - 1][1];
			nRanking[mode - 1][1] = time;
		}

		else if (time < nRanking[mode - 1][2] || nRanking[mode - 1][2] == 0)
		{
			nRanking[mode - 1][2] = time;
		}
	}
}

int countCorrect(Cell** grid, int sizeX, int sizeY)
{
	int correct = 0;
	
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (grid[x][y].isBomb == 1 && grid[x][y].isFlag == 1) correct++;
		}
	}

	return correct;
}

void windowSnap(int width, int height)
{
	Vector2 position = GetWindowPosition();

	if (position.y < 0) SetWindowPosition(position.x, 0);
	if (position.y + height + 2 * POLE + 3 * SPACE > GetMonitorHeight(0)) SetWindowPosition(position.x, GetMonitorHeight(0) - height - 46);

	if (position.x < 0) SetWindowPosition(0, position.y);
	if (position.x + width > GetMonitorWidth(0)) SetWindowPosition(GetMonitorWidth(0) - width, position.y);
}
