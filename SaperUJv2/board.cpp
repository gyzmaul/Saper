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
Texture2D MSbackground, Ranks;

Font font = { 0 };


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
	DrawTexture(MSbackground, 0, 0, WHITE);
	DrawRectangle(screenWidth / 4, 5.5 * screenHeight / 12, screenWidth / 2, screenHeight / 12, DARKGRAY);
	DrawRectangle(screenWidth / 4,   7 * screenHeight / 12, screenWidth / 2, screenHeight / 12, DARKGRAY);
	DrawRectangle(screenWidth / 4, 8.5 * screenHeight / 12, screenWidth / 2, screenHeight / 12, DARKGRAY);
	DrawRectangle(screenWidth / 4,  10 * screenHeight / 12, screenWidth / 4 - screenHeight / 48, screenHeight / 12, DARKGRAY);
	DrawRectangle(screenWidth - screenWidth / 2 + screenHeight / 48,  10 * screenHeight / 12, screenWidth / 4 - screenHeight / 48, screenHeight / 12, DARKGRAY);

	if(menu==1)DrawRectangle(screenWidth / 4 + 2, 5.5 * screenHeight / 12 + 2, screenWidth / 2 - 4, screenHeight / 12 - 4, GRAY);
	if(menu==2)DrawRectangle(screenWidth / 4 + 2,   7 * screenHeight / 12 + 2, screenWidth / 2 - 4, screenHeight / 12 - 4, GRAY);
	if(menu==3)DrawRectangle(screenWidth / 4 + 2, 8.5 * screenHeight / 12 + 2, screenWidth / 2 - 4, screenHeight / 12 - 4, GRAY);
	if(menu==4)DrawRectangle(screenWidth / 4 + 2,  10 * screenHeight / 12 + 2, screenWidth / 4 - screenHeight / 48 - 4, screenHeight / 12 - 4, GRAY);

	//DrawText(TextFormat("easy")  , screenWidth / 4 + 36, 5.5 * screenHeight / 12    , screenHeight / 12 - 4, GREEN);
	DrawTextEx(font, "easy", { (float)(screenWidth / 4 + 36), (float)(5.5 * screenHeight / 12) }, screenHeight / 12 - 4, 4, GREEN);
	DrawText(TextFormat("medium"), screenWidth / 4 + 18,   7 * screenHeight / 12 + 2, screenHeight / 12 - 4, YELLOW);
	DrawText(TextFormat("hard")  , screenWidth / 4 + 36, 8.5 * screenHeight / 12 + 5, screenHeight / 12 - 4, RED);
	DrawTexture(Ranks, screenWidth / 4, 10 * screenHeight / 12, WHITE);


	EndDrawing();
}

/*void DrawEndgameLose()
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawText(TextFormat("Przegrales"), screenWidth / 8  +10, screenHeight / 6, 2 * POLE, RED);
	DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 12, DARKBROWN);

	EndDrawing();
}

void DrawEndgameWin(int time)
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawText(TextFormat("Wygrales"), screenWidth / 4 - 2, screenHeight / 6, 2 * POLE, GREEN);
	DrawText(TextFormat("0%d:%d", (time / 60), time % 60), 3 * screenWidth / 8, screenHeight / 3, 2 * POLE, WHITE);

	DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 12, DARKGREEN);

	EndDrawing();
}*/

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
	MSbackground = LoadTexture("textures/msaghMED.png");
	Ranks = LoadTexture("textures/ranks.png");
}

void UnloadTexturesMenu()
{
	UnloadTexture(MSbackground);
	UnloadTexture(Ranks);
}

void DrawRanking(int** nRanking)
{
	BeginDrawing();

	ClearBackground(BLACK);
	DrawTexture(MSbackground, 0, 0, WHITE);
	DrawRectangle(screenWidth / 4, 10 * screenHeight / 12, screenWidth / 4 - screenHeight / 48, screenHeight / 12, DARKGRAY);
	DrawRectangle(screenWidth / 4 + 2, 10 * screenHeight / 12 + 2, screenWidth / 4 - screenHeight / 48 - 4, screenHeight / 12 - 4, GRAY);
	DrawTexture(Ranks, screenWidth / 4, 10 * screenHeight / 12, WHITE);

	DrawRectangle(3 * screenWidth / 5,     screenHeight / 12 - 4, 100, 3 * screenHeight / 12 - 12, BLACK);
	DrawRectangle(3 * screenWidth / 5, 4 * screenHeight / 12 - 4, 100, 3 * screenHeight / 12 - 12, BLACK);
	DrawRectangle(3 * screenWidth / 5, 7 * screenHeight / 12 - 4, 100, 3 * screenHeight / 12 - 12, BLACK);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			DrawText(TextFormat("%02d:%02d", nRanking[i][j] / 60, nRanking[i][j] % 60), 3 * screenWidth / 5 + 10, (i*3+j+1) * screenHeight / 12, screenHeight / 18, WHITE);
		}
	}

	DrawRectangle(screenWidth / 10,     screenHeight / 12 - 4, screenWidth / 2 - 16, screenHeight / 12, BLACK);
	DrawRectangle(screenWidth / 10, 4 * screenHeight / 12 - 4, screenWidth / 2 - 16, screenHeight / 12, BLACK);
	DrawRectangle(screenWidth / 10, 7 * screenHeight / 12 - 4, screenWidth / 2 - 16, screenHeight / 12, BLACK);

	DrawText(TextFormat("easy")  , screenWidth / 8 + 18,     screenHeight / 12 - 6, screenHeight / 12 - 4, GREEN );
	DrawText(TextFormat("medium"), screenWidth / 8 + 2 , 4 * screenHeight / 12 - 2, screenHeight / 12 - 4, YELLOW);
	DrawText(TextFormat("hard")  , screenWidth / 8 + 18, 7 * screenHeight / 12    , screenHeight / 12 - 4, RED   );

	EndDrawing();
}

void LoadFonts()
{
	//font = LoadFont("resources/fonts/alagard.png");
	//font = LoadFont("resources/fonts/pixelplay.png");
	//font = LoadFont("resources/fonts/mecha.png");
	//font = LoadFont("resources/fonts/setback.png");
	//font = LoadFont("resources/fonts/romulus.png");
	//font = LoadFont("resources/fonts/pixantiqua.fnt");
	font = LoadFontEx("upheavtt.ttf", 20, NULL, 0);
	//font = LoadFont("resources/fonts/alpha_beta.png");
	//font = LoadFont("resources/fonts/jupiter_crash.png");
}

void UnloadFonts()
{
	//for (int i = 0; i < 8; i++) 
	UnloadFont(font);
}