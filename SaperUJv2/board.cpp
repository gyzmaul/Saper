#include "utility.h"
#include "board.h"
#include "raylib.h"
#include <iostream>
#include <string>

using namespace std;

int sizeX = MEDX;
int sizeY = MEDY;

int menuHeight = 2 * POLE + SPACE + 2 * MARGINES;
int screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
int screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

void sizeEasy(int* x, int* y, int* b)
{
	sizeX = EASYX;
	sizeY = EASYY;

	*x = EASYX;
	*y = EASYY;
	*b = EASYB;

	menuHeight = 2 * POLE + SPACE + 2 * MARGINES;
	screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
	screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

}

void sizeMed(int* x, int* y, int* b)
{
	sizeX = MEDX;
	sizeY = MEDY;

	*x = MEDX;
	*y = MEDY;
	*b = MEDB;

	menuHeight = 2 * POLE + SPACE + 2 * MARGINES;
	screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
	screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

}

void sizeHard(int* x, int* y, int* b)
{
	sizeX = HARDX;
	sizeY = HARDY;

	*x = HARDX;
	*y = HARDY;
	*b = HARDB;

	menuHeight = 2 * POLE + SPACE + 2 * MARGINES;
	screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
	screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

}

void CellDraw(Cell** grid, int i, int j, int status)
{
	int x = MARGINES + i * (POLE + SPACE);
	int y = MARGINES + j * (POLE + SPACE) + menuHeight;

	if (grid[i][j].isBomb == 1 && status==4) DrawRectangle(x, y, POLE, POLE, RED);

	else if (grid[i][j].isRevealed == true && grid[i][j].isFlag == false)
	{
		if (grid[i][j].bombsAround == 0) DrawRectangle(x, y, POLE, POLE, BLACK);
		else if (grid[i][j].bombsAround > 0) DrawText(TextFormat("%d", grid[i][j].bombsAround), x + 6, y, POLE, WHITE);
	}
	else DrawRectangle(x, y, POLE, POLE, PINK);

	if (grid[i][j].isFlag == true)
	{
		DrawText(TextFormat("F"), x + 4, y+1, POLE, DARKGRAY);
	}
}

void CellReveal(Cell** grid, int i, int j, int sizeX, int sizeY, int *status, int* revealed)
{
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
						if (grid[i + x][j + y].isRevealed == false) CellReveal(grid, i + x, j + y, sizeX, sizeY, status, revealed);
					}
				}
			}
		}

		if(!grid[i][j].isRevealed)(*revealed)--;
		grid[i][j].isRevealed = true;

		if (grid[i][j].isBomb == 1) *status = 4;
		else if (grid[i][j].bombsAround == 0)
		{
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					if (CheckIndex(i + x, j + y, sizeX, sizeY))
					{
						if (grid[i + x][j + y].isRevealed == false) CellReveal(grid, i + x, j + y, sizeX, sizeY, status, revealed);
					}
				}
			}
		}
	}
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

void DrawTaskbar(int bombsLeft, double time)
{
	DrawRectangle(0, 0, screenWidth, menuHeight, DARKGRAY);
	//DrawRectangle(0, menuHeight - MARGINES, screenWidth, 2, DARKGRAY);
	DrawRectangle(MARGINES, MARGINES+2, 4 * POLE + 3 * SPACE, 2 * POLE, BLACK);
	DrawText(TextFormat("%d", bombsLeft), MARGINES + POLE / 2, MARGINES + 2 * SPACE +1, 2 * POLE, RED);

	DrawRectangle(screenWidth - MARGINES - (6 * POLE + 5 * SPACE), MARGINES + 2, 6 * POLE + 5 * SPACE, 2 * POLE, BLACK);
	if(((int)time%60)<10) DrawText(TextFormat("0%d:0%d", (int)(time/60), (int)((int)time%60)), screenWidth - MARGINES - (6 * POLE + 5 * SPACE) + POLE / 2, MARGINES + 2 * SPACE + 1, 2 * POLE, RED);
	else DrawText(TextFormat("0%d:%d", (int)(time/60), (int)((int)time%60)), screenWidth - MARGINES - (6 * POLE + 5 * SPACE) + POLE / 2, MARGINES + 2 * SPACE + 1, 2 * POLE, RED);
}

void OpenWindow(const char* caption)
{
	InitWindow(screenWidth, screenHeight, caption);
	SetTargetFPS(144);
}

void DrawMenu()
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawRectangle(screenWidth / 4, screenHeight / 6, screenWidth / 2, screenHeight / 6, GREEN);
	DrawRectangle(screenWidth / 4, 2.5 * screenHeight / 6, screenWidth / 2, screenHeight / 6, YELLOW);
	DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, RED);

	EndDrawing();
}

void DrawEndgameLose()
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawText(TextFormat("Przegrales"), screenWidth / 8  +10, screenHeight / 6, 2 * POLE, RED);
	DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 12, DARKBROWN);

	EndDrawing();
}

void DrawEndgameWin()
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, DARKGREEN);

	EndDrawing();
}
