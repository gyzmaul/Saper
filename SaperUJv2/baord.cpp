#include "utility.h"
#include "board.h"
#include "raylib.h"
#include <iostream>




void CellDraw(Cell** grid, int i, int j)
{
	int x = MARGINES + i * (POLE + SPACE);
	int y = MARGINES + j * (POLE + SPACE) + menuHeight;


	if (grid[i][j].isRevealed == true && grid[i][j].isFlag == false)
	{
		if (grid[i][j].isBomb == 1) DrawRectangle(x, y, POLE, POLE, RED);
		else if (grid[i][j].bombsAround == 0) DrawRectangle(x, y, POLE, POLE, BLACK);
		else if (grid[i][j].bombsAround > 0) DrawText(TextFormat("%d", grid[i][j].bombsAround), x + 6, y, POLE, WHITE);
	}
	else DrawRectangle(x, y, POLE, POLE, PINK);

	if (grid[i][j].isFlag == true)
	{
		DrawText(TextFormat("F"), x + 6, y, POLE, DARKGRAY);
	}
}

int CellReveal(Cell** grid, int i, int j, int sizeX, int sizeY)
{
	int status = 1;
	if (grid[i][j].isFlag == 0)
	{
		if (grid[i][j].isRevealed == true && grid[i][j].bombsAround == FlagCount(grid, i, j, sizeX, sizeY))
		{
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					if (CheckIndex(i + x, j + y, sizeX, sizeY))
					{
						if (grid[i + x][j + y].isRevealed == false) status = CellReveal(grid, i + x, j + y, sizeX, sizeY);
					}
				}
			}
		}

		grid[i][j].isRevealed = true;

		if (grid[i][j].isBomb == 1) status = 0;
		if (grid[i][j].bombsAround == 0)
		{
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					if (CheckIndex(i + x, j + y, sizeX, sizeY))
					{
						if (grid[i + x][j + y].isRevealed == false) CellReveal(grid, i + x, j + y, sizeX, sizeY);
					}
				}
			}
		}
	}
	return status;
}

void CellFillNumbers(int sizeX, int sizeY, Cell** grid)
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			if (grid[i][j].isBomb == 0)
			{
				for (int x = -1; x <= 1; x++)
				{
					for (int y = -1; y <= 1; y++)
					{
						if (x == 0 && y == 0) continue;
						if (CheckIndex(i + x, j + y, sizeX, sizeY))
						{
							if (grid[i + x][j + y].isBomb) grid[i][j].bombsAround++;
						}
					}
				}
			}
		}
	}
}

void CellSetUJ(int sizeX, int sizeY, int nUJ, Cell** grid, int startX, int startY)
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			grid[i][j].isBomb = false;
			grid[i][j].bombsAround = 0;
		}
	}

	for (int i = 0; i < nUJ; i++)
	{
		int bombX, bombY;
		bombX = rand() % sizeX;
		bombY = rand() % sizeY;

		if (grid[bombX][bombY].isBomb == 1) i--;
		grid[bombX][bombY].isBomb = 1;
	}
}