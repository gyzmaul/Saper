#include "utility.h"
#include "board.h"
#include "raylib.h"
#include <iostream>
#include <string>

using namespace std;

int sizeX = MEDX;
int sizeY = MEDY;

int menuHeight = 2 * POLE + SPACE + 2 * MARGINES;
int screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
int screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

Texture2D AGHFlag, UJbomb, redX;
Texture2D menuBackground, Ranks, Cog, Biwo, rankBackground, arrow1, arrow2;

Font font = { 0 };

Rectangle backRec = { 30, 10, 68, 40 };


void sizeEasy(int* x, int* y, int* b)
{
	sizeX = EASYX;
	sizeY = EASYY;

	*x = EASYX;
	*y = EASYY;
	*b = EASYB;

	menuHeight = 2 * POLE + SPACE + 2 * MARGINES;
	screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
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
	screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
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
	screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
	screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

}

void CellDraw(Cell** grid, int i, int j, int status)
{
	int x = MARGINES + i * (POLE + SPACE);
	int y = MARGINES + j * (POLE + SPACE) + menuHeight;

	if (grid[i][j].isBomb == 1 && status == 4 && grid[i][j].isFlag == 0) DrawTexture(UJbomb, x, y, WHITE);

	else if (grid[i][j].isRevealed == true && grid[i][j].isFlag == false)
	{
		if (grid[i][j].bombsAround == 0) DrawRectangle(x, y, POLE, POLE, BLACK);
		else if (grid[i][j].bombsAround > 0) DrawText(TextFormat("%d", grid[i][j].bombsAround), x + 6, y, POLE, WHITE);
	}
	else DrawRectangle(x, y, POLE, POLE, GRAY);

	if (grid[i][j].isBomb == 1 && status == 5) DrawTexture(AGHFlag, x, y, WHITE);
	if (grid[i][j].isFlag == true)
	{
		//DrawText(TextFormat("F"), x + 4, y+1, POLE, DARKGRAY);
		DrawTexture(AGHFlag, x, y, WHITE);
	}
	if (grid[i][j].isBomb == 0 && status == 4 && grid[i][j].isFlag == 1) DrawTexture(redX, x, y, WHITE);
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

void DrawTaskbar(int bombsLeft, int time)
{
	DrawRectangle(0, 0, screenWidth, menuHeight, DARKGRAY);
	DrawRectangle(0, screenHeight - menuHeight, screenWidth, menuHeight, DARKGRAY);
	//DrawRectangle(0, menuHeight - MARGINES, screenWidth, 2, DARKGRAY);
	DrawRectangle(MARGINES, MARGINES+2, 4 * POLE + 3 * SPACE, 2 * POLE, BLACK);
	DrawText(TextFormat("%d", bombsLeft), MARGINES + POLE / 2 - 2, MARGINES + 2 * SPACE +1, 2 * POLE, RED);

	DrawRectangle(screenWidth - MARGINES - (6 * POLE + 5 * SPACE), MARGINES + 2, 6 * POLE + 5 * SPACE, 2 * POLE, BLACK);
	DrawText(TextFormat("%02d:%02d", (time/60), time%60), screenWidth - MARGINES - (6 * POLE + 5 * SPACE) + POLE / 2 + 4, MARGINES + 2 * SPACE + 1, 2 * POLE, RED);
}

void OpenWindow(const char* caption)
{
	InitWindow(screenWidth, screenHeight, caption);
	SetTargetFPS(144);
}

void DrawMenu(int menu)
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawTexture(menuBackground, 0, 0, WHITE);

	if(menu==1) DrawTextEx(font, "<", { (float)(3 * screenWidth / 4), (float)(6.2 * screenHeight / 12) }, screenHeight / 18, 4, WHITE);
	if(menu==2) DrawTextEx(font, "<", { (float)(3 * screenWidth / 4), (float)(7.2 * screenHeight / 12) }, screenHeight / 18, 4, WHITE);
	if(menu==3) DrawTextEx(font, "<", { (float)(3 * screenWidth / 4), (float)(8.2 * screenHeight / 12) }, screenHeight / 18, 4, WHITE);
	if(menu==4) DrawTextEx(font, "_", { (float)(screenWidth / 6 + 20), (float)(10 * screenHeight / 12) }, screenHeight / 18, 4, WHITE);
	if(menu==5) DrawTextEx(font, "_", { (float)(screenWidth / 2 - 12), (float)(10 * screenHeight / 12) }, screenHeight / 18, 4, WHITE);

	DrawTextEx(font, "easy",   { (float)(screenWidth / 6), (float)(6.2 * screenHeight / 12) }, screenHeight / 18, 4, GREEN);
	DrawTextEx(font, "medium", { (float)(screenWidth / 6), (float)(7.2 * screenHeight / 12) }, screenHeight / 18, 4, YELLOW);
	DrawTextEx(font, "hard"  , { (float)(screenWidth / 6), (float)(8.2 * screenHeight / 12) }, screenHeight / 18, 4, RED);
	DrawTexture(Ranks, screenWidth / 6 - 4, 9.6 * screenHeight / 12, WHITE);
	DrawTexture(Cog  , screenWidth / 2 - 36, 9.6 * screenHeight / 12 - 4, WHITE);
	DrawTexture(Biwo  , 3 * screenWidth / 4 - 30, 9.6 * screenHeight / 12 - 4, WHITE);


	EndDrawing();
}

void LoadTexturesGame()
{
	AGHFlag = LoadTexture("textures/AGH_400px.png");
	UJbomb = LoadTexture("textures/UJ_400px.png");
	redX = LoadTexture("textures/redX.png");
}

void UnloadTexturesGame()
{
	UnloadTexture(AGHFlag);
	UnloadTexture(UJbomb);
	UnloadTexture(redX);
}

void LoadTexturesMenu()
{
	menuBackground = LoadTexture("textures/msaghMED.png");
	Ranks = LoadTexture("textures/ranks.png");
	Cog = LoadTexture("textures/cog.png");
	Biwo = LoadTexture("textures/biwo.png");
	rankBackground = LoadTexture("textures/msaghMEDranking2.png");
	arrow1 = LoadTexture("textures/arrow1.png");
	arrow2 = LoadTexture("textures/arrow2.png");
}

void UnloadTexturesMenu()
{
	UnloadTexture(menuBackground);
	UnloadTexture(Ranks);
	UnloadTexture(Cog);
	UnloadTexture(Biwo);
	UnloadTexture(rankBackground);
	UnloadTexture(arrow1);
	UnloadTexture(arrow2);
}

void DrawRanking(int** nRanking)
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawTexture(rankBackground, 0, 0, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), backRec)) DrawTexture(arrow2, 30, 10, WHITE);
	else DrawTexture(arrow1, 30, 10, WHITE);

	for (int i = 0; i < 3; i++)
	{
		if (nRanking[i][0] != 0) DrawTextEx(font, TextFormat("%02d:%02d", nRanking[i][0] / 60, nRanking[i][0] % 60), { (float) 2 * screenWidth / 5 - 4 , (float) ((i + 1) * 3.6 * screenHeight / 12 - 56)}, screenHeight / 24, 0, WHITE);
		if (nRanking[i][1] != 0) DrawTextEx(font, TextFormat("%02d:%02d", nRanking[i][1] / 60, nRanking[i][1] % 60), { (float) 1 * screenWidth / 5 - 24, (float) ((i + 1) * 3.6 * screenHeight / 12 - 24)}, screenHeight / 24, 0, WHITE);
		if (nRanking[i][2] != 0) DrawTextEx(font, TextFormat("%02d:%02d", nRanking[i][2] / 60, nRanking[i][2] % 60), { (float) 3 * screenWidth / 5 + 16, (float) ((i + 1) * 3.6 * screenHeight / 12 )}, screenHeight / 24, 0, WHITE);
	}

	EndDrawing();
}

void LoadFonts()
{
	font = LoadFontEx("Azonix.otf", 100, NULL, 0);
}

void UnloadFonts()
{
	UnloadFont(font);
}

void DrawSettings()
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawTexture(menuBackground, 0, 0, WHITE);

	EndDrawing();
}